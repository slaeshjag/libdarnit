#include "darnit.h"


int fsInit(void *handle, const char *dir_name) {
	DARNIT *m = handle;
	const char *data_dir;
	
	if (m->platform.platform & DARNIT_PLATFORM_PANDORA) {
		m->fs.data_dir = "./";
		m->fs.write_dir = "./";
	} else if (m->platform.platform & DARNIT_PLATFORM_LINUX) {
		data_dir = getenv("HOME");

		if ((m->fs.write_dir = malloc(strlen(data_dir) + 3 + strlen(dir_name))) == NULL)
			return -1;
		sprintf(m->fs.write_dir, "%s/.%s", data_dir, dir_name);

		if ((m->fs.data_dir = malloc(strlen(DATA_PATH) + 2 + strlen(dir_name))) == NULL)
			return -1;
		sprintf(m->fs.data_dir, "%s/%s", DATA_PATH, dir_name);
	} else {
		/* TODO: Add more platforms */
		m->fs.data_dir = (char *) "./";
		m->fs.write_dir = (char *) "./";
	}

	m->fs.mount = NULL;

	return 0;
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


FILESYSTEM_FILE *fsFileNew(char *name, const char *mode, FILE *fp, off_t file_size) {
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
	file->offset = ftell(file->fp);
	file->pos = 0;
	file->size = file_size;

	return file;
}


FILESYSTEM_FILE *fsFileOpen(void *handle, const char *name, const char *mode) {
	DARNIT *m = handle;
	char path[DARNIT_PATH_MAX];
	char *path_new;
	FILE *fp;
	int write = 0;

	if (strlen(m->fs.data_dir) + 2 + strlen(name) > DARNIT_PATH_MAX)
		return NULL;
	if (strlen(m->fs.write_dir) + 2 + strlen(name) > DARNIT_PATH_MAX)
		return NULL;
	
	if (*name == '/');				/* Path is absolute, skip all FS stuff */
	else if (strstr(mode, "w") == NULL 
	    && strstr(mode, "a") == NULL) {		/* Try read-only locations */
		/* Build data-dir path */
		sprintf(path, "%s/%s", m->fs.data_dir, name);
		path_new = utilPathTranslate(path);
		if ((fp = fopen(path_new, mode)) == NULL);
		else
			return fsFileNew(path_new, mode, fp, fsFILELenghtGet(fp));
		free(path_new);

		/* Look in data containers */
		path_new = malloc(strlen(name) + 1);
		sprintf(path_new, "%s", name);
		if ((fp = fsContainerFileInternalGet(handle, path_new)) == NULL);
		else
			return fsFileNew(path_new, mode, fsFILEDup(fsContainerFS(handle, fp)), fsContainerFILELength(handle, fp, name));
		free(path_new);
	} else {
		write = 1;
		/* Write-dir up next... */
		sprintf(path, "%s/%s", m->fs.write_dir, name);
		path_new = utilPathTranslate(path);
		if ((fp = fopen(path_new, mode)) == NULL);
		else
			return fsFileNew(path_new, mode, fp, -1);
		free(path_new);
	}

	/* Mkay, that didn't work. I guess we'll try open it directly */
	path_new = utilPathTranslate(path);
	if ((fp = fopen(path_new, mode)) == NULL);
	else						/* W00t! */
		return fsFileNew(path_new, mode, fp, (write) ? -1 : fsFILELenghtGet(fp));
	free(path_new);
	return NULL;
}


size_t fsFileRead(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	if (file == NULL)
		return 0;
	if (file->pos + bytes > file->size)
		return 0;
	file->pos += file->size;
	fread(buffer, bytes, 1, file->fp);
	return bytes;				/* This *should* always be true. Unless there's disk problems */
}


size_t fsFileReadInts(void *handle, unsigned int *buffer, size_t ints, FILESYSTEM_FILE *file) {
	int bytes;
	if (file == NULL)
		return 0;
	
	bytes = fsFileRead(buffer, ints<<2, file);
	bytes >>= 2;
	utilBlockToHostEndian(handle, buffer, bytes);

	return bytes;
}


size_t fsFileWrite(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	if (file == NULL)
		return 0;
	return fwrite(buffer, 1, bytes, file->fp);
}


size_t fsFileWriteInts(void *handle, void *buffer, size_t ints, FILESYSTEM_FILE *file) {
	int bytes;
	if (file == NULL)
		return 0;
	
	utilBlockToHostEndian(handle, buffer, ints);		/* It actually just swaps endian */
	bytes = fsFileWrite(buffer, ints << 2, file);
	utilBlockToHostEndian(handle, buffer, ints);		/* Swap it back */
	
	return bytes >> 2;
}


off_t fsFileTell(FILESYSTEM_FILE *file) {
	if (file == NULL)
		return 0;
	return file->pos;
}


int fsFileSeek(FILESYSTEM_FILE *file, off_t offset, int mode) {
	if (file == NULL)
		return 0;
	if (mode == SEEK_SET) {
		if ((file->size <= offset && file->size > 0) || offset < 0)
			return -1;
		else {
			file->pos = offset;
			fseek(file->fp, file->pos, SEEK_SET);
			return 0;
		}
	} else if (mode == SEEK_CUR) {
		if ((file->pos + offset >= file->size && file->size > 0) || file->pos + offset < 0)
			return -1;
		else {
			file->pos += offset;
			fseek(file->fp, file->pos, SEEK_SET);
			return 0;
		}
	} else if (mode == SEEK_END) {
		if ((file->pos + offset >= file->size && file->size > 0) || file->size + offset < 0)
			return -1;
		else {
			file->pos = file->size + offset;
			fseek(file->fp, file->pos, SEEK_SET);
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


int fsMount(void *handle, const char *name) {
	DARNIT *m = handle;
	struct FILESYSTEM_IMAGE *img;
	FILESYSTEM_IMG_HEADER header;
	int i;

	if ((img = malloc(sizeof(struct FILESYSTEM_IMAGE))) == NULL)
		return -1;
	if ((img->file = fsFileOpen(m, name, "rb")) == NULL) {
		free(img);
		return -1;
	}
	
	fsFileReadInts(handle, (unsigned int *) &header, sizeof(header) >> 2, img->file);

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
		fsFileReadInts(handle, &img->dir[i].sum, 3, img->file);
		img->dir[i].comp = utilStringSum(img->dir[i].name);
	}

	img->next = m->fs.mount;
	m->fs.mount = img;

	return 0;
}


void fsUnmount(void *handle, const char *name) {
	DARNIT *m = handle;
	struct FILESYSTEM_IMAGE *next, *old;
	char *path;
	
	if (m->fs.mount == NULL)
		return;
	path = utilPathTranslate(name);
	if (strcmp(m->fs.mount->file->file, path) == 0) {
		next = m->fs.mount->next;
		free(m->fs.mount);
		m->fs.mount = next;
		free(path);
		return;
	}

	old = m->fs.mount;
	next = m->fs.mount->next;
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


off_t fsContainerFILELength(void *handle, FILE *fp, const char *name) {
	DARNIT *m = handle;
	struct FILESYSTEM_IMAGE *next;
	unsigned int comp;
	int i;
	char *path;

	path = utilPathTranslate(name);
	comp = utilStringSum(path);
	next = m->fs.mount;
	while (next) {
		for (i = 0; i < next->dir_ents && next->file->fp == fp; i++)
			if (next->dir[i].comp == comp && strcmp(next->dir[i].name, path) == 0) {
				free(path);
				return next->dir[i].length;
			}
		next = next->next;
	}

	free(path);

	return 0;
}


FILESYSTEM_FILE *fsContainerFS(void *handle, FILE *fp) {
	DARNIT *m = handle;
	struct FILESYSTEM_IMAGE *next;

	next = m->fs.mount;
	while (next) {
		if (next->file->fp == fp)
			return next->file;
		next = next->next;
	}

	return NULL;
}


FILE *fsContainerFileInternalGet(void *handle, const char *name) {
	DARNIT *m = handle;
	unsigned int comp;
	struct FILESYSTEM_IMAGE *next;
	int i;
	char *path;

	path = utilPathTranslate(name);
	comp = utilStringSum(path);
	next = m->fs.mount;
	while (next) {
		for (i = 0; i < next->dir_ents; i++)
			if (next->dir[i].comp == comp)
				if (strcmp(path, next->file->file) == 0) {
					free(path);
					return next->file->fp;
				}
		next = next->next;
	}

	free(path);

	return NULL;
}
