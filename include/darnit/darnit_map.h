/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_map.h' - This file is part of libdarnit

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


#ifndef __DARNIT_MAP_H__
#define	__DARNIT_MAP_H__

typedef void			DARNIT_MAP_REF;
typedef void			DARNIT_MAP_STRINGDATA;

typedef struct {
	unsigned int		x;
	unsigned int		y;
	unsigned int		l;
	DARNIT_MAP_REF		*ref;
} DARNIT_MAP_OBJECT;


typedef struct {
	DARNIT_TILEMAP		*tilemap;
	DARNIT_TILESHEET	*ts;
	DARNIT_MAP_REF		*ref;
	unsigned int		offset_x;
	unsigned int		offset_y;
	unsigned int		tile_w;
	unsigned int		tile_h;
} DARNIT_MAP_LAYER;


typedef struct {
	DARNIT_MAP_LAYER	*layer;
	const unsigned int	layers;
	DARNIT_MAP_OBJECT	*object;
	const unsigned int	objects;
	DARNIT_MAP_REF		*prop;
	DARNIT_MAP_STRINGDATA	*stringdata;
	DARNIT_MAP_REF		*stringrefs;

	const int		cam_x;
	const int 		cam_y;
	unsigned int		isometric;
} DARNIT_MAP;


DARNIT_MAP *d_map_load(const char *fname);
DARNIT_MAP *d_map_load_override(const char *fname, int tile_w, int tile_h);
DARNIT_MAP *d_map_unload(DARNIT_MAP *map);
const char *d_map_prop(DARNIT_MAP_REF *prop, const char *key);
const char *d_map_layer_prop(DARNIT_MAP *map, int layer, const char *key);
void d_map_camera_move(DARNIT_MAP *map, int x, int y);

#endif
