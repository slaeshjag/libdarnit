/*
Copyright (c) 2013 Steven Arnow
'threads_sdl.c' - This file is part of libdarnit_tpw

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

#include "../threads.h"

TPW_MUTEX *tpw_mutex_create() {
	return (TPW_MUTEX *) CreateMutex(NULL, FALSE, NULL);
}


void tpw_mutex_lock(TPW_MUTEX *mutex) {
	WaitForSingleObject((HANDLE) mutex, INFINITE);
	
	return;
}


void tpw_mutex_unlock(TPW_MUTEX *mutex) {
	ReleaseMutex((HANDLE *) mutex);

	return;
}


TPW_THREAD *tpw_thread_new(void *func, void *func_arg) {
	return (TPW_THREAD *) CreateThread(NULL, 0, func, func_arg, 0, NULL);
}


void tpw_thread_exit(int exit_code) {
	return ExitThread(exit_code);
}


void tpw_thread_kill(TPW_THREAD *t) {
	TerminateThread((HANDLE) t, -127);
	return;
}


void tpw_kill_self(int exit_code) {
	return ExitProcess(exit_code);
}


TPW_SEMAPHORE *tpw_semaphore_new(int initial_count) {
	return (TPW_SEMAPHORE *) CreateSemaphore(NULL, initial_count, 8192, NULL);
}


void tpw_semaphore_wait(TPW_SEMAPHORE *s) {
	WaitForSingleObject((TPW_SEMAPHORE) s, INFINITE);
	return;
}


void tpw_semaphore_add(TPW_SEMAPHORE *s, int add) {
	ReleaseSemaphore((TPW_SEMAPHORE) s, add, NULL);
}


void tpw_semaphore_delete(TPW_SEMAPHORE *s) {
	CloseHandle((TPW_SEMAPHORE) s);

	return;
}
	
