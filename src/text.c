#include "darnit.h"


void *textLoadFont(void *handle, const char *fname, int w, int h, int linespec) {
	DARNIT *m = handle;
	TEXT_FONT *fnt;

	if ((fnt = malloc(sizeof(TEXT_FONT))) == NULL) {
		MALLOC_ERROR
		return NULL;
	}

	if ((fnt->ts = renderTilesheetLoad(m, fname, w, h, PFORMAT_RGBA4)) == NULL) {
		fprintf(stderr, "Unable to load the font, we're going down...\n");
		return NULL;
	}

	fnt->w = w;
	fnt->h = h;
	fnt->linespec = linespec;

	return fnt;
}


int textFontGetW(TEXT_FONT *font) {
	if (font == NULL) return -1;
	return font->w;
}

int textFontGetH(TEXT_FONT *font) {
	if (font == NULL) return -1;
	return font->h;
}

int textFontGetHS(TEXT_FONT *font) {
	if (font == NULL) return -1;
	return font->h + font->linespec;
}

void textResetSurface(TEXT_SURFACE *srf) {
	if (srf == NULL) return;
	int i;
	float *arr;

	arr = (float *) srf->cache;

	for (i = 0; i < srf->len*24; i++)
		arr[i] = 0.0f;

	srf->pos = 0;
	srf->line = 0;
	srf->index = 0;

	return;
}


void *textMakeRenderSurface(int chars, TEXT_FONT *font, int linelen, int x, int y) {
	TEXT_SURFACE *surface;
	int i;
	float *arr;

	if ((surface = malloc(sizeof(TEXT_SURFACE))) == NULL) {
		MALLOC_ERROR
		return NULL;
	}

	if ((surface->cache = malloc(sizeof(TILE_CACHE)*chars)) == NULL) {
		MALLOC_ERROR
		return NULL;
	}

	arr = (float *) surface->cache;

	for (i = 0; i < chars*24; i++)
		arr[i] = 0.0f;
	
	surface->font = font;
	surface->pos = 0;
	surface->line = 0;
	surface->index = 0;
	surface->len = chars;
	surface->x = x;
	surface->y = y;
	surface->linelen = linelen;

	return surface;
}


void textSurfaceAppendChar(TEXT_SURFACE *surface, unsigned char c) {
	if (surface == NULL) return;
	int x, y;

	if (c == '\n') {
		surface->line++;
		surface->pos = 0;
		return;
	}

	x = surface->pos * surface->font->w + surface->x;
	y = surface->line * (surface->font->h + surface->font->linespec) + surface->y;
	if (c != ' ') {											/* "Temporary" work-around that makes OpenGL|ES happier... */
		renderCalcTilePosCache(&surface->cache[surface->index], surface->font->ts, x, y);
		renderCalcTileCache(&surface->cache[surface->index], surface->font->ts, c);
		surface->index++;
	} 
	
	surface->pos++;
	if (surface->pos >= surface->linelen) {
		surface->pos = 0;
		surface->line++;
	}

	return;
}


void textSurfaceAppendString(TEXT_SURFACE *surface, const char *str) {
	int i;

	for (i = 0; str[i] != 0; i++) {
		if (surface->pos >= surface->len)
			break;
		textSurfaceAppendChar(surface, str[i]);
	
	return;
}

void textRender(TEXT_SURFACE *surface) {
	if (surface == NULL) return;
	if (surface->index == 0)
		return;
	renderCache(surface->cache, surface->font->ts, surface->index);

	return;
}


void *textSurfaceDestroy(TEXT_SURFACE *surface) {
	if (surface == NULL)
		return NULL;
	
	free(surface->cache);
	free(surface);

	return NULL;
}
