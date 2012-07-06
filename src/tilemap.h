#ifndef __TILEMAP_H__
#define	__TILEMAP_H__

typedef struct {
	int			w;
	int			h;
	unsigned int		*data;
	RENDER_TILEMAP		*render;
} TILEMAP_ENTRY;


TILEMAP_ENTRY *tilemapLoad(void *handle, const char *fname, int invs_div, void *tilesheet, unsigned int mask);
void *tilemapFree(TILEMAP_ENTRY *tm);


#endif
