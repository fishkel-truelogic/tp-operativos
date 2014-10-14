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
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <src/fereTypes.h>
#include <src/fereSockets.h>
#include <src/fereStream.h>
#include <src/commons/config.h>
#include <src/commons/collections/dictionary.h>
#include <src/commons/collections/list.h>
#include "msp.h"
#include "swapping.h"


//==========================================//
//******************************************//
// Global Variables							//	
//******************************************//
//==========================================//
Int16U mspPort = 0;
Int16U memLengthKB = 0;
Int32U swapLengthMB = 0;
String swapAlgorithm = "\0";

Int32U swapCount = 0;

Byte* memory;
t_dictionary* processSegments = NULL;
t_list* frames = NULL;
t_list* socketConnections = NULL;
t_list* swappedPages = NULL;
//==========================================//
//******************************************//
// Prototypes								//	
//******************************************//
//==========================================//
Int32U getPagesCountBySize(Int16U, Int32U);
Int16U getCurrentTime();
Int32U getAddress(Int32U, Int32U, Int32U);

bool notUsed(void*);
bool used(void*);
void destroy(void*);


void initMemory();
Boolean loadConfig();

void setFrameNotUsed(void*);
Boolean nextPage(Page**, Int32U, Int32U, Int32U, Int32U);
Boolean checkSegFault(Int16U, Int32U, Int32U);
Segment getSegmentBy(Int32U, Int32U);
Segment reservePages(Int32U, Int32U, Int32U);

void printPages(String, void*);
void printFrame(void*);
void printSegment(String, void*);
void printSegments(String, void*);
void printDate();


void* mspConsoleThread(void*);
void manageSocketConnections();
void* manageSocketConnection(void*);
Boolean manageCpuRequest(Socket*, StrCpuMsp*);
Boolean manageKernelRequest(Socket*, StrKerMsp*);
Boolean sendResponse(Char, void*, Socket*);


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
	pthread_t consoleThread;
	void* (*consoleFunc)(void*) = mspConsoleThread;
	pthread_create(&consoleThread, NULL, consoleFunc, NULL);
	manageSocketConnections();
	pthread_join(consoleThread, NULL);
	return 0;
}

void manageSocketConnections() {
	socketConnections = list_create();
	Socket* s = socketCreateServer(mspPort);
	while (TRUE) {
		pthread_t* socketConnection = NULL;
		if (socketListen(s)) {
			Socket* socketClient = socketAcceptClient(s);
			void* (*manageSocketConnectionFunc)(void*) = manageSocketConnection;
			pthread_create(socketConnection, NULL, manageSocketConnectionFunc, (void*) socketClient);
			list_add(socketConnections, socketConnection);
		}
	}
}

void* manageSocketConnection(void* param) {
	Socket* socket = (Socket*) param;
	Boolean connected = TRUE;
	while (connected) {
		SocketBuffer* sb = socketReceive(socket);
		if (sb != NULL) {
			Char id = getStreamId((Stream) sb->data);
			StrKerMsp* skm = NULL;
			StrCpuMsp* scm = NULL;
			switch (id) {
				case KERNEL_ID:
					skm = unserializeKerMsp((Stream) sb->data);
					connected = manageKernelRequest(socket, skm);
					free(skm);
					break;
				case CPU_ID:
					scm = unserializeCpuMsp((Stream) sb->data);
					connected = manageCpuRequest(socket, scm);
					free(scm);
					break;
				default:
					connected = FALSE;
					break;
			}

		} else {
			connected = FALSE;
		}
	}
	
	return NULL;
}

Boolean manageCpuRequest(Socket* socket, StrCpuMsp* scm) {
	Byte* data = NULL;
	Boolean segFault = FALSE;
	Boolean memFull = FALSE;
	Int32U address;
	Char action;
	switch (scm->action) {
		case MEM_READ:
			data = readMemory(scm->pid, scm->address, scm->dataLen, &segFault);
			break;
		case MEM_WRITE:
			segFault = writeMemory(scm->pid, scm->address, scm->dataLen, scm->data);
			break;
		case CREATE_SEG:
			address = createSegment(scm->pid, scm->dataLen, &memFull);
			break;
		case DELETE_SEG:
			segFault = destroySegment(scm->pid, getSegment(scm->address));
			break;
		default:
			break;
	}

	if (segFault) {
		action = SEG_FAULT;	
	} else if (memFull) {
		action = MEM_FULL;
	} else {
		action = scm->action;
	}

	StrMspCpu* smc = newStrMspCpu(scm->dataLen, data, action, address);
	Boolean result = sendResponse(CPU_ID, smc, socket);

	free(smc);
	return result;

}

