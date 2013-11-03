/*
Copyright (c) 2011-2013 Steven Arnow
'text.h' - This file is part of libdarnit

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


#ifndef __TEXT_H__
#define	__TEXT_H__


typedef enum FONT_TYPE {
	NORMAL,
	COLOR
} FONT_TYPE;


typedef enum {
	TEXT_O_LEFT_TO_RIGHT,
	TEXT_O_RIGHT_TO_LEFT,
	TEXT_O_TOP_TO_BOTTOM
} FONT_ORIENTATION;


struct TEXT_FONT_GLYPH {
	float			u1;
	float			v1;
	float			u2;
	float			v2;

	float			wf;
	float			hf;
	float			rise;
	int			risei;

	int			ch;

	int			cw;
	int			adv;
	int			skip;

	float			advf;
	float			skipf;
	unsigned int		glyph;

	struct TEXT_FONT_CACHE	*tex_cache;
	struct TEXT_FONT_GLYPH	*next;
};


struct TEXT_FONT_CACHE {
	struct TEXT_FONT_CACHE	*next;
	TILESHEET		*ts;

	int			line_height;
	int			line_pos_x;
	int			line_pos_y;

	int			sheet_w;
	int			sheet_h;

	float			sheet_pwf;
	float			sheet_phf;

	struct TEXT_FONT_GLYPH	*glyph;
};


typedef struct {
	stbtt_fontinfo		face;
	void			*font_data;
	int			font_data_len;

	struct TEXT_FONT_CACHE	*cache;
	int			font_height;
	int			line_gap;
	int			ascent;
	int			descent;
	float			scale;

	int			tex_w;
	int			tex_h;

	float			line_pos_x;
	float			line_pos_y;
} TEXT_FONT;


struct TEXT_GLYPH_CACHE {
	struct TEXT_GLYPH_CACHE	*next;
	struct TEXT_FONT_CACHE	*f_cache;
	TILE_CACHE		*t_cache;
	int			glyphs;
};


typedef struct {
	struct TEXT_GLYPH_CACHE	*g_cache;
	struct TEXT_GLYPH_CACHE	*l_cache;
	union {
		TILE_COLOR_TEX_CACHE *cache_c;
		TILE_CACHE	*cache;
	};

	FONT_TYPE		type;
	unsigned char		color[4];

	TEXT_FONT		*font;
	int			pos;
	int			line;
	int			index;
	float			x;
	float			y;
	int			len;
	unsigned int		linelen;
	float			linelenf;
	unsigned int		last;


	float			cur_xf;
	float			cur_yf;
	float			yf_skip;
	float			orig_xf;
	float			orig_yf;
	float			y_offset;
	FONT_ORIENTATION	prim;
	FONT_ORIENTATION	sec;
} TEXT_SURFACE;


int textInit();
void *textLoadFont(const char *fname, int w, int h, int linespec);
int textGetGlyphWidth(TEXT_FONT *font, unsigned int glyph);
float textGetGlyphWidthf(TEXT_FONT *font, unsigned int glyph);
int textGetStringWidth(TEXT_FONT *font, const char *string);

int textStringWordLength(TEXT_FONT *font, const char *string, int *bytes);
int textStringGeometrics(TEXT_FONT *font, const char *string, int linelen, int *w_set);

int textFontGetH(TEXT_FONT *font);
int textFontGetHS(TEXT_FONT *font);
void *textMakeRenderSurface(int chars, TEXT_FONT *font, unsigned int linelen, int x, int y, FONT_TYPE type);
void textResetSurface(TEXT_SURFACE *srf);
int textSurfaceAppendChar(TEXT_SURFACE *surface, const char *ch);
void textSurfaceAppendString(TEXT_SURFACE *surface, const char *str);
void textSurfaceColorNextSet(TEXT_SURFACE *surface, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void *textSurfaceDestroy(TEXT_SURFACE *surface);
void textRender(TEXT_SURFACE *surface);
void textSurfaceSkip(TEXT_SURFACE *surface, int pixels);
int textSurfaceAppendCodepoint(TEXT_SURFACE *surface, unsigned int cp);
void textSurfaceSetPos(TEXT_SURFACE *surface, int x_pos);
void textSurfaceSetOrientation(TEXT_SURFACE *surface, FONT_ORIENTATION prim, FONT_ORIENTATION sec);


#endif
