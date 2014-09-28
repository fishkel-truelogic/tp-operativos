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
#include <src/commons/collections/queue.h>
#include <src/fereSockets.h>
#include <src/fereStream.h>

//CONSTANTES Y DEFINES
//==========================================================================
#define CONFIG_FILE "config.txt"
#define PARAM_LENGTH 6

#define PORT_ATRIB 		"PUERTO"
#define IP_MSP_ATRIB 	"IP_MSP"
#define PORT_MSP_ATRIB	"PUERTO_MSP"
#define QUANTUM_ATRIB 	"QUANTUM"
#define SYSCALLS_ATRIB 	"SYSCALLS"
#define STACK_ATRIB		"STACK"

#define TITLE "** Kernel V2.0 **"

#define KERNEL_MODE	'K'
#define USER_MODE 	'U'


//VARIABLES GLOBALES Y ESTRUCTURAS PROPIAS
//==========================================================================
typedef struct strKernelConfig {

	Int32U port;
	String mspAddress;
	Int32U mspPort;
	Int8U quantum;
	String syscalls;
	Int32U stack;

} KernelConfig;


//LA PRIMER Y SEGUNDA COLA SE PUEDEN ELIMINAR...LA AGREGO POR SI LAS DUDAS
//PERO YO NO SE SI EL SO TIENE QUE GESTIONAR LOS PROCESOS UNA VEZ TERMINADOS
//IDEM PARA CUANDO RECIEN SE CREAN
t_queue *newQueue;
t_queue *readyQueue;
t_queue *execQueue;
t_queue *blockQueue;
t_queue *exitQueue;

KernelConfig *config;
SocketClient *ptrSocketMsp;

Int32U lastPid;
Tcb *tcbKm;

pthread_t thrScheduler;


//PROTOTIPOS Y FUNCIONES
//==========================================================================
void printHeader() {

	printf("\n");
	printf(TITLE);
	printf("\n");

}

void init()
{
	printHeader();

	lastPid = 2; //PORQUE PID 1 ES EL TCB KERNEL MODE
	config = malloc(sizeof(KernelConfig));

	newQueue = queue_create();
	readyQueue = queue_create();
	execQueue = queue_create();
	blockQueue = queue_create();
	exitQueue = queue_create();
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

		//ACA SE TIENE QUE ATENDER PEDIDOS DE LAS CONSOLAS

		//PRIMRO SE TIENE QUE PEDIR EL ID PARA DETERMINAR SI ES CONSOLA
		//O CPU, SEGUN ESO, DESERIALIZO
		StrConKer *sck = unserializeConKer((Stream)ptrBuffer->data);

		//DESPUES TENGO QUE DETERMINAR QUE ACCION ES

		printf("%d: %s", clientDescriptor, ptrBuffer->data);

	}

}

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

Tcb* createTcb(Char mode){


	Tcb *tcb = malloc(sizeof(Tcb));

	if(mode == KERNEL_MODE){
		tcb->pid = 1;
		tcb->kernelMode = TRUE;
	}else{
		tcb->pid = lastPid;
		tcb->kernelMode = FALSE;

		//SINCRONIZAR BIEN ESTO!!!!!!
		//ESTA FUNCION PUEDE SER ACCEDIDA POR MUCHAS CONSOLAS A LA VEZ Y
		//lastPid ES VARIABLE GLOBAL
		//CLAVAR MUTEX!!!!

		lastPid++;
	}
	tcb->tid = 1; //COMO MINIMO, TENDREMOS UN PROCESO MONOHILO
	tcb->M = 0;
	tcb->csLenght = 0;
	tcb->P = 0;
	tcb->X = 0;
	tcb->S = 0;
	tcb->A = 0;
	tcb->B = 0;
	tcb->C = 0;
	tcb->D = 0;
	tcb->E = 0;
	tcb->F = 0;

	return tcb;

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
	ptrSocketMsp = socketCreateClient();
	if (!socketConnect(ptrSocketMsp, config->mspAddress, config->mspPort)) {

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
	tcbKm = createTcb(KERNEL_MODE);
	queue_push(blockQueue,tcbKm);

	//TODO: COMUNICARME CON MSP Y CARGAR EL SYSCALL DEL TCBKM Y SU STACK


	//TODO: LLEVAR ESTO A OTRO ARCHIVO....
	Socket *ptrServerSocket = socketCreateServer(config->port);
	socketListen(ptrServerSocket);

	//CONJUNTO MAESTRO DE DESCRIPTORES
	fd_set master;

	//CONJUNTO TEMPORAL DE DESCRIPTORES
	fd_set read_fds;

	Int32U fdmax, i;

	//SETEO A "0" EL MASTER Y EL TEMPORAL
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	//CARGO EL SOCKET DE ESCUCHA A MASTER
	FD_SET(ptrServerSocket->descriptor, &master);

	//CARGO EL SOCKET MAS GRANDE
	fdmax = ptrServerSocket->descriptor;

	struct timeval timeout;
	Int32U selectTimeout = 0;

	//BUCLE PRINCIPAL
	while (1) {

		read_fds = master;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;

		//HAGO EL SELECT
		Int32U selectResult = select(fdmax + 1, &read_fds, NULL, NULL,
				&timeout);

		if (selectResult == -1) {

			//SE CAYO MI SOCKET DE ESCUCHA
			printf("Error en el socket de escucha\n");
			break;


		} else if (selectResult == 0) { //VER SI ESTO DESPUES LO SEGUIMOS NECESITANDO

			//SALE POR TIMEOUT
			selectTimeout++;

		} else {

			//PASO ALGO POSTA
			selectTimeout = 0;

			//RECORRO TODOS LOS DESCRIPTORES MONITOREADOS PARA VER QUIEN LLAMO
			for (i = 0; i <= fdmax; i++) {

				if (FD_ISSET(i, &read_fds)) {

					//FUE EL SOCKET DE ESCUCHA??
					if (i == ptrServerSocket->descriptor) {

						//SI, CREO UN NUEVO SOCKET
						Socket *ptrClientSocket = socketAcceptClient(
								ptrServerSocket);

						//LO CARGO A LA LISTA DE DESCRIPTORES A MONITOREAR
						FD_SET(ptrClientSocket->descriptor, &master);

						if (ptrClientSocket->descriptor > fdmax) {
							fdmax = ptrClientSocket->descriptor;
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

