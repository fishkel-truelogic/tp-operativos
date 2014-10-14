//==========================================//
//******************************************//
// Dependencies								//	
//******************************************//
//==========================================//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <src/fereTypes.h>
#include "swapping.h"
#include "msp.h"

//==========================================//
//******************************************//
// Prototypes								//	
//******************************************//
//==========================================//
Page* applyAlgorithm(String, t_dictionary*);
Page* lru(t_dictionary*);
Page* clock(t_dictionary*);

String buildFileName(Page*);

Boolean saveFrame(Page*);
Boolean getFrame(Page*, Frame*);

//==========================================//
//******************************************//
// Implementations							//	
//******************************************//
//==========================================//

/**
 * Selecciona una page segun el algoritmo de intercambio
 * Mueve el contenido del frame de la pagina seleccionada a disco
 * Si el contenido de la page esta en disco, lo trae y lo coloca en el frame recien liberado asignando el frame a la page
 * Si la page no estaba swapped entonces quiere decir que nunca fue escrita entonces se le asigna el frame para poder escribir
 */
Boolean swap(String algorithm, t_dictionary* processSegments, Page* page) {

	Boolean hasBeenSwapped = swapped(page);

	if (!hasBeenSwapped) {
		if (getSwapCount() * FRAME_SIZE > getSwapSize() - FRAME_SIZE) {
			printf("No hay espacio de swapping\n");
			return FALSE;
		}
	}

	Page* swappingPage = applyAlgorithm(algorithm, processSegments);

	if (swappingPage == NULL) {
		printf("Error en el algoritmo de swapping|\n");
		return FALSE;
	}

	if (!saveFrame(swappingPage)) {
		printf("Error al swappear la pagina\n");
		return FALSE;
	}
	incrementSwapCount();
	swappingPage->swapped = TRUE;

	Frame* frame = swappingPage->frame;
	swappingPage->frame = NULL;

	if (hasBeenSwapped) {
		if (!getFrame(page, frame)) {
			printf("Error al obtener la pagina swappeada\n");
			return FALSE;
		}
		decrementSwapCount();
	} 

	page->frame = frame;
	page->swapped = FALSE;
	return TRUE;

}

Page* applyAlgorithm(String algorithm, t_dictionary* processSegments) {
	if (strcmp(algorithm, LRU) == 0) {
		return lru(processSegments);
	} else if (strcmp(algorithm, CLOCK) == 0) {
		return clock(processSegments);
	}

	return  NULL;
}

/**
 * Crea un archivo en disco con el contenido de la pagina
 */
Boolean saveFrame(Page* page) {
	String fileName = buildFileName(page);
	FILE* file = fopen (fileName, "wb");
    if (file == NULL) {
    	return FALSE;
    }
    
    fwrite (page->frame->address, FRAME_SIZE, 1, file);
    fclose (file);
    return TRUE;
}

/**
 * Obtiene la pagina swapeada leyendo y eliminado el archivo
 * Modifica el contenido de frame con el contenido del archivo
 */
Boolean getFrame(Page* page, Frame* frame) {
	FILE* file;
	String fileName = buildFileName(page);
	file = fopen(fileName, "rb"); 
	if (file == NULL) {
		return FALSE;
	} 
	fread(frame->address, FRAME_SIZE, 1, file); 
	fclose(file);

	if (remove(fileName) == -1) {
		return FALSE;
	}

	return TRUE;
}

Page* lru(t_dictionary* processSegments) {
	//TODO implementar LRU
	return NULL;
}

Page* clock(t_dictionary* processSegments) {
	// TODO implementar CLOCK
	return NULL;
}

String buildFileName(Page* page) {
	String pid = intToStr(page->pid);
	String address = intToStr(page->address);
	String name = malloc(strlen(pid) + strlen(address) + strlen(SWAPPING_FILE_PATH) + strlen(SWAPPING_FILE_EXTENSION) + strlen(UNDERSCORE));
	strcpy(name, SWAPPING_FILE_PATH);
	strcat(name, pid);
	strcat(name, UNDERSCORE);
	strcat(name, address);
	strcat(name, SWAPPING_FILE_EXTENSION);
	return name;
}
