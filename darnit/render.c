/*
Copyright (c) 2011-2013 Steven Arnow
'render.c' - This file is part of libdarnit

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

#ifndef DARNIT_HEADLESS

#ifndef HAVE_GLES
	#include "render/render_ogl.c"
#elif defined GCW_ZERO
	#include "render/render_gcwzero.c"
#else
	#include "render/render_gles.c"	
#endif

void *d_render_line_new(unsigned int lines, unsigned int line_w);
void d_render_line_move(void *buf, unsigned int line, int x1, int y1, int x2, int y2);
void d_render_line_draw(void *buf, int lines);
void *d_render_line_free(void *buf);
void d_render_begin();
void d_render_end();
void d_loop();


int renderLineTest() {
	void *line;
	unsigned int buff[2][25];
	int i;

	glClear(GL_COLOR_BUFFER_BIT);
	//d_loop();
	glClear(GL_COLOR_BUFFER_BIT);

	d->render_params.line_x1 = 0;
	d->render_params.line_x2 = 0;
	d->render_params.line_y1 = 0;
	d->render_params.line_y2 = 0;

	return 0;

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	line = d_render_line_new(1, 1);
	d_render_line_move(line, 0, 5, 5, 20, 5);

	d_render_begin();
	d_render_line_draw(line, 1);
	d_render_end();
	videoSwapBuffers();

	glReadPixels(3, d->video.h - 7, 5, 5, GL_RGBA, GL_UNSIGNED_BYTE, buff[0]);
	glReadPixels(18, d->video.h - 7, 5, 5, GL_RGBA, GL_UNSIGNED_BYTE, buff[1]);

	for (i = 0; i < 25; i++) {
		if (!(buff[0][i] & 0xFFFFFF))
			continue;
		break;
	}

	if (i == 25) {
		fprintf(stderr, "Line test failed. Assuming 0,0 offset, although this is clearly wrong.\n");
		return 0;
	}

	d->render_params.line_x1 = i % 5 - 2;
	d->render_params.line_y1 = (i / 5 - 2) * -1;

	for (i = 25; i > 0; i--) {
		if (!(buff[1][i-1] & 0xFFFFFF))
			continue;
		break;
	}


	line = d_render_line_free(line);
	if (i == 0) {
		fprintf(stderr, "Line test 2 failed. Assuming 0,0 offset, although this is clearly wrong\n");
		return 0;
	}

	d->render_params.line_x2 = (i - 1) % 5 - 2;
	d->render_params.line_y2 = ((i - 1) / 5 - 2) * -1;

	return 0;
}


int renderInit() {
	d->tsr.cnt = 0;
	d->tsr.tsr = NULL;

	d->video.fade.fading = 0;
	d->video.fade.fadedir = 0.0f;
	d->video.fade.a = 0.0f;
	d->video.fade.fadestart = 0;
	d->video.blend = 0;

	return 0;
}


void renderCalcTilePosCache(TILE_CACHE *cache, TILESHEET *ts, float x, float y, int angle) {
	int i;

	x /= ts->wsq;
	y /= ts->hsq;

	if (!angle) {
		cache->vertex[0].coord.x = x*ts->sw;
		cache->vertex[0].coord.y = y*ts->sh;
		cache->vertex[1].coord.x = ts->sw + x*ts->sw;
		cache->vertex[1].coord.y = y*ts->sh;
		cache->vertex[2].coord.x = ts->sw + x*ts->sw;
		cache->vertex[2].coord.y = y*ts->sh + ts->sh;
		cache->vertex[3].coord.x = ts->sw + x*ts->sw;
		cache->vertex[3].coord.y = y*ts->sh + ts->sh;
		cache->vertex[4].coord.x = x*ts->sw;
		cache->vertex[4].coord.y = y*ts->sh + ts->sh;
		cache->vertex[5].coord.x = x*ts->sw;
		cache->vertex[5].coord.y = y*ts->sh;
	} else {
		x += 0.5;
		y += 0.5;
		
		for (i = 0; i < 6; i++) {
			cache->vertex[i].coord.x = (i > 0 && i < 4) ? 0.5f : -0.5f; 
			cache->vertex[i].coord.y = (i > 1 && i < 5) ? 0.5f : -0.5f;
			utilCoordinatesRotate(&cache->vertex[i].coord.x, &cache->vertex[i].coord.y, angle);
		}

		for (i = 0; i < 6; i++) {
			cache->vertex[i].coord.x += x; 
			cache->vertex[i].coord.y += y;
		}
		
		for (i = 0; i < 6; i++) {
			cache->vertex[i].coord.x *= ts->sw; 
			cache->vertex[i].coord.y *= ts->sh;
		}
	}
	
	for (i = 0; i < 6; i++) {
		cache->vertex[i].coord.x -= 1.0f;
		cache->vertex[i].coord.y = 1.0f - cache->vertex[i].coord.y;
	}


	return;
}


void renderTileSetSize(TILE_CACHE *cache, int w, int h) {
	cache->vertex[1].coord.x = cache->vertex[0].coord.x + d->video.swgran * w;
	cache->vertex[2].coord.y = cache->vertex[0].coord.y - d->video.shgran * h;
	cache->vertex[2].coord.x = cache->vertex[3].coord.x = cache->vertex[1].coord.x;
	cache->vertex[3].coord.y = cache->vertex[4].coord.y = cache->vertex[2].coord.y;

	return;
}


void renderLineGet(LINE_CACHE *cache, int *x, int *y, int *x2, int *y2) {
	*x = (cache->x1 + 1.0f) / d->video.swgran;
	*y = (1.0f - cache->y1) / d->video.shgran;
	*x2 = (cache->x2 + 1.0f) / d->video.swgran;
	*y2 = (1.0f - cache->y2) / d->video.shgran;

	(*x) += d->render_params.line_x1;
	(*y) += d->render_params.line_y1;
	(*x2) += d->render_params.line_x2;
	(*y2) += d->render_params.line_y2;

	return;
}


void renderLineCalc(LINE_CACHE *cache, int x, int y, int x2, int y2) {
	x -= d->render_params.line_x1;
	y -= d->render_params.line_y1;
	x2 -= d->render_params.line_x2;
	y2 -= d->render_params.line_y2;

	cache->x1 = ((d->video.swgran * x) - 1.0f);
	cache->y1 = (1.0f - (d->video.shgran * y));
	cache->x2 = ((d->video.swgran * x2) - 1.0f);
	cache->y2 = (1.0f - (d->video.shgran * y2));

	return;
}


void renderPointCalc(POINT_CACHE *cache, int x, int y) {
	cache->coord[0].x = ((d->video.swgran * x) - 1.0f);
	cache->coord[0].y = (1.0f - (d->video.shgran * y));

	return;
}


void renderColorPointCalc(POINT_COLOR_CACHE *cache, int x, int y) {
	cache->coord[0].coord.x = ((d->video.swgran * x) - 1.0f);
	cache->coord[0].coord.y = (1.0f - (d->video.shgran * y));

	return;
}


void renderPointColor(POINT_COLOR_CACHE *cache, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	cache->coord[0].col.rgba[0] = r;
	cache->coord[0].col.rgba[1] = g;
	cache->coord[0].col.rgba[2] = b;
	cache->coord[0].col.rgba[3] = a;
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

	cache->vertex[0].tex.u = ts->tile[t].r;
	cache->vertex[0].tex.v = ts->tile[t].s;
	cache->vertex[1].tex.u = ts->tile[t].u;
	cache->vertex[1].tex.v = ts->tile[t].s;
	cache->vertex[2].tex.u = ts->tile[t].u;
	cache->vertex[2].tex.v = ts->tile[t].v;
	cache->vertex[3].tex.u = ts->tile[t].u;
	cache->vertex[3].tex.v = ts->tile[t].v;
	cache->vertex[4].tex.u = ts->tile[t].r;
	cache->vertex[4].tex.v = ts->tile[t].v;
	cache->vertex[5].tex.u = ts->tile[t].r;
	cache->vertex[5].tex.v = ts->tile[t].s;

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

	renderSetTileCoordinates(cache, cache->vertex[0].coord.x, cache->vertex[0].coord.y - vh, cache->vertex[0].coord.x + vw, cache->vertex[0].coord.y, xf, yf, wf, hf);

	return;
}


void renderBlitTile(TILESHEET *ts, unsigned int tile, int x, int y) {
	TILE_CACHE tile_c;

	renderCalcTileCache(&tile_c, ts, tile);
	renderCalcTilePosCache(&tile_c, ts, x, y, 0);
	renderCache(&tile_c, ts, 1);
	
	return;
}


void renderSetTileCoordinates(TILE_CACHE *cache, float x, float y, float x2, float y2, float u, float v, float u2, float v2) {
	cache->vertex[0].tex.u = u;
	cache->vertex[0].tex.v = v;
	cache->vertex[1].tex.u = u2;
	cache->vertex[1].tex.v = v;
	cache->vertex[2].tex.u = u2;
	cache->vertex[2].tex.v = v2;
	cache->vertex[3].tex.u = u2;
	cache->vertex[3].tex.v = v2;
	cache->vertex[4].tex.u = u;
	cache->vertex[4].tex.v = v2;
	cache->vertex[5].tex.u = u;
	cache->vertex[5].tex.v = v;

	cache->vertex[0].coord.x = x;
	cache->vertex[0].coord.y = y2;
	cache->vertex[1].coord.x = x2;
	cache->vertex[1].coord.y = y2;
	cache->vertex[2].coord.x = x2;
	cache->vertex[2].coord.y = y;
	cache->vertex[3].coord.x = x2;
	cache->vertex[3].coord.y = y;
	cache->vertex[4].coord.x = x;
	cache->vertex[4].coord.y = y;
	cache->vertex[5].coord.x = x;
	cache->vertex[5].coord.y = y2;

	return;
}


void renderSetTileCoordinatesColor(TILE_COLOR_TEX_CACHE *cache, float x, float y, float x2, float y2, float u, float v, float u2, float v2, unsigned char *color) {
	cache->vertex[0].tex.u = u;
	cache->vertex[0].tex.v = v;
	cache->vertex[1].tex.u = u2;
	cache->vertex[1].tex.v = v;
	cache->vertex[2].tex.u = u2;
	cache->vertex[2].tex.v = v2;
	cache->vertex[3].tex.u = u2;
	cache->vertex[3].tex.v = v2;
	cache->vertex[4].tex.u = u;
	cache->vertex[4].tex.v = v2;
	cache->vertex[5].tex.u = u;
	cache->vertex[5].tex.v = v;

	cache->vertex[0].coord.x = x;
	cache->vertex[0].coord.y = y2;
	cache->vertex[1].coord.x = x2;
	cache->vertex[1].coord.y = y2;
	cache->vertex[2].coord.x = x2;
	cache->vertex[2].coord.y = y;
	cache->vertex[3].coord.x = x2;
	cache->vertex[3].coord.y = y;
	cache->vertex[4].coord.x = x;
	cache->vertex[4].coord.y = y;
	cache->vertex[5].coord.x = x;
	cache->vertex[5].coord.y = y2;

	(cache->vertex[0].col.rgba)[0] = (color)[0];
	(cache->vertex[0].col.rgba)[1] = (color)[1];
	(cache->vertex[0].col.rgba)[2] = (color)[2];
	(cache->vertex[0].col.rgba)[3] = (color)[3];
	cache->vertex[1].col = cache->vertex[0].col;
	cache->vertex[2].col = cache->vertex[0].col;
	cache->vertex[3].col = cache->vertex[0].col;
	cache->vertex[4].col = cache->vertex[0].col;
	cache->vertex[5].col = cache->vertex[0].col;

	return;
}


void renderCache(TILE_CACHE *cache, TILESHEET *ts, int tiles) {
	if (cache == NULL) return;
	glBindTexture(GL_TEXTURE_2D, ts->texhandle);

	glVertexPointer(2, GL_FLOAT, 16, cache);
	glTexCoordPointer(2, GL_FLOAT, 16, &cache->vertex[0].tex.u);

	glDrawArrays(GL_TRIANGLES, 0, tiles*6);

	return;
}


void renderColCache(TILE_COLOR_TEX_CACHE *cache, TILESHEET *ts, int tiles) {
	if (!cache) return;

	glBindTexture(GL_TEXTURE_2D, ts->texhandle);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, 20, cache);
	glTexCoordPointer(2, GL_FLOAT, 20, &cache[0].vertex[0].tex);
	glColorPointer(4, GL_UNSIGNED_BYTE, 20, &cache->vertex->col);

	glDrawArrays(GL_TRIANGLES, 0, tiles*6);

	glDisableClientState(GL_COLOR_ARRAY);

	/* For some reason, this value gets overwritten on windows when color arrays are used */
	glColor4f(d->video.tint_r, d->video.tint_g, d->video.tint_b, d->video.tint_a);

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


