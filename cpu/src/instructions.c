/*
 * sockets.h
 *
 *  Created on: 23/09/2014
 *      Author: lfishkel
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
	instructionsOperators = dictionary_create();
	InstructionOperators* iopMOVR = malloc(sizeof(InstructionOperators));
	iopMOVR->op[0] = REGISTER;
	iopMOVR->op[1] = REGISTER;
	iopMOVR->op[2] = NONE;
	iopMOVR->func = funcMOVR;
	dictionary_put(instructionsOperators, "MOVR", iopMOVR);
	//TODO repetir para cada una de las instrucciones
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