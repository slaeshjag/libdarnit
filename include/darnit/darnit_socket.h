#ifndef __DARNIT_SOCKET_H__
#define	__DARNIT_SOCKET_H__

void *darnitSocketConnect(const char *host, int port);
int darnitSocketRecv(void *sock, void *buff, int len);
int darnitSocketRecvTry(void *sock, void *buff, int len);
int darnitSocketSend(void *sock, void *buff, int len);
void *darnitSocketClose(void *sock);

#endif
