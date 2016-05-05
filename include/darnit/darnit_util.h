/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_util.h' - This file is part of libdarnit

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


#ifndef __DARNIT_UTIL_H__
#define	__DARNIT_UTIL_H__

typedef struct {
	unsigned int		w;
	unsigned int		h;
	unsigned int		*data;
} DARNIT_IMAGE_DATA;

typedef void DARNIT_RANDOM;
typedef void DARNIT_MUTEX;
typedef void DARNIT_SEMAPHORE;
typedef void DARNIT_THREAD;


unsigned int d_util_htonl(unsigned int arg);
unsigned int d_util_ntohl(unsigned int arg);
char *d_util_path_translate(const char *path);
int d_util_string_to_int_array(const char *str, const char *delimiter, int *dest, int max_tokens);
void d_util_endian_convert(unsigned int *block, int elements);
int d_util_sin(int angle);
const char *d_str_null(const char *str);
DARNIT_IMAGE_DATA d_img_load_raw(const char *fname);
int d_util_compress(void *data_in, unsigned int data_len, void **data_out);
int d_util_decompress(void *data_in, unsigned int data_len, void **data_out);

DARNIT_RANDOM *d_util_random_new(unsigned int seed);
unsigned int d_util_random_get(DARNIT_RANDOM *random);
DARNIT_RANDOM *d_util_random_free(DARNIT_RANDOM *random);


DARNIT_MUTEX *d_util_mutex_create();
void d_util_mutex_lock(DARNIT_MUTEX *mutex);
void d_util_mutex_unlock(DARNIT_MUTEX *mutex);
DARNIT_THREAD *d_util_thread_new(void *func, void *func_arg);
void d_util_thread_exit(int exit_code);
void d_util_thread_kill(DARNIT_THREAD *thread);
void d_util_thread_kill_self(DARNIT_THREAD *thread);
DARNIT_SEMAPHORE *d_util_semaphore_new(int initial_count);
void d_util_semaphore_wait(DARNIT_SEMAPHORE *sem);
void d_util_semaphore_add(DARNIT_SEMAPHORE *sem, int add);
void d_util_semahpore_delete(DARNIT_SEMAPHORE *sem);


#endif
