#ifndef	__DARNIT_MTSPRITE_H__
#define	__DARNIT_MTSPRITE_H__

void *darnitMTSpriteLoad(void *handle, const char *fname);
void darnitMTSpriteAnimationEnable(void *mtsprite);
void darnitMTSpriteAnimationPause(void *mtsprite);
void darnitMTSpriteAnimationDisable(void *mtsprite);
void darnitMTSpriteDraw(void *mtsprite);
void *darnitMTSpriteDelete(void *handle, void *mtsprite);

#endif
