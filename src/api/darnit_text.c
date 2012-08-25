#include "darnit.h"


void EXPORT_THIS *darnitFontLoad(void *handle, const char *fname, unsigned int glyph_w, unsigned int glyph_h, int line_spacing) {
	return textLoadFont(handle, fname, glyph_w, glyph_h, line_spacing);
}


unsigned int EXPORT_THIS darnitFontGetGlyphW(void *font, const char *s) {
	unsigned int c;
	
	c = utf8GetChar(s);
	return textGetGlyphWidth(font, c);
}


unsigned int EXPORT_THIS darnitFontGetStringWidthPixels(void *font, const char *string) {
	return textGetStringWidth(font, string);
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


int EXPORT_THIS darnitTextSurfaceCharAppend(void *surface, char *c) {
	return textSurfaceAppendChar(surface, c);
}


void EXPORT_THIS darnitTextSurfaceStringAppend(void *surface, const char *string) {
	textSurfaceAppendString(surface, string);

	return;
}


void EXPORT_THIS darnitTextSurfaceDraw(void *surface) {
	textRender(surface);
	
	return;
}


void EXPORT_THIS darnitTextSurfaceSkip(void *surface, int pixels) {
	textSurfaceSkip(surface, pixels);

	return;
}


void EXPORT_THIS darnitTextSurfaceXposSet(void *surface, int x_pos) {
	textSurfaceSetPos(surface, x_pos);

	return;
}
