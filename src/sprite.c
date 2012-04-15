#include "darnit.h"


int spriteInit(void *handle, unsigned int prealloc) {
	DARNIT *m = handle;
	int i;

	m->sprite.sprites = prealloc;
	m->sprite.cnt = 0;
	if ((m->sprite.sprite = malloc(sizeof(SPRITE_ENTRY) * prealloc)) == NULL) {
		MALLOC_ERROR
		return -1;
	}

	for (i = 0; i < prealloc; i++) {
		m->sprite.sprite[i].used = 0;
		m->sprite.sprite[i].animate = 0;
	}

	return 0;
}


int spriteGetOne(void *handle) {
	DARNIT *m = handle;
	int i;

	for (i = 0; i < m->sprite.sprites; i++)
		if (m->sprite.sprite[i].used == 0)
			return i;
	
	// TODO: Implement dynamic resizing of the sprite array //
	fprintf(stderr, "STUB: Unhandled situation: preallocated sprite buffer is too small. No idea what to do D:\n");

	return -1;
}


int spriteGetIndex(void *handle, int id) {
	int i;
	DARNIT *m = handle;

	for (i = 0; i < m->sprite.sprites; i++)
		if (m->sprite.sprite[i].id == id && m->sprite.sprite[i].used == 1)
			return i;
	
	fprintf(stderr, "Error: Unable to get index for sprite id #%i, it does not exist!\n", id);

	return -1;
}


int spriteLoad(void *handle, const char *fname, unsigned int dir) {
	DARNIT *m = handle;
	FILE *fp;
	int spr, tile;
	SPRITE_ENTRY *sprite;

	if ((spr = spriteGetOne(m)) < 0) {
		SPRITE_ERROR
		return -1;
	}

	if ((fp = fopen(fname, "r")) == NULL) {
		FILE_OPEN_ERROR
		return -1;
	}

	sprite = &m->sprite.sprite[spr];

	fread(&m->sprite.sprite[spr], 1, sizeof(SPRITE_ENTRY), fp);
	fclose(fp);
	m->sprite.sprite[spr].time = SDL_GetTicks();

	sprite->ts = renderTilesheetLoad(m, sprite->tilesheet, sprite->wsq, sprite->hsq);

	m->sprite.sprite[spr].dir = dir;
	tile = sprite->spr[dir].tile[sprite->frame].tile;
	renderCalcTileCache(&sprite->cache, sprite->ts, tile);
	m->sprite.sprite[spr].id = spr;
	m->sprite.cnt++;

	return spr;
}



void spriteTeleport(void *handle, unsigned int sprite, unsigned int x, unsigned int y, unsigned int l) {
	DARNIT *m = handle;
	int tile, dir, index;

	if ((index = spriteGetIndex(m, sprite)) == -1)
		return;

	dir = m->sprite.sprite[index].dir;
	tile = m->sprite.sprite[index].spr[dir].tile[m->sprite.sprite[index].frame].tile;

	m->sprite.sprite[index].layer = l;
	m->sprite.sprite[index].y = y;
	renderCalcTilePosCache(&m->sprite.sprite[index].cache, m->sprite.sprite[index].ts, x, y);

	return;
}


void spriteEnableAnimation(void *handle, unsigned int sprite) {
	DARNIT *m = handle;
	int dir, index;

	if ((index = spriteGetIndex(m, sprite)) == -1)
		return;

	if (m->sprite.sprite[index].animate == 1)
		return;

	dir = m->sprite.sprite[index].dir;


	m->sprite.sprite[index].animate = 1;
	m->sprite.sprite[index].time = SDL_GetTicks();
	m->sprite.sprite[index].tleft = m->sprite.sprite[index].spr[dir].tile[m->sprite.sprite[index].frame].time;

	return;
}


void spriteDisableAnimation(void *handle, unsigned int sprite) {
	DARNIT *m = handle;
	int index;

	if ((index = spriteGetIndex(m, sprite)) == -1)
		return;

	m->sprite.sprite[index].animate = 0;
	m->sprite.sprite[index].frame = 0;

	return;
}


void spriteAnimate(void *handle) {
	DARNIT *m = handle;
	int i;
	unsigned int time, dir, tile;

	time = SDL_GetTicks();

	for (i = 0; i < m->sprite.sprites; i++) {
		if (m->sprite.sprite[i].used == 0 || m->sprite.sprite[i].animate == 0)
			continue;

		m->sprite.sprite[i].tleft -= (time - m->sprite.sprite[i].time);
		m->sprite.sprite[i].time = time;
		dir = m->sprite.sprite[i].dir;


		while (m->sprite.sprite[i].tleft < 0) {
			m->sprite.sprite[i].frame++;
			if (m->sprite.sprite[i].frame >= m->sprite.sprite[i].spr[dir].tiles)
				m->sprite.sprite[i].frame = 0;
			m->sprite.sprite[i].tleft += m->sprite.sprite[i].spr[dir].tile[m->sprite.sprite[i].frame].time;
		}

		tile = m->sprite.sprite[i].spr[dir].tile[m->sprite.sprite[i].frame].tile;
		renderCalcTileCache(&m->sprite.sprite[i].cache, m->sprite.sprite[i].ts, tile);

	}

	return;
}


void spriteDrawLayer(void *handle, unsigned int layer) {
	DARNIT *m = handle;
	int i;


	for (i = 0; i < m->sprite.sprites; i++)
		if (m->sprite.sprite[i].layer == layer && m->sprite.sprite[i].used == 1)
			renderCache(&m->sprite.sprite[i].cache, m->sprite.sprite[i].ts, 1);
	
	return;
}


void spriteSort(void *handle) {
	DARNIT *m = handle;
	SPRITE_ENTRY tmp;
	int i, j;
	
	// loopen börjar på 1 för att första elementet är definitivt sorterat när det står ensamt.
	// därefter hämtas ett element i taget ur "buffern" (resten av arrayen)
	for (i = 1; i < m->sprite.sprites; i++) {
		// så länge nuvarande bufferelement (j) är mindre än det precis till vänster i den sorterade listan
		// ska det skiftas ett steg till vänster.
		for (j = i; j > 0 && m->sprite.sprite[j].y < m->sprite.sprite[j-1].y; j--) {
			tmp = m->sprite.sprite[j];
			m->sprite.sprite[j] = m->sprite.sprite[j-1];
			m->sprite.sprite[j-1] = tmp;
		}
	}
	
	return;
}


void spriteChangeDirection(void *handle, unsigned int sprite, unsigned int dir) {
	DARNIT *m = handle;
	int id, tile;

	if ((id = spriteGetIndex(m, sprite)) == -1)
		return;
	
	m->sprite.sprite[id].dir = dir;

	tile = m->sprite.sprite[id].spr[dir].tile[m->sprite.sprite[id].frame].tile;
	renderCalcTileCache(&m->sprite.sprite[id].cache, m->sprite.sprite[id].ts, tile);
	
	return;
}


void spriteDelete(void *handle, unsigned int sprite) {
	DARNIT *m = handle;
	int id;

	if ((id = spriteGetIndex(m, sprite)) == -1)
		return;
	
	m->sprite.sprite[id].used = 0;
	m->sprite.sprite[id].ts = renderTilesheetFree(m->sprite.sprite[id].ts);

	return;
}


void spriteClear(void *handle) {
	DARNIT *m = handle;
	int i;

	for (i = 0; i < m->sprite.sprites; i++) {
		m->sprite.sprite[i].ts = renderTilesheetFree(m->sprite.sprite[i].ts);
		m->sprite.sprite[i].used = 0;
	}
	

	return;
}

