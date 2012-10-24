#ifndef __DARNIT_FILESYSTEM_H__
#define	__DARNIT_FILESYSTEM_H__

#include <stdlib.h>

#ifdef _WIN32
	#define		off_t		long long int
#endif

typedef void DARNIT_FILE;

int darnitFSMount(const char *fname);
void darnitFSUnmount(const char *fname);

DARNIT_FILE *darnitFileOpen(const char *fname, const char *mode);
DARNIT_FILE *darnitFileClose(DARNIT_FILE *file);

void darnitDirectoryCreate(void *dir_name);
size_t darnitFileIntsRead(void *buffer, size_t ints, DARNIT_FILE *file);
size_t darnitFileRead(void *buffer, size_t bytes, DARNIT_FILE *file);
size_t darnitFileIntsWrite(void *buffer, size_t ints, DARNIT_FILE *file);
size_t darnitFileWrite(void *buffer, size_t bytes, DARNIT_FILE *file);
size_t darnitFileGets(void *buffer, size_t bytes, DARNIT_FILE *file);
size_t darnitFileLineGet(void *buffer, size_t bytes, DARNIT_FILE *file);
size_t darnitFileWhitespaceSkip(DARNIT_FILE *file);
off_t darnitFileTell(DARNIT_FILE *file);
size_t darnitFileSeek(DARNIT_FILE *file, off_t offset, int mode);

#endif
