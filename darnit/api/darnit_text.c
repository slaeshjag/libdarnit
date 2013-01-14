#include "darnit.h"


void EXPORT_THIS *d_font_load(const char *fname, unsigned int glyph_w, unsigned int glyph_h, int line_spacing) {
	return textLoadFont(fname, glyph_w, glyph_h, line_spacing);
}


unsigned int EXPORT_THIS d_font_glyph_w(void *font, const char *s) {
	unsigned int c;
	
	c = utf8GetChar(s);
	return textGetGlyphWidth(font, c);
}


unsigned int EXPORT_THIS d_font_string_w(void *font, const char *string) {
	return textGetStringWidth(font, string);
}


unsigned int EXPORT_THIS d_font_glyph_h(void *font) {
	return textFontGetH(font);
}


unsigned int EXPORT_THIS d_font_glyph_hs(void *font) {
	return textFontGetHS(font);
}


unsigned int EXPORT_THIS d_font_word_w(void *font, const char *string, unsigned int *bytes) {
	return textStringWordLength(font, string, (int *) bytes);
}


unsigned int EXPORT_THIS d_font_string_geometrics(void *font, const char *string, int linelen, int *string_w) {
	return textStringGeometrics(font, string, linelen, string_w);
}


void EXPORT_THIS d_text_surface_reset(void *surface) {
	textResetSurface(surface);

	return;
}


void EXPORT_THIS *d_text_surface_new(void *font, unsigned int glyphs, unsigned int linelen, int x, int y) {
	return textMakeRenderSurface(glyphs, font, linelen, x, y, NORMAL);
}


void EXPORT_THIS *d_text_surface_color_new(void *font, unsigned int glyphs, unsigned int linelen, int x, int y) {
	return textMakeRenderSurface(glyphs, font, linelen, x, y, COLOR);
}


void EXPORT_THIS *d_text_surface_free(void *surface) {
	return textSurfaceDestroy(surface);
}


int EXPORT_THIS d_text_surface_char_append(void *surface, char *c) {
	return textSurfaceAppendChar(surface, c);
}


void EXPORT_THIS d_text_surface_string_append(void *surface, const char *string) {
	textSurfaceAppendString(surface, string);

	return;
}


void EXPORT_THIS d_text_surface_color_next(void *surface, unsigned char r, unsigned char g, unsigned char b) {
	textSurfaceColorNextSet(surface, r, g, b, 255);

	return;
}


void EXPORT_THIS d_text_surface_draw(void *surface) {
	textRender(surface);
	
	return;
}


void EXPORT_THIS d_text_surface_offset_next_add(void *surface, int pixels) {
	textSurfaceSkip(surface, pixels);

	return;
}


void EXPORT_THIS d_text_surface_offset_next_set(void *surface, int x_pos) {
	textSurfaceSetPos(surface, x_pos);

	return;
}
