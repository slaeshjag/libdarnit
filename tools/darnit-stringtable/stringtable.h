#ifndef __STRINGTABLE_H__
#define	__STRINGTABLE_H__

#include <zlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	unsigned int			magic;
	unsigned int			sections;
} FILE_MAIN;


typedef struct {
	unsigned int			strings;
	unsigned int			zlen;
	unsigned int			len;
	unsigned int			stringz;
	char				name[32];
} FILE_SECTION_HEADER;


typedef struct {
	unsigned int			sum;
	unsigned int			pos;
} FILE_STRING_ENTRY;


#endif
