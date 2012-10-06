#include "darnit.h"


void EXPORT_THIS *darnitSpriteNew(void *tilesheet) {
	return spriteNew(tilesheet);
}


void EXPORT_THIS *darnitSpriteLoad(const char *fname, int dir, unsigned int target_format) {
	return spriteLoad(fname, dir, target_format);
}


void EXPORT_THIS *darnitSpriteDelete(void *sprite) {
	return spriteDelete(sprite);
}


void EXPORT_THIS darnitSpriteDirectionChange(void *sprite, int dir) {
	spriteChangeDirection(sprite, dir);

	return;
}


void EXPORT_THIS darnitSpriteActivate(void *sprite, int dir) {
	spriteActivate(sprite, dir);

	return;
}


void EXPORT_THIS darnitSpriteSetFrame(void *sprite, int frame) {
	spriteSetFrame(sprite, frame);

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


void EXPORT_THIS darnitSpriteFrameEntrySet(void *sprite, int dir, int frame, int tile, int time) {
	spriteSetFrameEntry(sprite, dir, frame, tile, time);

	return;
}


int EXPORT_THIS darnitSpriteWidth(void *sprite) {
	SPRITE_ENTRY *sprite_e = sprite;
	if (sprite == NULL)
		return 0;
	return sprite_e->ts->wsq;
}



int EXPORT_THIS darnitSpriteHeight(void *sprite) {
	SPRITE_ENTRY *sprite_e = sprite;
	if (sprite == NULL)
		return 0;
	return sprite_e->ts->hsq;
}
