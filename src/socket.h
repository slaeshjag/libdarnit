#ifndef __SOCKET_H__
#define	__SOCKET_H__

typedef struct {
	#ifdef _WIN32
		SOCKET socket;
	#else
		int socket;
	#endif
} SOCKET_STRUCT;

#ifdef _WIN32
	#define		EWOULDBLOCK 	WSAEWOULDBLOCK
#endif


void *socketConnect(const char *host, int port);
int socketRecv(SOCKET_STRUCT *sock, char *buff, int len);
int socketRecvTry(SOCKET_STRUCT *sock, char *buff, int len);
int socketSend(SOCKET_STRUCT *sock, void *buff, int len);
void *socketClose(SOCKET_STRUCT *sock);


#endif
