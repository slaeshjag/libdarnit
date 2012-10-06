#ifndef	__DARNIT_MTSPRITE_H__
#define	__DARNIT_MTSPRITE_H__

void *darnitMTSpriteNew(void *tilesheet);
void *darnitMTSpriteLoad(const char *fname);
void darnitMTSpriteAnimationEnable(void *mtsprite);
void darnitMTSpriteAnimationPause(void *mtsprite);
void darnitMTSpriteTileAdd(void *sprite, int x, int y, int w, int h, int rx, int ry);
void darnitMTSpriteFrameSet(void *sprite, int time);
void darnitMTSpriteAnimationDisable(void *mtsprite);
void darnitMTSpriteDraw(void *mtsprite);
void *darnitMTSpriteDelete(void *mtsprite);

#endif
