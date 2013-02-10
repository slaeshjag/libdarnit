/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_mtsprite.c' - This file is part of libdarnit

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


#include "darnit.h"


void EXPORT_THIS *d_mtsprite_new(void *tilesheet) {
	return mtSpriteNew(0, 0, tilesheet);
}


void EXPORT_THIS *d_mtsprite_load(const char *fname) {
	return mtSpriteLoad(fname);
}


void EXPORT_THIS d_mtsprite_animate_start(void *mtsprite) {
	mtSpriteEnableAnimation(mtsprite);

	return;
}


void EXPORT_THIS d_mtsprite_animate_pause(void *mtsprite) {
	mtSpritePauseAnimation(mtsprite);

	return;
}


void EXPORT_THIS d_mtsprite_tile_add(void *sprite, int x, int y, int w, int h, int rx, int ry) {
	mtSpriteAddTile(sprite, x, y, w, h, rx, ry);

	return;
}


void EXPORT_THIS d_mtsprite_frame_set(void *sprite, int time) {
	mtSpriteSetAsFrame(sprite, time);

	return;
}


void EXPORT_THIS d_mtsprite_animate_stop(void *mtsprite) {
	mtSpriteDisableAnimation(mtsprite);

	return;
}


void EXPORT_THIS d_mtsprite_animate_repeat(void *mtsprite, int repeat) {
	mtSpriteSetRepeat(mtsprite, repeat);

	return;
}


void EXPORT_THIS d_mtsprite_draw(void *mtsprite) {
	mtSpriteDraw(mtsprite);

	return;
}


void EXPORT_THIS *d_mtsprite_free(void *mtsprite) {
	return mtSpriteDelete(mtsprite);
}
