#include "darnit.h"

#ifndef DARNIT_HEADLESS

#ifndef HAVE_GLES
	#include "render/render_ogl.c"
#elif defined GCW_ZERO
	#include "render/render_gcwzero.c"
#else
	#include "render/render_gles.c"	
#endif


int renderInit() {
	d->tsr.cnt = 0;
	d->tsr.tsr = NULL;

	d->video.fade.fading = 0;
	d->video.fade.fadedir = 0.0f;
	d->video.fade.a = 0.0f;
	d->video.fade.fadestart = 0;

	return 0;
}


int renderAddTSRef(const char *fname, TILESHEET *ts) {
	void *tmp;
	int i, len, fast_cmp;

	if (fname == NULL) return -1;
	len = strlen(fname);

	for (i = fast_cmp = 0; i < len; i++)
		fast_cmp += fname[i];
	
	for (i = 0; i < d->tsr.cnt; i++)
		if (d->tsr.tsr[i].fast_cmp == 0)
			break;
	
	if (i == d->tsr.cnt) {
		d->tsr.cnt++;
		if ((tmp = realloc(d->tsr.tsr, sizeof(TILESHEET_REF) * d->tsr.cnt)) == NULL) {
			d->tsr.cnt--;
			return -1;
		}
		d->tsr.tsr = tmp;
	}

	if ((d->tsr.tsr[i].fname = malloc(strlen(fname)+1)) == NULL) {
		d->tsr.tsr[i].fast_cmp = 0;
		return -1;
	}

	strcpy(d->tsr.tsr[i].fname, fname);
	d->tsr.tsr[i].fast_cmp = fast_cmp;
	d->tsr.tsr[i].ref = ts;

	return i;
}


void renderDelTSRef(int ref) {
	if (ref < 0 || ref >= d->tsr.cnt)
		return;
	
	d->tsr.tsr[ref].fast_cmp = 0;

	return;
}


TILESHEET *renderGetTilesheetFromRef(const char *fname) {
	int i, len, fast_cmp;

	if (fname == NULL) return NULL;
	len = strlen(fname);

	for (i = fast_cmp = 0; i < len; i++)
		fast_cmp += fname[i];
	
	for (i = 0; i < d->tsr.cnt; i++) {
		if (d->tsr.tsr[i].fast_cmp == fast_cmp)
			if (strcmp(d->tsr.tsr[i].fname, fname) == 0) {
				d->tsr.tsr[i].ref->ref_count++;
				return d->tsr.tsr[i].ref;
			}
	}

	return NULL;
}


void renderTilesheetAnimateAll() {
	int i;

	for (i = 0; i < d->tsr.cnt; i++) {
		if (!d->tsr.tsr[i].fast_cmp)
			continue;
		if (d->tsr.tsr[i].ref->animation.tiles > 0)
			renderTilesheetAnimate(d->tsr.tsr[i].ref);
	}

	return;
}


void renderTilesheetAnimate(TILESHEET *ts) {
	int i, j, change;
	TS_FRAME *frame;

	if (ts == NULL)
		return;


	for (i = 0; i < ts->animation.tiles; i++) {
		change = 0;
		ts->animation.tile[i].time_rest += darnitTimeLastFrameTook();
		j = ts->animation.tile[i].frame[ts->animation.tile[i].frame_at].time;
		while (ts->animation.tile[i].time_rest >= j) {
			ts->animation.tile[i].time_rest -= j;
			ts->animation.tile[i].frame_at++;
			if (ts->animation.tile[i].frame_at >= ts->animation.tile[i].frames)
				ts->animation.tile[i].frame_at = 0;
			j = ts->animation.tile[i].frame[ts->animation.tile[i].frame_at].time;
			frame = &ts->animation.tile[i].frame[ts->animation.tile[i].frame_at];
			change = 1;
		}

		if (change) {
			i = frame->tile_dst % (ts->w / ts->wsq);
			j = frame->tile_dst / (ts->w / ts->wsq);
			renderUpdateTilesheet(ts, i * ts->wsq, j * ts->hsq, &ts->animation.data[ts->animation.tile_skip * frame->tile_src], ts->wsq, ts->hsq);
		}
	}

	return;
}


