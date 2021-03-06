/*
 * main.c
 *
 *  Created on: 16/09/2014
 *      Author: cesarcappetto, ignaciodoring
 *
 *	Programa Kernel
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
#include "scheduler.h"

//VARIABLES GLOBALES
//==========================================================================

//CONFIGURACION DEL PROYECTO
KernelConfig *config;

//PID GLOBAL
Int32U lastPid;

//HILO PLANIFICADOR (GENERAL)
pthread_t thrScheduler;

//SOCKET DE ESCUCHA
Socket *serverSocket;

//SOCKET CLIENTE DE LA MSP
SocketClient *socketMsp;

//CONJUNTO MAESTRO DE DESCRIPTORES
fd_set master;

//CONJUNTO TEMPORAL DE DESCRIPTORES
fd_set read_fds;

//MAXIMO DESCRIPTOR A MONITOREAR
Int32U fdmax;

//LISTA DE CLIENTES CPU Y CONSOLA
t_list *cpuList;
t_list *consoleList;

//SEMAFOROS MUTEX PARA LISTA DE CPU Y DE CONSOLA
pthread_mutex_t mtxCpuList;
pthread_mutex_t mtxConsoleList;

//SEMAFORO CONTADOR DE CPUS DISPONIBLES
sem_t semCpuList;



//FUNCIONES
//==========================================================================
/**
 *@NAME: getFreeCpu
 *@DESC: Retorna una Cpu que no este ejecutando ningun tcb
 */
Cpu *getFreeCpu(){

	Cpu *cpuClient = NULL;

	Int16U i;
	for (i = 0; i < list_size(cpuList); i++) {

		mtxLock(&mtxCpuList);
		Cpu *temp = list_get(cpuList, i);
		if (temp->tcb == NULL) {
			cpuClient = temp;
			mtxUnlock(&mtxCpuList);
			break;
		}
		mtxUnlock(&mtxCpuList);
	}

	return cpuClient;
}
/**
 *@NAME	: releaseResourceThread
 *@DESC	: Envia a la Msp la orden de destruir el segmento de Stack de un
 *		: proceso determinado.
 *@PARAM:
 * tcb	: El tcb del que se quiere hacer la liberacion de recursos
 */
void releaseResourceThread(Tcb *tcb){

	StrKerMsp *skm = malloc(sizeof(StrKerMsp));

	skm->action = DELETE_SEG;
	skm->pid = tcb->pid;
	skm->address = tcb->X;

	SocketBuffer *sb = serializeKerMsp(skm);
	socketSend(socketMsp->ptrSocket, sb);
}
/**
 *@NAME	: releaseResourceProcess
 *@DESC	: Envia a la Msp la orden de destruir el segmento de Codigo de un
 *		: proceso determinado.
 *@PARAM:
 * tcb	: El tcb del que se quiere hacer la liberacion de recursos
 */
void releaseResourceProcess(Tcb *tcb){

	StrKerMsp *skm = malloc(sizeof(StrKerMsp));

	skm->action = DELETE_SEG;
	skm->pid = tcb->pid;
	skm->address = tcb->M;

	SocketBuffer *sb = serializeKerMsp(skm);
	socketSend(socketMsp->ptrSocket, sb);
}
/**
 * @NAME: sendTcbToFreeCpu
 * @DESC: Toma de la lista de Cpus disponibles, la primera que este libre
 * 		: y realiza el envio del Tcb recibido por parametro a esa CPU
 * 		: Como existe un semaforo contador de Cpus Libres, al llamar esta
 * 		: funcion siempre se garantiza que por lo menos, existe una Cpu libre
 * 		: Esta funcion se llama desde el Planificador
 * @PARAMS:
 * 	tcb	: El tcb a enviar
 */
