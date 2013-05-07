/*
Copyright (c) 2011-2013 Steven Arnow
'text.c' - This file is part of libdarnit

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/


#include "darnit.h"

#ifndef DARNIT_HEADLESS

int textThisManyGlyphsWillFit(TEXT_SURFACE *surface, const char *str, unsigned int width) {
	int i, k;
	float j;
	unsigned int glyph;

	for (i = j = k = 0; str[i] != 0; k++) {
		glyph = utf8GetChar(&str[i]);
		j += textGetGlyphWidthf(surface->font, glyph);
		if (j > width)
			return k;
		i += utf8GetValidatedCharLength(&str[i]);
	}

	return k;
}

void *textLoadFont(const char *fname, int size, int tex_w, int tex_h) {
	TEXT_FONT *font;
	FILESYSTEM_FILE *fp;
	int flen, asc, desc, linegap;

	if ((font = malloc(sizeof(TEXT_FONT))) == NULL) {
		return NULL;
	}

	if ((fp = fsFileOpen(fname, "rb")) == NULL) {
		fprintf(stderr, "Unable to open file %s\n", fname);
		free(font);
		return NULL;
	}

	fsFileSeek(fp, 0, SEEK_END);
	flen = fsFileTell(fp);
	fsFileSeek(fp, 0, SEEK_SET);

	if ((font->font_data = malloc(flen)) == NULL) {
		fsFileClose(fp);
		free(font);
		return NULL;
	}

	fsFileRead(font->font_data, flen, fp);
	font->font_data_len = flen;
	fsFileClose(fp);

	if (stbtt_InitFont(&font->face, font->font_data, 0) == 0) {
		free(font->font_data);
		free(font);
		return NULL;
	}

	font->cache = NULL;
	font->font_height = size;
	font->tex_w = bitwiseRoundUpToPow2(tex_w);
	font->tex_h = bitwiseRoundUpToPow2(tex_h);
	font->scale = stbtt_ScaleForPixelHeight(&font->face, font->font_height);

	stbtt_GetFontVMetrics(&font->face, &asc, &desc, &linegap);
	font->line_gap = font->scale * linegap;
	font->ascent = font->scale * asc;
	font->descent = font->scale * desc;

	return font;
}


int textWillGlyphFit(struct TEXT_FONT_CACHE *cache, int w, int h) {
	int pos_y;

	pos_y = (cache->line_pos_x + w >= cache->sheet_w) ? cache->line_pos_y + cache->line_height : cache->line_pos_y;
	if (pos_y + h >= cache->sheet_h) {
		fprintf(stderr, "No, it will not fit\n");
		return -1;
	}
	return 0;
}


struct TEXT_FONT_CACHE *textFindLastTexture(struct TEXT_FONT_CACHE *cache) {
	if (cache == NULL)
		return NULL;
	while (cache->next != NULL)
		cache = cache->next;
	
	return cache;
}


struct TEXT_FONT_CACHE *textAppendCache(TEXT_FONT *font, struct TEXT_FONT_CACHE *index, int tex_w, int tex_h) {
	struct TEXT_FONT_CACHE *next;

	index = textFindLastTexture(index);

	if ((next = malloc(sizeof(struct TEXT_FONT_CACHE))) == NULL)
		return NULL;
	
	if (index != NULL)
		index->next = next;

	next->next = NULL;
	next->glyph = NULL;

	if ((next->ts = renderNewTilesheet(1, 1, tex_w, tex_h, PFORMAT_A8)) == NULL) {
		free(next);
		return NULL;
	}

	next->line_pos_x = next->line_pos_y = next->line_height = 0;
	next->sheet_w = tex_w;
	next->sheet_h = tex_h;

	next->sheet_pwf = 1.0f / tex_w;
	next->sheet_phf = 1.0f / tex_h;
	next->line_height = 0;
	next->line_pos_x = next->line_pos_y = 0;

	return next;
}


void textCacheGetCoordinates(struct TEXT_FONT_CACHE *index, int w, int h, int *x, int *y) {
	*x = index->line_pos_x + w + 2;
	*y = index->line_pos_y;
	if (*x >= index->sheet_w) {
		*x = 0;
		*y += index->line_height;
		index->line_pos_y = *y;
		index->line_height = 0;
	} else {
		*x -= w;
		*x += 1;
	}

	if (index->line_height < h)
		index->line_height = h;

	index->line_pos_x = *x + w + 1;
	index->line_pos_y = *y;
	return;
}


struct TEXT_FONT_GLYPH *textRenderGlyph(struct TEXT_FONT_CACHE *index, unsigned int glyph, int glyph_index, TEXT_FONT *font) {
	int pos_x, pos_y, w, h, x1, x2, y1, y2, ad, sb;
	unsigned char *data;
	struct TEXT_FONT_GLYPH *next, *alloc;
	float skipf;

	if ((alloc = malloc(sizeof(struct TEXT_FONT_GLYPH))) == NULL)
		return NULL;

	next = index->glyph;
	alloc->next = NULL;

	if (next == NULL)
		index->glyph = alloc;
	else {
		while (next->next != NULL)
			next = next->next;
		next->next = alloc;
	}

	x1 = y1 = x2 = y2 = 0;
	data = stbtt_GetGlyphBitmap(&font->face, 0, font->scale, glyph_index, &w, &h, 0, 0);
	textCacheGetCoordinates(index, w, h, &pos_x, &pos_y);
	stbtt_GetGlyphBox(&font->face, glyph_index, &x1, &y1, &x2, &y2);
	stbtt_GetGlyphHMetrics(&font->face, glyph_index, &ad, &sb);

	alloc->u1 = index->sheet_pwf * pos_x;
	alloc->v1 = index->sheet_phf * pos_y;
	alloc->u2 = index->sheet_pwf * pos_x + index->sheet_pwf * (w);
	alloc->v2 = index->sheet_phf * pos_y + index->sheet_phf * (h);
	alloc->wf = index->sheet_pwf * w;
	alloc->hf = index->sheet_phf * h;
	alloc->risei = y2 * font->scale;
	alloc->rise = index->ts->shgran * alloc->risei;

	alloc->cw = w;
	alloc->ch = h;
	alloc->adv = lrintf(font->scale * ad);
	alloc->advf = index->ts->swgran * alloc->adv;
	skipf = font->scale * sb;
	alloc->skip = skipf;
	alloc->skipf = index->ts->swgran * skipf;

	alloc->glyph = glyph;
	alloc->tex_cache = index;

	renderUpdateTilesheet(index->ts, pos_x, pos_y, data, w, h);
	stbtt_FreeBitmap(data, NULL);

	return alloc;
}


struct TEXT_FONT_GLYPH *textRenderGlyphToCache(TEXT_FONT *font, unsigned int glyph) {
	int glyph_index, x1, y1, x2, y2;
	struct TEXT_FONT_CACHE *next;
	
	next = font->cache;
	glyph_index = stbtt_FindGlyphIndex(&font->face, glyph);
	
	/* Just in case the coords aren't reported correctly o_O */
	x1 = y1 = x2 = y2 = 0;
	stbtt_GetCodepointBox(&font->face, glyph_index, &x1, &y1, &x2, &y2);

	x1 = font->scale * x1;
	y1 = font->scale * y1;
	x2 = font->scale * x2;
	y2 = font->scale * y2;
	
	while (next != NULL) {
		if (textWillGlyphFit(next, x2 - x1, y2 - y1) == 0)
			break;
		next = next->next;
	}
	
	if (next == NULL)
		if ((next = textAppendCache(font, font->cache, font->tex_w, font->tex_h)) == NULL)
			return NULL;
	
	if (font->cache == NULL)
		font->cache = next;
	
	return textRenderGlyph(next, glyph, glyph_index, font);
}


