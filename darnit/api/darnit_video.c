/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_video.c' - This file is part of libdarnit

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


void EXPORT_THIS *d_render_tilesheet_new(int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format) {
	return renderNewTilesheet(tiles_w, tiles_h, tile_w, tile_h, format);
}


void EXPORT_THIS *d_render_tilesheet_load(const char *fname, unsigned int wsq, unsigned int hsq, unsigned int target_format) {
	return renderTilesheetLoad(fname, wsq, hsq, target_format);
}

void EXPORT_THIS *d_render_tilesheet_isom_load(const char *fname, unsigned int wsq, unsigned int hsq, unsigned int target_format) {
	return renderTilesheetLoadIsometric(fname, wsq, hsq, target_format);
}


void EXPORT_THIS d_render_tilesheet_geometrics(void *tilesheet, int *w, int *h, int *tile_w, int *tile_h) {
	renderTilesheetGeometrics(tilesheet, w, h, tile_w, tile_h);

	return;
}


void EXPORT_THIS d_render_tilesheet_update(void *tilesheet, int sheet_x, int sheet_y, int change_w, int change_h, void *data) {
	renderUpdateTilesheet(tilesheet, sheet_x, sheet_y, data, change_w, change_h);

	return;
}


void EXPORT_THIS *d_render_tilesheet_free(void *tilesheet) {
	return renderTilesheetFree(tilesheet);
}


void EXPORT_THIS d_render_tile_blit(void *tilesheet, unsigned int tile, int x, int y) {
	renderBlitTile(tilesheet, tile, x, y);

	return;
}


void EXPORT_THIS d_render_tile_move(DARNIT_RENDER_BUFFER *buf, unsigned int tile, int x, int y) {
	TILE_CACHE *cache;

	if (buf->tiles <= tile)
		return;
	
	cache = &buf->tc[tile];
	renderCalcTilePosCache(cache, buf->ts, x, y);

	return;
}


/* tile_ts is the tile index in the tilesheet */
void EXPORT_THIS d_render_tile_set(DARNIT_RENDER_BUFFER *buf, unsigned int tile, unsigned int tile_ts) {
	TILE_CACHE *cache;

	if (buf->tiles <= tile)
		return;
	cache = &buf->tc[tile];
	renderCalcTileCache(cache, buf->ts, tile_ts);

	return;
}


void EXPORT_THIS d_render_tile_init(DARNIT_RENDER_BUFFER *buf, unsigned int tile, unsigned int tile_ts, int x, int y) {
	d_render_tile_set(buf, tile, tile_ts);
	d_render_tile_move(buf, tile, x, y);

	return;
}


void EXPORT_THIS d_render_line_move(DARNIT_RENDER_LINE_BUFFER *buf, unsigned int line, int x1, int y1, int x2, int y2) {
	LINE_CACHE *cache;

	if (buf->lines <= line)
		return;
	cache = &buf->lc[line];
	renderLineCalc(cache, x1, y1, x2, y2);

	return;
}

void EXPORT_THIS d_render_circle_move(DARNIT_RENDER_LINE_BUFFER *buf, int x, int y, int radius) {
	int a, i, step=3600/(buf->lines - 1);
	for(a = 0, i = 0; a <= 3600; a += step, i++) {
		d_render_line_move(buf, i, x + ((radius*utilCos(a)) >> 16), y+((radius*utilSine(a)) >> 16), x + ((radius*utilCos(a+step)) >> 16), y + ((radius*utilSine(a+step)) >> 16));
	}
}


void EXPORT_THIS d_render_point_move(DARNIT_RENDER_POINT_BUFFER *buf, unsigned int point, int x, int y) {
	POINT_CACHE *cache;

	if (buf->points <= point)
		return;
	cache = &buf->pc[point];
	renderPointCalc(cache, x, y);

	return;
}


void EXPORT_THIS d_render_rect_move(DARNIT_RENDER_RECT_BUFFER *buf, unsigned int rect, int x1, int y1, int x2, int y2) {
	RECT_CACHE *cache;

	if (buf->rects <= rect)
		return;
	cache = &buf->rc[rect];
	renderRectCalc(cache, x1, y1, x2, y2);

	return;
}


