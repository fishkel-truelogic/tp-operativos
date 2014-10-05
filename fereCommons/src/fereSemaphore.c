/*
 * fereSemaphore.c
 *
 *  Created on: 26/09/2014
 *      Author: utnso
 */


//#include <semaphore.h>
#include "fereSemaphore.h"

/*
 * @NAME: mtxInit
 * @DESC: Inicializa un mutex dinámicamente y devuelve 0 si lo creo correctamente.
 * @PARAMS:
 *		mutex	: Puntero del mutex creado.
 */
Int32U mtxInit(pthread_mutex_t* mutex){
	 Int32U result = pthread_mutex_init(mutex, NULL);
	 return result;
}

/*
 * @NAME: mtxDestroy
 * @DESC: Destruye un mutex. No podra ser nuevamente inicializado.
 * @PARAMS:
 *		mutex	: Puntero del mutex creado.
 */
void mtxDestroy(pthread_mutex_t* mutex){
	pthread_mutex_destroy( mutex );
}

/*
 * @NAME: mtxLock
 * @DESC: Bloquea el mutex.  Si el mutex ya estaba bloqueado, el thread
 * se bloquea hasta que el mutex vuelva a estar disponible.
 * @PARAMS:
 *		mutex	: Puntero del mutex creado.
 */
void mtxLock(pthread_mutex_t* mutex){
	pthread_mutex_lock( mutex );

}

/*
 * @NAME: mtxUnlock
 * @DESC: Desbloquea el mutex, dejandolo disponible.
 * @PARAMS:
 *		mutex	: Puntero del mutex creado.
 */
void mtxUnlock(pthread_mutex_t* mutex){
	pthread_mutex_unlock( mutex );

}

/*
 * @NAME: semInit
 * @DESC: Inicializa un semaforo dinámicamente y devuelve 0 si lo creo correctamente.
 * @PARAMS:
 *		semCounter 		: Puntero del semáforo contador creado.
 *		initialValue	: Valor inicial con que comienza el semáforo.
 */
Int32U semInit(sem_t* semCounter, Int32S initialValue){
	 int result = sem_init( semCounter, 0, initialValue);
	 return result;
}

/*
 * @NAME: semDestroy
 * @DESC: Destruye un semaforo contador, liberando sus recursos.
 * No podra ser nuevamente inicializado. Ningun thread debe estar
 * esperando al semáforo despues que es destruido.
 * @PARAMS:
 *		semCounter	: Puntero del semáforo contador creado.
 */
void semDestroy(sem_t* semCounter){
	sem_destroy( semCounter );
}

/*
 * @NAME: semWait
 * @DESC: Bloquea el semáforo contador, decrementandolo en 1;
 * suspendiendo a los threads hasta que se incremente con semSignal
 * @PARAMS:
 *		semCounter	: Puntero del semáforo contador creado.
 */
void semWait(sem_t* semCounter){
	sem_wait( semCounter );
}

/*
 * @NAME: semSignal
 * @DESC: Desbloquea el semáforo contador, permitiendo desbloquear
 * los threads que estén esperando a causa de él.
 * @PARAMS:
 *		semCounter	: Puntero del semáforo contador creado.
 */
void semSignal(sem_t* semCounter){
	sem_post( semCounter );
}