struct TEXT_FONT_GLYPH *textGetGlyphEntry(TEXT_FONT *font, unsigned int glyph) {
	struct TEXT_FONT_CACHE *next;
	struct TEXT_FONT_GLYPH *next_g;

	next = font->cache;
	while (next != NULL) {
		next_g = next->glyph;
		while (next_g != NULL) {
			if (next_g->glyph == glyph)
				return next_g;
			next_g = next_g->next;
		}
		next = next->next;
	}

	return textRenderGlyphToCache(font, glyph);
}


float textGetGlyphWidthf(TEXT_FONT *font, unsigned int glyph) {
	struct TEXT_FONT_GLYPH *glyph_e;

	if ((glyph_e = textGetGlyphEntry(font, glyph)) == NULL)
		return 0;
	return glyph_e->advf;
}


int textGetGlyphWidth(TEXT_FONT *font, unsigned int glyph) {
	struct TEXT_FONT_GLYPH *glyph_e;

	if ((glyph_e = textGetGlyphEntry(font, glyph)) == NULL)
		return 0;
	return glyph_e->adv;
}


float textGetGlyphHeightf(TEXT_FONT *font) {
	float h = font->font_height;
	
	return h * d->video.shgran;
}


int textGetStringWidth(TEXT_FONT *font, const char *string) {
	int i, w;
	unsigned int glyph;

	for (i = w = 0; string[i] != 0; ) {
		glyph = utf8GetChar(&string[i]);
		i += utf8GetValidatedCharLength(&string[i]);
		w += textGetGlyphWidth(font, glyph);
	}

	return w;
}


