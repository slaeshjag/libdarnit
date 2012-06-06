#ifndef __DARNIT_TEXT_H__
#define	__DARNIT_TEXT_H__

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

#endif
