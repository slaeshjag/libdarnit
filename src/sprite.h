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


void *spriteBoxNew(unsigned int prealloc);
void *spriteBoxFree(SPRITE *sprite);
int spriteGetOne(SPRITE *sprite);
int spriteGetIndex(SPRITE *sprite, int id);
int spriteLoad(void *handle, SPRITE *sprite, const char *fname, unsigned int dir);
void spriteTeleport(SPRITE *sprite, unsigned int key, unsigned int x, unsigned int y, unsigned int l);
void spriteEnableAnimation(SPRITE *sprite, unsigned int key);
void spriteDisableAnimation(SPRITE *sprite, unsigned int key);
void spriteAnimate(SPRITE *sprite);
void spriteDrawLayer(SPRITE *sprite, unsigned int layer);
void spriteSort(SPRITE *sprite);
void spriteChangeDirection(SPRITE *sprite, unsigned int key, unsigned int dir);
void spriteDelete(SPRITE *sprite, unsigned int key);
void spriteClear(SPRITE *sprite);


#endif
