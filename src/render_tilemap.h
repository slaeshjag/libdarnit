#ifndef __RENDER_TILEMAP_H__
#define	__RENDER_TILEMAP_H__


typedef struct {
	int				tilew;
	int				tileh;
	TILESHEET			*ts;
	TILE_CACHE			*cache;
	int				tm_w;
	int				tm_h;
	unsigned int			dns_div;
	unsigned int			*map;
	int				*row;
	int				*col;
	int				*row_spare;
	int				*col_spare;
	int				col_spares;
	int				row_spares;
	int				*needbuf_x;
	int				*needbuf_y;
	float				cam_x;
	float				cam_y;
} RENDER_TILEMAP;


void *renderTilemapCreate(void *handle, unsigned int w, unsigned int h, unsigned int *map, int camera_x, int camera_y, unsigned int invisibility_divider, TILESHEET *ts);
void renderTilemapCameraMove(RENDER_TILEMAP *tm, int cam_x, int cam_y);
void renderTilemapRender(void *handle, RENDER_TILEMAP *tm);
void renderTilemapFree(RENDER_TILEMAP *tm);

#endif
