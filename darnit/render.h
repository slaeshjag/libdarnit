/*
Copyright (c) 2011-2013 Steven Arnow
'render.h' - This file is part of libdarnit

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


#ifndef __RENDER_H__
#define	__RENDER_H__


#define	LOGIC_OP_AND	1
#define	LOGIC_OP_NAND	2
#define	LOGIC_OP_OR	3
#define	LOGIC_OP_NOR	4
#define	LOGIC_OP_XOR	5

#define	DARNIT_SCALE_NEAREST	1
#define	DARNIT_SCALE_LINEAR	2


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int		line_x1;
	int		line_y1;
	int		line_x2;
	int		line_y2;
} RENDER_PARAMS;


typedef struct {
	float		x1;
	float		y1;
	float		x2;
	float		y2;
} LINE_CACHE;


typedef struct {
	float		x;
	float		y;
} COORDINATE;


typedef struct {
	float		u;
	float		v;
} TEX_COORDINATE;


typedef struct {
	unsigned char	rgba[4];
} COL_COORDINATE;


typedef struct {
	COORDINATE	coord;
	TEX_COORDINATE	tex;
	COL_COORDINATE	col;
} COLOR_TEX_VERTEX;


typedef struct {
	COORDINATE	coord;
	TEX_COORDINATE	tex;
} TEX_VERTEX;


typedef struct {
	COORDINATE	coord;
	COL_COORDINATE	col;
} COLOR_VERTEX;


typedef struct {
	COLOR_VERTEX	vertex[3];
} TRI_COLOR_CACHE;


typedef struct {
	COLOR_VERTEX	vertex[2];
} LINE_COLOR_CACHE;


typedef struct {
	COLOR_TEX_VERTEX vertex[6];
} TILE_COLOR_TEX_CACHE;


typedef struct {
	COORDINATE	coord[6];
} RECT_CACHE;

typedef struct {
	COLOR_VERTEX	vertex[6];

} TILE_COLOR_CACHE;


typedef struct {
	TEX_VERTEX	vertex[6];
} TILE_CACHE;


typedef struct {
	float		r;
	float		s;
	float		u;
	float		v;
} TILE;


typedef struct {
	unsigned int	tile_src;
	unsigned int	tile_dst;
	unsigned int	time;
} TS_FRAME;


typedef struct {
	TS_FRAME	*frame;
	unsigned int	frames;
	unsigned int	time_rest;
	unsigned int	frame_at;
} TS_TILE;


typedef struct {
	TS_TILE		*tile;
	int		tiles;
	unsigned int	*data;
	unsigned int	animation_tiles;
	unsigned int	tile_skip;
	void		*frame_data;
} TS_ANIMATION;


typedef struct {
	unsigned int	w;
	unsigned int	h;
	TILE		*tile;
	unsigned int	tiles;
	float		sw;
	float		sh;
	float		swgran;
	float		shgran;
	unsigned int	texhandle;
	unsigned int	wsq;
	unsigned int	hsq;

	unsigned int	ref_count;
	unsigned int	ref;
	unsigned int	format;

	TS_ANIMATION	animation;
} TILESHEET;


typedef struct {
	char		*fname;
	TILESHEET	*ref;
	unsigned int	fast_cmp;
} TILESHEET_REF;


typedef struct {
	TILESHEET_REF	*tsr;
	unsigned int	cnt;
} TILESHEET_REF_CONT;


typedef struct {
	float		r;
	float		g;
	float		b;
	float		a;
	float		fadefactor;
	unsigned int	fadestart;
	int		fadeprog;
	float		fadedir;
	int		fading;
} FADE_STRUCT;


int renderInit();
int videoInitPartial();

int renderLineTest();

TILESHEET *renderTilesheetLoad(const char *fname, unsigned int wsq, unsigned int hsq, unsigned int convert_to);
void renderPopulateTilesheet(TILESHEET *ts, int tiles_w, int tiles_h);
void *renderTilesheetFree(TILESHEET *ts);
void renderMapCacheSetTile(unsigned int layer, unsigned int k, unsigned int t, int x, int y);
void renderCalcTileCache(TILE_CACHE *cache, TILESHEET *ts, unsigned int tile);
void renderLineCalc(LINE_CACHE *cache, int x, int y, int x2, int y2);
void renderRectCalc(RECT_CACHE *cache, int x, int y, int x2, int y2);
void renderSetTileCoord(TILE_CACHE *cache, TILESHEET *ts, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void renderTileSetSize(TILE_CACHE *cache, int w, int h);
void renderCalcTilePosCache(TILE_CACHE *cache, TILESHEET *ts, float x, float y);
void renderCacheOne(TILE_CACHE *cache, TILESHEET *ts);
void renderCache(TILE_CACHE *cache, TILESHEET *ts, int tiles);
void renderColCache(TILE_COLOR_TEX_CACHE *cache, TILESHEET *ts, int tiles);
void renderLineCache(LINE_CACHE *cache, int lines, int line_w);
void renderRectCache(RECT_CACHE *cache, int rects);
void renderLineGet(LINE_CACHE *cache, int *x, int *y, int *x2, int *y2);
TILESHEET *renderNewTilesheet(int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format);
void renderTilesheetGeometrics(TILESHEET *ts, int *w, int *h, int *wsq, int *hsq);
void renderUpdateTilesheet(TILESHEET *ts, int pos_x, int pos_y, void *data, int w, int h);
void renderSetTileCoordinates(TILE_CACHE *cache, float x, float y, float x2, float y2, float u, float v, float u2, float v2);
void renderSetTileCoordinatesColor(TILE_COLOR_TEX_CACHE *cache, float x, float y, float x2, float y2, float u, float v, float u2, float v2, unsigned char *color);
void renderFadeLoop();
void renderFadeFade(unsigned int time, float r, float g, float b);
void renderFadeUnfade(unsigned int time);

void renderBlitTile(TILESHEET *ts, unsigned int tile, int x, int y);

void renderTilesheetAnimate(TILESHEET *ts);
int renderTilesheetAnimationApply(TILESHEET *ts, const char *fname);

void renderSetLogicOp(unsigned int logicop);
void renderTilesheetScalingSet(TILESHEET *ts, unsigned int scaling);
int videoSetIcon(const char *icon);

void renderTilesheetAnimateAll();


#ifndef DARNIT_HEADLESS

#ifndef HAVE_GLES
#include "render/render_ogl.h"
#elif defined GCW_ZERO
#include "render/render_gcwzero.h"
#else
#include "render/render_gles.h"
#endif

// DARNIT_HEADLESS
#endif

#endif
