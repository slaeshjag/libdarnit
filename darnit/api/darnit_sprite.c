/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_sprite.c' - This file is part of libdarnit

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


void EXPORT_THIS *d_sprite_new(void *tilesheet) {
	return spriteNew(tilesheet);
}


void EXPORT_THIS *d_sprite_load(const char *fname, int dir, unsigned int target_format) {
	return spriteLoad(fname, dir, target_format);
}


void EXPORT_THIS *d_sprite_free(void *sprite) {
	return spriteDelete(sprite);
}


void EXPORT_THIS d_sprite_direction_set(void *sprite, int dir) {
	spriteChangeDirection(sprite, dir);

	return;
}


void EXPORT_THIS d_sprite_activate(void *sprite, int dir) {
	spriteActivate(sprite, dir);

	return;
}


void EXPORT_THIS d_sprite_frame_set(void *sprite, int frame) {
	spriteSetFrame(sprite, frame);

	return;
}


void EXPORT_THIS d_sprite_move(void *sprite, int x, int y) {
	spriteTeleport(sprite, x, y);

	return;
}


void EXPORT_THIS d_sprite_animate_start(void *sprite) {
	spriteEnableAnimation(sprite);

	return;
}


void EXPORT_THIS d_sprite_animate_pause(void *sprite) {
	spritePauseAnimation(sprite);

	return;
}


void EXPORT_THIS d_sprite_animate_stop(void *sprite) {
	spriteDisableAnimation(sprite);

	return;
}


void EXPORT_THIS d_sprite_animate_repeat(void *sprite, int repeat) {
	spriteSetRepeat(sprite, repeat);

	return;
}


void EXPORT_THIS d_sprite_draw(void *sprite) {
	spriteDraw(sprite);

	return;
}


void EXPORT_THIS d_sprite_frame_entry_set(void *sprite, int dir, int frame, int tile, int time) {
	spriteSetFrameEntry(sprite, dir, frame, tile, time);

	return;
}


int EXPORT_THIS d_sprite_width(void *sprite) {
	SPRITE_ENTRY *sprite_e = sprite;
	if (sprite == NULL)
		return 0;
	return sprite_e->ts->wsq;
}



int EXPORT_THIS d_sprite_height(void *sprite) {
	SPRITE_ENTRY *sprite_e = sprite;
	if (sprite == NULL)
		return 0;
	return sprite_e->ts->hsq;
}
