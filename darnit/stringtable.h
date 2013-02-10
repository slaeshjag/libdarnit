/*
Copyright (c) 2011-2013 Steven Arnow
'stringtable.h' - This file is part of libdarnit

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


#ifndef __STRINGTABLE_H__
#define	__STRINGTABLE_H__

#define		STRINGTABLE_MAGIC		0x3411BF00

typedef struct {
	unsigned int		magic;
	unsigned int		sections;
} STRINGTABLE_FILE_MAIN;


typedef struct {
	unsigned int		strings;
	unsigned int		zlen;
	unsigned int		len;
	unsigned int		stringz;
	const char		name[32];
} STRINGTABLE_FILE_SECTION;


typedef struct {
	unsigned int		sum;
	unsigned int		pos;
	unsigned int		val;
} STRINGTABLE_FILE_ENTRY;


typedef struct {
	unsigned int		key;
	const char		*name;
	const char		*value;
} STRINGTABLE_ENTRY;


typedef struct {
	STRINGTABLE_FILE_SECTION	sec;
	unsigned int			file_pos;
	unsigned int			name_comp;
	STRINGTABLE_ENTRY		*string;
	unsigned int			strings;
	void				*string_data;
} STRINGTABLE_SECTION;


typedef struct {
	const char			**name;
	unsigned int			names;
} STRINGTABLE_SEC_LIST;


typedef struct {
	STRINGTABLE_SECTION	*section;
	unsigned int		sections;
	FILESYSTEM_FILE		*fp;
	STRINGTABLE_SEC_LIST	sec_list;
} STRINGTABLE;


void *stringtableOpen(const char *fname);
int stringtableLoadSection(STRINGTABLE *st, const char *section);
const STRINGTABLE_SEC_LIST *stringtableSectionList(STRINGTABLE *st);
const char *stringtableGetEntry(STRINGTABLE *st, const char *key);
int stringtableUnloadSection(STRINGTABLE *st, const char *section);
void *stringtableClose(STRINGTABLE *st);


#endif
