#ifndef __RENDER_TILEMAP_H__
#define	__RENDER_TILEMAP_H__


typedef struct {
	int				tilew;
	int				tileh;
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
	int				lastdir_x;
	int				lastdir_y;
} RENDER_TILEMAP;


void *renderTilemapCreate(void *handle, unsigned int w, unsigned int h, unsigned int *map, int camera_x, int camera_y, unsigned int invisibility_divider, TILESHEET *ts);
void renderTilemapCameraMove(RENDER_TILEMAP *tm, int cam_x, int cam_y);
void renderTilemapRender(RENDER_TILEMAP *tm);

#endif
