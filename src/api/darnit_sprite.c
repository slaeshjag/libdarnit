#include "darnit.h"


void EXPORT_THIS *darnitSpriteLoad(void *handle, const char *fname, int dir, unsigned int target_format) {
	return spriteLoad(handle, fname, dir, target_format);
}


void EXPORT_THIS *darnitSpriteDelete(void *handle, void *sprite) {
	return spriteDelete(handle, sprite);
}


void EXPORT_THIS darnitSpriteDirectionChange(void *sprite, int dir) {
	spriteChangeDirection(sprite, dir);

	return;
}


void EXPORT_THIS darnitSpriteMove(void *sprite, int x, int y) {
	spriteTeleport(sprite, x, y);

	return;
}


void EXPORT_THIS darnitSpriteAnimationEnable(void *sprite) {
	spriteEnableAnimation(sprite);

	return;
}


void EXPORT_THIS darnitSpriteAnimationPause(void *sprite) {
	spritePauseAnimation(sprite);

	return;
}


void EXPORT_THIS darnitSpriteAnimationDisable(void *sprite) {
	spriteDisableAnimation(sprite);

	return;
}


void EXPORT_THIS darnitSpriteDraw(void *sprite) {
	spriteDraw(sprite);

	return;
}

