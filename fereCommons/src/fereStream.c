/*
 * fereStream.c
 *
 *  Created on: 21/09/2014
 *      Author: fereRules
 */

//================================================================
/**
 * Dependencies
 */
#include <stdlib.h>
#include <string.h>
#include "fereTcb.h"
#include "fereSockets.h"
#include "fereStream.h"
#include "fereSockets.h"


//==============================================//
/**
 * Serialization
 */

SocketBuffer* serializeCpuKer(StrCpuKer* sck) {
	Int16U size = getSizeStrCpuKer(sck);

	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &sck->id;
	memcpy(ptrData, ptrByte, sizeof(sck->id));
	ptrData += sizeof(sck->id);

	ptrByte = (Byte*) &sck->action;
	memcpy(ptrData, ptrByte, sizeof(sck->action));
	ptrData += sizeof(sck->action);

	switch (sck->action) {
	case NEXT_TCB:
	case PROC_END:
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
		break;
	case FIRST_TCB:
		break;
	case INTE:
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

		ptrByte = (Byte*) &sck->address;
		memcpy(ptrData, ptrByte, sizeof(sck->address));
		ptrData += sizeof(sck->address);
		break;
	case STD_INPUT:
		ptrByte = (Byte*) &sck->tcb.pid;
		memcpy(ptrData, ptrByte, sizeof(sck->tcb.pid));
		ptrData += sizeof(sck->tcb.pid);

		ptrByte = (Byte*) &sck->tcb.B;
		memcpy(ptrData, ptrByte, sizeof(sck->tcb.B));
		ptrData += sizeof(sck->tcb.B);

		ptrByte = (Byte*) &sck->inputType;
		memcpy(ptrData, ptrByte, sizeof(sck->inputType));
		ptrData += sizeof(sck->inputType);
		break;
	case STD_OUTPUT:
		ptrByte = (Byte*) &sck->logLen;
		memcpy(ptrData, ptrByte, sizeof(sck->logLen));
		ptrData += sizeof(sck->logLen);

		ptrByte = (Byte*) sck->log;
		memcpy(ptrData, ptrByte, sck->logLen);
		ptrData += sck->logLen;

		ptrByte = (Byte*) &sck->tcb.pid;
		memcpy(ptrData, ptrByte, sizeof(sck->tcb.pid));
		ptrData += sizeof(sck->tcb.pid);
		break;
	case NEW_THREAD:
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
		break;
	case JOIN_THREADS:
		ptrByte = (Byte*) &sck->tcb.pid;
		memcpy(ptrData, ptrByte, sizeof(sck->tcb.pid));
		ptrData += sizeof(sck->tcb.pid);

		ptrByte = (Byte*) &sck->tcb.tid;
		memcpy(ptrData, ptrByte, sizeof(sck->tcb.tid));
		ptrData += sizeof(sck->tcb.tid);

		ptrByte = (Byte*) &sck->tid;
		memcpy(ptrData, ptrByte, sizeof(sck->tid));
		ptrData += sizeof(sck->tid);
		break;
	case BLOCK_THREAD:
		ptrByte = (Byte*) &sck->tcb.tid;
		memcpy(ptrData, ptrByte, sizeof(sck->tcb.tid));
		ptrData += sizeof(sck->tcb.tid);

		ptrByte = (Byte*) &sck->resource;
		memcpy(ptrData, ptrByte, sizeof(sck->resource));
		ptrData += sizeof(sck->resource);
		break;
	case WAKE_THREAD:
		ptrByte = (Byte*) &sck->resource;
		memcpy(ptrData, ptrByte, sizeof(sck->resource));
		ptrData += sizeof(sck->resource);
		break;
	case SEG_FAULT:
	case PROC_ABORT:
		ptrByte = (Byte*) &sck->tcb.pid;
		memcpy(ptrData, ptrByte, sizeof(sck->tcb.pid));
		ptrData += sizeof(sck->tcb.pid);
		break;
	default:
		break;
	}

	t_bitarray* barray = bitarray_create((char*) data, size);

	return bitarrayToSocketBuffer(barray);

}

