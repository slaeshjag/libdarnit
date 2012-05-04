#ifndef __DARNIT_H__
#define	__DARNIT_H__

#ifdef __cplusplus
extern "C" {
#endif


#define		DARNIT_PFORMAT_RGBA8		0
#define		DARNIT_PFORMAT_RGBA4		1
#define		DARNIT_PFORMAT_RGB5A1		2


typedef struct {
	unsigned int	left	: 1;
	unsigned int	right	: 1;
	unsigned int	up	: 1;
	unsigned int	down	: 1;
	unsigned int	x	: 1;
	unsigned int	y	: 1;
	unsigned int	a	: 1;
	unsigned int	b	: 1;
	unsigned int	start	: 1;
	unsigned int	select	: 1;
	unsigned int	l	: 1;
	unsigned int	r	: 1;
	unsigned int	lmb	: 1;
	unsigned int	rmb	: 1;
} DARNIT_KEYS;


typedef struct {
	unsigned int	x	: 16;
	unsigned int	y	: 16;
	signed int	wheel	: 32;
} DARNIT_MOUSE;


typedef struct {
	unsigned int			up;
	unsigned int			down;
	unsigned int			left;
	unsigned int			right;
	unsigned int			x;
	unsigned int			y;
	unsigned int			a;
	unsigned int			b;
	unsigned int			start;
	unsigned int			select;
	unsigned int			l;
	unsigned int			r;
} DARNIT_INPUT_MAP;


#define		BUTTON_ACCEPT		b
#define		BUTTON_CANCEL		a


typedef struct {
	float r, g, b, a;
} DARNIT_SHADE_COLOR;



typedef struct {
	int		w;
	int		h;
	unsigned int	*data;
	void		*render;
} DARNIT_TILEMAP;


/* MAIN */

void *darnitInit(const char *wtitle);
void *darnitInitCustom(const char *wtitle, int win_w, int win_h, int fullscreen);
void darnitLoop(void *handle);
unsigned int darnitTimeGet();


/* INPUT */

DARNIT_KEYS darnitButtonGet(void *handle);
void darnitButtonSet(void *handle, DARNIT_KEYS buttons);
DARNIT_KEYS darnitButtonZero();
void darnitInputGrab();
void darnitInputUngrab();
DARNIT_MOUSE darnitMouseGet(void *handle);
void darnitButtonMappingReset(void *handle);
void darnitButtonMappingSet(void *handle, DARNIT_INPUT_MAP map);
DARNIT_INPUT_MAP darnitButtonMappingGet(void *handle);



/* AUDIO */

void darnitMusicPlayTracked(void *handle, const char *fname);
void darnitMusicPlayVorbis(void *handle, const char *fname);
int darnitMusicPlayCheck(void *handle);
void darnitMusicVolSet(void *handle, int vol);
void darnitMusicStop(void *handle);
int darnitSFXLoad(void *handle, const char *fname);
void darnitSFXUnload(void *handle, int sfx);
void darnitSFXClear(void *handle);
unsigned int darnitSFXPlay(void *handle, int sfx, int vol_l, int vol_r);
void darnitSFXVolumeSet(void *handle, unsigned int sfx_p_index, int vol_l, int vol_r);


/* VIDEO */

void *darnitRenderTilesheetLoad(void *handle, const char *fname, unsigned int wsq, unsigned int hsq, unsigned int target_format);
void *darnitRenderTilesheetFree(void *tilesheet);
void darnitRenderTileMove(void *tile_p, unsigned int tile, void *tilesheet, unsigned int x, unsigned int y);
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


/* SPRITE */

void *darnitSpriteLoad(void *handle, const char *fname, int dir, unsigned int target_format);
void darnitSpriteDelete(void *sprite);
void darnitSpriteDirectionChange(void *sprite, int dir);
void darnitSpriteMove(void *sprite, int x, int y);
void darnitSpriteAnimationEnable(void *sprite);
void darnitSpriteAnimationPause(void *sprite);
void darnitSpriteAnimationDisable(void *sprite);
void darnitSpriteDraw(void *sprite);


/* TEXT */

void *darnitFontLoad(void *handle, const char *fname, unsigned int glyph_w, unsigned int glyph_h, int line_spacing);
unsigned int darnitFontGetGlyphW(void *font);
unsigned int darnitFontGetGlyphH(void *font);
unsigned int darnitFontGetGlyphHS(void *font);
void darnitTextSurfaceReset(void *surface);
void *darnitTextSurfaceAlloc(void *font, unsigned int glyphs, unsigned int linelen, int x, int y);
void *darnitTextSurfaceFree(void *surface);
void darnitTextSurfaceCharAppend(void *surface, char c);
void darnitTextSurfaceStringAppend(void *surface, const char *string);
void darnitTextSurfaceDraw(void *surface);



/* MENU */

void *darnitMenuHorizontalCreate(void *handle, const char *options, int x, int y, void *font, int max_w);
void *darnitMenuVerticalCreate(void *handle, const char *options, int x, int y, void *font, int menuw, int textskip_x, int max_h);
void *darnitMenuVerticalShadeCreate(void *handle, int x, int y, int shade_w, int shade_h, int y_advance, int options, int skip_option, int max_h);
void *darnitMenuSpinbuttonCreate(void *handle, const char *label, int x, int y, void *font, int step, int min, int max);
void darnitMenuVerticalShadeSelectionsReduceByOne(void *menu);
void darnitMenuVerticalShadeSelectionsIncreaseByOne(void *menu);
void darnitMenuShadeColorSet(void *menu, float r, float g, float b, float a);
int darnitMenuHandle(void *handle, void *menu);
void darnitMenuSelectionWaitForNew(void *menu);
int darnitMenuPeek(void *menu);
int darnitMenuChangedCheck(void *menu);
void darnitMenuHide(void *menu);
void darnitMenuUnhide(void *menu);
void *darnitMenuDestroy(void *menu);



/* TILEMAPS */

DARNIT_TILEMAP darnitRenderTilemapCreate(void *handle, const char *fname, unsigned int invisibility_divider, void *tilesheet);
void darnitRenderTilemapTileSet(DARNIT_TILEMAP tilemap, int x, int y, int tile);
void darnitRenderTilemapCameraMove(DARNIT_TILEMAP tilemap, int cam_x, int cam_y);
void darnitRenderTilemap(void *handle, DARNIT_TILEMAP tilemap);
DARNIT_TILEMAP darnitRenderTilemapDelete(DARNIT_TILEMAP tilemap);



/* BBOX */

void *darnitBBoxNew(unsigned int size);
void *DarnerBBoxFree(void *bbox);
int darnitBBoxCollTest(void *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen);
int darnitBBoxEntryAdd(void *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void darnitBBoxEntryDelete(void *bbox, int key);
void darnitBBoxEntryMove(void *bbox, int key, unsigned int x, unsigned int y);
void darnitBBoxEntryResize(void *bbox, int key, unsigned int w, unsigned int h);
void darnitBBoxClear(void *bbox);



/* DYNLIB */
void *darnitDynlibOpen(const char *fname);
void *darnitDynlibGet(void *lib, const char *symbol);
void *darnitDynlibClose(void *lib);



/* SOCKET */
void *darnitSocketConnect(const char *host, int port);
int darnitSocketRecv(void *sock, char *buff, int len);
int darnitSocketSend(void *sock, char *buff, int len);
void *darnitSocketClose(void *sock);


#ifdef __cplusplus
}
#endif

#endif
