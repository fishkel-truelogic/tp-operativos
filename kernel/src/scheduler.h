/*
 * scheduler.h
 *
 *  Created on: 12/10/2014
 *      Author: utnso
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

void *newProcessesHandlerThread(void *tcb);
void *sysCallsHandlerThread(void *sck);
void *execToReadyProcessesHandlerThread(void *tcb);
void *blockTcbByJoin(void *sck);
void *blockTcbByResource(void *sck);
void *wakeTcbByResource(void *sck);
void *execToNormalBlockProcessesHandlerThread(void *tcb);
void *seekAndDestroyPid(void *tcb);
void *execToExitProcessesHandlerThread(void *sck);
void *insertTcbKernelModeInBlock(void *tcb);
void *thrSchedulerHandler(void *ptr);


#endif /* SCHEDULER_H_ */
