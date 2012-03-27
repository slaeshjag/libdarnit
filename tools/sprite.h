#ifndef __SPRITE_H__
#define	__SPRITE_H__


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
	unsigned int		tleft;
	unsigned int		time;
	unsigned int		animate;
	unsigned int		used;
	SPRITE_DIRECTION	spr[8];
	char			tilesheet[32];
	unsigned int		wsq;
	unsigned int		hsq;
} SPRITE_ENTRY;


typedef struct {
	SPRITE_ENTRY		*sprite;
	unsigned int		sprites;
} SPRITE;

#endif
