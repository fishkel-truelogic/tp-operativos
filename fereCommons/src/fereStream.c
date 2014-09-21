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
	ptrByte = (Byte*) sck->log;
	ptrData += sck->logLen;
	memcpy(ptrData, ptrByte, sck->logLen);
	t_bitarray* barray = bitarray_create((char*) data, size);
	return barray;

}

t_bitarray* serializeMspCpu(StrMspCpu* smp) {
	//Calculo cuanta data voy a serealizar
	Int16U size = sizeof(smp->size) + smp->size * sizeof(Byte) + sizeof(smp->status);
	//Reservo memoria del tamaño de la estructura que tengo que serializar
	Stream data = malloc(size);
	//Guardo la direccion inicial para un pivote
	Stream ptrData = data;

	//este es el que va a apuntar a los datos de la structura
	Byte* ptrByte = (Byte*)&smp->size;
	memcpy(ptrData, ptrByte, sizeof(smp->size));
	ptrData += sizeof(smp->size);

	ptrByte = smp->data;
	memcpy(ptrData, ptrByte, smp->size);
	ptrData += smp->size;

	ptrByte = (Byte*)&smp->status;
	memcpy(ptrData, ptrByte, sizeof(smp->status));
	ptrData += sizeof(smp->status);

	t_bitarray*  barray = bitarray_create((char*) data, size);
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
	t_bitarray* barray = bitarray_create((char*) data, size);
		return barray;

}

t_bitarray* serializeKerMsp(StrKerMsp *skm) {

	Int32U size = 0;


	size += sizeof(skm->id);
	size += sizeof(skm->dataLength);
	size += skm->dataLength;
	size += sizeof(skm->action);
	size += sizeof(skm->size);
	size += sizeof(skm->pid);
	size += sizeof(skm->address);

	Stream data = malloc(size);
	Stream ptrData = data;


	Byte* ptrByte = (Byte*) &skm->id;
	memcpy(ptrData, ptrByte, sizeof(skm->id));
	ptrData += sizeof(skm->id);

	ptrByte = (Byte*) &skm->dataLength;
	memcpy(ptrData, ptrByte, sizeof(skm->dataLength));
	ptrData += sizeof(skm->dataLength);

	ptrByte = skm->data;
	memcpy(ptrData, ptrByte, skm->dataLength);
	ptrData += skm->dataLength;

	ptrByte = (Byte*) &skm->action;
	memcpy(ptrData, ptrByte, sizeof(skm->action));
	ptrData += sizeof(skm->action);

	ptrByte = (Byte*) &skm->size;
	memcpy(ptrData, ptrByte, sizeof(skm->size));
	ptrData += sizeof(skm->size);

	ptrByte = (Byte*) &skm->pid;
	memcpy(ptrData, ptrByte, sizeof(skm->pid));
	ptrData += sizeof(skm->pid);

	ptrByte = (Byte*) &skm->address;
	memcpy(ptrData, ptrByte, sizeof(skm->address));
	ptrData += sizeof(skm->address);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return barray;

}


t_bitarray* serializeConKer(StrConKer* sconk) {
	Int8U size = sizeof(Byte)*sconk->bufferWriterLen
			+ sizeof(Byte)*sconk->fileContentLen
			+ sizeof(sconk->action)
			+ sizeof(sconk->id)
			+ sizeof(sconk->fileContentLen)
			+ sizeof(sconk->bufferWriterLen);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte = (Byte*) &sconk->id;
	memcpy(ptrData, ptrByte, sizeof(sconk->id));
	ptrData += sizeof(sconk->id);
	ptrByte = (Byte*) &sconk->fileContentLen;
	memcpy(ptrData, ptrByte, sizeof(sconk->fileContentLen));
	ptrData += sizeof(sconk->fileContentLen);
	ptrByte = (Byte*) sconk->fileContent;
	memcpy(ptrData, ptrByte, sconk->fileContentLen);
	ptrData += sconk->fileContentLen;
	ptrByte = (Byte*) &sconk->bufferWriterLen;
	memcpy(ptrData, ptrByte, sizeof(sconk->bufferWriterLen));
	ptrData += sizeof(sconk->bufferWriterLen);
	ptrByte = (Byte*) sconk->bufferWriter;
	memcpy(ptrData, ptrByte, sconk->bufferWriterLen);
	ptrData += sconk->bufferWriterLen;
	ptrByte = (Byte*) &sconk->action;
	memcpy(ptrData, ptrByte, sizeof(sconk->action));
	ptrData += sizeof(sconk->action);
	t_bitarray* barray = bitarray_create((char*)data, size);
	return barray;

}

