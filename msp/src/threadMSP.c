/*
 * threadMSP.c
 *
 *  Created on: 28/09/2014
 *      Author: Ignacio Doring
 */

#include <stdio.h>

#include <pthread.h>

#include <stdlib.h>

#include <string.h>

#include <src/fereTypes.h>

#include <src/fereSockets.h>

#include <src/commons/collections/list.h>

int p = 5;

t_list *threadDescriptorsList = NULL;

void *newConnectionThreadHandler(void *ptr) {

	printf("Nueva Conexion aceptada!!\n");

	Socket *socketClient;

	socketClient = (Socket *) ptr;

	SocketBuffer *ptrBuffer;

	//SI SE CAE UNA CONEXIO, RECIBE NULL...

	while ((ptrBuffer = socketReceive(socketClient)) != NULL ) {

		printf("Se recibio de %d : %s\n", socketClient->descriptor,
				ptrBuffer->data);

	}

	printf("Se callo la conexion a %d\n", socketClient->descriptor);

	//aca hacer lo que tenga que hacer la MSP

	void *result = NULL;

	return result;

}

int main() {

	threadDescriptorsList = list_create();

	Socket *connectioCreaterSocket = socketCreateServer(5490);

	socketListen(connectioCreaterSocket);

	printf("Escuchando conexiones\n");

	Socket *newSocketConnection;

	while ((newSocketConnection = socketAcceptClient(connectioCreaterSocket))) {

		pthread_t newConnectionthread;

		pthread_create(&newConnectionthread, NULL, newConnectionThreadHandler,
				(void*) newSocketConnection);

		list_add(threadDescriptorsList, &newConnectionthread);

	}

	int i;

	for (i = 0; i < list_size(threadDescriptorsList); i++) {

		pthread_t *temp = list_get(threadDescriptorsList, i);

		pthread_join(*temp, NULL );

	}

	return 0;

}
