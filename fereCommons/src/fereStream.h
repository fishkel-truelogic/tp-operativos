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

t_bitarray* serializeCpuMsp(StrCpuMsp*);
t_bitarray* serializeCpuKer(StrCpuKer*);
t_bitarray* serializeKerCpu(StrKerCpu*);

StrCpuMsp* unserializeCpuMsp(Stream);
StrCpuKer* unserializeCpuKer(Stream);
StrKerCpu* unserializeKerCpu(Stream);

StrCpuMsp* newStrCpuMsp(Char, Int32U, Char, Byte*, Int16U);
StrKerCpu* newStrKerCpu(Tcb, Int8U);
StrCpuKer* newStrCpuKer(Char, String, Tcb, Char, Char, Int16U);

#endif /* FERESTREAM_H_ */
