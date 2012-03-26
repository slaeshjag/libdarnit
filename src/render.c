#ifndef HAVE_GLES
	#include "render_ogl.c"
#else
	#include "render_gles.c"	
#endif



TILESHEET *renderTilesheetLoad(void *handle, const char *fname, unsigned int wsq, unsigned int hsq) {
	DARNER *m = handle;
	TILESHEET *ts;
	IMGLOAD_DATA data;
	void *data_t;
	float twgran, thgran;
	int i;
	
	if ((ts = malloc(sizeof(TILESHEET))) == NULL) {
		MALLOC_ERROR
		return NULL;
	}
	
	data = imgloadLoad(fname);
	if (data.img_data == NULL) {
		free(ts);
		return NULL;
	}

	ts->w = data.w, ts->h = data.h, data_t = data.img_data;
	ts->texhandle = videoAddTexture(data_t, ts->w, ts->h);
	free(data_t);

	twgran = 1.0f / ts->w * wsq;
	thgran = 1.0f / ts->h * hsq;

	ts->sw = m->video.swgran * wsq;
	ts->sh = m->video.shgran * hsq;
	ts->wsq = wsq;
	ts->hsq = hsq;

	ts->tiles = (ts->w / wsq) * (ts->h / hsq);
	if ((ts->tile = malloc(ts->tiles * sizeof(TILE))) == NULL) {
		MALLOC_ERROR
		videoRemoveTexture(ts->texhandle);
		free(ts);
		return NULL;
	}

	for (i = 0; i < ts->tiles; i++) {
		ts->tile[i].r = twgran * (i % (ts->w / wsq));
		ts->tile[i].s = thgran * (i / (ts->w / wsq));
		ts->tile[i].u = ts->tile[i].r + twgran;
		ts->tile[i].v = ts->tile[i].s + thgran;
	}

	return ts;
}


void *renderTilesheetFree(TILESHEET *ts) {
	videoRemoveTexture(ts->texhandle);
	free(ts->tile);
	free(ts);

	return NULL;
}


void renderCalcTilePosCache(TILE_CACHE *cache, TILESHEET *ts, float x, float y) {
	x /= ts->wsq;
	y /= ts->hsq;
	cache->x = (x*ts->sw) - 1.0f;
	cache->y = 1.0f-(y*ts->sh);
	cache->x2 = (ts->sw + x*ts->sw) - 1.0f;
	cache->y2 = 1.0f-(y*ts->sh);
	cache->x3 = (ts->sw + x*ts->sw-1.0f);
	cache->y3 = (1.0f-y*ts->sh - ts->sh);
	cache->x4 = (ts->sw + x*ts->sw-1.0f);
	cache->y4 = (1.0f-y*ts->sh - ts->sh);
	cache->x5 = (x*ts->sw-1.0f);
	cache->y5 = (1.0f-y*ts->sh - ts->sh);
	cache->x6 = (x*ts->sw-1.0f);
	cache->y6 = (1.0f-y*ts->sh);

	return;
}


void renderCalcTileCache(TILE_CACHE *cache, TILESHEET *ts, unsigned int tile) {
	unsigned int t = tile;

	cache->u = ts->tile[t].r;
	cache->v = ts->tile[t].s;
	cache->u2 = ts->tile[t].u;
	cache->v2 = ts->tile[t].s;
	cache->u3 = ts->tile[t].u;
	cache->v3 = ts->tile[t].v;
	cache->u4 = ts->tile[t].u;
	cache->v4 = ts->tile[t].v;
	cache->u5 = ts->tile[t].r;
	cache->v5 = ts->tile[t].v;
	cache->u6 = ts->tile[t].r;
	cache->v6 = ts->tile[t].s;

	return;
}



void renderCache(TILE_CACHE *cache, TILESHEET *ts, int tiles) {
	glBindTexture(GL_TEXTURE_2D, ts->texhandle);

	glVertexPointer(2, GL_FLOAT, 16, cache);
	glTexCoordPointer(2, GL_FLOAT, 16, &cache->u);

	glDrawArrays(GL_TRIANGLES, 0, tiles*6);

	return;
}
