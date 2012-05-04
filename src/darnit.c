#include "darnit.h"

/* libDarnit glue */


/***************************/
/******** MAIN *************/
/***************************/


void EXPORT_THIS *darnitInit(const char *wtitle) {
	DARNIT *d;

	if ((d = malloc(sizeof(DARNIT))) == NULL) {
		fprintf(stderr, "libDarnit: Error: Unable to malloc(%i)\n", (int) sizeof(DARNIT));
		return d;
	}

	#ifdef PANDORA
	if (videoInit(d, wtitle, 800, 480, 1) < 0);
	#else
	if (videoInit(d, wtitle, 800, 480, 0) < 0);
	#endif
	else if (inputInit(d) < 0);
	else if (audioInit(d) < 0); 
	else return d;

	free(d);

	return NULL;
}


void EXPORT_THIS *darnitInitCustom(const char *wtitle, int win_w, int win_h, int fullscreen) {
	DARNIT *d;

	if ((d = malloc(sizeof(DARNIT))) == NULL) {
		fprintf(stderr, "libDarnit: Error: Unable to malloc(%i)\n", (int) sizeof(DARNIT));
		return d;
	}

	if (videoInit(d, wtitle, win_w, win_h, fullscreen) < 0);
	else if (inputInit(d) < 0);
	else if (audioInit(d) < 0);
	else return d;

	free(d);
	return NULL;
}


void EXPORT_THIS darnitLoop(void *handle) {
	DARNIT *d = handle;

	videoLoop(d);
	inputPoll(d);

	return;
}


unsigned int EXPORT_THIS darnitTimeGet() {
	return SDL_GetTicks();
}



/*************************/
/********* INPUT *********/
/*************************/


unsigned int EXPORT_THIS darnitButtonGet(void *handle) {
	DARNIT *d = handle;

	return (d->input.key ^ d->input.keypending);
}


unsigned int EXPORT_THIS darnitButtonZero() {
	return 0;
}


void EXPORT_THIS darnitButtonSet(void *handle, unsigned int button) {
	DARNIT *d = handle;

	d->input.keypending |= button;

	return;
}


void EXPORT_THIS darnitButtonMappingReset(void *handle) {
	inputKeymapReset(handle);

	return;
}


void EXPORT_THIS darnitButtonMappingSet(void *handle, INPUT_MAP map) {
	DARNIT *d = handle;

	d->input.map = map;

	return;
}


INPUT_MAP EXPORT_THIS darnitButtonMappingGet(void *handle) {
	DARNIT *d = handle;

	return d->input.map;
}


void EXPORT_THIS darnitInputGrab() {
	SDL_WM_GrabInput(SDL_GRAB_ON);

	return;
}


void EXPORT_THIS darnitInputUngrab() {
	SDL_WM_GrabInput(SDL_GRAB_OFF);

	return;
}


DARNIT_MOUSE EXPORT_THIS darnitMouseGet(void *handle) {
	DARNIT *d = handle;

	return d->input.mouse;
}


/***************************/
/********* AUDIO ***********/
/***************************/


void EXPORT_THIS darnitMusicPlayTracked(void *handle, const char *fname) {
	audioMusicPlayMod(handle, fname);

	return;
}


void EXPORT_THIS darnitMusicPlayVorbis(void *handle, const char *fname) {
	audioMusicPlayVorbis(handle, fname);

	return;
}


int EXPORT_THIS darnitMusicPlayCheck(void *handle) {
	DARNIT *d = handle;

	return (d->audio.music.modfile == NULL && d->audio.music.vorbis == NULL) ? -1 : 0;
}


void EXPORT_THIS darnitMusicVolSet(void *handle, int vol) {
	DARNIT *d = handle;

	d->audio.musicvol = vol;

	return;
}


void EXPORT_THIS darnitMusicStop(void *handle) {
	audioMusicStop(handle);

	return;
}


int EXPORT_THIS darnitSFXLoad(void *handle, const char *fname) {
	return audioSFXLoad(handle, fname);
}


void EXPORT_THIS darnitSFXUnload(void *handle, int sfx) {
	audioSFXFree(handle, sfx);

	return;
}


void EXPORT_THIS darnitSFXClear(void *handle) {
	audioSFXClear(handle);

	return;
}


unsigned int EXPORT_THIS darnitSFXPlay(void *handle, int sfx, int vol_l, int vol_r) {
	return audioSFXPlay(handle, sfx, vol_l, vol_r);
}


