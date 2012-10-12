#ifndef __DARNIT_MAP_H__
#define	__DARNIT_MAP_H__

typedef void			DARNIT_MAP_REF;
typedef void			DARNIT_MAP_STRINGDATA;

typedef struct {
	unsigned int		x;
	unsigned int		y;
	unsigned int		l;
	DARNIT_MAP_REF		*ref;
} DARNIT_MAP_OBJECT;


typedef struct {
	DARNIT_TILEMAP		*tilemap;
	DARNIT_TILESHEET	*ts;
	DARNIT_MAP_REF		*ref;
	unsigned int		offset_x;
	unsigned int		offset_y;
	unsigned int		tile_w;
	unsigned int		tile_h;
} DARNIT_MAP_LAYER;


typedef struct {
	DARNIT_MAP_LAYER	*layer;
	const unsigned int	layers;
	DARNIT_MAP_OBJECT	*object;
	const unsigned int	objects;
	DARNIT_MAP_REF		*prop;
	DARNIT_MAP_STRINGDATA	*stringdata;
	DARNIT_MAP_REF		*stringrefs;

	const int		cam_x;
	const int 		cam_y;
} DARNIT_MAP;


DARNIT_MAP *darnitMapLoad(const char *fname);
DARNIT_MAP *darnitMapUnload(DARNIT_MAP *map);
const char *darnitMapPropGet(DARNIT_MAP_REF *prop, const char *key);
const char *darnitMapLayerPropGet(DARNIT_MAP *map, int layer, const char *key);
void darnitMapCameraMove(DARNIT_MAP *map, int x, int y);

#endif
