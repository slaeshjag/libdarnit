/*
Copyright (c) 2013 Steven Arnow
'threads_dummy.c' - This file is part of libdarnit_tpw

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


#define	TPW_INTERNAL
#include "../threads.h"

TPW_MUTEX *tpw_mutex_create() {
	#warning tpw_mutex_create(): Not implemented yet
	return NULL;
}


void tpw_mutex_lock(TPW_MUTEX *mutex) {
	#warning tpw_mutex_lock(): Not implemented yet
	return;
}


void tpw_mutex_unlock(TPW_MUTEX *mutex) {
	#warning tpw_mutex_unlock(): Not implemented yet
	return;
}
