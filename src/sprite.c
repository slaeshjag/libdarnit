#include "darnit.h"


void spriteLoadText(void *handle, FILE *fp, SPRITE_ENTRY *se) {
	unsigned int i, j;
	char c, buf[512];

	rewind(fp);
	fscanf(fp, "%s %i %i\n", se->tilesheet, &se->wsq, &se->hsq);

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
	se->used = 1;

	j = i = 0;

	while (!feof(fp)) {
		c = fgetc(fp);
		switch (c) {
			case 'D':
				fgets(buf, 512, fp);
				j = 0;
				break;
			case 'T':
				fscanf(fp, "%i %i\n", &se->spr[i].tile[j].time, &se->spr[i].tile[j].tile);
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
				fgets(buf, 512, fp);
				break;
		}
	}

	return;
}



void *spriteLoad(void *handle, const char *fname, unsigned int dir, unsigned int target_format) {
	DARNIT *m = handle;
	FILE *fp;
	int tile;
	unsigned int header;
	SPRITE_ENTRY *sprite_e;

	if ((fp = fopen(fname, "r")) == NULL) {
		FILE_OPEN_ERROR
		return NULL;
	}

	fread(&header, 4, 1, fp);

	if ((sprite_e = malloc(sizeof(SPRITE_ENTRY))) == NULL) {
		fclose(fp);
		return NULL;
	}
	
	if (header != 0x00FF10EF)
		spriteLoadText(m, fp, sprite_e);
	else 
		fread(sprite_e, 1, sizeof(SPRITE_ENTRY), fp);

	fclose(fp);
	sprite_e->time = SDL_GetTicks();
	sprite_e->ts = renderTilesheetLoad(m, sprite_e->tilesheet, sprite_e->wsq, sprite_e->hsq, target_format);

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

