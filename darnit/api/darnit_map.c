/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_map.c' - This file is part of libdarnit

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

LDMZ_MAP EXPORT_THIS *d_map_load(const char *fname) {
	return mapLoad(fname);
}


LDMZ_MAP EXPORT_THIS *d_map_unload(LDMZ_MAP *map) {
	return mapDestroy(map);
}


const char EXPORT_THIS *d_map_prop(LDMZ_REF *ref, const char *key) {
	return mapPropSearch(ref, key);
}


const char EXPORT_THIS *d_map_layer_prop(LDMZ_MAP *map, int layer, const char *key) {
	return mapLayerPropGet(map, layer, key);
}


void EXPORT_THIS d_map_camera_move(LDMZ_MAP *map, int x, int y) {
	mapCameraMove(map, x, y);

	return;
}