int renderTilesheetAnimationApply(TILESHEET *ts, const char *fname) {
	unsigned int i, j, k, tiles, frames, pos, a, b, cc, d, e;
	unsigned int *newdata;
	char c, buf[512];
	FILESYSTEM_FILE *fp;
	IMGLOAD_DATA img;
	TS_FRAME *frame;
	
	if (ts == NULL)
		return -1;
	
	if (ts->animation.tiles > 0)
		return -1;

	if ((fp = fsFileOpen(fname, "rb")) == NULL)
		return -1;

	fsFileGetLine(buf, 512, fp);
	pos = fsFileTell(fp);
	img = imgloadLoad(buf);

	if (img.img_data == NULL) {
		fsFileClose(fp);
		return -1;
	}

	if (img.w != ts->wsq) {
		if (!(newdata = malloc(sizeof(unsigned int) * ts->wsq * ts->hsq * (img.w / ts->wsq * img.h / ts->hsq)))) {
			fsFileClose(fp);
			free(img.img_data);
			return -1;
		}

		a = img.w / ts->wsq;
		b = img.h / ts->hsq;
		cc = ts->wsq * ts->hsq;
		for (i = 0; i < a; i++) {
			for (j = 0; j < b; j++)
				for (k = 0; k < cc; k++) {
					/* Skip all the lines before us */
					d = (a * j * cc) + (k / ts->wsq * img.w);
					/* Skip all the cols before us */
					e = i * ts->wsq + k % ts->wsq;
					newdata[(j*a + i) * cc + k] = img.img_data[d + e];
				}
		}
		free(img.img_data);
		img.img_data = newdata;
		img.w = ts->wsq;
		img.h = ts->hsq * a * b;

	}

	ts->animation.animation_tiles = img.h / ts->hsq;
	tiles = frames = 0;
	
	while (!fsFileEOF(fp)) {
		fsFileRead(&c, 1, fp);
		switch (c) {
			case 'T':
				fsFileGets(buf, 512, fp);
				break;
			case 'F':
				fsFileGets(buf, 512, fp);
				frames++;
				break;
			case 'E':
				fsFileGets(buf, 512, fp);
				tiles++;
				break;
			case '\n':
				break;
			default:
				fsFileGets(buf, 512, fp);
				break;
		}
	}

	i = j = 0;
	fsFileSeek(fp, pos, SEEK_SET);

	
	frame = malloc(sizeof(TS_FRAME) * frames);
	ts->animation.tile = malloc(sizeof(TS_TILE) * tiles);

	if (frame == NULL || ts->animation.tile == NULL) {
		fsFileClose(fp);
		free(img.img_data);
		free(frame);
		free(ts->animation.tile);
		ts->animation.tile = NULL;
		return -1;
	}

	ts->animation.tiles = tiles;
	ts->animation.tile_skip = ts->wsq * ts->hsq;
	ts->animation.data = img.img_data;
	ts->animation.frame_data = frame;
	frames = 0;

	while (!fsFileEOF(fp)) {
		fsFileRead(&c, 1, fp);
		switch (c) {
			case 'T':
				fsFileGets(buf, 512, fp);
				ts->animation.tile[i].frame = &frame[frames];
				ts->animation.tile[i].frame_at = 0;
				ts->animation.tile[i].time_rest = 0;
				j = 0;
				break;
			case 'F':
				fsFileGets(buf, 512, fp);
				sscanf(buf, "%i %i %i\n", &ts->animation.tile[i].frame[j].tile_src, &ts->animation.tile[i].frame[j].tile_dst, &ts->animation.tile[i].frame[j].time);
				j++;
				break;
			case 'E':
				fsFileGets(buf, 512, fp);
				ts->animation.tile[i].frames = j;
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

	ts->animation.tiles = i;

	fsFileClose(fp);

	return 0;
}


TILESHEET *renderTilesheetLoad(const char *fname, unsigned int wsq, unsigned int hsq, unsigned int convert_to) {
	TILESHEET *ts;
	IMGLOAD_DATA data;
	void *data_t;

	if ((ts = renderGetTilesheetFromRef(fname)) != NULL) {
		return ts;
	}

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
	ts->animation.tiles = 0;
	ts->animation.frame_data = NULL;
	ts->animation.tile = NULL;
	ts->animation.data = NULL;
	
	imgloadDownsample(&data, convert_to);
	data_t = data.img_data;
	if (convert_to == PFORMAT_RGBA8)
		ts->texhandle = videoAddTexture(data_t, ts->w, ts->h);
	else if (convert_to == PFORMAT_RGBA4)
		ts->texhandle = videoAddTextureRGBA4(data_t, ts->w, ts->h);
	else if (convert_to == PFORMAT_RGB5A1)
		ts->texhandle = videoAddTextureRGB5A1(data_t, ts->w, ts->h);
	
	free(data_t);

	ts->wsq = wsq;
	ts->hsq = hsq;
	ts->format = convert_to;
	ts->tiles = (ts->w / wsq) * (ts->h / hsq);
	
	if ((ts->tile = malloc(ts->tiles * sizeof(TILE))) == NULL) {
		MALLOC_ERROR
		videoRemoveTexture(ts->texhandle);
		free(ts);
		return NULL;
	}

	renderPopulateTilesheet(ts, ts->w / wsq, ts->h / hsq);
	ts->ref = renderAddTSRef(fname, ts);

	return ts;
}


void renderPopulateTilesheet(TILESHEET *ts, int tiles_w, int tiles_h) {
	float twgran, thgran;
	int i, j, p;

	twgran = 1.0f / ts->w * ts->wsq;
	thgran = 1.0f / ts->h * ts->hsq;

	ts->sw = d->video.swgran * ts->wsq;
	ts->sh = d->video.shgran * ts->hsq;
	ts->swgran = d->video.swgran;
	ts->shgran = d->video.shgran;
	ts->ref_count = 1;
	
	for (i = 0; i < tiles_h; i++)
		for (j = 0; j < tiles_w; j++) {
			p = i * tiles_w + j;
			ts->tile[p].r = twgran * j;
			ts->tile[p].s = thgran * i;
			ts->tile[p].u = ts->tile[p].r + twgran;
			ts->tile[p].v = ts->tile[p].s + thgran;
		}
	return;
}


void *renderTilesheetFree(TILESHEET *ts) {
	if (ts == NULL) return NULL;

	ts->ref_count--;
	if (ts->ref_count > 0) return ts;
	videoRemoveTexture(ts->texhandle);
	renderDelTSRef(ts->ref);
	free(ts->animation.frame_data);
	free(ts->animation.tile);
	free(ts->animation.data);
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


void renderTileSetSize(TILE_CACHE *cache, int w, int h) {
	cache->x2 = cache->x + d->video.swgran * w;
	cache->y3 = cache->y - d->video.shgran * h;
	cache->x3 = cache->x4 = cache->x2;
	cache->y4 = cache->y5 = cache->y3;

	return;
}



void renderLineGet(LINE_CACHE *cache, int *x, int *y, int *x2, int *y2) {
	*x = (cache->x1 + 1.0f) / d->video.swgran;
	*y = (1.0f - cache->y1) / d->video.shgran;
	*x2 = (cache->x2 + 1.0f) / d->video.swgran;
	*y2 = (1.0f - cache->y2) / d->video.shgran;

	return;
}


void renderLineCalc(LINE_CACHE *cache, int x, int y, int x2, int y2) {
	cache->x1 = ((d->video.swgran * x) - 1.0f);
	cache->y1 = (1.0f - (d->video.shgran * y));
	cache->x2 = ((d->video.swgran * x2) - 1.0f);
	cache->y2 = (1.0f - (d->video.shgran * y2));

	return;
}


void renderRectCalc(RECT_CACHE *cache, int x, int y, int x2, int y2) {
	float xf, yf, xf2, yf2;

	xf = (d->video.swgran * x) - 1.0f;
	yf = 1.0f - (d->video.shgran * y);
	xf2 = (d->video.swgran * x2) - 1.0f;
	yf2 = 1.0f - (d->video.shgran * y2);

	cache->coord[0].x = cache->coord[5].x = cache->coord[4].x = xf;
	cache->coord[0].y = cache->coord[5].y = cache->coord[1].y = yf;
	cache->coord[1].x = cache->coord[2].x = cache->coord[3].x = xf2;
	cache->coord[2].y = cache->coord[3].y = cache->coord[4].y = yf2;

	return;
}


void renderCalcTileCache(TILE_CACHE *cache, TILESHEET *ts, unsigned int tile) {
	unsigned int t = tile;

	if (t >= ts->tiles)
		return;

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
	float xf, yf, wf, hf, tw, th, vw, vh;
	
	vw = d->video.swgran * w;
	vh = d->video.shgran * h;
	tw = 1.0f / ts->w;
	th = 1.0f / ts->h;
	xf = tw * x;
	yf = th * y;
	wf = tw * w;
	hf = th * h;
	wf += xf;
	hf += yf;

	renderSetTileCoordinates(cache, cache->x, cache->y - vh, cache->x + vw, cache->y, xf, yf, wf, hf);

/*	cache->u = xf;
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
	cache->v6 = yf;*/
	
	return;
}


void renderBlitTile(TILESHEET *ts, unsigned int tile, int x, int y) {
	TILE_CACHE tile_c;

	renderCalcTileCache(&tile_c, ts, tile);
	renderCalcTilePosCache(&tile_c, ts, x, y);
	renderCache(&tile_c, ts, 1);
	
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


void renderRectCache(RECT_CACHE *cache, int rects) {
	if (!cache)
		return;

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glVertexPointer(2, GL_FLOAT, 0, cache);
	glDrawArrays(GL_TRIANGLES, 0, rects * 6);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	return;
}


void renderLineCache(LINE_CACHE *cache, int lines, int line_w) {
	if (!cache)
		return;

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(line_w);
	glVertexPointer(2, GL_FLOAT, 0, cache);
	glDrawArrays(GL_LINES, 0, lines * 2);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	return;
}


TILESHEET *renderNewTilesheet(int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format) {
	unsigned int tilesheet_w, tilesheet_h, texture;
	TILESHEET *ts;
	
	tilesheet_w = bitwiseRoundUpToPow2(tiles_w * tile_w);
	tilesheet_h = bitwiseRoundUpToPow2(tiles_h * tile_h);

	if ((ts = malloc(sizeof(TILESHEET))) == NULL)
		return NULL;

	ts->animation.tiles = 0;
	ts->animation.frame_data = NULL;
	ts->animation.tile = NULL;
	ts->animation.data = NULL;
	ts->w = tilesheet_w;
	ts->h = tilesheet_h;
	ts->format = format;

	if (format == PFORMAT_RGBA8)
		ts->texhandle = videoAddTexture(NULL, ts->w, ts->h);
	else if (format == PFORMAT_RGBA4)
		ts->texhandle = videoAddTextureRGBA4(NULL, ts->w, ts->h);
	else if (format == PFORMAT_RGB5A1)
		ts->texhandle = videoAddTextureRGB5A1(NULL, ts->w, ts->h);
	else if (format == PFORMAT_A8)
		ts->texhandle = videoAddTextureA8(NULL, ts->w, ts->h);
	
	if ((ts->tile = malloc(sizeof(TILE) * tiles_w * tiles_h)) == NULL) {
		glDeleteTextures(1, &texture);
		free(ts);
		return NULL;
	}

	ts->tiles = tiles_w * tiles_h;
	ts->wsq = tile_w;
	ts->hsq = tile_h;
	ts->ref = -1;

	renderPopulateTilesheet(ts, tiles_w, tiles_h);

	return ts;
}


void renderTilesheetGeometrics(TILESHEET *ts, int *w, int *h, int *wsq, int *hsq) {
	if (!ts)
		return;
	if (w)
		*w = ts->w;
	if (h)
		*h = ts->h;
	if (wsq)
		*wsq = ts->wsq;
	if (hsq)
		*hsq = ts->hsq;

	return;
}


void renderUpdateTilesheet(TILESHEET *ts, int pos_x, int pos_y, void *data, int w, int h) {
	unsigned int format;

	glBindTexture(GL_TEXTURE_2D, ts->texhandle);
	format = (ts->format == PFORMAT_A8) ? GL_ALPHA : GL_RGBA;

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, pos_x, pos_y, w, h, format, GL_UNSIGNED_BYTE, data);

	return;
}


void renderTilesheetScalingSet(TILESHEET *ts, unsigned int scaling) {
	unsigned int flag;

	glBindTexture(GL_TEXTURE_2D, ts->texhandle);

	if (scaling == DARNIT_SCALE_NEAREST) {
		flag = GL_NEAREST;
	} else if (scaling == DARNIT_SCALE_LINEAR) {
		flag = GL_LINEAR;
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flag);

	return;
}


void renderFadeLoop() {
	int timediff;
	float coords[] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };

	if (d->video.fade.a == 0.0f && d->video.fade.fading == 0)
		return;

	timediff = SDL_GetTicks() - d->video.fade.fadestart;

	if (timediff < 0)
		timediff = 1;
	
	if (d->video.fade.fadedir != 0.0f) {
		d->video.fade.a = d->video.fade.fadefactor * d->video.fade.fadedir * timediff;
		d->video.fade.fadeprog = timediff;
	}

	if (d->video.fade.fadedir < 0)
		d->video.fade.a += 1.0f;
	
	if (d->video.fade.a < 0.0f && d->video.fade.fadedir < 0.0f) {
		d->video.fade.fadedir = 0.0f;
		d->video.fade.a = 0.0f;
		d->video.fade.fading = 0;
	} else if (d->video.fade.a >= 1.0f && d->video.fade.fadedir > 0.0f) {
		d->video.fade.fadedir = 0.0f;
		d->video.fade.a = 1.0f;
		d->video.fade.fading = 2;
	}

	if (d->video.blend == 0)
		glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor4f(d->video.fade.r, d->video.fade.g, d->video.fade.b, d->video.fade.a);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, coords);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glColor4f(d->video.tint_r, d->video.tint_g, d->video.tint_b, d->video.tint_a);
	if (d->video.blend == 0)
		glDisable(GL_BLEND);

	return;
}


void renderFadeFade(unsigned int time, float r, float g, float b) {
	if (d->video.fade.fadedir == 1.0f)
		return;

	d->video.fade.r = r;
	d->video.fade.g = g;
	d->video.fade.b = b;
	d->video.fade.fadefactor = 1.0f / time;
	d->video.fade.fadedir = 1.0f;
	d->video.fade.fadestart = SDL_GetTicks();
	d->video.fade.fading = 1;

	return;
}


void renderFadeUnfade(unsigned int time) {
	if (d->video.fade.fadedir == -1.0f)
		return;

	d->video.fade.fadedir = -1.0f;
	d->video.fade.fadestart = SDL_GetTicks();
	d->video.fade.fadestart -= (time - d->video.fade.fadeprog);
	d->video.fade.fadefactor = 1.0f / time;
	d->video.fade.fading = -1;

	return;
}


void renderSetLogicOp(unsigned int logicop) {
	unsigned int mode;
	if (!logicop) {
		glDisable(GL_COLOR_LOGIC_OP);
		return;
	}

	glEnable(GL_COLOR_LOGIC_OP);

	switch (logicop) {
		case LOGIC_OP_AND:
			mode = GL_AND;
			break;
		case LOGIC_OP_NAND:
			mode = GL_NAND;
			break;
		case LOGIC_OP_OR:
			mode = GL_OR;
			break;
		case LOGIC_OP_NOR:
			mode = GL_NOR;
			break;
		case LOGIC_OP_XOR:
			mode = GL_XOR;
			break;
		default:
			mode = GL_COPY;
			break;
	}

	glLogicOp(mode);

	return;
}


int videoInitPartial() {
	return SDL_Init(SDL_INIT_EVERYTHING);
}


int videoSetIcon(const char *icon) {
	SDL_Surface *surface;
	IMGLOAD_DATA img;

	if (!icon)
		return 0;
	
	if (!(img = imgloadLoad(icon)).img_data)
		return -1;

	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, img.w, img.h, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	free(surface->pixels);
	surface->pixels = img.img_data;
	d->icon = surface;

	SDL_WM_SetIcon(surface, NULL);
	
	return 0;
}

// DARNIT_HEADLESS
#endif
