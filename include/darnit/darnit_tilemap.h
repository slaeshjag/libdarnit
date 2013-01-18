#ifndef __DARNIT_TILEMAP_H__
#define	__DARNIT_TILEMAP_H__

#define			DARNIT_TILEMAP_DEFAULT_MASK	0xFFF

typedef struct {
	int		w;
	int		h;
	unsigned int	*data;
	void		*render;
} DARNIT_TILEMAP;


DARNIT_TILEMAP *d_tilemap_new(unsigned int invisibility_divider, DARNIT_TILESHEET *tilesheet, unsigned int mask, int w, int h);
DARNIT_TILEMAP *d_tilemap_load(const char *fname, unsigned int invisibility_divider, DARNIT_TILESHEET *tilesheet, unsigned int mask);
void d_tilemap_tile_set(DARNIT_TILEMAP *tilemap, int x, int y, int tile);
void d_tilemap_recalc(DARNIT_TILEMAP *tilemap);
void d_tilemap_camera_move(DARNIT_TILEMAP *tilemap, int cam_x, int cam_y);
void d_tilemap_draw(DARNIT_TILEMAP *tilemap);
DARNIT_TILEMAP *d_tilemap_free(DARNIT_TILEMAP *tilemap);

#endif
