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
