#include "stringtable.h"

void *zCompress(void *indata, unsigned int len, unsigned int *outlen) {
	void *zdata;
	int zlen;
	z_stream zstruct;


	zstruct.zalloc = Z_NULL;
	zstruct.zfree = Z_NULL;
	zstruct.opaque = Z_NULL;

	if (deflateInit(&zstruct, 9) != Z_OK) {
		fprintf(stderr, "deflateInit() failed\n");
		return NULL;
	}

	zlen = deflateBound(&zstruct, len);

	if ((zdata = malloc(zlen)) == NULL) {
		fprintf(stderr, "Malloc(%i) failed\n", len);
		return NULL;
	}
	
	zstruct.avail_in = len;
	zstruct.next_in = indata;
	zstruct.avail_out = zlen;
	zstruct.next_out = zdata;

	if (deflate(&zstruct, Z_FINISH) != Z_STREAM_END) {
		fprintf(stderr, "deflate() failed\n");
		free(zdata);
		return NULL;
	}

	*outlen = zlen - zstruct.avail_out;
	zdata = realloc(zdata, *outlen);
	return zdata;
}


int writeSection(const char *fname, FILE *fp) {
	FILE *in;
	char *buff, *zdata;
	int size, i, j, comp, name, strings, line, skip;
	char c;
	FILE_STRING_ENTRY *string;
	FILE_SECTION_HEADER sh;

	if ((in = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "Unable to open section %s\n", fname);
		return -1;
	}

	fseek(in, 0, SEEK_END);
	size = ftell(in) + 1;
	rewind(in);

	if ((buff = malloc(size)) == NULL) {
		fclose(in);
		return -1;
	}

	for (i = strings = name = line = skip = 0; !feof(in); i++) {
		c = fgetc(in);
		if (c == '\t') {
			if (name == 0 && skip == 0) {
				name = 1;
				buff[i] = 1;
			} else
				i--;
			line = 1;
		} else if (c == '\n') {
			if (name == 0)
				i--;
			else {
				name = 0;
				buff[i] = 0;
				strings++;
			}
			line = 0;
			skip = 0;
		} else if (c == '<') {
			buff[i] = '\n';
			line = 1;
		} else if (c == '#' && line == 0) {
			skip = 1;
			i--;
		} else if (skip)
			i--;
		else {
			line = 1;
			buff[i] = c;
		}
	}
	if(c != '\n')
		strings++;
	
	fclose(in);
	zdata = malloc(i);
	size = i;
	buff[size-1] = 0;
	sh.len = size;
	sh.strings = strings;
	
	string = malloc(sizeof(FILE_STRING_ENTRY) * (strings));
	
	string[0].pos = 0;
	for (i = comp = 0, j = 0; i < size; i++) {
		if (buff[i] == 0) {
			comp = 0;
			j++;
			if (j < strings)
				string[j].pos = htonl(i + 1);
		} else if (buff[i] == 1) {
			string[j].sum = htonl(comp);
			string[j].val = htonl(i + 1);
			buff[i] = 0;
		} else
			comp += buff[i];
	}

	zdata = zCompress(buff, size, &sh.zlen);
	free(buff);
	buff = zCompress(string, sizeof(FILE_STRING_ENTRY) * strings, &sh.stringz);
	strncpy(sh.name, fname, 32);
	sh.name[31] = 0;

	sh.strings = htonl(sh.strings);
	sh.zlen = htonl(sh.zlen);
	sh.len = htonl(sh.len);
	sh.stringz = htonl(sh.stringz);

	fwrite(&sh, sizeof(FILE_SECTION_HEADER) /** strings*/, 1, fp);
	fwrite(zdata, ntohl(sh.zlen), 1, fp);
	fwrite(buff, ntohl(sh.stringz), 1, fp);

	free(zdata);
	free(buff);

	fprintf(stderr, "Wrote section %s\n", fname);

	return 0;
}


int scanDirectory(const char *directory, FILE *fp) {
	DIR *dir;
	struct dirent *file;
	int successful = 0;

	if ((dir = opendir(directory)) == NULL) {
		fprintf(stderr, "Unable to open directory %s\n", directory);
		return -1;
	}

	successful=chdir(directory);
	successful=0;

	do {
		file = readdir(dir);
		if (file == NULL)
			break;
		if (file->d_name[0] != '.')
			successful += (writeSection(file->d_name, fp) == 0) ? 1 : 0;
	} while(1);

	closedir(dir);

	return successful;
}


int main(int argc, char **argv) {
	FILE *fp;
	int sec;
	FILE_MAIN m;

	if (argc <3) {
		fprintf(stderr, "Usage: %s <source directory> <output filename>\n", argv[0]);
		return -1;
	}

	if ((fp = fopen(argv[2], "wb+")) == NULL) {
		fprintf(stderr, "Error: Unable to open target %s (wb+)\n", argv[2]);
		return -1;
	}

	m.magic = htonl(MAGIC);
	m.sections = htonl(0);

	fwrite(&m, sizeof(FILE_MAIN), 1, fp);
	if ((sec = scanDirectory(argv[1], fp)) < 1) {
		fprintf(stderr, "No usable data, found, aborting...\n");
		fclose(fp);
		unlink(argv[2]);
		return -1;
	}

	fseek(fp, 0, SEEK_SET);
	m.sections = htonl(sec);
	fwrite(&m, sizeof(FILE_MAIN), 1, fp);
	fclose(fp);

	return 0;
}
