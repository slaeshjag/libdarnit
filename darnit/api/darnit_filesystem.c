/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_filesystem.c' - This file is part of libdarnit

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


void EXPORT_THIS *d_file_ldi_write(FILESYSTEM_FILE *f, int files) {
	return fsWriteLDI(f, files);
}


int EXPORT_THIS d_file_ldi_write_file(void *h, const char *filename, void *data, unsigned int size) {
	return fsWriteLDIFile(h, filename, data, size);
}


void EXPORT_THIS *d_file_ldi_write_end(void *h) {
	return fsWriteLDIFree(h);
}
	


#endif
