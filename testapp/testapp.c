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
	DARNIT_MAP *map;

	handle = darnitInit("TESTAPP - libDarnit", "testapp");

	music = darnitSoundLoadTracked("latyl-greasy_duck_v1.mod", DARNIT_AUDIO_STREAM, DARNIT_AUDIO_STEREO);
	darnitSoundPlaybackStart(music, 0, 127, 127, 0);

	test_text = malloc(64);
	font = darnitFontLoad("dejavu_sans.ttf", 28, 512, 512);
	surface = darnitMenuVerticalCreate("Hello\nGoodbye\nOther\nNothing\nLess than nothing", 50, 100, font, 200, 10, 3);

	sprite = darnitSpriteLoad("test.spr", 0, DARNIT_PFORMAT_RGB5A1);
	darnitSpriteMove(sprite, 50, 50);
	text = darnitTextSurfaceAlloc(font, 80, 800, 0, 460);

	mapsheet = darnitRenderTilesheetLoad("mapsheet.png", 32, 32, DARNIT_PFORMAT_RGBA8);
	if ((map = darnitMapLoad("testmap.ldmz")) == NULL)
		fprintf(stderr, "Map load failed\n");
//	tilemap = darnitRenderTilemapCreate("map.png", 10, mapsheet, DARNIT_TILEMAP_DEFAULT_MASK);
//	darnitRenderTint(handle, 0.5f, 0.5f, 0.5f, 1.0f);
	darnitSpriteAnimationEnable(sprite);
	sprintf(test_text, "Héllo, world. Modify m€! Test of offsets");
	textinput = darnitMenuTextinputCreate(0, 0, font, test_text, 64, 200);

	mtsprite = darnitMTSpriteLoad("testspr.mts");
	darnitMTSpriteAnimationEnable(mtsprite);

	fps_text = darnitTextSurfaceAlloc(font, 16, 200, 0, 40);

	tilebuf = darnitRenderTileAlloc(1);
	darnitRenderTileMove(tilebuf, 0, mapsheet, 64, 64);
	darnitRenderTileSetTilesheetCoord(tilebuf, 0, mapsheet, 16, 16, 32, 32);
	fprintf(stderr, "String lenght: %i\n", darnitFontGetStringWidthPixels(font, "ASDFÅÄÖ,,"));

//	for (i = 0; i < 10; i++) 
//		darnitRenderTilemapTileSet(tilemap, i, 5, 2);

	for (i = j = 0;;) {
		keys = darnitButtonGet();
		darnitTextSurfaceReset(text); darnitTextSurfaceReset(fps_text);
		mouse = darnitMouseGet();
		darnitJoystickGet(&js0_x, &js0_y, &js1_x, &js1_y);
		sprintf(test, "X: %i, Y: %i, W: %i;; TX: %i, TY: %i;; nub0 (%i,%i) ;; nub1(%i,%i)", mouse.x, mouse.y, mouse.wheel, i*4, j*4, js0_x, js0_y, js1_x, js1_y);
		sprintf(fps, "%i", darnitTimeLastFrameTook());
		darnitTextSurfaceStringAppend(text, test);
		darnitTextSurfaceStringAppend(fps_text, "ASDFÅÄÖ,,");

		if (keys.lmb)
			darnitRenderFadeIn(1000, 1.0f, 0.0f, 0.0f);
		if (keys.rmb)
			darnitRenderFadeOut(1000);

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
			darnitRenderTilemapCameraMove(map->layer->tilemap, i*4, j*4);
//		darnitRenderTint(handle, 1.0f, 1.0f, 1.0f, 1.0f);
		darnitRenderTilemap(map->layer->tilemap);
		darnitRenderBlendingEnable();
		if (keys.l == 1)
			if (darnitMenuHandle(surface) != -1)
				return 0;
		darnitMenuHandle(textinput); 
		darnitSpriteDraw(sprite);
		darnitTextSurfaceDraw(text);
		darnitTextSurfaceDraw(fps_text);

		darnitRenderOffset(-200, -200);
		darnitMTSpriteDraw(mtsprite);
		darnitRenderOffset(0, 0);

		darnitRenderBlendingDisable();
		darnitRenderTileDraw(tilebuf, mapsheet, 1);
		darnitRenderEnd();
		darnitLoop();
	}


	return 0;
}
