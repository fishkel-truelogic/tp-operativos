/*
 * scheduler.c
 *
 *  Created on: 28/09/2014
 *      Author: Ignacio Doring
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <src/commons/config.h>
#include <src/commons/collections/list.h>
#include <src/fereSockets.h>
#include <src/fereStream.h>
#include <src/fereTypes.h>

t_list *newList; //newList
t_list *readyList;
t_list *execList;
t_list *blockList;
t_list *exitList;

void *newProcessesHandlerThread(void *ptr);

//HILO ENCARGADO DE PASAR TODOS LOS PROCESO/HILOS DE LA LISTA DE NUEVOS A READY
void *newProcessesHandlerThread(void *ptr) {

	while (TRUE) {
		//CONSUMIDOR NEWLIST
		//PRODUCTOR  REDYLIST
		//MUTEX READYLIST Y NEWLIST
		list_add_all(readyList, newList); //AGREGA TODOS LOS PROCESOS DE NEW A READY
		//END MUTEX READYLIST
		list_clean(newList);
		//END MUTEX NEWLIST

	}

	printf("Se callo hilo newProcessesHandler\n");

	return NULL ;

}

void *thrSchedulerHandler(void *ptr) {
	printf("Planificador BPRR iniciado\n");
	return NULL ;

}