void EXPORT_THIS darnitSFXVolumeSet(void *handle, unsigned int sfx, int vol_l, int vol_r) {
	DARNIT *d = handle;
	int i;

	for (i = 0; i < AUDIO_SFX_CHANNELS; i++)
		if (d->audio.sfxchan[i].key == sfx) {
			d->audio.sfxchan[sfx].lvol = vol_l;
			d->audio.sfxchan[sfx].rvol = vol_r;
			break;
		}
	
	return;
}



/**************************/
/********* VIDEO **********/
/**************************/


void EXPORT_THIS *darnitRenderTilesheetLoad(void *handle, const char *fname, unsigned int wsq, unsigned int hsq, unsigned int target_format) {
	DARNIT *d = handle;
	
	return renderTilesheetLoad(d, fname, wsq, hsq, target_format);
}


void EXPORT_THIS *darnitRenderTilesheetFree(void *tilesheet) {
	return renderTilesheetFree(tilesheet);
}


void EXPORT_THIS darnitRenderTileMove(DARNIT_RENDER_BUFFER *buf, unsigned int tile, void *tilesheet, unsigned int x, unsigned int y) {
	TILE_CACHE *cache;

	if (buf->tiles <= tile)
		return;
	
	cache = &buf->tc[tile];
	renderCalcTilePosCache(cache, tilesheet, x, y);

	return;
}


/* tile_ts is the tile index in the tilesheet */
void EXPORT_THIS darnitRenderTileSet(DARNIT_RENDER_BUFFER *buf, unsigned int tile, void *tilesheet, unsigned int tile_ts) {
	TILE_CACHE *cache;

	if (buf->tiles <= tile)
		return;
	cache = &buf->tc[tile];
	renderCalcTileCache(cache, tilesheet, tile_ts);

	return;
}


void EXPORT_THIS darnitRenderTileClear(DARNIT_RENDER_BUFFER *buf, unsigned int tile) {
	float *ptr;
	int i;

	if (tile >= buf->tiles)
		return;
	ptr = (float *) &buf->tc[tile];
	for (i = 0; i < 24; i++)
		ptr[i] = 0.0f;
	
	return;
}


void EXPORT_THIS darnitRenderTileDraw(DARNIT_RENDER_BUFFER *buf, void *tilesheet, unsigned int tiles) {
	if (buf->tiles > tiles)
		tiles = buf->tiles;
	renderCache(buf->tc, tilesheet, tiles);

	return;
}


void EXPORT_THIS *darnitRenderTileAlloc(unsigned int tiles) {
	DARNIT_RENDER_BUFFER *buf;
	int i, j;
	float *arr;

	buf = malloc(sizeof(DARNIT_RENDER_BUFFER));
	if ((buf->tc = malloc(sizeof(TILE_CACHE)*tiles)) == NULL)
		return NULL;
	
	for (i = 0; i < tiles; i++) {
		arr = (void *) &buf->tc[i];
		for (j = 0; j < 24; j++)
			arr[j] = 0.0f;
	}

	return buf;
}


void EXPORT_THIS *darnitRenderTileFree(DARNIT_RENDER_BUFFER *buf) {
	free(buf->tc);
	free(buf);

	return NULL;
}



void EXPORT_THIS darnitRenderBegin() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}


void EXPORT_THIS darnitRenderEnd() {
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}


void EXPORT_THIS darnitRenderBlendingEnable(void *handle) {
	DARNIT *d = handle;
	
	d->video.blend = 1;
	glEnable(GL_BLEND);
}


void EXPORT_THIS darnitRenderBlendingDisable(void *handle) {
	DARNIT *d = handle;

	d->video.blend = 0;
	glDisable(GL_BLEND);
}


void EXPORT_THIS darnitRenderTint(void *handle, float r, float g, float b, float a) {
	DARNIT *d = handle;
	glColor4f(r, g, b, a);
	d->video.tint_r = r;
	d->video.tint_g = g;
	d->video.tint_b = b;
	d->video.tint_a = a;
	

	return;
}


void EXPORT_THIS darnitRenderOffset(void *handle, int x, int y) {
	DARNIT *d = handle;
	glLoadIdentity();
	glTranslatef(d->video.swgran * x * -1, d->video.shgran * y, 0.0f);
	d->video.offset_x = x;
	d->video.offset_y = y * -1;

	return;
}


/*****************************/
/****** SPRITE ***************/
/*****************************/


