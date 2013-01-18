#ifndef __DARNIT_FILESYSTEM_H__
#define	__DARNIT_FILESYSTEM_H__

#include <stdlib.h>

#ifdef _WIN32
	#define		off_t		long long int
#endif


#define	DARNIT_FILESYSTEM_TYPE_READ	0x1
#define	DARNIT_FILESYSTEM_TYPE_WRITE	0x2

typedef struct DARNIT_DIR_LIST {
	const char			*fname;
	unsigned int			directory	: 1;
	unsigned int			file		: 1;
	unsigned int			writeable	: 1;
	unsigned int			in_file_image	: 1;
	struct DARNIT_DIR_LIST		*next;
} DARNIT_DIR_LIST;


typedef void DARNIT_FILE;

int d_fs_mount(const char *fname);
void d_fs_unmount(const char *fname);

int d_fs_mount_self();
void d_fs_unmount_self();

DARNIT_FILE *d_file_open(const char *fname, const char *mode);
DARNIT_FILE *d_file_close(DARNIT_FILE *file);

void d_directory_create(void *dir_name);
size_t d_file_read_ints(void *buffer, size_t ints, DARNIT_FILE *file);
size_t d_file_read(void *buffer, size_t bytes, DARNIT_FILE *file);
size_t d_file_write_ints(void *buffer, size_t ints, DARNIT_FILE *file);
size_t d_file_write(void *buffer, size_t bytes, DARNIT_FILE *file);
size_t d_file_gets(void *buffer, size_t bytes, DARNIT_FILE *file);
size_t d_file_getln(void *buffer, size_t bytes, DARNIT_FILE *file);
size_t d_file_whitespace_skip(DARNIT_FILE *file);

int d_file_read_compressed(DARNIT_FILE *file, void *data, int len);
int d_file_write_compressed(DARNIT_FILE *file, void *data, int len);

off_t d_file_tell(DARNIT_FILE *file);
size_t d_file_seek(DARNIT_FILE *file, off_t offset, int mode);
int d_file_eof(DARNIT_FILE *file);

DARNIT_DIR_LIST *d_file_list(const char *path, unsigned int type, int *entries);
DARNIT_DIR_LIST *d_file_list_free(DARNIT_DIR_LIST *list);

#endif
