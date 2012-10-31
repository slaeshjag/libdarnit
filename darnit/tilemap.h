#ifndef __TILEMAP_H__
#define	__TILEMAP_H__

#define	TILEMAP_DEFAULT_INV_DIV		0xFFF


typedef struct {
	int			w;
	int			h;
	unsigned int		*data;
	RENDER_TILEMAP		*render;
} TILEMAP_ENTRY;


TILEMAP_ENTRY *tilemapNew(int invs_div, void *tilesheet, unsigned int mask, int w, int h);
TILEMAP_ENTRY *tilemapLoad(const char *fname, int invs_div, void *tilesheet, unsigned int mask);
void *tilemapFree(TILEMAP_ENTRY *tm);


#endif