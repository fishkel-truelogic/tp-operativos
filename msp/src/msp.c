/*
 * msp.c
 *
 *  Created on: 12/09/2014
 *      Author: sampower06 - Leandro Sampayo
 */

#include "msp.h"

//GLOBAL VARIABLES
Int16U mspPort = 0;
Int16U memLengthKB = 0;
Int16U swapLengthMB = 0;
String swapAlgorithm = "\0";

Byte* memory;
t_dictionary* segmentTable = NULL;
t_list* frames = NULL;

int main() {

	//Imprimo hora al arrancar
	Char * ptrTime = temporal_get_string_time();
	printf("Server time is %s\n\n", ptrTime);
	free (ptrTime);

	//INICIALIZO, LEO ARCHIVO DE CONFIGURACION
	if (!loadConfig()){
		return FALSE;
	}

	//INICIALIZO ESTRUTURAS ADMINISTRATIVAS
	initMemory();



	while(TRUE){
		mspConsole();


	}
	return 0;

}


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
			//strcat(swapAlgorithm, "\0");
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

//Hago el malloc inicial, divido en marcos y creo la tabla de segmentos
Boolean initMemory(void){
	
	//malloc reserva en bytes, reservo el gran bloque inicial
	memory = malloc( memLengthKB * 1000);
	
	//creo el diccionario de frames
	frames = list_create();

	//top es la cantidad de marcos que puede llegar a haber
	Int32U top = sizeof(memory) / FRAME_SIZE;

	Frame* ptrFrame=NULL;
	Int32U offset=0;
	
	//DIVIDO EL GRAN BLOQUE DE MEMORIA EN MARCOS
	for(Int32U index = 0; index < top; index++){
		//Creo un marco
		ptrFrame = malloc(sizeof(Frame));
		//esta libre => True
		ptrFrame->pid=0;
		//esta libre => True
		ptrFrame->used=FALSE;
		//La dir de Mem Principal va a ser + el offset de los que ya cree
		prtFrame->address= memory+offset;
		offset += FRAME_SIZE;
		//agrego al dicc
		list_add_in_index(frames, index, ptrFrame);
	}

	segmentTable = dictionary_create();

	return TRUE;
}


//Creo un segmento en mi estrucutras administrativas
Boolean createSegment(Int32U pid, Int32U size){
	SegmentsTable* ptrSegments;
	String pidStr = intToStr(pid);
	
	//me fijo si en la segmentTable tengo una entrada por ese PID
	if (!dictionary_has_key(segmentTable, pidStr)){
		ptrSegments = malloc(sizeof(SegmentsTable));
		ptrSegments->segments = dictionary_create();

		dictionary_put(segmentTable, pidStr, ptrSegments);
	}

	//ptrSegments apunta a la tabla de segmentos de ese PID
	ptrSegments = dictionary_get(segmentTable, pidStr);

	//el ID del nuevo segmento va a ser 1 más del ultimo que ya exista
	Int8U segmentId = dictionary_size(ptrSegments);

	//creo las paginas
	Segment* ptrPages = reservePages(size);

	//agrego la lista de Paginas a la tabla de Segmentos de PID
	dictionary_put(ptrSegments, intToStr(segmentId), ptrPages);

	return TRUE;
}

/**
 * Libera la memoria del segmento y las paginas asociadas
 */
Boolean destroySegment(Int32U pid, Int32U segmentNumber) {
	String pidStr = intToStr(pid);
	
	if (!dictionary_has_key(segmentTable, pidStr)) {
		printf("No existe ningun segmento para el proceso %s\n", pidStr);
		return FALSE;
	}

	String segmentStr = intToStr(segmentNumber);
	SegmentsTable* segments = dictionary_get(segmentTable, pid);
	Segment* segment = dictionary_get(segments->segments, segmentStr);
	while(list_any_satisfy(segment->pagesList, swapped)) {
		// TODO SWAPPING
	}

	list_destroy_and_destroy_elements(segment->pagesList);
	dictionary_remove_and_destroy(segmentTable, segmentStr);
	free(segmentStr);
	free(pidStr);
	return TRUE;
}

/**
 * Devuelve TRUE si escribio correctamente
 * Setea  el flag segFault en TRUE si ocurre segmentation fault
 * Busca pagina por pagina segun el size y el address
 * y para cada una de ellas pisa su contendio con el content
 * para la primera pagina
 */
