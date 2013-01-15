#include "darnit.h"


void EXPORT_THIS *darnitRenderTilesheetNew(int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format) {
	return renderNewTilesheet(tiles_w, tiles_h, tile_w, tile_h, format);
}


void EXPORT_THIS *darnitRenderTilesheetLoad(const char *fname, unsigned int wsq, unsigned int hsq, unsigned int target_format) {
	return renderTilesheetLoad(fname, wsq, hsq, target_format);
}


void EXPORT_THIS darnitRenderTilesheetGeometricsGet(void *tilesheet, int *w, int *h, int *tile_w, int *tile_h) {
	renderTilesheetGeometrics(tilesheet, w, h, tile_w, tile_h);

	return;
}


void EXPORT_THIS darnitRenderTilesheetUpdate(void *tilesheet, int sheet_x, int sheet_y, int change_w, int change_h, void *data) {
	renderUpdateTilesheet(tilesheet, sheet_x, sheet_y, data, change_w, change_h);

	return;
}


void EXPORT_THIS *darnitRenderTilesheetFree(void *tilesheet) {
	return renderTilesheetFree(tilesheet);
}


void EXPORT_THIS darnitRenderTileBlit(void *tilesheet, unsigned int tile, int x, int y) {
	renderBlitTile(tilesheet, tile, x, y);

	return;
}


void EXPORT_THIS darnitRenderTileMove(DARNIT_RENDER_BUFFER *buf, unsigned int tile, void *tilesheet, unsigned int x, unsigned int y) {
	TILE_CACHE *cache;

	if (buf->tiles <= tile)
		return;
	
	cache = &buf->tc[tile];
	renderCalcTilePosCache(cache, tilesheet, x, y);

	return;
}


/* tile_ts is the tile index in the tilesheet */
void EXPORT_THIS darnitRenderTileSet(DARNIT_RENDER_BUFFER *buf, unsigned int tile, void *tilesheet, unsigned int tile_ts) {
	TILE_CACHE *cache;

	if (buf->tiles <= tile)
		return;
	cache = &buf->tc[tile];
	renderCalcTileCache(cache, tilesheet, tile_ts);

	return;
}


void EXPORT_THIS darnitRenderLineMove(DARNIT_RENDER_LINE_BUFFER *buf, unsigned int line, int x1, int y1, int x2, int y2) {
	LINE_CACHE *cache;

	if (buf->lines <= line)
		return;
	cache = &buf->lc[line];
	renderLineCalc(cache, x1, y1, x2, y2);

	return;
}

void EXPORT_THIS darnitRenderCircleMove(DARNIT_RENDER_LINE_BUFFER *buf, int x, int y, int radius) {
	int a, i, step=3600/(buf->lines - 1);
	for(a = 0, i = 0; a <= 3600; a += step, i++) {
		darnitRenderLineMove(buf, i, x + ((radius*utilCos(a)) >> 16), y+((radius*utilSine(a)) >> 16), x + ((radius*utilCos(a+step)) >> 16), y + ((radius*utilSine(a+step)) >> 16));
	}
}


void EXPORT_THIS darnitRenderRectSet(DARNIT_RENDER_RECT_BUFFER *buf, unsigned int rect, int x1, int y1, int x2, int y2) {
	RECT_CACHE *cache;

	if (buf->rects <= rect)
		return;
	cache = &buf->rc[rect];
	renderRectCalc(cache, x1, y1, x2, y2);

	return;
}


void EXPORT_THIS darnitRenderTileSetTilesheetCoord(DARNIT_RENDER_BUFFER *buf, unsigned int tile, void *tilesheet, 
							unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	TILE_CACHE *cache;

	if (buf == NULL) return;
	if (buf->tiles <= tile)
		return;
	cache = &buf->tc[tile];
	renderSetTileCoord(cache, tilesheet, x, y, w, h);

	return;
}


