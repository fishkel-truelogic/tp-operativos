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
#include "instructions.h"
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

	InstructionOperators* iopFLCL = malloc(sizeof(InstructionOperators));
	iopFLCL->op[0] = NONE;
	iopFLCL->op[1] = NONE;
	iopFLCL->op[2] = NONE;
	iopFLCL->func = funcFLCL;
	dictionary_put(instructionsOperators, "FLCL", iopFLCL);

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