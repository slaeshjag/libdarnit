/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_util.h' - This file is part of libdarnit

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


#ifndef __DARNIT_UTIL_H__
#define	__DARNIT_UTIL_H__


unsigned int d_util_htonl(unsigned int arg);
unsigned int d_util_ntohl(unsigned int arg);
char *d_util_path_translate(const char *path);
int d_util_string_to_int_array(const char *str, const char *delimiter, int *dest, int max_tokens);
void d_util_endian_convert(unsigned int *block, int elements);
int d_util_sin(int angle);
const char *d_str_null(const char *str);


#endif