void EXPORT_THIS d_render_rect_get(DARNIT_RENDER_RECT_BUFFER *buf, unsigned int rect, int *x1, int *y1, int *x2, int *y2) {
	RECT_CACHE *cache;
	
	if (buf->rects <= rect)
		return;
	cache = &buf->rc[rect];
	if (x1)
		*x1 = (cache->coord->x + 1.0f) / d->video.swgran;
	if (x2)
		*x2 = (cache->coord[1].x + 1.0f) / d->video.swgran;
	if (y1)
		*y1 = (1.0f - cache->coord->y) / d->video.shgran;
	if (y2)
		*y2 = (1.0f - cache->coord[2].y) / d->video.shgran;
	
	return;
}


void EXPORT_THIS d_render_tile_tilesheet_coord_set(DARNIT_RENDER_BUFFER *buf, unsigned int tile, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	TILE_CACHE *cache;

	if (buf == NULL) return;
	if (buf->tiles <= tile)
		return;
	cache = &buf->tc[tile];
	renderSetTileCoord(cache, buf->ts, x, y, w, h);

	return;
}


void EXPORT_THIS d_render_tile_size_set(DARNIT_RENDER_BUFFER *buf, unsigned int tile, int w, int h) {
	TILE_CACHE *cache;

	if (buf == NULL)
		return;
	if (buf->tiles <= tile)
		return;
	cache = &buf->tc[tile];
	renderTileSetSize(cache, w, h);

	return;
}


