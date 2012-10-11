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
		renderTilemapCameraMove(map->layer[i].tilemap->render, x, y);
	
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
	LDMZ_MAP *map;
	LDMZ_FILE_MAP map_h;
	LDMZ_FILE_STRTABLE_REF *map_r;
	LDMZ_FILE_LAYER *map_l;
	LDMZ_FILE_OBJECT *object;
	FILESYSTEM_FILE *file;
	void *buff, *tmp;
	char *stringdata;
	int i;

	if ((file = fsFileOpen(fname, "rb")) == NULL)
		return NULL;
	
	fsFileReadInts((unsigned int *) &map_h, sizeof(LDMZ_FILE_MAP) >> 2, file);
	if (map_h.magic != LDMZ_MAGIC || map_h.version != LDMZ_VERSION) {
		fsFileClose(file);
		return NULL;
	}

	if ((buff = malloc(map_h.strtable_zlen)) == NULL) {
		fsFileClose(file);
		return NULL;
	}

	stringdata = malloc(map_h.strtable_len);
	map = malloc(sizeof(LDMZ_MAP));
	map->stringrefs = malloc(sizeof(LDMZ_REF) * (map_h.strtable_refs_len / sizeof(LDMZ_FILE_STRTABLE_REF)));
	map_r = malloc(map_h.strtable_refs_len);
	map_l = malloc(map_h.layer_zlen);
	object = malloc(map_h.objects * sizeof(LDMZ_OBJECT));
	map->object = malloc(sizeof(LDMZ_REF) * (map_h.objects));
	map->layer = malloc(sizeof(LDMZ_LAYER) * map_h.layers);

	map->stringdata = stringdata;
	map->cam_x = map->cam_y = 0;
	map->layers = map_h.layers;

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
	utilBlockToHostEndian((unsigned int *) map_r, map_h.strtable_refs_len / sizeof(LDMZ_FILE_STRTABLE_REF));
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

	if ((tmp = realloc(buff, map_h.objects * sizeof(LDMZ_FILE_OBJECT))) == NULL)
		goto error;		/* Down at the bottom of the function */
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

	if ((tmp = realloc(buff, map_h.layers * sizeof(LDMZ_FILE_LAYER))) == NULL)
		goto error;		/* Down at the bottom of the function */
	buff = tmp;
	fsFileRead(buff, map_h.layer_zlen, file);
	stbi_zlib_decode_buffer((void *)map_l, sizeof(LDMZ_LAYER) * map_h.layers, buff, map_h.layer_zlen);
	utilBlockToHostEndian((unsigned int *) map_l, (sizeof(LDMZ_LAYER) * map_h.layers) >> 2);
	for (i = 0; i < map_h.layers; i++) {
		map->layer[i].tilemap = NULL;
		map->layer[i].ts = NULL;
	}
	for (i = 0; i < map_h.layers; i++) {
		map->layer[i].offset_x = map_l[i].layer_offset_x;
		map->layer[i].offset_y = map_l[i].layer_offset_y;
		map->layer[i].tile_w = map_l[i].tile_w;
		map->layer[i].tile_h = map_l[i].tile_h;
		map->layer[i].ref = &map->stringrefs[map_l[i].prop_ref];

		if ((tmp = realloc(buff, map_l[i].layer_zlen)) == NULL)
			goto error;		/* Down at the bottom of the function */
		tmp = buff;
		fsFileRead(buff, map_l[i].layer_zlen, file);
		if (mapLayerPropGet(map, i, "tileset") == NULL) {
			if (mapPropGet(map, "tileset") == NULL)
				goto error;	/* Down at the bottom of the function */
			else
				if ((map->layer[i].ts = renderTilesheetLoad(mapPropGet(map, "tileset"),
				    map->layer[i].tile_w, map->layer[i].tile_h, PFORMAT_RGB5A1)) == NULL)
					goto error;	/* Down at the bottom of the function */
		} else
			if ((map->layer[i].ts = renderTilesheetLoad(mapPropGet(map, "tileset"),
			    map->layer[i].tile_w, map->layer[i].tile_h, PFORMAT_RGB5A1)) == NULL)
				goto error;		/* Down at the bottom of the function */

		if ((map->layer[i].tilemap = tilemapNew(TILEMAP_DEFAULT_INV_DIV, map->layer[i].ts, 
		    TILEMAP_DEFAULT_INV_DIV, map_l[i].layer_w, map_l[i].layer_h)) == NULL)
		    	goto error;		/* Down at the bottom of the function */
		renderTilemapCameraMove(map->layer[i].tilemap->render, map->cam_x + map->layer[i].offset_x, 
		    map->cam_y + map->layer[i].offset_y);
		fsFileRead(buff, map_l[i].layer_zlen, file);
		stbi_zlib_decode_buffer((void *) map->layer[i].tilemap->data, map_l[i].layer_w * map_l[i].layer_h, 
		    buff, map_h.layer_zlen);
		utilBlockToHostEndian(map->layer[i].tilemap->data, map_l[i].layer_w * map_l[i].layer_h);
	}
	
	free(map_l);
	free(buff);
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
