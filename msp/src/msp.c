/*
 * msp.c
 *
 *  Created on: 12/09/2014
 *      Author: sampower06 - Leandro Sampayo
 */

//==========================================//
//******************************************//
// Dependencies								//	
//******************************************//
//==========================================//
#include <stdio.h>
#include <stdlib.h>
#include <src/commons/collections/dictionary.h>
#include <src/commons/collections/list.h>
#include "msp.h"


//==========================================//
//******************************************//
// Global Variables							//	
//******************************************//
//==========================================//
Int16U mspPort = 0;
Int16U memLengthKB = 0;
Int16U swapLengthMB = 0;
String swapAlgorithm = "\0";

Byte* memory;
t_dictionary* processSegments = NULL;
t_list* frames = NULL;

//==========================================//
//******************************************//
// Prototypes								//	
//******************************************//
//==========================================//
Int32U getSegment(Int32U);
Int32U getPage(Int32U);
Int32U getOffset(Int32U);
Int32U getPagesCountBySize(Int32U, Int32U);
Boolean notUsed(Frame*);
Boolean used(Frame*);
Boolean swapped(Page*);
Boolean loadConfig();
Boolean nextPage(Page*, Int32U, Int8U, Int32U, Int32U);
Boolean checkSegFault(Int32U, Int32U, Int32U);
Boolean showPages(Int32U);
Segment getSegmentBy(Int32U, Int32U);
Segment reservePages(Int32U);
void initMemory();
void printPages(String, void*);

//==========================================//
//******************************************//
// Main Function							//	
//******************************************//
//==========================================//
int main() {
	printDate();
	if (!loadConfig()){
		return FALSE;
	}
	initMemory();
	while(mspConsole()) {
		//TODO poner la consola en un thread
	}
	return TRUE;
}

/**
 * Imprimo la fecha y hora de inicio del programa
 */
void printDate() {
	String ptrTime = temporal_get_string_time();
	printf("Server time is %s\n\n", ptrTime);
	free (ptrTime);
}

/**
 * Se crea la porcion de memoria y se divide en marcos. 
 * Se la tabla de segmentos
 */
void initMemory() { 
	Int32U memorySize = memLengthKB * 1000; 	
	Int32U framesCount = memorySize / FRAME_SIZE;  // cantidad de marcos que puede llegar a haber
	
	memory = malloc(memorySize);
	frames = list_create();

	Frame* frame = NULL;
	Int32U offset = 0;
	
	//DIVIDO EL GRAN BLOQUE DE MEMORIA EN MARCOS
	for(Int32U index = 0; index < framesCount; index++) {
		frame = malloc(sizeof(Frame));
		frame->pid = 0;
		frame->used = FALSE;
		frame->address = memory + offset;
		offset += FRAME_SIZE;

		list_add_in_index(frames, index, frame);
	}

	processSegments = dictionary_create();
}


/**
 * Se crea el segmento paginado segun el size
 * Si el pid no tiene tabla de segmentos se crea
 */
Boolean createSegment(Int32U pid, Int32U size) {
	SegmentsTable segments;
	String pidStr = intToStr(pid);
	
	//me fijo si en la processSegments tengo una entrada por ese PID
	if (!dictionary_has_key(processSegments, pidStr)){
		segments = malloc(sizeof(SegmentsTable));
		segments->table = dictionary_create();
		segments->lastId = 0;
		dictionary_put(processSegments, pidStr, segments);
	}
	// Se crea el segmento paginado
	Segment segment = reservePages(size);

	segments = dictionary_get(processSegments, pidStr);
	String segmentId = intToStr(segments->lastId);
	segments->lastId++;
	
	dictionary_put(segments->table, segmentId, segment);
	
	free(segmentId);
	free(pidStr);
	return TRUE;
}

/**
 * Devuelve un segmento con la cantidad de paginas correspondientes al size
 */
Segment reservePages(Int32U size) {
	Segment segment = list_create();
	Int32U pageCount = getPagesCountBySize(size, 0); //cantidad paginas a crear

	Page* page = NULL;
	for(Int32U index = 0; index < pageCount; index++) { //Creo las paginas
		page = malloc(sizeof(Page)); 
		page->frame = NULL;
		page->timestamp = time();
		page->clock = TRUE;
		page->swapped = FALSE; 

		list_add_in_index(segment, index, page);
	}
	return segment;
}

/**
 * Libera la memoria del segmento y las paginas asociadas
 */
