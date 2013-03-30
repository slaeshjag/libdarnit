/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_mtsprite.h' - This file is part of libdarnit

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


#ifndef	__DARNIT_MTSPRITE_H__
#define	__DARNIT_MTSPRITE_H__

typedef void DARNIT_MTSPRITE;

DARNIT_MTSPRITE *d_mtsprite_new(void *tilesheet);
DARNIT_MTSPRITE *d_mtsprite_load(const char *fname);
DARNIT_MTSPRITE *d_mtsprite_free(DARNIT_MTSPRITE *mtsprite);
void d_mtsprite_animate_start(DARNIT_MTSPRITE *mtsprite);
void d_mtsprite_animate_pause(DARNIT_MTSPRITE *mtsprite);
void d_mtsprite_animate_stop(DARNIT_MTSPRITE *mtsprite);
void d_mtsprite_animate_repeat(DARNIT_MTSPRITE *mtsprite, int repeat);
void d_mtsprite_tile_add(DARNIT_MTSPRITE *mtsprite, int x, int y, int w, int h, int rx, int ry);
void d_mtsprite_frame_set(DARNIT_MTSPRITE *mtsprite, int time);
void d_mtsprite_draw(DARNIT_MTSPRITE *mtsprite);

#endif
