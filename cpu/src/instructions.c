/*
 * instructions.c
 *
 *  Created on: 23/09/2014
 *      Author: SebastianEsperanza
 */

//=================================================================	
// DEPENDENCIES
//=================================================================
#include <src/commons/collections/dictionary.h>
#include <src/fereTypes.h>
#include <src/fereStream.h>
#include "instructions.h"
#include "cpu.h"

//=================================================================

//=================================================================	
// CONSTANTS
//=================================================================
#define NONE -1
#define REGISTER 0
#define NUMBER 1
#define ADDRESS 2
//=================================================================

Boolean loadInstructionDictionary(t_dictionary* instructionsOperators) {
	instructionsOperators = dictionary_create();

	InstructionOperators* iopLOAD = malloc(sizeof(InstructionOperators));
	iopLOAD->op[0] = REGISTER;
	iopLOAD->op[1] = NUMBER;
	iopLOAD->op[2] = NONE;
	iopLOAD->func = funcLOAD;
	dictionary_put(instructionsOperators, "LOAD", iopLOAD);

	InstructionOperators* iopGETM = malloc(sizeof(InstructionOperators));
	iopGETM->op[0] = REGISTER;
	iopGETM->op[1] = REGISTER;
	iopGETM->op[2] = NONE;
	iopGETM->func = funcGETM;
	dictionary_put(instructionsOperators, "GETM", iopGETM);

	InstructionOperators* iopSETM = malloc(sizeof(InstructionOperators));
	iopSETM->op[0] = NUMBER;
	iopSETM->op[1] = REGISTER;
	iopSETM->op[2] = REGISTER;
	iopSETM->func = funcSETM;
	dictionary_put(instructionsOperators, "SETM", iopSETM);

	InstructionOperators* iopMOVR = malloc(sizeof(InstructionOperators));
	iopMOVR->op[0] = REGISTER;
	iopMOVR->op[1] = REGISTER;
	iopMOVR->op[2] = NONE;
	iopMOVR->func = funcMOVR;
	dictionary_put(instructionsOperators, "MOVR", iopMOVR);

	InstructionOperators* iopADDR = malloc(sizeof(InstructionOperators));
	iopADDR->op[0] = REGISTER;
	iopADDR->op[1] = REGISTER;
	iopADDR->op[2] = NONE;
	iopADDR->func = funcADDR;
	dictionary_put(instructionsOperators, "ADDR", iopADDR);

	InstructionOperators* iopSUBR = malloc(sizeof(InstructionOperators));
	iopSUBR->op[0] = REGISTER;
	iopSUBR->op[1] = REGISTER;
	iopSUBR->op[2] = NONE;
	iopSUBR->func = funcSUBR;
	dictionary_put(instructionsOperators, "SUBR", iopSUBR);

	InstructionOperators* iopMULR = malloc(sizeof(InstructionOperators));
	iopMULR->op[0] = REGISTER;
	iopMULR->op[1] = REGISTER;
	iopMULR->op[2] = NONE;
	iopMULR->func = funcMULR;
	dictionary_put(instructionsOperators, "MULR", iopMULR);

	InstructionOperators* iopMODR = malloc(sizeof(InstructionOperators));
	iopMODR->op[0] = REGISTER;
	iopMODR->op[1] = REGISTER;
	iopMODR->op[2] = NONE;
	iopMODR->func = funcMODR;
	dictionary_put(instructionsOperators, "MODR", iopMODR);

	InstructionOperators* iopDIVR = malloc(sizeof(InstructionOperators));
	iopDIVR->op[0] = REGISTER;
	iopDIVR->op[1] = REGISTER;
	iopDIVR->op[2] = NONE;
	iopDIVR->func = funcDIVR;
	dictionary_put(instructionsOperators, "DIVR", iopDIVR);

	InstructionOperators* iopINCR = malloc(sizeof(InstructionOperators));
	iopINCR->op[0] = REGISTER;
	iopINCR->op[1] = NONE;
	iopINCR->op[2] = NONE;
	iopINCR->func = funcINCR;
	dictionary_put(instructionsOperators, "INCR", iopINCR);

	InstructionOperators* iopDECR = malloc(sizeof(InstructionOperators));
	iopDECR->op[0] = REGISTER;
	iopDECR->op[1] = NONE;
	iopDECR->op[2] = NONE;
	iopDECR->func = funcDECR;
	dictionary_put(instructionsOperators, "DECR", iopDECR);

	InstructionOperators* iopCOMP = malloc(sizeof(InstructionOperators));
	iopCOMP->op[0] = REGISTER;
	iopCOMP->op[1] = REGISTER;
	iopCOMP->op[2] = NONE;
	iopCOMP->func = funcCOMP;
	dictionary_put(instructionsOperators, "COMP", iopCOMP);

	InstructionOperators* iopCGEQ = malloc(sizeof(InstructionOperators));
	iopCGEQ->op[0] = REGISTER;
	iopCGEQ->op[1] = REGISTER;
	iopCGEQ->op[2] = NONE;
	iopCGEQ->func = funcCGEQ;
	dictionary_put(instructionsOperators, "CGEQ", iopCGEQ);

	InstructionOperators* iopCLEQ = malloc(sizeof(InstructionOperators));
	iopCLEQ->op[0] = REGISTER;
	iopCLEQ->op[1] = REGISTER;
	iopCLEQ->op[2] = NONE;
	iopCLEQ->func = funcCLEQ;
	dictionary_put(instructionsOperators, "CLEQ", iopCLEQ);

	InstructionOperators* iopGOTO = malloc(sizeof(InstructionOperators));
	iopGOTO->op[0] = REGISTER;
	iopGOTO->op[1] = NONE;
	iopGOTO->op[2] = NONE;
	iopGOTO->func = funcGOTO;
	dictionary_put(instructionsOperators, "GOTO", iopGOTO);

	InstructionOperators* iopJMPZ = malloc(sizeof(InstructionOperators));
	iopJMPZ->op[0] = NUMBER;
	iopJMPZ->op[1] = NONE;
	iopJMPZ->op[2] = NONE;
	iopJMPZ->func = funcJMPZ;
	dictionary_put(instructionsOperators, "JMPZ", iopJMPZ);

	InstructionOperators* iopJPNZ = malloc(sizeof(InstructionOperators));
	iopJPNZ->op[0] = NUMBER;
	iopJPNZ->op[1] = NONE;
	iopJPNZ->op[2] = NONE;
	iopJPNZ->func = funcJPNZ;
	dictionary_put(instructionsOperators, "JPNZ", iopJPNZ);

	InstructionOperators* iopINTE = malloc(sizeof(InstructionOperators));
	iopINTE->op[0] = ADDRESS;
	iopINTE->op[1] = NONE;
	iopINTE->op[2] = NONE;
	iopINTE->func = funcINTE;
	dictionary_put(instructionsOperators, "INTE", iopINTE);

	InstructionOperators* iopSHIF = malloc(sizeof(InstructionOperators));
	iopSHIF->op[0] = NUMBER;
	iopSHIF->op[1] = REGISTER;
	iopSHIF->op[2] = NONE;
	iopSHIF->func = funcSHIF;
	dictionary_put(instructionsOperators, "SHIF", iopSHIF);

	InstructionOperators* iopNOPP = malloc(sizeof(InstructionOperators));
	iopNOPP->op[0] = NONE;
	iopNOPP->op[1] = NONE;
	iopNOPP->op[2] = NONE;
	iopNOPP->func = funcNOPP;
	dictionary_put(instructionsOperators, "NOPP", iopNOPP);

	InstructionOperators* iopPUSH = malloc(sizeof(InstructionOperators));
	iopPUSH->op[0] = NUMBER;
	iopPUSH->op[1] = REGISTER;
	iopPUSH->op[2] = NONE;
	iopPUSH->func = funcPUSH;
	dictionary_put(instructionsOperators, "PUSH", iopPUSH);

	InstructionOperators* iopTAKE = malloc(sizeof(InstructionOperators));
	iopTAKE->op[0] = NUMBER;
	iopTAKE->op[1] = REGISTER;
	iopTAKE->op[2] = NONE;
	iopTAKE->func = funcTAKE;
	dictionary_put(instructionsOperators, "TAKE", iopTAKE);

	InstructionOperators* iopXXXX = malloc(sizeof(InstructionOperators));
	iopXXXX->op[0] = NONE;
	iopXXXX->op[1] = NONE;
	iopXXXX->op[2] = NONE;
	iopXXXX->func = funcXXXX;
	dictionary_put(instructionsOperators, "XXXX", iopXXXX);

//Instrucciones Protegidas

	InstructionOperators* iopMALC = malloc(sizeof(InstructionOperators));
	iopMALC->op[0] = NONE;
	iopMALC->op[1] = NONE;
	iopMALC->op[2] = NONE;
	iopMALC->func = funcMALC;
	dictionary_put(instructionsOperators, "MALC", iopMALC);

	InstructionOperators* iopFREE = malloc(sizeof(InstructionOperators));
	iopFREE->op[0] = NONE;
	iopFREE->op[1] = NONE;
	iopFREE->op[2] = NONE;
	iopFREE->func = funcFREE;
	dictionary_put(instructionsOperators, "FREE", iopFREE);

	InstructionOperators* iopINNN = malloc(sizeof(InstructionOperators));
	iopINNN->op[0] = NONE;
	iopINNN->op[1] = NONE;
	iopINNN->op[2] = NONE;
	iopINNN->func = funcINNN;
	dictionary_put(instructionsOperators, "INNN", iopINNN);

	InstructionOperators* iopINNC = malloc(sizeof(InstructionOperators));
	iopINNC->op[0] = NONE;
	iopINNC->op[1] = NONE;
	iopINNC->op[2] = NONE;
	iopINNC->func = funcINNC;
	dictionary_put(instructionsOperators, "INNC", iopINNC);

	InstructionOperators* iopOUTN = malloc(sizeof(InstructionOperators));
	iopOUTN->op[0] = NONE;
	iopOUTN->op[1] = NONE;
	iopOUTN->op[2] = NONE;
	iopOUTN->func = funcOUTN;
	dictionary_put(instructionsOperators, "OUTN", iopOUTN);

	InstructionOperators* iopOUTC = malloc(sizeof(InstructionOperators));
	iopOUTC->op[0] = NONE;
	iopOUTC->op[1] = NONE;
	iopOUTC->op[2] = NONE;
	iopOUTC->func = funcOUTC;
	dictionary_put(instructionsOperators, "OUTC", iopOUTC);

	InstructionOperators* iopCREA = malloc(sizeof(InstructionOperators));
	iopCREA->op[0] = NONE;
	iopCREA->op[1] = NONE;
	iopCREA->op[2] = NONE;
	iopCREA->func = funcCREA;
	dictionary_put(instructionsOperators, "CREA", iopCREA);

	InstructionOperators* iopJOIN = malloc(sizeof(InstructionOperators));
	iopJOIN->op[0] = NONE;
	iopJOIN->op[1] = NONE;
	iopJOIN->op[2] = NONE;
	iopJOIN->func = funcJOIN;
	dictionary_put(instructionsOperators, "JOIN", iopJOIN);

	InstructionOperators* iopBLOK = malloc(sizeof(InstructionOperators));
	iopBLOK->op[0] = NONE;
	iopBLOK->op[1] = NONE;
	iopBLOK->op[2] = NONE;
	iopBLOK->func = funcBLOK;
	dictionary_put(instructionsOperators, "BLOK", iopBLOK);

	InstructionOperators* iopWAKE = malloc(sizeof(InstructionOperators));
	iopWAKE->op[0] = NONE;
	iopWAKE->op[1] = NONE;
	iopWAKE->op[2] = NONE;
	iopWAKE->func = funcWAKE;
	dictionary_put(instructionsOperators, "WAKE", iopWAKE);

	return TRUE;
}


