/*
 * msp.c
 *
 *  Created on: 12/09/2014
 *      Author: sampower06 - Leandro Sampayo
 */

//LIBRERIAS INCLUIDAS
#include <stdlib.h>
#include <stdio.h>
#include <src/fereTypes.h>
#include <src/fereString.h>
#include <src/fereProperties.h>
#include <src/commons/temporal.h>

//DEFINICIONES
#define MSP_PORT "PUERTO"				//PUERTO DE ESCUCHA DE LA MSP
#define MEM_LENGTH "CANTIDAD_MEMORIA"	//Tamaño en kilobytes de la memoria principal sin contar el espacio de intercambio (swapping).
#define SWAP_LENGTH "CANTIDAD_SWAP"		//Tamaño máximo en megabytes que pueden ocupar los archivos de swapping.
#define SWAP_ALGORITHM "SUST_PAGS"		//Algoritmo de sustitucion de paginas

//PROTOTYPES
void init(void);

//GLOBAL VARIABLES
Int8U mspPort = 0;
Int8U memLengthKB = 0;
Int8U swapLengthMB = 0;
String swapAlgorithm = "\0";


int main() {

	//local variables
	Char * ptrTime = temporal_get_string_time();



	//Imprimo hora al arrancar
	printf("Server time is %s\n\n", ptrTime);

	init();
	//libero memoria
	free (ptrTime);

	return 0;

}


void init() {

	printf("Archivo de config MSP leído\n===========\n");
	//Obtengo todas las propiedades del config.txt y las paso a su formato en caso de ser necesario
	//mspPort = strToInt(getProperty(MSP_PORT));
	mspPort = strToInt(getProperty("PUERTO"));
	memLengthKB = strToInt(getProperty(MEM_LENGTH));
	swapLengthMB = strToInt(getProperty(SWAP_LENGTH));
	swapAlgorithm = getProperty(SWAP_ALGORITHM);
	printf(
			"MSP Port: %u\nMemory Length: %ukB\nSwap Memory Length: %uMB\nSwap Algorithm: %s\n",
			mspPort, memLengthKB, swapLengthMB, swapAlgorithm);
}
