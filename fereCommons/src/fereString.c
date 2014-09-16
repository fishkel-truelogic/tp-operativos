/*
 * fereString.c
 *
 *  Created on: 16/09/2014
 *      Author: lfishkel
 */
#include <string.h>
#include <stdlib.h>
#include "typeDef.h"


char* newString(int size) {
	return (char*) malloc(sizeof(char) * size);
}

int strToInt(String str) {
	if (str != NULL) {
		return (int) strtol(str, (char **) NULL, 10);
	} else {
		return 0;
	}
}