void sendTcbToFreeCpu(Tcb *tcb){

	Cpu *freeCpu = getFreeCpu();

	//NO HACE FALTA CHEQUEAR QUE SEA DISTINTO DE NULL PORQUE
	//SIEMPRE VA A HABER COMO MINIMO UNA CPU LIBRE (VER SEMAFORO CONTADOR)
	StrKerCpu *skc = malloc(sizeof(StrKerCpu));
	skc->tcb = *(freeCpu->tcb);
	skc->quantum = config->quantum;
	skc->action = NEXT_TCB;

	SocketBuffer *sb = serializeKerCpu(skc);
	socketSend(freeCpu->cpuClient, sb);

}
/**
 * @NAME: consoleCpuDown
 * @DESC: Notifica a una consola, que la cpu donde se estaba ejecutando el TCB
 * 		: enviado, se desconecto
 */
void consoleCpuDown(Console *console){
	StrKerCon *skc = malloc(sizeof(StrKerCon));

	skc->action = STD_OUTPUT;
	skc->log = (Byte *) "El programa enviado debera ser abortado por problemas en la conexion\n";
	skc->logLen = strlen((char *) skc->log) + 1;

	//ENVIO A MSP
	SocketBuffer *sb = serializeKerCon(skc);
	socketSend(console->consoleClient, sb);
}
/**
 * @NAME: consoleDown
 * @DESC: Proceso que se llama cuando una Consola se desconecta. Le notifca al planificador
 * 		: para que mueva ese tcb a exit
 * @PARAMS:
 * console	: Estructura con la consola que se desconecto
 */
void consoleDown(Console *console){

	//LLAMO AL SEEK AND DESTROY
	pthread_t thr;
	pthread_create(&thr,NULL, seekAndDestroyPid,(void *)console->tcb);
	pthread_join(thr,NULL);

	//ELIMINO LA CONSOLA DE LA LISTA
	//CLAVAR MUTEX ACA
	Int32S i;
	for(i = 0; i < list_size(consoleList);i++){
		Console *temp = list_get(consoleList,i);
		if(temp->consoleClient->descriptor == console->consoleClient->descriptor){
			break;
		}
	}
	list_remove(consoleList,i);
	//Y ACA...

}
/**
 * @NAME: cpuDown
 * @DESC: Proceso que se llama cuando una CPU se desconecta. Si la cpu que se desconecto
 * 		: tenia un tcb en ejecucion, le notifca al planificador para que mueva ese
 * 		: tcb a exit. Ademas notifica a la consola que envio ese tcb a ejecutar
 * @PARAMS:
 * 	cpu	: Estructura con los datos de esa cpu que se callo
 */
void cpuDown(Cpu *cpu){

	if(cpu->tcb != NULL){
		//TIENE TCB EN EJECUCION
		//MUEVO ESE TCB A EXIT
		pthread_t thr;
		pthread_create(&thr,NULL, seekAndDestroyPid,(void *)cpu->tcb);
		pthread_join(thr,NULL);

		//NOTIFICO A LA CONSOLA QUE MANDO ESTE TCB
		Console *console = getConsoleByPid(cpu->tcb->pid);
		consoleCpuDown(console);
	}else{
		//JUSTO ESA CPU ESTABA LIBRE
		//BAJO EL SEMAFORO DE CPUS DISPONIBLES
		semWait(&semCpuList);
	}


	//ELIMINO A ESTA CPU DE LA LISTA DE CPUS
	Int32S i;
	for(i = 0; i < list_size(cpuList);i++){
		Cpu *temp = list_get(cpuList,i);
		if(temp->cpuClient->descriptor == cpu->cpuClient->descriptor){
			break;
		}
	}
	list_remove(cpuList,i);


}
/**
 * @NAME: getConsoleByPid
 * @DESC: Devuelve un elemento de la lista de consolas en funcion del pid
 * 		: La lista de Consolas es una lista donde se guarda, el socket de
 * 		: la consola cliente y el tcb que esta envio
 * @PARAM:
 * 	pid	: El pid por el que se desea filtrar la lista
 */
Console *getConsoleByPid(Int32U pid){

	Console *consoleClient = NULL;

	Int16U i;
	for(i = 0; i < list_size(consoleList); i++){

		mtxLock(&mtxConsoleList);
		Console *temp = list_get(consoleList,i);
		if(temp->tcb->pid == pid){
			consoleClient = temp;
			mtxUnlock(&mtxConsoleList);
			break;
		}
		mtxUnlock(&mtxConsoleList);
	}

	return consoleClient;

}
/**
 * @NAME: getConsoleByDescriptor
 * @DESC: Devuelve un elemento de la lista de consolas en funcion del descriptor
 * 		: La lista de Consolas es una lista donde se guarda, el socket de
 * 		: la consola cliente y el tcb que esta envio
 * @PARAM:
 * 	descriptor	: El descriptor por el que se desea filtrar la lista
 */
