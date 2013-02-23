/*
Copyright (c) 2011-2013 Steven Arnow
'tilesheet.h' - This file is part of libdarnit

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


#ifndef __TILESHEET_H__
#define	__TILESHEET_H__


typedef struct {
	float		r;
	float		s;
	float		u;
	float		v;
	float		h_p;
} TILESHEET_TILE;


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
	TILESHEET_TILE	*tile;
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


TILESHEET *renderTilesheetLoad(const char *fname, unsigned int wsq, unsigned int hsq, unsigned int convert_to);
TILESHEET *renderTilesheetLoadIsometric(const char *fname, unsigned int wsq, unsigned int hsq, unsigned int convert_to);
void renderPopulateTilesheet(TILESHEET *ts, int tiles_w, int tiles_h);
void renderPopulateIsometricTilesheet(TILESHEET *ts, int tiles_w, int tiles_h, unsigned int *data);
void *renderTilesheetFree(TILESHEET *ts);
TILESHEET *renderNewTilesheet(int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format);
void renderTilesheetGeometrics(TILESHEET *ts, int *w, int *h, int *wsq, int *hsq);
void renderUpdateTilesheet(TILESHEET *ts, int pos_x, int pos_y, void *data, int w, int h);
void renderTilesheetAnimate(TILESHEET *ts);
int renderTilesheetAnimationApply(TILESHEET *ts, const char *fname);
void renderTilesheetScalingSet(TILESHEET *ts, unsigned int scaling);
void renderTilesheetAnimateAll();


#endif
