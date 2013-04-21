/*
Copyright (c) 2013 Steven Arnow
'main.c' - This file is part of libdarnit_tpw

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

#include "main.h"

TPW tpw;

int tpw_init() {
	if (!tpw_event_init(512))
		return 0;
	memset(tpw.common.sdl_keys, 0, 311);
	return tpw_init_platform();
}

char *tpw_keystate() {
	return tpw.common.sdl_keys;
}
