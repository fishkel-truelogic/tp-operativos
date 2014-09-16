/*
 * fereTypes.h
 *
 *  Created on: 16/09/2014
 *      Author: lfishkel, cesarcappetto
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

//CONSTANTES
//=====================================================================================

//CONSTANTES BOOLEANAS
static int const FALSE = 0;
static int const TRUE = 1;

//=====================================================================================


//TIPOS DE DATOS
//=====================================================================================

//CADENAS VARIABLES
typedef char* String;

//LOGICO
typedef unsigned char Boolean;

//VALOR 8-BITS NO SIGNADO
typedef unsigned char Byte;

//VALOR 8-BITS SIGNADO
typedef char Char;

//VALOR 8-BITS NO SIGNADO
typedef unsigned char Int8U;

//VALOR 8-BITS SIGNADO
typedef signed char Int8S;

//VALOR 16-BITS NO SIGNADO
typedef unsigned short Int16U;

//VALOR 16-BITS SIGNADO
typedef signed short Int16S;

//ENTERO 32 NO SIGNADO
typedef unsigned int Int32U;

//ENTERO 32 SIGNADO
typedef signed int Int32S;

//VALOR FLOTANTE 32-BITS
typedef float fP32;

//VALOR FLOTANTE 64-BITS
typedef double fP64;
//=====================================================================================



#endif /* TYPEDEF_H_ */
