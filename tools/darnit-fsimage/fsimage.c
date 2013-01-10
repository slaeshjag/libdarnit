#include "fsimage.h"


int writeHeader(const char *name, const char *path, FILE *fp, int pos) {
	FILE *input;
	FILE_ENTRY entry;
	int len;

	if (strlen(name) + strlen(path) + 1 > 127) {
		fprintf(stderr, "Path %s/%s is too long\n", path, path);
		exit(-1);
	}

	memset(entry.name, 0, 128);
	if (path[strlen(path) - 1] == '/')
		sprintf(entry.name, "%s%s", path, name);
	else
		sprintf(entry.name, "%s/%s", path, name);
	
	if ((input = fopen(entry.name, "rb")) == NULL)
		return 0;
	fseek(input, 0, SEEK_END);
	len = ftell(input);
	fclose(input);

	entry.pos = htonl(pos);
	entry.len = htonl(len);

	
	fwrite(&entry, sizeof(entry), 1, fp);

	return len;
}


int writeSection(const char *fname, const char *path, FILE *fp) {
	FILE *in;
	char name[128], buff[8192];

	sprintf(name, "%s/%s", path, fname);

	if ((in = fopen(name, "rb")) == NULL) {
		fprintf(stderr, "Unable to open section %s\n", fname);
		return -1;
	}

	while (!feof(in))
		fwrite(buff, fread(buff, 1, 4096, in), 1, fp);
	fclose(in);

	return 0;
}


int scanDirectory(const char *directory, FILE *fp, int prep) {
	DIR *dir;
	struct dirent *file;
	int successful = 0, pos = 0;

	if ((dir = opendir(directory)) == NULL) {
		fprintf(stderr, "Unable to open directory %s\n", directory);
		return -1;
	}

	do {
		file = readdir(dir);
		if (file == NULL)
			break;
		if (file->d_name[0] != '.') {
			if (prep)
				pos += writeHeader(file->d_name, directory, fp, pos);
			else 
				successful += (writeSection(file->d_name, directory, fp) == 0) ? 1 : 0;
		}
	} while(1);

	closedir(dir);

	return successful;
}


int main(int argc, char **argv) {
	FILE *fp;
	int sec, file_len;
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
	m.version = htonl(VERSION);
	m.files = htonl(0);

	fwrite(&m, sizeof(FILE_MAIN), 1, fp);
	scanDirectory(argv[1], fp, 1);
	if ((sec = scanDirectory(argv[1], fp, 0)) < 1) {
		fprintf(stderr, "No usable data, found, aborting...\n");
		fclose(fp);
		unlink(argv[2]);
		return -1;
	}

	fseek(fp, 0, SEEK_SET);
	m.files = htonl(sec);
	fwrite(&m, sizeof(FILE_MAIN), 1, fp);
	fseek(fp, 0, SEEK_END);
	file_len = htonl(ftell(fp));
	fwrite(&file_len, 4, 1, fp);
	fwrite(&m.magic, 4, 1, fp);

	fclose(fp);

	return 0;
}
