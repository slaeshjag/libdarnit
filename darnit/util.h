/*
Copyright (c) 2011-2013 Steven Arnow
'util.h' - This file is part of libdarnit

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


#ifndef __UTIL_H__
#define	__UTIL_H__

#include <time.h>

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <arpa/inet.h>
#endif


unsigned int utilHtonl(unsigned int val);
unsigned int utilNtohl(unsigned int val);
char *utilPathTranslate(const char *path);
int utilStringToIntArray(const char *str, const char *delimiter, int *dest, int max_tokens);
void utilBlockToHostEndian(unsigned int *block, int elements);
unsigned int utilStringSum(const char *str);


typedef struct {
	int			sine[3600];
} UTIL;

void utilInit();
int utilSine(int angle);
#define utilCos(x) utilSine(900+(x))
#define UTIL_PI 31


#endif
