/*
 * mspconsole.c
 *
 *  Created on: 25/09/2014
 *      Author: Leandro Sampayo
 */

#include "msp.h"
#include "mspconsole.h"

//PROTOTYPES
void waitForEnter();


Boolean mspConsole() {
	Int32U option = 0;
	Boolean leave = FALSE;
	puts("\n\n\n\n\n");
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
    
    printf("\nPID:");
    scanf("%d", &pid);

	printf("\nSIZE(bytes):");
	scanf("%d", &size);
	
	if (createSegment(pid, size)) {
		printf(
				"Creo un nuevo segmento en el espacio de direcciones del PID: %d de tamaño %d bytes con exito.\n\n",
				pid, size);
		waitForEnter();
	} else {
		printf(
				"Ocurrio un error al intentar crear un segmento para el PID: %d de tamaño %d bytes.\n\n",
				pid, size);
		waitForEnter();
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
	Int32U segmentNr, pageNr, offsetNr;
	printf("\nPID:");
	scanf("%d", &pid);

	printf("\nSEGMENT BASE ADDRESS (E.g. 0xF0F0F0F0):");
	scanf("%i", &base);

	segmentNr = getSegment(base);
	pageNr = getPage(base);
	offsetNr = getOffset(base);

	if(offsetNr == 0 && pageNr == 0 ){
		if (destroySegment(pid, segmentNr)){
			printf("Se destruyo el segmento del PID %d con base %#08x (%d).\n\n", pid, base, base);
			waitForEnter();
		} else {
			printf("Error al destruir segmento\n\n");
			waitForEnter();
		}
	}else{
		if( pageNr != 0 )
			printf("ERROR: La pagina %d no corresponde a la base del segmento.\n", pageNr);
		if( offsetNr != 0 )
			printf("ERROR: El offset %d no corresponde a la base del segmento.\n", offsetNr);
		waitForEnter();
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
	String text = malloc(4096);

	printf("\nPID:");
	scanf("%d", &pid);

	printf("%d\nVIRTUAL ADDRESS:", pid);
	scanf("%i", &virAddress);

	printf("\nSIZE(bytes):");
	scanf("%d", &size);

	printf("\nTEXT (4096caracteres max.):");
	scanf("%s", text);


	if (writeMemory(pid, virAddress, size, (Byte*) text)) {
		printf("Escribio en la direccion %#08x (%d) del PID: %d.\n\n", virAddress, virAddress, pid);
		waitForEnter();
	} else {
		printf("Error: Segmentation Fault\n\n");
		waitForEnter();
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
	if (pid >= 0 && pid <= 65535){
		printf("Imprimo %d bytes de memoria del PID: %d comenzando en %d.\n\n",
				size, pid, virAddress);
		waitForEnter();
	}
	else{
		printf("Error\n\n");
		waitForEnter();
	}
}

/** 
 * Tabla de Segmentos Imprime el contenido de la/las tablas de
 * segmentos de todos los procesos ESO. Por cada segmento se
 * debe imprimir al menos: PID, número de segmento, tamaño,
 * dirección virtual base del segmento.
 */
void consoleShowSegmentTable() {
	puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	puts(" *** MUESTRO LA TABLA DE SEGMENTOS *** \n");
	showSegments();
	waitForEnter();
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
	scanf("%d", &pid);

	if (showPages(pid)) {
		printf("Muestro la tabla de paginas del proceso %d.\n\nPresione ENTER para continuar", pid);
		waitForEnter();
	} else {
		printf("Error\n\n");
		waitForEnter();
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
	puts("\nMUESTRO LOS MARCOS DE MEMORIA.\n===============================\n");
	puts("99999999 indica que el marco no fue asignado a ningun proceso");
	showFrames();
	waitForEnter();
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
puts("Presione ENTER para continuar");
waitForEnter();
}


void waitForEnter(){
	printf("Presione ENTER para continuar");
	fflush (stdin);
	fflush (stdout);
	getchar();
	getchar();
}
