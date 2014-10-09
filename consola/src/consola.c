/**
 *
 * consola.c
 *
 *
 * Created on: 25/9/2014
 * 	   Author: lfishkel
 *
 */

//==========================================//
//******************************************//
// Dependencies								//	
//******************************************//
//==========================================//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <src/fereTypes.h>
#include <src/fereSockets.h>
#include <src/fereStream.h>
#include <src/commons/config.h>
#include <src/commons/string.h>
#include <src/commons/bitarray.h>

//==========================================//
//******************************************//
// Constants								//	
//******************************************//
//==========================================//
#define KERNEL_PORT "KERNEL_PORT"
#define KERNEL_IP "KERNEL_IP"
#define PARAM_LENGTH 2
#define ESO_CONFIG "ESO_CONFIG"
#define MAX_INPUT_STR_LEN 256
//==========================================//
//******************************************//
// Global Variables							//	
//******************************************//
//==========================================//
SocketClient* kernelClient = NULL;

StrConKer* sck = NULL;
StrKerCon* skc = NULL;

String kernelIp = NULL;
Int16U kernelPort = 0;

//==========================================//
//******************************************//
// Prototypes								//	
//******************************************//
//==========================================//
Boolean socketConnection(); 
Boolean loadConfig();
Boolean parameterValidation(int);
Boolean getAndSendBESO(String);
Boolean sendStream();
Boolean instructionsFromKernel(); 
Boolean handleStdInput(); 
Boolean handleStdOutput(); 
//==========================================//
//******************************************//
// Main Function							//	
//******************************************//
//==========================================//
int main(int argc, char *argv[]) {
	
	//Inicializo la consola verificando el parametro cargando la configuracion externa
	// y conectandome por socket al kernel si algo de esto falla se termina la consola
	if (parameterValidation(argc) && loadConfig() && socketConnection()) {
		// obtengo el codigo BESO del archivo cuyo path llega por parametro y lo envio al kernel
		if (!getAndSendBESO(argv[0])) {
			return FALSE;
		}
		// quedo a la espera de instrucciones del kernel eternamente hasta que se termine el programa
		while(TRUE) {
			if(!instructionsFromKernel()) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

//==========================================//
//******************************************//
// Prototypes implementation				//	
//******************************************//
//==========================================//

/**
 * Chequea que la cantidad de parametros a la consola sea correcta
 * En este caso solo se necesita un parametro (el path al archivo con el codigo BESO) 
 */
Boolean parameterValidation(int argc) {
	if (argc == 1) {
		return TRUE;
	}
	printf("La cantidad de parametros es incorrecta: %d\n", argc);
	return FALSE;
}

/**
 *	Se intenta conectar a la msp y kernel 
 *	En caso de no poder conectarse devuelve FALSE
 *	
 *	Funciones de sockets.h utilizadas:
 *	=================================
 *	SocketClient* socketCreateClient();
 *	Boolean socketConnect(SocketClient *ptrSocketClient, String ptrServerAddress, Int32U serverPort);
 */
Boolean socketConnection() {

	//Creo socket cliente para kernel y me conecto con la ip y el puerto
	kernelClient = socketCreateClient();
	if (!socketConnect(kernelClient, kernelIp, kernelPort)) {
		printf("No se pudo conectar al kernel en la direccion %s puerto %d \n", kernelIp, kernelPort);
		return FALSE;
	}
	return TRUE;
}

/**
 * Carga las variables de configuracion externa
 */
Boolean loadConfig() {
	String configFilePath = getenv(ESO_CONFIG);

	if (configFilePath == NULL) {
		printf("ERROR: no se pudo leer la variable de entorno '%s'.\n", ESO_CONFIG);
		return FALSE;
	}
	
	//Gennero tabla de configuracion
	t_config* tConfig = config_create(configFilePath);
	if (tConfig == NULL){
		printf("ERROR: No se encuentra o falta el archivo de configuracion en la direccion '%s'.\n", configFilePath);
		return FALSE;
	}
	//Verifico consistencia, tiene que haber 2 campos
	if (config_keys_amount(tConfig) == PARAM_LENGTH) {

		//Verifico que los parametros tengan sus valores OK-> PORT=12456 | NOK->PORT=
		if (config_has_property(tConfig, KERNEL_PORT)) {
			kernelPort = config_get_int_value(tConfig, KERNEL_PORT);
		} else{
			printf("ERROR: Falta un parametro.\n");
			return FALSE;
		}

		if (config_has_property(tConfig, KERNEL_IP)) {
			kernelIp = config_get_string_value(tConfig, KERNEL_IP);
		} else {
			printf("ERROR: Falta un parametro.\n");
			return FALSE;
		}

		//Libero tabla config
		config_destroy(tConfig);
		printf("Archivo de config MSP leído\n===========\n");
		printf("Kernel Port: %d\nKernel IP: %s\n", kernelPort, kernelIp);

		return TRUE;

	} else {
		printf("El archivo config.txt no tiene todos los campos.\n");
		return FALSE;
	}
}

/**
 * Lee el archivo de la direccion path
 * Si no lo encuentra devuelve FALSE
 * Instancia sck y le setea el fileContent
 * Se lo manda al kernel
 */
Boolean getAndSendBESO(String path) {

	//si no hay path, devuelvo FALSE
	if (path == NULL || string_is_empty(path)) {
		printf("La direccion del archivo BESO es vacia. ---- Terminando \n");
		return FALSE;
	} 

	// Abro el archivo referenciado por path para rb = leerlo binario 
	FILE* file = fopen(path, "rb");
	// si no se pudo abrir el archivo indicado, devuelvo FALSE
	if (!file) {
		printf("No se encontro el archivo %s. ---- Terminando \n", path);
		return FALSE;
	}

	Byte* read = malloc(sizeof(Byte));
	Byte* ptrBuffer;
	Stream buffer = NULL;
	Int32U size = 0;
	Int32U readed;

	while(!feof(file)){
		readed = fread(read, sizeof(Byte), sizeof(Byte), file);
		// voy copiando y resizeando el buffer
		if(readed){
			buffer = realloc(buffer, size + readed);
			ptrBuffer = buffer + readed;
			memcpy(ptrBuffer, read, readed);
			size += readed;
		}
	} 

	fclose(file);

	if (sck != NULL) {
		free(sck);
	}

	sck = newStrConKer((Char) CONSOLA_ID, buffer, NULL, (Char) BESO_FILE, size, 0);
	
	return sendStream();
}

/**
 *  Envia por socket el sck 
 *	Funciones de sockets.h utilizadas:
 *	=================================
 *	Boolean socketSend(Socket *ptrDestination, SocketBuffer *ptrBuffer);
 */
Boolean sendStream() {
	if(sck == NULL) {
		printf("Error al tratar de enviar stream sin inicializar ---- Terminando \n");
		return FALSE;
	}

	SocketBuffer* sb = serializeConKer(sck);

	//Envio el socketBuffer
	if(!socketSend(kernelClient->ptrSocketServer, sb)) {
		printf("No se pudo enviar el Stream a kernel. ---- Terminando \n");
		return FALSE;
	}
	free(sb);
	return TRUE;
}

/**
 *  Recibe instrucciones del kernel ya sea para STD_INPUT o STD_OUTPUT
 *	y las manda a ejecutar
 *
 *	Funciones de sockets.h utilizadas:
 *	=================================
 *	SocketBuffer *socketReceive(Socket *emisor);
 */
Boolean instructionsFromKernel() {
	
	SocketBuffer* sb;

	if (skc != NULL) {
		free(skc);
	}

	// Recibo la orden del kernel y deserializo
	if ((sb = socketReceive(kernelClient->ptrSocket)) == NULL) {
		printf("No se pudo recibir el Stream del kernel. ---- Terminando \n");
		return FALSE;
	}

	skc = unserializeKerCon((Stream) sb->data);

	switch (skc->action) {
		case STD_INPUT:
			return handleStdInput();
		case STD_OUTPUT:
			return handleStdOutput();
		default: return FALSE;
	}

}

/**
 * 
 */
Boolean handleStdInput() {
	void* input;
	Int16U size;
	
	if (skc->inputType == NUMBER_INPUT) {
		printf("Ingresar numero\n");
		size = sizeof(Int32S);
		input = malloc(size);
		if (!scanf("%d", (int*) input)) {
			printf("error al ingresar numero. ---- Terminando\n");
			return FALSE;	
		}
	} else if (skc->inputType == TEXT_INPUT) {
		printf("Ingresar texto\n");
		size = MAX_INPUT_STR_LEN;
		input = malloc(size);
		if (!scanf("%s", (Byte*) input)) {
			printf("error al ingresar texto. ---- Terminando\n");
			return FALSE;
		}
	} else {
		printf("input type %d desconocido. ---- Terminando\n", skc->inputType);
		return FALSE;
	}
	
	free(skc);

	if (sck != NULL) {
		free(sck);
	}

	sck = newStrConKer((Char) CONSOLA_ID, NULL, (Byte*) input, (Char) STD_INPUT, 0, size);

	if(!sendStream()) {
		return FALSE;
	}

	return TRUE;
}

/**
 * Imprime por pantalla el log del kernel
 */
Boolean handleStdOutput() {
	printf("%s\n", skc->log);	
	free(skc);
	return TRUE;
}
