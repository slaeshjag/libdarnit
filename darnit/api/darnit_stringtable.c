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
