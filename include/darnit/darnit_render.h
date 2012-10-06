#ifndef __DARNIT_RENDER_H__
#define	__DARNIT_RENDER_H__

void *darnitRenderTilesheetLoad(const char *fname, unsigned int wsq, unsigned int hsq, unsigned int target_format);
void *darnitRenderTilesheetNew(int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format);
void darnitRenderTilesheetUpdate(void *tilesheet, int sheet_x, int sheet_y, int change_w, int change_h, void *data);
void *darnitRenderTilesheetFree(void *tilesheet);
void darnitRenderTileMove(void *tile_p, unsigned int tile, void *tilesheet, unsigned int x, unsigned int y);
void darnitRenderTileSetTilesheetCoord(void *tile_p, unsigned int tile, void *tilesheet, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void darnitRenderTileSet(void *tile_p, unsigned int tile, void *tilesheet, unsigned int tile_ts);
void darnitRenderTileClear(void *tile_p, unsigned int tile);
void darnitRenderTileDraw(void *tile_p, void *tilesheet, unsigned int tiles);
void *darnitRenderTileAlloc(unsigned int tiles);
void *darnitRenderTileFree(void *tile_p);
void darnitRenderBegin();
void darnitRenderEnd();
void darnitRenderBlendingEnable();
void darnitRenderBlendingDisable();
void darnitRenderTint(float r, float g, float b, float a);
void darnitRenderClearColorSet(unsigned char r, unsigned char g, unsigned char b);
void darnitRenderOffset(int x, int y);

void darnitRenderFadeIn(unsigned int time, float r, float g, float b);
void darnitRenderFadeOut(unsigned int time);
int darnitRenderFadeChanging();
void darnitRenderStateRestore();

int darnitRenderTilesheetAnimationApply(void *tilesheet, const char *fname);
void darnitRenderTilesheetAnimationAnimate(void *tilesheet);

#endif
