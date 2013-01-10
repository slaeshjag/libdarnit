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
	char				*mode;
	off_t				offset;
	off_t				pos;
	off_t				size;
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
	char				*data_dir;
	char				*write_dir;
	struct FILESYSTEM_IMAGE		*mount;
} FILESYSTEM;



int fsMount(const char *name);
void fsUnmount(const char *name);

int fsInit(const char *dir_name);
void fsDirectoryCreate(const char *dir_name);
off_t fsFILELenghtGet(FILE *fp);
FILE *fsFILEDup(FILESYSTEM_FILE *file);
FILESYSTEM_FILE *fsFileNew(char *name, const char *mode, FILE *fp, off_t file_size, off_t file_start);
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
FILESYSTEM_FILE *fsFileClose(FILESYSTEM_FILE *file);

DIR_LIST *fsDirectoryList(const char *path, unsigned int type, unsigned int *entries);
DIR_LIST *fsDirectoryListFree(DIR_LIST *list);

/* Internal stuff. Etc. */
off_t fsContainerFILELength(FILE *fp, const char *name);
off_t fsContainerFILEStart(FILE *fp, const char *name);
FILESYSTEM_FILE *fsContainerFS(FILE *fp);
FILE *fsContainerFileInternalGet(const char *name);


#endif
