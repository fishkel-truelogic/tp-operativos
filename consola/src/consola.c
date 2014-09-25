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
#include <src/fereTypes.h>
#include <src/fereSockets.h>
#include <src/fereStream.h>
#include <src/commons/config.h>

//==========================================//
//******************************************//
// Constants								//	
//******************************************//
//==========================================//
#define KERNEL_PORT "KERNEL_PORT"
#define KERNEL_IP "KERNEL_IP"
#define PARAM_LENGTH 2
#define ESO_CONFIG "ESO_CONFIG"

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
Boolean getBESO(String); //TODO
Boolean sendBESO(); //TODO
Boolean instructionsFromKernel(); //TODO

//==========================================//
//******************************************//
// Main Function							//	
//******************************************//
//==========================================//
int main(int argc, char *argv[]) {
	
	//Inicializo la consola verificando el parametro cargando la configuracion externa
	// y conectandome por socket al kernel si algo de esto falla se termina la consola
	if (parameterValidation(argc) && loadConfig() && socketConnection()) {
		// obtengo el codigo BESO del archivo cuyo path llega por parametro
		if (!getBESO(argv[0])) {
			return FALSE;
		}
		// Envio el codigo BESO al kernel
		if (!sendBESO()) {
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
	return argc == 1;
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
		printf("ERROR: No se encuentra o falta el archivo de configuracion.\n");
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
		if (DEBUG == 1){
			printf("Archivo de config MSP leído\n===========\n");
			printf("MSP Port: %d\nKernel Port: %d\nMsp IP: %s\nKernel IP: %s\n",
					mspPort, kernelPort, mspIp, kernelIp);
		}

		return TRUE;

	} else {
		printf("El archivo config.txt no tiene todos los campos.\n");
		return FALSE;
	}
}
