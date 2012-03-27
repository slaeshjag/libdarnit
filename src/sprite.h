#ifndef __SPRITE_H__
#define	__SPRITE_H__


#define				SPRITE_PREALLOC_DEF		64


typedef struct {
	unsigned int		time;
	unsigned int		tile;
} SPRITE_TILE;


typedef struct {
	SPRITE_TILE		tile[8];
	unsigned int		tiles;
} SPRITE_DIRECTION;


typedef struct {
	unsigned int		dir;
	unsigned int		frame;
	int			tleft;
	unsigned int		time;
	unsigned int		animate;
	unsigned int		used;
	SPRITE_DIRECTION	spr[8];
	char			tilesheet[32];
	unsigned int		wsq;
	unsigned int		hsq;
	TILESHEET		*ts;
	TILE_CACHE		cache;
	unsigned int		layer;
	unsigned int		y;
	unsigned int		id;
} SPRITE_ENTRY;


//typedef struct {
	


typedef struct {
	SPRITE_ENTRY		*sprite;
	unsigned int		sprites;
	unsigned int		cnt;
} SPRITE;


int spriteInit(void *handle, unsigned int prealloc);
int spriteGetOne(void *handle);
int spriteGetIndex(void *handle, int id);
int spriteLoad(void *handle, const char *fname, unsigned int dir);
void spriteTeleport(void *handle, unsigned int sprite, unsigned int x, unsigned int y, unsigned int l);
void spriteEnableAnimation(void *handle, unsigned int sprite);
void spriteDisableAnimation(void *handle, unsigned int sprite);
void spriteAnimate(void *handle);
void spriteDrawLayer(void *handle, unsigned int layer);
void spriteSort(void *handle);
void spriteChangeDirection(void *handle, unsigned int sprite, unsigned int dir);
void spriteDelete(void *handle, unsigned int sprite);
void spriteClear(void *handle);


#endif
