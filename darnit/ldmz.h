/*
Copyright (c) 2011-2013 Steven Arnow
'ldmz.h' - This file is part of libdarnit

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


#ifndef __TTMZ_H__
#define	__TTMZ_H__

#define	LDMZ_MAGIC		0xFF00E00E
#define	LDMZ_VERSION		0x55555555

typedef struct {
	unsigned int		magic;
	unsigned int		version;
	unsigned int		strtable_len;
	unsigned int		strtable_zlen;
	unsigned int		strtable_refs_len;
	unsigned int		strtable_refs_zlen;
	unsigned int		layers;
	unsigned int		layer_zlen;
	unsigned int		objects;
	unsigned int		object_zlen;
	unsigned int		refs;
	unsigned int		main_ref;
} LDMZ_FILE_MAP;


typedef struct {
	int			key;
	int			value;
} LDMZ_FILE_STRTABLE_REF;


typedef struct {
	unsigned int		layer_w;
	unsigned int		layer_h;
	int			layer_offset_x;
	int			layer_offset_y;
	unsigned int		layer_zlen;
	unsigned int		prop_ref;
	unsigned int		tile_w;
	unsigned int		tile_h;
} LDMZ_FILE_LAYER;


typedef struct {
	unsigned int		x;
	unsigned int		y;
	unsigned int		l;
	unsigned int		ref;
} LDMZ_FILE_OBJECT;


typedef struct {
	const char		*key;
	const char		*value;
} LDMZ_REF;


typedef struct {
	unsigned int		x;
	unsigned int		y;
	unsigned int		l;
	LDMZ_REF		*ref;
} LDMZ_OBJECT;


typedef struct {
	TILEMAP_ENTRY		*tilemap;
	TILESHEET		*ts;
	LDMZ_REF		*ref;
	unsigned int		offset_x;
	unsigned int		offset_y;
	unsigned int		tile_w;
	unsigned int		tile_h;
} LDMZ_LAYER;


typedef struct {
	LDMZ_LAYER		*layer;
	unsigned int		layers;
	LDMZ_OBJECT		*object;
	unsigned int		objects;
	LDMZ_REF		*prop;
	void			*stringdata;
	LDMZ_REF		*stringrefs;

	int			cam_x;
	int			cam_y;
} LDMZ_MAP;


const char *mapPropSearch(LDMZ_REF *ref, const char *key);
const char *mapLayerPropGet(LDMZ_MAP *map, int layer, const char *key);
void mapCameraMove(LDMZ_MAP *map, int x, int y);
LDMZ_MAP *mapDestroy(LDMZ_MAP *map);
LDMZ_MAP *mapLoad(const char *fname);


#endif
