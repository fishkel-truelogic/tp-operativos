/*
 * sockets.h
 *
 *  Created on: 23/09/2014
 *      Author: lfishkel
 */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_
//=================================================================	
// DEPENDENCIES
//=================================================================
#include <src/commons/collections/dictionary.h>	
#include <src/fereTypes.h>
//=================================================================

//=================================================================
//TIPOS DE DATOS
//=================================================================
typedef struct instruction {
	String instructionName;
	Int32S op1, op2, op3;
} Instruction;

typedef struct intructionOperators {
	Char[3] op;
} InstructionOperators;

Boolean loadInstructionDictionary(t_dictionary*);
Int8U getInstructionOperatorsTotal(t_dictionary*, String);
Boolean operatorIsRegister(t_dictionary*, String name, index);

//=================================================================
//FUNCIONES DE LAS INSTRUCCIONES BESO
//=================================================================

/**
 * Carga en el registro, el número dado.
 */
void funcLOAD(Int32S*, Int32S);

/**
 * Obtiene el valor de memoria apuntado por el segundo registro. El valor obtenido lo asigna en el
 * primer registro.
 */
void funcGETM(Int32S*, Int32S*);

/**
 * Pone tantos bytes desde el segundo registro, hacia la memoria apuntada por el primer registro
 */
void funcSETM(Int32S, Int32S*, Int32S*);

/**
 * Copia el valor del segundo registro hacia el primero
 */
void funcMOVR(Int32S*, Int32S*);

/**
 * Suma el primer registro con el segundo registro. El resultado de la operación se almacena en el
 * registro A.
 */
void funcADDR(Int32S*, Int32S*);

/**
 * Resta el primer registro con el segundo registro. El resultado de la operación se almacena en el
 * registro A.
 */
void funcSUBR(Int32S*, Int32S*);

/**
 * Multiplica el primer registro con el segundo registro. El resultado de la operación se almacena
 * en el registro A.
 */
void funcMULR(Int32S*, Int32S*);

/**
 * Obtiene el resto de la división del primer registro con el segundo registro. El resultado de la
 * operación se almacena en el registro A.
 */
void funcMODR(Int32S*, Int32S*);

/**
 * Divide el primer registro con el segundo registro. El resultado de la operación se almacena en el
 * registro A; a menos que el segundo operando sea 0, en cuyo caso se asigna el flag de ZERO_DIV
 * y no se hace la operación.
 **/
void funcDIVR(Int32S*, Int32S*);

/**
 *Incrementar una unidad al registro.
 **/
void funcINCR(Int32S*);

/**
 *Decrementa una unidad al registro.
 **/
void funcDECR(Int32S*);

/**
 * Compara si el primer registro es igual al segundo. De ser verdadero, se almacena el valor 1. De lo
 * contrario el valor 0. El resultado de la operación se almacena en el registro A.
 **/
void funcCOMP(Int32S*, Int32S*); // [Registro], [Registro]

/**
 * Compara si el primer registro es mayor o igual al segundo. De ser verdadero, se almacena el
 * valor 1. De lo contrario el valor 0. El resultado de la operación se almacena en el registro A.
 **/
void funcCGEQ(Int32S*, Int32S*);

/**
 * Compara si el primer registro es menor o igual al segundo. De ser verdadero, se almacena el
 * valor 1. De lo contrario el valor 0. El resultado de la operación se almacena en el registro A.
 **/
void funcCLEQ(Int32S*, Int32S*);

/**
 * Altera el flujo de ejecución para ejecutar la instrucción apuntada por el registro. El valor es el
 * desplazamiento desde el inicio del programa.
 **/
void funcGOTO(Int32S*);

/**
 * Altera el flujo de ejecución, solo si el valor del registro A es cero, para ejecutar la instrucción
 * apuntada por el registro. El valor es el desplazamiento desde el inicio del programa.
 **/
void funcJMPZ(Int32S);

/**
 * Altera el flujo de ejecución, solo si el valor del registro A es cero, para ejecutar la instrucción
 * apuntada por el registro. El valor es el desplazamiento desde el inicio del programa.
 **/
void funcJPNZ(Int32S);

/**
 * Interrumpe la ejecución del programa para ejecutar la rutina del kernel que se encuentra en la
 * posición apuntada por la direccion. El ensamblador admite ingresar una cadena indicando el
 * nombre, que luego transformará en el número correspondiente. Los posibles valores son
 * “MALC”, “FREE”, “INNN”, “INNC”, “OUTN”, “OUTC”, “BLOK”, “WAKE”, “CREA” y “JOIN”. Invoca al
 * servicio correspondiente en el proceso Kernel. Notar que el hilo en cuestión debe bloquearse
 * tras una interrupción.
 **/
void funcINTE(Int32S);

/**
 * Limpia el registro de flags.
 **/
void funcFLCL();

/**
 * Desplaza 12 los bits del registro, tantas veces como se indique en el Número. De ser
 * desplazamiento positivo, se considera hacia la derecha. De lo contrario hacia la izquierda.
 **/
void funcSHIF(Int32S, Int32S*);

/**
 * Consume un ciclo del CPU sin hacer nada
 **/
