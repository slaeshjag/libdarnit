#include "darnit.h"


int stringtableFindSection(STRINGTABLE *st, const char *section) {
	int i;
	unsigned int comp;

	comp = utilStringSum(section);
	for (i = 0; i < st->sections; i++)
		if (st->section[i].name_comp == comp)
			if (strcmp(section, st->section[i].sec.name) == 0)
				return i;
	return -1;
}


int stringtableUnloadSection(STRINGTABLE *st, const char *section) {
	int sec;
	if (st == NULL)
		return -1;

	if ((sec = stringtableFindSection(st, section)) == -1)
		return -1;
	free(st->section[sec].string);
	free(st->section[sec].string_data);

	st->section[sec].string = NULL;
	st->section[sec].string_data = NULL;
	st->section[sec].strings = 0;

	return 0;
}


const char *stringtableGetEntry(STRINGTABLE *st, const char *key) {
	int i, j;
	unsigned int comp = utilStringSum(key);
	if (st == NULL)
		return key;

	for (i = 0; i < st->sections; i++)
		for (j = 0; j < st->section[i].strings; j++) 
			if (st->section[i].string[j].key == comp)
				if (strcmp(st->section[i].string[j].name, key) == 0)
					return st->section[i].string[j].value;
			
	return key;
}


int stringtableLoadSection(STRINGTABLE *st, const char *section) {
	STRINGTABLE_FILE_ENTRY *fe;
	int i, sec;
	void *data, *zdata, *fez;
	char *str;

	if (st == NULL)
		return -1;

	if ((sec = stringtableFindSection(st, section)) == -1)
		return -1;
	if (st->section[sec].strings != 0)
		return 0;

	fsFileSeek(st->fp, st->section[sec].file_pos, SEEK_SET);
	
	zdata = malloc(st->section[sec].sec.zlen);
	data = malloc(st->section[sec].sec.len);
	fez = malloc(st->section[sec].sec.stringz);
	fe = malloc(st->section[sec].sec.strings * sizeof(STRINGTABLE_FILE_ENTRY));
	st->section[sec].string = malloc(st->section[sec].sec.strings * sizeof(STRINGTABLE_ENTRY));

	if (zdata == NULL || data == NULL || fez == NULL || fe == NULL || st->section[sec].string == NULL) {
		free(st->section[sec].string);
		free(zdata);
		free(data);
		free(fez);
		free(fe);
		return -1;
	}

	fsFileRead(zdata, st->section[sec].sec.zlen, st->fp);
	fsFileRead(fez, st->section[sec].sec.stringz, st->fp);
	
	stbi_zlib_decode_buffer(data, st->section[sec].sec.len, zdata, st->section[sec].sec.zlen);
	stbi_zlib_decode_buffer((void *) fe, st->section[sec].sec.strings * sizeof(STRINGTABLE_FILE_ENTRY), fez, st->section[sec].sec.stringz);

	free(zdata);
	free(fez);
	str = data;
	
	for (i = 0; i < st->section[sec].sec.strings; i++) {
		st->section[sec].string[i].name = &str[ntohl(fe[i].pos)];
		st->section[sec].string[i].value = &str[ntohl(fe[i].val)];
		st->section[sec].string[i].key = utilStringSum(st->section[sec].string[i].name);
	}

	st->section[sec].string_data = data;
	st->section[sec].strings = st->section[sec].sec.strings;
	free(fe);

	return 0;
}


const STRINGTABLE_SEC_LIST *stringtableSectionList(STRINGTABLE *st) {
	if (!st)
		return NULL;
	return &st->sec_list;
}


void *stringtableOpen(const char *fname) {
	FILESYSTEM_FILE *fp;
	int i;
	STRINGTABLE_FILE_MAIN stfm;
	STRINGTABLE *st;

	if ((st = malloc(sizeof(STRINGTABLE))) == NULL)
		return NULL;
	
	if ((fp = fsFileOpen(fname, "rb")) == NULL) {
		free(st);
		return NULL;
	}

	fsFileRead(&stfm, sizeof(STRINGTABLE_FILE_MAIN), fp);

	if (ntohl(stfm.magic) != STRINGTABLE_MAGIC) {
		fsFileClose(fp);
		free(st);
		return NULL;
	}

	st->sections = ntohl(stfm.sections);
	st->fp = fp;

	st->sec_list.name = malloc(sizeof(char *) * st->sections);
	st->section = malloc(sizeof(STRINGTABLE_SECTION) * st->sections);
	if (!st->section || !st->sec_list.name) {
		free(st->section);
		free(st->sec_list.name);
		fsFileClose(fp);
		free(st);
		return NULL;
	}

	st->sec_list.names = st->sections;

	for (i = 0; i < st->sections; i++) {
		fsFileRead(&st->section[i].sec, sizeof(STRINGTABLE_FILE_SECTION), fp);
		st->section[i].sec.zlen = ntohl(st->section[i].sec.zlen);
		st->section[i].sec.strings = ntohl(st->section[i].sec.strings);
		st->section[i].sec.len = ntohl(st->section[i].sec.len);
		st->section[i].sec.stringz = ntohl(st->section[i].sec.stringz);

		st->section[i].file_pos = fsFileTell(fp);
		st->section[i].string = NULL;
		st->section[i].strings = 0;
		st->section[i].string_data = NULL;
		st->section[i].name_comp = utilStringSum(st->section[i].sec.name);
		st->sec_list.name[i] = st->section[i].sec.name;
		fsFileSeek(fp, st->section[i].sec.zlen + st->section[i].sec.stringz, SEEK_CUR);
	}

	return st;
}


void *stringtableClose(STRINGTABLE *st) {
	int i;

	if (st == NULL)
		return NULL;

	for (i = 0; i < st->sections; i++)
		stringtableUnloadSection(st, st->section[i].sec.name);
	free(st->section);
	free(st->sec_list.name);
	fsFileClose(st->fp);
	free(st);

	return NULL;
}
