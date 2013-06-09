/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_sprite.h' - This file is part of libdarnit

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


#ifndef	__DARNIT_SPRITE_H__
#define	__DARNIT_SPRITE_H__


typedef enum {
	DARNIT_SPRITE_ANIMATION_NOREPEAT = 0,
	DARNIT_SPRITE_ANIMATION_REPEAT = 1,
} DARNIT_SPRITE_ANIMATION;

typedef void DARNIT_SPRITE;


DARNIT_SPRITE *d_sprite_new(DARNIT_TILESHEET *tilesheet);
DARNIT_SPRITE *d_sprite_load(const char *fname, int dir, DARNIT_PFORMAT target_format);
DARNIT_SPRITE *d_sprite_free(DARNIT_SPRITE *sprite);
void d_sprite_direction_set(DARNIT_SPRITE *sprite, int dir);
void d_sprite_activate(DARNIT_SPRITE *sprite, int dir);
void d_sprite_frame_set(DARNIT_SPRITE *sprite, int frame);
void d_sprite_move(DARNIT_SPRITE *sprite, int x, int y);
void d_sprite_rotate(DARNIT_SPRITE *sprite, int angle);
void d_sprite_animate_start(DARNIT_SPRITE *sprite);
void d_sprite_animate_pause(DARNIT_SPRITE *sprite);
void d_sprite_animate_stop(DARNIT_SPRITE *sprite);
void d_sprite_animate_repeat(DARNIT_SPRITE *sprite, DARNIT_SPRITE_ANIMATION repeat);
void d_sprite_draw(DARNIT_SPRITE *sprite);
void d_sprite_frame_entry(DARNIT_SPRITE *sprite, int dir, int frame, int tile, int time);

int d_sprite_width(DARNIT_SPRITE *sprite);
int d_sprite_height(DARNIT_SPRITE *sprite);
int d_sprite_sprite(DARNIT_SPRITE *sprite);

#endif
