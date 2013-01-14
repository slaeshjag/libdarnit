#ifndef __DARNIT_TEXT_H__
#define	__DARNIT_TEXT_H__

typedef void DARNIT_FONT;
typedef void DARNIT_TEXT_SURFACE;

DARNIT_FONT *d_font_load(const char *fname, unsigned int size, unsigned int sheet_w, int sheet_h);
unsigned int d_font_glyph_w(DARNIT_FONT *font, const char *s);
unsigned int d_font_string_w(DARNIT_FONT *font, const char *string);
unsigned int d_font_word_w(DARNIT_FONT *font, const char *string, unsigned int *bytes);
unsigned int d_font_string_geometrics(DARNIT_FONT *font, const char *string, int linelen, int *string_w);
unsigned int d_font_glyph_h(DARNIT_FONT *font);
unsigned int d_font_glyph_hs(DARNIT_FONT *font);
void d_text_surface_reset(DARNIT_TEXT_SURFACE *surface);
DARNIT_TEXT_SURFACE *d_text_surface_new(DARNIT_FONT *font, unsigned int glyphs, unsigned int linelen, int x, int y);
DARNIT_TEXT_SURFACE *d_text_surface_color_new(DARNIT_FONT *font, unsigned int glyphs, unsigned int linelen, int x, int y);
DARNIT_TEXT_SURFACE *d_text_surface_free(DARNIT_TEXT_SURFACE *surface);
int d_text_surface_char_append(DARNIT_TEXT_SURFACE *surface, const char *c);
void d_text_surface_string_append(DARNIT_TEXT_SURFACE *surface, const char *string);
void d_text_surface_color_next(DARNIT_TEXT_SURFACE *surface, unsigned char r, unsigned char g, unsigned char b);
void d_text_surface_draw(DARNIT_TEXT_SURFACE *surface);
void d_text_surface_offset_next_add(DARNIT_TEXT_SURFACE *surface, int pixels);
void d_text_surface_offset_next_set(DARNIT_TEXT_SURFACE *surface, int x_pos);

#endif
