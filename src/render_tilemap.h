#ifndef __RENDER_TILEMAP_H__
#define	__RENDER_TILEMAP_H__


typedef struct {
	int				map_w;
	int				map_h;
	int				w;
	int				h;
	TILESHEET			*ts;
	TILE_CACHE			*cache;
	unsigned int			inv_div;
	unsigned int			*map;
	float				cam_x;
	float				cam_y;
	int				cam_xi;
	int				cam_yi;
	unsigned int			mask;
} RENDER_TILEMAP;


void *renderTilemapCreate(void *handle, unsigned int w, unsigned int h, unsigned int *map, int camera_x, int camera_y, unsigned int invisibility_divider, TILESHEET *ts, unsigned int mask);
void renderTilemapTileSet(RENDER_TILEMAP *tm, int x, int y, int tile);
void renderTilemapCameraMove(RENDER_TILEMAP *tm, int cam_x, int cam_y);
void renderTilemapRender(void *handle, RENDER_TILEMAP *tm);
void renderTilemapFree(RENDER_TILEMAP *tm);
void renderTilemapForceRecalc(RENDER_TILEMAP *tm);

#endif