Boolean destroySegment(Int32U pid, Int32U segmentNumber) {
	String pidStr = intToStr(pid);
	
	if (!dictionary_has_key(processSegments, pidStr)) {
		printf("No existe ningun segmento para el proceso %s\n", pidStr);
		free(pidStr);
		return FALSE;
	}

	String segmentStr = intToStr(segmentNumber);
	if (!dictionary_has_key(segments->table, segmentStr)) {
		printf("No existe el segmento nro %d para el proceso %s\n -- Segmentation Fault\n", getSegment(address), pidStr);
		free(segmentStr);
		free(pidStr);
		return FALSE;
	}
	SegmentsTable segments = dictionary_get(processSegments, pidStr);

	Segment segment = dictionary_get(segments->table, segmentStr);

	while(list_any_satisfy(segment, swapped)) {
		// TODO SWAPPING 
		// Page* page = list_find(segment, swapped); 
		// swappingDestroy(page);
	}

	list_clean_and_destroy_elements(segment, NULL);
	dictionary_remove_and_destroy(segments, segmentStr, NULL);
	dictionary_remove_and_destroy(processSegments, pidStr, NULL);
	free(segmentStr);
	free(pidStr);
	return TRUE;
}

/**
 * Devuelve TRUE si escribio correctamente y FALSE si ocurrio Segmentation Fault
 * Busca pagina por pagina segun el size y el address
 * y para cada una de ellas pisa su contendio con el content
 * para la primera pagina
 */
Boolean writeMemory(Int32U pid, Int32U address, Int32U size, Byte* content) {
	if (checkSegFault(size, address, pid)) {
		printf("Ocurrio un segmentation fault al tratar de escribir en la direccion %d\n", address);
		return FALSE;
	}
	Page* page = NULL;
	Int8U segmentOffset = 0;
	Boolean firstPage = TRUE;
	Byte* ptrContent = content;
	Int32U pagesCount = getPagesCountBySize(size, offset);
	while (nextPage(page, pagesCount, segmentOffset, address, pid)) {
		if (page->frame == NULL) {
			if (page->swapped || list_all_satisfy(frames, used)) {
				// TODO SWAPPING 
				// swapping(page);
			} else {
				Frame* frame = list_find(frames, notUsed);
				page->frame = frame;	
				frame->used = TRUE;
			}
		}
		
		Byte* writeLocation;
		Int8U writeSize;
		if (firstPage) {  										// first page
			writeLocation = page->frame->address + offset;
			writeSize = (size >= FRAME_SIZE - offset) ? FRAME_SIZE - offset : size;
		} else if (pagesCount == segmentOffset + 1) { 			// last page
			writeLocation = page->frame->address;
			writeSize = size - FRAME_SIZE * (pagesCount - 1);
		} else { 												// middle page
			writeLocation = page->frame->address;
			writeSize = (size >= FRAME_SIZE) ? FRAME_SIZE : size;
		}

		memcpy(writeLocation, ptrContent, writeSize);
		ptrContent += writeSize;
		
		page->timestamp = time();
		page->clock = TRUE;

		firstPage = FALSE;
		segmentOffset++;
	}

	return TRUE;	
}

/**
 * Busco la siguiente pagina segun el desplazamiento dentro del segmento, la direccion y el tamaño del dato
 * Devuelvo FALSE si ya accedio a todas las paginas segun el segmentOffset y el size
 */
Boolean nextPage(Page* page, Int32U pagesCount, Int8U segmentOffset, Int32U address, Int32U pid) {
	Int32U pageNumber = getPage(address);
	if (pagesCount == segmentOffset) {
		return FALSE;
	}
	Segment segment = getSegmentBy(address, pid);
	page = list_get(segment, pageNumber + segmentOffset);
	return TRUE;
}

/**
 * obtengo el segmento que contiene la lista de paginas correspondiente al address y pid
 * Devuelve NULL si el segmento no existe
 */
Segment getSegmentBy(Int32U address, Int32U pid) {
	String pidStr = intToStr(pid);
	Int32U segmentNumber = getSegment(address);
	SegmentsTable segments = dictionary_get(processSegments, pidStr);
	String segmentStr = intToStr(segmentNumber);
	Segment segment = dictionary_get(segments->table, segmentStr);
	free(pidStr);
	free(segmentStr);
	return segment;
}

/**
 * Obtiene la cantidad de paginas a la que se quiere acceder segun el size y offset
 */
Int32U getPagesCountBySize(Int32U size, Int32U offset) {
	Int32U sizeTotal = size + offset;
	Int32U pagesCount = sizeTotal / FRAME_SIZE;
	pagesCount += (sizeTotal % FRAME_SIZE != 0) ? 1 : 0;
	return pagesCount;	
}

/**
 * Se fija la cantidad de paginas a las que tiene que acceder,
 * a partir de cual y con que desplazamiento.
 * Si no existe alguna de esas paginas devuelve TRUE porque ocurre segmentation fault
 * 
 * Esta funcion recibe como parametros:
 * 1. Int32U size - Es el tamaño de lo que quiero leer o escribir
 * 2. Int32U address - la direccion logica de donde quiere leer o escribir
 * 3. Int32U pid - identificador del proceso
 */
