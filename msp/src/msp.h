#ifndef MSP_H_
#define MSP_H_

/* msp.h
 *
 *  Created on: 19/09/2014
 *      Author: sampower06 - Leandro Sampayo
 */


//LIBRERIAS INCLUIDAS
#include <stdlib.h>
#include <stdio.h>
#include <src/fereTypes.h>
#include <src/fereString.h>
#include <src/commons/config.h>
#include <src/commons/temporal.h>

//DEFINICIONES
#define DEBUG 1							//Habilitar o deshabilitar el debug por screen
#define CONFIG_FILE "config.txt"		//ruta del archivo de configuracion
#define PARAM_LENGTH 4					//Cantidad de parametros
#define MSP_PORT "PUERTO"				//PUERTO DE ESCUCHA DE LA MSP
#define MEM_LENGTH "CANTIDAD_MEMORIA"	//Tamaño en kilobytes de la memoria principal sin contar el espacio de intercambio (swapping).
#define SWAP_LENGTH "CANTIDAD_SWAP"		//Tamaño máximo en megabytes que pueden ocupar los archivos de swapping.
#define SWAP_ALGORITHM "SUST_PAGS"		//Algoritmo de sustitucion de paginas

//PROTOTYPES
Boolean loadConfig(void);


#endif /* MSP_H_ */
