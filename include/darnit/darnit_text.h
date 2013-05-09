/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_text.h' - This file is part of libdarnit

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


#ifndef __DARNIT_TEXT_H__
#define	__DARNIT_TEXT_H__

typedef void DARNIT_FONT;
typedef void DARNIT_TEXT_SURFACE;


typedef enum {
	DARNIT_FONT_LEFT_TO_RIGHT,
	DARNIT_FONT_RIGHT_TO_LEFT,
	DARNIT_FONT_TOP_TO_BOTTOM
} DARNIT_FONT_ORIENTATION;


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
void d_text_surface_orientation(void *surface, DARNIT_FONT_ORIENTATION prim, DARNIT_FONT_ORIENTATION sec);
int d_text_surface_char_append(DARNIT_TEXT_SURFACE *surface, const char *c);
void d_text_surface_string_append(DARNIT_TEXT_SURFACE *surface, const char *string);
void d_text_surface_color_next(DARNIT_TEXT_SURFACE *surface, unsigned char r, unsigned char g, unsigned char b);
void d_text_surface_draw(DARNIT_TEXT_SURFACE *surface);
void d_text_surface_offset_next_add(DARNIT_TEXT_SURFACE *surface, int pixels);
void d_text_surface_offset_next_set(DARNIT_TEXT_SURFACE *surface, int x_pos);

#endif
