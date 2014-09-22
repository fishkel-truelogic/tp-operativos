/*
 * fereTypes.h
 *
 *  Created on: 22/09/2014
 *      Author: lfishkel
 */

#ifndef FERETCB_H_
#define FERETCB_H_

//============================================================
 /**
  * Dependencies
  */
#include "fereTypes.h"
//============================================================


// Thread Control Block
typedef struct tcb {
	Int32U pid; //process id
	Int32U tid; //thread id
	Int32U M; //code segment base
	Int32U P; //program counter
	Int32U X; //stack segment base
	Int32U S; //stack pointer
	Boolean kernelMode; //indicates whether it is a kernel tcb
	Int8U csLenght; // code segment lenght
	Int32S A, B, C, D, E, F; //cpu registers
} Tcb;

Tcb* newTcb(Int32U pid, Int32U tid, Int32U M, Int32U P, Int32U X, Int32U S, Boolean kernelMode, Int8U csLenght, Int32S A, Int32S B, Int32S C, Int32S D, Int32S E, Int32S F,);
Tcb* newEmptyTcb();

#endif /* FERETCB_H_ */