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

typedef struct strConKer {

} StrConKer;

typedef struct strKerCon {

} StrKerCon;

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

} StrMspKer;

typedef struct strMspCpu {

} StrMspCpu;

typedef struct strCpuMsp {

} StrCpuMsp;

t_bitarray* serializeCpuMsp(StrCpuMsp*);
t_bitarray* serializeMspCpu(StrMspCpu*);
t_bitarray* serializeCpuKer(StrCpuKer*);
t_bitarray* serializeKerCpu(StrKerCpu*);
t_bitarray* serializeConKer(StrConKer*);
t_bitarray* serializeKerCon(StrKerCon*);
t_bitarray* serializeKerMsp(StrKerMsp*);
t_bitarray* serializeMspKer(StrMspKer*);

StrCpuMsp* unserializeCpuMsp(Stream);
StrMspCpu* unserializeMspCpu(Stream);
StrCpuKer* unserializeCpuKer(Stream);
StrKerCpu* unserializeKerCpu(Stream);
StrConKer* unserializeConKer(Stream);
StrKerCon* unserializeKerCon(Stream);
StrKerMsp* unserializeKerMsp(Stream);
StrMspKer* unserializeMspKer(Stream);

StrCpuMsp* newStrCpuMsp(Char, Int32U, Char, Byte[]);
StrKerCpu* newStrKerCpu(Tcb, Int8U);
StrCpuKer* newStrCpuKer(Char, String, Tcb, Char, Char);

#endif /* FERESTREAM_H_ */
