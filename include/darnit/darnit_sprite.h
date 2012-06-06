#ifndef	__DARNIT_SPRITE_H__
#define	__DARNIT_SPRITE_H__

void *darnitSpriteLoad(void *handle, const char *fname, int dir, unsigned int target_format);
void darnitSpriteDelete(void *handle, void *sprite);
void darnitSpriteDirectionChange(void *sprite, int dir);
void darnitSpriteMove(void *sprite, int x, int y);
void darnitSpriteAnimationEnable(void *sprite);
void darnitSpriteAnimationPause(void *sprite);
void darnitSpriteAnimationDisable(void *sprite);
void darnitSpriteDraw(void *sprite);

#endif
