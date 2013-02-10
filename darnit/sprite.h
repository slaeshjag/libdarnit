/*
Copyright (c) 2011-2013 Steven Arnow
'sprite.h' - This file is part of libdarnit

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


#ifndef __SPRITE_H__
#define	__SPRITE_H__


#define				SPRITE_PREALLOC_DEF		64


typedef struct {
	unsigned int		time;
	unsigned int		tile;
} SPRITE_TILE;


typedef struct {
	SPRITE_TILE		tile[8];
	unsigned int		tiles;
} SPRITE_DIRECTION;


typedef struct {
	unsigned int		dir;
	unsigned int		frame;
	int			tleft;
	unsigned int		time;
	unsigned int		animate;
	unsigned int		used;
	SPRITE_DIRECTION	spr[32];
	char			tilesheet[32];
	unsigned int		wsq;
	unsigned int		hsq;
	TILESHEET		*ts;
	TILE_CACHE		cache;
	unsigned int		y;
	unsigned int		repeat;
} SPRITE_ENTRY;


typedef struct {
	SPRITE_ENTRY		*sprite;
	unsigned int		sprites;
	unsigned int		cnt;
} SPRITE;


SPRITE_ENTRY *spriteNew(TILESHEET *ts);
void *spriteLoad(const char *fname, unsigned int dir, unsigned int target_format);
void spriteTeleport(SPRITE_ENTRY *sprite, int x, int y);
void spriteEnableAnimation(SPRITE_ENTRY *sprite);
void spritePauseAnimation(SPRITE_ENTRY *sprite);
void spriteDisableAnimation(SPRITE_ENTRY *sprite);
void spriteAnimate(SPRITE_ENTRY *sprite);
void spriteSetFrame(SPRITE_ENTRY *sprite, int frame);
void spriteActivate(SPRITE_ENTRY *sprite, int dir);
void spriteSetRepeat(SPRITE_ENTRY *sprite, int repeat);
void spriteDraw(SPRITE_ENTRY *sprite);
void spriteChangeDirection(SPRITE_ENTRY *sprite, unsigned int dir);
void *spriteDelete(SPRITE_ENTRY *sprite);
void spriteSetFrameEntry(SPRITE_ENTRY *sprite, int dir, int frame, int tile, int time);


#endif
