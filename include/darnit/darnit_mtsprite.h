#ifndef	__DARNIT_MTSPRITE_H__
#define	__DARNIT_MTSPRITE_H__

typedef void DARNIT_MTSPRITE;

DARNIT_MTSPRITE *darnitMTSpriteNew(void *tilesheet);
DARNIT_MTSPRITE *darnitMTSpriteLoad(const char *fname);
void darnitMTSpriteAnimationEnable(DARNIT_MTSPRITE *mtsprite);
void darnitMTSpriteAnimationPause(DARNIT_MTSPRITE *mtsprite);
void darnitMTSpriteTileAdd(DARNIT_MTSPRITE *sprite, int x, int y, int w, int h, int rx, int ry);
void darnitMTSpriteFrameSet(DARNIT_MTSPRITE *sprite, int time);
void darnitMTSpriteAnimationDisable(DARNIT_MTSPRITE *mtsprite);
void darnitMTSpriteDraw(DARNIT_MTSPRITE *mtsprite);
void *darnitMTSpriteDelete(DARNIT_MTSPRITE *mtsprite);

#endif
