/*
 * fereStream.c
 *
 *  Created on: 21/09/2014
 *      Author: utnso
 */
#include "fereStream.h"
#include <stdlib.h>
#include <string.h>

t_bitarray* serializeCpuKer(StrCpuKer* sck) {
	Int8U size = sizeof(Byte) * sizeof(StrCpuKer);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte = (Byte*) &sck->id;
	memcpy(ptrData, ptrByte, sizeof(sck->id));
	ptrByte = (Byte*) &sck->tcb.A;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.A));
	ptrData += sizeof(sck->tcb.A);
	ptrByte = (Byte*) &sck->tcb.B;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.B));
	ptrData += sizeof(sck->tcb.B);
	ptrByte = (Byte*) &sck->tcb.C;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.C));
	ptrData += sizeof(sck->tcb.C);
	ptrByte = (Byte*) &sck->tcb.D;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.D));
	ptrData += sizeof(sck->tcb.D);
	ptrByte = (Byte*) &sck->tcb.E;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.E));
	ptrData += sizeof(sck->tcb.E);
	ptrByte = (Byte*) &sck->tcb.F;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.F));
	ptrData += sizeof(sck->tcb.F);
	ptrByte = (Byte*) &sck->tcb.P;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.P));
	ptrData += sizeof(sck->tcb.P);
	ptrByte = (Byte*) &sck->tcb.M;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.M));
	ptrData += sizeof(sck->tcb.M);
	ptrByte = (Byte*) &sck->tcb.S;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.S));
	ptrData += sizeof(sck->tcb.S);
	ptrByte = (Byte*) &sck->tcb.X;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.X));
	ptrData += sizeof(sck->tcb.X);
	ptrByte = (Byte*) &sck->tcb.pid;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.pid));
	ptrData += sizeof(sck->tcb.pid);
	ptrByte = (Byte*) &sck->tcb.tid;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.tid));
	ptrData += sizeof(sck->tcb.tid);
	ptrByte = (Byte*) &sck->tcb.kernelMode;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.kernelMode));
	ptrData += sizeof(sck->tcb.kernelMode);
	ptrByte = (Byte*) &sck->tcb.csLenght;
	memcpy(ptrData, ptrByte, sizeof(sck->tcb.csLenght));
	ptrData += sizeof(sck->tcb.csLenght);
	ptrByte = (Byte*) &sck->status;
	memcpy(ptrData, ptrByte, sizeof(sck->status));
	ptrByte = (Byte*) &sck->action;
	ptrData += sizeof(sck->action);
	memcpy(ptrData, ptrByte, sizeof(sck->action));
	ptrByte = (Byte*) &sck->logLen;
	ptrData += sizeof(sck->logLen);
	memcpy(ptrData, ptrByte, sizeof(sck->logLen));
	ptrByte = sck->log;
	ptrData += sck->logLen;
	memcpy(ptrData, ptrByte, sck->logLen);
	t_bitarray* barray = bitarray_create((char*) data, size);
	return barray;

}

t_bitarray* serializeKerCpu(StrKerCpu* skc) {
	Int8U size = sizeof(Byte) * sizeof(StrKerCpu);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte = (Byte*) &skc->tcb.A;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.A));
	ptrData += sizeof(skc->tcb.A);
	ptrByte = (Byte*) &skc->tcb.B;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.B));
	ptrData += sizeof(skc->tcb.B);
	ptrByte = (Byte*) &skc->tcb.C;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.C));
	ptrData += sizeof(skc->tcb.C);
	ptrByte = (Byte*) &skc->tcb.D;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.D));
	ptrData += sizeof(skc->tcb.D);
	ptrByte = (Byte*) &skc->tcb.E;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.E));
	ptrData += sizeof(skc->tcb.E);
	ptrByte = (Byte*) &skc->tcb.F;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.F));
	ptrData += sizeof(skc->tcb.F);
	ptrByte = (Byte*) &skc->tcb.P;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.P));
	ptrData += sizeof(skc->tcb.P);
	ptrByte = (Byte*) &skc->tcb.M;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.M));
	ptrData += sizeof(skc->tcb.M);
	ptrByte = (Byte*) &skc->tcb.S;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.S));
	ptrData += sizeof(skc->tcb.S);
	ptrByte = (Byte*) &skc->tcb.X;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.X));
	ptrData += sizeof(skc->tcb.X);
	ptrByte = (Byte*) &skc->tcb.pid;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.pid));
	ptrData += sizeof(skc->tcb.pid);
	ptrByte = (Byte*) &skc->tcb.tid;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.tid));
	ptrData += sizeof(skc->tcb.tid);
	ptrByte = (Byte*) &skc->tcb.kernelMode;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.kernelMode));
	ptrData += sizeof(skc->tcb.kernelMode);
	ptrByte = (Byte*) &skc->tcb.csLenght;
	memcpy(ptrData, ptrByte, sizeof(skc->tcb.csLenght));
	ptrData += sizeof(skc->tcb.csLenght);
	ptrByte = (Byte*) &skc->quantum;
	memcpy(ptrData, ptrByte, sizeof(skc->quantum));
	ptrData += sizeof(skc->quantum);
	t_bitarray* barray = bitarray_create((char*) data, size);
	return barray;
}

