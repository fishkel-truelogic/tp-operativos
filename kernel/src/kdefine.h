/*
 * kdefine.h
 *
 *  Created on: 28/09/2014
 *      Author: utnso
 */

#ifndef KDEFINE_H_
#define KDEFINE_H_

//DEFINES Y TADS
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

typedef struct cpu{

	Socket *cpuClient;
	Tcb *tcb;
}Cpu;

//PROTOTIPOS
//==========================================================================

//INICIO
void printHeader();
void init();
Boolean loadConfig();
Boolean initTcbKM();
Boolean initServer();
Stream loadSyscalls();

//NUEVOS CLIENTES
void newClientHandler(Socket *);
void newCpuClient(Socket *,Stream);
void newConsoleClient(Socket *, Stream);
Int32U getSegmentFromMSP(Int16U,Tcb *);
void creationError(Socket *);

//MANEJO DE TCBS
Tcb *createNewTcb(StrConKer *);
Tcb *createNewTcbKM();

//SERVER
void checkConections();
void clientHandler(Int32U);
void cpuClientHandler(Socket *, Stream);
void consoleClientHandler(Socket *, Stream);

//MANEJO DE CLIENTES
Console *getConsoleByPid(Int32U);
Console *getConsoleByDescriptor(Int32U);
Cpu *getCpuByDescriptor(Int32U);
void cpuDown(Cpu *);
void consoleDown(Console *);
void clientDown(Int32U);
void consoleCpuDown(Console*);

//SERVICIOS EXPUESTOS A CPU
void serviceInterrupt(StrCpuKer *sck);
void serviceStdInput(Int32U, Char);
void serviceStdOutput(Int32U, String);
void serviceCreateThread(Tcb *);
void serviceJoinThread(Int32U, Int32U);
void serviceBlock(Tcb *, Int32U);
void serviceWake(Int32U);

//SERVICIOS GENERALES
void nextTcbHandler(Tcb);
void joinThreadsHandler(StrCpuKer *);
void procEndHandler(StrCpuKer *);

//HILOS
void *thrSchedulerHandler(void *);


Tcb* cloneTcb(Tcb *);


#endif /* KDEFINE_H_ */
