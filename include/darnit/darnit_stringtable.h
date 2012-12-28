#ifndef __DARNIT_STRINGTABLE_H__
#define	__DARNIT_STRINGTABLE_H__

typedef void DARNIT_STRINGTABLE;

typedef struct DARNIT_STRINGTABLE_SECTIONS {
	const char		**name;
	unsigned int		names;
} DARNIT_STRINGTABLE_SECTIONS;


DARNIT_STRINGTABLE *darnitStringtableOpen(const char *fname);
int darnitStringtableSectionLoad(DARNIT_STRINGTABLE *stringtable, const char *section);
const DARNIT_STRINGTABLE_SECTIONS *darnitStringtableSectionList(DARNIT_STRINGTABLE *stringtable);
const char *darnitStringtableEntryGet(DARNIT_STRINGTABLE *stringtable, const char *entry);
int darnitStringtableSectionUnload(DARNIT_STRINGTABLE *stringtable, const char *section);
DARNIT_STRINGTABLE *darnitStringtableClose(DARNIT_STRINGTABLE *stringtable);


#endif
