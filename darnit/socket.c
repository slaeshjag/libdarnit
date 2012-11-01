#include "darnit.h"


void *socketConnect(const char *host, int port, void (*callback)(int, void *, void *), void *data) {
	struct sockaddr_in sin;
	SOCKET_STRUCT *sock;

	#ifdef _WIN32
		WSADATA wsaData;
		WORD version;
		struct hostent *hp;
		u_long iMode=1;
	#else
		int x;
		struct hostent *hp;
	#endif

	sock = malloc(sizeof(SOCKET_STRUCT));

	#ifdef _WIN32
		version = MAKEWORD(2, 0);
		if (WSAStartup(version, &wsaData) != 0) {
			free(sock);
			return NULL;
		} else if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0) {
			WSACleanup();
			free(sock);
			return NULL;
		}
	#endif

	sock->socket = socket(AF_INET, SOCK_STREAM, 0);
	if ((hp = gethostbyname(host)) == NULL) {
		#ifdef _WIN32
			closesocket(sock->socket);
		#else
			close(sock->socket);
		#endif
		free(sock);
		return NULL;
	}

	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	
	#ifdef _WIN32
		sin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
	#else
		sin.sin_addr.s_addr = *(unsigned long *) hp->h_addr_list[0];
	#endif

	if (callback) {
		#ifdef _WIN32
			ioctlsocket(sock->socket, FIONBIO, &iMode);
		#else
			x = fcntl(sock->socket, F_GETFL, 0);
			fcntl(sock->socket, F_SETFL, x | O_NONBLOCK);
		#endif
	}

	if (connect(sock->socket, (void *) &sin, sizeof(struct sockaddr_in)) == -1) {
		if (!callback) {
			fprintf(stderr, "libDarnit: Unable to connect to host %s\n", host);
			free(sock);
			return NULL;
		}
	}
	
	if (!callback) {
		#ifdef _WIN32
			ioctlsocket(sock->socket, FIONBIO, &iMode);
		#else
			x = fcntl(sock->socket, F_GETFL, 0);
			fcntl(sock->socket, F_SETFL, x | O_NONBLOCK);
		#endif
	} else
		socketListAdd(sock, callback, data);

	return sock;
}


int socketRecv(SOCKET_STRUCT *sock, char *buff, int len) {
	int ret;
	if (sock == NULL) return -1;

	ret = recv(sock->socket, buff, len, MSG_NOSIGNAL);
	if (ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
		fprintf(stderr, "Socket error; connection died\n");
		socketClose(sock);
		return -1;
	}

	if (errno == EWOULDBLOCK || errno == EAGAIN)
		return 0;
	return ret;
}


int socketRecvTry(SOCKET_STRUCT *sock, char *buff, int len) {
	if (sock == NULL) return -1;
	int ret;
	void *buff_tmp;

	buff_tmp = malloc(len);
	if ((ret = recv(sock->socket, buff_tmp, len, MSG_PEEK | MSG_NOSIGNAL)) == len)
		recv(sock->socket, buff, len, MSG_NOSIGNAL);
	free(buff_tmp);

	if (ret == len)
		return len;
	if (ret > -1)
		return 0;
	if (errno == EAGAIN || errno == EWOULDBLOCK)
		return 0;

	return -1;
}


int socketSend(SOCKET_STRUCT *sock, void *buff, int len) {
	if (sock == NULL) return -1;
	return send(sock->socket, buff, len, MSG_NOSIGNAL);
}


void *socketClose(SOCKET_STRUCT *sock) {
	if (sock == NULL) return NULL;
	#ifdef _WIN32
		closesocket(sock->socket);
	#else
		close(sock->socket);
	#endif

	free(sock);

	return NULL;
}


void socketListAdd(SOCKET_STRUCT *sock, void (*callback)(int, void *, void *), void *data) {
	SOCKET_LIST *entry;

	if ((entry = malloc(sizeof(SOCKET_LIST))) == NULL)
		return;
	entry->next = d->connect_list;
	d->connect_list = entry->next;
	
	entry->socket = sock;
	entry->callback = callback;
	entry->data = data;

	return;
}


void socketConnectLoop() {
	SOCKET_LIST *list, **parent, *tmp_p;
	int tmp, t;

	parent = &d->connect_list;
	list = *parent;
	while (list != NULL) {
		if ((t = recv(list->socket->socket, &tmp, 4, MSG_PEEK | MSG_NOSIGNAL) < 0)) {
			if (errno == EWOULDBLOCK || errno == EAGAIN)
				goto loop;
		} else if (t == 0)
			goto loop;
		(list->callback)(0, list->data, list->socket);
		tmp_p = list;
		*parent = list->next;
		free(tmp_p);
		continue;

		loop:
			parent = &list->next;
	}

	return;
}

int socketInit() {
	d->connect_list = NULL;

	return 0;
}
