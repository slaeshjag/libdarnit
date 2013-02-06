#ifndef __DARNIT_FILESYSTEM_H__
#define	__DARNIT_FILESYSTEM_H__

#include "darnit.h"


int EXPORT_THIS d_fs_mount(const char *fname) {
	return fsMount(fname);
}


void EXPORT_THIS d_fs_unmount(const char *fname) {
	fsUnmount(fname);
	
	return;
}


char EXPORT_THIS *d_fs_exec_path() {
	return fsFindBinaryPath();
}


int EXPORT_THIS d_fs_mount_self() {
	return fsMountSelf();
}


void EXPORT_THIS d_fs_unmount_self() {
	fsUnmountSelf();

	return;
}


FILESYSTEM_FILE EXPORT_THIS *d_file_open(const char *fname, const char *mode) {
	return fsFileOpen(fname, mode);
}


FILESYSTEM_FILE EXPORT_THIS *d_file_close(FILESYSTEM_FILE *file) {
	return fsFileClose(file);
}


void EXPORT_THIS d_directory_create(const char *dir_name) {
	fsDirectoryCreate(dir_name);

	return;
}


size_t EXPORT_THIS d_file_read_ints(void *buffer, size_t ints, FILESYSTEM_FILE *file) {
	return fsFileReadInts(buffer, ints, file);
}


size_t EXPORT_THIS d_file_read(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	return fsFileRead(buffer, bytes, file);
}


size_t EXPORT_THIS d_file_write_ints(void *buffer, size_t ints, FILESYSTEM_FILE *file) {
	return fsFileWriteInts(buffer, ints, file);
}


size_t EXPORT_THIS d_file_write(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	return fsFileWrite(buffer, bytes, file);
}


size_t EXPORT_THIS d_file_gets(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	return fsFileGets(buffer, bytes, file);
}


size_t EXPORT_THIS d_file_getln(void *buffer, size_t bytes, FILESYSTEM_FILE *file) {
	return fsFileGetLine(buffer, bytes, file);
}


void EXPORT_THIS d_file_whitespace_skip(FILESYSTEM_FILE *file) {
	fsFileSkipWhitespace(file);
	
	return;
}


off_t EXPORT_THIS d_file_tell(FILESYSTEM_FILE *file) {
	return fsFileTell(file);
}


void EXPORT_THIS d_file_seek(FILESYSTEM_FILE *file, off_t offset, int mode) {
	fsFileSeek(file, offset, mode);

	return;
}


void EXPORT_THIS *d_file_list(const char *path, unsigned int type, unsigned int *entries) {
	return fsDirectoryList(path, type, entries);
}


void EXPORT_THIS *d_file_list_free(DIR_LIST *list) {
	return fsDirectoryListFree(list);
}


int EXPORT_THIS d_file_eof(FILESYSTEM_FILE *file) {
	return fsFileEOF(file);
}


int EXPORT_THIS d_file_read_compressed(FILESYSTEM_FILE *file, void *data, int len) {
	return fsReadCompressed(file, data, len);
}


int EXPORT_THIS d_file_write_compressed(FILESYSTEM_FILE *file, void *data, int len) {
	return fsWriteCompressed(file, data, len);
}


#endif