Int8U getInstructionOperatorsTotal(t_dictionary* iopDic, String name) {
	Int8U i;
	InstructionOperators* iop = dictionary_get(iopDic, name);
	for (i = 0; i < 3; i++) {
		if (iop->op[i] == NONE) {
			return i;
		}
	}
	return 3;
}

Boolean operatorIsRegister(t_dictionary* iopDic, String name, Int8U index) {
	InstructionOperators* iop = dictionary_get(iopDic, name);
	return iop->op[index] == REGISTER; 
}

//=================================================================
//FUNCIONES DE LAS INSTRUCCIONES BESO
//=================================================================

/**
 * Carga en el registro, el número dado.
 */
void funcLOAD(void* op1, void* op2, void* op3){
	//me parece que aca hay que implementar una funcion que llame a la MSP...by Lean
	*(*op1) = *op2; //para mi esto es *op1 = *op2;   lean!
}

/**
 * Obtiene el valor de memoria apuntado por el segundo registro. El valor obtenido lo asigna en el
 * primer registro.
 */
void funcGETM(void* op1, void* op2, void* op3){
	//me parece que aca hay que implementar una funcion que llame a la MSP...by Lean
	*(*op1) = *op2; // esto es *op1 = op2; lean!
}
/**
 * Pone tantos bytes desde el segundo registro, hacia la memoria apuntada por el primer registro
 */
