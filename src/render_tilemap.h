#ifndef __RENDER_TILEMAP_H__
#define	__RENDER_TILEMAP_H__


typedef struct {
	unsigned int			tilew;
	unsigned int			tileh;
	int				strip_x;
	int				strip_y;
	TILESHEET			*ts;
	TILE_CACHE			*cache;
	unsigned int			tm_w;
	unsigned int			tm_h;
	int				cam_x;
	int				cam_y;
	unsigned int			dns_div;
	unsigned int			*map;
} RENDER_TILEMAP;


void *renderTilemapCreate(void *handle, unsigned int w, unsigned int h, unsigned int *map, int camera_x, int camera_y, unsigned int invisibility_divider, TILESHEET *ts);


#endif
