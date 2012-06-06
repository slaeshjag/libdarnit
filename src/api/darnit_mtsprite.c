#include "darnit.h"


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