void funcSETM(void* op1, void* op2, void* op3){
	//me parece que aca hay que implementar una funcion que llame a la MSP...by Lean
	memcpy(op1, op2,(int)op1); //memcpy(dest, source, len)
}

/**
 * Copia el valor del segundo registro hacia el primero
 */
void funcMOVR(void* op1, void* op2, void* op3){
	*(*op1) = *(*op2);
}

/**
 * Suma el primer registro con el segundo registro. El resultado de la operación se almacena en el
 * registro A.
 */
void funcADDR(void* op1, void* op2, void* op3){
	//la solucion para acceder al registro A, es hacer un #include cpu.c, y hacer un getCurrentTcb()
	//que devuelva el tcb actual de la cpu y ahi le seteas el valor
	Tcb* currentTcb = getCurrentTcb();
	*currentTcb->A = *(*op1) + *(*op2);
}

/**
 * Resta el primer registro con el segundo registro. El resultado de la operación se almacena en el
 * registro A.
 */
void funcSUBR(void* op1, void* op2, void* op3){
	Tcb* currentTcb = getCurrentTcb();
	*currentTcb->A = *(*op1) - *(*op2);
}

/**
 * Multiplica el primer registro con el segundo registro. El resultado de la operación se almacena
 * en el registro A.
 */
