/*
Copyright (c) 2011-2013 Steven Arnow
'tilemap.c' - This file is part of libdarnit

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


TILEMAP_ENTRY *tilemapNew(int invs_div, void *tilesheet, unsigned int mask, int w, int h) {
	TILEMAP_ENTRY *tilemap;
	int i;

	if ((tilemap = malloc(sizeof(TILEMAP_ENTRY))) == NULL)
		return NULL;
	if ((tilemap->data = malloc(sizeof(unsigned int) * w * h)) == NULL) {
		free(tilemap);
		return NULL;
	}
	
	for (i = 0; i < w * h; i++)
		tilemap->data[i] = 0;

	#ifndef DARNIT_HEADLESS
	if ((tilemap->render = renderTilemapCreate(w, h, tilemap->data, 0, 0, invs_div, tilesheet, mask)) == NULL) {
		free(tilemap->data);
		free(tilemap);
		return NULL;
	}
	#endif

	tilemap->w = w;
	tilemap->h = h;

	return tilemap;
}


TILEMAP_ENTRY *tilemapLoad(const char *fname, int invs_div, void *tilesheet, unsigned int mask) {
	IMGLOAD_DATA data;
	TILEMAP_ENTRY *tilemap;
	int i;
	unsigned int tmp;

	if ((tilemap = malloc(sizeof(TILEMAP_ENTRY))) == NULL)
		return NULL;

	data = imgloadLoad(fname);
	tilemap->w = data.w;
	tilemap->h = data.h;
	tilemap->data = data.img_data;

	if (tilemap->data == NULL) {
		free(tilemap);
		return NULL;
	}

	for (i = 0; i < tilemap->w * tilemap->h; i++) {
		tmp = (tilemap->data[i] & 0xF0) >> 4;
		tmp |= (tilemap->data[i] & 0xF000) >> 8;
		tmp |= (tilemap->data[i] & 0xF00000) >> 12;
		tmp |= (tilemap->data[i] & 0xF0000000) >> 16;
		tilemap->data[i] = tmp;
	}

	#ifndef DARNIT_HEADLESS
	tilemap->render = renderTilemapCreate(tilemap->w, tilemap->h, tilemap->data, 0, 0, invs_div, tilesheet, mask);
	#endif

	return tilemap;
}



void *tilemapFree(TILEMAP_ENTRY *tm) {
	if (tm == NULL) return NULL;
	free(tm->data);

	#ifndef DARNIT_HEADLESS
	renderTilemapFree(tm->render);
	#endif

	free(tm);

	return NULL;
}