Boolean checkSegFault(Int32U size, Int32U address, Int32U pid) {
	String pidStr = intToStr(pid);
	if (!dictionary_has_key(processSegments, pidStr)) {
		printf("No existe ningun segmento para el proceso %s\n -- Segmentation Fault\n", pidStr);
		free(pidStr);
		return TRUE;
	}

	Segment segment = getSegmentBy(address, pid);
	if (segment == NULL) {
		printf("No existe el segmento nro %d para el proceso %s\n -- Segmentation Fault\n", getSegment(address), pidStr);
		free(pidStr);
		return TRUE;
	}

	Int32U offset = getOffset(address);
	Int32U pagesCount = getPagesCountBySize(size, offset);
	Int32U pageNumber = getPage(address);


	if (list_size(segment) < pageNumber + pagesCount) {
		printf("Tratando de acceder a la pagina nro %d cuando tan solo hay %d\n -- Segmentation Fault\n", pageNumber + pagesCount - 1, list_size(segment) - 1);
		free(pidStr);
		return TRUE;
	}

	free(pidStr);
	return FALSE;
}

/**
 * Itera la tabla de segmentos del pid, y para cada uno ejecuta la funcion printPages
 */
Boolean showPages(Int32U pid) {
	String pidStr = intToStr(pid);
	if (!dictionary_has_key(processSegments, pidStr)) {
		printf("No existe ningun segmento para el proceso %s\n", pidStr);
		return FALSE;
	}
	SegmentsTable segments = dictionary_get(processSegments, pidStr);
	dictionary_iterator(segments->table, printPages);
	return TRUE;
}

/**
 * Imprime por pantalla nro de pagina, swapped y last time para cada una de las paginas del segmento 'segment'
 */
void printPages(String segmentNumber, void* segment) {
	printf("Paginas del segmento nro %s\n\n", segmentNumber);
	for (Int16U i = 0; i < list_size((Segment) segment, i++) {
		Page* page = list_get((Segment) segment, i);
		printf("Nro de pagina: %d, swapped: %s, last time: %d\n", i, page->swapped ? "Si": "No", page->timestamp);
	}
}


/**
 * Devuelve TRUE si la pagina esta swappeada
 */
Boolean swapped(Page* page) {
	return page->swapped;
}

/**
 * Devuelve TRUE si el marco esta siendo usado
 */
Boolean used(Frame* frame) {
	return frame->used;
}

/**
 * Devuelve TRUE si el marco esta libre
 */
Boolean notUsed(Frame* frame) {
	return !frame->used;
}

/**
 * obtengo el offset correspondiente al address
 */
Int32U getOffset(Int32U address) {
	Int32U offset;
	Int32U mask = 255; // 00000000000000000000000011111111
	offset = address & mask;
	return offset;
}

/**
 * obtengo el numero de pagina correspondiente al address
 */
Int32U getPage(Int32U address) {
	Int32U page;
	Int32U mask = 1048320; // 00000000000011111111111100000000
	mask = mask & address;
	page = mask >> 8;
	return page;
}

/**
 * obtengo el numero de segmento correspondiente al address
 */
Int32U getSegment(Int32U address) {
	Int32U segment;
	Int32U mask = 4293918720; // 11111111111100000000000000000000
	mask = mask & address;
	segment = mask >> 20;
	return segment;
}

/**
 * Carga las variables de configuracion externa
 */
Boolean loadConfig() {

	//Gennero tabla de configuracion
	t_config * tConfig = config_create(CONFIG_FILE);
	if (tConfig == NULL){
		printf("ERROR: No se encuentra o falta el archivo de configuracion.\n");
		return FALSE;
	}
	//Verifico consistencia, tiene que haber 4 campos
	if (config_keys_amount(tConfig) == PARAM_LENGTH){

		//Verifico que los parametros tengan sus valores OK-> PORT=12456 | NOK->PORT=
		if (config_has_property(tConfig, MSP_PORT)){
			mspPort = config_get_int_value(tConfig, MSP_PORT);
		}else{
			printf("ERROR: Falta un parametro.\n");
			return FALSE;
		}

		if (config_has_property(tConfig, MEM_LENGTH)){
			memLengthKB = config_get_int_value(tConfig, MEM_LENGTH);
		}else{
			printf("ERROR: Falta un parametro.\n");
			return FALSE;
		}

		if (config_has_property(tConfig, SWAP_LENGTH)){
			swapLengthMB = config_get_int_value(tConfig, SWAP_LENGTH);
		}else{
			printf("ERROR: Falta un parametro.\n");
			return FALSE;
		}

		if (config_has_property(tConfig, SWAP_ALGORITHM)){
			swapAlgorithm = config_get_string_value(tConfig, SWAP_ALGORITHM);
		}else{
			printf("ERROR: Falta un parametro.\n");
			return FALSE;
		}

		//Libero tabla config
		config_destroy(tConfig);
		if (DEBUG == 1){
			printf("Archivo de config MSP leído\n===========\n");
			printf("MSP Port: %u\nMemory Length: %ukB\nSwap Memory Length: %uMB\nSwap Algorithm: %s\n",
					mspPort, memLengthKB, swapLengthMB, swapAlgorithm);
		}

		return TRUE;
	}
	else{
		printf("El archivo config.txt no tiene todos los campos.\n");
		return TRUE;
	}
}