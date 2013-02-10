/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_stringtable.c' - This file is part of libdarnit

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


void EXPORT_THIS *d_stringtable_open(const char *fname) {
	return stringtableOpen(fname);
}


int EXPORT_THIS d_stringtable_section_load(void *stringtable, const char *section) {
	return stringtableLoadSection(stringtable, section);
}


const STRINGTABLE_SEC_LIST EXPORT_THIS *d_stringtable_section_list(void *stringtable) {
	return stringtableSectionList(stringtable);
}


const char EXPORT_THIS *d_stringtable_entry(void *stringtable, const char *entry) {
	return stringtableGetEntry(stringtable, entry);
}


int EXPORT_THIS d_stringtable_section_unload(void *stringtable, const char *section) {
	return stringtableUnloadSection(stringtable, section);
}


void EXPORT_THIS *d_stringtable_close(void *stringtable) {
	return stringtableClose(stringtable);
}
