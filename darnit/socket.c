/*
Copyright (c) 2011-2013 Steven Arnow
'socket.c' - This file is part of libdarnit

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/


#include "darnit.h"


void *socketConnect(const char *host, int port, void (*callback)(int, void *, void *), void *data) {
	struct sockaddr_in sin;
	struct hostent *hp;
	SOCKET_STRUCT *sock;

	#ifndef _WIN32
	int x;
	#else
	u_long iMode=1;
	#endif

	sock = malloc(sizeof(SOCKET_STRUCT));


	sock->socket = socket(AF_INET, SOCK_STREAM, 0);
	if ((hp = gethostbyname(host)) == NULL) {
		if (callback)
			(callback)(-1, NULL, data);
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
	} else {
		#ifdef TARGET_IS_RETARDED
			sock->retarded_wait=d_time_get();
		#endif
		socketListAdd(sock, callback, data);
	}


	#ifdef __APPLE__
	int set = 1;
	setsockopt(sd, SOL_SOCKET, SO_NOSIGPIPE, (void *) &set, sizeof(int));
	#endif

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

	#ifndef _WIN32
	if (errno == EWOULDBLOCK || errno == EAGAIN)
		return 0;
	#else
	int error_n = WSAGetLastError();
	if (error_n == WSAEWOULDBLOCK)
		return 0;
	#endif
	return ret;
}


int socketRecvTry(SOCKET_STRUCT *sock, char *buff, int len) {
	if (sock == NULL) return -1;
	int ret;

	if ((ret = recv(sock->socket, buff, len, MSG_PEEK | MSG_NOSIGNAL)) == len)
		recv(sock->socket, buff, len, MSG_NOSIGNAL);

	if (ret == len)
		return len;
	if (ret > -1)
		return 0;
	#ifndef _WIN32
	if (errno == EAGAIN || errno == EWOULDBLOCK)
		return 0;
	#else
	int error_n = WSAGetLastError();
	if (error_n == WSAEWOULDBLOCK)
		return 0;
	#endif

	return -1;
}


int socketSend(SOCKET_STRUCT *sock, void *buff, int len) {
	if (sock == NULL) return -1;
	return send(sock->socket, buff, len, MSG_NOSIGNAL);
}


void *socketClose(SOCKET_STRUCT *sock) {
	if (sock == NULL) return NULL;

	socketLoopDisconnect(sock);
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
	d->connect_list = entry;
	
	entry->socket = sock;
	entry->callback = callback;
	entry->data = data;

	return;
}


void socketConnectLoop() {
	SOCKET_LIST *list, **parent, *tmp_p;
	void *tmp_sock;
	int t, tmp;
	
	#ifdef _WIN32
	fd_set fd_win_use, fd_win_error;
	struct timeval time_delay;
	FILE *connect_error;
	connect_error = fopen("socket_error.txt", "a");
	#endif

	parent = &d->connect_list;
	list = *parent;
	while (list != NULL) {
		#ifdef TARGET_IS_RETARDED
		time_delay.tv_sec = 0;
		time_delay.tv_usec = 10;
		FD_ZERO(&fd_win_use);
		FD_ZERO(&fd_win_error);
		FD_SET(list->socket->socket, &fd_win_use);
		FD_SET(list->socket->socket, &fd_win_error);
		select(list->socket->socket + 1, NULL, /*&fd_win_use*/NULL, &fd_win_error, &time_delay);
		
		/*lol.*/
		if (/*FD_ISSET(list->socket->socket, &fd_win_use)*/d_time_get()-list->socket->retarded_wait>RETARDED_WAIT_TIMEOUT) {
			fprintf(connect_error, "Apparently, the connect has happenedi\n");
			t=0;
		/*} else if (FD_ISSET(list->socket->socket, &fd_win_error)) {
			fprintf(connect_error, "Connect did not succeed\n");
			t=1;
		*/} else
			goto loop;

		#else
		if ((t = recv(list->socket->socket, (void *) &tmp, 4, MSG_PEEK | MSG_NOSIGNAL) < 0)) {
			if (errno == EWOULDBLOCK || errno == EAGAIN)
				goto loop;
		}
		#endif

		if (t == 1)
			t *= -1;

		tmp_p = list;
		*parent = list->next;
		tmp_sock = list->socket;
		list->socket = NULL;
		(list->callback)(t, list->data, tmp_sock);
		list = *parent;
		free(tmp_p);
		continue;

		loop:
			parent = &list->next;
			list = *parent;
	}

	#ifdef _WIN32
	fclose(connect_error);
	#endif

	return;
}


void socketLoopDisconnect(SOCKET_STRUCT *sock) {
	SOCKET_LIST *list, **parent, *tmp;

	if (sock == NULL)
		return;
	parent = &d->connect_list;
	list = *parent;
	while (list != NULL) {
		if (list->socket == sock) {
			tmp = list;
			*parent = list->next;
			list = *parent;
			free(tmp);
			return;
		}

		parent = &list->next;
		list = *parent;
	}

	return;
}

int socketInit() {
	#ifdef _WIN32
		WSADATA wsaData;
		WORD version;
		version = MAKEWORD(2, 0);
		
		if (WSAStartup(version, &wsaData) != 0) {
			return -1;
		} else if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0) {
			WSACleanup();
			return -1;
		}
	#endif
	d->connect_list = NULL;

	return 0;
}
