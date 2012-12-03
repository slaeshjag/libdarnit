#ifndef __DARNIT_FILESYSTEM_H__
#define	__DARNIT_FILESYSTEM_H__

#include "darnit.h"


int EXPORT_THIS darnitFSMount(const char *fname) {
	return fsMount(fname);
}


void EXPORT_THIS darnitFSUnmount(const char *fname) {
	fsUnmount(fname);
	
	return;
}


FILESYSTEM_FILE EXPORT_THIS *darnitFileOpen(const char *fname, const char *mode) {
	return fsFileOpen(fname, mode);
}


FILESYSTEM_FILE EXPORT_THIS *darnitFileClose(FILESYSTEM_FILE *file) {
	return fsFileClose(file);
}


void EXPORT_THIS darnitDirectoryCreate(const char *dir_name) {
	fsDirectoryCreate(dir_name);

	return;
}


size_t EXPORT_THIS darnitFileIntsRead(void *buffer, size_t ints, FILESYSTEM_FILE *file) {
	return fsFileReadInts(buffer, ints, file);
}


size_t EXPORT_THIS darnitFileRead(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	return fsFileRead(buffer, bytes, file);
}


size_t EXPORT_THIS darnitFileIntsWrite(void *buffer, size_t ints, FILESYSTEM_FILE *file) {
	return fsFileWriteInts(buffer, ints, file);
}


size_t EXPORT_THIS darnitFileWrite(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	return fsFileWrite(buffer, bytes, file);
}


size_t EXPORT_THIS darnitFileGets(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	return fsFileGets(buffer, bytes, file);
}


size_t EXPORT_THIS darnitFileLineGet(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	return fsFileGetLine(buffer, bytes, file);
}


void EXPORT_THIS darnitFileWhitespaceSkip(FILESYSTEM_FILE *file) {
	fsFileSkipWhitespace(file);
	
	return;
}


off_t EXPORT_THIS darnitFileTell(FILESYSTEM_FILE *file) {
	return fsFileTell(file);
}


void EXPORT_THIS darnitFileSeek(FILESYSTEM_FILE *file, off_t offset, int mode) {
	fsFileSeek(file, offset, mode);

	return;
}


void EXPORT_THIS *darnitFileList(const char *path, unsigned int type, unsigned int *entries) {
	return fsDirectoryList(path, type, entries);
}


void EXPORT_THIS *darnitFileListFree(DIR_LIST *list) {
	return fsDirectoryListFree(list);
}


int EXPORT_THIS darnitFileEOF(FILESYSTEM_FILE *file) {
	return fsFileEOF(file);
}


int EXPORT_THIS darnitFileCompressedRead(FILESYSTEM_FILE *file, void *data, int len) {
	return fsReadCompressed(file, data, len);
}


int EXPORT_THIS darnitFileCompressedWrite(FILESYSTEM_FILE *file, void *data, int len) {
	return fsWriteCompressed(file, data, len);
}


#endif
