#include "darnit.h"


int fsInit(const char *dir_name) {
	const char *data_dir;
	
	if (d->platform.platform & DARNIT_PLATFORM_PANDORA) {
		d->fs.data_dir = "./";
		d->fs.write_dir = "./";
	} else if (d->platform.platform & DARNIT_PLATFORM_LINUX) {
		#ifndef _WIN32
		data_dir = getenv("HOME");

		if ((d->fs.write_dir = malloc(strlen(data_dir) + 3 + strlen(dir_name) + strlen(".darnit/"))) == NULL)
			return -1;
		sprintf(d->fs.write_dir, "%s/.darnit", data_dir);
		mkdir(d->fs.write_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		sprintf(d->fs.write_dir, "%s/.darnit/%s", data_dir, dir_name);

		if ((d->fs.data_dir = malloc(strlen(DATA_PATH) + 2 + strlen(dir_name))) == NULL)
			return -1;
		sprintf(d->fs.data_dir, "%s/%s", DATA_PATH, dir_name);
		#endif
	} else if (d->platform.platform & DARNIT_PLATFORM_WIN32) { 
		data_dir = getenv("%%APPDATA%%");

		if ((d->fs.write_dir = malloc(strlen(data_dir) + 2 + strlen(dir_name))) == NULL)
			return -1;
		sprintf(d->fs.write_dir, "%s/%s", data_dir, dir_name);

		/* Ugh... WinAPI... */
		#ifdef _WIN32
			char tmp[256], val[256];
			int sz;
			sprintf(tmp, "software/libdarnit/%s", dir_name);
		
			if (SHGetValue(HKEY_LOCAL_MACHINE, tmp, "path", NULL, val, (LPDWORD) &sz) != ERROR_SUCCESS)
				d->fs.data_dir = "./";
			else {
				if ((d->fs.data_dir = malloc(strlen(val) + 1)) == NULL)
					d->fs.data_dir = "./";
				else
					sprintf(d->fs.data_dir, "%s", val);
			}
		#else
			d->fs.data_dir = "./";
		#endif
	} else {
		/* TODO: Add more platforms */
		d->fs.data_dir = (char *) "./";
		d->fs.write_dir = (char *) "./";
	}

	#ifdef _WIN32
		CreateDirectory(d->fs.write_dir, NULL);
	#else
		mkdir(d->fs.write_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	#endif

	d->fs.mount = NULL;

	return 0;
}


void fsDirectoryCreate(const char *dir_name) {
	char tmp[256];
	if (strlen(dir_name) + 2 + strlen(d->fs.write_dir) > 256)
		return;
	#ifdef _WIN32
		sprintf(tmp, "%s\\%s", d->fs.write_dir, dir_name);
		CreateDirectory(tmp, NULL);
	#else
		sprintf(tmp, "%s/%s", d->fs.write_dir, dir_name);
		mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	#endif

	return;
}
	
		


off_t fsFILELenghtGet(FILE *fp) {
	off_t orig, end;

	orig = ftell(fp);
	fseek(fp, 0, SEEK_END);
	end = ftell(fp);
	fseek(fp, orig, SEEK_SET);
	return end - orig;
}


FILE *fsFILEDup(FILESYSTEM_FILE *file) {
	FILE *copy;
	
	if (file->fp == NULL)
		return NULL;

	if ((copy = fopen(file->file, file->mode)) == NULL)
		return NULL;
	fseek(copy, ftell(file->fp), SEEK_SET);

	return copy;
}


FILESYSTEM_FILE *fsFileNew(char *name, const char *mode, FILE *fp, off_t file_size, off_t file_start) {
	FILESYSTEM_FILE *file;

	if ((file = malloc(sizeof(FILESYSTEM_FILE))) == NULL)
		return NULL;
	file->fp = fp;

	if ((file->mode = malloc(strlen(mode) + 1)) == NULL) {
		free(file->file);
		free(file);
		return NULL;
	}

	file->file = name;
	sprintf(file->mode, "%s", mode);
	file->offset = file_start;
	file->pos = 0;
	file->size = file_size;
	fseek(file->fp, file_start, SEEK_SET);

	return file;
}


FILESYSTEM_FILE *fsFileOpen(const char *name, const char *mode) {
	char path[DARNIT_PATH_MAX];
	char *path_new;
	FILE *fp;
	int write = 0;

	if (strlen(d->fs.data_dir) + 2 + strlen(name) > DARNIT_PATH_MAX)
		return NULL;
	if (strlen(d->fs.write_dir) + 2 + strlen(name) > DARNIT_PATH_MAX)
		return NULL;
	
	if (*name == '/');				/* Path is absolute, skip all FS stuff */
	else if (strstr(mode, "w") == NULL 
	    && strstr(mode, "a") == NULL) {		/* Try read-only locations */
		/* Build data-dir path */
		sprintf(path, "%s/%s", d->fs.data_dir, name);
		path_new = utilPathTranslate(path);
		if ((fp = fopen(path_new, mode)) == NULL);
		else
			return fsFileNew(path_new, mode, fp, fsFILELenghtGet(fp), 0);
		free(path_new);

		/* Look in data containers */
		path_new = malloc(strlen(name) + 1);
		sprintf(path_new, "%s", name);
		if ((fp = fsContainerFileInternalGet(path_new)) == NULL);
		else {
			return fsFileNew(path_new, mode, fsFILEDup(fsContainerFS(fp)), fsContainerFILELength(fp, name), fsContainerFILEStart(fp, name));
		}
		free(path_new);
	} else {
		write = 1;
		/* Write-dir up next... */
		sprintf(path, "%s/%s", d->fs.write_dir, name);
		path_new = utilPathTranslate(path);
		if ((fp = fopen(path_new, mode)) == NULL);
		else
			return fsFileNew(path_new, mode, fp, -1, 0);
		free(path_new);
	}

	/* Mkay, that didn't work. I guess we'll try open it directly */
	path_new = utilPathTranslate(name);
	if ((fp = fopen(path_new, mode)) == NULL);
	else						/* W00t! */
		return fsFileNew(path_new, mode, fp, (write) ? -1 : fsFILELenghtGet(fp), 0);
	free(path_new);
	return NULL;
}


size_t fsFileRead(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	if (file == NULL)
		return 0;
	if (bytes < 0)
		return 0;
	if (file->pos + bytes > file->size)
		bytes = file->size - file->pos;
	file->pos += bytes;
	fread(buffer, bytes, 1, file->fp);
	return bytes;				/* This *should* always be true. Unless there's disk problems */
}


size_t fsFileReadInts(unsigned int *buffer, size_t ints, FILESYSTEM_FILE *file) {
	int bytes;
	if (file == NULL)
		return 0;
	
	bytes = fsFileRead(buffer, ints<<2, file);
	bytes >>= 2;
	utilBlockToHostEndian(buffer, bytes);

	return bytes;
}


size_t fsFileWrite(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	if (file == NULL)
		return 0;
	if (bytes < 0)
		return 0;
	file->pos += bytes;
	return fwrite(buffer, 1, bytes, file->fp);
}


size_t fsFileWriteInts(void *buffer, size_t ints, FILESYSTEM_FILE *file) {
	int bytes;
	if (file == NULL)
		return 0;
	
	utilBlockToHostEndian(buffer, ints);			/* It actually just swaps endian */
	bytes = fsFileWrite(buffer, ints << 2, file);
	utilBlockToHostEndian(buffer, ints);			/* Swap it back */
	
	return bytes >> 2;
}


size_t fsFileGets(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	int lim;
	if (file == NULL)
		return 0;
	if (bytes <= 0)
		return 0;
	lim = (bytes > file->size - file->pos) ? file->size - file->pos : bytes;
	fgets(buffer, lim, file->fp);
	file->pos += strlen(buffer);

	return strlen(buffer);
}


size_t fsFileGetLine(char *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	if (file == NULL)
		return 0;
	if (buffer[(bytes = fsFileGets(buffer, bytes, file)) - 1] == '\n')
		buffer[bytes - 1] = 0;
	return bytes;
}	


off_t fsFileTell(FILESYSTEM_FILE *file) {
	if (file == NULL)
		return 0;
	return file->pos;
}


void fsFileSkipWhitespace(FILESYSTEM_FILE *file) {
	char c;

	do {
		fsFileRead(&c, 1, file);
		if (fsFileEOF(file)) break;
	} while (c == ' ' || c == '\t');

	fsFileSeek(file, -1, SEEK_CUR);
	return;
}


int fsFileEOF(FILESYSTEM_FILE *file) {
	if (file->pos == file->size)
		return 1;
	if (file->size == -1)
		return feof(file->fp);
	return 0;
}


int fsFileSeek(FILESYSTEM_FILE *file, off_t offset, int mode) {
	if (file == NULL)
		return 0;
	if (mode == SEEK_SET) {
		if ((file->size < offset && file->size > 0) || offset < 0)
			return -1;
		else {
			file->pos = offset;
			fseek(file->fp, file->pos + file->offset, SEEK_SET);
			return 0;
		}
	} else if (mode == SEEK_CUR) {
		if ((file->pos + offset > file->size && file->size > 0) || file->pos + offset < 0)
			return -1;
		else {
			file->pos += offset;
			fseek(file->fp, file->pos, SEEK_SET);
			return 0;
		}
	} else if (mode == SEEK_END) {
		if ((file->pos + offset > file->size && file->size > 0) || file->size + offset < 0)
			return -1;
		else {
			file->pos = file->size;
			fseek(file->fp, file->pos + offset, SEEK_SET);
			return 0;
		}
	}

	return -1;
}


FILESYSTEM_FILE *fsFileClose(FILESYSTEM_FILE *file) {
	if (file == NULL)
		return NULL;
	fclose(file->fp);
	free(file->file);
	free(file->mode);
	free(file);

	return NULL;
}


int fsMount(const char *name) {
	struct FILESYSTEM_IMAGE *img;
	FILESYSTEM_IMG_HEADER header;
	int i;

	if ((img = malloc(sizeof(struct FILESYSTEM_IMAGE))) == NULL)
		return -1;
	if ((img->file = fsFileOpen(name, "rb")) == NULL) {
		free(img);
		return -1;
	}
	
	fsFileReadInts((unsigned int *) &header, sizeof(header) >> 2, img->file);

	if (header.magic != DARNIT_FS_IMG_MAGIC || header.version != DARNIT_FS_IMG_VERSION) {
		fsFileClose(img->file);
		free(img);
		return -1;
	}

	img->dir_ents = header.files;

	if ((img->dir = malloc(sizeof(FILESYSTEM_IMAGE_FILE) * img->dir_ents)) == NULL) {
		fsFileClose(img->file);
		free(img);
		return -1;
	}
	
	for (i = 0; i < img->dir_ents; i++) {
		fsFileRead(img->dir[i].name, 128, img->file);
		fsFileReadInts(&img->dir[i].pos, 3, img->file);
		img->dir[i].comp = utilStringSum(img->dir[i].name);
		img->dir[i].pos += (12 + 140 * img->dir_ents);
	}

	img->next = d->fs.mount;
	d->fs.mount = img;

	return 0;
}


void fsUnmount(const char *name) {
	struct FILESYSTEM_IMAGE *next, *old;
	char *path;
	
	if (d->fs.mount == NULL)
		return;
	path = utilPathTranslate(name);
	if (strcmp(d->fs.mount->file->file, path) == 0) {
		next = d->fs.mount->next;
		free(d->fs.mount);
		d->fs.mount = next;
		free(path);
		return;
	}

	old = d->fs.mount;
	next = d->fs.mount->next;
	while (next != NULL) {
		if (strcmp(next->file->file, path) == 0) {
			old->next = next->next;
			free(next);
			free(path);
			return;
		}
	}
	
	free(path);

	return;
}


FILESYSTEM_IMAGE_FILE *fsContainerGetDir(FILE *fp, const char *name) {
	struct FILESYSTEM_IMAGE *next;
	unsigned int comp;
	int i;

	comp = utilStringSum(name);
	next = d->fs.mount;
	while (next) {
		for (i = 0; i < next->dir_ents && next->file->fp == fp; i++)
			if (next->dir[i].comp == comp && strcmp(next->dir[i].name, name) == 0)
				return &next->dir[i];
		next = next->next;
	}

	return 0;
}


off_t fsContainerFILEStart(FILE *fp, const char *name) {
	FILESYSTEM_IMAGE_FILE *img;

	if ((img = fsContainerGetDir(fp, name)) == NULL)
		return 0;
	return img->pos;
}


off_t fsContainerFILELength(FILE *fp, const char *name) {
	FILESYSTEM_IMAGE_FILE *img;

	if ((img = fsContainerGetDir(fp, name)) == NULL)
		return 0;
	return img->length;
}


FILESYSTEM_FILE *fsContainerFS(FILE *fp) {
	struct FILESYSTEM_IMAGE *next;

	next = d->fs.mount;
	while (next) {
		if (next->file->fp == fp)
			return next->file;
		next = next->next;
	}

	return NULL;
}


FILE *fsContainerFileInternalGet(const char *name) {
	unsigned int comp;
	struct FILESYSTEM_IMAGE *next;
	int i;

	comp = utilStringSum(name);
	next = d->fs.mount;
	while (next) {
		for (i = 0; i < next->dir_ents; i++)
			if (next->dir[i].comp == comp)
				if (strcmp(name, next->dir[i].name) == 0) {
					return next->file->fp;
				}
		next = next->next;
	}

	return NULL;
}
