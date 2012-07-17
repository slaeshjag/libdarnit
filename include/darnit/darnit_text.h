#ifndef __DARNIT_TEXT_H__
#define	__DARNIT_TEXT_H__

void *darnitFontLoad(void *handle, const char *fname, unsigned int size, unsigned int sheet_w, int sheet_h);
unsigned int darnitFontGetGlyphW(void *font, const char *s);
unsigned int darnitFontGetGlyphH(void *font);
unsigned int darnitFontGetGlyphHS(void *font);
void darnitTextSurfaceReset(void *surface);
void *darnitTextSurfaceAlloc(void *font, unsigned int glyphs, unsigned int linelen, int x, int y);
void *darnitTextSurfaceFree(void *surface);
int darnitTextSurfaceCharAppend(void *surface, const char *c);
void darnitTextSurfaceStringAppend(void *surface, const char *string);
void darnitTextSurfaceDraw(void *surface);
void darnitTextSurfaceSkip(void *surface, int pixels);
void darnitTextSurfaceXposSet(void *surface, int x_pos);

#endif
