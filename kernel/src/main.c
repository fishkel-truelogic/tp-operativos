/*
 * main.c
 *
 *  Created on: 16/09/2014
 *      Author: cesarcappetto
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <src/commons/config.h>
#include <src/commons/collections/list.h>
#include <src/fereTypes.h>
#include <src/fereSockets.h>
#include <src/fereStream.h>
#include <src/fereSemaphore.h>

#include "kdefine.h"




//VARIABLES GLOBALES Y ESTRUCTURAS PROPIAS
//==========================================================================
typedef struct strKernelConfig {

	Int32U port;
	String mspAddress;
	Int32U mspPort;
	Int8U quantum;
	String syscalls;
	Int16U stack;

} KernelConfig;


t_list *newQueue;
t_list *readyQueue;
t_list *execQueue;
t_list *blockQueue;
t_list *exitQueue;

KernelConfig *config;
SocketClient *socketMsp;

Int32U lastPid;
Tcb *tcbKm;

pthread_t thrScheduler;

//CONJUNTO MAESTRO DE DESCRIPTORES
fd_set master;
//CONJUNTO TEMPORAL DE DESCRIPTORES
fd_set read_fds;
//MAXIMO DESCRIPTOR A MONITOREAR
Int32U fdmax;

//LISTA DE CLIENTES CPU Y CONSOLA
t_list *cpuList;
t_list *consoleList;

pthread_mutex_t mtxCpuList;
sem_t semCpuList;

//PROTOTIPOS Y FUNCIONES
//==========================================================================

/**
 * @NAME: printHeader
 * @DESC: Imprime un pequeño encabezado por pantalla
 */
void printHeader() {

	printf("\n");
	printf(TITLE);
	printf("\n");

}
/**
 * @NAME: init
 * @DESC: Realiza las inicializaciones de TODOS los componentes del sistema
 */
void init()
{
	printHeader();

	lastPid = 2; //PORQUE PID 1 ES EL TCB KERNEL MODE
	config = malloc(sizeof(KernelConfig));

	newQueue = list_create();
	readyQueue = list_create();
	execQueue = list_create();
	blockQueue = list_create();
	exitQueue = list_create();

	cpuList = list_create();
	consoleList = list_create();

	mtxInit(&mtxCpuList);
	semInit(&semCpuList, 0);
}
/**
 * @NAME: loadConfig
 * @DESC: Realiza la carga del archivo config del kernel
 * 		: Retorna TRUE si el proceso tuvo exito, FALSE caso contrario
 */
Boolean loadConfig() {

	//Gennero tabla de configuracion
	t_config * tConfig = config_create(CONFIG_FILE);
	if (tConfig == NULL ) {
		printf("ERROR: No se encuentra o falta el archivo de configuracion.\n");
		return FALSE;
	}
	//Verifico consistencia, tiene que haber 4 campos
	if (config_keys_amount(tConfig) == PARAM_LENGTH) {

		if (config_has_property(tConfig, PORT_ATRIB)) {
			config->port = config_get_int_value(tConfig, PORT_ATRIB);
		} else {
			printf("ERROR: Falta parametro PUERTO\n");
			return FALSE;
		}

		if (config_has_property(tConfig, IP_MSP_ATRIB)) {
			config->mspAddress = config_get_string_value(tConfig, IP_MSP_ATRIB);
		} else {
			printf("ERROR: Falta parametro IP_MSP\n");
			return FALSE;
		}

		if (config_has_property(tConfig, PORT_MSP_ATRIB)) {
			config->mspPort = config_get_int_value(tConfig, PORT_MSP_ATRIB);
		} else {
			printf("ERROR: Falta parametro PUERTO_MSP\n");
			return FALSE;
		}

		if (config_has_property(tConfig, QUANTUM_ATRIB)) {
			config->quantum = config_get_int_value(tConfig, QUANTUM_ATRIB);
		} else {
			printf("ERROR: Falta parametro QUANTUM\n");
			return FALSE;
		}

		if (config_has_property(tConfig, SYSCALLS_ATRIB)) {
			config->syscalls = config_get_string_value(tConfig, SYSCALLS_ATRIB);
		} else {
			printf("ERROR: Falta parametro SYSCALLS\n");
			return FALSE;
		}

		if (config_has_property(tConfig, STACK_ATRIB)) {
			config->stack = config_get_int_value(tConfig, STACK_ATRIB);
		} else {
			printf("ERROR: Falta parametro STACK\n");
			return FALSE;
		}

		//Libero tabla config
		config_destroy(tConfig);

		/*printf(
		 "Archivo de config Kernel leído\n=============================\n");
		 printf("PUERTO: %d\n",config->port);
		 printf("IP_MSP: ");
		 puts((char*)config->mspAddress);
		 printf("PUERTO_MSP: %d\n", config->mspPort);
		 printf("QUANTUM: %d\n", config->quantum);
		 printf("SYSCALLS: ");
		 puts((char*)config->syscalls);*/

		return TRUE;
	} else {
		printf("El archivo config.txt no tiene todos los campos necesarios\n");
		return FALSE;
	}
}


