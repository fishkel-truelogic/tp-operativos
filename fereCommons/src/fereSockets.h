/*
 * sockets.h
 *
 *  Created on: 14/09/2014
 *      Author: cesarcappetto
 */

#ifndef FERESOCKETS_H_
#define FERESOCKETS_H_

	#include "fereTypes.h"

	#define MAX_CONEXIONES 10
	#define BUFF_SIZE 4096

	//TIPOS DE DATOS
	//=================================================================
	typedef struct {

		Int32U 				descriptor;
		struct sockaddr_in *ptrAddress;

	} Socket;

	typedef struct {

		Socket *ptrSocket;
		Socket *ptrSocketServer;

	} SocketClient;

	typedef struct {

		Char 	data[BUFF_SIZE];
		Int32U 	size;

	} SocketBuffer ;

	typedef struct {

		Int8T 	type;
		Int16T 	length;

	} __attribute__((packed)) SocketHeader;

	typedef struct {

		SocketHeader	header;
		void			*ptrData;

	} __attribute__((packed)) SocketPackage;
	//=================================================================




	//FUNCIONES DE MANEJO DE SOCKETS
	//=================================================================
	Socket* socketCreateServer(Int32U port);
	SocketClient* socketCreateClient();

	Boolean socketListen(Socket *ptrSocket);
	Boolean socketConnect(SocketClient *ptrSocketClient, String ptrServerAddress, Int32U serverPort);

	Socket* socketAcceptClient(Socket *ptrListenSocket);

	SocketBuffer *socketReceive(Socket *ptrSender);
	Boolean socketSend(Socket *ptrDestination, SocketBuffer *ptrBuffer);
	Boolean socketDestroy(Socket *ptrSocket);

	//=================================================================

#endif /* FERESOCKETS_H_ */
