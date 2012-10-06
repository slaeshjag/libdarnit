#include "darnit.h"


TILEMAP_ENTRY EXPORT_THIS *darnitRenderTilemapNew(unsigned int invisibility_divider, void *tilesheet, unsigned int mask, int w, int h) {
	return tilemapNew(invisibility_divider, tilesheet, mask, w, h);
}


TILEMAP_ENTRY EXPORT_THIS *darnitRenderTilemapCreate(const char *fname, unsigned int invisibility_divider, void *tilesheet, unsigned int mask) {
	return tilemapLoad(fname, invisibility_divider, tilesheet, mask);
}


void EXPORT_THIS darnitRenderTilemapTileSet(TILEMAP_ENTRY *tilemap, int x, int y, int tile) {
	if (tilemap == NULL) return;
	renderTilemapTileSet(tilemap->render, x, y, tile);

	return;
}


void EXPORT_THIS darnitRenderTilemapRecalculate(TILEMAP_ENTRY *tilemap) {
	renderTilemapForceRecalc(tilemap->render);
	
	return;
}


void EXPORT_THIS darnitRenderTilemapCameraMove(TILEMAP_ENTRY *tilemap, int cam_x, int cam_y) {
	if (tilemap == NULL) return;
	renderTilemapCameraMove(tilemap->render, cam_x, cam_y);

	return;
}


void EXPORT_THIS darnitRenderTilemap(TILEMAP_ENTRY *tilemap) {
	if (tilemap == NULL) return;
	renderTilemapRender(tilemap->render);

	return;
}


TILEMAP_ENTRY EXPORT_THIS *darnitRenderTilemapDelete(TILEMAP_ENTRY *tilemap) {
	return tilemapFree(tilemap);
}
