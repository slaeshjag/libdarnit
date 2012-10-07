#ifndef __DARNIT_SOCKET_H__
#define	__DARNIT_SOCKET_H__

typedef void DARNIT_SOCKET;

DARNIT_SOCKET *darnitSocketConnect(const char *host, int port);
int darnitSocketRecv(DARNIT_SOCKET *sock, void *buff, int len);
int darnitSocketRecvTry(DARNIT_SOCKET *sock, void *buff, int len);
int darnitSocketSend(DARNIT_SOCKET *sock, void *buff, int len);
DARNIT_SOCKET *darnitSocketClose(DARNIT_SOCKET *sock);

#endif
