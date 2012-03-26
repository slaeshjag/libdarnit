#ifndef __DARNER_H__
#define	__DARNER_H__


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
} DARNER_KEYS;

#define		BUTTON_ACCEPT		b
#define		BUTTON_CANCEL		a


typedef struct {
	float r, g, b, a;
} DARNER_SHADE_COLOR;


/* MAIN */

void *darnerInit(const char *wtitle);
void darnerLoop(void *handle);


/* INPUT */

DARNER_KEYS darnerButtonGet(void *handle);
void darnerButtonSet(void *handle, DARNER_KEYS buttons);


/* AUDIO */

void darnerMusicPlayMod(void *handle, const char *fname);
void darnerMusicPlayVorbis(void *handle, const char *fname);
int darnerMusicPlayCheck(void *handle);
void darnerMusicStop(void *handle);
int darnerSFXLoad(void *handle, const char *fname);
void darnerSFXUnload(void *handle, int sfx);
void darnerSFXClear(void *handle);
int darnerSFXPlay(void *handle, int sfx, int vol_l, int vol_r);
void darnerSFXVolumeSet(void *handle, int sfx_chan, int vol_l, int vol_r);


/* VIDEO */

void *darnerRenderTilesheetLoad(void *handle, const char *fname, unsigned int wsq, unsigned int hsq);
void *darnerRenderTilesheetFree(void *tilesheet);
void darnerRenderTileMove(void *tile_p, unsigned int tile, void *tilesheet, unsigned int x, unsigned int y);
void darnerRenderTileSet(void *tile_p, void *tilesheet, unsigned int tile, unsigned int tile_ts);
void darnerRenderTileDraw(void *tile_p, void *tilesheet, unsigned int tiles);
void *darnerRenderTileAlloc(unsigned int tiles);
void *darnerRenderTileFree(void *tile_p);
void darnerRenderBegin();
void darnerRenderEnd();
void darnerRenderBlendingEnable();
void darnerRenderBlendingDisable();


/* TEXT */

void *darnerFontLoad(void *handle, const char *fname, unsigned int glyph_w, unsigned int glyph_h, int line_spacing);
unsigned int darnerFontGetGlyphW(void *font);
unsigned int darnerFontGetGlyphH(void *font);
unsigned int darnerFontGetGlyphHS(void *font);
void darnerTextSurfaceReset(void *surface);
void *darnerTextSurfaceAlloc(void *font, unsigned int glyphs, unsigned int linelen, int x, int y);
void *darnerTextSurfaceFree(void *surface);
void darnerTextSurfaceCharAppend(void *surface, char c);
void darnerTextSurfaceStringAppend(void *surface, const char *string);
void darnerTextSurfaceDraw(void *surface);



/* MENU */

void *darnerMenuHorisontalCreate(void *handle, const char *options, int x, int y, void *font);
void *darnerMenuVerticalCreate(void *handle, const char *options, int x, int y, void *font, int menuw, int textskip_x);
void *darnerMenuVerticalShadeCreate(void *handle, int x, int y, int shade_w, int shade_h, int y_advance, int options, int skip_option);
void *darnerMenuSpinbuttonCreate(void *handle, const char *label, int x, int y, void *font, int step, int min, int max);
void darnerMenuVerticalShadeSelectionsReduceByOne(void *menu);
void darnerMenuShadeColorSet(void *menu, DARNER_SHADE_COLOR *color);
int darnerMenuHandle(void *handle, void *menu);
void darnerMenuSelectionWaitForNew(void *menu);
int darnerMenuPeek(void *menu);
int darnerMenuChangedCheck(void *menu);
void darnerMenuHide(void *menu);
void darnerMenuUnhide(void *menu);
void *darnerMenuDestroy(void *menu);



#endif
