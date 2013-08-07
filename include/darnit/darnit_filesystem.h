/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_filesystem.h' - This file is part of libdarnit

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


#ifndef __DARNIT_FILESYSTEM_H__
#define	__DARNIT_FILESYSTEM_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#define		off_t		long long int
#endif


typedef enum {
	DARNIT_FILESYSTEM_TYPE_READ = 0x1,
	DARNIT_FILESYSTEM_TYPE_WRITE = 0x2,
} DARNIT_FILESYSTEM_TYPE;

typedef struct DARNIT_DIR_LIST {
	const char			*fname;
	unsigned int			directory	: 1;
	unsigned int			file		: 1;
	unsigned int			writeable	: 1;
	unsigned int			in_file_image	: 1;
	struct DARNIT_DIR_LIST		*next;
} DARNIT_DIR_LIST;


typedef void DARNIT_FILE;
typedef void DARNIT_LDI_WRITER;

int d_fs_mount(const char *fname);
void d_fs_unmount(const char *fname);

int d_fs_mount_self();
void d_fs_unmount_self();

char *d_fs_exec_path();

DARNIT_FILE *d_file_open(const char *fname, const char *mode);
DARNIT_FILE *d_file_close(DARNIT_FILE *file);

void d_directory_create(const char *dir_name);
size_t d_file_read_ints(void *buffer, size_t ints, DARNIT_FILE *file);
size_t d_file_read(void *buffer, size_t bytes, DARNIT_FILE *file);
size_t d_file_write_ints(void *buffer, size_t ints, DARNIT_FILE *file);
size_t d_file_write(void *buffer, size_t bytes, DARNIT_FILE *file);
size_t d_file_gets(void *buffer, size_t bytes, DARNIT_FILE *file);
size_t d_file_getln(void *buffer, size_t bytes, DARNIT_FILE *file);
void  d_file_whitespace_skip(DARNIT_FILE *file);

int d_file_read_compressed(DARNIT_FILE *file, void *data, int len);
int d_file_write_compressed(DARNIT_FILE *file, void *data, int len);

off_t d_file_tell(DARNIT_FILE *file);
int d_file_seek(DARNIT_FILE *file, off_t offset, int mode);
int d_file_eof(DARNIT_FILE *file);
void d_file_size_set(DARNIT_FILE *file, off_t size);

DARNIT_DIR_LIST *d_file_list(const char *path, DARNIT_FILESYSTEM_TYPE type, int *entries);
DARNIT_DIR_LIST *d_file_list_free(DARNIT_DIR_LIST *list);

DARNIT_LDI_WRITER *d_file_ldi_write(DARNIT_FILE *f, int files);
int d_file_ldi_write_file(DARNIT_LDI_WRITER *h, const char *filename, void *data, unsigned int size);
DARNIT_LDI_WRITER *d_file_ldi_write_end(DARNIT_LDI_WRITER *h);

#endif
