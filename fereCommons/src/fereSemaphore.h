/*
 * fereSemaphore.h
 *
 *  Created on: 26/09/2014
 *      Author: utnso
 */

#ifndef FERESEMAPHORE_H_
#define FERESEMAPHORE_H_

	#include <pthread.h>
	#include <time.h>
	#include <semaphore.h>

	#include "fereTypes.h"

	Int32U mtxInit(pthread_mutex_t* mutex);
	void mtxDestroy(pthread_mutex_t* mutex);
	void mtxLock(pthread_mutex_t* mutex);
	void mtxUnlock(pthread_mutex_t* mutex);
	Int32U semInit(sem_t* semCounter, Int32S initialValue);
	void semDestroy(sem_t* semCounter);
	void semWait(sem_t* semCounter);
	void semSignal(sem_t* semCounter);


#endif /* FERESEMAPHORE_H_ */
