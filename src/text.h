#ifndef __TEXT_H__
#define	__TEXT_H__


typedef struct {
	TILESHEET		*ts;
	int			w;
	int			h;
	int			linespec;
} TEXT_FONT;


typedef struct {
	TILE_CACHE		*cache;
	TEXT_FONT		*font;
	int			pos;
	int			line;
	int			index;
	int			x;
	int			y;
	int			len;
	int			linelen;
} TEXT_SURFACE;


typedef struct {
	int			act;
	const char		*str;
	const char		*ans_str;
	int			strlen;
	int			pos;
	int			speed;
	int			scroll;
	int			npcCallback;
	int			lastTick;
	TEXT_SURFACE		*surface;
	TILE_CACHE		*bgcache;
	TILE_CACHE		facec;
	int			bgcachelen;
	TILESHEET		*bgts;
	TILESHEET		*facets;
	void			*menu;
	int			selection;
} TEXT_BOX;


typedef struct {
	TEXT_FONT		*def;
	TEXT_BOX		box;
} TEXT;


void *textLoadFont(void *handle, const char *fname, int w, int h, int linespec);
int textFontGetW(TEXT_FONT *font);
int textFontGetH(TEXT_FONT *font);
int textFontGetHS(TEXT_FONT *font);
int textInit(void *handle);
void *textMakeRenderSurface(int chars, TEXT_FONT *font, int linelen, int x, int y);
void textResetSurface(TEXT_SURFACE *srf);
void textSurfaceAppendChar(TEXT_SURFACE *surface, unsigned char c);
void textSurfaceAppendString(TEXT_SURFACE *surface, const char *str);
void *textSurfaceDestroy(TEXT_SURFACE *surface);
void textRender(TEXT_SURFACE *surface);


#endif
