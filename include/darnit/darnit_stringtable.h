/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_stringtable.h' - This file is part of libdarnit

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


#ifndef __DARNIT_STRINGTABLE_H__
#define	__DARNIT_STRINGTABLE_H__

typedef void DARNIT_STRINGTABLE;

typedef struct DARNIT_STRINGTABLE_SECTIONS {
	const char		**name;
	unsigned int		names;
} DARNIT_STRINGTABLE_SECTIONS;


DARNIT_STRINGTABLE *d_stringtable_open(const char *fname);
int d_stringtable_section_load(DARNIT_STRINGTABLE *stringtable, const char *section);
const DARNIT_STRINGTABLE_SECTIONS *d_stringtable_section_list(DARNIT_STRINGTABLE *stringtable);
const char *d_stringtable_entry(DARNIT_STRINGTABLE *stringtable, const char *entry);
int d_stringtable_section_unload(DARNIT_STRINGTABLE *stringtable, const char *section);
DARNIT_STRINGTABLE *d_stringtable_close(DARNIT_STRINGTABLE *stringtable);


#endif