void funcMULR(void* op1, void* op2, void* op3){
	Tcb* currentTcb = getCurrentTcb();
	*currentTcb->A = (*(*op1)) * (*(*op2)); //creo que no hacen falta los (), pero por las dudas los puse
}

/**
 * 	Obtiene el resto de la división del primer registro con el segundo registro. El resultado de la
 * operación se almacena en el registro A.
 */
void funcMODR(void* op1, void* op2, void* op3){
	Tcb* currentTcb = getCurrentTcb();
	*currentTcb->A = (*(*op1)) % (*(*op2));
}

/**
 * Divide el primer registro con el segundo registro. El resultado de la operación se almacena en el
 * registro A; a menos que el segundo operando sea 0, en cuyo caso se asigna el flag de ZERO_DIV
 * y no se hace la operación.
 **/
void funcDIVR(void* op1, void* op2, void* op3){
	Tcb* currentTcb = getCurrentTcb();
	*currentTcb->A = (op2 == 0) ? ZERO_DIV : (*(*op1)) / (*(*op2));
}

/**
 *Incrementar una unidad al registro.
 **/
void funcINCR(void* op1, void* op2, void* op3){
	*(*op1) = *(*op1) + 1;
}

/**
 *Decrementa una unidad al registro.
 **/
void funcDECR(void* op1, void* op2, void* op3){
	*(*op1) = *(*op1) - 1;
}

/**
 * Compara si el primer registro es igual al segundo. De ser verdadero, se almacena el valor 1. De lo
 * contrario el valor 0. El resultado de la operación se almacena en el registro A.
 **/