SocketBuffer* serializeMspCpu(StrMspCpu* smc) {
	Int16U size = getSizeStrMspCpu(smc);
	Stream data = malloc(size);
	Stream ptrData = data;

	Byte* ptrByte = (Byte*) &smc->action;
	memcpy(ptrData, ptrByte, sizeof(smc->action));
	ptrData += sizeof(smc->action);

	switch (smc->action) {
		case MEM_READ:
			ptrByte = (Byte*) &smc->dataLen;
			memcpy(ptrData, ptrByte, sizeof(smc->dataLen));
			ptrData += sizeof(smc->dataLen);

			ptrByte = smc->data;
			memcpy(ptrData, ptrByte, smc->dataLen);
			break;
		case CREATE_SEG:
			ptrByte = (Byte*) &smc->address;
			memcpy(ptrData, ptrByte, sizeof(smc->address));
			break;
		case SEG_FAULT:
		case MEM_FULL:
		case MEM_WRITE:
		case DELETE_SEG:
		default:
			break;
	}

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

SocketBuffer* serializeKerCpu(StrKerCpu* skc) {

	Int8U size = sizeof(StrKerCpu);
	Stream data = malloc(size);
	Stream ptrData = data;

	Byte* ptrByte = (Byte*) &skc->action;
	memcpy(ptrData, ptrByte, sizeof(skc->action));
	ptrData += sizeof(skc->action);

	ptrByte = (Byte*) &skc->tcb.A;
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

	switch (skc->action) {

	case NEXT_TCB:
		ptrByte = (Byte*) &skc->quantum;
		memcpy(ptrData, ptrByte, sizeof(skc->quantum));
		ptrData += sizeof(skc->quantum);
		break;

	case STD_INPUT:

		ptrByte = (Byte*) &skc->inputType;
		memcpy(ptrData, ptrByte, sizeof(skc->inputType));
		ptrData += sizeof(skc->inputType);

		if (skc->inputType == TEXT_INPUT) {
			ptrByte = skc->bufferWriter;
			memcpy(ptrData, ptrByte, skc->tcb.B);
			ptrData += skc->tcb.B;
		} else {
			ptrByte = skc->bufferWriter;
			memcpy(ptrData, ptrByte,sizeof(Int32S) );
			ptrData += sizeof(Int32S);
		}

		break;

	default:
		break;

	}

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

SocketBuffer* serializeCpuMsp(StrCpuMsp* scm) {
	Int8U size = getSizeStrCpuMsp(scm);
	Stream data = malloc(size);
	Stream ptrData = data;

	Byte* ptrByte = (Byte*) &scm->id;
	memcpy(ptrData, ptrByte, sizeof(scm->id));
	ptrData += sizeof(scm->id);

	ptrByte = (Byte*) &scm->action;
	memcpy(ptrData, ptrByte, sizeof(scm->action));
	ptrData += sizeof(scm->action);

	ptrByte = (Byte*) &scm->pid;
	memcpy(ptrData, ptrByte, sizeof(scm->pid));
	ptrData += sizeof(scm->pid);

	switch (scm->action) {
	case MEM_READ:
		ptrByte = (Byte*) &scm->dataLen;
		memcpy(ptrData, ptrByte, sizeof(scm->dataLen));
		ptrData += sizeof(scm->dataLen);

		ptrByte = (Byte*) &scm->address;
		memcpy(ptrData, ptrByte, sizeof(scm->address));
		break;

	case MEM_WRITE:
		ptrByte = (Byte*) &scm->address;
		memcpy(ptrData, ptrByte, sizeof(scm->address));
		ptrData += sizeof(scm->address);

		ptrByte = (Byte*) &scm->dataLen;
		memcpy(ptrData, ptrByte, sizeof(scm->dataLen));
		ptrData += sizeof(scm->dataLen);

		ptrByte = (Byte*) scm->data;
		memcpy(ptrData, ptrByte, scm->dataLen);
		break;

	case CREATE_SEG:
		ptrByte = (Byte*) &scm->dataLen;
		memcpy(ptrData, ptrByte, sizeof(scm->dataLen));
		break;
	case DELETE_SEG:
		ptrByte = (Byte*) &scm->address;
		memcpy(ptrData, ptrByte, sizeof(scm->address));
		break;
	default:
		break;
	}

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

SocketBuffer* serializeMspKer(StrMspKer* smk) {
	Int16U size = getSizeStrMspKer(smk);
	Stream data = malloc(size);
	Stream ptrData = data;

	Byte* ptrByte = (Byte*) &smk->id;
	memcpy(ptrData, ptrByte, sizeof(smk->id));
	ptrData += sizeof(smk->id);

	ptrByte = (Byte*) &smk->action;
	memcpy(ptrData, ptrByte, sizeof(smk->action));
	ptrData += sizeof(smk->action);

	switch (smk->action) {
		case CREATE_SEG:
			ptrByte = (Byte*) &smk->address;
			memcpy(ptrData, ptrByte, sizeof(smk->address));
			ptrData += sizeof(smk->address);
			break;
		case SEG_FAULT:
		case DELETE_SEG:
		case MEM_WRITE:
		case MEM_FULL:
		default:
			break;
	}

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

SocketBuffer* serializeKerMsp(StrKerMsp* skm) {
	Int32U size = getSizeStrKerMsp(skm);
	Stream data = malloc(size);
	Stream ptrData = data;

	Byte* ptrByte = (Byte*) &skm->id;
	memcpy(ptrData, ptrByte, sizeof(skm->id));
	ptrData += sizeof(skm->id);

	ptrByte = (Byte*) &skm->action;
	memcpy(ptrData, ptrByte, sizeof(skm->action));
	ptrData += sizeof(skm->action);

	switch (skm->action) {
	case CREATE_SEG:
		ptrByte = (Byte*) &skm->pid;
		memcpy(ptrData, ptrByte, sizeof(skm->pid));
		ptrData += sizeof(skm->pid);

		ptrByte = (Byte*) &skm->size;
		memcpy(ptrData, ptrByte, sizeof(skm->size));
		ptrData += sizeof(skm->size);
		break;
	case DELETE_SEG:
		ptrByte = (Byte*) &skm->pid;
		memcpy(ptrData, ptrByte, sizeof(skm->pid));
		ptrData += sizeof(skm->pid);

		ptrByte = (Byte*) &skm->address;
		memcpy(ptrData, ptrByte, sizeof(skm->address));
		ptrData += sizeof(skm->address);
		break;
	case MEM_WRITE:
		ptrByte = (Byte*) &skm->pid;
		memcpy(ptrData, ptrByte, sizeof(skm->pid));
		ptrData += sizeof(skm->pid);

		ptrByte = (Byte*) &skm->address;
		memcpy(ptrData, ptrByte, sizeof(skm->address));
		ptrData += sizeof(skm->address);

		ptrByte = (Byte*) &skm->size;
		memcpy(ptrData, ptrByte, sizeof(skm->size));
		ptrData += sizeof(skm->size);

		ptrByte = skm->data;
		memcpy(ptrData, ptrByte, skm->size);
		break;
	default:
		break;
	}

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

SocketBuffer* serializeConKer(StrConKer* sconk) {
	Int8U size = getSizeStrConKer(sconk);
	Stream data = malloc(size);
	Stream ptrData = data;

	Byte* ptrByte = (Byte*) &sconk->id;
	memcpy(ptrData, ptrByte, sizeof(sconk->id));
	ptrData += sizeof(sconk->id);

	ptrByte = (Byte*) &sconk->action;
	memcpy(ptrData, ptrByte, sizeof(sconk->action));
	ptrData += sizeof(sconk->action);

	switch (sconk->action) {
	case STD_INPUT:

		ptrByte = (Byte*) &sconk->fileContentLen;
		memcpy(ptrData, ptrByte, sizeof(sconk->fileContentLen));
		ptrData += sizeof(sconk->fileContentLen);

		ptrByte = (Byte*) sconk->fileContent;
		memcpy(ptrData, ptrByte, sconk->fileContentLen);
		ptrData += sconk->fileContentLen;

		break;
	case BESO_FILE:

		ptrByte = (Byte*) &sconk->bufferWriterLen;
		memcpy(ptrData, ptrByte, sizeof(sconk->bufferWriterLen));
		ptrData += sizeof(sconk->bufferWriterLen);

		ptrByte = (Byte*) sconk->bufferWriter;
		memcpy(ptrData, ptrByte, sconk->bufferWriterLen);
		ptrData += sconk->bufferWriterLen;

		break;
	default:
		break;
	}
	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

SocketBuffer* serializeKerCon(StrKerCon* skc) {
	Int32U size = getSizeStrKerCon(skc);
	Stream data = malloc(size);
	Stream ptrData = data;

	Byte* ptrByte = (Byte*) &skc->action;
	memcpy(ptrData, ptrByte, sizeof(skc->action));

	switch (skc->action) {
	case STD_INPUT:
		ptrByte = (Byte*) &skc->inputType;
		memcpy(ptrData, ptrByte, sizeof(skc->inputType));
		break;
	case STD_OUTPUT:
		ptrByte = (Byte*) &skc->logLen;
		memcpy(ptrData, ptrByte, sizeof(skc->logLen));
		ptrData += sizeof(skc->logLen);
		ptrByte = skc->log;
		memcpy(ptrData, ptrByte, skc->logLen);
		ptrData += skc->logLen;
		break;
	default:
		break;
	}

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

//==============================================//
/**
 * Un serialization
 */

StrMspCpu* unserializeMspCpu(Stream dataStream) {
	Stream ptrByte = dataStream;

	Int32U dataLen, address;
	Byte * data = NULL;
	Char action;

	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	switch (action) {
	case MEM_READ:
		memcpy(&dataLen, ptrByte, sizeof(dataLen));
		ptrByte += sizeof(dataLen);

		data = malloc(dataLen);
		memcpy(data, ptrByte, dataLen);
		break;
	case CREATE_SEG:
		memcpy(&address, ptrByte, sizeof(address));
		break;
	case SEG_FAULT:
	case MEM_FULL:
	case MEM_WRITE:
	case DELETE_SEG:
	default:
		break;
	}

	return newStrMspCpu(dataLen, data, action, address);
}

StrCpuMsp* unserializeCpuMsp(Stream data) {
	Stream ptrByte = data;
	Char id;
	Int32U address, pid;
	Char action;
	Int16U dataLen;
	Byte* writeData = NULL;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);

	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);
	switch (action) {
	case MEM_READ:
		memcpy(&dataLen, ptrByte, sizeof(dataLen));
		ptrByte += sizeof(dataLen);

		memcpy(&address, ptrByte, sizeof(address));
		break;
	case MEM_WRITE:
		memcpy(&address, ptrByte, sizeof(address));
		ptrByte += sizeof(address);

		memcpy(&dataLen, ptrByte, sizeof(dataLen));
		ptrByte += sizeof(dataLen);
		writeData = malloc(dataLen);
		memcpy(writeData, ptrByte, dataLen);
		break;
	case CREATE_SEG:
		memcpy(&dataLen, ptrByte, sizeof(dataLen));
		break;
	case DELETE_SEG:
		memcpy(&address, ptrByte, sizeof(address));
		break;
	default:
		break;
	}
	free(data);
	StrCpuMsp* scm = newStrCpuMsp(id, address, action, writeData, dataLen, pid);
	return scm;
}

StrCpuKer* unserializeCpuKer(Stream data) {
	Stream ptrByte = data;
	Char id, inputType, resource, action;
	Int32U address, tid;
	Int16U logLen;
	Tcb tcb;
	String log;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);

	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	switch (action) {
	case NEXT_TCB:
	case PROC_END:
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
		break;
	case FIRST_TCB:
		break;
	case INTE:
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
		memcpy(&address, ptrByte, sizeof(address));
		ptrByte += sizeof(address);
		break;
	case STD_INPUT:
		memcpy(&tcb.pid, ptrByte, sizeof(tcb.pid));
		ptrByte += sizeof(tcb.pid);

		memcpy(&tcb.B, ptrByte, sizeof(tcb.pid));
		ptrByte += sizeof(tcb.B);

		memcpy(&inputType, ptrByte, sizeof(inputType));
		ptrByte += sizeof(inputType);
		break;
	case STD_OUTPUT:
		memcpy(&logLen, ptrByte, sizeof(logLen));
		ptrByte += sizeof(logLen);

		log = malloc(logLen);
		memcpy(log, ptrByte, logLen);
		ptrByte += logLen;

		memcpy(&tcb.pid, ptrByte, sizeof(tcb.pid));
		ptrByte += sizeof(tcb.pid);
		break;
	case NEW_THREAD:
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
		break;
	case JOIN_THREADS:
		memcpy(&tcb.pid, ptrByte, sizeof(tcb.pid));
		ptrByte += sizeof(tcb.pid);

		memcpy(&tcb.tid, ptrByte, sizeof(tcb.tid));
		ptrByte += sizeof(tcb.tid);

		memcpy(&tid, ptrByte, sizeof(tid));
		ptrByte += sizeof(tid);
		break;
	case BLOCK_THREAD:
		memcpy(&tcb.tid, ptrByte, sizeof(tcb.tid));
		ptrByte += sizeof(tcb.tid);

		memcpy(&resource, ptrByte, sizeof(resource));
		ptrByte += sizeof(resource);
		break;
	case WAKE_THREAD:
		memcpy(&resource, ptrByte, sizeof(resource));
		break;
	case SEG_FAULT:
	case PROC_ABORT:
		memcpy(&tcb.pid, ptrByte, sizeof(tcb.pid));
		ptrByte += sizeof(tcb.pid);
		break;
	default:
		break;
	}

	free(data);
	StrCpuKer* sck = newStrCpuKer(id, tcb, action, logLen, log, address, tid,
			inputType, resource);
	return sck;
}

StrKerCpu* unserializeKerCpu(Stream data) {

	Stream ptrByte = data;
	Tcb tcb;
	Int8U quantum;
	Char action;
	Byte *bufferWriter = NULL;
	Char inputType;

	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

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

	switch (action) {
	case NEXT_TCB:
		memcpy(&quantum, ptrByte, sizeof(quantum));
		ptrByte += sizeof(quantum);
		break;
	case STD_INPUT:

		memcpy(&inputType, ptrByte, sizeof(inputType));
		ptrByte += sizeof(inputType);

		if(inputType == TEXT_INPUT){
			bufferWriter = malloc(tcb.B);
			memcpy(bufferWriter, ptrByte, tcb.B);
			ptrByte += tcb.B;
		}else{
			bufferWriter = malloc(sizeof(Int32S));
			memcpy(bufferWriter, ptrByte, sizeof(Int32S));
			ptrByte += sizeof(Int32S);
		}


		break;
	default:
		break;
	}

	free(data);
	return newStrKerCpu(tcb, quantum,bufferWriter,inputType,action);
}

StrConKer* unserializeConKer(Stream data) {
	Stream ptrByte = data;
	Char id;
	Char action;
	Byte* fileContent = NULL;
	Int16U fileContentLen, bufferWriterLen;
	Byte* bufferWriter = NULL;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	switch (action) {
	case STD_INPUT:

		memcpy(&fileContentLen, ptrByte, sizeof(fileContentLen));
		ptrByte += sizeof(fileContentLen);

		fileContent = malloc(fileContentLen);
		memcpy(fileContent, ptrByte, fileContentLen);
		ptrByte += fileContentLen;

		break;
	case BESO_FILE:

		memcpy(&bufferWriterLen, ptrByte, sizeof(bufferWriterLen));
		ptrByte += sizeof(bufferWriterLen);

		bufferWriter = malloc(bufferWriterLen);
		memcpy(bufferWriter, ptrByte, bufferWriterLen);
		ptrByte += bufferWriterLen;

		break;
	default:
		break;
	}
	free(data);
	return newStrConKer(id, fileContent, bufferWriter, action, bufferWriterLen,
			fileContentLen);
}

StrMspKer* unserializeMspKer(Stream dataStream) {
	Stream ptrByte = dataStream;
	Char id;
	Int32U address;
	Char action;

	memcpy(&id, ptrByte, sizeof(Char));
	ptrByte += sizeof(Char);

	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	switch (action) {
		case CREATE_SEG:
			memcpy(&address, ptrByte, sizeof(Int32U));
			break;
		case SEG_FAULT:
		case DELETE_SEG:
		case MEM_WRITE:
		case MEM_FULL:
		default:
			break;
		}
	free(dataStream);
	return newStrMspKer(id, address, action);
}

StrKerMsp* unserializeKerMsp(Stream dataSerialized) {
	Stream ptrByte = dataSerialized;
	Char id;
	Byte* data = NULL;
	Char action;
	Int16U size;
	Int32U pid;
	Int32U address;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);

	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	switch (action) {
	case CREATE_SEG:
		memcpy(&pid, ptrByte, sizeof(pid));
		ptrByte += sizeof(pid);

		memcpy(&size, ptrByte, sizeof(size));
		ptrByte += sizeof(size);
		break;
	case DELETE_SEG:
		memcpy(&pid, ptrByte, sizeof(pid));
		ptrByte += sizeof(pid);
		memcpy(&address, ptrByte, sizeof(address));
		ptrByte += sizeof(address);
		break;
	case MEM_WRITE:
		memcpy(&pid, ptrByte, sizeof(pid));
		ptrByte += sizeof(pid);

		memcpy(&address, ptrByte, sizeof(address));
		ptrByte += sizeof(address);

		memcpy(&size, ptrByte, sizeof(size));
		ptrByte += sizeof(size);

		data = malloc(size);
		memcpy(data, ptrByte, size);
		ptrByte += size;
		break;
	default:
		break;
	}

	free(dataSerialized);
	return newStrKerMsp(id, data, action, size, pid, address);
}

StrKerCon* unserializeKerCon(Stream dataSerialized) {
	Stream ptrByte = dataSerialized;
	Int32U logLen;
	Char action;
	Char inputType;
	Byte* data = NULL;

	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	switch (action) {
	case STD_INPUT:
		memcpy(&inputType, ptrByte, sizeof(inputType));
		ptrByte += sizeof(inputType);
		break;
	case STD_OUTPUT:
		memcpy(&logLen, ptrByte, sizeof(logLen));
		ptrByte += sizeof(logLen);

		data = malloc(logLen);
		memcpy(data, ptrByte, logLen);
		ptrByte += logLen;
		break;
	default:
		break;
	}

	free(dataSerialized);
	return newStrKerCon(logLen, data, action, inputType);
}

//==============================================//
/**
 * Constructors
 */

StrCpuMsp* newStrCpuMsp(Char id, Int32U address, Char action, Byte* data,
		Int16U dataLen, Int32U pid) {
	StrCpuMsp* scm = malloc(sizeof(StrCpuMsp));
	scm->id = id;
	scm->address = address;
	scm->data = data;
	scm->dataLen = dataLen;
	scm->action = action;
	scm->pid = pid;
	return scm;
}

StrKerCpu* newStrKerCpu(Tcb tcb, Int8U quantum, Byte* bufferWriter, Char inputType ,Char action) {
	StrKerCpu* skc = malloc(sizeof(StrKerCpu));
	skc->tcb = tcb;
	skc->quantum = quantum;
	skc->bufferWriter = bufferWriter;
	skc->action = action;
	skc->inputType = inputType;
	return skc;
}

StrCpuKer* newStrCpuKer(Char id, Tcb tcb, Char action, Int16U logLen,
		String log, Int32U address, //interruption
		Int32U tid, // join
		Char inputType, Char resource) {

	StrCpuKer* sck = malloc(sizeof(StrCpuKer));
	sck->id = id;
	sck->log = log;
	sck->tcb = tcb;
	sck->action = action;
	sck->logLen = logLen;
	sck->inputType = inputType;
	sck->resource = resource;
	sck->tid = tid;
	return sck;
}

StrConKer* newStrConKer(Char id, Byte* fileContent, Byte* bufferWriter,
		Char action, Int16U bufferWriterLen, Int16U fileContentLen) {
	StrConKer* sconk = malloc(sizeof(StrConKer));
	sconk->id = id;
	sconk->fileContent = fileContent;
	sconk->bufferWriter = bufferWriter;
	sconk->fileContentLen = fileContentLen;
	sconk->bufferWriterLen = bufferWriterLen;
	sconk->action = action;
	return sconk;
}

StrMspCpu* newStrMspCpu(Int16U dataLen, Byte* data, Char action, Int32U address) {
	StrMspCpu* smc = malloc(sizeof(StrMspCpu));
	smc->dataLen = dataLen;
	smc->data = data;
	smc->action = action;
	smc->address = address;
	return smc;
}

StrMspKer* newStrMspKer(Char id, Int32U address, Char action) {
	StrMspKer* smk = malloc(sizeof(StrMspKer));
	smk->id = id;
	smk->address = address;
	smk->action = action;
	return smk;
}

StrKerMsp* newStrKerMsp(Char id, Byte *data, Char action, Int16U size,
		Int32U pid, Int32U address) {
	StrKerMsp* skm = malloc(sizeof(StrKerMsp));
	skm->id = id;
	skm->data = data;
	skm->action = action;
	skm->size = size;
	skm->pid = pid;
	skm->address = address;
	return skm;
}

StrKerCon* newStrKerCon(Int32U logLen, Byte *log, Char action, Char inputType) {
	StrKerCon* skc = malloc(sizeof(StrKerCon));
	skc->logLen = logLen;
	skc->log = log;
	skc->action = action;
	skc->inputType = inputType;
	return skc;
}

//==============================================//
/**
 * Sizes
 */

Int16U getSizeStrCpuKer(StrCpuKer* sck) {
	Int16U size = 0;
	size += sizeof(sck->id);
	size += sizeof(sck->action);
	switch (sck->action) {
	case NEXT_TCB:
	case PROC_END:
		size += sizeof(sck->tcb.A);
		size += sizeof(sck->tcb.B);
		size += sizeof(sck->tcb.C);
		size += sizeof(sck->tcb.D);
		size += sizeof(sck->tcb.E);
		size += sizeof(sck->tcb.F);
		size += sizeof(sck->tcb.X);
		size += sizeof(sck->tcb.M);
		size += sizeof(sck->tcb.P);
		size += sizeof(sck->tcb.S);
		size += sizeof(sck->tcb.kernelMode);
		size += sizeof(sck->tcb.csLenght);
		size += sizeof(sck->tcb.pid);
		size += sizeof(sck->tcb.tid);
		break;
	case FIRST_TCB:
		break;
	case INTE:
		size += sizeof(sck->tcb.A);
		size += sizeof(sck->tcb.B);
		size += sizeof(sck->tcb.C);
		size += sizeof(sck->tcb.D);
		size += sizeof(sck->tcb.E);
		size += sizeof(sck->tcb.F);
		size += sizeof(sck->tcb.X);
		size += sizeof(sck->tcb.M);
		size += sizeof(sck->tcb.P);
		size += sizeof(sck->tcb.S);
		size += sizeof(sck->tcb.kernelMode);
		size += sizeof(sck->tcb.csLenght);
		size += sizeof(sck->tcb.pid);
		size += sizeof(sck->tcb.tid);
		size += sizeof(sck->address);
		break;
	case STD_INPUT:
		size += sizeof(sck->inputType);
		size += sizeof(sck->tcb.pid);
		size += sizeof(sck->tcb.B);
		break;
	case STD_OUTPUT:
		size += sizeof(sck->logLen);
		size += sck->logLen;
		size += sizeof(sck->tcb.pid);
		break;
	case NEW_THREAD:
		size += sizeof(sck->tcb.A);
		size += sizeof(sck->tcb.B);
		size += sizeof(sck->tcb.C);
		size += sizeof(sck->tcb.D);
		size += sizeof(sck->tcb.E);
		size += sizeof(sck->tcb.F);
		size += sizeof(sck->tcb.X);
		size += sizeof(sck->tcb.M);
		size += sizeof(sck->tcb.P);
		size += sizeof(sck->tcb.S);
		size += sizeof(sck->tcb.kernelMode);
		size += sizeof(sck->tcb.csLenght);
		size += sizeof(sck->tcb.pid);
		size += sizeof(sck->tcb.tid);
		break;
	case JOIN_THREADS:
		size += sizeof(sck->tcb.pid);
		size += sizeof(sck->tcb.tid);
		size += sizeof(sck->tid);
		break;
	case BLOCK_THREAD:
		size += sizeof(sck->tcb.tid);
		size += sizeof(sck->resource);
		break;
	case WAKE_THREAD:
		size += sizeof(sck->resource);
		break;
	case SEG_FAULT:
	case PROC_ABORT:
		size += sizeof(sck->tcb.pid);
		break;
	default:
		break;
	}
	return size;
}

Int16U getSizeStrConKer(StrConKer* sck) {
	Int16U size = 0;
	size += sizeof(sck->id);
	size += sizeof(sck->action);
	switch (sck->action) {
	case STD_INPUT:
		size += sizeof(sck->bufferWriterLen);
		size += sck->bufferWriterLen;
		break;
	case BESO_FILE:
		size += sizeof(sck->fileContentLen);
		size += sck->fileContentLen;
		break;
	default:
		break;
	}
	return size;
}


Int16U getSizeStrMspKer(StrMspKer* smk) {
	Int16U size = 0;
	size += sizeof(smk->id);
	size += sizeof(smk->action);
	switch (smk->action) {
	case CREATE_SEG:
		size += sizeof(smk->address);
		break;
	case MEM_WRITE:
	case SEG_FAULT:
	case MEM_FULL:
	case DELETE_SEG:
	default:
		break;
	}
	return size;
}

Int16U getSizeStrKerMsp(StrKerMsp* skm) {
	Int16U size = 0;
	size += sizeof(skm->id);
	size += sizeof(skm->action);
	switch (skm->action) {
	case CREATE_SEG:
		size += sizeof(skm->pid);
		size += sizeof(skm->size);
		break;
	case DELETE_SEG:
		size += sizeof(skm->pid);
		size += sizeof(skm->address);
		break;
	case MEM_WRITE:
		size += sizeof(skm->pid);
		size += sizeof(skm->address);
		size += sizeof(skm->size);
		size += skm->size;
		break;
	default:
		break;
	}
	return size;
}

Int16U getSizeStrCpuMsp(StrCpuMsp* scm) {
	Int16U size = 0;
	size += sizeof(scm->id);
	size += sizeof(scm->action);
	size += sizeof(scm->pid);
	switch (scm->action) {
	case MEM_READ:
		size += sizeof(scm->dataLen);
		size += sizeof(scm->address);
		break;
	case MEM_WRITE:
		size += sizeof(scm->address);
		size += sizeof(scm->dataLen);
		size += scm->dataLen;
		break;
	case CREATE_SEG:
		size += sizeof(scm->dataLen);
		break;
	case DELETE_SEG:
		size += sizeof(scm->address);
		break;
	default:
		break;
	}
	return size;
}

Int16U getSizeStrMspCpu(StrMspCpu* smc) {
	Int16U size = 0;
	size += sizeof(smc->action);
	switch (smc->action) {
	case MEM_READ:
		size += sizeof(smc->dataLen);
		size += smc->dataLen;
		break;
	case CREATE_SEG:
		size += sizeof(smc->address);
		break;
	case SEG_FAULT:
	case MEM_FULL:
	case MEM_WRITE:
	case DELETE_SEG:
	default:
		break;
	}
	return size;
}

Int16U getSizeStrKerCon(StrKerCon* skc) {
	Int16U size = 0;
	size += sizeof(skc->action);
	switch (skc->action) {
	case STD_INPUT:
		size += sizeof(skc->inputType);
		break;
	case STD_OUTPUT:
		size += sizeof(skc->logLen);
		size += skc->logLen;
		break;
	default:
		break;
	}
	return size;
}

Int16U getSizeStrKerCpu(StrKerCpu *skc) {

	Int16U size = 0;

	size += sizeof(skc->tcb);
	size += sizeof(skc->action);

	switch (skc->action) {

	case NEXT_TCB:
		size += sizeof(skc->quantum);
		break;

	case STD_INPUT:
		size += sizeof(skc->inputType);

		if (skc->inputType == TEXT_INPUT) {
			size += skc->tcb.B;
		} else {
			size += sizeof(Int32S);
		}

		break;

	default:
		break;

	}

	return size;
}
//==============================================//
/**
 * Handshake
 */

Char getStreamId(Stream dataSerialized) {
	Char id;
	Stream ptrByte = dataSerialized;
	memcpy(&id, ptrByte, sizeof(id));
	return id;
}

//==============================================//
/**
 * bitarrayToSocketBuffer
 */

SocketBuffer* bitarrayToSocketBuffer(t_bitarray* barray) {

	SocketBuffer* sb = malloc(sizeof(SocketBuffer));
	Byte* ptrByte = (Byte*) barray->bitarray;

	Int32U i;
	for (i = 0; i < barray->size; i++) {
		sb->data[i] = *ptrByte;
		ptrByte++;
	}
	sb->size = barray->size;
	return sb;
}