void EXPORT_THIS d_render_tile_clear(DARNIT_RENDER_BUFFER *buf, unsigned int tile) {
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


void EXPORT_THIS d_render_tile_draw(DARNIT_RENDER_BUFFER *buf, unsigned int tiles) {
	if (buf == NULL) return;
	if (buf->tiles <= tiles)
		tiles = buf->tiles;
	renderCache(buf->tc, buf->ts, tiles);

	return;
}


void EXPORT_THIS d_render_tile_tilesheet(DARNIT_RENDER_BUFFER *buf, void *texture) {
	if (!buf)
		return;
	buf->ts = texture;

	return;
}


void EXPORT_THIS d_render_line_draw(DARNIT_RENDER_LINE_BUFFER *buf, int lines) {
	if (buf == NULL)
		return;
	if (buf->lines < lines)
		lines = buf->lines;
	renderLineCache(buf->lc, lines, buf->line_w);

	return;
}


void EXPORT_THIS d_render_circle_draw(DARNIT_RENDER_LINE_BUFFER *buf) {
	d_render_line_draw(buf, buf->lines);
}


void EXPORT_THIS d_render_point_draw(DARNIT_RENDER_POINT_BUFFER *buf, int points) {
	if (points == -1 || buf->points < points)
		points = buf->points;
	renderPointCache(buf->pc, points, buf->point_w);
}


void EXPORT_THIS d_render_rect_draw(DARNIT_RENDER_RECT_BUFFER *buf, int rects) {
	if (buf == NULL)
		return;
	if (buf->rects < rects)
		rects = buf->rects;
	renderRectCache(buf->rc, rects);

	return;
}


void EXPORT_THIS *d_render_tile_new(unsigned int tiles, void *tilesheet) {
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
	buf->ts = tilesheet;

	return buf;
}



void EXPORT_THIS *d_render_rect_new(unsigned int rects) {
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


void EXPORT_THIS *d_render_line_new(unsigned int lines, unsigned int line_w) {
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

void EXPORT_THIS *d_render_circle_new(unsigned int lines, unsigned int line_w) {
	return d_render_line_new(lines+1, line_w);
}


void EXPORT_THIS *d_render_point_new(unsigned int points, unsigned int point_w) {
	DARNIT_RENDER_POINT_BUFFER *buf;
	int i, j;
	float *arr;

	if (!(buf = malloc(sizeof(DARNIT_RENDER_POINT_BUFFER) * points)))
		return NULL;
	if (!(buf->pc = malloc(sizeof(POINT_CACHE) * points))) {
		free(buf);
		return NULL;
	}

	for (i = 0; i < points; i++) {
		arr = (void *) &buf->pc[i];
		for (j = 0; j < 2; j++)
			arr[j] = 0;
	}

	buf->points = points;
	buf->point_w = point_w;

	return buf;
}


void EXPORT_THIS *d_render_tile_free(DARNIT_RENDER_BUFFER *buf) {
	if (buf == NULL) return NULL;
	free(buf->tc);
	free(buf);

	return NULL;
}


void EXPORT_THIS *d_render_line_free(DARNIT_RENDER_LINE_BUFFER *buf) {
	if (buf == NULL)
		return NULL;
	free(buf->lc);
	free(buf);

	return NULL;
}


void EXPORT_THIS *d_render_circle_free(DARNIT_RENDER_LINE_BUFFER *buf) {
	return d_render_line_free(buf);
}


void EXPORT_THIS *d_render_point_free(DARNIT_RENDER_POINT_BUFFER *buf) {
	if (!buf)
		return NULL;
	free(buf->pc);
	free(buf);

	return NULL;
}


void EXPORT_THIS *d_render_rect_free(DARNIT_RENDER_RECT_BUFFER *buf) {
	if (buf == NULL)
		return NULL;
	free(buf->rc);
	free(buf);

	return NULL;
}


void EXPORT_THIS d_render_line_get(DARNIT_RENDER_LINE_BUFFER *buf, unsigned int line, int *x, int *y, int *x2, int *y2) {
	if (line >= buf->lines)
		return;
	renderLineGet(&buf->lc[line], x, y, x2, y2);

	return;
}


void EXPORT_THIS d_render_begin() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}


void EXPORT_THIS d_render_end() {
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}


void EXPORT_THIS d_render_blend_enable() {
	d->video.blend = 1;
	glEnable(GL_BLEND);
}


void EXPORT_THIS d_render_blend_disable() {
	d->video.blend = 0;
	glDisable(GL_BLEND);
}


void EXPORT_THIS d_render_tint_get(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a) {
	if (r)
		*r = d->video.tint_r * 255;
	if (g)
		*g = d->video.tint_g * 255;
	if (b)
		*b = d->video.tint_b * 255;
	if (a)
		*a = d->video.tint_a * 255;
	return;
}



void EXPORT_THIS d_render_tint(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	d->video.tint_r = (float) r/255;
	d->video.tint_g = (float) g/255;
	d->video.tint_b = (float) b/255;
	d->video.tint_a = (float) a/255;
	glColor4f(d->video.tint_r, d->video.tint_g, d->video.tint_b, d->video.tint_a);
	

	return;
}


void EXPORT_THIS d_render_clearcolor_set(unsigned char r, unsigned char g, unsigned char b) {
	float rf, gf, bf;

	rf = 1.0f/255 * r;
	gf = 1.0f/255 * g;
	bf = 1.0f/255 * b;

	glClearColor(rf, gf, bf, 0.0f);

	return;
}


void EXPORT_THIS d_render_offset(int x, int y) {
	glLoadIdentity();
	glTranslatef(d->video.swgran * x * -1, d->video.shgran * y, 0.0f);
	d->video.offset_x = x * -1;
	d->video.offset_y = y;

	return;
}


void EXPORT_THIS d_render_fade_in(unsigned int time, unsigned char r, unsigned char g, unsigned char b) {
	renderFadeFade(time, 1.0f/(256-r), 1.0f/(256-g), 1.0f/(256-b));

	return;
}


void EXPORT_THIS d_render_fade_out(unsigned int time) {
	renderFadeUnfade(time);

	return;
}


int EXPORT_THIS d_render_fade_status() {
	return d->video.fade.fading;
}


void EXPORT_THIS d_render_state_restore() {
	d_render_offset(d->video.offset_x, d->video.offset_y);
//	FIXME: Implement
//	darnitRenderTint(d->video.tint_r, d->video.tint_g, d->video.tint_b, d->video.tint_a);
	glEnable(GL_TEXTURE_2D);
	if (d->video.blend)
		d_render_blend_enable();
	else
		d_render_blend_disable();
	return;
}


int EXPORT_THIS d_render_tilesheet_animation_apply(void *tilesheet, const char *fname) {
	return renderTilesheetAnimationApply(tilesheet, fname);
}


void EXPORT_THIS d_render_tilesheet_animate(void *tilesheet) {
	renderTilesheetAnimate(tilesheet);

	return;
}


void EXPORT_THIS d_render_logic_op(unsigned int logicop) {
	renderSetLogicOp(logicop);

	return;
}


void EXPORT_THIS d_render_tilesheet_scale_algorithm(void *tilesheet, unsigned int scaling) {
	renderTilesheetScalingSet((TILESHEET *) tilesheet, scaling);

	return;
}
