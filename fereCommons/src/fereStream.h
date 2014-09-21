/*
 * fereStream.h
 *
 *  Created on: 21/09/2014
 *      Author: utnso
 */


#ifndef FERESTREAM_H_
#define FERESTREAM_H_

#include "fereTypes.h"
#include "commons/bitarray.h"

typedef Byte* Stream;


typedef struct strConKer { //size 1+4096+20+1
	Char id;
	Byte* fileContent;
	Int16U fileContentLen;
	String bufferWriter;
	Int16U bufferWriterLen;
	Char action;
} StrConKer;


typedef struct strKerCpu { //size 51
	Tcb tcb;
	Int8U quantum;
} StrKerCpu;

typedef struct strCpuKer { //size 192
	Char id;
	Tcb tcb;
	Char status;
	Char action;
	Int16U logLen;
	String log;
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

//==============================================//

/**
 * serialize
 */

t_bitarray* serializeConKer(StrConKer*);

t_bitarray* serializeCpuMsp(StrCpuMsp*);
t_bitarray* serializeCpuKer(StrCpuKer*);

t_bitarray* serializeMspCpu(StrMspCpu*);
t_bitarray* serializeMspKer(StrMspKer*);

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

StrKerCpu* unserializeKerCpu(Stream);



#endif /* FERESTREAM_H_ */
