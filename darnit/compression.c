/*
Copyright (c) 2013 Steven Arnow
'compression.c' - This file is part of libdarnit

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


int compressionCompress(void *buffer, unsigned int data_len, void **dest) {
	void *dbuf; 
	unsigned int dest_len;

	dest_len = data_len + (data_len / 100) + 604;
	dbuf = malloc(dest_len);

	if (BZ2_bzBuffToBuffCompress(dbuf + 4, &dest_len, buffer, data_len, 9, 0, 0) != BZ_OK)
		return -1;
	dbuf = realloc(dbuf, dest_len + 4);
	*((unsigned int *) dbuf) = data_len;
	utilBlockToHostEndian(dbuf, 1);
	*dest = dbuf;

	return dest_len;
}


int compressionDecompress(void *buffer, unsigned int data_len, void **outbuf) {
	unsigned int outsize;
	void *dbuf;

	utilBlockToHostEndian(buffer, 1);
	outsize = *((unsigned int *) buffer);
	utilBlockToHostEndian(buffer, 1);

	dbuf = malloc(outsize);
	BZ2_bzBuffToBuffDecompress(dbuf, &outsize, ((char *) buffer) + 4, data_len - 4, 0, 0);
	*outbuf = dbuf;
	return outsize;
}