int textStringWordLength(TEXT_FONT *font, const char *string, int *bytes) {
	int i, w;
	unsigned int glyph;

	if (!font)
		return 0;

	for (i = w = 0; string[i] != 0 && string[i] != ' ' && string[i] != '\n';) {
		glyph = utf8GetChar(&string[i]);
		i += utf8GetValidatedCharLength(&string[i]);
		w += textGetGlyphWidth(font, glyph);
	}
	
	if (bytes)
		*bytes = i;
	return w;
}


int textStringBytesOnLine(TEXT_FONT *font, const char *string, int linelen, int *w_m) {
	int i, w;
	unsigned int glyph;

	if (!font)
		return 0;
	
	for (i = w = 0; string[i] != 0;) {
		glyph = utf8GetChar(&string[i]);
		if (textGetGlyphWidth(font, glyph) + w > linelen) {
			*w_m = w;
			return i;
		}
		w += textGetGlyphWidth(font, glyph);
		i += utf8GetValidatedCharLength(&string[i]);
	}

	*w_m = w;
	return i;
}


int textStringGeometrics(TEXT_FONT *font, const char *string, int linelen, int *w_set) {
	int j,  w, w_max, t, b;

	if (!font)
		return 0;
	w_max = 0;
	for (j = 0; *string; j++) {
		for (w = 0; *string != 0 && *string != '\n';) {
			t = textStringWordLength(font, string, &b);
			if (t + w >= linelen) {
				if (w > 0)
					break;
				else 
					b = textStringBytesOnLine(font, string, linelen, &t);
			}

			string += b;
			w += t;

			if (*string == ' ') {
				if (textGetGlyphWidth(font, ' ') + w >= linelen) {
					string++;
					break;
				}

				w += textGetGlyphWidth(font, ' ');
				string++;
			}
		}
		if (w == 0 && *string != '\n') {	// We can't fit a single glyph. Meh. //
			*w_set = 0;
			return 0;
		}
			
		if (w > w_max)
			w_max = w;
		if (*string == '\n')
			string++;
	}

	if (w_set)
		*w_set = w_max;
	return j * textFontGetHS(font);
}


int textFontGetH(TEXT_FONT *font) {
	if (font == NULL) return -1;
	return font->font_height;
}

int textFontGetHS(TEXT_FONT *font) {
	if (font == NULL) return -1;
	return font->font_height + font->line_gap;
}

void textResetSurface(TEXT_SURFACE *srf) {
	if (srf == NULL) return;
	int i;
	float *arr;
	struct TEXT_GLYPH_CACHE *cache, *tmp;

	arr = (float *) srf->cache;

	for (i = 0; i < srf->len*24; i++)
		arr[i] = 0.0f;

	cache = srf->g_cache;
	while (cache != NULL) {
		tmp = cache->next;
		free(cache);
		cache = tmp;
	}

	srf->g_cache = srf->l_cache = NULL;
	srf->pos = 0;
	srf->line = 0;
	srf->index = 0;
	srf->last = 0;
	srf->cur_xf = srf->orig_xf;
	srf->cur_yf = srf->orig_yf;


	return;
}