Boolean writeMemory(Int32U pid, Int32U address, Int32U size, Byte* content, &Boolean segFault) {
	if (*segFault = checkSegFault(size, address, pid)) {
		printf("Ocurrio un segmentation fault al tratar de escribir en la direccion %d\n", address);
		return FALSE;
	}
	Page* page = NULL;
	Int8U segmentOffset = 0;
	Boolean firstPage = TRUE;
	Byte* ptrContent = content;
	Int8U pagesToRead = getPagesToRead(size, offset);
	while (nextPage(page, size, segmentOffset, address, offset, pid)) {
		if (page->frame == NULL) {
			if (page->swapped || list_all_satisfy(frames, used)) {
				// TODO SWAPPING
			} else {
				Frame* frame = list_find(frames, notUsed);
				page->frame = frame;	
				frame->used = TRUE;
			}
		}
		
		Byte* writeLocation;
		Int8U writeSize;
		if (firstPage) {
			writeLocation = page->frame->address + offset;
			writeSize = FRAME_SIZE - offset;
		} else if (pagesToRead == segmentOffset + 1) {
			writeLocation = page->frame->address;
			writeSize = size - FRAME_SIZE * (pagesToRead - 1);
		} else {
			writeLocation = page->frame->address;
			writeSize = FRAME_SIZE;
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
 */
Boolean nextPage(Page* page, Int32U size, Int8U segmentOffset, Int32U address, Int32U offset, Int32U pid) {
	Int32U pageNumber = getPage(address);
	Int8U pagesToRead = getPagesToRead(size, offset);
	if (pagesToRead == segmentOffset) {
		return FALSE;
	}
	Segment* segment = getSegmentBy(address, pid);
	page = list_get(segment->pagesList, pageNumber + segmentOffset);
	return TRUE;
}

/**
 * obtengo el segmento que contiene la lista de paginas correspondiente al address y pid
 */
Segment* getSegmentBy(Int32U address, Int32U pid) {
	String pidStr = intToStr(pid);
	Int32U offset = getOffset(address);
	Int32U segmentNumber = getSegment(address);
	String segmentStr = intToStr(segmentNumber);
	SegmentsTable* segments = dictionary_get(segmentTable, pidStr);
	free(pidStr);
	Segment* segment = dictionary_get(segments->segments, segmentStr);
	free(segmentStr);
	return segment;
}

/**
 * Obtiene la cantidad de paginas a la que se quiere acceder segun el size y offset
 */
Int8U getPagesToRead(Int32U size, Int32U offset) {
	// calculo la cantidad a paginas a leer segun el size
	Int32U sizeTotal = size + offset;
	Int8U pagesToRead = sizeTotal / FRAME_SIZE;
	pagesToRead += (sizeTotal % FRAME_SIZE != 0) ? 1 : 0;
	return pagesToRead;	
}

/**
 * Esta funcion recibe como parametros:
 * 1. Int32U size - Es el tamaño de lo que quiero leer o escribir
 * 2. Int32U address - la direccion logica de donde quiere leer o escribir
 * 3. Int32U pid - identificador del proceso
 * 
 * Se fija la cantidad de paginas a las que tiene que acceder,
 * a partir de cual y con que desplazamiento.
 * Si no existe alguna de esas paginas devuelve TRUE porque ocurre segmentation fault
 */
Boolean checkSegFault(Int32U size, Int32U address, Int32U pid) {
	String pidStr = intToStr(pid);
	if (!dictionary_has_key(segmentTable, pidStr)) {
		printf("No existe ningun segmento para el proceso %s\n -- Segmentation Fault\n", pidStr);
		return TRUE;
	}

	Int32U offset = getOffset(address);
	Int32U pageNumber = getPage(address);
	Int8U pagesToRead = getPagesToRead(size, offset);

	Segment* segment = getSegmentBy(address, pid);

	if (list_size(segment->pagesList) < pageNumber + pagesToRead) {
		printf("Tratando de acceder a la pagina nro %d cuando tan solo hay %d\n -- Segmentation Fault\n", pageNumber + pagesToRead - 1, list_size(segment->pagesList) - 1);
		return TRUE;
	}
	return FALSE;
}

Boolean showPages(Int32U pid) {
	String pidStr = intToStr(pid);
	if (!dictionary_has_key(segmentTable, pidStr)) {
		printf("No existe ningun segmento para el proceso %s\n", pidStr);
		return FALSE;
	}

	SegmentsTable* segments = dictionary_get(segmentTable, pidStr);
	dictionary_iterator(segments, printPages);
	return TRUE;
}

void printPages(String key, void* value) {
	printf("Paginas del segmento nro %s\n", key);
	t_list* segment = ((Segment) value)->pagesList;
	for (Int8U = 0; i < FRAME_SIZE, i++) {
		Page* page = list_get(i);
		String pageContent;
		if (page->frame != NULL) {
			pageContent = malloc(FRAME_SIZE);
			memcpy(pageContent, page->frame->address, FRAME_SIZE);
			
		}  else {
			pageContent = "";
		}
		printf("Nro de pagina: %d, swapped: %s, last time: %d, content: %s\n", i, page->swapped ? "Si": "No", page->timestamp, pageContent);
	}
}

/**
 * Devuelve un segmento con la cantidad de paginas correspondientes al size
 */
Segment* reservePages(Int32U size) {
	//creo la estructura que 
	Segment* segment = malloc(sizeof(Segment));

	//creo la lista
	segment->pagesList = listCreate();
	
	//top es la cantidad paginas a crear
	Int32U top = size / FRAME_SIZE;
	
	Page* page = NULL;
	//CREO LAS PAGINAS EN LA LISTA
	for(Int32U index = 0; index < top; index++){
		//Creo una pagina
		page = malloc(sizeof(Page));
		
		//inicializo
		page->frame = NULL;
		page->timestamp = time();
		page->clock = TRUE;
		page->swapped = FALSE; 

		list_add(segment->pagesList, page);
	}

	return segment;
}

/**
 * Devuelve TRUE si la pagina esta swappeada
 */
Boolean used(Page* page) {
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