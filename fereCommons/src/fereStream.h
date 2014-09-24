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
#include "fereTypes.h"
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

 //============================================================
 /**
  * IDs (Dueño del stream)
  */
#define CONSOLA_ID = 0
#define KERNEL_ID = 1
#define CPU_ID = 2
#define MSP_ID = 3


//============================================================
 /**
  * Estructuras de Streaming
  */

typedef Byte* Stream;

typedef struct strConKer { //size 1+4096+20+1
	Char id;
	Char action;
	Byte* fileContent;
	Int16U fileContentLen;
	String bufferWriter;
	Int16U bufferWriterLen;
} StrConKer;

typedef struct strKerCpu { //size 51
	Tcb tcb;
	Int8U quantum;
} StrKerCpu;

typedef struct strCpuKer { //size 192
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
} StrCpuMsp;

typedef struct strKerMsp { //size 98 + data (4096 MAX)
	Char id;
	Int16U dataLength;
	Byte *data;
	Char action;
	Int16U size;
	Int32U pid;
	Int32U address;
} StrKerMsp;

typedef struct strMspKer {
	Char id;
	Int32U address;
	Char status;
	Int16U size;
} StrMspKer;

typedef struct strMspCpu {
	Int32U size;
	Char status;
	Byte * data;
} StrMspCpu;


typedef struct strKerCon { //size 4 + data
	Int32U logLen;
	Byte *log;
}StrKerCon;

//==============================================//
/**
 * Constructores
 */

StrConKer* newStrConKer(Char, Byte*, String, Char, Int16U, Int16U);

StrCpuMsp* newStrCpuMsp(Char, Int32U, Char, Byte*, Int16U);
StrCpuKer* newStrCpuKer(Char, String, Tcb, Char, Char, Int16U);

StrKerCpu* newStrKerCpu(Tcb, Int8U);
StrKerMsp* newStrKerMsp(Char, Int16U, Byte*,Char , Int16U ,Int32U , Int32U );

StrMspCpu* newStrMspCpu(Int32U, Byte *, Boolean);
StrMspKer* newStrMspKer(Char, Int32U, Char, Int16U);

StrKerCon* newStrKerCon(Int32U, Byte*);

//==============================================//
/**
 * serialize
 */

t_bitarray* serializeConKer(StrConKer*);

t_bitarray* serializeCpuMsp(StrCpuMsp*);
t_bitarray* serializeCpuKer(StrCpuKer*);

t_bitarray* serializeMspCpu(StrMspCpu*);
t_bitarray* serializeMspKer(StrMspKer*);

t_bitarray* serializeKerMsp(StrKerMsp*);
t_bitarray* serializeKerCon(StrKerCon*);

t_bitarray* serializeKerCpu(StrKerCpu*);

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
 * Handshake
 */
Char getStreamId(Stream);

#endif /* FERESTREAM_H_ */
