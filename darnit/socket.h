/*
Copyright (c) 2011-2013 Steven Arnow
'socket.h' - This file is part of libdarnit

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
#elif defined __APPLE__
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
