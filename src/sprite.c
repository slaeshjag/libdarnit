#include "darnit.h"



void *spriteLoad(void *handle, const char *fname, unsigned int dir) {
	DARNIT *m = handle;
	FILE *fp;
	int tile;
	SPRITE_ENTRY *sprite_e;

	if ((fp = fopen(fname, "r")) == NULL) {
		FILE_OPEN_ERROR
		return NULL;
	}

	if ((sprite_e = malloc(sizeof(SPRITE_ENTRY))) == NULL) {
		fclose(fp);
		return NULL;
	}

	fread(sprite_e, 1, sizeof(SPRITE_ENTRY), fp);
	fclose(fp);
	sprite_e->time = SDL_GetTicks();

	sprite_e->ts = renderTilesheetLoad(m, sprite_e->tilesheet, sprite_e->wsq, sprite_e->hsq);

	sprite_e->dir = dir;
	sprite_e->frame = 0;
	tile = sprite_e->spr[dir].tile[sprite_e->frame].tile;
	renderCalcTileCache(&sprite_e->cache, sprite_e->ts, tile);

	return sprite_e;
}



void spriteTeleport(SPRITE_ENTRY *sprite, unsigned int x, unsigned int y) {
	int tile, dir;

	dir = sprite->dir;
	tile = sprite->spr[dir].tile[sprite->frame].tile;

	renderCalcTilePosCache(&sprite->cache, sprite->ts, x, y);

	return;
}


void spriteEnableAnimation(SPRITE_ENTRY *sprite) {
	int dir;

	if (sprite->animate == 1)
		return;

	dir = sprite->dir;

	sprite->animate = 1;
	sprite->time = SDL_GetTicks();
	sprite->tleft = sprite->spr[dir].tile[sprite->frame].time;

	return;
}


void spritePauseAnimation(SPRITE_ENTRY *sprite) {
	sprite->animate = 0;

	return;
}


void spriteDisableAnimation(SPRITE_ENTRY *sprite) {

	sprite->frame = 0;
	sprite->time = SDL_GetTicks();
	spriteAnimate(sprite);
	sprite->animate = 0;

	return;
}


void spriteAnimate(SPRITE_ENTRY *sprite) {
	unsigned int time, dir, tile;

	time = SDL_GetTicks();

	if (sprite->used == 0 || sprite->animate == 0)
		return;

	sprite->tleft -= (time - sprite->time);
	sprite->time = time;
	dir = sprite->dir;

	while (sprite->tleft < 0) {
		sprite->frame++;
		if (sprite->frame >= sprite->spr[dir].tiles)
			sprite->frame = 0;
		sprite->tleft += sprite->spr[dir].tile[sprite->frame].time;
	}
	
	tile = sprite->spr[dir].tile[sprite->frame].tile;
	renderCalcTileCache(&sprite->cache, sprite->ts, tile);
	
	return;
}


void spriteDraw(SPRITE_ENTRY *sprite) {

	spriteAnimate(sprite);
	renderCache(&sprite->cache, sprite->ts, 1);
	
	return;
}


void spriteChangeDirection(SPRITE_ENTRY *sprite, unsigned int dir) {
	int tile;

	sprite->dir = dir;

	tile = sprite->spr[dir].tile[sprite->frame].tile;
	renderCalcTileCache(&sprite->cache, sprite->ts, tile);
	
	return;
}


void *spriteDelete(SPRITE_ENTRY *sprite) {

	sprite->ts = renderTilesheetFree(sprite->ts);
	free(sprite);

	return NULL;
}