Console *getConsoleByDescriptor(Int32U descriptor){

	Console *consoleClient = NULL;

	Int16U i;
	for (i = 0; i < list_size(consoleList); i++) {

		mtxLock(&mtxConsoleList);
		Console *temp = list_get(consoleList, i);
		if (temp->consoleClient->descriptor == descriptor) {
			consoleClient = temp;
			mtxUnlock(&mtxConsoleList);
			break;
		}
		mtxUnlock(&mtxConsoleList);
	}

	return consoleClient;

}
/**
 * @NAME: getCpuByDescriptor
 * @DESC: Devuelve un elemento de la lista de cpus en funcion del descriptor
 * 		: La lista de Cpus es una lista donde se guarda, el socket de
 * 		: la Cpu cliente y el tcb que esta ejecutando
 * @PARAM:
 * 	descriptor	: El descriptor por el que se desea filtrar la lista
 */
Cpu *getCpuByDescriptor(Int32U descriptor){

	Cpu *cpuClient = NULL;

	Int16U i;
	for (i = 0; i < list_size(cpuList); i++) {

		mtxLock(&mtxCpuList);
		Cpu *temp = list_get(cpuList, i);
		if (temp->cpuClient->descriptor == descriptor) {
			cpuClient = temp;
			mtxUnlock(&mtxConsoleList);
			break;
		}
		mtxUnlock(&mtxCpuList);
	}

	return cpuClient;

}
/**
 * @NAME: clientDown
 * @DESC: Determina que cliente fue el que se desconecto del sistema.
 * 		: Ademas de determinarlo, tambien realiza el proceso correspondiente
 * @PARAMS:
 *  clientDescriptor	: El descriptor de esa conexion
 */
void clientDown(Int32U clientDescriptor){

	//DETERMINAR QUE CLIENTE ES (CONSOLA O CPU)
	Console *consoleTemp = getConsoleByDescriptor(clientDescriptor);
	if(consoleTemp == NULL){
		//ES CPU
		Cpu *cpuTemp = getCpuByDescriptor(clientDescriptor);
		cpuDown(cpuTemp);

	}else
	{
		//ES CONSOLA
		consoleDown(consoleTemp);
	}
}
/**
 *@NAME	: nextTcbHandler
 *@DESC	: Funcion que lanza un hilo que toma el tcb que "vuelve"
 *		: de la Cpu y lo pone en ready. El tcb que vuelve deberia
 *		: volver porque se le acabo el quantum
 *@PARAMS:
 * tcb	: El tcb que "vuelve" de exec y que tiene que ir a ready
 */
void nextTcbHandler(Tcb tcb) {
	pthread_t thr;
	pthread_create(&thr, NULL, execToReadyProcessesHandlerThread, (void*) &tcb);
	pthread_join(thr,NULL);
}
/**
 *@NAME	: procEndHandler
 *@DESC	: Funcion que lanza un hilo que realiza la finalizacion de
 *		: un proceso
 *@PARAMS:
 * sck	: Estructura que contiene el Tcb a finalizar
 */
void procEndHandler(StrCpuKer *sck) {
	pthread_t thr;
	pthread_create(&thr, NULL, execToExitProcessesHandlerThread,(void*) &sck->tcb);
	pthread_join(thr,NULL);
}
/**
 *@NAME: cloneTcb
 *@DESC	: Retorna un Tcb igual, al ingresado
 *		: Se utiliza para cuando el ESO tiene que crear un
 *		: hijo
 *@PARAMS:
 * tcbParent	: El tcb a clonar
 */
