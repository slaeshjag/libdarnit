#include "darnit.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i, sfx, sprite, j;
	void *font, *surface, *handle, *text, *mapsheet;
	char test[32];
	DARNIT_MOUSE mouse;
	handle = darnitInit("TESTAPP - libDarner");
	TILEMAP tilemap;
	
	font = darnitFontLoad(handle, "font.png", 10, 16, 4);
	surface = darnitMenuVerticalCreate(handle, "Hello\nGoodbye\nOther\nNothing\nLess than nothing", 50, 100, font, 200, 10, 0);
//	darnitMusicPlayVorbis(handle, "test.ogg");

	sprite = darnitSpriteLoad(handle, "test.spr", 0);
	darnitSpriteMove(handle, sprite , 50, 50, 0);
	darnitSpriteAnimationEnable(handle, sprite);
//	darnitRenderOffset(handle, 100, -100);
	text = darnitTextSurfaceAlloc(font, 32, 32, 0, 464);

	mapsheet = darnitRenderTilesheetLoad(handle, "mapsheet.png", 32, 32);
	tilemap = darnitRenderTilemapCreate(handle, "map.png", 10, mapsheet);

	for (i = 0;; i++) {
		darnitTextSurfaceReset(text);
		mouse = darnitMouseGet(handle);
		sprintf(test, "X: %i, Y: %i, W: %i", mouse.x, mouse.y, mouse.wheel);
		darnitTextSurfaceStringAppend(text, test);

		darnitRenderBegin();
		darnitTextSurfaceDraw(text);
		darnitSpriteLayerDraw(handle, 0);

		if ((i % 100) >= 50)
			j = (100 - (i % 100)) * 8;
		else
			j = (i % 100) * 8;
		darnitRenderOffset(handle, j, j);
//		fprintf(stderr, "Rendering at offset %i %i\n", j, j);
		darnitRenderTilemapCameraMove(tilemap, j, j);
		darnitRenderTilemap(tilemap);
		darnitRenderOffset(handle, 0, 0);
		darnitRenderBlendingEnable();
		if (darnitMenuHandle(handle, surface) != -1)
			return 0;
		darnitRenderBlendingDisable();
		darnitRenderEnd();
		darnitLoop(handle);
	}


	return 0;
}
