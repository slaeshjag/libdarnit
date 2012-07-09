#include <darnit/darnit.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i, sfx, j, js0_x, js0_y, js1_x, js1_y;
	void *font, *surface, *handle, *text, *mapsheet, *sprite, *textinput, *mtsprite, *fps_text, *tilebuf;
	void *music;
	char test[256], fps[16];
	char *test_text;
	DARNIT_MOUSE mouse;
	DARNIT_TILEMAP *tilemap;
	DARNIT_KEYS keys;

	handle = darnitInit("TESTAPP - libDarnit");

	music = darnitSoundLoadTracked("latyl-greasy_duck_v1.mod", DARNIT_AUDIO_STREAM, DARNIT_AUDIO_STEREO);
	darnitSoundPlaybackStart(handle, music, 0, 15, 15, 0);

	test_text = malloc(64);
	font = darnitFontLoad(handle, "dejavu_sans.ttf", 20, 512, 512);
	surface = darnitMenuVerticalCreate(handle, "Hello\nGoodbye\nOther\nNothing\nLess than nothing", 50, 100, font, 200, 10, 3);

	sprite = darnitSpriteLoad(handle, "test.spr", 0, DARNIT_PFORMAT_RGB5A1);
	darnitSpriteMove(sprite , 50, 50);
	text = darnitTextSurfaceAlloc(font, 80, 800, 0, 460);

	mapsheet = darnitRenderTilesheetLoad(handle, "mapsheet.png", 32, 32, DARNIT_PFORMAT_RGBA8);
	tilemap = darnitRenderTilemapCreate(handle, "map.png", 10, mapsheet, DARNIT_TILEMAP_DEFAULT_MASK);
	darnitRenderTint(handle, 0.5f, 0.5f, 0.5f, 1.0f);
	darnitSpriteAnimationEnable(sprite);
	sprintf(test_text, "Héllo, world. Modify m€!");
	textinput = darnitMenuTextinputCreate(0, 0, font, test_text, 64, 200);

	mtsprite = darnitMTSpriteLoad(handle, "testspr.mts");
	darnitMTSpriteAnimationEnable(mtsprite);

	fps_text = darnitTextSurfaceAlloc(font, 16, 200, 0, 40);

	tilebuf = darnitRenderTileAlloc(1);
	darnitRenderTileMove(tilebuf, 0, mapsheet, 64, 64);
	darnitRenderTileSetTilesheetCoord(tilebuf, 0, mapsheet, 16, 16, 32, 32);

//	for (i = 0; i < 10; i++) 
//		darnitRenderTilemapTileSet(tilemap, i, 5, 2);

	for (i = j = 0;;) {
		keys = darnitButtonGet(handle);
		darnitTextSurfaceReset(text); darnitTextSurfaceReset(fps_text);
		mouse = darnitMouseGet(handle);
		darnitJoystickGet(handle, &js0_x, &js0_y, &js1_x, &js1_y);
		sprintf(test, "X: %i, Y: %i, W: %i;; TX: %i, TY: %i;; nub0 (%i,%i) ;; nub1(%i,%i)", mouse.x, mouse.y, mouse.wheel, i*4, j*4, js0_x, js0_y, js1_x, js1_y);
		sprintf(fps, "%i", darnitTimeLastFrameTook(handle));
		darnitTextSurfaceStringAppend(text, test);
		darnitTextSurfaceStringAppend(fps_text, "Hëllô ẇõŗłd¡ чайка");

		if (keys.lmb)
			darnitRenderFadeIn(handle, 1000, 1.0f, 0.0f, 0.0f);
		if (keys.rmb)
			darnitRenderFadeOut(handle, 1000);

		darnitRenderBegin();

		if (keys.left == 1)
			i--;
		if (keys.right == 1)
			i++;
		if (keys.up == 1)
			j--;
		if (keys.down == 1)
			j++;

		if (keys.r == 1)
			darnitRenderTilemapCameraMove(tilemap, i*4, j*4);
//		darnitRenderTint(handle, 1.0f, 1.0f, 1.0f, 1.0f);
		darnitRenderTilemap(handle, tilemap);
		darnitRenderBlendingEnable(handle);
		if (keys.l == 1)
			if (darnitMenuHandle(handle, surface) != -1)
				return 0;
		darnitMenuHandle(handle, textinput); 
		darnitSpriteDraw(sprite);
		darnitTextSurfaceDraw(text);
		darnitTextSurfaceDraw(fps_text);

		darnitRenderOffset(handle, -200, -200);
		darnitMTSpriteDraw(mtsprite);
		darnitRenderOffset(handle, 0, 0);

		darnitRenderBlendingDisable(handle);
		darnitRenderTileDraw(tilebuf, mapsheet, 1);
		darnitRenderEnd();
		darnitLoop(handle);
	}


	return 0;
}
