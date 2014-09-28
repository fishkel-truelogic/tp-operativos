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
t_dictionary* frames = NULL;

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
	//initMemory();



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
	frames = dictionary_create();

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
		dictionary_put(frames, intToStr(index), ptrFrame);
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
	Page* ptrPages = reservePages(size);

	//agrego la lista de Paginas a la tabla de Segmentos de PID
	dictionary_put(ptrSegments, intToStr(segmentId), ptrPages);

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

		list_add(ptrPages, ptrPage);
	}

	return ptrPages
}

Int32U getOffset(Int32U address) {
	Int32U offset;
	Int32U mask = 4293918720; // last 12 bits
	offset = address & mask;
	return offset;
}

Int32U getPage(Int32U address) {
	Int32U page;
	Int32U mask = 1048320; //12 middle bits
	mask = mask & address;
	page = mask >> 8;
	return page;
}

Int32U getSegment(Int32U address) {
	Int32U segment;
	Int32U mask = 255; // first 8 bits
	mask = mask & address;
	segment = mask >> 20;
	return segment;
}