/*
 * fereStream.c
 *
 *  Created on: 21/09/2014
 *      Author: utnso
 */
#include "fereStream.h"
#include <stdlib.h>

t_bitarray* serializeCpuMsp(StrCpuMsp*) {

}
t_bitarray* serializeMspCpu(StrMspCpu*) {

}
t_bitarray* serializeCpuKer(StrCpuKer*) {

}
t_bitarray* serializeKerCpu(StrKerCpu* skc) {
	Int8U size = sizeof(Byte) * sizeof(StrKerCpu);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte = skc->tcb.A;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.A));
	ptrData += sizeof(skc->tcb.A);
	Byte* ptrByte = skc->tcb.B;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.B));
	ptrData += sizeof(skc->tcb.B);
	Byte* ptrByte = skc->tcb.C;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.C));
	ptrData += sizeof(skc->tcb.C);
	Byte* ptrByte = skc->tcb.D;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.D));
	ptrData += sizeof(skc->tcb.D);
	Byte* ptrByte = skc->tcb.E;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.E));
	ptrData += sizeof(skc->tcb.E);
	Byte* ptrByte = skc->tcb.F;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.F));
	ptrData += sizeof(skc->tcb.F);
	Byte* ptrByte = skc->tcb.P;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.P));
	ptrData += sizeof(skc->tcb.P);
	Byte* ptrByte = skc->tcb.M;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.M));
	ptrData += sizeof(skc->tcb.M);
	Byte* ptrByte = skc->tcb.S;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.S));
	ptrData += sizeof(skc->tcb.S);
	Byte* ptrByte = skc->tcb.X;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.X));
	ptrData += sizeof(skc->tcb.X);
	Byte* ptrByte = skc->tcb.pid;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.pid));
	ptrData += sizeof(skc->tcb.pid);
	Byte* ptrByte = skc->tcb.tid;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.tid));
	ptrData += sizeof(skc->tcb.tid);
	Byte* ptrByte = skc->tcb.kernelMode;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.kernelMode));
	ptrData += sizeof(skc->tcb.kernelMode);
	Byte* ptrByte = skc->tcb.csLenght;
	memccpy(ptrData, ptrByte, sizeof(skc->tcb.csLenght));
	ptrData += sizeof(skc->tcb.csLenght);
	Byte* ptrByte = skc->quantum;
	memccpy(ptrData, ptrByte, sizeof(skc->quantum));
	ptrData += sizeof(skc->quantum);
	t_bitarray barray = bitarray_create(data, size);
	return barray;
}
t_bitarray* serializeConKer(StrConKer*) {

}
t_bitarray* serializeKerCon(StrKerCon*) {

}
t_bitarray* serializeKerMsp(StrKerMsp*) {

}
t_bitarray* serializeMspKer(StrMspKer*) {

}

StrCpuMsp* unserializeCpuMsp(Stream) {

}
StrMspCpu* unserializeMspCpu(Stream) {

}
StrCpuKer* unserializeCpuKer(Stream) {

}
StrKerCpu* unserializeKerCpu(Stream data) {
	Stream ptrByte = data;
	Tcb tcb;
	Int8U quantum;
	memccpy(&tcb.A, ptrByte, sizeof(tcb.A));
	ptrByte += sizeof(tcb.A);
	memccpy(&tcb.B, ptrByte, sizeof(tcb.B));
	ptrByte += sizeof(tcb.B);
	memccpy(&tcb.C, ptrByte, sizeof(tcb.C));
	ptrByte += sizeof(tcb.C);
	memccpy(&tcb.D, ptrByte, sizeof(tcb.D));
	ptrByte += sizeof(tcb.D);
	memccpy(&tcb.E, ptrByte, sizeof(tcb.E));
	ptrByte += sizeof(tcb.E);
	memccpy(&tcb.F, ptrByte, sizeof(tcb.F));
	ptrByte += sizeof(tcb.F);
	memccpy(&tcb.P, ptrByte, sizeof(tcb.P));
	ptrByte += sizeof(tcb.P);
	memccpy(&tcb.M, ptrByte, sizeof(tcb.M));
	ptrByte += sizeof(tcb.M);
	memccpy(&tcb.S, ptrByte, sizeof(tcb.S));
	ptrByte += sizeof(tcb.S);
	memccpy(&tcb.X, ptrByte, sizeof(tcb.X));
	ptrByte += sizeof(tcb.X);
	memccpy(&tcb.pid, ptrByte, sizeof(tcb.pid));
	ptrByte += sizeof(tcb.pid);
	memccpy(&tcb.tid, ptrByte, sizeof(tcb.tid));
	ptrByte += sizeof(tcb.tid);
	memccpy(&tcb.kernelMode, ptrByte, sizeof(tcb.kernelMode));
	ptrByte += sizeof(tcb.kernelMode);
	memccpy(&tcb.csLenght, ptrByte, sizeof(tcb.csLenght));
	ptrByte += sizeof(tcb.csLenght);
	memccpy(&quantum, ptrByte, sizeof(quantum));
	ptrByte += sizeof(quantum);

	return newStrKerCpu(tcb, quantum);
}
StrConKer* unserializeConKer(Stream) {

}
StrKerCon* unserializeKerCon(Stream) {

}
StrKerMsp* unserializeKerMsp(Stream) {

}
StrMspKer* unserializeMspKer(Stream) {

}

StrCpuMsp* newStrCpuMsp(Char, Int32U, Char, Byte[]) {

}
StrKerCpu* newStrKerCpu(Tcb tcb, Int8U quantum) {
	StrKerCpu* skc = malloc(sizeof(StrKerCpu));
	skc->tcb = tcb;
	skc->quantum = quantum;
	return skc;
}
