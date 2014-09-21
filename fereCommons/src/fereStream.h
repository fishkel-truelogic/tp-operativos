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
	String bufferWriter;
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
	String log;
} StrCpuKer;


t_bitarray* serializeCpuKer(StrCpuKer*);
t_bitarray* serializeKerCpu(StrKerCpu*);
t_bitarray* serializeConKer(StrConKer*);



StrCpuKer* unserializeCpuKer(Stream);
StrKerCpu* unserializeKerCpu(Stream);
StrConKer* unserializeConKer(Stream);


StrKerCpu* newStrKerCpu(Tcb, Int8U);
StrCpuKer* newStrCpuKer(Char, String, Tcb, Char, Char);
StrConKer* newStrConKer(Char, Byte*, String, Char);

#endif /* FERESTREAM_H_ */