void renderColorPointCache(POINT_COLOR_CACHE *cache, int points, int point_w) {
	if (!cache)
		return;

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glPointSize(point_w);
	glVertexPointer(2, GL_FLOAT, 12, cache);
	glColorPointer(4, GL_UNSIGNED_BYTE, 12, &cache->coord->col);
	glDrawArrays(GL_POINTS, 0, points);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	
	/* Since this value gets overwritten on windows, set it again! */
	glColor4f(d->video.tint_r, d->video.tint_g, d->video.tint_b, d->video.tint_a);

	return;
}


void renderPointCache(POINT_CACHE *cache, int points, int point_w) {
	if (!cache)
		return;

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glPointSize(point_w);
	glVertexPointer(2, GL_FLOAT, 0, cache);
	glDrawArrays(GL_POINTS, 0, points);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	return;
}


void renderTriCache(TRI_COLOR_CACHE *cache, int triangles) {
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, 12, cache);
	glColorPointer(4, GL_UNSIGNED_BYTE, 12, &cache->vertex->col);
	glDrawArrays(GL_TRIANGLES, 0, triangles * 3);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}


void renderFadeLoop() {
	int timediff;
	float coords[] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };

	if (d->video.fade.a == 0.0f && d->video.fade.fading == 0)
		return;
	
	timediff = tpw_ticks() - d->video.fade.fadestart;

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
	if (d->video.fade.a == 1.0f)
		return;

	d->video.fade.r = r;
	d->video.fade.g = g;
	d->video.fade.b = b;
	d->video.fade.fadefactor = 1.0f / time;
	d->video.fade.fadedir = 1.0f;
	d->video.fade.fadestart = tpw_ticks();
	d->video.fade.fading = 1;

	return;
}


void renderFadeUnfade(unsigned int time) {
	if (d->video.fade.fadedir == -1.0f)
		return;
	if (d->video.fade.a == 0.0f)
		return;

	d->video.fade.fadedir = -1.0f;
	d->video.fade.fadestart = tpw_ticks();
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


int videoSetIcon(const char *icon) {

	IMGLOAD_DATA img;

	if (!icon)
		return 0;
	
	if (!(img = imgloadLoad(icon)).img_data)
		return -1;

	tpw_icon_set(img.img_data, img.w, img.h);
	free(img.img_data);
	
	return 0;
}

// DARNIT_HEADLESS
#endif
