#include "darnit.h"


void EXPORT_THIS *darnitSocketConnect(const char *host, int port, void (*callback)(int ret, void *data, void *socket), void *data) {
	return socketConnect(host, port, callback, data);
}


int EXPORT_THIS darnitSocketRecv(void *sock, void *buff, int len) {
	return socketRecv(sock, buff, len);
}


int EXPORT_THIS darnitSocketRecvTry(void *sock, void *buff, int len) {
	return socketRecvTry(sock, buff, len);
}


int EXPORT_THIS darnitSocketSend(void *sock, void *buff, int len) {
	return socketSend(sock, buff, len);
}


void EXPORT_THIS *darnitSocketClose(void *sock) {
	return socketClose(sock);
}
