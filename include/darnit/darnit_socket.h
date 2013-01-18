#ifndef __DARNIT_SOCKET_H__
#define	__DARNIT_SOCKET_H__

typedef void DARNIT_SOCKET;

DARNIT_SOCKET *d_socket_connect(const char *host, int port, void (*callback)(int ret, void *data, void *socket), void *data);
int d_socket_recv(DARNIT_SOCKET *sock, void *buff, int len);
int d_socket_recv_try(DARNIT_SOCKET *sock, void *buff, int len);
int d_socket_send(DARNIT_SOCKET *sock, void *buff, int len);
DARNIT_SOCKET *d_socket_close(DARNIT_SOCKET *sock);

#endif