void funcNOPP();

/**
 * Apila los primeros bytes, indicado por el número, del registro hacia el stack. Modifica el valor del
 * registro cursor de stack de forma acorde.
 **/
void funcPUSH(Int32S, Int32S*);

/**
 * Desapila los primeros bytes, indicado por el número, del stack hacia el registro. Modifica el valor
 * del registro de stack de forma acorde.
 **/
void funcTAKE(Int32S, Int32S*);

/**
 * Finaliza la ejecución.
 * Instrucciones Protegidas
 * Además el CPU puede ejecutar otro conjunto de instrucciones solo si el valor de KM es 1. Ninguna de
 * 12
 * http://es.wikipedia.org/wiki/Operador_a_nivel_de_bits#Desplazamiento_l.C3.B3gico
 * 25estas operaciones tiene operadores:
 **/
void funcXXXX();

/**
 * Reserva una cantidad de memoria especificada por el registro A. La direccion de esta se
 * almacena en el registro A. Crea en la MSP un nuevo segmento del tamaño especificado asociado
 * al programa en ejecución.
 **/
void funcMALC();

/**
 * Libera la memoria apuntada por el registro A. Solo se podrá liberar memoria alocada por la
 * instrucción de MALC. Destruye en la MSP el segmento indicado en el registro A.
 **/
void funcFREE();

/**
 * Pide por consola del programa que se ingrese un número, con signo entre –2.147.483.648 y
 * 2.147.483.647. El mismo será almacenado en el registro A. Invoca al servicio correspondiente en
 * el proceso Kernel.
 **/
void funcINNN();

/**
 * Pide por consola del programa que se ingrese una cadena no más larga de lo indicado por el
 * registro B. La misma será almacenada en la posición de memoria apuntada por el registro A.
 * invoca al servicio correspondiente en el proceso Kernel.
 **/
void funcINNC();

/**
 * Imprime por consola del programa el número, con signo almacenado en el registro A. Invoca al
 * servicio correspondiente en el proceso Kernel.
 **/
void funcOUTN();

/**
 * Imprime por consola del programa una cadena de tamaño indicado por el registro B que se
 * encuentra en la direccion apuntada por el registro A. Invoca al servicio correspondiente en el
 * proceso Kernel.
 **/
void funcOUTC();

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
void funcCREA();

/**
 * Bloquea el programa que ejecutó la llamada al sistema hasta que el hilo con el identificador
 * almacenado en el registro A haya finalizado. Invoca al servicio correspondiente en el proceso
 * Kernel.
 **/
void funcJOIN();

/**
 * De tener una base de stack en 100, y un cursor en 130 (S-X=30). Al crear un nuevo stack, la dirección de este
 * podría ser 500, por lo que el cursor tendrá que ser 530 (S-X=30).
 * 26Bloquea el programa que ejecutó la llamada al sistema hasta que el recurso apuntado por B se
 * libere.
 * La evaluación y decisión de si el recurso está libre o no es hecha por la llamada al sistema WAIT
 * pre-compilada.
 **/
void funcBLOK();

/**
 * Desbloquea al primer programa bloqueado por el recurso apuntado por B.
 * La evaluación y decisión de si el recurso está libre o no es hecha por la llamada al sistema
 * SIGNAL pre-compilada.
 * Notar que las instrucciones son de tamaño variable tanto por la cantidad de parámetros que reciben
 * como por el tamaño de cada uno de los mismos. Sin embargo, interpretando los primeros 4 bytes de
 * cada una es posible conocer de qué instrucción se trata, y, por lo tanto, cual es el tamaño de la misma.
 **/
void funcWAKE();

/*
27Anexo II: Especificación de las llamadas al sistema
Como se explicó antes, el sistema cuenta con algunas llamadas al sistema (system calls) que son
ejecutadas luego de pedir por ellas con la operación de INTErrupción. Muchas de estas son simplemente
una operación codificada en ESO junto con algunas operaciones para consumir algunos ciclos.
MALLOC();
Ejecuta la operación de MALC.
2. FREE
Ejecuta la operación de FREE.
3. INN
Ejecuta la operación de INNN.
4. INC
Ejecuta la operación de INNC.
5. OUTN
Ejecuta la operación de OUTN.
6. OUTC
Ejecuta la operación de OUTC.
7. CREATE_THREAD
Ejecuta la operación de CREA.
8. JOIN_THREAD
Ejecuta la operación de JOIN.
9. WAIT
Toma el valor apuntado por el registro B, resta una unidad, guarda en la memoria
correspondiente el nuevo valor y evalúa si este contador es negativo o cero. De serlo, invoca a la
operación de BLOK; de lo contrario no. Altera el registro A por uso interno.
10. SIGNAL
Toma el valor apuntado por el registro B, suma una unidad, guarda en la memoria
correspondiente el nuevo valor y evalúa si este contador es negativo o cero. De serlo, invoca a la
operación de WAKE; de lo contrario no. Altera el registro A por uso interno.
11. SETSEM
Fija el valor del recurso apuntado por el registro B al valor del registro D. Altera el registro A por
uso interno.*/



//=================================================================


#endif /* INSTRUCTIONS_H_ */