Boolean manageKernelRequest(Socket* socket, StrKerMsp* skm) {
	Boolean segFault = FALSE;
	Boolean memFull = FALSE;
	Int32U address;
	Char action;
	switch (skm->action) {
		case MEM_WRITE:
			segFault = writeMemory(skm->pid, skm->address, skm->size, skm->data);
			break;
		case CREATE_SEG:
			address = createSegment(skm->pid, skm->size, &memFull);
			break;
		case DELETE_SEG:
			segFault = destroySegment(skm->pid, getSegment(skm->address));
			break;
		default:
			break;
	}

	if (segFault) {
		action = SEG_FAULT;	
	} else if (memFull) {
		action = MEM_FULL;
	} else {
		action = skm->action;
	}

	StrMspKer* smk = newStrMspKer(MSP_ID, address, action);
	Boolean result = sendResponse(KERNEL_ID, smk, socket);

	free(smk);
	return result;
}

void* mspConsoleThread(void* param) {
	while(mspConsole()) {
		puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	}
	return NULL;
}

Boolean sendResponse(Char target, void* stream, Socket* socket) {
	
	SocketBuffer* sb = NULL;

	//Serializo 
	switch (target) {
		case CPU_ID:
			sb = serializeMspCpu((StrMspCpu*) stream);
			break;
		case KERNEL_ID:
			sb = serializeMspKer((StrMspKer*) stream);
			break;
		default:
			return FALSE;
	}

	//Envio response
	if(!socketSend(socket, sb)) {
		printf("No se pudo enviar el Stream. \n");
		free(sb);
		return FALSE;
	}
	free(sb);

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
	Int32U index;
	
	//DIVIDO EL GRAN BLOQUE DE MEMORIA EN MARCOS
	for(index = 0; index < framesCount; index++) {
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
Int32U createSegment(Int32U pid, Int32U size, Boolean* memFull) {
	SegmentsTable* segments;
	String pidStr = intToStr(pid);

	Int32U framesBySize = size / FRAME_SIZE;
	framesBySize += (size % FRAME_SIZE == 0) ? 0 : 1;
	bool (*fptrNotUsed)(void*) = notUsed;
	Int32U freeFrames = list_count_satisfying(franes, fptrNotUsed);

	if (framesBySize > freeFrames) {
		if (getSwapCount() * FRAME_SIZE > getSwapSize() - FRAME_SIZE * framesBySize) {
			*memFull = TRUE;
			return 0;
		}		
	}
	*memFull = FALSE;

	//me fijo si en la processSegments tengo una entrada por ese PID
	if (!dictionary_has_key(processSegments, pidStr)){
		segments = malloc(sizeof(SegmentsTable));
		segments->table = dictionary_create();
		segments->lastId = 0;
		dictionary_put(processSegments, pidStr, segments);
	}

	segments = dictionary_get(processSegments, pidStr);
	String segmentId = intToStr(segments->lastId);
	// Se crea el segmento paginado
	Segment segment = reservePages(size, segments->lastId, pid);
	
	segments->lastId++;
	
	dictionary_put(segments->table, segmentId, segment);
	
	free(pidStr);
	return getAddress(segments->lastId - 1, 0, 0);
}

/**
 * Devuelve un segmento con la cantidad de paginas correspondientes al size
 */
Segment reservePages(Int32U size, Int32U segmentId, Int32U pid) {
	Segment segment = list_create();
	Int32U pageCount = getPagesCountBySize(size, 0); //cantidad paginas a crear
	Int32U index;
	Page* page = NULL;
	for(index = 0; index < pageCount; index++) { //Creo las paginas
		page = malloc(sizeof(Page)); 
		page->frame = NULL;
		page->timestamp = getCurrentTime();
		page->clock = TRUE;
		page->swapped = FALSE; 
		page->address = getAddress(segmentId, index, 0);
		page->pid = pid;
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
	SegmentsTable* segments = dictionary_get(processSegments, pidStr);
	if (!dictionary_has_key(segments->table, segmentStr)) {
		printf("No existe el segmento nro %d para el proceso %s\n -- Segmentation Fault\n", segmentNumber, pidStr);
		free(segmentStr);
		free(pidStr);
		return FALSE;
	}

	Segment segment = dictionary_get(segments->table, segmentStr);

	bool (*fptrSwapped)(void*) = swapped;
	while (list_any_satisfy(segment, fptrSwapped)) {
		Page* page = list_find(segment, fptrSwapped); 
		swap(swapAlgorithm, processSegments, page);

	}

	void (*setFrameNotUsedFunc)(void*) = setFrameNotUsed;
	list_iterate(segment, setFrameNotUsedFunc);

	void (*destroyFunc)(void*) = destroy;
	list_destroy_and_destroy_elements(segment, destroyFunc);
	dictionary_remove_and_destroy(segments->table, segmentStr, destroyFunc);
	free(segmentStr);
	free(pidStr);
	return TRUE;
}


/** Solicitar Memoria (PID, Dirección Lógica, Tamaño): Para el espacio de direcciones del proceso PID,
* devuelve la cantidad en bytes Tamaño comenzando desde Dirección Lógica. En caso que el pedido 
* intente solicitar datos desde una posición de memoria inválida o que el mismo exceda los límites 
* del segmento, retornará el correspondiente error de Violación de Segmento (Segmentation Fault).
*/
Byte* readMemory(Int32U pid, Int32U address, Int16U size, Boolean* segFault) {
	if (checkSegFault(size, address, pid)) {
		*segFault = TRUE;
		printf("Ocurrio un segmentation fault al tratar de leer en la direccion %d\n", address);
		return NULL;
	}
	Page* page = NULL;
	Int32U offset = getOffset(address);
	Int32U segmentOffset = 0;
	Boolean firstPage = TRUE;
	Int32U pagesCount = getPagesCountBySize(size, offset);

	Byte* read = malloc(sizeof(Byte) * size);
	Byte* ptrRead = read;
	Byte* readLocation;
	Int16U readSize;

	while (nextPage(&page, pagesCount, segmentOffset, address, pid)) {
		if (page->frame == NULL) {
			if (page->swapped) {
				swap(swapAlgorithm, processSegments, page);
			} else {
				ptrRead += FRAME_SIZE;
				continue;
			}
		}
		if (firstPage) {  										// first page
			readLocation = page->frame->address + offset;
			readSize = (size >= FRAME_SIZE - offset) ? FRAME_SIZE - offset : size;
		} else if (pagesCount == segmentOffset + 1) { 			// last page
			readLocation = page->frame->address;
			readSize = size - FRAME_SIZE * (pagesCount - 1);
		} else { 												// middle page
			readLocation = page->frame->address;
			readSize = (size >= FRAME_SIZE) ? FRAME_SIZE : size;
		}
		memcpy(ptrRead, readLocation, readSize);
		ptrRead += readSize;

		page->timestamp = getCurrentTime();
		page->clock = TRUE;

		firstPage = FALSE;
		segmentOffset++;
	}
	return read;
}

/**
 * Devuelve TRUE si escribio correctamente y FALSE si ocurrio Segmentation Fault
 * Busca pagina por pagina segun el size y el address
 * y para cada una de ellas pisa su contendio con el content
 * para la primera pagina
 */
Boolean writeMemory(Int32U pid, Int32U address, Int16U size, Byte* content) {
	if (checkSegFault(size, address, pid)) {
		printf("Ocurrio un segmentation fault al tratar de escribir en la direccion %d\n", address);
		return FALSE;
	}
	Page* page = NULL;
	Int32U offset = getOffset(address);
	Int32U segmentOffset = 0;
	Boolean firstPage = TRUE;
	Byte* ptrContent = content;
	Int32U pagesCount = getPagesCountBySize(size, offset);
	bool (*fptrUsed)(void*) = used;
	bool (*fptrNotUsed)(void*) = notUsed;

	while (nextPage(&page, pagesCount, segmentOffset, address, pid)) {
		if (page->frame == NULL) {
			if (page->swapped || list_all_satisfy(frames, fptrUsed)) {
				swap(swapAlgorithm, processSegments, page);
			} else {
				Frame* frame = list_find(frames, fptrNotUsed);
				page->frame = frame;	
				frame->used = TRUE;
				frame->pid = pid;
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
		
		page->timestamp = getCurrentTime();
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
Boolean nextPage(Page** page, Int32U pagesCount, Int32U segmentOffset, Int32U address, Int32U pid) {
	Int32U pageNumber = getPage(address);
	if (pagesCount == segmentOffset) {
		return FALSE;
	}
	Segment segment = getSegmentBy(address, pid);
	*(page) = list_get(segment, pageNumber + segmentOffset);
	return TRUE;
}

/**
 * obtengo el segmento que contiene la lista de paginas correspondiente al address y pid
 * Devuelve NULL si el segmento no existe
 */
Segment getSegmentBy(Int32U address, Int32U pid) {
	String pidStr = intToStr(pid);
	Int32U segmentNumber = getSegment(address);
	SegmentsTable* segments = dictionary_get(processSegments, pidStr);
	String segmentStr = intToStr(segmentNumber);
	Segment segment = dictionary_get(segments->table, segmentStr);
	free(pidStr);
	free(segmentStr);
	return segment;
}

/**
 * Obtiene la cantidad de paginas a la que se quiere acceder segun el size y offset
 */
Int32U getPagesCountBySize(Int16U size, Int32U offset) {
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
Boolean checkSegFault(Int16U size, Int32U address, Int32U pid) {
	String pidStr = intToStr(pid);
	if (!dictionary_has_key(processSegments, pidStr)) {
		printf("No existe ningun segmento para el proceso %s\n -- Segmentation Fault\n", pidStr);
		free(pidStr);
		return TRUE;
	}

	Segment segment = getSegmentBy(address, pid);
	if (segment == NULL) {
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
	SegmentsTable* segments = dictionary_get(processSegments, pidStr);
	dictionary_iterator(segments->table, printPages);
	return TRUE;
}

void showSegments() {
	dictionary_iterator(processSegments, printSegments);
}

void showFrames() {
	list_iterate(frames, printFrame);
	printf("\n\n Aqui esta el detalle de las paginas swappeadas:\n");
	list_iterate(swappedPages, printSwappedFrame);
}

void printFrame(void* frame) {
	Frame* f = (Frame*) frame;
	Int32U pid;
	String usedStr = "No";
	Byte* direccion = NULL;
	if (f->used) {
		pid = f->pid;
		usedStr = "Si";
		direccion = f->address;
		printf("Nro de proceso: %d, usado: %s, Address: %p \n", pid, usedStr, direccion);
	} else {
		printf("Usado: %s, Address: %p \n", usedStr, direccion);
	}
}

void printSwappedFrame(void* fileName) {
	printf("%s\n", (String) fileName);
}

/**
 * Imprime por pantalla nro de pagina, swapped y last time para cada una de las paginas del segmento 'segment'
 */
void printPages(String segmentNumber, void* segment) {
	printf("\nPaginas del segmento nro %s", segmentNumber);
	puts("\n=========================\n");
	Int16U i;
	for (i = 0; i < list_size((Segment) segment); i++) {
		Page* page = list_get((Segment) segment, i);
		printf("Nro de pagina: %d, swapped: %s, last time: %d\n", i, page->swapped ? "Si": "No", page->timestamp);
	}
	puts("\n=========================\n");
}

/**
 * Imprime por pantalla nro de pagina, swapped y last time para cada una de las paginas del segmento 'segment'
 */
void printSegments(String pid, void* segments) {
	printf("Segmentos del proceso nro %s", pid);
	puts("\n=========================\n");
	SegmentsTable* segmentsTable = (SegmentsTable*) segments;
	dictionary_iterator(segmentsTable->table, printSegment);
	puts("\n=========================\n");
}

void printSegment(String sid, void* segment) {
	Int32U size = list_size((Segment) segment) * 256;
	printf("Nro de segmento: %s, tamanio: %d\n", sid, size);
}



/**
 * Devuelve TRUE si la pagina esta swappeada
 */
bool swapped(void* page) {
	Page* ptr;
	ptr = (Page*)page;
	return ptr->swapped;
}

/**
 * Devuelve TRUE si el marco esta siendo usado
 */
bool used(void* frame) {
	Frame* ptr;
	ptr = (Frame*)frame;
	return ptr->used;
}

/**
 * Devuelve TRUE si el marco esta libre
 */
bool notUsed(void* frame) {
	Frame* ptr;
	ptr = (Frame*)frame;
	return !ptr->used;
}

void setFrameNotUsed(void* page) {
	Page* ptr;
	ptr = (Page*) page;
	if (ptr->frame != NULL) {
		ptr->frame->used = FALSE;
	}
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
	// 11111111111100000000000000000000

	// 0xfff00000
	Int32U mask = 0xfff00000;
	mask = mask & address;
	segment = mask >> 20;
	return segment;
}

Int32U getAddress(Int32U segmentNumber, Int32U pageNumber, Int32U offset) {
	Int32U address = 0;
	segmentNumber <<= 20;
	address += segmentNumber;
	pageNumber <<= 8;
	address += pageNumber;
	address += offset;
	return address; 
}

/**
 * Convierte un int en un String
 */
String intToStr(Int32U integer) {
	String result = malloc(sizeof(Byte) * 10);
	sprintf(result, "%d", integer);
	return result;
}

/**
 * Obtiene la fecha actual en milisegundos
 */
Int16U getCurrentTime() {
	return 0;
}

/**
 * Destruye una pagina (sirve para pasar como parametro a las funciones de lista)
 */
void destroy(void* element) {
	free(element);
}

Int32U getSwapSize() {
	return swapLengthMB * 1024 * 1024;
}

Int32U getSwapCount() {
	return swapCount;
}
void incrementSwapCount() {
	swapCount++;
}

void decrementSwapCount() {
	swapCount--;
}

t_list* getSwappedPages() {
	if (swappedPages == NULL) {
		swappedPages = list_create();
	}
	return swappedPages;
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
