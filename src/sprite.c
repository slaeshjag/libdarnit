#include "darnit.h"


void *spriteBoxNew(unsigned int prealloc) {
	int i;
	SPRITE *sprite;

	if ((sprite = malloc(sizeof(SPRITE))) == NULL) 
		return NULL;

	sprite->sprites = prealloc;
	sprite->cnt = 0;
	if ((sprite->sprite = malloc(sizeof(SPRITE_ENTRY) * prealloc)) == NULL) {
		MALLOC_ERROR
		return NULL;
	}

	for (i = 0; i < prealloc; i++) {
		sprite->sprite[i].used = 0;
		sprite->sprite[i].animate = 0;
	}

	return sprite;
}


int spriteGetOne(SPRITE *sprite) {
	int i;

	for (i = 0; i < sprite->sprites; i++)
		if (sprite->sprite[i].used == 0)
			return i;
	
	// TODO: Implement dynamic resizing of the sprite array //
	fprintf(stderr, "STUB: Unhandled situation: preallocated sprite buffer is too small. No idea what to do D:\n");

	return -1;
}


int spriteGetIndex(SPRITE *sprite, int id) {
	int i;

	for (i = 0; i < sprite->sprites; i++)
		if (sprite->sprite[i].id == id && sprite->sprite[i].used == 1)
			return i;
	
	fprintf(stderr, "Error: Unable to get index for sprite id #%i, it does not exist!\n", id);

	return -1;
}


int spriteLoad(void *handle, SPRITE *sprite, const char *fname, unsigned int dir) {
	DARNIT *m = handle;
	FILE *fp;
	int spr, tile;
	SPRITE_ENTRY *sprite_e;

	if ((spr = spriteGetOne(sprite)) < 0) {
		SPRITE_ERROR
		return -1;
	}

	if ((fp = fopen(fname, "r")) == NULL) {
		FILE_OPEN_ERROR
		return -1;
	}

	sprite_e = &sprite->sprite[spr];

	fread(&sprite->sprite[spr], 1, sizeof(SPRITE_ENTRY), fp);
	fclose(fp);
	sprite->sprite[spr].time = SDL_GetTicks();

	sprite_e->ts = renderTilesheetLoad(m, sprite_e->tilesheet, sprite_e->wsq, sprite_e->hsq);

	sprite->sprite[spr].dir = dir;
	tile = sprite_e->spr[dir].tile[sprite_e->frame].tile;
	renderCalcTileCache(&sprite_e->cache, sprite_e->ts, tile);
	sprite->sprite[spr].id = spr;
	sprite->cnt++;

	return spr;
}



void spriteTeleport(SPRITE *sprite, unsigned int key, unsigned int x, unsigned int y, unsigned int l) {
	int tile, dir, index;

	if ((index = spriteGetIndex(sprite, key)) == -1)
		return;

	dir = sprite->sprite[index].dir;
	tile = sprite->sprite[index].spr[dir].tile[sprite->sprite[index].frame].tile;

	sprite->sprite[index].layer = l;
	sprite->sprite[index].y = y;
	renderCalcTilePosCache(&sprite->sprite[index].cache, sprite->sprite[index].ts, x, y);

	return;
}


void spriteEnableAnimation(SPRITE *sprite, unsigned int key) {
	int dir, index;

	if ((index = spriteGetIndex(sprite, key)) == -1)
		return;

	if (sprite->sprite[index].animate == 1)
		return;

	dir = sprite->sprite[index].dir;


	sprite->sprite[index].animate = 1;
	sprite->sprite[index].time = SDL_GetTicks();
	sprite->sprite[index].tleft = sprite->sprite[index].spr[dir].tile[sprite->sprite[index].frame].time;

	return;
}


void spriteDisableAnimation(SPRITE *sprite, unsigned int key) {
	int index;

	if ((index = spriteGetIndex(sprite, key)) == -1)
		return;

	sprite->sprite[index].animate = 0;
	sprite->sprite[index].frame = 0;

	return;
}


void spriteAnimate(SPRITE *sprite) {
	int i;
	unsigned int time, dir, tile;

	time = SDL_GetTicks();

	for (i = 0; i < sprite->sprites; i++) {
		if (sprite->sprite[i].used == 0 || sprite->sprite[i].animate == 0)
			continue;

		sprite->sprite[i].tleft -= (time - sprite->sprite[i].time);
		sprite->sprite[i].time = time;
		dir = sprite->sprite[i].dir;


		while (sprite->sprite[i].tleft < 0) {
			sprite->sprite[i].frame++;
			if (sprite->sprite[i].frame >= sprite->sprite[i].spr[dir].tiles)
				sprite->sprite[i].frame = 0;
			sprite->sprite[i].tleft += sprite->sprite[i].spr[dir].tile[sprite->sprite[i].frame].time;
		}

		tile = sprite->sprite[i].spr[dir].tile[sprite->sprite[i].frame].tile;
		renderCalcTileCache(&sprite->sprite[i].cache, sprite->sprite[i].ts, tile);

	}

	return;
}


void spriteDrawLayer(SPRITE *sprite, unsigned int layer) {
	int i;


	for (i = 0; i < sprite->sprites; i++)
		if (sprite->sprite[i].layer == layer && sprite->sprite[i].used == 1)
			renderCache(&sprite->sprite[i].cache, sprite->sprite[i].ts, 1);
	
	return;
}


void spriteSort(SPRITE *sprite) {
	SPRITE_ENTRY tmp;
	int i, j;
	
	// loopen börjar på 1 för att första elementet är definitivt sorterat när det står ensamt.
	// därefter hämtas ett element i taget ur "buffern" (resten av arrayen)
	for (i = 1; i < sprite->sprites; i++) {
		// så länge nuvarande bufferelement (j) är mindre än det precis till vänster i den sorterade listan
		// ska det skiftas ett steg till vänster.
		for (j = i; j > 0 && sprite->sprite[j].y < sprite->sprite[j-1].y; j--) {
			tmp = sprite->sprite[j];
			sprite->sprite[j] = sprite->sprite[j-1];
			sprite->sprite[j-1] = tmp;
		}
	}
	
	return;
}


void spriteChangeDirection(SPRITE *sprite, unsigned int key, unsigned int dir) {
	int id, tile;

	if ((id = spriteGetIndex(sprite, key)) == -1)
		return;
	
	sprite->sprite[id].dir = dir;

	tile = sprite->sprite[id].spr[dir].tile[sprite->sprite[id].frame].tile;
	renderCalcTileCache(&sprite->sprite[id].cache, sprite->sprite[id].ts, tile);
	
	return;
}


void spriteDelete(SPRITE *sprite, unsigned int key) {
	int id;

	if ((id = spriteGetIndex(sprite, key)) == -1)
		return;
	
	sprite->sprite[id].used = 0;
	sprite->sprite[id].ts = renderTilesheetFree(sprite->sprite[id].ts);

	return;
}


void spriteClear(SPRITE *sprite) {
	int i;

	for (i = 0; i < sprite->sprites; i++) {
		sprite->sprite[i].ts = renderTilesheetFree(sprite->sprite[i].ts);
		sprite->sprite[i].used = 0;
	}
	

	return;
}


void *spriteBoxFree(SPRITE *sprite) {
	spriteClear(sprite);
	free(sprite->sprite);
	free(sprite);

	return NULL;
}
