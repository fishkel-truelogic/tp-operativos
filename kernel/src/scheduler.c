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

//VARIABLES GLOBALES
//==========================================================================
t_list *newList; //newList
t_list *readyList;
t_list *execList;
t_list *blockList;
t_list *exitList;
//==========================================================================

//FUNCIONES
//==========================================================================
void *newProcessesHandlerThread(void *ptr);
Boolean isTCBKernelMode(void *ptr);
void *readyProcessesHandlerThread(void *ptr);
//void *thrSchedulerHandler(void *ptr);

//==========================================================================


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

	printf("Se callo hilo newProcessesHandlerThread \n");

	return NULL ;

}

//FUNCION PARA SABER SI UN TCB ES KERNEL MODE
Boolean isTCBKernelMode(void *ptr){

	Tcb *tcb;
	tcb = (Tcb *) ptr;
	return tcb->kernelMode;
}

//HILO ENCARGADO DE PASAR TODOS LOS PROCESO/HILOS DE LA LISTA DE READY A EXEC
//DISPATCHER
void *readyProcessesHandlerThread(void *ptr) {

	while (TRUE) {
		//CONSUMIDOR READYLIST
		//SEMAFORO CANTIDAD DE CPUS LIBRES
		//tener en cuenta bajar lo antes posible la cantidad de cpu disponible
		//PRODUCTOR  EXECLIST
		//MUTEX READYLIST

		/*Tcb tcbToExec = NULL;

		if(list_any_satisfy(readyList, isTCBKernelMode)){

			tcbToExec = list_remove_by_condition(readyList , isTCBKernelMode);
		}else{

			tcbToExec = list_remove(readyList,0);
		}
		//END MUTEX READYLIST

		//MUTEX EXCECLIST
		list_add(execList, tcbToExec);
		//END MUTEX EXECLIST
		sendTcbToAFreeCPU(tcbToExec);*/

	}

	printf("Se callo hilo readyProcessesHandlerThread \n");

	return NULL ;

}

/*void *thrSchedulerHandler(void *ptr) {
	printf("Planificador BPRR iniciado\n");
	return NULL ;

}*/