void *textMakeRenderSurface(int chars, TEXT_FONT *font, unsigned int linelen, int x, int y, FONT_TYPE type) {
	TEXT_SURFACE *surface;
	int i;
	float *arr;

	if ((surface = malloc(sizeof(TEXT_SURFACE))) == NULL) {
		MALLOC_ERROR
		return NULL;
	}

	if (type == NORMAL) {
		if ((surface->cache = malloc(sizeof(TILE_CACHE)*chars)) == NULL) {
			MALLOC_ERROR
			return NULL;
		}
	} else if (type == COLOR) {
		if (!(surface->cache_c = malloc(sizeof(TILE_COLOR_TEX_CACHE) * chars))) {
			MALLOC_ERROR
			return NULL;
		}
	}

	surface->type = type;
	arr = (float *) surface->cache;

	for (i = 0; i < chars * ((type == NORMAL) ? 24 : 30); i++)
		arr[i] = 0.0f;

	surface->font = font;
	surface->pos = 0;
	surface->line = 0;
	surface->index = 0;

	surface->color[0] = surface->color[1] = surface->color[2] = surface->color[3] = 255;
	
	surface->len = chars;
	surface->linelen = linelen;
	surface->linelenf = d->video.swgran * linelen;
	if (linelen == ~0)
		linelen = 0;
	surface->cur_xf = d->video.swgran * -1 * (linelen >> 1);
	surface->cur_yf = d->video.shgran * ((font->font_height) >> 1) * -1;
	surface->yf_skip = d->video.shgran * ((float) font->font_height + font->line_gap) * -1;
	surface->orig_xf = surface->cur_xf;
	surface->orig_yf = surface->cur_yf;
	surface->x = d->video.swgran * (x + (linelen >> 1)) - 1.0f;
	surface->y = 1.0f - d->video.shgran * (y + ((font->ascent + font->descent) >> 1));
	surface->last = 0;
	surface->prim = TEXT_O_LEFT_TO_RIGHT;
	surface->sec = TEXT_O_TOP_TO_BOTTOM;

	surface->g_cache = surface->l_cache = NULL;

	return surface;
}


/* continue here */


struct TEXT_GLYPH_CACHE *textGlyphCacheNew(struct TEXT_GLYPH_CACHE *start) {
	struct TEXT_GLYPH_CACHE *new;

	if ((new = malloc(sizeof(struct TEXT_GLYPH_CACHE))) == NULL)
		return NULL;

	new->next = NULL;
	
	if (start != NULL) {
		while (start->next != NULL)
			start = start->next;
		start->next = new;
	}

	return new;
}		


void *textSurfaceGlyphCacheFree(struct TEXT_GLYPH_CACHE *cache) {
	struct TEXT_GLYPH_CACHE *tmp;

	while (cache != NULL) {
		tmp = cache->next;
		free(cache);
		cache = tmp;
	}

	return NULL;
}


int textSurfaceAppendChar(TEXT_SURFACE *surface, const char *ch) {
	unsigned int glyph;
	if (surface == NULL) return 1;

	glyph = utf8GetChar(ch);
	textSurfaceAppendCodepoint(surface, glyph);
	return utf8GetValidatedCharLength(ch);
}


float textGetKern(TEXT_SURFACE *surface, unsigned int cp) {
	float adv;
	
	adv = surface->font->scale * stbtt_GetGlyphKernAdvance(&surface->font->face, surface->last, cp);
	adv *= surface->font->cache->ts->swgran;

	return adv;
}


void textSurfaceSetOrientation(TEXT_SURFACE *surface, FONT_ORIENTATION prim, FONT_ORIENTATION sec) {
	if (!surface)
		return;
	surface->prim = prim;
	surface->sec = sec;
	return;
}


