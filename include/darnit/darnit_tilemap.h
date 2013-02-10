/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_tilemap.h' - This file is part of libdarnit

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


#ifndef __DARNIT_TILEMAP_H__
#define	__DARNIT_TILEMAP_H__

#define			DARNIT_TILEMAP_DEFAULT_MASK	0xFFF

typedef struct {
	int		w;
	int		h;
	unsigned int	*data;
	void		*render;
} DARNIT_TILEMAP;


DARNIT_TILEMAP *d_tilemap_new(unsigned int invisibility_divider, DARNIT_TILESHEET *tilesheet, unsigned int mask, int w, int h);
DARNIT_TILEMAP *d_tilemap_load(const char *fname, unsigned int invisibility_divider, DARNIT_TILESHEET *tilesheet, unsigned int mask);
void d_tilemap_tile_set(DARNIT_TILEMAP *tilemap, int x, int y, int tile);
void d_tilemap_recalc(DARNIT_TILEMAP *tilemap);
void d_tilemap_camera_move(DARNIT_TILEMAP *tilemap, int cam_x, int cam_y);
void d_tilemap_draw(DARNIT_TILEMAP *tilemap);
DARNIT_TILEMAP *d_tilemap_free(DARNIT_TILEMAP *tilemap);

#endif
