/*
 * main.c
 *
 *  Created on: 16/09/2014
 *      Author: cesarcappetto
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <src/fereSockets.h>


#define TITLE "** Kernel V2.0 **"
#define PORT 6667

void printHeader(){

	printf("\n");
	printf(TITLE);
	printf("\n");

}

void clientHandler(Int32U clientDescriptor,fd_set *master){

	Socket *ptrTemp = malloc(sizeof(Socket));

	ptrTemp->descriptor = clientDescriptor;

	SocketBuffer *ptrBuffer = socketReceive(ptrTemp);

	if(ptrBuffer == NULL){
		printf("Ha ocurrido un error al intentar recibir de %d\n",clientDescriptor);

		//ELIMINO EL DESCRIPTOR DEL CONJUNTO
		FD_CLR(clientDescriptor, master);

		//CIERRO EL SOCKET
		//close(clientDescriptor);

		free(ptrTemp);
	} else {

		printf("%d: %s",clientDescriptor, ptrBuffer->data);

	}


}

int main(){

	printHeader();

	Socket *ptrServerSocket = socketCreateServer(PORT);
	socketListen(ptrServerSocket);

	//CONJUNTO MAESTRO DE DESCRIPTORES
	fd_set master;

	//CONJUNTO TEMPORAL DE DESCRIPTORES
	fd_set read_fds;

	Int32U fdmax,i;

	//SETEO A "0" EL MASTER Y EL TEMPORAL
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	//CARGO EL SOCKET DE ESCUCHA A MASTER
	FD_SET(ptrServerSocket->descriptor, &master);

	//CARGO EL SOCKET MAS GRANDE
	fdmax = ptrServerSocket->descriptor;


	struct timeval timeout;
	Int32U selectTimeout = 0;

	//BUCLE PRINCIPAL
	while(1){

		read_fds 		= master;
		timeout.tv_sec 	= 10;
		timeout.tv_usec	= 0;

		//HAGO EL SELECT
		Int32U selectResult = select(fdmax+1, &read_fds, NULL, NULL, &timeout);

		if (selectResult == -1) {

			//SE CAYO MI SOCKET DE ESCUCHA
			printf("Error en el socket de escucha!!\n");
			exit(1);

		} else if (selectResult == 0) {

			//SALE POR TIMEOUT
			selectTimeout++;

		} else {

			//PASO ALGO POSTA
			selectTimeout = 0;

			//RECORRO TODOS LOS DESCRIPTORES MONITOREADOS PARA VER QUIEN LLAMO
			for(i = 0; i <= fdmax; i++)	{

				if (FD_ISSET(i, &read_fds)) {

					//FUE EL SOCKET DE ESCUCHA??
					if (i == ptrServerSocket->descriptor){

						//SI, CREO UN NUEVO SOCKET
						Socket *ptrClientSocket = socketAcceptClient(ptrServerSocket);

						//LO CARGO A LA LISTA DE DESCRIPTORES A MONITOREAR
						FD_SET(ptrClientSocket->descriptor, &master);

						if (ptrClientSocket->descriptor > fdmax) {
							fdmax = ptrClientSocket ->descriptor;
						}



					} else {

						//NO, ENTONCES GESTIONO EL SOCKET QUE HABLO...
						clientHandler(i,&master);

					}
				}
			}
		}

	}

	//close(ptrServerSocket->descriptor);
	free(ptrServerSocket);


	return 0;
}






