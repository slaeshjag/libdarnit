#include "darner.h"

/* libDarner glue */


/***************************/
/******** MAIN *************/
/***************************/


void *darnerInit(const char *wtitle) {
	DARNER *d;

	if ((d = malloc(sizeof(DARNER))) == NULL) {
		fprintf(stderr, "libDarner: Error: Unable to malloc(%i)\n", (int) sizeof(DARNER));
		return d;
	}

	videoInit(d, wtitle);
	inputInit(d);
	audioInit(d);
	spriteInit(d, SPRITE_PREALLOC_DEF);

	return d;
}


void darnerLoop(void *handle) {
	DARNER *d = handle;

	spriteAnimate(d);
	videoLoop(d);
	inputPoll(d);

	return;
}



/*************************/
/********* INPUT *********/
/*************************/


unsigned int darnerButtonGet(void *handle, unsigned int button) {
	DARNER *d = handle;

	return (d->input.key ^ d->input.keypending) & button;
}


void darnerButtonSet(void *handle, unsigned int button) {
	DARNER *d = handle;

	d->input.keypending |= button;

	return;
}


void darnerInputGrab() {
	SDL_WM_GrabInput(SDL_GRAB_ON);

	return;
}


void darnerInputUngrab() {
	SDL_WM_GrabInput(SDL_GRAB_OFF);

	return;
}


DARNER_MOUSE darnerMouseGet(void *handle) {
	DARNER *d = handle;

	return d->input.mouse;
}


/***************************/
/********* AUDIO ***********/
/***************************/


void darnerMusicPlayMod(void *handle, const char *fname) {
	audioMusicPlayMod(handle, fname);

	return;
}


void darnerMusicPlayVorbis(void *handle, const char *fname) {
	audioMusicPlayVorbis(handle, fname);

	return;
}


int darnerMusicPlayCheck(void *handle) {
	DARNER *d = handle;

	return (d->audio.music.modfile == NULL && d->audio.music.vorbis == NULL) ? -1 : 0;
}


void darnerMusicStop(void *handle) {
	audioMusicStop(handle);

	return;
}


int darnerSFXLoad(void *handle, const char *fname) {
	return audioSFXLoad(handle, fname);
}


void darnerSFXUnload(void *handle, int sfx) {
	audioSFXFree(handle, sfx);

	return;
}


void darnerSFXClear(void *handle) {
	audioSFXClear(handle);

	return;
}


int darnerSFXPlay(void *handle, int sfx, int vol_l, int vol_r) {
	return audioSFXPlay(handle, sfx, vol_l, vol_r);
}


void darnerSFXVolumeSet(void *handle, int sfx_chan, int vol_l, int vol_r) {
	DARNER *d = handle;
	
	if (sfx_chan == -1)
		return;

	d->audio.sfxchan[sfx_chan].lvol = vol_l;
	d->audio.sfxchan[sfx_chan].rvol = vol_r;
	
	return;
}



/**************************/
/********* VIDEO **********/
/**************************/


void *darnerRenderTilesheetLoad(void *handle, const char *fname, unsigned int wsq, unsigned int hsq) {
	DARNER *d = handle;
	
	return renderTilesheetLoad(d, fname, wsq, hsq);
}


void *darnerRenderTilesheetFree(void *tilesheet) {
	return renderTilesheetFree(tilesheet);
}


void darnerRenderTileMove(void *tile_p, unsigned int tile, void *tilesheet, unsigned int x, unsigned int y) {
	TILE_CACHE *cache = tile_p;
	cache = &cache[tile];

	renderCalcTilePosCache(cache, tilesheet, x, y);

	return;
}


/* tile_ts is the tile index in the tilesheet */
void darnerRenderTileSet(void *tile_p, void *tilesheet, unsigned int tile, unsigned int tile_ts) {
	TILE_CACHE *cache = tile_p;
	cache = &cache[tile];
	
	renderCalcTileCache(cache, tilesheet, tile_ts);

	return;
}


void darnerRenderTileDraw(void *tile_p, void *tilesheet, unsigned int tiles) {
	renderCache(tile_p, tilesheet, tiles);

	return;
}


void *darnerRenderTileAlloc(unsigned int tiles) {
	TILE_CACHE *cache;
	int i, j;
	float *arr;

	if ((cache = malloc(sizeof(TILE_CACHE)*tiles)) == NULL)
		return NULL;
	
	for (i = 0; i < tiles; i++) {
		arr = (void *) &cache[i];
		for (j = 0; j < 24; j++)
			arr[j] = 0.0f;
	}

	return cache;
}


void *darnerRenderTileFree(void *tile_p) {
	free(tile_p);

	return NULL;
}



void darnerRenderBegin() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}


void darnerRenderEnd() {
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}


void darnerRenderBlendingEnable() {
	glEnable(GL_BLEND);
}


void darnerRenderBlendingDisable() {
	glDisable(GL_BLEND);
}


