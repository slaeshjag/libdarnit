/*
Copyright (c) 2011-2013 Steven Arnow
'mt_sprite.h' - This file is part of libdarnit

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


#ifndef __MT_SPRITE_H__
#define	__MT_SPRITE_H__


typedef struct {
	PARTICLE		**particle_b;
	int			particles_b;
	PARTICLE		**particle_t;
	int			particles_t;
} MTSPRITE_PARTICLE;


typedef struct {
	TILE_CACHE		*cache;
	int			tiles;
	int			time;
	PARTICLE		**pulse;
	int			pulses;
} MTSPRITE_FRAME;


typedef struct {
	char			tilesheet[64];
	TILESHEET		*ts;
	TILE_CACHE		*cache;
	MTSPRITE_FRAME		*frame;
	int			frames;
	int			tiles;
	int			cur_frame;
	int			time_left;
	int			time_last;
	int			animate;
	unsigned int		repeat;
	MTSPRITE_PARTICLE	p;
} MTSPRITE_ENTRY;


void *mtSpriteNew(int tiles, int frames, void *ts);
void *mtSpriteLoad(const char *fname);
void mtSpriteDraw(MTSPRITE_ENTRY *spr);
void mtSpriteEnableAnimation(MTSPRITE_ENTRY *spr);
void mtSpritePauseAnimation(MTSPRITE_ENTRY *spr);
void mtSpriteSetAsFrame(MTSPRITE_ENTRY *spr, int time);
void mtSpriteAddTile(MTSPRITE_ENTRY *spr, int x, int y, int w, int h, int rx, int ry);
PARTICLE *mtSpriteLoadParticle(MTSPRITE_ENTRY *spr, int type, int max_particles, int top);
void mtSpriteDisableAnimation(MTSPRITE_ENTRY *spr);
void mtSpriteSetRepeat(MTSPRITE_ENTRY *spr, int repeat);
void *mtSpriteDelete(MTSPRITE_ENTRY *spr);


#endif