int textSurfaceAppendCodepoint(TEXT_SURFACE *surface, unsigned int cp) {
	if (surface == NULL) return 1;

	struct TEXT_FONT_GLYPH *glyph_e;
	unsigned int glyph = cp;
	float x, y, x2, y2, hf;
	int w, h;

	if (surface->len == surface->index) return 1;

	if (cp == '\n') {
		switch (surface->sec) {
			case TEXT_O_TOP_TO_BOTTOM:
				surface->cur_xf = surface->orig_xf;
				surface->cur_yf += surface->yf_skip;
			case TEXT_O_LEFT_TO_RIGHT:
				surface->cur_yf = surface->orig_yf;
				surface->cur_xf += surface->yf_skip;
			case TEXT_O_RIGHT_TO_LEFT:
				surface->cur_yf = surface->orig_yf;
				surface->cur_xf -= surface->yf_skip;
		}
		
		surface->pos = 0;
		return 1;
	}
	
	if ((glyph_e = textGetGlyphEntry(surface->font, glyph)) == NULL)
		return 1;

//	wf = textGetGlyphWidthf(surface->font, glyph);
	hf = textFontGetHS(surface->font);
	w = textGetGlyphWidth(surface->font, glyph);
	h = textFontGetH(surface->font);
	if (surface->prim == TEXT_O_LEFT_TO_RIGHT)
		surface->cur_xf += textGetKern(surface, cp);

	
	if (surface->prim == TEXT_O_LEFT_TO_RIGHT || surface->prim == TEXT_O_TOP_TO_BOTTOM) {
		if (surface->pos + w >= surface->linelen) {
			textSurfaceAppendCodepoint(surface, '\n');
	
			if (cp == ' ')
				return 1;
			surface->pos = w;
		} else
			surface->pos += w;
	} else if (surface->prim == TEXT_O_TOP_TO_BOTTOM) {
		if (surface->pos + h >= surface->linelen) {
			textSurfaceAppendCodepoint(surface, '\n');

			if (cp == ' ')
				return 1;
			surface->pos = h;
		} else
			surface->pos += h;
	}

	x = surface->cur_xf;
	
	if (surface->prim == TEXT_O_LEFT_TO_RIGHT) {
		surface->cur_xf += glyph_e->advf;
		x += glyph_e->skipf;
		x2 = x + d->video.swgran * glyph_e->cw;
		y2 = surface->cur_yf + glyph_e->rise;
		y = y2 - d->video.shgran * glyph_e->ch;
	} else if (surface->prim == TEXT_O_RIGHT_TO_LEFT) {
		surface->cur_xf -= glyph_e->advf;
		x2 = x;
		x2 -= glyph_e->skipf;
		x = x2 - d->video.swgran * glyph_e->cw;
		y2 = surface->cur_yf + glyph_e->rise;
		y = y2 - d->video.shgran * glyph_e->ch;
	} else if (surface->prim == TEXT_O_TOP_TO_BOTTOM) {
		x2 = x + d->video.swgran * glyph_e->cw;
		y2 = surface->cur_yf;
		y = y2 - d->video.shgran * glyph_e->ch;
		surface->cur_yf += hf;
	}


	if (cp != ' ') {		/* "Temporary" work-around that makes OpenGL|ES happier... */
		if (surface->type == NORMAL)
			renderSetTileCoordinates(&surface->cache[surface->index], x, y, x2, y2, glyph_e->u1, glyph_e->v1, glyph_e->u2, glyph_e->v2);
		else if (surface->type == COLOR)
			renderSetTileCoordinatesColor(&surface->cache_c[surface->index], x, y, x2, y2, glyph_e->u1, glyph_e->v1, glyph_e->u2, glyph_e->v2, surface->color);

		if (surface->l_cache == NULL || glyph_e->tex_cache != surface->l_cache->f_cache) {
			if ((surface->l_cache = textGlyphCacheNew(surface->g_cache)) == NULL)
				return 1;
			if (surface->g_cache == NULL)
				surface->g_cache = surface->l_cache;
			if (surface->type == NORMAL)
				surface->l_cache->t_cache = &surface->cache[surface->index];
			else if (surface->type == COLOR)
				surface->l_cache->t_cache = (TILE_CACHE *) &((TILE_COLOR_TEX_CACHE *) surface->cache)[surface->index];
			surface->l_cache->f_cache = glyph_e->tex_cache;
			surface->l_cache->glyphs = 0;
		}

		surface->l_cache->glyphs++;
		surface->index++;
	}

	surface->last = cp;
	
	return 1;
}


