/*
 * fereStream.h
 *
 *  Created on: 21/09/2014
 *      Author: utnso
 */


#ifndef FERESTREAM_H_
#define FERESTREAM_H_

//============================================================
 /**
  * Dependencies
  */
#include "fereTcb.h"
#include "fereTypes.h"
#include "fereSockets.h"
#include "commons/bitarray.h"

 //============================================================
 /**
  * Actions
  */
#define NEXT_TCB 0          // cpu a kernel para siguiente TCB ready
#define FIRST_TCB 1         // cpu a kernel para que le de el primer TCB
#define INTE 2              // llamada a una interrupcion
#define STD_INPUT 3         // cpu a kernel, kernel a consola y consola a kernel para ingreso por teclado
#define STD_OUTPUT 4        // cpu a kernel y kernel a consola para logueo
#define NEW_THREAD 5        // syscall para crear nuevo thread
#define JOIN_THREADS 6      // syscall para bloquear proceso hasta que termine su fork
#define BLOCK_THREAD 7      // syscall para bloquear thread
#define WAKE_THREAD 8       // syscall para despertar thread bloqueado
#define NEXT_INSTRUCTION 9  // cpu le pide instruccion a la msp
#define BESO_FILE 10        // cuando la consola le manda el contenido del archivo BESO
#define MEM_READ 11			// cuando la CPU lee la MSP
#define MEM_WRITE 12 		// cuadno la CPU escribe en la MSP
#define MEM_ALLOC 13 		// cuadno el Kernel solicita memoria de la MSP
#define SEG_FAULT 14		// cuadno ocurre segmentation fault en la MSP
#define CREATE_SEG 15 		// la MSP debe crear un segmento
#define DELETE_SEG 16		// la MSP debe destruir un segmento
#define PROC_END 17			// Ocurrio un XXXX en la ejecucion de un TCB

 //============================================================
 /**
  * IDs (Dueño del stream)
  */
#define CONSOLA_ID 0
#define KERNEL_ID 1
#define CPU_ID 2
#define MSP_ID 3

#define NUMBER_INPUT 0
#define TEXT_INPUT 1


//============================================================
 /**
  * Estructuras de Streaming
  */

typedef Byte* Stream;

typedef struct strConKer { 
	Char id;
	Char action;
	Byte* fileContent;
	Int16U fileContentLen;
	Byte* bufferWriter;
	Int16U bufferWriterLen;
} StrConKer;

typedef struct strKerCpu { 
	Tcb tcb;
	Int8U quantum;
	Byte *bufferWriter;
	Char action;
	Char inputType;
} StrKerCpu;

typedef struct strCpuKer { 
	Char id;
	Tcb tcb;
	Char action;
	Int16U logLen;
	String log;
	Int32U address; //interruption
	Int32U tid; // join
	Char inputType;
	Char resource;
} StrCpuKer;

typedef struct strCpuMsp { 
	Char id;
	Int32U address;
	Char action;
	Int16U dataLen;
	Byte* data;
	Int32U pid;
} StrCpuMsp;

typedef struct strKerMsp { 
	Char id;
	Char action;
	Byte *data;
	Int16U size;
	Int32U pid;
	Int32U address;
} StrKerMsp;

typedef struct strMspKer { 
	Char id;
	Char action;
	Int32U address;
} StrMspKer;

typedef struct strMspCpu {
	Char action;
	Int16U dataLen;
	Byte * data;
} StrMspCpu;


typedef struct strKerCon {
	Char action;
	Char inputType; 
	Int32U logLen;
	Byte *log;
} StrKerCon;

//==============================================//
/**
 * Constructores
 */

StrConKer* newStrConKer(Char, Byte*, Byte*, Char, Int16U, Int16U);

StrCpuMsp* newStrCpuMsp(Char, Int32U, Char, Byte*, Int16U, Int32U);
StrCpuKer* newStrCpuKer(Char, Tcb, Char, Int16U, String, Int32U, Int32U, Char, Char);

StrKerCpu* newStrKerCpu(Tcb, Int8U,Byte*,Char,Char);
StrKerMsp* newStrKerMsp(Char, Byte*, Char, Int16U, Int32U, Int32U);
StrKerCon* newStrKerCon(Int32U, Byte*, Char, Char);

StrMspCpu* newStrMspCpu(Int16U, Byte*, Char);
StrMspKer* newStrMspKer(Char, Int32U, Char);


//==============================================//
/**
 * serialize
 */

SocketBuffer* serializeConKer(StrConKer*);

SocketBuffer* serializeCpuMsp(StrCpuMsp*);
SocketBuffer* serializeCpuKer(StrCpuKer*);

SocketBuffer* serializeMspCpu(StrMspCpu*);
SocketBuffer* serializeMspKer(StrMspKer*);

SocketBuffer* serializeKerMsp(StrKerMsp*);
SocketBuffer* serializeKerCon(StrKerCon*);

SocketBuffer* serializeKerCpu(StrKerCpu*);

//==============================================//
/**
 * Unserialize
 */

StrConKer* unserializeConKer(Stream);

StrCpuMsp* unserializeCpuMsp(Stream);
StrCpuKer* unserializeCpuKer(Stream);

StrMspKer* unserializeMspKer(Stream);
StrMspCpu* unserializeMspCpu(Stream);

StrKerMsp* unserializeKerMsp(Stream);
StrKerCon* unserializeKerCon(Stream);

StrKerCpu* unserializeKerCpu(Stream);

//==============================================//
/**
 * Sizes
 */
Int16U getSizeStrCpuKer(StrCpuKer* sck);
Int16U getSizeStrCpuMsp(StrCpuMsp* scm);

Int16U getSizeStrConKer(StrConKer* sck);

Int16U getSizeStrMspCpu(StrMspCpu* smp);

Int16U getSizeStrKerMsp(StrKerMsp* skm);
Int16U getSizeStrKerCon(StrKerCon* skc);

//==============================================//
/**
 * Handshake
 */
Char getStreamId(Stream);

//==============================================//
/**
 * bitarrayToSocketBuffer
 */
SocketBuffer* bitarrayToSocketBuffer(t_bitarray*);

#endif /* FERESTREAM_H_ */
