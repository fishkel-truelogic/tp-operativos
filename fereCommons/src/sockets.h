/*
 * sockets.h
 *
 *  Created on: 14/09/2014
 *      Author: fere-rules
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

	#define MAX_CONEXIONES 10
	#define BUFF_SIZE 1024

	typedef struct {
		int descriptor;
		struct sockaddr_in *ptrAddress;
	} Socket;

	typedef struct {
		Socket *socket;
		Socket *socketServer;
	} t_socket_client;

	typedef struct {
		char data[BUFF_SIZE];
		int size;
	} t_socket_buffer ;

	typedef struct {
		int8_t type;
		int16_t length;
	} __attribute__((packed)) t_socket_header;

	typedef struct {
		t_socket_header header;
		//char data[BUFF_SIZE];
		void* data;
	} __attribute__((packed)) t_socket_paquete;

	Socket* socket_createServer(int port);
	t_socket_client* socket_createClient();
	int socket_listen(Socket* socketEscucha);
	int socket_connect(t_socket_client *cliente, char *ipServer, int portServer);
	Socket* socket_acceptClient(Socket* socketEscucha);
	t_socket_buffer *socket_recv(Socket *emisor);
	int socket_recvPaquete(Socket* emisor, t_socket_paquete* paquete);
	int socket_send(Socket *destinatario, t_socket_buffer *buffer);
	int socket_sendPaquete(Socket *destinatario, int tipo, int tamanio, void *contenido);
	char* socket_getIP(Socket* socket);
	void socket_freePaquete(t_socket_paquete* paquete);

#endif /* SOCKETS_H_ */
