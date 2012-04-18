#ifndef __DARNIT_H__
#define	__DARNIT_H__

#ifdef __cplusplus
extern "C" {
#endif

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
} TILEMAP;


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

void *darnitRenderTilesheetLoad(void *handle, const char *fname, unsigned int wsq, unsigned int hsq);
void *darnitRenderTilesheetFree(void *tilesheet);
void darnitRenderTileMove(void *tile_p, unsigned int tile, void *tilesheet, unsigned int x, unsigned int y);
void darnitRenderTileSet(void *tile_p, unsigned int tile, void *tilesheet, unsigned int tile_ts);
void darnitRenderTileClear(void *tile_p, unsigned int tile);
void darnitRenderTileDraw(void *tile_p, void *tilesheet, unsigned int tiles);
void *darnitRenderTileAlloc(unsigned int tiles);
void *darnitRenderTileFree(void *tile_p);
void darnitRenderBegin();
void darnitRenderEnd();
void darnitRenderBlendingEnable();
void darnitRenderBlendingDisable();
void darnitRenderTint(void *handle, float r, float g, float b, float a);
void darnitRenderOffset(void *handle, int x, int y);


/* SPRITE */

int darnitSpriteLoad(void *handle, void *box, const char *fname, int dir);
void darnitSpriteDelete(void *box, int sprite);
void darnitSpriteClear(void *box);
void darnitSpriteDirectionChange(void *box, int sprite, int dir);
void darnitSpriteMove(void *box, int sprite, int x, int y, int l);
void darnitSpriteBoxAnimate(void *box);
void darnitSpriteAnimationEnable(void *box, int sprite);
void darnitSpriteAnimationDisable(void *box, int sprite);
void darnitSpriteLayerDraw(void *box, int layer);
void *darnitSpriteBoxNew(unsigned int sprites);
void *darnitSpriteBoxFree(void *box);


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

TILEMAP darnitRenderTilemapCreate(void *handle, const char *fname, unsigned int invisibility_divider, void *tilesheet);
void darnitRenderTilemapCameraMove(TILEMAP tilemap, int cam_x, int cam_y);
void darnitRenderTilemap(void *handle, TILEMAP tilemap);
TILEMAP darnitRenderTilemapDelete(TILEMAP tilemap);



/* BBOX */

void *darnitBBoxNew(unsigned int size);
void *DarnerBBoxFree(void *bbox);
int darnitBBoxCollTest(void *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen);
int darnitBBoxEntryAdd(void *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void darnitBBoxEntryDelete(void *bbox, int key);
void darnitBBoxEntryMove(void *bbox, int key, unsigned int x, unsigned int y);
void darnitBBoxClear(void *bbox);

#ifdef __cplusplus
}
#endif

#endif
