#ifndef	__DARNIT_SPRITE_H__
#define	__DARNIT_SPRITE_H__

#define	DARNIT_SPRITE_ANIMATION_NOREPEAT	0
#define	DARNIT_SPRITE_ANIMATION_REPEAT		1

typedef void DARNIT_SPRITE;


DARNIT_SPRITE *darnitSpriteNew(DARNIT_TILESHEET *tilesheet);
DARNIT_SPRITE *darnitSpriteLoad(const char *fname, int dir, unsigned int target_format);
DARNIT_SPRITE *darnitSpriteDelete(DARNIT_SPRITE *sprite);
void darnitSpriteDirectionChange(DARNIT_SPRITE *sprite, int dir);
void darnitSpriteActivate(DARNIT_SPRITE *sprite, int dir);
void darnitSpriteSetFrame(DARNIT_SPRITE *sprite, int frame);
void darnitSpriteMove(DARNIT_SPRITE *sprite, int x, int y);
void darnitSpriteAnimationEnable(DARNIT_SPRITE *sprite);
void darnitSpriteAnimationPause(DARNIT_SPRITE *sprite);
void darnitSpriteAnimationDisable(DARNIT_SPRITE *sprite);
void darnitSpriteAnimationRepeatSet(void *sprite, int repeat);
void darnitSpriteDraw(DARNIT_SPRITE *sprite);
void darnitSpriteFrameEntrySet(DARNIT_SPRITE *sprite, int dir, int frame, int tile, int time);

int darnitSpriteWidth(DARNIT_SPRITE *sprite);
int darnitSpriteHeight(DARNIT_SPRITE *sprite);

#endif
