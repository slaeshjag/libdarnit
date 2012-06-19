#ifndef HAVE_GLES
	#include "render_ogl.c"
#else
	#include "render_gles.c"	
#endif


int renderInit(void *handle) {
	DARNIT *m = handle;

	m->tsr.cnt = 0;
	m->tsr.tsr = NULL;

	return 0;
}


int renderAddTSRef(void *handle, const char *fname, TILESHEET *ts) {
	DARNIT *m = handle;
	void *tmp;
	int i, len, fast_cmp;

	if (fname == NULL) return -1;
	len = strlen(fname);

	for (i = fast_cmp = 0; i < len; i++)
		fast_cmp += fname[i];
	
	for (i = 0; i < m->tsr.cnt; i++)
		if (m->tsr.tsr[i].fast_cmp == 0)
			break;
	
	if (i == m->tsr.cnt) {
		m->tsr.cnt++;
		if ((tmp = realloc(m->tsr.tsr, sizeof(TILESHEET_REF) * m->tsr.cnt)) == NULL) {
			m->tsr.cnt--;
			return -1;
		}
		m->tsr.tsr = tmp;
	}

	if ((m->tsr.tsr[i].fname = malloc(strlen(fname)+1)) == NULL) {
		m->tsr.tsr[i].fast_cmp = 0;
		return -1;
	}

	strcpy(m->tsr.tsr[i].fname, fname);
	m->tsr.tsr[i].fast_cmp = fast_cmp;
	m->tsr.tsr[i].ref = ts;

	return i;
}
	

void renderDelTSRef(void *handle, int ref) {
	DARNIT *m = handle;

	if (ref < 0 || ref >= m->tsr.cnt)
		return;
	
	m->tsr.tsr[ref].fast_cmp = 0;

	return;
}


TILESHEET *renderGetTilesheetFromRef(void *handle, const char *fname) {
	DARNIT *m = handle;
	int i, len, fast_cmp;

	if (fname == NULL) return NULL;
	len = strlen(fname);

	for (i = fast_cmp = 0; i < len; i++)
		fast_cmp += fname[i];
	
	for (i = 0; i < m->tsr.cnt; i++) {
		if (m->tsr.tsr[i].fast_cmp == fast_cmp)
			if (strcmp(m->tsr.tsr[i].fname, fname) == 0) {
				m->tsr.tsr[i].ref->ref_count++;
				return m->tsr.tsr[i].ref;
			}
	}

	return NULL;
}


TILESHEET *renderTilesheetLoad(void *handle, const char *fname, unsigned int wsq, unsigned int hsq, unsigned int convert_to) {
	TILESHEET *ts;
	IMGLOAD_DATA data;
	void *data_t;

	if ((ts = renderGetTilesheetFromRef(handle, fname)) != NULL)
		return ts;

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
	
/*	#ifndef HAVE_GLES */
		ts->texhandle = videoAddTexture(data_t, ts->w, ts->h);
/*	#else
		imgloadDownsample(&data, convert_to);
		data_t = data.img_data;
		if (convert_to == PFORMAT_RGBA8)
			ts->texhandle = videoAddTexture(data_t, ts->w, ts->h);
		else if (convert_to == PFORMAT_RGBA4)
			ts->texhandle = videoAddTextureRGBA4(data_t, ts->w, ts->h);
		else if (convert_to == PFORMAT_RGB5A1)
			ts->texhandle = videoAddTextureRGB5A1(data_t, ts->w, ts->h);
	#endif*/
	
	free(data_t);

	ts->wsq = wsq;
	ts->hsq = hsq;
	ts->tiles = (ts->w / wsq) * (ts->h / hsq);
	
	if ((ts->tile = malloc(ts->tiles * sizeof(TILE))) == NULL) {
		MALLOC_ERROR
		videoRemoveTexture(ts->texhandle);
		free(ts);
		return NULL;
	}

	renderPopulateTilesheet(handle, ts, ts->w / wsq, ts->h / hsq);
	renderAddTSRef(handle, fname, ts);

	return ts;
}


void renderPopulateTilesheet(void *handle, TILESHEET *ts, int tiles_w, int tiles_h) {
	DARNIT *m = handle;
	float twgran, thgran;
	int i, j, p;

	twgran = 1.0f / ts->w * ts->wsq;
	thgran = 1.0f / ts->h * ts->hsq;

	ts->sw = m->video.swgran * ts->wsq;
	ts->sh = m->video.shgran * ts->hsq;
	ts->swgran = m->video.swgran;
	ts->shgran = m->video.shgran;
	ts->ref_count = 0;
	
	for (i = 0; i < tiles_h; i++)
		for (j = 0; j < tiles_w; j++) {
			p = i * tiles_h + j;
			ts->tile[p].r = twgran * j;
			ts->tile[p].s = thgran * i;
			ts->tile[p].u = ts->tile[p].r + twgran;
			ts->tile[p].v = ts->tile[p].s + thgran;
		}
	return;
}


