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
	Segments* ptrSegments;
	String pidStr = intToStr(pid);
	
	//me fijo si en la segmentTable tengo una entrada por ese PID
	if (!dictionary_has_key(segmentTable, pidStr)){
		ptrSegments = malloc(sizeof(Segments));
		ptrSegments->segments = dictionary_create();

		dictionary_put(segmentTable, pidStr, ptrSegments);
	}

	//ptrSegments apunta a la tabla de segmentos de ese PID
	ptrSegments = dictionary_get(segmentTable, pidStr);

	//el ID del nuevo segmento va a ser 1 más del ultimo que ya exista
	Int8U segmentId = dictionary_size(ptrSegments);

	//creo las paginas
	Pages* ptrPages = reservePages(size);

	//agrego la lista de Paginas a la tabla de Segmentos de PID
	dictionary_put(ptrSegments, intToStr(segmentId), ptrPages);

	return TRUE;
}
/**
 * Libera la memoria del segmento y las paginas asociadas
 */
Boolean destroySegment(Int32U pid, Int32U segment) {
	String pidStr = intToStr(pid);
	
	if (!dictionary_has_key(segmentTable, pidStr)) {
		printf("No existe ningun segmento para el proceso %s\n", pidStr);
		return FALSE;
	}

	String segmentStr = intToStr(getSegment(segment));
	Segments* segments = dictionary_get(segmentTable, pid);
// TODO SWAPPING
	Pages* pages = dictionary_get(segments->segments, segmentStr);
	list_destroy_and_destroy_elements(pages);
	dictionary_remove_and_destroy(segmentTable, segmentStr);
	free(segmentStr);
	free(pidStr);
	return TRUE;
}

Boolean writeMemory(Int32U pid, Int32U address, Int32U size, Byte* content, &Boolean segFault) {
	String pidStr = intToStr(pid);
	if (!dictionary_has_key(segmentTable, pidStr)) {
		printf("No existe ningun segmento para el proceso %s\n", pidStr);
		return FALSE;
	}

	Int32U offset = getOffset(address);
	Int32U pageNumber = getPage(address);
	Int32U segmentNumber = getSegment(address);
	String segmentStr = intToStr(segmentNumber);

	Segments* segments = dictionary_get(segmentTable, pid);
// TODO SWAPPING
	Pages* pages = dictionary_get(segments->segments, segmentStr);
	Page* page = list_get(pages->pagesList, pageNumber);
	Byte* writeLocation;

	if (size > FRAME_SIZE - 1 - offset) {
		segFault = TRUE;
		printf("Ocurrio un segmentation fault al tratar de escribir en la direccion %d\n", address);
		return FALSE;
	}
	if (page != NULL) {
		if (page ->frame == NULL) {

			// asgignar marco de memoria a la pagina
			if(page->swapped || list_all_satisfy(frames, used)) {
				// TODO SWAPPING
			}
			Frame* frame = list_find(frames, notUsed);
			page->frame = frame;	
			frame->used = TRUE;
		}
		//Actualizo el estado de la pagina respecto de los algoritmos
		page->timestamp = time();
		page->clock = TRUE;
		writeLocation = page->frame->address + offset;
		memcpy(writeLocation, content, size);
	} else {
		printf("No existe la pagina numero %d para el segmento numero %d \n", pageNumber, segmentNumber);
		return FALSE;
		
	}
	return TRUE;	
}


Boolean showPages(Int32U pid) {
	String pidStr = intToStr(pid);
	if (!dictionary_has_key(segmentTable, pidStr)) {
		printf("No existe ningun segmento para el proceso %s\n", pidStr);
		return FALSE;
	}

	Segments* segments = dictionary_get(segmentTable, pid);
	dictionary_iterator(segments, printPagesClosure);
	return TRUE;
}

void printPagesClosure(String key, void* value) {
	printf("Paginas del segmento nro %s\n", key);
	t_list* pages = ((Pages) value)->pagesList;
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

//Recibo un size en BYTES y creo la lista con las paginas para ese size
Pages* reservePages(Int32U size){
	//creo la estructura que 
	Pages* ptrPages= malloc(sizeof(Pages));

	//creo la lista
	ptrPages->pagesList = listCreate();
	
	//top es la cantidad paginas a crear
	Int32U top = size / FRAME_SIZE;
	
	Page* ptrPage = NULL;
	//CREO LAS PAGINAS EN LA LISTA
	for(Int32U index = 0; index < top; index++){
		//Creo un marco
		ptrPage = malloc(sizeof(Page));
		
		//inicializo
		ptrPage->frame = NULL;
		ptrPage->timestamp = time();
		ptrPage->clock = TRUE;
		ptrPage->swapped = FALSE; 

		list_add(ptrPages, ptrPage);
	}

	return ptrPages
}

Boolean used(Frame* frame) {
	return frame->used;
}

Boolean notUsed(Frame* frame) {
	return !frame->used;
}

Int32U getOffset(Int32U address) {
	Int32U offset;
	Int32U mask = 255; // 00000000000000000000000011111111
	offset = address & mask;
	return offset;
}

Int32U getPage(Int32U address) {
	Int32U page;
	Int32U mask = 1048320; // 00000000000011111111111100000000
	mask = mask & address;
	page = mask >> 8;
	return page;
}

Int32U getSegment(Int32U address) {
	Int32U segment;
	Int32U mask = 4293918720; // 11111111111100000000000000000000
	mask = mask & address;
	segment = mask >> 20;
	return segment;
}