#ifndef __MT_SPRITE_H__
#define	__MT_SPRITE_H__


typedef struct {
	TILE_CACHE		*cache;
	int			tiles;
	int			time;
} MTSPRITE_FRAME;


typedef struct {
	char			tilesheet[64];
	TILESHEET		*ts;
	TILE_CACHE		*cache;
	MTSPRITE_FRAME		*frame;
	int			frames;
	int			tiles;
	int			cur_frame;
	int			time_left;
	int			time_last;
	int			animate;
} MTSPRITE_ENTRY;


void *mtSpriteNew(int tiles, int frames, void *ts);
void *mtSpriteLoad(const char *fname);
void mtSpriteDraw(MTSPRITE_ENTRY *spr);
void mtSpriteEnableAnimation(MTSPRITE_ENTRY *spr);
void mtSpritePauseAnimation(MTSPRITE_ENTRY *spr);
void mtSpriteSetAsFrame(MTSPRITE_ENTRY *spr, int time);
void mtSpriteAddTile(MTSPRITE_ENTRY *spr, int x, int y, int w, int h, int rx, int ry);
void mtSpriteDisableAnimation(MTSPRITE_ENTRY *spr);
void *mtSpriteDelete(MTSPRITE_ENTRY *spr);


#endif
