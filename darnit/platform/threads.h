/*
Copyright (c) 2013 Steven Arnow
'threads.h' - This file is part of libdarnit_tpw

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

#ifndef __TPW_THREADS_H__
#define	__TPW_THREADS_H__

#include "platform_threads.h"


TPW_MUTEX *tpw_mutex_create();
void tpw_mutex_lock(TPW_MUTEX *mutex);
void tpw_mutex_unlock(TPW_MUTEX *mutex);

TPW_THREAD tpw_thread_new(void *func, void *func_arg);
void tpw_thread_exit(int exit_code);


#endif
