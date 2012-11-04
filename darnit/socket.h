#ifndef __SOCKET_H__
#define	__SOCKET_H__

typedef struct {
	#ifdef _WIN32
		SOCKET socket;
	#else
		int socket;
	#endif
} SOCKET_STRUCT;


typedef struct SOCKET_LIST {
	SOCKET_STRUCT		*socket;
	void			(*callback)(int, void *, void *);
	void			*data;
	struct SOCKET_LIST	*next;
} SOCKET_LIST;


#ifdef _WIN32
	#define		EWOULDBLOCK 	WSAEWOULDBLOCK
	#define		MSG_NOSIGNAL	0
#endif


int socketInit();
void socketConnectLoop();
void *socketConnect(const char *host, int port, void (*callback)(int, void *, void *), void *data);
void socketListAdd(SOCKET_STRUCT *sock, void (*callback)(int, void*, void *), void *data);
int socketRecv(SOCKET_STRUCT *sock, char *buff, int len);
int socketRecvTry(SOCKET_STRUCT *sock, char *buff, int len);
int socketSend(SOCKET_STRUCT *sock, void *buff, int len);
void *socketClose(SOCKET_STRUCT *sock);
void socketLoopDisconnect(SOCKET_STRUCT *sock);


#endif
