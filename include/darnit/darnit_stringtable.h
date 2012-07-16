#ifndef __DARNIT_STRINGTABLE_H__
#define	__DARNIT_STRINGTABLE_H__

void *darnitStringtableOpen(const char *fname);
int darnitStringtableSectionLoad(void *stringtable, const char *section);
const char *darnitStringtableEntryGet(void *stringtable, const char *entry);
int darnitStringtableSectionUnload(void *stringtable, const char *section);
void *darnitStringtableClose(void *stringtable);


#endif
