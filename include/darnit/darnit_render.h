#ifndef __DARNIT_RENDER_H__
#define	__DARNIT_RENDER_H__

#define	DARNIT_RENDER_LOGIC_OP_NONE	0
#define	DARNIT_RENDER_LOGIC_OP_AND	1
#define	DARNIT_RENDER_LOGIC_OP_NAND	2
#define	DARNIT_RENDER_LOGIC_OP_OR	3
#define	DARNIT_RENDER_LOGIC_OP_NOR	4
#define	DARNIT_RENDER_LOGIC_OP_XOR	5

#define	DARNIT_SCALE_NEAREST		1
#define	DARNIT_SCALE_LINEAR		2


typedef void DARNIT_TILESHEET;
typedef void DARNIT_TILE;
typedef void DARNIT_LINE;
typedef void DARNIT_CIRCLE;
typedef void DARNIT_RECT;


DARNIT_TILESHEET *darnitRenderTilesheetLoad(const char *fname, unsigned int wsq, unsigned int hsq, unsigned int target_format);
DARNIT_TILESHEET *darnitRenderTilesheetNew(int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format);
void darnitRenderTilesheetUpdate(DARNIT_TILESHEET *tilesheet, int sheet_x, int sheet_y, int change_w, int change_h, void *data);
void darnitRenderTilesheetGeometricsGet(DARNIT_TILESHEET *tilesheet, int *w, int *h, int *tile_w, int *tile_h);
DARNIT_TILESHEET *darnitRenderTilesheetFree(DARNIT_TILESHEET *tilesheet);

void darnitRenderTileMove(DARNIT_TILE *tile_p, unsigned int tile, void *tilesheet, unsigned int x, unsigned int y);
void darnitRenderTileSetTilesheetCoord(DARNIT_TILE *tile_p, unsigned int tile, DARNIT_TILESHEET *tilesheet, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void darnitRenderTileSizeSet(DARNIT_TILE *buf, unsigned int tile, int w, int h);
void darnitRenderTileSet(DARNIT_TILE *tile_p, unsigned int tile, DARNIT_TILESHEET *tilesheet, unsigned int tile_ts);
void darnitRenderTileClear(DARNIT_TILE *tile_p, unsigned int tile);
void darnitRenderTileDraw(DARNIT_TILE *tile_p, void *tilesheet, unsigned int tiles);
DARNIT_TILE *darnitRenderTileAlloc(unsigned int tiles);
DARNIT_TILE *darnitRenderTileFree(DARNIT_TILE *tile_p);

DARNIT_LINE *darnitRenderLineAlloc(unsigned int lines, unsigned int line_w);
void darnitRenderLineMove(DARNIT_LINE *line_p, unsigned int line, int x1, int y1, int x2, int y2);
void darnitRenderLineDraw(DARNIT_LINE *line_p, int lines);
DARNIT_LINE *darnitRenderLineFree(DARNIT_LINE *line_p);

DARNIT_CIRCLE *darnitRenderCircleAlloc(unsigned int lines, unsigned int line_w);
void darnitRenderCircleMove(DARNIT_CIRCLE *circle_p, int x, int y, int radius);
void darnitRenderCircleDraw(DARNIT_CIRCLE *circle_p);
DARNIT_CIRCLE *darnitRenderCircleFree(DARNIT_CIRCLE *circle_p);

DARNIT_RECT *darnitRenderRectAlloc(unsigned int rects);
void darnitRenderRectSet(DARNIT_RECT *rect_p, unsigned int rect, int x1, int y1, int x2, int y2);
void darnitRenderRectDraw(DARNIT_RECT *rect_p, int rects);
DARNIT_RECT *darnitRenderRectFree(DARNIT_RECT *rect_p);

void darnitRenderLineGet(DARNIT_LINE *buf, unsigned int line, int *x, int *y, int *x2, int *y2);
void darnitRenderBegin();
void darnitRenderEnd();
void darnitRenderBlendingEnable();
void darnitRenderBlendingDisable();
void darnitRenderTint(float r, float g, float b, float a);
void darnitRenderTintGet(float *r, float *g, float *b, float *a);
void darnitRenderClearColorSet(unsigned char r, unsigned char g, unsigned char b);
void darnitRenderOffset(int x, int y);

void darnitRenderFadeIn(unsigned int time, float r, float g, float b);
void darnitRenderFadeOut(unsigned int time);
int darnitRenderFadeChanging();
void darnitRenderStateRestore();

int darnitRenderTilesheetAnimationApply(DARNIT_TILESHEET *tilesheet, const char *fname);
void darnitRenderTilesheetAnimationAnimate(DARNIT_TILESHEET *tilesheet);

void darnitRenderLogicOp(unsigned int logicop);
void darnitRenderTilesheetScaleAlgorithm(DARNIT_TILESHEET *tilesheet, unsigned int scaling);










/* Please don't use this function. I hate it <.< */
void darnitRenderTileBlit(void *tilesheet, unsigned int tile, int x, int y);


#endif
