/*
Copyright (c) 2011-2013 Steven Arnow
'ldmz.c' - This file is part of libdarnit

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


const char *mapPropSearch(LDMZ_REF *ref, const char *key) {
	int i;

	for (i = 0; ref[i].key != NULL; i++) 
		if (strcmp(ref[i].key, key) == 0)
			return ref[i].value;

	return "NO SUCH KEY";
}


const char *mapPropGet(LDMZ_MAP *map, const char *key) {
	return mapPropSearch(map->prop, key);
}


const char *mapLayerPropGet(LDMZ_MAP *map, int layer, const char *key) {
	if (map->layers <= layer)
		return "LAYER OUT OF BOUND";
	return mapPropSearch(map->layer[layer].ref, key);
}


void mapCameraMove(LDMZ_MAP *map, int x, int y) {
	int i;

	if (!map)
		return;

	map->cam_x = x;
	map->cam_y = y;

	for (i = 0; i < map->layers; i++)
		renderTilemapCameraMove(map->layer[i].tilemap->render, x + map->layer[i].offset_x,
		    y + map->layer[i].offset_y);
	
	return;
}


LDMZ_MAP *mapDestroy(LDMZ_MAP *map) {
	int i;
	
	if (!map)
		return NULL;

	free(map->stringrefs);
	free(map->object);
	free(map->stringdata);

	for (i = 0; i < map->layers; i++) {
		tilemapFree(map->layer[i].tilemap);
		renderTilesheetFree(map->layer[i].ts);
	}

	free(map->layer);
	free(map);

	return NULL;
}


LDMZ_MAP *mapLoad(const char *fname) {
	return mapLoadReal(fname, -1, -1);
}


void mapLoadExtended(LDMZ_MAP *map, FILESYSTEM_FILE *f) {
	LDMZ_FILE_EXT1 *ext;
	void *tsdataz;
	int *tsdata;
	int i, j;

	for (i = 0; i < map->layers; i++) {
		map->layer[i].tilesheet_ref = NULL;
		map->layer[i].tilesheet_refs = 0;
	}

	tsdata = tsdataz = NULL;
	if (fsFileEOF(f))
		return;
	ext = malloc(sizeof(*ext) * map->layers);
	fsFileRead(ext, sizeof(*ext) * map->layers, f);
	utilBlockToHostEndian((unsigned int *) ext, map->layers * 2);

	for (i = 0; i < (int) map->layers; i++) {
		tsdataz = realloc(tsdataz, ext[i].ref_size_z);
		tsdata = realloc(tsdata, ext[i].refs * sizeof(*tsdata));
		fsFileRead(tsdataz, ext[i].ref_size_z, f);
		stbi_zlib_decode_buffer((void *) tsdata, sizeof(*tsdata) * ext[i].refs, tsdataz, ext[i].ref_size_z);
		utilBlockToHostEndian((void *) tsdata, ext[i].refs);
		map->layer[i].tilesheet_ref = malloc(sizeof(LDMZ_REF) * ext[i].refs);
		map->layer[i].tilesheet_refs = ext[i].refs;

		for (j = 0; j < ext[i].refs; j++)
			map->layer[i].tilesheet_ref[j] = map->stringrefs + tsdata[j];
	}

	free(tsdataz);
	free(tsdata);
	free(ext);

	return;
}


LDMZ_MAP *mapLoadReal(const char *fname, int tile_w, int tile_h) {
	LDMZ_MAP *map;
	LDMZ_FILE_MAP map_h;
	LDMZ_FILE_STRTABLE_REF *map_r;
	LDMZ_FILE_LAYER *map_l;
	LDMZ_FILE_OBJECT *object;
	FILESYSTEM_FILE *file;
	void *buff, *tmp;
	char *stringdata;
	int i, iso, tile_h_inc;

	map_h.magic = map_h.version = 0;

	if ((file = fsFileOpen(fname, "rb")) == NULL)
		return NULL;
	
	fsFileReadInts((unsigned int *) &map_h, sizeof(LDMZ_FILE_MAP) >> 2, file);
	if (map_h.magic != LDMZ_MAGIC) { 
		fsFileClose(file);
		return NULL;
	}

	if (map_h.version != LDMZ_VERSION && map_h.version != LDMZ_VERSION_ISOM) {
		fsFileClose(file);
		return NULL;
	}

	if ((buff = malloc(map_h.strtable_zlen)) == NULL) {
		fsFileClose(file);
		return NULL;
	}

	iso = (map_h.version == LDMZ_VERSION) ? 0 : 1;

	stringdata = malloc(map_h.strtable_len);
	map = malloc(sizeof(LDMZ_MAP));
	map->stringrefs = malloc(sizeof(LDMZ_REF) * (map_h.strtable_refs_len / sizeof(LDMZ_FILE_STRTABLE_REF)));
	map_r = malloc(map_h.strtable_refs_len);
	map_l = malloc(sizeof(LDMZ_FILE_LAYER) * map_h.layers);
	object = malloc(map_h.objects * sizeof(LDMZ_OBJECT));
	map->object = malloc(sizeof(LDMZ_OBJECT) * (map_h.objects));
	map->layer = malloc(sizeof(LDMZ_LAYER) * map_h.layers);

	map->isometric = !(map_h.version == LDMZ_VERSION);
	map->stringdata = stringdata;
	map->cam_x = map->cam_y = 0;
	map->layers = map_h.layers;
	map->objects = map_h.objects;

	if (!map || (!stringdata && map_h.strtable_len > 0) || (!map->stringrefs && map_h.strtable_refs_len > 0)
	    || (!map->object && map_h.objects) || (!map->layer && map_h.layers) 
	    || (!map_r && map_h.strtable_refs_len) || (!object && map_h.objects) || (!map_l)) 

		goto error;		/* Down at the bottom of the function */
	
	/**** STRINGS ****/

	fsFileRead(buff, map_h.strtable_zlen, file);
	stbi_zlib_decode_buffer(stringdata, map_h.strtable_len, buff, map_h.strtable_zlen);
	/**** REFS ****/

	if ((tmp = realloc(buff, map_h.strtable_refs_zlen)) == NULL)
		goto error;		/* Down at the bottom of the function */
	buff = tmp;
	fsFileRead(buff, map_h.strtable_refs_zlen, file);
	stbi_zlib_decode_buffer((void *) map_r, map_h.strtable_refs_len, buff, map_h.strtable_refs_zlen);
	utilBlockToHostEndian((unsigned int *) map_r, map_h.strtable_refs_len / 4);
	for (i = 0; i < (map_h.strtable_refs_len / sizeof(LDMZ_FILE_STRTABLE_REF)); i++) {
		if (map_r[i].key != -1) {
			map->stringrefs[i].key = &stringdata[map_r[i].key];
			map->stringrefs[i].value = &stringdata[map_r[i].value];
		} else {
			map->stringrefs[i].key = NULL;
			map->stringrefs[i].value = NULL;
		}
	}
	free(map_r);
	map_r = NULL;
	map->prop = &map->stringrefs[map_h.main_ref];

	/**** OBJECTS ****/

	if ((tmp = realloc(buff, map_h.object_zlen)) == NULL && map_h.objects > 0) {
		goto error;		/* Down at the bottom of the function */
	}
	buff = tmp;
	fsFileRead(buff, map_h.object_zlen, file);
	stbi_zlib_decode_buffer((void *) object, sizeof(LDMZ_OBJECT) * map_h.objects, buff, map_h.object_zlen);
	utilBlockToHostEndian((unsigned int *) object, (sizeof(LDMZ_OBJECT) * map_h.objects) >> 2);
	for (i = 0; i < map_h.objects; i++) {
		map->object[i].ref = &map->stringrefs[object[i].ref];
		map->object[i].x = object[i].x;
		map->object[i].y = object[i].y;
		map->object[i].l = object[i].l;
	}
	free(object);
	object = NULL;

	/**** MAP LAYER ****/

	if ((tmp = realloc(buff, map_h.layer_zlen)) == NULL)
		goto error;		/* Down at the bottom of the function */
	buff = tmp;
	fsFileRead(buff, map_h.layer_zlen, file);
	stbi_zlib_decode_buffer((void *)map_l, sizeof(LDMZ_FILE_LAYER) * map_h.layers, buff, map_h.layer_zlen);
	utilBlockToHostEndian((unsigned int *) map_l, (sizeof(LDMZ_FILE_LAYER) * map_h.layers) >> 2);
	for (i = 0; i < map_h.layers; i++) {
		map->layer[i].tilemap = NULL;
		map->layer[i].ts = NULL;
	}
	for (i = 0; i < map_h.layers; i++) {
		map->layer[i].tile_w = (tile_w > 0) ? tile_w : map_l[i].tile_w;
		map->layer[i].tile_h = (tile_h > 0) ? tile_h : map_l[i].tile_h;
		if (iso && tile_h > 0) {
			tile_h_inc = (tile_h << 16) / (map_l[i].tile_h);
			map_l[i].layer_offset_x *= tile_h_inc;
			map_l[i].layer_offset_x >>= 16;
		}
		map_l[i].tile_w = 0;
		map_l[i].tile_h = 0;
		
		map->layer[i].offset_x = map_l[i].layer_offset_x;
		map->layer[i].offset_y = map_l[i].layer_offset_y;
		
		map->layer[i].ref = &map->stringrefs[map_l[i].prop_ref];

		if ((tmp = realloc(buff, map_l[i].layer_zlen)) == NULL) 
			goto error;		/* Down at the bottom of the function */
		buff = tmp;
		if (strcmp(mapLayerPropGet(map, i, "tileset"), "NO SUCH KEY") == 0) {
			if (strcmp(mapPropGet(map, "tileset"), "NO SUCH KEY") == 0)
				goto error;	/* Down at the bottom of the function */
			else {
				if (iso) {
					if ((map->layer[i].ts = renderTilesheetLoadIsometric(mapPropGet(map, "tileset"),
					    map->layer[i].tile_w, map->layer[i].tile_h, PFORMAT_RGB5A1)) == NULL)
						goto error;	/* Down at the bottom of the function */
				} else {
					if ((map->layer[i].ts = renderTilesheetLoad(mapPropGet(map, "tileset"),
					    map->layer[i].tile_w, map->layer[i].tile_h, PFORMAT_RGB5A1)) == NULL)
						goto error;	/* Down at the bottom of the function */
				}
			}

			if (strcmp(mapPropGet(map, "animation"), "NO SUCH KEY"))
				renderTilesheetAnimationApply(map->layer[i].ts, mapPropGet(map, "animation"));
		} else {
			if (iso) {
				if ((map->layer[i].ts = renderTilesheetLoadIsometric(mapPropGet(map, "tileset"),
				    map->layer[i].tile_w, map->layer[i].tile_h, PFORMAT_RGB5A1)) == NULL)
					goto error;		/* Down at the bottom of the function */
			} else {
				if ((map->layer[i].ts = renderTilesheetLoad(mapPropGet(map, "tileset"),
				    map->layer[i].tile_w, map->layer[i].tile_h, PFORMAT_RGB5A1)) == NULL)
					goto error;		/* Down at the bottom of the function */
			}

			if (strcmp(mapLayerPropGet(map, i, "animation"), "NO SUCH KEY"))
				renderTilesheetAnimationApply(map->layer[i].ts, mapLayerPropGet(map, i, "animation"));
		}

		if (iso) {
			if ((map->layer[i].tilemap = tilemapNew(TILEMAP_DEFAULT_INV_DIV, map->layer[i].ts, 
			    TILEMAP_DEFAULT_INV_DIV, map_l[i].layer_w, map_l[i].layer_h, map_l[i].layer_offset_x)) == NULL)
			    	goto error;		/* Down at the bottom of the function */
		} else {
			if ((map->layer[i].tilemap = tilemapNew(TILEMAP_DEFAULT_INV_DIV, map->layer[i].ts, 
			    TILEMAP_DEFAULT_INV_DIV, map_l[i].layer_w, map_l[i].layer_h, 0)) == NULL)
			    	goto error;		/* Down at the bottom of the function */
		}

		if (!iso)
			renderTilemapCameraMove(map->layer[i].tilemap->render, map->cam_x + map->layer[i].offset_x, 
			    map->cam_y + map->layer[i].offset_y);
		else
			renderTilemapCameraMove(map->layer[i].tilemap->render, 0, 0); 
		fsFileRead(buff, map_l[i].layer_zlen, file);
		stbi_zlib_decode_buffer((void *) map->layer[i].tilemap->data, map_l[i].layer_w * map_l[i].layer_h * 4, 
		    buff, map_l[i].layer_zlen);
		utilBlockToHostEndian(map->layer[i].tilemap->data, map_l[i].layer_w * map_l[i].layer_h);
		renderTilemapForceRecalc(map->layer[i].tilemap->render);
	}
	
	free(map_l);
	free(buff);

	mapLoadExtended(map, file);

	fsFileClose(file);
	return map;

	error:		/* I know... This is my first goto in C. Ever. <.< */
		free(map_l);
		free(map_r);
		free(object);
		free(buff);
		fsFileClose(file);
		mapDestroy(map);
	return NULL;
}
