#ifndef SWAPPING_H_
#define SWAPPING_H_

//==========================================//
//******************************************//
// Dependencies								//	
//******************************************//
//==========================================//
#include <stdlib.h>
#include <stdio.h>
#include <src/fereTypes.h>
#include "msp.h"

//==========================================//
//******************************************//
// Constants								//	
//******************************************//
//==========================================//
#define LRU "LRU"
#define CLOCK "CLOCK"
#define SWAPPING_FILE_PATH "~/swapping/"
#define SWAPPING_FILE_EXTENSION ".txt"
#define UNDERSCORE "_"

//==========================================//
//******************************************//
// Prototypes								//	
//******************************************//
//==========================================//
Boolean swap(String, t_dictionary*, Page*);

#endif /* SWAPPING_H_ */