void funcCOMP(void* op1, void* op2, void* op3){
	Tcb* currentTcb = getCurrentTcb();
	*currentTcb->A = (*(*op1) = *(*op2)) ? TRUE : FALSE;	
}

/**
 * Compara si el primer registro es mayor o igual al segundo. De ser verdadero, se almacena el
 * valor 1. De lo contrario el valor 0. El resultado de la operación se almacena en el registro A.
 **/
void funcCGEQ(void* op1, void* op2, void* op3){
	Tcb* currentTcb = getCurrentTcb();
	*currentTcb->A = (*(*op1) >= *(*op2)) ? TRUE : FALSE;
}

/**
 * Compara si el primer registro es menor o igual al segundo. De ser verdadero, se almacena el
 * valor 1. De lo contrario el valor 0. El resultado de la operación se almacena en el registro A.
 **/
void funcCLEQ(void* op1, void* op2, void* op3){
	Tcb* currentTcb = getCurrentTcb();
	*currentTcb->A = (*(*op1) <= *(*op2)) ? TRUE : FALSE;	
}

/**
 * Altera el flujo de ejecución para ejecutar la instrucción apuntada por el registro. El valor es el
 * desplazamiento desde el inicio del programa.
 **/
void funcGOTO(void* op1, void* op2, void* op3){
}

/**
 * Altera el flujo de ejecución, solo si el valor del registro A es cero, para ejecutar la instrucción
 * apuntada por el registro. El valor es el desplazamiento desde el inicio del programa.
 **/
void funcJMPZ(void* op1, void* op2, void* op3){
}

/**
 * Altera el flujo de ejecución, solo si el valor del registro A es cero, para ejecutar la instrucción
 * apuntada por el registro. El valor es el desplazamiento desde el inicio del programa.
 **/
void funcJPNZ(void* op1, void* op2, void* op3){
}

/**
 * Interrumpe la ejecución del programa para ejecutar la rutina del kernel que se encuentra en la
 * posición apuntada por la direccion. El ensamblador admite ingresar una cadena indicando el
 * nombre, que luego transformará en el número correspondiente. Los posibles valores son
 * “MALC”, “FREE”, “INNN”, “INNC”, “OUTN”, “OUTC”, “BLOK”, “WAKE”, “CREA” y “JOIN”. Invoca al
 * servicio correspondiente en el proceso Kernel. Notar que el hilo en cuestión debe bloquearse
 * tras una interrupción.
 **/
void funcINTE(void* op1, void* op2, void* op3) {
	StrCpuKer* sck = getSCK();
	Tcb* tcb = getCurrentTcb();
	sck->tcb = *tcb;
	sck->address = (Int32U) *op1;
	sck->action = INTE;
	sck->tcb.P++;
}

/**
 * Desplaza 12 los bits del registro, tantas veces como se indique en el Número. De ser
 * desplazamiento positivo, se considera hacia la derecha. De lo contrario hacia la izquierda.
 **/
void funcSHIF(void* op1, void* op2, void* op3){
	
}

/**
 * Consume un ciclo del CPU sin hacer nada
 **/
void funcNOPP(void* op1, void* op2, void* op3){
}

/**
 * Apila los primeros bytes, indicado por el número, del registro hacia el stack. Modifica el valor del
 * registro cursor de stack de forma acorde.
 **/
void funcPUSH(void* op1, void* op2, void* op3){
}

/**
 * Desapila los primeros bytes, indicado por el número, del stack hacia el registro. Modifica el valor
 * del registro de stack de forma acorde.
 **/
void funcTAKE(void* op1, void* op2, void* op3){
}

/**
 * Finaliza la ejecución.
 **/
 void funcXXXX(void* op1, void* op2, void* op3)){
}

/**
 * Reserva una cantidad de memoria especificada por el registro A. La direccion de esta se
 * almacena en el registro A. Crea en la MSP un nuevo segmento del tamaño especificado asociado
 * al programa en ejecución.
 **/
