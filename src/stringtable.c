#include "darnit.h"


unsigned int stringtableCalcComp(const char *section) {
	int i;
	unsigned int comp = 0;
	
	for (i = 0; i < strlen(section); i++)
		comp += section[i];
	
	return comp;
}


int stringtableFindSection(STRINGTABLE *st, const char *section) {
	int i;
	unsigned int comp;

	comp = stringtableCalcComp(section);
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
	unsigned int comp = stringtableCalcComp(key);
	if (st == NULL)
		return "STRINGTABLE NOT LOADED";

	for (i = 0; i < st->sections; i++)
		for (j = 0; j < st->section[i].strings; j++) 
			if (st->section[i].string[j].key == comp)
				if (strcmp(st->section[i].string[j].name, key) == 0)
					return st->section[i].string[j].value;
			
	return "NOT IN STRINGTABLE";
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

	fseek(st->fp, st->section[sec].file_pos, SEEK_SET);
	
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

	fread(zdata, st->section[sec].sec.zlen, 1, st->fp);
	fread(fez, st->section[sec].sec.stringz, 1, st->fp);
	
	stbi_zlib_decode_buffer(data, st->section[sec].sec.len, zdata, st->section[sec].sec.zlen);
	stbi_zlib_decode_buffer((void *) fe, st->section[sec].sec.strings * sizeof(STRINGTABLE_FILE_ENTRY), fez, st->section[sec].sec.stringz);

	free(zdata);
	free(fez);
	str = data;
	
	for (i = 0; i < st->section[sec].sec.strings; i++) {
		st->section[sec].string[i].name = &str[ntohl(fe[i].pos)];
		st->section[sec].string[i].value = &str[ntohl(fe[i].val)];
		st->section[sec].string[i].key = stringtableCalcComp(st->section[sec].string[i].name);
	}

	st->section[sec].string_data = data;
	st->section[sec].strings = st->section[sec].sec.strings;
	free(fe);

	return 0;
}



void *stringtableOpen(const char *fname) {
	FILE *fp;
	int i;
	STRINGTABLE_FILE_MAIN stfm;
	STRINGTABLE *st;
	char *fname_n;

	if ((st = malloc(sizeof(STRINGTABLE))) == NULL)
		return NULL;
	
	fname_n = utilPathTranslate(fname);
	if ((fp = fopen(fname_n, "rb")) == NULL) {
		free(st);
		free(fname_n);
		return NULL;
	}
	free(fname_n);

	fread(&stfm, sizeof(STRINGTABLE_FILE_MAIN), 1, fp);

	if (ntohl(stfm.magic) != STRINGTABLE_MAGIC) {
		fclose(fp);
		free(st);
		return NULL;
	}

	st->sections = ntohl(stfm.sections);
	st->fp = fp;

	if ((st->section = malloc(sizeof(STRINGTABLE_SECTION) * st->sections)) == NULL) {
		fclose(fp);
		free(st);
		return NULL;
	}

	for (i = 0; i < st->sections; i++) {
		fread(&st->section[i].sec, sizeof(STRINGTABLE_FILE_SECTION), 1, fp);
		st->section[i].sec.zlen = ntohl(st->section[i].sec.zlen);
		st->section[i].sec.strings = ntohl(st->section[i].sec.strings);
		st->section[i].sec.len = ntohl(st->section[i].sec.len);
		st->section[i].sec.stringz = ntohl(st->section[i].sec.stringz);

		st->section[i].file_pos = ftell(fp);
		st->section[i].string = NULL;
		st->section[i].strings = 0;
		st->section[i].string_data = NULL;
		st->section[i].name_comp = stringtableCalcComp(st->section[i].sec.name);
		fseek(fp, st->section[i].sec.zlen + st->section[i].sec.stringz, SEEK_CUR);
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
	fclose(st->fp);
	free(st);

	return NULL;
}
