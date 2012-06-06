#ifndef __DARNIT_RENDER_H__
#define	__DARNIT_RENDER_H__

void *darnitRenderTilesheetLoad(void *handle, const char *fname, unsigned int wsq, unsigned int hsq, unsigned int target_format);
void *darnitRenderTilesheetFree(void *handle, void *tilesheet);
void darnitRenderTileMove(void *tile_p, unsigned int tile, void *tilesheet, unsigned int x, unsigned int y);
void darnitRenderTileSetTilesheetCoord(void *tile_p, unsigned int tile, void *tilesheet, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void darnitRenderTileSet(void *tile_p, unsigned int tile, void *tilesheet, unsigned int tile_ts);
void darnitRenderTileClear(void *tile_p, unsigned int tile);
void darnitRenderTileDraw(void *tile_p, void *tilesheet, unsigned int tiles);
void *darnitRenderTileAlloc(unsigned int tiles);
void *darnitRenderTileFree(void *tile_p);
void darnitRenderBegin();
void darnitRenderEnd();
void darnitRenderBlendingEnable(void *handle);
void darnitRenderBlendingDisable(void *handle);
void darnitRenderTint(void *handle, float r, float g, float b, float a);
void darnitRenderOffset(void *handle, int x, int y);

#endif
