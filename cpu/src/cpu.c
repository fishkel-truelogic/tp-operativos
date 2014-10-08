
//==========================================//
/**
 * Dependencies
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <src/fereTcb.h>
#include <src/fereTypes.h>
#include <src/fereSockets.h>
#include <src/fereStream.h>
#include <src/commons/bitarray.h>
#include <src/commons/config.h>
#include "instructions.h"
#include "cpu.h"

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

//instruction dictionary
t_dictionary* instructionOperators = NULL;

//=========================================//

/**
 * Prototypes
 */
Boolean socketConnection();
Boolean getNextTcb();
Boolean processTcb();
Boolean loadConfig();
void execute(Instruction*);
Boolean mspRequest();
Instruction* getInstruction();
void* setRegisterOperator(Byte*);


int main() {
	// Cargo las variables de configuracion y me conecto al kernel y msp
	if (loadInstructionDictionary(instructionOperators) && loadConfig() && socketConnection()) {
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
	if (!socketConnect(kernelClient, kernelIp, kernelPort)) {
		printf("No se pudo conectar al kernel en la direccion %s puerto %d \n", kernelIp, kernelPort);
		return FALSE;
	}

	//Creo socket cliente para msp y me conecto con la ip y el puerto
	mspClient = socketCreateClient();
	if (!socketConnect(mspClient, mspIp, mspPort)) {
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
		currentTcb = newEmptyTcb();
		sck = newStrCpuKer(CPU_ID, *currentTcb, FIRST_TCB, 0, NULL, 0, 0, 0, 0);
	}
	//Serializo y armo el socketBuffer
	SocketBuffer* sb = serializeCpuKer(sck);

	//Envio el socketBuffer
	if(!socketSend(kernelClient->ptrSocketServer, sb)) {
		printf("No se pudo enviar el Stream a kernel. \n");
		return FALSE;
	}
	free(sb);

	// Recibo la respuesta del kernel y deserializo
	if ((sb = socketReceive(kernelClient->ptrSocket)) == NULL) {
		printf("No se pudo recibir el Stream del kernel. \n");
		return FALSE;
	}

	skc = unserializeKerCpu((Stream) sb->data);
	currentTcb = skc->tcb;
	return TRUE;
}

/**
 * Se procesa las instrucciones, tantas dependiendo el modo kernel, el quantum y si ocurrio alguna interrupcion
 * El tcb es devuelto al kernel en esta funcion unicamente si es expulsado por quantum
 */
Boolean processTcb() {
	Instruction* instruction;
	Int8U quantum = skc->quantum;
	Boolean inte = FALSE;
	while (currentTcb->kernelMode || quantum > 0) {
		if ((instruction = getInstruction()) == NULL) {
			printf("No se pudo obtener la siguiente instruccion\n");
			return FALSE;
		}
		execute(instruction);
		switch (sck->action) {
			case INTE:
			case STD_INPUT:
			case STD_OUTPUT:
			case NEW_THREAD:
			case JOIN_THREADS:
			case BLOCK_THREAD:
			case WAKE_THREAD:
			case PROC_END:
				inte = TRUE;
				break;
			default: 
				quantum--;
				if (quantum == 0) {
					sck->tcb = currentTcb;
					sck->action = NEXT_TCB;
				}
				break;
		}
		//Si ocurrio una interrupcion, se sale de la ejecucion para dejar lugar a otro proceso
		if (inte) {
			break;
		}
	}

	return TRUE;
}

/**
 *	Le pide la siguente instruccion BESO a la MSP segun el registro P del TCB
 *  Parsea los primeros 4 bytes del stream para saber que instruccion es y determina que parametros recibe
 * 	Completa la instruccion
 */
Instruction* getInstruction() {
	Int8U i, operatorsTotal;
	String instructionName = malloc(sizeof(Char) * 4);
	Instruction* instruction = malloc(sizeof(instruction));
	if (mspRequest()) {
		Byte* ptrData = smc->data;
		for (i = 0; i < 4; i++) {
			memcpy(instructionName, ptrData, sizeof(Char));
			ptrData++;
		}
		instruction->name = instructionName;

		operatorsTotal = getInstructionOperatorsTotal(instructionOperators, instructionName);
		for (i = 0; i < operatorsTotal; i++) {
			if (operatorIsRegister(instructionOperators, instructionName, i)) {
				instruction->op[i] = setRegisterOperator(ptrData);
				ptrData++;
			} else { //si no es un registro entonces es un numero o una direccion siezof(Int32) = 4
				memcpy(instruction->op[i], ptrData, 4));
				ptrData += 4;
			}
		}
		free(ptrData);
		return instruction;
	}
	return NULL;
}

/**
 *  Obtiene una nuevo instruccion para ejecutar de la MSP
 * 
 *	Funciones de sockets.h utilizadas:
 *	=================================
 *	SocketBuffer *socketReceive(Socket *emisor);
 *	Boolean socketSend(Socket *ptrDestination, SocketBuffer *ptrBuffer);
 **/
Boolean mspRequest() {
	int i;
	if (scm != NULL) {
		free(scm);
	}
	scm = newStrCpuMsp(CPU_ID, currentTcb->P, NEXT_INSTRUCTION, NULL, 0, currentTcb->pid);

	//Serializo y armo el socketBuffer
	SocketBuffer* sb = serializeCpuMsp(scm);

/**
 * Carga las variables de configuracion externa
 */
Boolean loadConfig() {

	//Gennero tabla de configuracion
	t_config* tConfig = config_create(CONFIG_FILE);
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

	//Envio el socketBuffer
	if(!socketSend(mspClient->ptrSocketServer, sb)) {
		printf("No se pudo enviar el Stream a la MSP. \n");
		return FALSE;
	}
	free(sb);

	// Recibo la respuesta de la msp y deserializo
	if ((sb = socketReceive(mspClient->ptrSocket)) == NULL) {
		printf("No se pudo recibir el Stream de la MSP. \n");
		return FALSE;
	}

	smc = unserializeMspCpu((Stream) sb->data);

	return TRUE;
}
 
 /**
  * Se fija cual es el registro que vino como parametro en la instruccion y lo devuelve
  * luego incrementa el valor del puntero a la instruccion BESO 
  */
void* setRegisterOperator(Byte* ptrData) {
	void* ptr = NULL;
	if ((Char) *ptrData == 'A') {
		ptr = &currentTcb->A;
	} else if ((Char) *ptrData == 'B') {
		ptr = &currentTcb->B;
	} else if ((Char) *ptrData == 'C') {
		ptr = &currentTcb->C;
	} else if ((Char) *ptrData == 'D') {
		ptr = &currentTcb->D;
	} else if ((Char) *ptrData == 'E') {
		ptr = &currentTcb->E;
	} else if ((Char) *ptrData == 'F') {
		ptr = &currentTcb->F;
	}
	return ptr;

}

/**
 * Ejecuta la funcion correspondiente segun el nombre de la instruccion
 **/
void execute(Instruction* instruction) {
	InstructionOperators* iop = dictionary_get(instructionOperators, instruction->name);
	iop->func(instruction->op[0], instruction->op[1], instruction->op3[2]);
}

Tcb* getCurrentTcb() {
	return currentTcb;
}

StrCpuKer* getSCK() {
	return sck;
}

StrKerCpu* getSKC() {
	return skc;
}