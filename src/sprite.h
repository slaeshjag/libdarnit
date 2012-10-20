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
	SPRITE_DIRECTION	spr[32];
	char			tilesheet[32];
	unsigned int		wsq;
	unsigned int		hsq;
	TILESHEET		*ts;
	TILE_CACHE		cache;
	unsigned int		y;
} SPRITE_ENTRY;


typedef struct {
	SPRITE_ENTRY		*sprite;
	unsigned int		sprites;
	unsigned int		cnt;
} SPRITE;


SPRITE_ENTRY *spriteNew(TILESHEET *ts);
void *spriteLoad(const char *fname, unsigned int dir, unsigned int target_format);
void spriteTeleport(SPRITE_ENTRY *sprite, int x, int y);
void spriteEnableAnimation(SPRITE_ENTRY *sprite);
void spritePauseAnimation(SPRITE_ENTRY *sprite);
void spriteDisableAnimation(SPRITE_ENTRY *sprite);
void spriteAnimate(SPRITE_ENTRY *sprite);
void spriteSetFrame(SPRITE_ENTRY *sprite, int frame);
void spriteActivate(SPRITE_ENTRY *sprite, int dir);
void spriteDraw(SPRITE_ENTRY *sprite);
void spriteChangeDirection(SPRITE_ENTRY *sprite, unsigned int dir);
void *spriteDelete(SPRITE_ENTRY *sprite);
void spriteSetFrameEntry(SPRITE_ENTRY *sprite, int dir, int frame, int tile, int time);


#endif