void EXPORT_THIS *darnitSpriteLoad(void *handle, const char *fname, int dir, unsigned int target_format) {
	return spriteLoad(handle, fname, dir, target_format);
}


void EXPORT_THIS *darnitSpriteDelete(void *sprite) {
	return spriteDelete(sprite);
}


void EXPORT_THIS darnitSpriteDirectionChange(void *sprite, int dir) {
	spriteChangeDirection(sprite, dir);

	return;
}


void EXPORT_THIS darnitSpriteMove(void *sprite, int x, int y) {
	spriteTeleport(sprite, x, y);

	return;
}


void EXPORT_THIS darnitSpriteAnimationEnable(void *sprite) {
	spriteEnableAnimation(sprite);

	return;
}


void EXPORT_THIS darnitSpriteAnimationPause(void *sprite) {
	spritePauseAnimation(sprite);

	return;
}


void EXPORT_THIS darnitSpriteAnimationDisable(void *sprite) {
	spriteDisableAnimation(sprite);

	return;
}


void EXPORT_THIS darnitSpriteDraw(void *sprite) {
	spriteDraw(sprite);

	return;
}


/****************************/
/********** TEXT ************/
/****************************/


void EXPORT_THIS *darnitFontLoad(void *handle, const char *fname, unsigned int glyph_w, unsigned int glyph_h, int line_spacing) {
	return textLoadFont(handle, fname, glyph_w, glyph_h, line_spacing);
}


unsigned int EXPORT_THIS darnitFontGetGlyphW(void *font) {
	return textFontGetW(font);
}


unsigned int EXPORT_THIS darnitFontGetGlyphH(void *font) {
	return textFontGetH(font);
}


unsigned int EXPORT_THIS darnitFontGetGlyphHS(void *font) {
	return textFontGetHS(font);
}


void EXPORT_THIS darnitTextSurfaceReset(void *surface) {
	textResetSurface(surface);

	return;
}


void EXPORT_THIS *darnitTextSurfaceAlloc(void *font, unsigned int glyphs, unsigned int linelen, int x, int y) {
	return textMakeRenderSurface(glyphs, font, linelen, x, y);
}


void EXPORT_THIS *darnitTextSurfaceFree(void *surface) {
	return textSurfaceDestroy(surface);
}


void EXPORT_THIS darnitTextSurfaceCharAppend(void *surface, char c) {
	textSurfaceAppendChar(surface, c);

	return;
}


void EXPORT_THIS darnitTextSurfaceStringAppend(void *surface, const char *string) {
	textSurfaceAppendString(surface, string);

	return;
}


void EXPORT_THIS darnitTextSurfaceDraw(void *surface) {
	textRender(surface);
	
	return;
}




/**************************/
/****** MENUTK ************/
/**************************/


void EXPORT_THIS *darnitMenuHorizontalCreate(void *handle, const char *options, int x, int y, void *font, int max_w) {
	return menutkHorisontalCreate(handle, options, x, y, font, 0);
}


void EXPORT_THIS *darnitMenuVerticalCreate(void *handle, const char *options, int x, int y, void *font, int menuw, int textskip_x, int max_h) {
	return menutkVerticalCreate(handle, options, x, y, font, menuw, textskip_x, 0);
}


void EXPORT_THIS *darnitMenuVerticalShadeCreate(void *handle, int x, int y, int shade_w, int shade_h, int y_advance, int options, int skip_option, int max_h) {
	return menutkVerticalShadeCreate(handle, x, y, shade_h, y_advance, options, shade_w, 0, skip_option);
}


void EXPORT_THIS *darnitMenuSpinbuttonCreate(void *handle, const char *label, int x, int y, void *font, int step, int min, int max) {
	return menutkSpinbuttonCreate(handle, label, x, y, font, step, min, max);
}


void EXPORT_THIS darnitMenuVerticalShadeSelectionsReduceByOne(void *menu) {
	menutkVerticalShadeReduceSelectionsByOne(menu);

	return;
}


void EXPORT_THIS darnitMenuVerticalShadeSelectionsIncreaseByOne(void *menu) {
	menutkVerticalShadeIncreaseSelectionsByOne(menu);
	
	return;
}


void EXPORT_THIS darnitMenuShadeColorSet(void *menu, float r, float g, float b, float a) {
	MENUTK_COLOR col;
	MENUTK_ENTRY *m = menu;

	col.r = r, col.g = g, col.b = b, col.a = a;

	m->hl.color[0] = col;
	m->hl.color[1] = col;
	m->hl.color[2] = col;
	m->hl.color[3] = col;

	return;
}