Tcb* cloneTcb(Tcb *tcbParent) {
	Tcb* tcb = malloc(sizeof(Tcb));
	tcb->pid = tcbParent->pid;
	tcb->tid = tcbParent->tid;
	tcb->M = tcbParent->M;
	tcb->S = tcbParent->S;
	tcb->X = tcbParent->X;
	tcb->P = tcbParent->P;
	tcb->kernelMode = tcbParent->kernelMode;
	tcb->csLenght = tcbParent->csLenght;
	tcb->A = tcbParent->A;
	tcb->B = tcbParent->B;
	tcb->C = tcbParent->C;
	tcb->D = tcbParent->D;
	tcb->E = tcbParent->E;
	tcb->F = tcbParent->F;
	return tcb;
}
/**
 * @NAME: cpuClientHandler
 * @DESC: Funcion que realiza la gestion de los clientes CPU
 * @PARAMS:
 *  cpuClient	: Socket del cliente
 *  data		: Stream con los datos recibidos de ese cliente
 */
void cpuClientHandler(Socket *cpuClient, Stream data){

	StrCpuKer *sck = unserializeCpuKer(data);

	switch (sck->action) {

	case INTE:
		serviceInterrupt(sck);
		break;
	case NEXT_TCB:
		nextTcbHandler(sck->tcb);
		break;
	case STD_INPUT:
		serviceStdInput(sck->tcb.pid, sck->inputType);
		break;
	case STD_OUTPUT:
		serviceStdOutput(sck->tcb.pid, sck->log);
		break;
	case NEW_THREAD:
		serviceCreateThread(&sck->tcb);
		break;
	case JOIN_THREADS:
		serviceJoinThread(sck);
		break;
	case BLOCK_THREAD:
		serviceBlock(sck);
		break;
	case WAKE_THREAD:
		serviceWake(sck);
		break;
	case PROC_END:
		procEndHandler(sck);
		break;

	default:
		break;

	}


}

void consoleClientHandler(Socket *consoleClient, Stream data){

	StrConKer *sck = unserializeConKer(data);

	if(sck->action == STD_INPUT){

	}


}
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

	cpuList = list_create();
	consoleList = list_create();

	mtxInit(&mtxCpuList);
	mtxInit(&mtxConsoleList);
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
/**
 * @NAME: initTcbKM
 * @DESC: Realiza el proceso de inicializacion del Tcb Kernel Mode.
 * 		: Inicializa, levanta archivo de Syscalls, crea los CS y SS, y copia el codigo
 * 		: en la MSP
 * 		: Retorna TRUE si todo el proceso salio bien, FALSE si hubo algun error
 */
Boolean initTcbKM(){

	//CREA E INICIALIZA EL TCB KM
	Tcb *tcbKm;
	tcbKm = createNewTcbKM();

	//CARGA EL ARCHIVO DE SYSCALLS
	Stream stream = loadSyscalls();
	if(stream == NULL){
		return FALSE;
	}
	Int32U streamLength = strlen((char *)stream);

	//RESERVAR DE MSP SEGMENTO DE CODIGO
	Int32U csDir = getSegmentFromMSP(streamLength,tcbKm);

	//RESERVAR DE MSP SEGMENTO DE STACK
	Int32U ssDir = getSegmentFromMSP(config->stack,tcbKm);

	if(csDir != ERROR && ssDir != ERROR){

		//ESCRIBIR EL CODIGO BESO EN EL SEGMENTO CS DE LA MSP

		StrKerMsp *skm = malloc(sizeof(StrKerMsp));

		skm->action = MEM_WRITE;
		skm->pid = tcbKm->pid;
		skm->address = csDir;
		skm->size = streamLength;
		skm->data = stream;

		SocketBuffer *codeBuffer = serializeKerMsp(skm);
		socketSend(socketMsp->ptrSocket,codeBuffer);

		//ACTUALIZO LOS REGISTROS DE CS Y SS
		tcbKm->M = csDir;
		tcbKm->P = csDir;

		tcbKm->X = ssDir;
		tcbKm->S = ssDir;

	}else{

		return FALSE;

	}

	//ENCOLA EL HILO EN BLOCK
	pthread_t thr;
	pthread_create(&thr, NULL, insertTcbKernelModeInBlock, (void*)tcbKm);
	pthread_join(thr,NULL);

	return TRUE;
}
/**
 * @NAME: loadSyscalls
 * @DESC: Levanta el archivo de Syscalls y retorna una estructura Stream de lo leido
 * 		: La ruta del archivo se encuentra en el archivo de configuracion
 */
