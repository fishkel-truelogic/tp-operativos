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

typedef struct strKernelConfig {

	Int32U port;
	String mspAddress;
	Int32U mspPort;
	Int8U quantum;
	String syscalls;
	Int16U stack;

} KernelConfig;

typedef struct console {

	Socket *consoleClient;
	Tcb *tcb;
} Console;

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
Console *getConsoleByPid(Int32U);
void checkConections();
void clientHandler(Int32U);
void cpuClientHandler();
void consoleClientHandler(Socket *, Stream);

void serviceInterrupt();
void serviceStdInput(Int32U, Char);
void serviceStdOutput(Int32U, String);
void serviceCreateThread(Tcb *);
void serviceJoinThread(Int32U, Int32U);
void serviceBlock(Tcb *, Int32U);
void serviceWake(Int32U);

void *thrSchedulerHandler(void *);

//VER DE PONER ESTO EN LAS COMMONS
SocketBuffer *barrayToBuffer(t_bitarray *);


#endif /* KDEFINE_H_ */