t_bitarray* serializeKerCon(StrKerCon *skc) {

	Int32U size = 0;

	size += sizeof(skc->logLen);
	size += skc->logLen;

	Stream data = malloc(size);
	Stream ptrData = data;


	Byte* ptrByte = (Byte*) &skc->logLen;
	memcpy(ptrData, ptrByte, sizeof(skc->logLen));
	ptrData += sizeof(skc->logLen);

	ptrByte = skc->log;
	memcpy(ptrData, ptrByte, skc->logLen);
	ptrData += skc->logLen;

	t_bitarray* barray = bitarray_create((char*) data, size);
	return barray;

}

StrMspCpu* unserializeMspCpu(Stream dataStream) {
	//Guardo la direccion de memoria inicial del stream
	Stream ptrByte = dataStream;
	//defino las variables de la estructura
	Int32U size;
	Byte * data=NULL;
	Boolean status;

	memcpy(&size, ptrByte, sizeof(size));
	ptrByte += sizeof(size);
	data = malloc (size);
	//leo los datos por el largo que me pasaron en size
	memcpy(data, ptrByte, size);
	ptrByte += size;

	memcpy(&status, ptrByte, sizeof(Boolean));
	ptrByte += sizeof(Boolean);

	free(dataStream);

	return newStrMspCpu(size, data, status);
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

StrConKer* unserializeConKer(Stream data) {
	Stream ptrByte = data;
	Char id;
	Byte* fileContent=NULL;
	Int16U fileContentLen, bufferWriterLen;
	String bufferWriter=NULL;
	Char action;
	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&fileContentLen, ptrByte, sizeof(fileContentLen));
	ptrByte += sizeof(fileContentLen);
	fileContent = malloc(fileContentLen);
	memcpy(fileContent, ptrByte, fileContentLen);
	ptrByte += fileContentLen;
	memcpy(&bufferWriterLen, ptrByte, sizeof(bufferWriterLen));
	ptrByte += sizeof(bufferWriterLen);
	bufferWriter = malloc(bufferWriterLen);
	memcpy(bufferWriter, ptrByte, bufferWriterLen);
	ptrByte += bufferWriterLen;
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	free(data);
	return newStrConKer(id, fileContent, bufferWriter, action, bufferWriterLen, fileContentLen);
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
	return newStrMspKer(id, address, status, size);
}

StrKerMsp* unserializeKerMsp(Stream dataSerialized) {

			Stream ptrByte = dataSerialized;
			Char id;
			Int16U dataLength;
			Byte *data = NULL;
			Char action;
			Int16U size;
			Int32U pid;
			Int32U address;


			memcpy(&id, ptrByte, sizeof(id));
			ptrByte += sizeof(id);

			memcpy(&dataLength, ptrByte, sizeof(dataLength));
			ptrByte += sizeof(dataLength);

			data = malloc(dataLength);
			memcpy(data, ptrByte, dataLength);
			ptrByte += dataLength;

			memcpy(&action, ptrByte, sizeof(action));
			ptrByte += sizeof(action);

			memcpy(&size, ptrByte, sizeof(size));
			ptrByte += sizeof(size);

			memcpy(&pid, ptrByte, sizeof(pid));
			ptrByte += sizeof(pid);

			memcpy(&address, ptrByte, sizeof(address));

			free(dataSerialized);
			return newStrKerMsp(id, dataLength, data, action, size, pid, address);

}


StrKerCon* unserializeKerCon(Stream dataSerialized) {

			Stream ptrByte = dataSerialized;

			Int32U logLen;
			Byte *data = NULL;

			memcpy(&logLen, ptrByte, sizeof(logLen));
			ptrByte += sizeof(logLen);

			data = malloc(logLen);
			memcpy(data, ptrByte, logLen);
			ptrByte += logLen;

			free(dataSerialized);
			return newStrKerCon(logLen,data);

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


StrConKer* newStrConKer(Char id, Byte* fileContent, String bufferWriter, Char action, Int16U bufferWriterLen, Int16U fileContentLen) {
	StrConKer* sconk = malloc(sizeof(StrConKer));
	sconk->id = id;
	sconk->fileContent = fileContent;
	sconk->bufferWriter = bufferWriter;
	sconk->fileContentLen = fileContentLen;
	sconk->bufferWriterLen = bufferWriterLen;
	sconk->action = action;
	return sconk;
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

StrKerMsp* newStrKerMsp(Char id, Int16U dataLength,Byte *data,Char action, Int16U size,Int32U pid, Int32U address) {

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


StrKerCon* newStrKerCon(Int32U logLen, Byte *log) {

	StrKerCon* skc = malloc(sizeof(StrKerCon));

	skc->logLen = logLen;
	skc->log = log;

	return skc;
}