Stream loadSyscalls(){

	//rb = READ BINARY
	FILE* file = fopen(config->syscalls, "rb");

	if (!file) {
		return NULL;
	}

	Byte* read = malloc(sizeof(Byte));
	Byte* ptrBuffer;
	Stream buffer = NULL;
	Int32U size = 0;
	Int32U readed;

	while (!feof(file)) {

		readed = fread(read, sizeof(Byte), sizeof(Byte), file);

		//CARGA EL BUFFER DINAMICAMENTE
		if (readed) {
			buffer = realloc(buffer, size + readed);
			ptrBuffer = buffer + readed;
			memcpy(ptrBuffer, read, readed);
			size += readed;
		}

	}

	fclose(file);

	return buffer;

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
		Cpu *cpu = malloc(sizeof(Cpu));
		cpu->cpuClient = cpuClient;
		cpu->tcb = NULL;

		mtxLock(&mtxCpuList);
		list_add(cpuList,cpu);
		mtxUnlock(&mtxCpuList);

		//ACTIVO LA LISTA DE CPUS PARA EL PLANIFICADOR
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
	SocketBuffer *sb = serializeKerMsp(skm);
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
	SocketBuffer *sb = serializeKerCon(skc);
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
			StrKerMsp *skm = malloc(sizeof(StrKerMsp));

			skm->action = MEM_WRITE;
			skm->pid = tcb->pid;
			skm->address = csDir;
			skm->size = sck->fileContentLen;
			skm->data = sck->fileContent;

			SocketBuffer *codeBuffer = serializeKerMsp(skm);
			socketSend(socketMsp->ptrSocket,codeBuffer);

			//ACTUALIZO LOS REGISTROS DE CS Y SS
			tcb->M = csDir;
			tcb->P = csDir;

			tcb->X = ssDir;
			tcb->S = ssDir;


			//CARGO LA CONSOLA INGRESADA JUNTO CON EL TCB QUE
			//TRAJO A LA LISTA DE CONSOLAS QUE VOY A USAR PARA GESTIONAR
			//LOS SERVICIOS EXPUESTOS A LA CPU
			Console *clientConsole = malloc(sizeof(Console));

			clientConsole->consoleClient = consoleClient;
			clientConsole->tcb = tcb;

			//CALZAR UN MUTEX ACA
			list_add(consoleList,clientConsole);
			//Y ACA...

			//MUEVO EL NUEVO TCB A LA COLA DE NEW
			pthread_t thr;
			pthread_create(&thr, NULL, newProcessesHandlerThread,(void*) tcb);
			pthread_join(thr, NULL);


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
/**
 * @NAME: initServer
 * @DESC: Realiza la inicializacion del socket de escucha, y lo pone a escuchar
 * 		: Retorna TRUE si el proceso tuvo exito. FALSE si hubo errores
 */
Boolean initServer(){

	serverSocket = socketCreateServer(config->port);
	if(serverSocket == NULL){
		return FALSE;
	}

	if(!socketListen(serverSocket)){
		return FALSE;
	}

	return TRUE;

}
/**
 * @NAME: checkConections
 * @DESC: Realiza la configuracion inicial del SELECT y lo realiza periodicamente.
 * 		: Gestiona conexiones nuevas y existentes
 */
void checkConections(){

	//SETEO A "0" EL MASTER Y EL TEMPORAL
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	//CARGO EL SOCKET DE ESCUCHA A MASTER
	FD_SET(serverSocket->descriptor, &master);

	//CARGO EL SOCKET MAS GRANDE
	fdmax = serverSocket->descriptor;

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
					if (i == serverSocket->descriptor) {

						//SI, ENTONCES GESTIONO LA NUEVA CONEXION ENTRANTE
						Socket *clientSocket = socketAcceptClient(serverSocket);
						newClientHandler(clientSocket);

						//LO CARGO A LA LISTA DE DESCRIPTORES A MONITOREAR Y ACTUALIZO EL MAXIMO
						FD_SET(clientSocket->descriptor, &master);

						if (clientSocket->descriptor > fdmax) {
							fdmax = clientSocket->descriptor;
						}


					} else {

						//NO, ENTONCES GESTIONO EL SOCKET QUE HABLO...
						clientHandler(i);

					}
				}
			}
		}

	}

}
/**
 * @NAME: clientHandler
 * @DESC: Realiza la gestion de conexion de un cliente que ya se encuentra conectado
 * 		: al sistema. La funcion NO GESTIONA NUEVAS CONEXIONES, sino ya existentes
 * @PARAMS:
 * 	clientDescriptor	: El descriptor que produjo el llamado
 */
