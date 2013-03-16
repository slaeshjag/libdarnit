/*
Copyright (c) 2011-2013 Steven Arnow
'render_tilemap.h' - This file is part of libdarnit

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


#ifndef __RENDER_TILEMAP_H__
#define	__RENDER_TILEMAP_H__

#define	TILEMAP_SPRITE_START		32


/* Tror jag råkade göra en Exophase :p */
#define	RENDER_TILEMAP_FILL(k, X, Y, w, h, t)			\
	tm->cache[(k)].vertex[0].coord.x = (X);			\
	tm->cache[(k)].vertex[0].coord.y = (Y) + (h);		\
	tm->cache[(k)].vertex[1].coord.x = (X) + (w);		\
	tm->cache[(k)].vertex[1].coord.y = (Y) + (h); 		\
	tm->cache[(k)].vertex[2].coord.x = (X) + (w);		\
	tm->cache[(k)].vertex[2].coord.y = (Y);			\
	tm->cache[(k)].vertex[3].coord.x = (X) + (w);		\
	tm->cache[(k)].vertex[3].coord.y = (Y);			\
	tm->cache[(k)].vertex[4].coord.x = (X);			\
	tm->cache[(k)].vertex[4].coord.y = (Y);			\
	tm->cache[(k)].vertex[5].coord.x = (X);			\
	tm->cache[(k)].vertex[5].coord.y = (Y) + (h);		\
	tm->cache[(k)].vertex[0].tex.u = tm->ts->tile[(t)].r;	\
	tm->cache[(k)].vertex[0].tex.v = tm->ts->tile[(t)].s;	\
	tm->cache[(k)].vertex[1].tex.u = tm->ts->tile[(t)].u;	\
	tm->cache[(k)].vertex[1].tex.v = tm->ts->tile[(t)].s;	\
	tm->cache[(k)].vertex[2].tex.u = tm->ts->tile[(t)].u;	\
	tm->cache[(k)].vertex[2].tex.v = tm->ts->tile[(t)].v;	\
	tm->cache[(k)].vertex[3].tex.u = tm->ts->tile[(t)].u;	\
	tm->cache[(k)].vertex[3].tex.v = tm->ts->tile[(t)].v;	\
	tm->cache[(k)].vertex[4].tex.u = tm->ts->tile[(t)].r;	\
	tm->cache[(k)].vertex[4].tex.v = tm->ts->tile[(t)].v;	\
	tm->cache[(k)].vertex[5].tex.u = tm->ts->tile[(t)].r;	\
	tm->cache[(k)].vertex[5].tex.v = tm->ts->tile[(t)].s;	\



typedef struct {
	int				map_w;
	int				map_h;
	int				w;
	int				h;
	TILESHEET			*ts;
	TILE_CACHE			*cache;
	int				cache_used;
	unsigned int			inv_div;
	unsigned int			*map;
	float				cam_x;
	float				cam_y;
	int				cam_xi;
	int				cam_yi;
	int				cam_xp;
	int				cam_yp;
	unsigned int			mask;
	unsigned int			isometric;
	unsigned int			r_h;

	SPRITE_ENTRY			**sprite;
	unsigned int			sprites;
	unsigned int			sprites_used;
} RENDER_TILEMAP;

void renderTilemapISOCoordinates(RENDER_TILEMAP *tm, int x, int y, int *r_x, int *r_y);
void renderTilemapToISOCoordinates(RENDER_TILEMAP *tm, int x, int y, int *r_x, int *r_y);


void *renderTilemapCreate(unsigned int w, unsigned int h, unsigned int *map, int camera_x, int camera_y, unsigned int invisibility_divider, TILESHEET *ts, unsigned int mask);
void *renderTilemapCreateISO(unsigned int w, unsigned int h, unsigned int *map, int camera_x, int camera_y, unsigned int invisibility_divider, TILESHEET *ts, unsigned int mask, int r_h);
void renderTilemapTileSet(RENDER_TILEMAP *tm, int x, int y, int tile);
void renderTilemapCameraMove(RENDER_TILEMAP *tm, int cam_x, int cam_y);
void renderTilemapRender(RENDER_TILEMAP *tm);
void renderTilemapFree(RENDER_TILEMAP *tm);
void renderTilemapForceRecalc(RENDER_TILEMAP *tm);

#endif