void darnerRenderTint(float r, float g, float b, float a) {
	glColor4f(r, g, b, a);

	return;
}


void darnerRenderOffset(void *handle, int x, int y) {
	DARNER *d = handle;
	glLoadIdentity();
	glTranslatef(d->video.swgran * x, d->video.shgran * y, 0.0f);
	d->video.offset_x = x;
	d->video.offset_y = y * -1;

	return;
}


/*****************************/
/****** SPRITE ***************/
/*****************************/


int darnerSpriteLoad(void *handle, const char *fname) {
	return spriteLoad(handle, fname, 0);
}


void darnerSpriteDelete(void *handle, int sprite) {
	spriteDelete(handle, sprite);

	return;
}


void darnerSpriteClear(void *handle) {
	spriteClear(handle);

	return;
}


void darnerSpriteDirectionChange(void *handle, int sprite, int dir) {
	spriteChangeDirection(handle, sprite, dir);

	return;
}


void darnerSpriteMove(void *handle, int sprite, int x, int y, int l) {
	spriteTeleport(handle, sprite, x, y, l);

	return;
}


void darnerSpriteAnimationEnable(void *handle, int sprite) {
	spriteEnableAnimation(handle, sprite);

	return;
}


void darnerSpriteAnimationDisable(void *handle, int sprite) {
	spriteDisableAnimation(handle, sprite);

	return;
}


void darnerSpriteLayerDraw(void *handle, int layer) {
	spriteDrawLayer(handle, layer);

	return;
}



/****************************/
/********** TEXT ************/
/****************************/


void *darnerFontLoad(void *handle, const char *fname, unsigned int glyph_w, unsigned int glyph_h, int line_spacing) {
	return textLoadFont(handle, fname, glyph_w, glyph_h, line_spacing);
}


unsigned int darnerFontGetGlyphW(void *font) {
	return textFontGetW(font);
}


unsigned int darnerFontGetGlyphH(void *font) {
	return textFontGetH(font);
}


unsigned int darnerFontGetGlyphHS(void *font) {
	return textFontGetHS(font);
}


void darnerTextSurfaceReset(void *surface) {
	textResetSurface(surface);

	return;
}


void *darnerTextSurfaceAlloc(void *font, unsigned int glyphs, unsigned int linelen, int x, int y) {
	return textMakeRenderSurface(glyphs, font, linelen, x, y);
}


void *darnerTextSurfaceFree(void *surface) {
	return textSurfaceDestroy(surface);
}


void darnerTextSurfaceCharAppend(void *surface, char c) {
	textSurfaceAppendChar(surface, c);

	return;
}


void darnerTextSurfaceStringAppend(void *surface, const char *string) {
	textSurfaceAppendString(surface, string);

	return;
}


void darnerTextSurfaceDraw(void *surface) {
	textRender(surface);
	
	return;
}




/**************************/
/****** MENUTK ************/
/**************************/


void *darnerMenuHorisontalCreate(void *handle, const char *options, int x, int y, void *font) {
	return menutkHorisontalCreate(handle, options, x, y, font, 0);
}


void *darnerMenuVerticalCreate(void *handle, const char *options, int x, int y, void *font, int menuw, int textskip_x) {
	return menutkVerticalCreate(handle, options, x, y, font, menuw, textskip_x, 0);
}


void *darnerMenuVerticalShadeCreate(void *handle, int x, int y, int shade_w, int shade_h, int y_advance, int options, int skip_option) {
	return menutkVerticalShadeCreate(handle, x, y, shade_h, y_advance, options, shade_w, 0, skip_option);
}


void *darnerMenuSpinbuttonCreate(void *handle, const char *label, int x, int y, void *font, int step, int min, int max) {
	return menutkSpinbuttonCreate(handle, label, x, y, font, step, min, max);
}


void darnerMenuVerticalShadeSelectionsReduceByOne(void *menu) {
	menutkVerticalShadeReduceSelectionsByOne(menu);

	return;
}


void darnerMenuShadeColorSet(void *menu, void *color) {
	MENUTK_COLOR *col = (void *) color;
	MENUTK_ENTRY *m = menu;

	m->hl.color[0] = col[0];
	m->hl.color[1] = col[1];
	m->hl.color[2] = col[2];
	m->hl.color[3] = col[3];

	return;
}


int darnerMenuHandle(void *handle, void *menu) {
	return menutkMenuRoutine(handle, menu);
}


void darnerMenuSelectionWaitForNew(void *menu) {
	menutkWaitForNewSelection(menu);

	return;
}


int darnerMenuPeek(void *menu) {
	return menutkPeek(menu);
}


int darnerMenuChangedCheck(void *menu) {
	return menutkGetIfChanged(menu);
}


void darnerMenuHide(void *menu) {
	menutkMenuHide(menu);

	return;
}


void darnerMenuUnhide(void *menu) {
	menutkMenuUnhide(menu);

	return;
}


void *darnerMenuDestroy(void *menu) {
	return menutkDestroy(menu);
}