void clientHandler(Int32U clientDescriptor) {

	Socket *tempClient = malloc(sizeof(Socket));
	tempClient->descriptor = clientDescriptor;

	SocketBuffer *buffer = socketReceive(tempClient);

	if (buffer == NULL ) {

		printf("Ha ocurrido un error al intentar recibir de %d\n",
				clientDescriptor);

		//REALIZO EL PROCEDIMIENTO DE CAIDA DE UN CLIENTE
		clientDown(clientDescriptor);

		//ELIMINO EL DESCRIPTOR DEL CONJUNTO
		FD_CLR(clientDescriptor, &master);

		//CIERRO EL SOCKET
		socketDestroy(tempClient);
		free(tempClient);

	} else {

		// ** SE ATIENDEN SOLICITUDES DE CLIENTES **
		printf("El cliente %d ha enviado un paquete\n",clientDescriptor);

		Stream strReceived = (Stream)buffer->data;
		Char id = getStreamId(strReceived);

		switch (id) {

		case CPU_ID:
			cpuClientHandler(tempClient,strReceived);
			break;

		case CONSOLA_ID:
			consoleClientHandler(tempClient,strReceived);
			break;

		}


	}

}
//SERVICIOS EXPUESTOS A LA CPU
//==========================================================================
/**
 * @NAME: serviceInterrupt
 * @DESC: Recibe un TCB y una direccion de memoria de la Syscall a ejecutar
 * 		: y lo encola en estado BLOCK. Luego agregara una entrada en la cola
 * 		: de llamadas al sistema con el TCB en cuestion
 * @PARAMS:
 * 	tcb		: Estructura Tcb que solicito el syscall
 * 	address	: Direccion de memoria de la Syscall a ejecutar
 */
void serviceInterrupt(StrCpuKer *sck){

	pthread_t thr;
	pthread_create(&thr, NULL, sysCallsHandlerThread, (void*) sck);
	pthread_join(thr,NULL);
}
/**
 * @NAME: serviceStdInput
 * @DESC: Recibe un PID y un identificador de tipo. Pide a la consola del programa
 * 		: identificado por ese PID que se ingrese una cadena/texto
 * @PARAMS:
 * 	pid		: El pid que identifica a la consola
 * 	type	: El identificador de tipo de la cadena a ingresar (texto/numeros)
 */
void serviceStdInput(Int32U pid, Char type){

	Console *consoleClient = getConsoleByPid(pid);
	if(consoleClient != NULL){

		StrKerCon *skc = malloc(sizeof(StrKerCon));

		skc->action = STD_INPUT;

		switch (type){

		case TEXT_INPUT:
			skc->inputType = TEXT_INPUT;
			break;

		case NUMBER_INPUT:
			skc->inputType = NUMBER_INPUT;
			break;
		}

		SocketBuffer *sb = serializeKerCon(skc);
		socketSend(consoleClient->consoleClient,sb);

	}
}
/**
 * @NAME: serviceStdOutput
 * @DESC: Recibe un pid y una cadena de texto. Escribe en la consola del programa
 * 		: identificado por ese pid la cadena de texto recibida
 * @PARAMS:
 * 	pid		: El pid que identifica a la consola
 * 	text	: La cadena a imprimir en consola
 */