//ESTA FUNCION SE ELIMINA CUANDO TENGAMOS PLANIFICADOR, POR AHORA LA DEJO
//PARA QUE ME COMPILE EL PROGRAMA
void moveToNew(Tcb *tcb){

}


/**
 * @NAME: newCpuClient
 * @DESC: Realiza la gestion de una nueva conexion de cpu recibida
 * @PARAM:
 * 	cpuClient		: Estrucutura Socket con el descriptor del socket
 * 	dataSerialized	: Stream con los datos sin deserializar recibidos por la cpu
 *
 */
void newCpuClient(Socket *cpuClient,Stream dataSerialized){

	printf("Nuevo Cliente CPU aceptado!! \n");

	StrCpuKer * sck = unserializeCpuKer(dataSerialized);

	if(sck->action == FIRST_TCB){

		//AGREGARLO A LA LISTA DE DESCRIPTORES DE CPU  DEL PLANIFICADOR
		mtxLock(&mtxCpuList);
		list_add(cpuList,cpuClient);
		mtxUnlock(&mtxCpuList);
		semSignal(&semCpuList);


	}else{
		printf("No se pudo determinar la accion del cliente CPU\n");
	}

}
/**
 * @NAME: createNewTcb
 * @DESC: Realiza la creacion de un Tcb no kernel mode.
 * 		: Al principio se inicializan los valores de CS y SS en 0,
 * 		: mas adelante seran modificados, cuando se posean esos valores
 * @PARAM:
 * 	sck	: Estructura StrConKer que contendra la longitud del CS
 */
Tcb *createNewTcb(StrConKer *sck){

	Tcb *tcb;

	tcb = newTcb(lastPid, 1, 0, 0, 0, 0, FALSE, sck->fileContentLen, 0, 0, 0, 0,0, 0);

	//ACTUALIZO EL PID
	lastPid++;

	return tcb;

}
/**
 * @NAME: createNewTcbKM
 * @DESC: Realiza la creacion de un Tcb en modo Kernel.
 */
Tcb *createNewTcbKM(){


	Tcb *tcb = malloc(sizeof(Tcb));

	tcb = newTcb(1, 1, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0,0, 0);

	return tcb;

}
/**
 * @NAME: getSegmentFromMSP
 * @DESC: Solicita a la MSP un Segmento para el tamaño ingresado.
 * 		: Retorna la direccion base de ese segmento o un codigo de error
 * @PARAM:
 * 	size	: La longitud del segmento a solicitar
 * 	tcb		: Estrcutura Tcb de donde se obtendra el pid del proceso
 */
Int32U getSegmentFromMSP(Int16U size,Tcb *tcb){

	StrKerMsp *skm = malloc(sizeof(StrKerMsp));
	skm->id = KERNEL_ID;
	skm->action = CREATE_SEG;
	skm->pid = tcb->pid;
	skm->size = size;

	//ENVIO A MSP
	t_bitarray *barray = serializeKerMsp(skm);
	SocketBuffer *sb = malloc(sizeof(SocketBuffer));
	Byte *ptrByte = (Byte*) barray->bitarray;

	int i;
	for (i = 0; i < barray->size; i++) {
		sb->data[i] = *ptrByte;
		ptrByte++;
	}
	sb->size = barray->size;

	socketSend(socketMsp->ptrSocket,sb);

	//RECIBO DE MSP
	sb = socketReceive(socketMsp->ptrSocket);
	StrMspKer *smk = unserializeMspKer((Stream) sb->data);

	//ESTE CHEQUEO ES MEDIO REDUNDANTE...VER SI SE QUEDA...
	if(smk->action == CREATE_SEG){

		if(smk->address == ERROR){
			return ERROR;
		}else{
			return smk->address;
		}


	}

	return ERROR;
}
/**
 * @NAME: creationError
 * @DESC: Notifica a la consola de que ocurrio un error al momento de crear
 * 		: el nuevo proceso solicitado. Esto ocurre al producirse un error al
 * 		: reservar los segmentos CS y SS en la MSP
 * @PARAM:
 * 	client	: Estructura Socket con el descriptor del cliente consola
 */
