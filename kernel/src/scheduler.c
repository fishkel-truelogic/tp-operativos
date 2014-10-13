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
t_list *syscallList;
t_dictionary *tcbsWaitingForTidDic;
t_dictionary *tcbsWaitingForResourceDic;
t_dictionary *amountOfTidForPidDic;
//==========================================================================

//FUNCIONES
//==========================================================================
void *newProcessesHandlerThread(void *ptr);
bool isTCBKernelMode(void *ptr);
void *readyProcessesHandlerThread(void *ptr);
//void *thrSchedulerHandler(void *ptr);

//==========================================================================


//HILO ENCARGADO DE INGRESAR NUEVOS TCB A NEW
void *newProcessesHandlerThread (void *ptr){
	Tcb *tcb;
	tcb = (Tcb *) ptr;
	//PRODUCTOR NEWLIST
	//MUTEX NEWLIST
	list_add(newList, tcb);
	//END MUTEX
	return NULL;
}

//HILO ENCARGADO DE PASAR TODOS LOS PROCESO/HILOS DE LA LISTA DE NUEVOS A READY
void *newToReadyProcessesHandlerThread(void *ptr) {

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
bool isTCBKernelMode(void *ptr){

	Tcb *tcb;
	tcb = (Tcb *) ptr;
	return tcb->kernelMode;
}




//HILO ENCARGADO DE PASAR TODOS LOS PROCESO/HILOS DE LA LISTA DE READY A EXEC
//DISPATCHER
void *readyToExecProcessesHandlerThread(void *ptr) {

	while (TRUE) {
		//CONSUMIDOR READYLIST
		//SEMAFORO CANTIDAD DE CPUS LIBRES
		//tener en cuenta bajar lo antes posible la cantidad de cpu disponible
		//PRODUCTOR  EXCECDIC
		//MUTEX READYLIST

		Tcb tcbToExec;
		//VERIFICO SI DENTRO DE LA LISTA DE READY ESTA EL TCB KERNEL
		//SI ESTA ENVIO ESE A EJECUTAR
		if(list_any_satisfy(readyList, isTCBKernelMode)){

			tcbToExec = list_remove_by_condition(readyList , isTCBKernelMode);
		}else{
			//SI NO ESTA EL TCB KERNEL TOMO EL PRIMERO DE LA LISTA Y LO ENVIO A EJECUTAR
			tcbToExec = list_remove(readyList,0);
		}
		//END MUTEX READYLIST

		//MUTEX EXCECDIC

		list_add(execList, tcbToExec);
		//END MUTEX EXCECDIC


		//TODO AVISAR A CESAR PARA QUE IMPLEMENTE ESTA FUNCION
		sendTcbToAFreeCPU(tcbToExec);

	}

	printf("Se callo hilo readyProcessesHandlerThread \n");

	return NULL ;

}

Boolean removeFromExecList(Tcb* tcb) {
	//MUTEX EXECLIST
	Boolean tcbFound = FALSE;
	Int16U i;
	for (i = 0; i < list_size(execList); i++) {
		Tcb* temp = list_get(execList, i);
		if (temp->tid == tcb->tid) {
			tcbFound = TRUE;
			list_remove(execList, i);
			break;
		}
	}
	return tcbFound;
}

//HILO ENCARGADO DE ENVIAR UN TCB QUE FINALIZO SU QUANTUM
//PERO DEBE SEGUIR EJECUTANDOSE
// EXEC A READY
void *execToReadyProcessesHandlerThread (void *ptr){

	Tcb *tcb;
	tcb = (Tcb *) ptr;
	//CONSUMIDOR EXECLIST
	//PRODUCTOR READYLIST
	//AUMENTAR SEMAFORO DE CPU DISPONIBLES
	//MUTEX EXECLIST
	if(removeFromExecList(tcb)){
		//END MUTEX

		//MUTEX READYLIST
		list_add(readyList, tcb);
		//END MUTEX
	} else {
		list_add(exitList, tcb);
	}
	return NULL;
}

//HILO ENCARGADO DE ENVIAR A EXIT UN PROCESO QUE FINALIZO
//SE PUEDE USAR PARA CUANDO CONSOLA O CPU ENVIAN FALLO
//EXEC A EXIT
void *execToExitProcessesHandlerThread (void *ptr){

	Tcb *tcb;
	tcb = (Tcb *) ptr;
	//CONSUMIDOR EXECLIST
	//MUTEX EXECLIST
	if(removeFromExecList(tcb)){
		//END MUTEX

		//MUTEX EXITLIST
		list_add(exitList, tcb);

		//TODO if (cant de tid por pid > 1){
		//	releaseResourceThread(tcb);
		//} else {
		//releaseResourceProcess(tcb);
		//}

		//END MUTEX
	}else {
		list_add(exitList, tcb);
	}
	return NULL;
}

//HILO ENCARGADO DE ENVIAR A BLOCK UN TCB POR OTRO MOTIVO DISTINTO A SYSCALL
//EXEC A NORMAL BLOCK
void *execToNormalBlockProcessesHandlerThread (void *ptr){

	Tcb *tcb;
	tcb = (Tcb *) ptr;
	//CONSUMIDOR EXECLIST
	//PRODUCTOR BLOCKLIST
	//AUMENTAR SEMAFORO DE CPU DISPONIBLES
	//MUTEX EXECLIST
	if(removeFromExecList(tcb)){
		//END MUTEX

		//MUTEX BLOCKLIST
		list_add(blockList, tcb);
		//END MUTEX
	}else {
		list_add(exitList, tcb);
	}
	return NULL;
}

//*****************************************************************************

Boolean removeTcbsFromList(Tcb* tcb, t_list* listToSeekAndRemove) {
	Boolean tcbFound = FALSE;
	Int16U i;
	for (i = 0; i < list_size(listToSeekAndRemove); i++) {
		Tcb* temp = list_get(listToSeekAndRemove, i);
		if (temp->pid == tcb->pid) {
			tcbFound = TRUE;
			list_add(exitList,(list_remove(listToSeekAndRemove, i)));
			break;
		}
	}
	return tcbFound;
}

void *seekAndDestroyPid(void *ptr){

	Tcb *tcb;
	tcb = (Tcb *) ptr;
	//MUTEX TODAS LAS COLAS
	removeTcbsFromList(tcb, execList);
	//END MUTEX EXEC
	removeTcbsFromList(tcb, readyList);
	//END MUTEX READY
	removeTcbsFromList(tcb, syscallList);
	//END MUTEX SYSCALL
	removeTcbsFromList(tcb, blockList);
	//END MUTEX BLOCK
	removeTcbsFromList(tcb, newList);
	//END MUTEX NEW

	return NULL;
}

//CUANDO ME TIRARON SYSCAL Y LO TENGO QUE BLOQUIAR Y USAR LA ADRESS PARA SYSCALL
void *sysCallsHandlerThread (void *ptr){
	StrCpuKer *strCpuKer;
	strCpuKer = (strCpuKer *) ptr;
	Tcb tcb = strCpuKer->tcb;
	Int32U address = strCpuKer->address; //interruption

	return NULL;
}

//PARA CUANDO SE HACE UN JOIN
void *blockTcbByJoin (void *ptr){
	StrCpuKer *strCpuKer;
	strCpuKer = (strCpuKer *) ptr;
	Tcb tcb = strCpuKer->tcb;
	Int32U tid = strCpuKer->tid; //es el que tengo que esperar que termine
	//HACER DICCIONARIO CON LA KEY: TID
	// VALUE: TCB
	//EL TCB LO BLOQUEO Y SE DESBLOQUEA CUANDO EL TID LLEGA A LA COLA DE EXIT
	return NULL;
}

//PARA CUANDO SE HACE BLOQUE ALGO POR RECURSO
void *blockTcbByResource (void *ptr){
	StrCpuKer *strCpuKer;
	strCpuKer = (strCpuKer *) ptr;
	Tcb tcb = strCpuKer->tcb;
	Int32U tid = strCpuKer->resource; //es el que tengo que esperar que se liber
	//HACER DICCIONARIO CON LA KEY: resource
	// VALUE: LISTADO DE TCBS
	//EL TCB LO BLOQUEO Y SE DESBLOQUEA CUANDO EL RESOURCE SE LIBERA
	return NULL;
}

//PARA CUANDO SE HACE UN JOIN MANDAR A READY
void wakeTcbByJoin (Int32U tid){
	//USO EL TID PARA BUSCAR SI HAY UN TCB QUE DESBLOQUEAR Y MANDAR A READY
	//RELACIONADO CON EL TID QUE TERMINO.
	//USAR DICCIONARIO CON LA KEY: TID
	// VALUE: TCB

}

//PARA CUANDO SE LIBER RECURSO MANDAR A READY
void *wakeTcbByResource (void *ptr){
	StrCpuKer *strCpuKer;
	strCpuKer = (strCpuKer *) ptr;
	Tcb tcb = strCpuKer->tcb;
	Int32U tid = strCpuKer->resource; //es el que tengo que esperar que se liber
	//BUSCAR EN EL DICCIONARIO EL PRIMER TCB ASOCIADO A ESE RESOURCE
	// PASAR EL TCB DE BLOCK A READY
	return NULL;
}


//INGRESA POR PRIMERA VEZ EL TCB KERNEL MODE EN LA LISTA DE BLOCK
void *insertTcbKernelModeInBlock (void *ptr){
	Tcb *tcb;
	tcb = (Tcb *) ptr;

	return NULL;
}



/*void *thrSchedulerHandler(void *ptr) {
	printf("Planificador BPRR iniciado\n");
	return NULL ;

}*/
