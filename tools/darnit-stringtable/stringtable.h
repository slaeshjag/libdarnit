#ifndef __STRINGTABLE_H__
#define	__STRINGTABLE_H__

#include <zlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>

#define					MAGIC		0x3411BF00


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
	unsigned int			val;
} FILE_STRING_ENTRY;


#endif
