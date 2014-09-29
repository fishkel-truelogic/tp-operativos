/*
 * mspconsole.c
 *
 *  Created on: 25/09/2014
 *      Author: Leandro Sampayo
 */

#include "msp.h"
#include "mspconsole.h"


void mspConsole(void){

	Int32U option = 0;

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
	scanf("%d",&option);
			while (getchar() != '\n');
			switch (option) {
				case 1: consoleCreateSegment(); break;
				case 2: consoleDestroySegment(); break;
				case 3: consoleWriteMemory(); break;
				case 4: consoleReadMemory(); break;
				case 5: consoleShowSegmentTable(); break;
				case 6: consoleShowPagesTable(); break;
				case 7: consoleShowFrames(); break;
				case 8: consoleHelpMsp(); break;
				case 9: break;
				default: puts("Error, por favor intente de nuevo."); break;
			}

}


/*Crear Segmento [PID], [Tamaño] Crea un nuevo segmento
en el espacio de direcciones del Proceso <PID> de
tamaño <Tamaño>. Imprime la dirección base del segmento
creado.*/
void consoleCreateSegment(){
	Int32U pid=0;
	Int32U size=0;

	//LEO PID/BASE
	printf("PID:");
	scanf("%d", &pid);
	printf("SIZE:");
	scanf("%d", &size);



	if (pid >= 0 && pid <= 65535)
		createSegment(pid, size);
		printf("Creo un nuevo segmento en el espacio de direcciones del PID: %d de tamaño %d bytes.\n\n", pid, size);
	else
		printf("Error\n\n");
}

/*Destruir Segmento [PID], [Dirección Base] Destruye el
segmento identificado por <Dirección Base> en el
espacio de direcciones del Proceso <PID>.*/
void consleDestroySegment() {
	Int32U pid;
	Int32U base;

	//LEO PID/BASE
	printf("PID:");
	scanf("%d", &pid);
	printf("BASE ADDRESS:");
	scanf("%d", &base);

	if (pid >= 0 && pid <= 65535 && destroySegment(pid, base)) {
		printf("Destruyo el segmento del PID: %d con base %d.\n\n", pid, base);
	}
	else {
		printf("Error numero de proceso invalido\n\n");
	}
}

/*Escribir Memoria [PID], [Dirección Virtual], [Tamaño],
[Texto] Escribe hasta <Tamaño> bytes del <Texto>
ingresado en la dirección <Dirección Virtual> del espacio
de direcciones del Proceso <PID>.
En caso de error por violación de segmento debe ser informado.*/
void consoleWriteMemory() {
	Int32U pid;
	Int32U virAddress;
	Int32U size;
	Boolean segFault = FALSE;
	String text = malloc(200);

	//LEO PID/VIRTUAL ADDRESS/SIZE/TEXT
	printf("PID:");
	scanf("%d", &pid);
	printf("VIRTUAL ADDRESS:");
	scanf("%d", &virAddress);
	printf("SIZE:");
	scanf("%d", &size);
	printf("TEXT (200caracteres max.):");
	scanf("%s", text);

	if (pid >= 0 && pid <= 65535) {
		if (writeMemory(pid, virAddress, size, text, &segFault)) {
			printf("Error: Abortado");
		}
		if (segFault) {
			printf("Error: Abortado por segmentation fault");
		}
	} else {

		printf("Error: PID incorrecto\n\n");
	}

	free(text);
}

/*Leer Memoria [PID], [Dirección Virtual],
[Tamaño] Imprime hasta <Tamaño> del contenido de la memoria
comenzando en <Dirección Virtual>, del espacio de direcciones
del Proceso <PID>.*/
void consoleReadMemory(){
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
		printf("Imprimo %d bytes de memoria del PID: %d comenzando en %d.\n\n", size, pid, virAddress);
	else
		printf("Error\n\n");
}

/*Tabla de Segmentos Imprime el contenido de la/las tablas de
segmentos de todos los procesos ESO. Por cada segmento se
debe imprimir al menos: PID, número de segmento, tamaño,
dirección virtual base del segmento.*/
void consoleShowSegmentTable(){
	puts("MUESTRO LA TABLA DE SEGMENTOS.");
}

/*Tabla de Páginas [PID] Imprime el contenido de la/las tablas
de páginas del Proceso ESO <PID>. Por cada segmento se debe
imprimir al menos: Número de segmento al que pertenece, si
se encuentra en memoria principal o esta swappeada.*/
void consoleShowPagesTable(){
	Int32U pid;

	//LEO PID
	printf("PID:");
	scanf("%d", &pid);

	//SI ESTA DENTRO DEL RANGO IMPRIMO --->IMPLEMENTAR UN int FETCHPID() a futuro
	/*
	if (!fetchPid(pid))
		printf("Muestro la tabla de paginas del proceso %d.\n\n", pid);
	else
		printf("Error: No existe el PID: %d\n\n", pid);
	 */
	if (pid >= 0 && pid <= 65535)
		printf("Muestro la tabla de paginas del proceso %d.\n\n", pid);
	else
		printf("Error\n\n");

}

/*Listar Marcos Imprime una lista de todos los marcos de memoria
existentes en el sistema, indicando: Número de Marco, si está
ocupado o no y por qué programa, y toda la información
relacionada con los algoritmos de reemplazo de páginas
implementados.*/
void consoleShowFrames(void){
	
	//IMPRIMO LA CABECERA
	puts("MUESTRO LOS MARCOS DE MEMORIA (PAGINAS LIBRES).");
	puts("#\tNro de marco de memoria");
	puts("PID\tID del Proceso");
	puts("O\t* si esta ocupado");
	puts("S\tLo que sea de swapeo ");
	puts("#\t\tPID\t\tO\t\tS");

	Int32U size = dictionary_size(frames);
	Frame* ptrFrame = NULL;
	Char used = ' '
	for (Int32U i = 0; i < size; i++)
	{
		//obtengo el frame correspondiente al i
		ptrFrame = dictionary_get(frames, intToChar(i));
		//si esta en uso le clavo un asterisco para mostrar piola
		if (ptrFrame->usde==TRUE)
			used = '*'
		else
			used = ' '
		//muestro
		printf("%d\t\t%d\t\t%c\t\tS", i, ptrFrame->pid, used)
	}
//TODO SWAPPING --> Aca hay que implementar ir a leer los marcos de memoria que esten swapeados
}


void consoleHelpMsp(void){
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

