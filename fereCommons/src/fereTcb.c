/*
 * fereTypes.h
 *
 *  Created on: 22/09/2014
 *      Author: lfishkel
 */

//============================================================
 /**
  *
  * Dependencies
  *
  */
#include <stdlib.h>
#include "fereTypes.h"
#include "fereTcb.h"

/**
 * Devuelve una nueva instancia de Tcb con los valores ingresados por parametro
 */
Tcb* newTcb(Int32U pid, Int32U tid, Int32U M, Int32U P, Int32U X, Int32U S, Boolean kernelMode, Int8U csLenght, Int32S A, Int32S B, Int32S C, Int32S D, Int32S E, Int32S F) {
	Tcb* tcb = malloc(sizeof(Tcb));
	tcb->pid = pid;
	tcb->tid = tid;
	tcb->M = M;
	tcb->S = S;
	tcb->X = X;
	tcb->P = P;
	tcb->kernelMode = kernelMode;
	tcb->csLenght = csLenght;
	tcb->A = A;
	tcb->B = B;
	tcb->C = C;
	tcb->D = D;
	tcb->E = E;
	tcb->F = F;
	return tcb;
}

/**
 * Devuelve una nueva instancia de Tcb con los valores por default
 */
Tcb* newEmptyTcb() {
	return newTcb(0, 0, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0);
}
