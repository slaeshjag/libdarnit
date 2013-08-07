/*
Copyright (c) 2011-2013 Steven Arnow
'filesystem.h' - This file is part of libdarnit

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/


#ifndef __FILESYSTEM_H__
#define	__FILESYSTEM_H__


#define DARNIT_PATH_MAX		2048
#define	DARNIT_FS_IMG_MAGIC	0x83B3661B
#define	DARNIT_FS_IMG_VERSION	0xBBA77ABC


#define	DARNIT_FS_READABLE	1
#define	DARNIT_FS_WRITEABLE	2

#include <bzlib.h>

#ifndef _WIN32
	#include	<sys/stat.h>
	#include	<sys/types.h>
	#include	<sys/stat.h>
	#include	<dirent.h>
#else
	#define		DATA_PATH
	#include 	<shlwapi.h>
#endif



typedef struct DIR_LIST {
	char				*fname;
	unsigned int			directory	: 1;
	unsigned int			file		: 1;
	unsigned int			writeable	: 1;
	unsigned int			in_file_image	: 1;
	struct DIR_LIST			*next;
} DIR_LIST;


typedef struct {
	unsigned int			magic;
	unsigned int			version;
	unsigned int			files;
} FILESYSTEM_IMG_HEADER;


typedef struct {
	FILE				*fp;
	char				*file;
	char				*parent;
	char				*mode;
	off_t				offset;
	off_t				pos;
	off_t				size;
	int				temporary;
} FILESYSTEM_FILE;


typedef struct {
	char				name[128];
	unsigned int			pos;
	unsigned int			length;
	unsigned int			comp;
} FILESYSTEM_IMAGE_FILE;


typedef struct FILESYSTEM_IMAGE {
	struct FILESYSTEM_IMAGE		*next;

	FILESYSTEM_FILE			*file;
	FILESYSTEM_IMAGE_FILE		*dir;
	off_t				offset;
	unsigned int			dir_ents;
} FILESYSTEM_IMAGE;


typedef struct {
	FILESYSTEM_FILE			*f;
	unsigned int			files;
	unsigned int			file;
	off_t				cur_offset;
	off_t				start_offset;
} FILESYSTEM_IMAGE_WRITER;


typedef struct {
	char				*data_dir;
	char				*write_dir;
	struct FILESYSTEM_IMAGE		*mount;
	char				*directory_suffix;
	int				temp_counter;
} FILESYSTEM;



int fsMount(const char *name);
void fsUnmount(const char *name);

int fsMountSelf();
void fsUnmountSelf();

int fsInit(const char *dir_name);
void fsDirectoryCreate(const char *dir_name);
char *fsFindBinaryPath();


off_t fsFILELenghtGet(FILE *fp);
FILE *fsFILEDup(FILESYSTEM_FILE *file);
FILESYSTEM_FILE *fsFileNew(char *name, const char *mode, FILE *fp, off_t file_size, off_t file_start, char *parent);
FILESYSTEM_FILE *fsFileOpen(const char *name, const char *mode);
size_t fsFileReadInts(unsigned int *buffer, size_t ints, FILESYSTEM_FILE *file);
size_t fsFileRead(void *buffer, size_t bytes, FILESYSTEM_FILE *file);
size_t fsFileWriteInts(void *buffer, size_t ints, FILESYSTEM_FILE *file);
size_t fsFileWrite(void *buffer, size_t bytes, FILESYSTEM_FILE *file);
size_t fsFileGets(void *buffer, size_t bytes, FILESYSTEM_FILE *file);
size_t fsFileGetLine(char *buffer, size_t bytes, FILESYSTEM_FILE *file);
void fsFileSkipWhitespace(FILESYSTEM_FILE *file);

int fsWriteCompressed(FILESYSTEM_FILE *f, void *data, int len);
int fsReadCompressed(FILESYSTEM_FILE *f, void *data, int len);

off_t fsFileTell(FILESYSTEM_FILE *file);
int fsFileEOF(FILESYSTEM_FILE *file);
int fsFileSeek(FILESYSTEM_FILE *file, off_t offset, int mode);
void fsFileSetSize(FILESYSTEM_FILE *f, off_t size);
FILESYSTEM_FILE *fsFileClose(FILESYSTEM_FILE *file);

DIR_LIST *fsDirectoryList(const char *path, unsigned int type, unsigned int *entries);
DIR_LIST *fsDirectoryListFree(DIR_LIST *list);

FILESYSTEM_IMAGE_WRITER *fsWriteLDI(FILESYSTEM_FILE *f, int files);
int fsWriteLDIFile(FILESYSTEM_IMAGE_WRITER *w, const char *filename, void *data, unsigned int data_size);
void *fsWriteLDIFree(FILESYSTEM_IMAGE_WRITER *w);

/* Internal stuff. Etc. */
off_t fsContainerFILELength(FILE *fp, const char *name);
off_t fsContainerFILEStart(FILE *fp, const char *name);
FILESYSTEM_FILE *fsContainerFS(FILE *fp);
FILE *fsContainerFileInternalGet(const char *name);
FILESYSTEM_FILE *fsGetRealFile(const char *path_src);


#endif
