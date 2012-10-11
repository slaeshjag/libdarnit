#include "darnit.h"

LDMZ_MAP EXPORT_THIS *darnitMapLoad(const char *fname) {
	return mapLoad(fname);
}


LDMZ_MAP EXPORT_THIS *darnitMapUnload(LDMZ_MAP *map) {
	return mapDestroy(map);
}


const char EXPORT_THIS *darnitMapPropGet(LDMZ_REF *ref, const char *key) {
	return mapPropSearch(ref, key);
}


const char EXPORT_THIS *darnitMapLayerPropGet(LDMZ_MAP *map, int layer, const char *key) {
	return mapLayerPropGet(map, layer, key);
}


void EXPORT_THIS darnitMapCameraMove(LDMZ_MAP *map, int x, int y) {
	mapCameraMove(map, x, y);

	return;
}