void EXPORT_THIS darnitRenderTileSizeSet(DARNIT_RENDER_BUFFER *buf, unsigned int tile, int w, int h) {
	TILE_CACHE *cache;

	if (buf == NULL)
		return;
	if (buf->tiles <= tile)
		return;
	cache = &buf->tc[tile];
	renderTileSetSize(cache, w, h);

	return;
}


void EXPORT_THIS darnitRenderTileClear(DARNIT_RENDER_BUFFER *buf, unsigned int tile) {
	float *ptr;
	int i;

	if (buf == NULL) return;
	if (tile >= buf->tiles)
		return;
	ptr = (float *) &buf->tc[tile];
	for (i = 0; i < 24; i++)
		ptr[i] = 0.0f;
	
	return;
}


void EXPORT_THIS darnitRenderTileDraw(DARNIT_RENDER_BUFFER *buf, void *tilesheet, unsigned int tiles) {
	if (buf == NULL) return;
	if (buf->tiles <= tiles)
		tiles = buf->tiles;
	renderCache(buf->tc, tilesheet, tiles);

	return;
}


void EXPORT_THIS darnitRenderLineDraw(DARNIT_RENDER_LINE_BUFFER *buf, int lines) {
	if (buf == NULL)
		return;
	if (buf->lines < lines)
		lines = buf->lines;
	renderLineCache(buf->lc, lines, buf->line_w);

	return;
}


void EXPORT_THIS darnitRenderCircleDraw(DARNIT_RENDER_LINE_BUFFER *buf) {
	darnitRenderLineDraw(buf, buf->lines);
}


void EXPORT_THIS darnitRenderRectDraw(DARNIT_RENDER_RECT_BUFFER *buf, int rects) {
	if (buf == NULL)
		return;
	if (buf->rects < rects)
		rects = buf->rects;
	renderRectCache(buf->rc, rects);

	return;
}


void EXPORT_THIS *darnitRenderTileAlloc(unsigned int tiles) {
	DARNIT_RENDER_BUFFER *buf;
	int i, j;
	float *arr;

	buf = malloc(sizeof(DARNIT_RENDER_BUFFER));
	if ((buf->tc = malloc(sizeof(TILE_CACHE)*tiles)) == NULL)
		return NULL;
	
	for (i = 0; i < tiles; i++) {
		arr = (void *) &buf->tc[i];
		for (j = 0; j < 24; j++)
			arr[j] = 0.0f;
	}

	buf->tiles = tiles;

	return buf;
}



void EXPORT_THIS *darnitRenderRectAlloc(unsigned int rects) {
	DARNIT_RENDER_RECT_BUFFER *buf;
	int i, j;
	float *arr;

	buf = malloc(sizeof(DARNIT_RENDER_RECT_BUFFER));
	if ((buf->rc = malloc(sizeof(RECT_CACHE)*rects)) == NULL)
		return NULL;
	for (i = 0; i < rects; i++) {
		arr = (void *) &buf->rc[i];
		for (j = 0; j < 12; j++)
			arr[j] = 0.0f;
	}

	buf->rects = rects;

	return buf;
}


void EXPORT_THIS *darnitRenderLineAlloc(unsigned int lines, unsigned int line_w) {
	DARNIT_RENDER_LINE_BUFFER *buf;
	int i, j;
	float *arr;

	buf = malloc(sizeof(DARNIT_RENDER_LINE_BUFFER));
	if ((buf->lc = malloc(sizeof(LINE_CACHE)*lines)) == NULL)
		return NULL;
	for (i = 0; i < lines; i++) {
		arr = (void *) &buf->lc[i];
		for (j = 0; j < 4; j++)
			arr[j] = 0.0f;
	}

	buf->lines = lines;
	buf->line_w = line_w;

	return buf;
}

void EXPORT_THIS *darnitRenderCircleAlloc(unsigned int lines, unsigned int line_w) {
	return darnitRenderLineAlloc(lines+1, line_w);
}


void EXPORT_THIS *darnitRenderTileFree(DARNIT_RENDER_BUFFER *buf) {
	if (buf == NULL) return NULL;
	free(buf->tc);
	free(buf);

	return NULL;
}


