#ifndef __DARNIT_TEXT_H__
#define	__DARNIT_TEXT_H__

typedef void DARNIT_FONT;
typedef void DARNIT_TEXT_SURFACE;

DARNIT_FONT *darnitFontLoad(const char *fname, unsigned int size, unsigned int sheet_w, int sheet_h);
unsigned int darnitFontGetGlyphW(DARNIT_FONT *font, const char *s);
unsigned int darnitFontGetStringWidthPixels(DARNIT_FONT *font, const char *string);
unsigned int darnitTextWordWidth(DARNIT_FONT *font, const char *string, unsigned int *bytes);
unsigned int darnitTextStringGeometrics(DARNIT_FONT *font, const char *string, int linelen, int *string_w);
unsigned int darnitFontGetGlyphH(DARNIT_FONT *font);
unsigned int darnitFontGetGlyphHS(DARNIT_FONT *font);
void darnitTextSurfaceReset(DARNIT_TEXT_SURFACE *surface);
DARNIT_TEXT_SURFACE *darnitTextSurfaceAlloc(DARNIT_FONT *font, unsigned int glyphs, unsigned int linelen, int x, int y);
DARNIT_TEXT_SURFACE *darnitTextSurfaceFree(DARNIT_TEXT_SURFACE *surface);
int darnitTextSurfaceCharAppend(DARNIT_TEXT_SURFACE *surface, const char *c);
void darnitTextSurfaceStringAppend(DARNIT_TEXT_SURFACE *surface, const char *string);
void darnitTextSurfaceDraw(DARNIT_TEXT_SURFACE *surface);
void darnitTextSurfaceSkip(DARNIT_TEXT_SURFACE *surface, int pixels);
void darnitTextSurfaceXposSet(DARNIT_TEXT_SURFACE *surface, int x_pos);

#endif
