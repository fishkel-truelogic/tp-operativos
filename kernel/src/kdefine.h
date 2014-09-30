/*
 * kdefine.h
 *
 *  Created on: 28/09/2014
 *      Author: utnso
 */

#ifndef KDEFINE_H_
#define KDEFINE_H_

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

//DEFINE TEMPORAL, CUANDO ESTE HECHO EN EL STREAM.H, SACARLO
#define ERROR -1
#define ABORT -1

void printHeader();
void init();
Boolean loadConfig();
Boolean initTcbKM();
Boolean initServer();
Stream loadSyscalls();
void newCpuClient(Socket *,Stream);
Tcb *createNewTcb(StrConKer *);
Tcb *createNewTcbKM();
Int32U getSegmentFromMSP(Int16U,Tcb *);
void creationError(Socket *);
void newConsoleClient(Socket *, Stream);
void newClientHandler(Socket *);

void checkConections();

void clientHandler(Int32U, fd_set *);

void *thrSchedulerHandler(void *);

//VER DE PONER ESTO EN LAS COMMONS
SocketBuffer *barrayToBuffer(t_bitarray *);

#endif /* KDEFINE_H_ */