void textSurfaceAppendString(TEXT_SURFACE *surface, const char *str) {
	int i, next_word, t;

	next_word = 1;
	for (i = 0; str[i] != 0; ) {
		if (surface->index >= surface->len)
			break;
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\n') {
			i += textSurfaceAppendChar(surface, &str[i]);
			next_word = 1;
			continue;
		}
		if (surface->pos == 0 && !next_word) {
			i += textSurfaceAppendChar(surface, &str[i]);
			continue;
		}

		if (textStringWordLength(surface->font, &str[i], &t) + surface->pos <= surface->linelen && next_word) {
			i += textSurfaceAppendChar(surface, &str[i]);
			next_word = 0;
		}

		else if (textStringWordLength(surface->font, &str[i], &t) + surface->pos >= surface->linelen && next_word && surface->pos) {
			next_word = 0;
			textSurfaceAppendChar(surface, "\n");
		} else {
			i += textSurfaceAppendChar(surface, &str[i]);
			next_word = 0;
		}
	}

	return;
}


void textSurfaceColorNextSet(TEXT_SURFACE *surface, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	if (!surface)
		return;
	surface->color[0] = r;
	surface->color[1] = g;
	surface->color[2] = b;
	surface->color[3] = a;
	
	return;
}


void textRender(TEXT_SURFACE *surface) {
	struct TEXT_GLYPH_CACHE *next;
	
	if (surface == NULL)
		return;
	if (surface->g_cache == NULL)
		return;
	glTranslatef(surface->x, surface->y, 0.0f);
	
	if (surface == NULL) return;
	if (surface->index == 0)
		return;
	
	next = surface->g_cache;
	while (next != NULL) {
		if (surface->type == NORMAL)
			renderCache(next->t_cache, next->f_cache->ts, next->glyphs);
		else if (surface->type == COLOR)
			/* This is actually a TILE_COLOR_CACHE, in the function above, it gets casted via union. Ugly, right? */
			renderColCache((TILE_COLOR_TEX_CACHE *) next->t_cache, next->f_cache->ts, next->glyphs);
			
		next = next->next;
	}

	glTranslatef(-surface->x, -surface->y, 0.0f);

	return;
}


void *textSurfaceDestroy(TEXT_SURFACE *surface) {
	if (surface == NULL)
		return NULL;

	textSurfaceGlyphCacheFree(surface->g_cache);
	free(surface->cache);
	free(surface);

	return NULL;
}


void textSurfaceSkip(TEXT_SURFACE *surface, int pixels) {
	if (surface == NULL)
		return;
	surface->cur_xf += d->video.swgran * pixels;

	return;
}


void textSurfaceSetPos(TEXT_SURFACE *surface, int x_pos) {
	if (surface == NULL)
		return;
	if (surface->prim == TEXT_O_LEFT_TO_RIGHT)
		surface->cur_xf = surface->orig_xf + d->video.swgran * x_pos;
	else if (surface->prim == TEXT_O_RIGHT_TO_LEFT)
		surface->cur_xf = surface->orig_xf - d->video.swgran * x_pos;
	else if (surface->prim == TEXT_O_TOP_TO_BOTTOM)
		surface->cur_xf = surface->orig_yf + d->video.shgran * x_pos;
	
	return;
}


void EXPORT_THIS textGlyphCoords(TEXT_FONT *font, char *ch, int *x, int *y, int *w, int *h, TILESHEET **ts) {
	struct TEXT_FONT_GLYPH *glyph;
	
	if (!(glyph = textGetGlyphEntry(font, utf8GetChar(ch))))
		return;
	*x = glyph->u1 / glyph->tex_cache->sheet_pwf + 0.4f;
	*w = (glyph->u2 - glyph->u2) / glyph->tex_cache->sheet_pwf + 0.4f;
	*y = glyph->v1 / glyph->tex_cache->sheet_phf + 0.4f;
	*x = (glyph->v2 - glyph->v1) / glyph->tex_cache->sheet_phf + 0.4f;
	*ts = glyph->tex_cache->ts;

	return;
}

//DARNIT_HEADLESS
#endif
