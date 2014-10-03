/*
 * mspconsole.c
 *
 *  Created on: 25/09/2014
 *      Author: Leandro Sampayo
 */

#include "msp.h"
#include "mspconsole.h"

Boolean mspConsole() {
	Int32U option = 0;
	Boolean leave = FALSE;

	puts("++++++++++++++++CONSOLA MSP++++++++++++++++");
	puts("*\tSeleccione el comando a ejecutar:");
	puts("*");
	puts("*\t 1.  CREAR SEGMENTO [PID] [SIZE]");
	puts("*\t 2.  DESTRUIR SEGMENTO [PID] [BASE ADDR]");
	puts("*\t 3.  ESCRIBIR MEMORIA [PID] [VIRT ADDR] [SIZE] [TEXT]");
	puts("*\t 4.  LEER MEMORIA [PID] [VIRT ADDR] [SIZE]");
	puts("*\t 5.  TABLA DE SEGMENTOS");
	puts("*\t 6.  TABLA DE PAGINAS");
	puts("*\t 7.  LISTAR MARCOS");
	puts("*\t 8.  AYUDA");
	puts("*\t 9.  SALIR DEL PROGRAMA");
	puts("+++++++++++++++++++++++++++++++++++++++++++");
	puts("");
	printf("Ingrese numero de operacion: ");
	scanf("%d", &option);
	switch (option) {
	case 1:
		consoleCreateSegment();
		break;
	case 2:
		consoleDestroySegment();
		break;
	case 3:
		consoleWriteMemory();
		break;
	case 4:
		consoleReadMemory();
		break;
	case 5:
		consoleShowSegmentTable();
		break;
	case 6:
		consoleShowPagesTable();
		break;
	case 7:
		consoleShowFrames();
		break;
	case 8:
		consoleHelpMsp();
		break;
	case 9:
		leave = TRUE;
		break;
	default:
		puts("Error, por favor intente de nuevo.");
		break;
	}
	return !leave;
}

int clean_stdin()
{
    while (getchar()!='\n');
    return 1;
}


/**
 * Crear Segmento [PID], [Tamaño] Crea un nuevo segmento
 * en el espacio de direcciones del Proceso <PID> de
 * tamaño <Tamaño>. Imprime la dirección base del segmento
 * creado.
 */
void consoleCreateSegment() {
	Int32U pid = 0;
	Int32U size = 0;
    char c;
    do {
		printf("\nPID:");
    } while (((scanf("%d%c", &pid, &c)!=2 || c!='\n') && clean_stdin()) || pid < 0 || pid > 65535);

	printf("\nSIZE:");
	while (!scanf("%d", &size)) {
		printf("Error: valor de size incorrecto, intente nuevamente:\n\n");
	}

	if (createSegment(pid, size)) {
		printf(
				"Creo un nuevo segmento en el espacio de direcciones del PID: %d de tamaño %d bytes con exito.\n\n",
				pid, size);
	} else {
		printf(
				"Ocurrio un error al intentar crear un segmento para el PID: %d de tamaño %d bytes.\n\n",
				pid, size);
	}
}

/**
 * Destruir Segmento [PID], [Dirección Base] Destruye el
 * segmento identificado por <Dirección Base> en el
 * espacio de direcciones del Proceso <PID>.
 */
void consoleDestroySegment() {
	Int32U pid;
	Int32U base;

	printf("\nPID:");
	while (!scanf("%d", &pid)) {
		printf("Error: valor de pid incorrecto, intente nuevamente:\n\n");
	}

	printf("\nSEGMENT BASE ADDRESS:");
	while (!scanf("%d", &base)) {
		printf(
				"Error: valor de BASE ADDRESS es incorrecto, intente nuevamente:\n\n");
	}

	if (destroySegment(pid, base)) {
		printf("Destruyo el segmento del PID: %d con base %d.\n\n", pid, base);
	} else {
		printf("Error al destruir segmento\n\n");
	}
}

/**
 * Escribir Memoria [PID], [Dirección Virtual], [Tamaño],
 * [Texto] Escribe hasta <Tamaño> bytes del <Texto>
 * ingresado en la dirección <Dirección Virtual> del espacio
 * de direcciones del Proceso <PID>.
 * En caso de error por violación de segmento debe ser informado.
 */
void consoleWriteMemory() {
	Int32U pid;
	Int32U virAddress;
	Int32U size;
	String text = malloc(200);

	printf("\nPID:");
	while (!scanf("%d", &pid)) {
		printf("Error: valor de pid incorrecto, intente nuevamente:\n\n");
	}

	printf("\nVIRTUAL ADDRESS:");
	while (!scanf("%d", &virAddress)) {
		printf(
				"Error: valor de BASE ADDRESS es incorrecto, intente nuevamente:\n\n");
	}
	printf("\nSIZE:");
	while (!scanf("%d", &size)) {
		printf("Error: valor de size incorrecto, intente nuevamente:\n\n");
	}

	printf("\nTEXT (200caracteres max.):");
	while (!scanf("%s", text)) {
		printf("Error: valor de TEXT incorrecto, intente nuevamente:\n\n");
	}

	if (writeMemory(pid, virAddress, size, (Byte*) text)) {
		printf("Escribio en la direccion %d del PID: %d.\n\n", virAddress, pid);
	} else {
		printf("Error: Segmentation Fault\n\n");
	}

	free(text);
}

