#include "darnit.h"


TILEMAP_ENTRY *tilemapLoad(void *handle, const char *fname, int invs_div, void *tilesheet) {
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
		tilemap->data[i] = tmp;
	}

	tilemap->render = renderTilemapCreate(handle, tilemap->w, tilemap->h, tilemap->data, 0, 0, invs_div, tilesheet);

	return tilemap;
}



void *tilemapFree(TILEMAP_ENTRY *tm) {
	if (tm == NULL) return NULL;
	free(tm->data);
	renderTilemapFree(tm->render);
	free(tm);

	return NULL;
}

