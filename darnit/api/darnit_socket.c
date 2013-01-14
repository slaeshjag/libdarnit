#include "darnit.h"


void EXPORT_THIS *d_socket_connect(const char *host, int port, void (*callback)(int ret, void *data, void *socket), void *data) {
	return socketConnect(host, port, callback, data);
}


int EXPORT_THIS d_socket_recv(void *sock, void *buff, int len) {
	return socketRecv(sock, buff, len);
}


int EXPORT_THIS d_socket_recv_try(void *sock, void *buff, int len) {
	return socketRecvTry(sock, buff, len);
}


int EXPORT_THIS d_socket_send(void *sock, void *buff, int len) {
	return socketSend(sock, buff, len);
}


void EXPORT_THIS *d_socket_close(void *sock) {
	return socketClose(sock);
}
