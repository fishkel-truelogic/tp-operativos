#ifndef MSP_H_
#define MSP_H_

/* msp.h
 *
 *  Created on: 19/09/2014
 *      Author: sampower06 - Leandro Sampayo
 */


//==========================================//
//******************************************//
// Dependencies								//	
//******************************************//
//==========================================//
#include <stdlib.h>
#include <stdio.h>
#include <src/fereTypes.h>
#include <src/commons/config.h>
#include <src/commons/temporal.h>
#include <src/commons/collections/dictionary.h>
#include <src/commons/collections/list.h>
#include "mspconsole.h"

//==========================================//
//******************************************//
// Constants								//	
//******************************************//
//==========================================//
#define DEBUG 1							//Habilitar o deshabilitar el debug por screen
#define CONFIG_FILE "config.txt"		//ruta del archivo de configuracion
#define PARAM_LENGTH 4					//Cantidad de parametros
#define MSP_PORT "PUERTO"				//PUERTO DE ESCUCHA DE LA MSP
#define MEM_LENGTH "CANTIDAD_MEMORIA"	//Tamaño en kilobytes de la memoria principal sin contar el espacio de intercambio (swapping).
#define SWAP_LENGTH "CANTIDAD_SWAP"		//Tamaño máximo en megabytes que pueden ocupar los archivos de swapping.
#define SWAP_ALGORITHM "SUST_PAGS"		//Algoritmo de sustitucion de paginas
#define FRAME_SIZE 256					//TAMAÑO DE LOS MARCOS DE PAGINA

//==========================================//
//******************************************//
// Prototypes								//	
//******************************************//
//==========================================//
Boolean createSegment(Int32U, Int32U); 
Boolean destroySegment(Int32U, Int32U);

Boolean writeMemory(Int32U, Int32U, Int32U, Byte*);
Boolean readMemory(Int32U, Int32U, Int32U);

Boolean showPages(Int32U);
void showFrames();
void showSegments();


//==========================================//
//******************************************//
// Structures								//	
//******************************************//
//==========================================//

// table -> (key: segmentId, value: Segment)
typedef struct segmentsTable {
	t_dictionary* table;
	Int32U lastId;
} SegmentsTable;

// Segment = Lista de paginas
typedef t_list* Segment;

typedef struct frame {
	Int32U pid;
	Byte* address;
	Boolean used;
} Frame;

typedef struct page {
	Frame* frame;
	Int16U timestamp;
	Boolean clock; 
	Boolean swapped;
} Page;

#endif /* MSP_H_ */
