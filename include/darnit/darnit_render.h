#ifndef __DARNIT_RENDER_H__
#define	__DARNIT_RENDER_H__

#define	DARNIT_TILESHEET_FORMAT_ALPHA		0
#define	DARNIT_TILESHEET_FORMAT_RGBA		1


void *darnitRenderTilesheetLoad(void *handle, const char *fname, unsigned int wsq, unsigned int hsq, unsigned int target_format);
void *darnitRenderTilesheetNew(void *handle, int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format);
void darnitRenderTilesheetUpdate(void *tilesheet, int sheet_x, int sheet_y, int change_w, int change_h, void *data);
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
