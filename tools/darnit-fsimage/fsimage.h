#ifndef __STRINGTABLE_H__
#define	__STRINGTABLE_H__

#include <zlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>

#define					MAGIC		0x83B3661B
#define					VERSION		0xBBA77ABC


typedef struct {
	unsigned int			magic;
	unsigned int			version;
	unsigned int			files;
} FILE_MAIN;


typedef struct {
	char				name[128];
	unsigned int			pos;
	unsigned int			len;
	unsigned int			pad;
} FILE_ENTRY;


#endif
