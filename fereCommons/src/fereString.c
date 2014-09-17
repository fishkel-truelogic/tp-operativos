/*
 * fereString.c
 *
 *  Created on: 16/09/2014
 *      Author: lfishkel
 */
#include <string.h>
#include <stdlib.h>
#include "fereTypes.h"


String newString(Int8U size) {
	return (String) malloc(sizeof(Char) * size);
}

Int32S strToInt(String str) {
	if (str != NULL) {
		return (Int32S) strtol(str, (Char**) NULL, 10);
	} else {
		return 0;
	}
}
