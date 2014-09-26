/*
 * mspconsole.h
 *
 *  Created on: 25/09/2014
 *      Author: utnso
 */

#ifndef MSPCONSOLE_H_
#define MSPCONSOLE_H_

#include<stdio.h>
#include<stdlib.h>
#include <src/fereTypes.h>


void mspConsole(void);
void helpMspConsole(void);


/*Crear Segmento [PID], [Tamaño] Crea un nuevo segmento
en el espacio de direcciones del Proceso <PID> de
tamaño <Tamaño>. Imprime la dirección base del segmento
creado.*/
void createSegment(void);

/*Destruir Segmento [PID], [Dirección Base] Destruye el
segmento identificado por <Dirección Base> en el
espacio de direcciones del Proceso <PID>.*/
void destroySegment(void);

/*Escribir Memoria [PID], [Dirección Virtual], [Tamaño],
[Texto] Escribe hasta <Tamaño> bytes del <Texto>
ingresado en la dirección <Dirección Virtual> del espacio
de direcciones del Proceso <PID>.
En caso de error por violación de segmento debe ser informado.*/
void writeMemory(void);

/*Leer Memoria [PID], [Dirección Virtual],
[Tamaño] Imprime hasta <Tamaño> del contenido de la memoria
comenzando en <Dirección Virtual>, del espacio de direcciones
del Proceso <PID>.*/
void readMemory(void);

/*Tabla de Segmentos Imprime el contenido de la/las tablas de
segmentos de todos los procesos ESO. Por cada segmento se
debe imprimir al menos: PID, número de segmento, tamaño,
dirección virtual base del segmento.*/
void showSegmentTable(void);

/*Tabla de Páginas [PID] Imprime el contenido de la/las tablas
de páginas del Proceso ESO <PID>. Por cada segmento se debe
imprimir al menos: Número de segmento al que pertenece, si
se encuentra en memoria principal o esta swappeada.*/
void showPagesTable(void);

/*Listar Marcos Imprime una lista de todos los marcos de memoria
existentes en el sistema, indicando: Número de Marco, si está
ocupado o no y por qué programa, y toda la información
relacionada con los algoritmos de reemplazo de páginas
implementados9.*/
void showFreePages(void);


#endif /* MSPCONSOLE_H_ */