void serviceStdOutput(Int32U pid, String text){

	Console *consoleClient = getConsoleByPid(pid);
	if(consoleClient != NULL){

		StrKerCon *skc = malloc(sizeof(StrKerCon));

		skc->action = STD_OUTPUT;
		skc->logLen = strlen(text);
		skc->log = (Byte*)text;

		SocketBuffer *sb = serializeKerCon(skc);

		socketSend(consoleClient->consoleClient,sb);

	}

}
/**
 * @NAME: serviceCreateThread
 * @DESC: Recibe una estructura TCB. Crea un nuevo hilo con el TCB recibido y lo envia
 * 		: a planificar normalmente. Se le debe crear un nuevo segmento de stack en la MSP
 * 		: pero no un nuevo segmento de codigo ya que este es igual para todos los hilos de
 * 		: un mismo programa
 * @PARAMS:
 * 	tcbParent	: El tcb padre a copiar
 */
void serviceCreateThread(Tcb *tcbParent){

	Tcb *tcbChild = malloc(sizeof(Tcb));

	tcbChild = cloneTcb(tcbParent);

	//ESTO HACE FALTA???? PORQUE CREO QUE VOY A TENER
	//QUE DIFERENCIAR POR HILO TAMBIEN...VER MAS ADELANTE
	tcbChild->tid++;

	Int32U ssDir = getSegmentFromMSP(config->stack,tcbChild);
	if(ssDir != ERROR){
		tcbChild->X = ssDir;
		tcbChild->S = ssDir;
	}

	pthread_t thr;
	Int32U createResult;
	createResult = pthread_create(&thr, NULL, newProcessesHandlerThread, (void*) tcbChild);
	if (createResult != 0) {
		printf("No se ha podido iniciar el Sub Hilo MoveToNew\n");
	}
	pthread_join(thr,NULL);

}
/**
 * @NAME: serviceJoinThread
 * @DESC: Recibe un TID llamador y un TID a esperar.
 * 		: Envia al hilo identificado por el Tid llamador recibido
 * 		: al estado de Block hasta que el otro hilo del mismo
 * 		: programa identificado por el TID a esperar finalice su ejecucion
 * @PARAMS:
 * 	sck	: Estructura StrCpuKer con la informacion necesaria
 */
void serviceJoinThread(StrCpuKer *sck){
	pthread_t thr;
	pthread_create(&thr, NULL, blockTcbByJoin, (void*) sck);
	pthread_join(thr,NULL);
}
/**
 * @NAME: serviceBlock
 * @DESC: Recibe una estructura Tcb e identificador de recurso
 * 		: Envia al estado BLOCK al hilo identificado en el TCB recibido
 * 		: Ademas lo introduce al final de la cola de espera del recurso
 *		: especificado en el identificador de Recurso recibido
 * @PARAMS:
 * 	sck	: Estructura StrCpuKer con la informacion necesaria
 */
void serviceBlock(StrCpuKer *sck){
	pthread_t thr;
	pthread_create(&thr, NULL, blockTcbByResource, (void*) sck);
	pthread_join(thr,NULL);
}
/**
 * @NAME: serviceWake
 * @DESC: Recibe un identificador de recurso
 * 		: Remueve el primer hilo de la cola de espera del recurso
 * 		: especificado por el identificador de recurso, ademas lo remueve
 *		: del estado BLOCK y lo coloca al final de la cola READY para que
 *		: vuelva a ser palnificado
 * @PARAMS:
 * 	sck	: Estructura StrCpuKer con la informacion necesaria
 */
void serviceWake(StrCpuKer *sck){
	pthread_t thr;
	pthread_create(&thr, NULL, wakeTcbByResource, (void*) sck);
	pthread_join(thr,NULL);
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

	//CREO EL TCB KM
	if(!initTcbKM()){
		printf("No se ha podido iniciar el Tcb Kernel Mode. El programa terminara\n");
		return -1;
	}

	//GENERO EL SERVER Y LO PONGO A ESCUCHAR
	if(!initServer()){
		printf("No se ha podido iniciar el Socket de escucha. El programa terminara\n");
		return -1;
	}

	//REALIZO EL SELECT PARA GESTIONAR CONEXIONES
	checkConections();


	//FINALIZO EL PROGRAMA
	pthread_join(thrScheduler, NULL );

	socketDestroy(serverSocket);
	free(serverSocket);


	return 0;
}

