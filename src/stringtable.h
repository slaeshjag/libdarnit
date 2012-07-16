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
	STRINGTABLE_SECTION	*section;
	unsigned int		sections;
	FILE			*fp;
} STRINGTABLE;


void *stringtableOpen(const char *fname);
int stringtableLoadSection(STRINGTABLE *st, const char *section);
const char *stringtableGetEntry(STRINGTABLE *st, const char *key);
int stringtableUnloadSection(STRINGTABLE *st, const char *section);
void *stringtableClose(STRINGTABLE *st);


#endif