void creationError(Socket *client){

	StrKerCon *skc = malloc(sizeof(StrKerCon));

	skc->action = STD_OUTPUT;
	skc->log = (Byte *) "No se pudo reservar los segmentos CS y SS. El programa terminara\n";
	skc->logLen =  strlen((char *)skc->log) + 1;

	//ENVIO A MSP
	t_bitarray *barray = serializeKerCon(skc);

	SocketBuffer *sb = malloc(sizeof(SocketBuffer));
	Byte *ptrByte = (Byte*) barray->bitarray;

	int i;
	for (i = 0; i < barray->size; i++) {
		sb->data[i] = *ptrByte;
		ptrByte++;
	}
	sb->size = barray->size;

	socketSend(client,sb);

	//ELIMINO EL DESCRIPTOR DEL CONJUNTO
	FD_CLR(client->descriptor, &master);

	//CIERRO EL SOCKET
	socketDestroy(client);
	free(client);


}
/**
 * @NAME: newConsoleClient
 * @DESC: Realiza la gestion de una nueva conexion de consola recibida
 * @PARAM:
 * 	consoleClient	: Estrucutura Socket con el descriptor del socket
 * 	dataSerialized	: Stream con los datos sin deserializar recibidos por la consola
 */
void newConsoleClient(Socket *consoleClient, Stream dataSerialized){

	StrConKer * sck = unserializeConKer(dataSerialized);

	printf("Nuevo Cliente Consola aceptado!! \n");

	if(sck->action == BESO_FILE){

		//SI ESAS DOS SALEN BIEN, SE GENERA EL NUEVO TCB
		Tcb *tcb = createNewTcb(sck);

		//RESERVAR DE MSP SEGMENTO DE CODIGO
		Int32U csDir = getSegmentFromMSP(sck->fileContentLen,tcb);

		//RESERVAR DE MSP SEGMENTO DE STACK
		Int32U ssDir = getSegmentFromMSP(config->stack,tcb);

		if(csDir != ERROR && ssDir != ERROR){

			//ESCRIBIR EL CODIGO BESO EN EL SEGMENTO CS DE LA MSP
			SocketBuffer *codeBuffer = malloc(sizeof(SocketBuffer));

			int i;
			for (i = 0; i < sck->fileContentLen; i++) {
				codeBuffer->data[i] = *sck->fileContent;
				sck->fileContent++;
			}
			codeBuffer->size = sck->fileContentLen;

			socketSend(socketMsp->ptrSocket,codeBuffer);

			//ACTUALIZO LOS REGISTROS DE CS Y SS
			tcb->M = csDir;
			tcb->P = csDir;

			tcb->X = ssDir;
			tcb->S = ssDir;

			//MUEVO EL NUEVO TCB A LA COLA DE NEW
			//TODO PREGUNTARLE A NACHO COMO SE LLAMA LA FUNCION...
			moveToNew(tcb);
		}
		else{

			//NOTIFICO A CONSOLA QUE SE ABORTARA LA CONEXION
			creationError(consoleClient);
		}



	}else{
		printf("No se pudo determinar la accion de un cliente Consola\n");
	}



}
/**
 * @NAME: newClientHandler
 * @DESC: Realiza la gestion de las NUEVAS conexiones que ingresan al kernel.
 * 		: Como pueden ser CPU o Consola, la funcion realiza la distincion y
 * 		: las guarda en la lista que corresponda (cpuList, consoleList)
 * @PARAMS:
 *
 * 	client	: Estructura Socket con el descriptor del nuevo cliente que se conecto
 */
void newClientHandler(Socket *client){

	printf("Nueva Conexion detectada, realizando handshake\n");

	//ADMINISTRO LA NUEVA CONEXION Y REALIZO EL HANDSHAKE
	SocketBuffer *buffer = socketReceive(client);
	Stream strReceived = (Stream)buffer->data;
	Char id = getStreamId(strReceived);

	switch (id) {

	case CPU_ID:
		newCpuClient(client,strReceived);
		break;

	case CONSOLA_ID:
		newConsoleClient(client,strReceived);
		break;

	}
}