void funcMALC(void* op1, void* op2, void* op3) {
	Tcb* tcb = getCurrentTcb();
	Boolean segFault = FALSE;
	StrCpuMsp* scm = newStrCpuMsp(CPU_ID, 0, CREATE_SEG, NULL, tcb->A, tcb->pid);
	if (sendRequestMsp(scm, segFault)) { //TODO que pasa si hay seg fault?
		//TODO que pasa si es exitoso?
	} else if (segFault) {
		//TODO que pasa si segFault?
	} else {
		//TODO que pasa si falla la conexion	
	}
	free(scm);
}

/**
 * Libera la memoria apuntada por el registro A. Solo se podrá liberar memoria alocada por la
 * instrucción de MALC. Destruye en la MSP el segmento indicado en el registro A.
 **/
void funcFREE(void* op1, void* op2, void* op3) {
	Tcb* tcb = getCurrentTcb();
	Boolean segFault = FALSE;
	StrCpuMsp* scm = newStrCpuMsp(CPU_ID, tcb->A, DELETE_SEG, NULL, 0, tcb->pid);
	if (sendRequestMsp(scm, segFault)) { //TODO que pasa si hay seg fault?
		//TODO que pasa si es exitoso?
	} else if (segFault) {
		//TODO que pasa si segFault?
	} else {
		//TODO que pasa si falla la conexion	
	}
	free(scm);
}

/**
 * Pide por consola del programa que se ingrese un número, con signo entre –2.147.483.648 y
 * 2.147.483.647. El mismo será almacenado en el registro A. Invoca al servicio correspondiente en
 * el proceso Kernel.
 **/
void funcINNN(void* op1, void* op2, void* op3) {
	StrKerCpu* skc = getSKC();
	StrCpuKer* sck = getSCK();
	Tcb* tcb = getCurrentTcb();
	if (skc->bufferWriter == NULL) {
		 sck->action = STD_INPUT;
		 sck->inputType = NUMBER_INPUT;
		 sck->tcb = *tcb;
		 sck->tcb->P += 4;
	} else {
		Tcb* tcb = getCurrentTcb();
		tcb->A = *((Int32S*) skc->bufferWriter);
	}
}

/**
 * Pide por consola del programa que se ingrese una cadena no más larga de lo indicado por el
 * registro B. La misma será almacenada en la posición de memoria apuntada por el registro A.
 * invoca al servicio correspondiente en el proceso Kernel.
 **/
void funcINNC(void* op1, void* op2, void* op3) {
	StrKerCpu* skc = getSKC();
	StrCpuKer* sck = getSCK();
	Tcb* tcb = getCurrentTcb();
	Boolean segFault = FALSE;
	if (skc->bufferWriter == NULL) {
		 sck->action = STD_INPUT;
		 sck->inputType = TEXT_INPUT;
		 sck->tcb = *tcb;
		 sck->tcb->P += 4;
	} else {
		StrCpuMsp* scm = newStrCpuMsp(CPU_ID, tcb->A, MEM_WRITE, skc->bufferWriter, tcb->B, tcb->pid);
		if (sendRequestMsp(scm, segFault)) { //TODO que pasa si hay seg fault?
			//TODO que pasa si es exitoso?
		} else if (segFault) {
			//TODO que pasa si segFault?
		} else {
			//TODO que pasa si falla la conexion	
		}
		free(scm);
	}
}

/**
 * Imprime por consola del programa el número, con signo almacenado en el registro A. Invoca al
 * servicio correspondiente en el proceso Kernel.
 **/
void funcOUTN(void* op1, void* op2, void* op3) {
	Tcb* tcb = getCurrentTcb();
	StrCpuKer* sck = getSCK();

	Int32S number = (Int32S) tcb->A;
	sck->log = intSToStr(number);
	sck->logLen = sizeof(Int32S);
	sck->action = STD_OUTPUT;
	sck->tcb = *tcb;
	sck->tcb->P += 4;
}	

/**
 * Imprime por consola del programa una cadena de tamaño indicado por el registro B que se
 * encuentra en la direccion apuntada por el registro A. Invoca al servicio correspondiente en el
 * proceso Kernel.
 **/
