#ifndef __DARNIT_STRINGTABLE_H__
#define	__DARNIT_STRINGTABLE_H__

typedef void DARNIT_STRINGTABLE;

DARNIT_STRINGTABLE *darnitStringtableOpen(const char *fname);
int darnitStringtableSectionLoad(DARNIT_STRINGTABLE *stringtable, const char *section);
const char *darnitStringtableEntryGet(DARNIT_STRINGTABLE *stringtable, const char *entry);
int darnitStringtableSectionUnload(DARNIT_STRINGTABLE *stringtable, const char *section);
DARNIT_STRINGTABLE *darnitStringtableClose(DARNIT_STRINGTABLE *stringtable);


#endif
