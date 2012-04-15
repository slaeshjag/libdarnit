#include "darner.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i, sfx, sprite, j;
	void *font, *surface, *handle, *text, *mapsheet;
	char test[32];
	DARNER_MOUSE mouse;
	handle = darnerInit("TESTAPP - libDarner");
	TILEMAP tilemap;
	
	font = darnerFontLoad(handle, "font.png", 10, 16, 4);
	surface = darnerMenuVerticalCreate(handle, "Hello\nGoodbye\nOther\nNothing\nLess than nothing", 50, 100, font, 200, 10);
//	darnerMusicPlayVorbis(handle, "test.ogg");

	sprite = darnerSpriteLoad(handle, "test.spr", 0);
	darnerSpriteMove(handle, sprite , 50, 50, 0);
	darnerSpriteAnimationEnable(handle, sprite);
//	darnerRenderOffset(handle, 100, -100);
	text = darnerTextSurfaceAlloc(font, 32, 32, 0, 464);

	mapsheet = darnerRenderTilesheetLoad(handle, "mapsheet.png", 32, 32);
	tilemap = darnerRenderTilemapCreate(handle, "map.png", 10, mapsheet);

	for (i = 0;; i++) {
		darnerTextSurfaceReset(text);
		mouse = darnerMouseGet(handle);
		sprintf(test, "X: %i, Y: %i, W: %i", mouse.x, mouse.y, mouse.wheel);
		darnerTextSurfaceStringAppend(text, test);

		darnerRenderBegin();
		darnerTextSurfaceDraw(text);
		darnerSpriteLayerDraw(handle, 0);

		if ((i % 100) >= 50)
			j = (100 - (i % 100)) * 8;
		else
			j = (i % 100) * 8;
		darnerRenderOffset(handle, -1*j, j);
//		fprintf(stderr, "Rendering at offset %i %i\n", j, j);
		darnerRenderTilemapCameraMove(tilemap, j, j);
		darnerRenderTilemap(tilemap);
		darnerRenderOffset(handle, 0, 0);
		if (darnerMenuHandle(handle, surface) != -1)
			return 0;
		darnerRenderEnd();
		darnerLoop(handle);
	}


	return 0;
}
