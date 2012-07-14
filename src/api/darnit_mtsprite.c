#include "darnit.h"


void EXPORT_THIS *darnitMTSpriteNew(void *tilesheet) {
	return mtSpriteNew(0, 0, tilesheet);
}


void EXPORT_THIS *darnitMTSpriteLoad(void *handle, const char *fname) {
	return mtSpriteLoad(handle, fname);
}


void EXPORT_THIS darnitMTSpriteAnimationEnable(void *mtsprite) {
	mtSpriteEnableAnimation(mtsprite);

	return;
}


void EXPORT_THIS darnitMTSpriteAnimationPause(void *mtsprite) {
	mtSpritePauseAnimation(mtsprite);

	return;
}


void EXPORT_THIS darnitMTSpriteTileAdd(void *sprite, int x, int y, int w, int h, int rx, int ry) {
	mtSpriteAddTile(sprite, x, y, w, h, rx, ry);

	return;
}


void EXPORT_THIS darnitMTSpriteFrameSet(void *sprite, int time) {
	mtSpriteSetAsFrame(sprite, time);

	return;
}


void EXPORT_THIS darnitMTSpriteAnimationDisable(void *mtsprite) {
	mtSpriteDisableAnimation(mtsprite);

	return;
}


void EXPORT_THIS darnitMTSpriteDraw(void *mtsprite) {
	mtSpriteDraw(mtsprite);

	return;
}


void EXPORT_THIS *darnitMTSpriteDelete(void *handle, void *mtsprite) {
	return mtSpriteDelete(handle, mtsprite);
}