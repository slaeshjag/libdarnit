#include "darnit.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i, sfx, j;
	void *font, *surface, *handle, *text, *mapsheet, *sprite, *textinput, *mtsprite, *fps_text, *tilebuf;
	void *music;
	char test[256], fps[16];
	char *test_text;
	DARNIT_MOUSE mouse;
	DARNIT_TILEMAP *tilemap;
	DARNIT_KEYS keys;

	handle = darnitInit("TESTAPP - libDarner");

//	music = darnitSoundLoadTracked("latyl-greasy_duck_v1.mod", 0, 2);
//	darnitSoundPlaybackStart(handle, music, 2, 0, 127, 127, 0);

	test_text = malloc(64);
	font = darnitFontLoad(handle, "font.png", 10, 16, 4);
	surface = darnitMenuVerticalCreate(handle, "Hello\nGoodbye\nOther\nNothing\nLess than nothing", 50, 100, font, 200, 10, 0);

	sprite = darnitSpriteLoad(handle, "test.spr", 0, DARNIT_PFORMAT_RGB5A1);
	darnitSpriteMove(sprite , 50, 50);
	text = darnitTextSurfaceAlloc(font, 80, 80, 0, 464);

	mapsheet = darnitRenderTilesheetLoad(handle, "mapsheet.png", 32, 32, DARNIT_PFORMAT_RGBA8);
	tilemap = darnitRenderTilemapCreate(handle, "map.png", 10, mapsheet);
//	darnitRenderTint(handle, 0.5f, 0.5f, 0.5f, 1.0f);
	darnitSpriteAnimationEnable(sprite);
	sprintf(test_text, "Hello, world. Modify me!");
	textinput = darnitMenuTextinputCreate(0, 0, font, test_text, 64, 16);

	mtsprite = darnitMTSpriteLoad(handle, "testspr.mts");
	darnitMTSpriteAnimationEnable(mtsprite);

	fps_text = darnitTextSurfaceAlloc(font, 16, 16, 730, 0);

	tilebuf = darnitRenderTileAlloc(1);
	darnitRenderTileMove(tilebuf, 0, mapsheet, 64, 64);
	darnitRenderTileSetTilesheetCoord(tilebuf, 0, mapsheet, 16, 16, 32, 32);

//	for (i = 0; i < 10; i++) 
//		darnitRenderTilemapTileSet(tilemap, i, 5, 2);

	for (i = j = 0;;) {
		keys = darnitButtonGet(handle);
		darnitTextSurfaceReset(text); darnitTextSurfaceReset(fps_text);
		mouse = darnitMouseGet(handle);
		sprintf(test, "X: %i, Y: %i, W: %i;; TX: %i, TY: %i;; %s", mouse.x, mouse.y, mouse.wheel, i*4, j*4, test_text);
		sprintf(fps, "%i", darnitTimeLastFrameTook(handle));
		darnitTextSurfaceStringAppend(text, test);
		darnitTextSurfaceStringAppend(fps_text, fps);

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
