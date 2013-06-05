/*
Copyright (c) 2011-2013 Steven Arnow
'sprite.c' - This file is part of libdarnit

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

SPRITE_ENTRY *spriteNew(TILESHEET *ts) {
	unsigned int i, j;
	SPRITE_ENTRY *se;

	if ((se = malloc(sizeof(SPRITE_ENTRY))) == NULL)
		return NULL;
	
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			se->spr[i].tile[j].time = 50;
			se->spr[i].tile[j].tile = 0;
		}
		se->spr[i].tiles = 0;
	}

	se->dir = 0;
	se->frame = 0;
	se->time = 0;
	se->tleft = 0;
	se->animate = 0;
	se->repeat = 1;
	se->used = 1;
	se->y = 0;
	se->angle = 0;
	if (!ts)
		se->wsq = se->hsq = 0;
	else {
		se->wsq = ts->wsq;
		se->hsq = ts->hsq;
	}

	se->ts = ts;
	*se->tilesheet = 0;

	return se;
}



void spriteLoadText(FILESYSTEM_FILE *fp, SPRITE_ENTRY *se) {
	unsigned int i, j;
	char c, buf[512];

	fsFileSeek(fp, 0, SEEK_SET);
	fsFileGets(buf, 512, fp);
	sscanf(buf, "%s %i %i\n", se->tilesheet, &se->wsq, &se->hsq);

	j = i = 0;

	while (!fsFileEOF(fp)) {
		fsFileRead(&c, 1, fp);
		switch (c) {
			case 'D':
				fsFileGets(buf, 512, fp);
				j = 0;
				break;
			case 'T':
				fsFileGets(buf, 512, fp);
				sscanf(buf, "%i %i\n", &se->spr[i].tile[j].time, &se->spr[i].tile[j].tile);
				j++;
				break;
			case 'E':
				se->spr[i].tiles = j;
				j = 0;
				i++;
				break;
			case '\n':
				break;
			default:
				fsFileGets(buf, 512, fp);
				break;
		}
	}



	return;
}


void spriteSetFrame(SPRITE_ENTRY *sprite, int frame) {
	if (sprite == NULL)
		return;
	sprite->frame = frame;
	spriteActivate(sprite, sprite->dir);

	return;
}


void spriteSetFrameEntry(SPRITE_ENTRY *sprite, int dir, int frame, int tile, int time) {
	if (sprite == NULL)
		return;

	if (tile >= 8 || time < 0 || tile < 0 || dir < 0 || dir >= 32)
		return;
	if (tile >= sprite->spr[dir].tiles)
		sprite->spr[dir].tiles = tile + 1;
	sprite->spr[dir].tile[tile].tile = tile;
	sprite->spr[dir].tile[tile].time = time;

	return;
}


#ifndef DARNIT_HEADLESS
void spriteActivate(SPRITE_ENTRY *sprite, int dir) {
	if (sprite == NULL)
		return;

	if (dir < 0 || dir >= 32)
		dir = 0;
	if (sprite->spr[dir].tiles < sprite->frame)
		sprite->frame = 0;
	sprite->time = tpw_ticks();
	sprite->dir = dir;
	sprite->repeat = 1;
	renderCalcTileCache(&sprite->cache, sprite->ts, sprite->spr[dir].tile[sprite->frame].tile);

	return;
}
#endif


void spriteSetRepeat(SPRITE_ENTRY *sprite, int repeat) {
	if (!sprite)
		return;
	sprite->repeat = (repeat) ? 1 : 0;

	return;
}


void *spriteLoad(const char *fname, unsigned int dir, unsigned int target_format) {
	FILESYSTEM_FILE *fp;
	unsigned int header;
	SPRITE_ENTRY *sprite_e;

	if ((fp = fsFileOpen(fname, "rb")) == NULL) {
		FILE_OPEN_ERROR
		return NULL;
	}

	fsFileRead(&header, 4, fp);

	if ((sprite_e = spriteNew(NULL)) == NULL) {
		fsFileClose(fp);
		return NULL;
	}
	
	if (header != 0x00FF10EF)
		spriteLoadText(fp, sprite_e);
	else 
		fsFileRead(sprite_e, sizeof(SPRITE_ENTRY), fp);

	fsFileClose(fp);
	#ifndef DARNIT_HEADLESS
	if ((sprite_e->ts = renderTilesheetLoad(sprite_e->tilesheet, sprite_e->wsq, sprite_e->hsq, target_format)) == NULL) {
		free(sprite_e);
		return NULL;
	}
	#endif

	#ifndef DARNIT_HEADLESS
	spriteActivate(sprite_e, dir);
	#endif

	return sprite_e;
}



void spriteTeleport(SPRITE_ENTRY *sprite, int x, int y) {
	if (sprite == NULL) return;

	sprite->y = y;
	sprite->x = x;
	#ifndef DARNIT_HEADLESS
	renderCalcTilePosCache(&sprite->cache, sprite->ts, x, y, sprite->angle);
	#endif

	return;
}


#ifndef DARNIT_HEADLESS
void spriteEnableAnimation(SPRITE_ENTRY *sprite) {
	if (sprite == NULL) return;
	int dir;

	if (sprite->animate == 1)
		return;

	dir = sprite->dir;

	sprite->animate = 1;
	sprite->time = tpw_ticks();
	sprite->tleft = sprite->spr[dir].tile[sprite->frame].time;

	return;
}


void spritePauseAnimation(SPRITE_ENTRY *sprite) {
	if (sprite == NULL) return;
	sprite->animate = 0;

	return;
}


void spriteDisableAnimation(SPRITE_ENTRY *sprite) {
	if (sprite == NULL) return;

	sprite->frame = 0;
	sprite->time = tpw_ticks();
	spriteAnimate(sprite);
	sprite->animate = 0;

	return;
}


void spriteAnimate(SPRITE_ENTRY *sprite) {
	if (sprite == NULL) return;
	unsigned int time, dir, tile, start;

	time = tpw_ticks();

	if (sprite->used == 0 || sprite->animate == 0)
		return;

	sprite->tleft -= (time - sprite->time);
	sprite->time = time;
	dir = sprite->dir;
	start = sprite->frame;

	while (sprite->tleft < 0) {
		sprite->frame++;
		if (sprite->frame == start)
			break;
		if (sprite->frame >= sprite->spr[dir].tiles)
			sprite->frame = (sprite->repeat) ? 0 : sprite->frame - 1;
		sprite->tleft += sprite->spr[dir].tile[sprite->frame].time;
	}
	
	tile = sprite->spr[dir].tile[sprite->frame].tile;
	renderCalcTileCache(&sprite->cache, sprite->ts, tile);
	
	return;
}


void spriteDraw(SPRITE_ENTRY *sprite) {
	if (sprite == NULL) return;

	spriteAnimate(sprite);
	renderCache(&sprite->cache, sprite->ts, 1);
	
	return;
}


void spriteChangeDirection(SPRITE_ENTRY *sprite, unsigned int dir) {
	if (sprite == NULL) return;
	int tile;

	sprite->dir = dir;

	tile = sprite->spr[dir].tile[sprite->frame].tile;
	renderCalcTileCache(&sprite->cache, sprite->ts, tile);
	
	return;
}

// DARNIT_HEADLESS
#endif

void *spriteDelete(SPRITE_ENTRY *sprite) {
	if (sprite == NULL) return NULL;

	#ifndef DARNIT_HEADLESS
	if (*sprite->tilesheet != 0)
		sprite->ts = renderTilesheetFree(sprite->ts);
	#endif
	free(sprite);

	return NULL;
}