void funcOUTC(void* op1, void* op2, void* op3) {
	Tcb* tcb = getCurrentTcb();
	StrCpuKer* sck = getSCK();
	Boolean segFault = FALSE;
	sck->log = intSToStr(number);
	sck->logLen = sizeof(Int32S);
	sck->action = STD_OUTPUT;
	sck->tcb = *tcb;
	sck->tcb->P += 4;
	StrCpuMsp* scm = newStrCpuMsp(CPU_ID, tcb->A, MEM_READ, NULL, 0, tcb->pid);
	if (sendRequestMsp(scm, segFault)) { //TODO que pasa si hay seg fault?
		//TODO que pasa si es exitoso?
	} else if (segFault) {
		//TODO que pasa si segFault?
	} else {
		//TODO que pasa si falla la conexion	
	}
	free(scm);
}

/**
 * Crea un hilo, hijo del TCB que ejecutó la llamada al sistema correspondiente. El nuevo hilo
 * tendrá su Program Counter apuntado al número almacenado en el registro B. El identificador del
 * nuevo hilo se almacena en el registro A.
 * Para lograrlo debe generar un nuevo TCB como copia del TCB actual, asignarle un nuevo TID
 * correlativo al actual, cargar en el Puntero de Instrucción la rutina donde comenzará a ejecutar el
 * nuevo hilo (registro B), pasarlo de modo Kernel a modo Usuario, duplicar el segmento de stack
 * desde la base del stack, hasta el cursor del stack. Asignar la base y cursor de forma acorde (tal
 * que la diferencia entre cursor y base se mantenga igual) 13 y luego invocar al servicio
 * correspondiente en el proceso Kernel con el TCB recién generado.
 **/
void funcCREA(void* op1, void* op2, void* op3) {

}

/**
 * Bloquea el programa que ejecutó la llamada al sistema hasta que el hilo con el identificador
 * almacenado en el registro A haya finalizado. Invoca al servicio correspondiente en el proceso
 * Kernel.
 **/
void funcJOIN(void* op1, void* op2, void* op3) {

}

/**
 * De tener una base de stack en 100, y un cursor en 130 (S-X=30). Al crear un nuevo stack, la dirección de este
 * podría ser 500, por lo que el cursor tendrá que ser 530 (S-X=30).
 * 26Bloquea el programa que ejecutó la llamada al sistema hasta que el recurso apuntado por B se
 * libere.
 * La evaluación y decisión de si el recurso está libre o no es hecha por la llamada al sistema WAIT
 * pre-compilada.
 **/
void funcBLOK(void* op1, void* op2, void* op3) {

}

/**
 * Desbloquea al primer programa bloqueado por el recurso apuntado por B.
 * La evaluación y decisión de si el recurso está libre o no es hecha por la llamada al sistema
 * SIGNAL pre-compilada.
 * Notar que las instrucciones son de tamaño variable tanto por la cantidad de parámetros que reciben
 * como por el tamaño de cada uno de los mismos. Sin embargo, interpretando los primeros 4 bytes de
 * cada una es posible conocer de qué instrucción se trata, y, por lo tanto, cual es el tamaño de la misma.
 **/
void funcWAKE(void* op1, void* op2, void* op3) {

}

Boolean sendRequestMsp(StrCpuMsp* scm, Boolean* segFault) {
	//TODO switchear si es read o write y obtener response
	SocketBuffer* sb = serializeCpuMsp(scm);
	//Envio el socketBuffer
	if(!socketSend(mspClient->ptrSocketServer, sb)) {
		printf("No se pudo enviar el Stream a la MSP. \n");
		return FALSE;
	}
	free(sb);

	switch () {
		case DELETE_SEG: break;
		case CREATE_SEG: break;
		case MEM_WRITE: break;
		case MEM_READ: break;
	}

	return TRUE;
}

/**
 * Convierte un int en un String
 */
String intSToStr(Int32S integer) {
	String result = malloc(sizeof(Byte) * 10);
	sprintf(result, "%d", integer);
	return result;
}