int EXPORT_THIS darnitMenuHandle(void *handle, void *menu) {
	return menutkMenuRoutine(handle, menu);
}


void EXPORT_THIS darnitMenuSelectionWaitForNew(void *menu) {
	menutkWaitForNewSelection(menu);

	return;
}


int EXPORT_THIS darnitMenuPeek(void *menu) {
	return menutkPeek(menu);
}


int EXPORT_THIS darnitMenuChangedCheck(void *menu) {
	return menutkGetIfChanged(menu);
}


void EXPORT_THIS darnitMenuHide(void *menu) {
	menutkMenuHide(menu);

	return;
}


void EXPORT_THIS darnitMenuUnhide(void *menu) {
	menutkMenuUnhide(menu);

	return;
}


void EXPORT_THIS *darnitMenuDestroy(void *menu) {
	return menutkDestroy(menu);
}



/***************************/
/******* TILEMAPS **********/
/***************************/


TILEMAP_ENTRY EXPORT_THIS darnitRenderTilemapCreate(void *handle, const char *fname, unsigned int invisibility_divider, void *tilesheet) {
	return tilemapLoad(handle, fname, invisibility_divider, tilesheet);
}


void EXPORT_THIS darnitRenderTilemapTileSet(TILEMAP_ENTRY tilemap, int x, int y, int tile) {
	renderTilemapTileSet(tilemap.render, x, y, tile);

	return;
}


void EXPORT_THIS darnitRenderTilemapCameraMove(TILEMAP_ENTRY tilemap, int cam_x, int cam_y) {
	renderTilemapCameraMove(tilemap.render, cam_x, cam_y);

	return;
}


void EXPORT_THIS darnitRenderTilemap(void *handle, TILEMAP_ENTRY tilemap) {
	renderTilemapRender(handle, tilemap.render);

	return;
}


TILEMAP_ENTRY EXPORT_THIS darnitRenderTilemapDelete(TILEMAP_ENTRY tilemap) {
	tilemapFree(tilemap);
	tilemap.w = tilemap.h = 0;
	tilemap.data = NULL;
	tilemap.render = NULL;

	return tilemap;
}




/************************/
/***** BBOX *************/
/************************/


void EXPORT_THIS *darnitBBoxNew(unsigned int size) {
	return bboxNew(size);
}


void EXPORT_THIS *darnerBBoxFree(BBOX *bbox) {
	bboxFree(bbox);

	return NULL;
}


int EXPORT_THIS darnitBBoxCollTest(BBOX *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen) {
	return bboxCollBoxTest(bbox, x, y, w, h, list, listlen);
}


int EXPORT_THIS darnitBBoxEntryAdd(BBOX *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	return bboxAdd(bbox, x, y, w, h);
}


void EXPORT_THIS darnitBBoxEntryDelete(BBOX *bbox, int key) {
	bboxDelete(bbox, key);

	return;
}


void EXPORT_THIS darnitBBoxEntryMove(BBOX *bbox, int key, unsigned int x, unsigned int y) {
	bboxMove(bbox, key, x, y);

	return;
}


void EXPORT_THIS darnitBBoxEntryResize(BBOX *bbox, int key, unsigned int w, unsigned int h) {
	bboxResize(bbox, key, w, h);

	return;
}


void EXPORT_THIS darnitBBoxClear(BBOX *bbox) {
	bboxClear(bbox);

	return;
}


/**************************************/
/******* DYNAMC LIBRARIES *************/
/**************************************/


void EXPORT_THIS *darnitDynlibOpen(const char *fname) {
	return dynlibOpen(fname);
}


void EXPORT_THIS *darnitDynlibGet(void *lib, const char *symbol) {
	return dynlibSymbolGet(lib, symbol);
}

void EXPORT_THIS *darnitDynlibClose(void *lib) {
	return dynlibClose(lib);
}



/***************************************/
/*********** SOCKETS *******************/
/***************************************/


void EXPORT_THIS *darnitSocketConnect(const char *host, int port) {
	return socketConnect(host, port);
}


int EXPORT_THIS darnitSocketRecv(void *sock, char *buff, int len) {
	return socketRecv(sock, buff, len);
}


int EXPORT_THIS darnitSocketSend(void *sock, char *buff, int len) {
	return socketSend(sock, buff, len);
}


void EXPORT_THIS *darnitSocketClose(void *sock) {
	return socketClose(sock);
}