/**
 * Leer Memoria [PID], [Dirección Virtual],
 * [Tamaño] Imprime hasta <Tamaño> del contenido de la memoria
 * comenzando en <Dirección Virtual>, del espacio de direcciones
 * del Proceso <PID>.
 */
void consoleReadMemory() {
	Int32U pid;
	Int32U virAddress;
	Int32U size;

	//LEO PID/VIRTUAL ADDRESS/SIZE
	printf("PID:");
	scanf("%d", &pid);
	printf("VIRTUAL ADDRESS:");
	scanf("%d", &virAddress);
	printf("SIZE:");
	scanf("%d", &size);

	//SI ESTA DENTRO DEL RANGO IMPRIMO --->IMPLEMENTAR UN int FETCHPID() a futuro
	/*
	 if (!fetchPid(pid))
	 printf("Muestro la tabla de paginas del proceso %d.\n\n", pid);
	 else
	 printf("Error: No existe el PID: %d\n\n", pid);
	 */
	if (pid >= 0 && pid <= 65535)
		printf("Imprimo %d bytes de memoria del PID: %d comenzando en %d.\n\n",
				size, pid, virAddress);
	else
		printf("Error\n\n");
}

/** 
 * Tabla de Segmentos Imprime el contenido de la/las tablas de
 * segmentos de todos los procesos ESO. Por cada segmento se
 * debe imprimir al menos: PID, número de segmento, tamaño,
 * dirección virtual base del segmento.
 */
void consoleShowSegmentTable() {
	puts("MUESTRO LA TABLA DE SEGMENTOS.");
}

/**
 * Tabla de Páginas [PID] Imprime el contenido de la/las tablas
 * de páginas del Proceso ESO <PID>. Por cada segmento se debe
 * imprimir al menos: Número de segmento al que pertenece, si
 * se encuentra en memoria principal o esta swappeada.
 */
void consoleShowPagesTable() {
	Int32U pid;

	//LEO PID
	printf("\nPID:");
	while (!scanf("%d", &pid)) {
		printf("Error: valor de pid incorrecto, intente nuevamente:\n\n");
	}

	if (showPages(pid)) {
		printf("Muestro la tabla de paginas del proceso %d.\n\n", pid);
	} else {
		printf("Error\n\n");
	}

}

/**
 * Listar Marcos Imprime una lista de todos los marcos de memoria
 * existentes en el sistema, indicando: Número de Marco, si está
 * ocupado o no y por qué programa, y toda la información
 * relacionada con los algoritmos de reemplazo de páginas
 * implementados.
 */
void consoleShowFrames(void) {

	//IMPRIMO LA CABECERA
	puts("MUESTRO LOS MARCOS DE MEMORIA (PAGINAS LIBRES).");
	puts("#\tNro de marco de memoria");
	puts("PID\tID del Proceso");
	puts("O\t* si esta ocupado");
	puts("S\tLo que sea de swapeo ");
	puts("#\t\tPID\t\tO\t\tS");
	showFrames();
//TODO SWAPPING --> Aca hay que implementar ir a leer los marcos de memoria que esten swapeados
}

/**
 * Help para la consola
 */
void consoleHelpMsp() {
puts("CREAR SEGMENTO [PID], [Tamaño] Crea un nuevo segmento");
puts("en el espacio de direcciones del Proceso <PID> de");
puts("tamaño <Tamaño>. Imprime la dirección base del segmento");
puts("creado.");
puts("");
puts("");
puts("DESTRUIR SEGMENTO [PID], [Dirección Base] Destruye el");
puts("segmento identificado por <Dirección Base> en el");
puts("espacio de direcciones del Proceso <PID>.");
puts("");
puts("");
puts("ESCRIBIR MEMORIA [PID], [Dirección Virtual], [Tamaño],");
puts("[Texto] Escribe hasta <Tamaño> bytes del <Texto>");
puts("ingresado en la dirección <Dirección Virtual> del espacio");
puts("de direcciones del Proceso <PID>.");
puts("En caso de error por violación de segmento debe ser informado.");
puts("");
puts("");
puts("LEER MEMORIA [PID], [Dirección Virtual],");
puts("[Tamaño] Imprime hasta <Tamaño> del contenido de la memoria");
puts("comenzando en <Dirección Virtual>, del espacio de direcciones");
puts("del Proceso <PID>.");
puts("");
puts("");
puts("IMPRIMIR TABLA DE SEGMENTOS: Imprime el contenido de la/las tablas de");
puts("segmentos de todos los procesos ESO. Por cada segmento se");
puts("debe imprimir al menos: PID, número de segmento, tamaño,");
puts("dirección virtual base del segmento.");
puts("");
puts("");
puts("IMPRIMIR TABLA DE PAGINAS [PID] Imprime el contenido de la/las tablas");
puts("de páginas del Proceso ESO <PID>. Por cada segmento se debe");
puts("imprimir al menos: Número de segmento al que pertenece, si");
puts("se encuentra en memoria principal o esta swappeada.");
puts("");
puts("");
puts("LISTAR MARCOS: Imprime una lista de todos los marcos de memoria");
puts("existentes en el sistema, indicando: Número de Marco, si está");
puts("ocupado o no y por qué programa, y toda la información");
puts("relacionada con los algoritmos de reemplazo de páginas");
puts("implementados.");
puts("");
puts("");
}

