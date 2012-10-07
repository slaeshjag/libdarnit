#ifndef __DARNIT_FILESYSTEM_H__
#define	__DARNIT_FILESYSTEM_H__

#include "darnit.h"


FILESYSTEM_FILE EXPORT_THIS *darnitFileOpen(const char *fname, const char *mode) {
	return fsFileOpen(fname, mode);
}


FILESYSTEM_FILE EXPORT_THIS *darnitFileClose(FILESYSTEM_FILE *file) {
	return fsFileClose(file);
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


#endif
