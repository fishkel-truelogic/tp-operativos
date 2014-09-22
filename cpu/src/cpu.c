
//==========================================//
/**
 * Dependencies
 */
#include <stdlib.h>
#include <stdio.h>
#include <src/fereTcb.h>
#include <src/fereTypes.h>
#include <src/fereSockets.h>
#include <src/fereStream.h>
#include <src/commons/bitarray.h>

//==========================================//
/**
 * Constants
 */

#define CONFIG_FILE = "config.txt"
#define PARAM_LENGTH = 5
#define KERNEL_PORT "KERNEL_PORT"
#define MSP_PORT "MSP_PORT"
#define KERNEL_IP "KERNEL_IP"
#define MSP_IP "MSP_IP"
#define SLEEP_IN_MILLIS "SLEEP_IN_MILLIS"

//==========================================//
/**
 * Global variables
 */

// Sockets
SocketClient* kernelClient = NULL;
SocketClient* mspClient = NULL;

// Streaming
StrCpuKer* sck = NULL;
StrCpuMsp* scm = NULL;
StrKerCpu* skc = NULL;
StrMspCpu* smc = NULL;

// Configuration
String kernelIp = NULL;
String mspIp = NULL;
Int16U kernelPort = 0;
Int16U mspPort= 0;
Int16U sleepInMillis = 0;

// Current Tcb
Tcb* currentTcb = NULL;

//=========================================//

/**
 * Prototypes
 */
Boolean socketConnection();
Boolean getNextTcb();
Boolean processTcb();
Boolean loadConfig();

int main() {
	// Cargo las variables de configuracion y me conecto al kernel y msp
	if (loadConfig() && socketConnection()) {
		while(TRUE) {
			// devuelvo el tcb procesado y obtengo uno nuevo del kernel
			if(!getNextTcb()) {
				return FALSE;
			}
			// proceso el tcb del kernel
			if(!processTcb()) {
				return FALSE;
			}
		}
	}
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
	if (!scoketConnect(kernelClient->ptrSocket, kernelIp, kernelPort)) {
		printf("No se pudo conectar al kernel en la direccion %s puerto %d \n", kernelIp, kernelPort);
		return FALSE;
	}

	//Creo socket cliente para msp y me conecto con la ip y el puerto
	mspClient = socketCreateClient();
	if (!scoketConnect(mspClient->ptrSocket, mspIp, mspPort)) {
		printf("No se pudo conectar a la msp en la direccion %s puerto %d \n", mspIp, mspPort);
		return FALSE;
	}
	return TRUE;
}

/**
 *  Devuelve el Tcb actualizado al kernel y obtiene uno nuevo para procesar
 * 
 *	Funciones de sockets.h utilizadas:
 *	=================================
 *	SocketBuffer *socketReceive(Socket *emisor);
 *	Boolean socketSend(Socket *ptrDestination, SocketBuffer *ptrBuffer);
 */
Boolean getNextTcb() {
	int i;
	if (sck == NULL) {
		Char id = CPU_ID; 
		String log "\0"; 
		Tcb tcb = newEmptyTcb(); 
		Char status = FIRST_TCB, 
		Char action = FIRST_TCB,
		Int16U logLen = 0;
		sck = newStrCpuKer(id, log, tcb, status, action, logLen);
	}
	//Serializo y armo el socketBuffer
	SocketBuffer* sb = malloc(sizeof(SocketBuffer));
	t_bitarray* barray = serializeCpuKer(sck);
	Byte* ptrByte = (Byte*) barray->bitarray;
	for (i = 0; i < barray->size, i++) {
		sb->data[i] = *ptrByte;
		ptrByte++;
	}
	sb->size = barray->size;

	//Envio el socketBuffer
	if(socketSend(kernelClient->ptrSocketServer, sb)) {
		printf("No se pudo enviar el Stream a kernel. \n");
		return FALSE;
	}
	free(sb);

	// Recibo la respuesta del kernel y deserializo
	if (sb = socketReceive(kernelClient->ptrSocket) == NULL) {
		printf("No se pudo recibir el Stream del kernel. \n");
		return FALSE;
	}

	skc = unserializeKerCpu((Stream) sb->data);

	return TRUE;
}

/**
 *
 */
Boolean processTcb() {
	Instruction* instruction;
	Int8U quantum = skc->quantum;
	while (tcb->kernelMode || quantum > 0) {
		if (instruction = getInstruction() == NULL) {
			return FALSE;
		}
		switch (execute(instruction)) {
			case FATAL_ERROR: return FALSE;
			case SYS_CALL: break;//TODO syscall impl;
			case SEG_FAULT: break; // TODO impl segmentation fault case;
			case OK: quantum--; break;
			default: break;
		}
	}

	return TRUE;
}

/**
 *
 */
Boolean loadConfig() {

	//Gennero tabla de configuracion
	t_config * tConfig = config_create(CONFIG_FILE);
	if (tConfig == NULL){
		printf("ERROR: No se encuentra o falta el archivo de configuracion.\n");
		return FALSE;
	}
	//Verifico consistencia, tiene que haber 5 campos
	if (config_keys_amount(tConfig) == PARAM_LENGTH) {

		//Verifico que los parametros tengan sus valores OK-> PORT=12456 | NOK->PORT=
		if (config_has_property(tConfig, MSP_PORT)) {
			mspPort = config_get_int_value(tConfig, MSP_PORT);
		} else {
			printf("ERROR: Falta un parametro.\n");
			return FALSE;
		}

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

		if (config_has_property(tConfig, MSP_IP)) {
			mspIp = config_get_string_value(tConfig, MSP_IP);
		} else {
			printf("ERROR: Falta un parametro.\n");
			return FALSE;
		}

		if (config_has_property(tConfig, SLEEP_IN_MILLIS)) {
			sleepInMillis = config_get_string_value(tConfig, SLEEP_IN_MILLIS);
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