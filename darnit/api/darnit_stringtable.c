#include "darnit.h"


void EXPORT_THIS *darnitStringtableOpen(const char *fname) {
	return stringtableOpen(fname);
}


int EXPORT_THIS darnitStringtableSectionLoad(void *stringtable, const char *section) {
	return stringtableLoadSection(stringtable, section);
}


const STRINGTABLE_SEC_LIST EXPORT_THIS *darnitStringtableSectionList(void *stringtable) {
	return stringtableSectionList(stringtable);
}


const char EXPORT_THIS *darnitStringtableEntryGet(void *stringtable, const char *entry) {
	return stringtableGetEntry(stringtable, entry);
}


int EXPORT_THIS darnitStringtableSectionUnload(void *stringtable, const char *section) {
	return stringtableUnloadSection(stringtable, section);
}


void EXPORT_THIS *darnitStringtableClose(void *stringtable) {
	return stringtableClose(stringtable);
}
