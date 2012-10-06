#ifndef __TEXT_H__
#define	__TEXT_H__


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
	TILE_CACHE		*cache;
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
} TEXT_SURFACE;


int textInit();
void *textLoadFont(const char *fname, int w, int h, int linespec);
int textGetGlyphWidth(TEXT_FONT *font, unsigned int glyph);
float textGetGlyphWidthf(TEXT_FONT *font, unsigned int glyph);
int textGetStringWidth(TEXT_FONT *font, const char *string);
int textFontGetH(TEXT_FONT *font);
int textFontGetHS(TEXT_FONT *font);
void *textMakeRenderSurface(int chars, TEXT_FONT *font, unsigned int linelen, int x, int y);
void textResetSurface(TEXT_SURFACE *srf);
int textSurfaceAppendChar(TEXT_SURFACE *surface, const char *ch);
void textSurfaceAppendString(TEXT_SURFACE *surface, const char *str);
void *textSurfaceDestroy(TEXT_SURFACE *surface);
void textRender(TEXT_SURFACE *surface);
void textSurfaceSkip(TEXT_SURFACE *surface, int pixels);
int textSurfaceAppendCodepoint(TEXT_SURFACE *surface, unsigned int cp);
void textSurfaceSetPos(TEXT_SURFACE *surface, int x_pos);


#endif
