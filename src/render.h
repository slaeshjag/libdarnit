#ifndef __RENDER_H__
#define	__RENDER_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define			RENDER_DATA_TYPE_ALPHA		0
#define			RENDER_DATA_TYPE_RGBA		1


typedef struct {
	float		x;
	float		y;
	float		u;
	float		v;
	float		x2;
	float		y2;
	float		u2;
	float		v2;
	float		x3;
	float		y3;
	float		u3;
	float		v3;
	float		x4;
	float		y4;
	float		u4;
	float		v4;
	float		x5;
	float		y5;
	float		u5;
	float		v5;
	float		x6;
	float		y6;
	float		u6;
	float		v6;
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


int renderInit(void *handle);

TILESHEET *renderTilesheetLoad(void *handle, const char *fname, unsigned int wsq, unsigned int hsq, unsigned int convert_to);
void renderPopulateTilesheet(void *handle, TILESHEET *ts, int tiles_w, int tiles_h);
void *renderTilesheetFree(void *handle, TILESHEET *ts);
void renderMapCacheSetTile(void *handle, unsigned int layer, unsigned int k, unsigned int t, unsigned int x, unsigned int y);
void renderCalcTileCache(TILE_CACHE *cache, TILESHEET *ts, unsigned int tile);
void renderSetTileCoord(TILE_CACHE *cache, TILESHEET *ts, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void renderCalcTilePosCache(TILE_CACHE *cache, TILESHEET *ts, float x, float y);
void renderCacheOne(TILE_CACHE *cache, TILESHEET *ts);
void renderCache(TILE_CACHE *cache, TILESHEET *ts, int tiles);
TILESHEET *renderNewTilesheet(void *handle, int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format);
void renderUpdateTilesheet(TILESHEET *ts, int pos_x, int pos_y, void *data, int w, int h);
void renderSetTileCoordinates(TILE_CACHE *cache, float x, float y, float x2, float y2, float u, float v, float u2, float v2);
void renderFadeLoop(void *handle);
void renderFadeFade(void *handle, unsigned int time, float r, float g, float b);
void renderFadeUnfade(void *handle, unsigned int time);

void renderTilesheetAnimate(void *handle, TILESHEET *ts);
int renderTilesheetAnimationApply(TILESHEET *ts, const char *fname);



#ifndef HAVE_GLES
#include "render_ogl.h"
#else
#include "render_gles.h"
#endif


#endif
