/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_socket.h' - This file is part of libdarnit

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


#ifndef __DARNIT_SOCKET_H__
#define	__DARNIT_SOCKET_H__

typedef void DARNIT_SOCKET;

DARNIT_SOCKET *d_socket_connect(const char *host, int port, void (*callback)(int ret, void *data, void *socket), void *data);
int d_socket_recv(DARNIT_SOCKET *sock, void *buff, int len);
int d_socket_recv_try(DARNIT_SOCKET *sock, void *buff, int len);
int d_socket_send(DARNIT_SOCKET *sock, void *buff, int len);
DARNIT_SOCKET *d_socket_close(DARNIT_SOCKET *sock);

#endif
