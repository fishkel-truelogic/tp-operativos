/*
 * fereStream.c
 *
 *  Created on: 21/09/2014
 *      Author: utnso
 */
#include "fereStream.h"
#include <stdlib.h>
#include <string.h>

//t_bitarray* serializeCpuMsp(StrCpuMsp*) {
//
//}
//t_bitarray* serializeMspCpu(StrMspCpu*) {
//
//}
//t_bitarray* serializeCpuKer(StrCpuKer*) {
//
//}
t_bitarray* serializeKerCpu(StrKerCpu* skc) {
	Int8U size = sizeof(Byte) * sizeof(StrKerCpu);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte = (Byte*)&skc->tcb.A;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.A));
	ptrData += sizeof(skc->tcb.A);
	ptrByte = (Byte*)&skc->tcb.B;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.B));
	ptrData += sizeof(skc->tcb.B);
	ptrByte = (Byte*)&skc->tcb.C;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.C));
	ptrData += sizeof(skc->tcb.C);
	ptrByte = (Byte*)&skc->tcb.D;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.D));
	ptrData += sizeof(skc->tcb.D);
	ptrByte = (Byte*)&skc->tcb.E;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.E));
	ptrData += sizeof(skc->tcb.E);
	ptrByte = (Byte*)&skc->tcb.F;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.F));
	ptrData += sizeof(skc->tcb.F);
	ptrByte = (Byte*)&skc->tcb.P;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.P));
	ptrData += sizeof(skc->tcb.P);
	ptrByte = (Byte*)&skc->tcb.M;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.M));
	ptrData += sizeof(skc->tcb.M);
	ptrByte = (Byte*)&skc->tcb.S;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.S));
	ptrData += sizeof(skc->tcb.S);
	ptrByte = (Byte*)&skc->tcb.X;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.X));
	ptrData += sizeof(skc->tcb.X);
	ptrByte = (Byte*)&skc->tcb.pid;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.pid));
	ptrData += sizeof(skc->tcb.pid);
	ptrByte = (Byte*)&skc->tcb.tid;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.tid));
	ptrData += sizeof(skc->tcb.tid);
	ptrByte = (Byte*)&skc->tcb.kernelMode;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.kernelMode));
	ptrData += sizeof(skc->tcb.kernelMode);
	ptrByte = (Byte*)&skc->tcb.csLenght;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.csLenght));
	ptrData += sizeof(skc->tcb.csLenght);
	ptrByte = (Byte*)&skc->quantum;
	memcpy(ptrData, ptrByte, sizeof(skc->quantum));
	ptrData += sizeof(skc->quantum);
	t_bitarray* barray = bitarray_create((char*)data, size);
	return barray;
}
//t_bitarray* serializeConKer(StrConKer*) {
//
//}
//t_bitarray* serializeKerCon(StrKerCon*) {
//
//}
//t_bitarray* serializeKerMsp(StrKerMsp*) {
//
//}
//t_bitarray* serializeMspKer(StrMspKer*) {
//
//}
//
//StrCpuMsp* unserializeCpuMsp(Stream) {
//
//}
//StrMspCpu* unserializeMspCpu(Stream) {
//
//}
//StrCpuKer* unserializeCpuKer(Stream) {
//
//}
StrKerCpu* unserializeKerCpu(Stream data) {
	Stream ptrByte = data;
	Tcb tcb;
	Int8U quantum;
	memcpy(&tcb.A, ptrByte, sizeof(tcb.A));
	ptrByte += sizeof(tcb.A);
	memcpy(&tcb.B, ptrByte, sizeof(tcb.B));
	ptrByte += sizeof(tcb.B);
	memcpy(&tcb.C, ptrByte, sizeof(tcb.C));
	ptrByte += sizeof(tcb.C);
	memcpy(&tcb.D, ptrByte, sizeof(tcb.D));
	ptrByte += sizeof(tcb.D);
	memcpy(&tcb.E, ptrByte, sizeof(tcb.E));
	ptrByte += sizeof(tcb.E);
	memcpy(&tcb.F, ptrByte, sizeof(tcb.F));
	ptrByte += sizeof(tcb.F);
	memcpy(&tcb.P, ptrByte, sizeof(tcb.P));
	ptrByte += sizeof(tcb.P);
	memcpy(&tcb.M, ptrByte, sizeof(tcb.M));
	ptrByte += sizeof(tcb.M);
	memcpy(&tcb.S, ptrByte, sizeof(tcb.S));
	ptrByte += sizeof(tcb.S);
	memcpy(&tcb.X, ptrByte, sizeof(tcb.X));
	ptrByte += sizeof(tcb.X);
	memcpy(&tcb.pid, ptrByte, sizeof(tcb.pid));
	ptrByte += sizeof(tcb.pid);
	memcpy(&tcb.tid, ptrByte, sizeof(tcb.tid));
	ptrByte += sizeof(tcb.tid);
	memcpy(&tcb.kernelMode, ptrByte, sizeof(tcb.kernelMode));
	ptrByte += sizeof(tcb.kernelMode);
	memcpy(&tcb.csLenght, ptrByte, sizeof(tcb.csLenght));
	ptrByte += sizeof(tcb.csLenght);
	memcpy(&quantum, ptrByte, sizeof(quantum));
	ptrByte += sizeof(quantum);

	return newStrKerCpu(tcb, quantum);
}
//StrConKer* unserializeConKer(Stream) {
//
//}
//StrKerCon* unserializeKerCon(Stream) {
//
//}
StrKerMsp* unserializeKerMsp(Stream dataSerialized) {

			Stream ptrByte = dataSerialized;
			Char id;
			Int16U dataLength;
			Byte *data;
			Char action;
			Int16U size;
			Int32U pid;
			Int32U address;


			memcpy(&id, ptrByte, sizeof(id));
			ptrByte += sizeof(id);
			memccpy(&dataLength, ptrByte, sizeof(dataLength));
			ptrByte += sizeof(dataLength);
			memccpy(&data, ptrByte, dataLength);
			ptrByte += dataLength;
			memccpy(&action, ptrByte, sizeof(action));
			ptrByte += sizeof(action);
			memccpy(&size, ptrByte, sizeof(size));
			ptrByte += sizeof(size);
			memccpy(&pid, ptrByte, sizeof(pid));
			ptrByte += sizeof(pid);
			memccpy(&address, ptrByte, sizeof(address));

			return newStrKerMsp(id, dataLength, *data, action, size, pid, address);

}
//StrMspKer* unserializeMspKer(Stream) {
//
//}
//
//StrCpuMsp* newStrCpuMsp(Char, Int32U, Char, Byte[]) {
//
//}
StrKerCpu* newStrKerCpu(Tcb tcb, Int8U quantum) {
	StrKerCpu* skc = malloc(sizeof(StrKerCpu));
	skc->tcb = tcb;
	skc->quantum = quantum;
	return skc;
}

StrCpuKer* newStrCpuKer(Char id, String log, Tcb tcb, Char status, Char action) {
	StrCpuKer* sck = malloc(sizeof(StrCpuKer));
	sck->id = id;
	sck->log = log;
	sck->tcb = tcb;
	sck->status = status;
	sck->action = action;
	return sck;
}

StrCpuKer* newStrKerMsp(Char id, Int16U dataLength,Byte *data,Char action, Int16U size,Int32U pid, Int32U address) {
	StrKerMsp* skm = malloc(sizeof(StrKerMsp));
	skm->id = id;
	skm->dataLength = dataLength;
	skm->data = data;
	skm->action = action;
	skm->size = size;
	skm->pid = pid;
	skm->address = address;
	return skm;
}