void *renderTilesheetFree(void *handle, TILESHEET *ts) {
	DARNIT *m = handle;
	if (ts == NULL) return NULL;

	ts->ref_count--;
	if (ts->ref_count > 0) return ts;
	videoRemoveTexture(ts->texhandle);
	renderDelTSRef(m, ts->ref);
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


void renderSetTileCoord(TILE_CACHE *cache, TILESHEET *ts, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	float xf, yf, wf, hf, tw, th;
	
	tw = 1.0f / ts->w;
	th = 1.0f / ts->h;
	xf = tw * x;
	yf = th * y;
	wf = tw * w;
	hf = th * h;
	wf += xf;
	hf += yf;

	cache->u = xf;
	cache->v = yf;
	cache->u2 = wf;
	cache->v2 = yf;
	cache->u3 = wf;
	cache->v3 = hf;
	cache->u4 = wf;
	cache->v4 = hf;
	cache->u5 = xf;
	cache->v5 = hf;
	cache->u6 = xf;
	cache->v6 = yf;
	
	return;
}


void renderSetTileCoordinates(TILE_CACHE *cache, float x, float y, float x2, float y2, float u, float v, float u2, float v2) {
	cache->u = u;
	cache->v = v;
	cache->u2 = u2;
	cache->v2 = v;
	cache->u3 = u2;
	cache->v3 = v2;
	cache->u4 = u2;
	cache->v4 = v2;
	cache->u5 = u;
	cache->v5 = v2;
	cache->u6 = u;
	cache->v6 = v;

	cache->x = x;
	cache->y = y2;
	cache->x2 = x2;
	cache->y2 = y2;
	cache->x3 = x2;
	cache->y3 = y;
	cache->x4 = x2;
	cache->y4 = y;
	cache->x5 = x;
	cache->y5 = y;
	cache->x6 = x;
	cache->y6 = y2;

	return;
}


void renderCache(TILE_CACHE *cache, TILESHEET *ts, int tiles) {
	if (cache == NULL) return;
	glBindTexture(GL_TEXTURE_2D, ts->texhandle);

	glVertexPointer(2, GL_FLOAT, 16, cache);
	glTexCoordPointer(2, GL_FLOAT, 16, &cache->u);

	glDrawArrays(GL_TRIANGLES, 0, tiles*6);

	return;
}


TILESHEET *renderNewTilesheet(void *handle, int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format) {
	unsigned int tilesheet_w, tilesheet_h, size, texture;
	TILESHEET *ts;
	
	tilesheet_w = bitwiseRoundUpToPow2(tiles_w * tile_w);
	tilesheet_h = bitwiseRoundUpToPow2(tiles_h * tile_h);

	size = tilesheet_w * tilesheet_h;
	if (format) size *= 4;

	format = (format == RENDER_DATA_TYPE_RGBA) ? GL_RGBA : GL_ALPHA;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, format, tilesheet_w, tilesheet_h, 0, format, GL_UNSIGNED_BYTE, 0);


	if ((ts = malloc(sizeof(TILESHEET))) == NULL) {
		glDeleteTextures(1, &texture);
		return NULL;
	}

	ts->texhandle = texture;
	
	if ((ts->tile = malloc(sizeof(TILE) * tiles_w * tiles_h)) == NULL) {
		glDeleteTextures(1, &texture);
		free(ts);
		return NULL;
	}

	ts->tiles = tiles_w * tiles_h;
	ts->wsq = tile_w;
	ts->hsq = tile_h;
	ts->w = tilesheet_w;
	ts->h = tilesheet_h;
	ts->ref = -1;

	renderPopulateTilesheet(handle, ts, tiles_w, tiles_h);

	return ts;
}


void renderUpdateTilesheet(TILESHEET *ts, int pos_x, int pos_y, void *data, int w, int h, int type) {
	unsigned int format;
	glBindTexture(GL_TEXTURE_2D, ts->texhandle);

	format = (type == RENDER_DATA_TYPE_RGBA) ? GL_RGBA : GL_ALPHA;
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, pos_x, pos_y, w, h, format, GL_UNSIGNED_BYTE, data);

	return;
}
