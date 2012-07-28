#include "darnit.h"


void EXPORT_THIS *darnitRenderTilesheetNew(void *handle, int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format) {
	return renderNewTilesheet(handle, tiles_w, tiles_h, tile_w, tile_h, format);
}


void EXPORT_THIS *darnitRenderTilesheetLoad(void *handle, const char *fname, unsigned int wsq, unsigned int hsq, unsigned int target_format) {
	return renderTilesheetLoad(handle, fname, wsq, hsq, target_format);
}


void EXPORT_THIS darnitRenderTilesheetUpdate(void *tilesheet, int sheet_x, int sheet_y, int change_w, int change_h, void *data) {
	renderUpdateTilesheet(tilesheet, sheet_x, sheet_y, data, change_w, change_h);

	return;
}


void EXPORT_THIS *darnitRenderTilesheetFree(void *handle, void *tilesheet) {
	return renderTilesheetFree(handle, tilesheet);
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


void EXPORT_THIS *darnitRenderTileFree(DARNIT_RENDER_BUFFER *buf) {
	if (buf == NULL) return NULL;
	free(buf->tc);
	free(buf);

	return NULL;
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


void EXPORT_THIS darnitRenderBlendingEnable(void *handle) {
	DARNIT *d = handle;
	
	d->video.blend = 1;
	glEnable(GL_BLEND);
}


void EXPORT_THIS darnitRenderBlendingDisable(void *handle) {
	DARNIT *d = handle;

	d->video.blend = 0;
	glDisable(GL_BLEND);
}


void EXPORT_THIS darnitRenderTint(void *handle, float r, float g, float b, float a) {
	DARNIT *d = handle;
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


void EXPORT_THIS darnitRenderOffset(void *handle, int x, int y) {
	DARNIT *d = handle;
	glLoadIdentity();
	glTranslatef(d->video.swgran * x * -1, d->video.shgran * y, 0.0f);
	d->video.offset_x = x * -1;
	d->video.offset_y = y;

	return;
}


void EXPORT_THIS darnitRenderFadeIn(void *handle, unsigned int time, float r, float g, float b) {
	renderFadeFade(handle, time, r, g, b);

	return;
}


void EXPORT_THIS darnitRenderFadeOut(void *handle, unsigned int time) {
	renderFadeUnfade(handle, time);

	return;
}


int EXPORT_THIS darnitRenderFadeChanging(void *handle) {
	DARNIT *d = handle;

	return d->video.fade.fading;
}


void EXPORT_THIS darnitRenderStateRestore(void *handle) {
	DARNIT *d = handle;

	darnitRenderOffset(d, d->video.offset_x, d->video.offset_y);
	darnitRenderTint(d, d->video.tint_r, d->video.tint_g, d->video.tint_b, d->video.tint_a);
	glEnable(GL_TEXTURE_2D);
	if (d->video.blend)
		darnitRenderBlendingEnable(d);
	else
		darnitRenderBlendingDisable(d);
	return;
}


int EXPORT_THIS darnitRenderTilesheetAnimationApply(void *tilesheet, const char *fname) {
	return renderTilesheetAnimationApply(tilesheet, fname);
}


void EXPORT_THIS darnitRenderTilesheetAnimationAnimate(void *handle, void *tilesheet) {
	renderTilesheetAnimate(handle, tilesheet);

	return;
}