void EXPORT_THIS *darnitRenderLineFree(DARNIT_RENDER_LINE_BUFFER *buf) {
	if (buf == NULL)
		return NULL;
	free(buf->lc);
	free(buf);

	return NULL;
}


void EXPORT_THIS *darnitRenderCircleFree(DARNIT_RENDER_LINE_BUFFER *buf) {
	return darnitRenderLineFree(buf);
}


void EXPORT_THIS *darnitRenderRectFree(DARNIT_RENDER_RECT_BUFFER *buf) {
	if (buf == NULL)
		return NULL;
	free(buf->rc);
	free(buf);

	return NULL;
}


void EXPORT_THIS darnitRenderLineGet(DARNIT_RENDER_LINE_BUFFER *buf, unsigned int line, int *x, int *y, int *x2, int *y2) {
	if (line >= buf->lines)
		return;
	renderLineGet(&buf->lc[line], x, y, x2, y2);

	return;
}


void EXPORT_THIS darnitRenderBegin() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}


void EXPORT_THIS darnitRenderEnd() {
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}


void EXPORT_THIS darnitRenderBlendingEnable() {
	d->video.blend = 1;
	glEnable(GL_BLEND);
}


void EXPORT_THIS darnitRenderBlendingDisable() {
	d->video.blend = 0;
	glDisable(GL_BLEND);
}


void EXPORT_THIS darnitRenderTintGet(float *r, float *g, float *b, float *a) {
	if (r)
		*r = d->video.tint_r;
	if (g)
		*g = d->video.tint_g;
	if (b)
		*b = d->video.tint_b;
	if (a)
		*a = d->video.tint_a;
	return;
}



void EXPORT_THIS darnitRenderTint(float r, float g, float b, float a) {
	glColor4f(r, g, b, a);
	d->video.tint_r = r;
	d->video.tint_g = g;
	d->video.tint_b = b;
	d->video.tint_a = a;
	

	return;
}


void EXPORT_THIS darnitRenderClearColorSet(unsigned char r, unsigned char g, unsigned char b) {
	float rf, gf, bf;

	rf = 1.0f/255 * r;
	gf = 1.0f/255 * g;
	bf = 1.0f/255 * b;

	glClearColor(rf, gf, bf, 0.0f);

	return;
}


void EXPORT_THIS darnitRenderOffset(int x, int y) {
	glLoadIdentity();
	glTranslatef(d->video.swgran * x * -1, d->video.shgran * y, 0.0f);
	d->video.offset_x = x * -1;
	d->video.offset_y = y;

	return;
}


void EXPORT_THIS darnitRenderFadeIn(unsigned int time, float r, float g, float b) {
	renderFadeFade(time, r, g, b);

	return;
}


void EXPORT_THIS darnitRenderFadeOut(unsigned int time) {
	renderFadeUnfade(time);

	return;
}


int EXPORT_THIS darnitRenderFadeChanging() {
	return d->video.fade.fading;
}


void EXPORT_THIS darnitRenderStateRestore() {
	darnitRenderOffset(d->video.offset_x, d->video.offset_y);
	darnitRenderTint(d->video.tint_r, d->video.tint_g, d->video.tint_b, d->video.tint_a);
	glEnable(GL_TEXTURE_2D);
	if (d->video.blend)
		darnitRenderBlendingEnable();
	else
		darnitRenderBlendingDisable();
	return;
}


int EXPORT_THIS darnitRenderTilesheetAnimationApply(void *tilesheet, const char *fname) {
	return renderTilesheetAnimationApply(tilesheet, fname);
}


void EXPORT_THIS darnitRenderTilesheetAnimationAnimate(void *tilesheet) {
	renderTilesheetAnimate(tilesheet);

	return;
}


void EXPORT_THIS darnitRenderLogicOp(unsigned int logicop) {
	renderSetLogicOp(logicop);

	return;
}


void EXPORT_THIS darnitRenderTilesheetScaleAlgorithm(void *tilesheet, unsigned int scaling) {
	renderTilesheetScalingSet((TILESHEET *) tilesheet, scaling);

	return;
}
