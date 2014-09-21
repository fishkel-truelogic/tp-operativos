/*
 * fereStream.c
 *
 *  Created on: 21/09/2014
 *      Author: utnso
 */
#include "fereStream.h"
#include <stdlib.h>
#include <string.h>

t_bitarray* serializeMspCpu(StrMspCpu* smp) {
	//Calculo cuanta data voy a serealizar
	Int16U size = sizeof(Boolean) + sizeof(Int32U) + strlen(smp->data) ;
	//Reservo memoria del tamaño de la estructura que tengo que serializar
	Stream data = malloc(size);
	//Guardo la direccion inicial para un pivote
	Stream ptrData = data;

	//este es el que va a apuntar a los datos de la structura
	Byte* ptrByte = (Byte*)&smp->size;
	memcpy(ptrData, ptrByte, strlen(smp->size));
	ptrData += strlen(smp->size);

	ptrByte = smp->data;
	memcpy(ptrData, ptrByte, strlen(smp->data));
	ptrData += strlen(smp->data);

	ptrByte = (Byte*)&smp->status;
	memcpy(ptrData, ptrByte, sizeof(smp->status));
	ptrData += sizeof(smp->status);

	t_bitarray barray = bitarray_create(data, size);
	return barray;
}

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

t_bitarray* serializeMspKer(StrMspKer* smk) {
	//Calculo cuanta data voy a serealizar
	Int16U size = sizeof(StrMspKer);
	//Reservo memoria del tamaño de la estructura que tengo que serializar
	Stream data = malloc(size);
	//Guardo la direccion inicial para un pivote
	Stream ptrData = data;
	//este es el que va a apuntar a los datos de la structura
	Byte* ptrByte = (Byte*)&smk->id;
	memcpy(ptrData, ptrByte, sizeof(smk->id));
	ptrData += sizeof(smk->id);

	ptrByte = (Byte*)&smk->address;
	memcpy(ptrData, ptrByte, sizeof(smk->address));
	ptrData += sizeof(smk->address);

	ptrByte = (Byte*)&smk->status;
	memcpy(ptrData, ptrByte, sizeof(smk->status));
	ptrData += sizeof(smk->status);

	ptrByte = (Byte*)&smk->size;
	memcpy(ptrData, ptrByte, sizeof(smk->size));
	ptrData += sizeof(smk->size);

	t_bitarray barray = bitarray_create(data, size);
	return barray;
}

StrMspCpu* unserializeMspCpu(Stream dataStream) {
	//Guardo la direccion de memoria inicial del stream
	Stream ptrByte = dataStream;
	//defino las variables de la estructura
	Int32U size;
	Byte * data;
	Boolean status;

	memcpy(&size, ptrByte, sizeof(Int32U));
	ptrByte += sizeof(Int32U);

	//leo los datos por el largo que me pasaron en size
	memcpy(&data, ptrByte, size);
	ptrByte += size;

	memcpy(&status, ptrByte, sizeof(Boolean));
	ptrByte += sizeof(Boolean);

	free(dataStream);

	return newStrMspCpu(size, data, status);
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

	return newStrKerCpu(tcb, quantum);
}
StrMspKer* unserializeMspKer(Stream dataStream) {
	//Guardo la direccion de memoria inicial del stream
	Stream ptrByte = dataStream;
	//defino las variables de la estructura
	Char id;
	Int32U address;
	Char status;
	Int16U size;

	memcpy(&id, ptrByte, sizeof(Char));
	ptrByte += sizeof(Char);

	memcpy(&address, ptrByte, sizeof(Int32U));
	ptrByte += sizeof(Int32U);

	memcpy(&status, ptrByte, sizeof(Char));
	ptrByte += sizeof(Char);

	memcpy(&size, ptrByte, sizeof(Int16U));
	ptrByte += sizeof(Int16U);
	free (dataStream);
	return newStrMspCpu(id, address, status, size);
}

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

StrMspCpu* newStrMspCpu(Int32U size, Byte* data, Boolean status) {
	StrMspCpu* smc = malloc(sizeof(StrMspCpu));
	smc->size = size;
	smc->data = data;
	smc->status = status;
	return smc;
}

StrMspKer* newStrMspKer(Char id, Int32U address, Char status, Int16U size) {
	StrMspKer* smk = malloc(sizeof(StrMspKer));
	smk->id = id;
	smk->address = address;
	smk->status = status;
	smk->size = size;
	return smk;
}