t_bitarray* serializeCpuMsp(StrCpuMsp* scm) {
	Int8U size = sizeof(Byte) * sizeof(StrCpuMsp);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte = (Byte*) &scm->id;
	memcpy(ptrData, ptrByte, sizeof(scm->id));
	ptrData += sizeof(scm->id);
	ptrByte = (Byte*) &scm->address;
	memcpy(ptrData, ptrByte, sizeof(scm->address));
	ptrData += sizeof(scm->address);
	ptrByte = (Byte*) &scm->action;
	memcpy(ptrData, ptrByte, sizeof(scm->action));
	ptrData += sizeof(scm->action);
	ptrByte = (Byte*) &scm->dataLen;
	memcpy(ptrData, ptrByte, sizeof(scm->dataLen));
	ptrData += sizeof(scm->dataLen);
	ptrByte = (Byte*) scm->data;
	memcpy(ptrData, ptrByte, scm->dataLen);
	t_bitarray* barray = bitarray_create((char*) data, size);
	return barray;

}

StrCpuMsp* unserializeCpuMsp(Stream data) {
	Stream ptrByte = data;
	Char id;
	Int32U address;
	Char action;
	Int16U dataLen;
	Byte* dataa = NULL;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&address, ptrByte, sizeof(address));
	ptrByte += sizeof(address);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	dataa = malloc(dataLen);
	memcpy(dataa, ptrByte, dataLen);
	free(data);
	StrCpuMsp* scm = newStrCpuMsp(id, address, action, dataa, dataLen);
	return scm;

}

StrCpuKer* unserializeCpuKer(Stream data) {
	Stream ptrByte = data;
	Char id;
	Tcb tcb;
	Char status;
	Char action;
	Int16U logLen;
	String log;
	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
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
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&status, ptrByte, sizeof(status));
	ptrByte += sizeof(status);
	memcpy(&logLen, ptrByte, sizeof(logLen));
	ptrByte += sizeof(logLen);
	log = malloc(logLen);
	memcpy(log, ptrByte, logLen);
	ptrByte += logLen;
	free(data);
	StrCpuKer* sck = newStrCpuKer(id, log, tcb, status, action, logLen);
	return sck;
}

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
	free(data);
	return newStrKerCpu(tcb, quantum);
}

StrCpuMsp* newStrCpuMsp(Char id, Int32U address, Char action, Byte* data,
		Int16U dataLen) {
	StrCpuMsp* scm = malloc(sizeof(StrCpuMsp));
	scm->id = id;
	scm->address = address;
	scm->data = data;
	scm->dataLen = dataLen;
	scm->action = action;
	return scm;
}

StrKerCpu* newStrKerCpu(Tcb tcb, Int8U quantum) {
	StrKerCpu* skc = malloc(sizeof(StrKerCpu));
	skc->tcb = tcb;
	skc->quantum = quantum;
	return skc;
}

StrCpuKer* newStrCpuKer(Char id, String log, Tcb tcb, Char status, Char action,
		Int16U logLen) {
	StrCpuKer* sck = malloc(sizeof(StrCpuKer));
	sck->id = id;
	sck->log = log;
	sck->tcb = tcb;
	sck->status = status;
	sck->action = action;
	sck->logLen = logLen;
	return sck;
}
