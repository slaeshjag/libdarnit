#ifndef __FILESYSTEM_H__
#define	__FILESYSTEM_H__


#define DARNIT_PATH_MAX		2048
#define	DARNIT_FS_IMG_MAGIC	0x83B3661B
#define	DARNIT_FS_IMG_VERSION	0xBBA77ABC


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
	unsigned int			sum;
	unsigned int			pos;
	unsigned int			length;
	unsigned int			comp;
} FILESYSTEM_IMAGE_FILE;


struct FILESYSTEM_IMAGE {
	struct FILESYSTEM_IMAGE		*next;

	FILESYSTEM_FILE			*file;
	FILESYSTEM_IMAGE_FILE		*dir;
	unsigned int			dir_ents;
} FILESYSTEM_IMAGE;


typedef struct {
	char				*data_dir;
	char				*write_dir;
	struct FILESYSTEM_IMAGE		*mount;
} FILESYSTEM;



int fsInit(void *handle, const char *dir_name);
off_t fsFILELenghtGet(FILE *fp);
FILE *fsFILEDup(FILESYSTEM_FILE *file);
FILESYSTEM_FILE *fsFileNew(char *name, const char *mode, FILE *fp, off_t file_size);
FILESYSTEM_FILE *fsFileOpen(void *handle, const char *name, const char *mode);
size_t fsFileRead(void *buffer, size_t bytes, FILESYSTEM_FILE *file);
size_t fsFileWrite(void *buffer, size_t bytes, FILESYSTEM_FILE *file);
off_t fsFileTell(FILESYSTEM_FILE *file);
int fsFileSeek(FILESYSTEM_FILE *file, off_t offset, int mode);
FILESYSTEM_FILE *fsFileClose(FILESYSTEM_FILE *file);


/* Internal stuff. Etc. */
off_t fsContainerFILELength(void *handle, FILE *fp, const char *name);
FILESYSTEM_FILE *fsContainerFS(void *handle, FILE *fp);
FILE *fsContainerFileInternalGet(void *handle, const char *name);


#endif