void clientHandler(Int32U clientDescriptor, fd_set *master) {

	Socket *ptrTemp = malloc(sizeof(Socket));
	ptrTemp->descriptor = clientDescriptor;

	SocketBuffer *ptrBuffer = socketReceive(ptrTemp);

	if (ptrBuffer == NULL ) {
		printf("Ha ocurrido un error al intentar recibir de %d\n",
				clientDescriptor);

		//ELIMINO EL DESCRIPTOR DEL CONJUNTO
		FD_CLR(clientDescriptor, master);

		//CIERRO EL SOCKET
		socketDestroy(ptrTemp);
		free(ptrTemp);

	} else {

		// ** SE ATIENDEN SOLICITUDES DE CLIENTES **



		//PRIMRO SE TIENE QUE PEDIR EL ID PARA DETERMINAR SI ES CONSOLA
		//O CPU, SEGUN ESO, DESERIALIZO
		//StrConKer *sck = unserializeConKer((Stream)ptrBuffer->data);

		//DESPUES TENGO QUE DETERMINAR QUE ACCION ES

		printf("%d: %s", clientDescriptor, ptrBuffer->data);

	}

}

void *thrSchedulerHandler(void *ptr) {

	printf("Planificador BPRR iniciado\n");

	while(TRUE)
	{
		//PRIMERO TOMO CUALQUIER PROCESO DE LA COLA NEW, Y LO AGREGO A LA COLA DE READY

		//PLANIFICO EL PROXIMO PROCESO A EJECUTAR
	}

	/*char *message;
	message = (char *) ptr;

	int i = 0;
	for (i = 0; i < 1000; i++) {

		p = p + 1;

		printf("%s : %d\n", message, p);
		sleep(1);
	}*/
	//PARA QUE NO HINCHE LAS PELOTAS CUANDO COMPILO
	return NULL;
}

//PROGRAMA PRINCIPAL
//==========================================================================
int main() {

	init();

	//LEVANTO LA CONFIG DEL KERNEL
	if (!loadConfig()) {

		printf(
				"No se ha podido leer el archivo de configuracion. El programa terminara\n");
		return -1;

	}

	//HACER CONEXION CON MSP
	socketMsp = socketCreateClient();
	if (!socketConnect(socketMsp, config->mspAddress, config->mspPort)) {

		printf(
				"No se ha podido realizar la conexion con la MSP. El programa terminara\n");
		return -1;
	}

	//LANZO HILO PLANIFICADOR
	Int32U createResult;
	createResult = pthread_create(&thrScheduler, NULL, thrSchedulerHandler, NULL);
	if (createResult != 0) {
		printf(
				"No se ha podido iniciar el planificador. El programa terminara\n");
		return -1;
	}


	//CREO TCB KERNEL MODE Y LO ENCOLO EN BLOCK
	tcbKm = createNewTcbKM();


	//TODO: COMUNICARME CON MSP Y CARGAR EL SYSCALL DEL TCBKM Y SU STACK


	//TODO: LLEVAR ESTO A OTRO ARCHIVO....
	Socket *ptrServerSocket = socketCreateServer(config->port);
	socketListen(ptrServerSocket);

	//SETEO A "0" EL MASTER Y EL TEMPORAL
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	//CARGO EL SOCKET DE ESCUCHA A MASTER
	FD_SET(ptrServerSocket->descriptor, &master);

	//CARGO EL SOCKET MAS GRANDE
	fdmax = ptrServerSocket->descriptor;

	struct timeval timeout;

	//BUCLE PRINCIPAL
	while (TRUE) {

		read_fds = master;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;

		//HAGO EL SELECT
		Int32U selectResult = select(fdmax + 1, &read_fds, NULL, NULL,&timeout);

		if (selectResult == -1) {

			//SE CAYO MI SOCKET DE ESCUCHA
			printf("Error en el socket de escucha\n");
			break;


		} else {

			//RECORRO TODOS LOS DESCRIPTORES MONITOREADOS PARA VER QUIEN LLAMO
			Int32U i;
			for (i = 0; i <= fdmax; i++) {

				if (FD_ISSET(i, &read_fds)) {

					//FUE EL SOCKET DE ESCUCHA??
					if (i == ptrServerSocket->descriptor) {

						//SI, ENTONCES GESTIONO LA NUEVA CONEXION ENTRANTE
						Socket *clientSocket = socketAcceptClient(ptrServerSocket);
						newClientHandler(clientSocket);

						//LO CARGO A LA LISTA DE DESCRIPTORES A MONITOREAR Y ACTUALIZO EL MAXIMO
						FD_SET(clientSocket->descriptor, &master);

						if (clientSocket->descriptor > fdmax) {
							fdmax = clientSocket->descriptor;
						}


					} else {

						//NO, ENTONCES GESTIONO EL SOCKET QUE HABLO...
						clientHandler(i, &master);

					}
				}
			}
		}

	}

	//FINALIZO EL HILO PLANIFICADOR
	pthread_join(thrScheduler, NULL );

	socketDestroy(ptrServerSocket);
	free(ptrServerSocket);


	return 0;
}

