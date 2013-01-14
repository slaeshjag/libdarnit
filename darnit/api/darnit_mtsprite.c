#include "darnit.h"


void EXPORT_THIS *d_mtsprite_new(void *tilesheet) {
	return mtSpriteNew(0, 0, tilesheet);
}


void EXPORT_THIS *d_mtsprite_load(const char *fname) {
	return mtSpriteLoad(fname);
}


void EXPORT_THIS d_mtsprite_animate_start(void *mtsprite) {
	mtSpriteEnableAnimation(mtsprite);

	return;
}


void EXPORT_THIS d_mtsprite_animate_pause(void *mtsprite) {
	mtSpritePauseAnimation(mtsprite);

	return;
}


void EXPORT_THIS d_mtsprite_tile_add(void *sprite, int x, int y, int w, int h, int rx, int ry) {
	mtSpriteAddTile(sprite, x, y, w, h, rx, ry);

	return;
}


void EXPORT_THIS d_mtsprite_frame_set(void *sprite, int time) {
	mtSpriteSetAsFrame(sprite, time);

	return;
}


void EXPORT_THIS d_mtsprite_animate_stop(void *mtsprite) {
	mtSpriteDisableAnimation(mtsprite);

	return;
}


void EXPORT_THIS d_mtsprite_animate_repeat(void *mtsprite, int repeat) {
	mtSpriteSetRepeat(mtsprite, repeat);

	return;
}


void EXPORT_THIS d_mtsprite_draw(void *mtsprite) {
	mtSpriteDraw(mtsprite);

	return;
}


void EXPORT_THIS *d_mtsprite_delete(void *mtsprite) {
	return mtSpriteDelete(mtsprite);
}
