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
