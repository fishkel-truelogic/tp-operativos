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
	Byte * data;
	Char status;
} StrMspCpu;

typedef struct strKerCon { //size 4 + data
	Int32U logLen;
	Byte *log;
}StrKerCon;

t_bitarray* serializeMspCpu(StrMspCpu*);
t_bitarray* serializeCpuKer(StrCpuKer*);
t_bitarray* serializeKerCpu(StrKerCpu*);
t_bitarray* serializeConKer(StrConKer*);
t_bitarray* serializeMspKer(StrMspKer*);
t_bitarray* serializeKerMsp(StrKerMsp*);
t_bitarray* serializeKerCon(StrKerCon*);

StrCpuKer* unserializeCpuKer(Stream);
StrKerCpu* unserializeKerCpu(Stream);
StrConKer* unserializeConKer(Stream);
StrMspKer* unserializeMspKer(Stream);
StrMspCpu* unserializeMspCpu(Stream);
StrKerMsp* unserializeKerMsp(Stream);
StrKerCon* unserializeKerCon(Stream);

StrKerCpu* newStrKerCpu(Tcb, Int8U);
StrCpuKer* newStrCpuKer(Char, String, Tcb, Char, Char);
StrConKer* newStrConKer(Char, Byte*, String, Char);
StrMspCpu* newStrMspCpu(Int32U, Byte *, Boolean);
StrMspKer* newStrMspKer(Char, Int32U, Char, Int16U);
StrKerMsp* newStrKerMsp(Char, Int16U, Byte*,Char , Int16U ,Int32U , Int32U );
StrKerCon* newStrKerCon(Int32U, Byte*);

#endif /* FERESTREAM_H_ */
