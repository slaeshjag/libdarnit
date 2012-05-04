#include "darnit.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i, sfx, j;
	void *font, *surface, *handle, *text, *mapsheet, *sprite;
	char test[32];
	DARNIT_MOUSE mouse;
	DARNIT_TILEMAP tilemap;
	DARNIT_KEYS keys;

	handle = darnitInit("TESTAPP - libDarner");
	
	font = darnitFontLoad(handle, "font.png", 10, 16, 4);
	surface = darnitMenuVerticalCreate(handle, "Hello\nGoodbye\nOther\nNothing\nLess than nothing", 50, 100, font, 200, 10, 0);

	sprite = darnitSpriteLoad(handle, "test.spr", 0, DARNIT_PFORMAT_RGB5A1);
	darnitSpriteMove(sprite , 50, 50);
	text = darnitTextSurfaceAlloc(font, 32, 32, 0, 464);

	mapsheet = darnitRenderTilesheetLoad(handle, "mapsheet.png", 32, 32, DARNIT_PFORMAT_RGBA8);
	tilemap = darnitRenderTilemapCreate(handle, "map.png", 10, mapsheet);
//	darnitRenderTint(handle, 0.5f, 0.5f, 0.5f, 1.0f);
	darnitSpriteAnimationEnable(sprite);

	for (i = 0; i < 10; i++) 
		darnitRenderTilemapTileSet(tilemap, i, 5, 2);

	for (i = 0;;) {
		keys = darnitButtonGet(handle);
		darnitTextSurfaceReset(text);
		mouse = darnitMouseGet(handle);
		sprintf(test, "X: %i, Y: %i, W: %i", mouse.x, mouse.y, mouse.wheel);
		darnitTextSurfaceStringAppend(text, test);

		darnitRenderBegin();
		darnitTextSurfaceDraw(text);

		if (keys.left == 1)
			i--;
		if (keys.right == 1)
			i++;
		if (keys.up == 1)
			j--;
		if (keys.down == 1)
			j++;

		if (keys.r == 0)
			darnitRenderTilemapCameraMove(tilemap, i*4, j*4);
		darnitRenderTilemap(handle, tilemap);
		darnitRenderBlendingEnable(handle);
		if (keys.l == 1)
			if (darnitMenuHandle(handle, surface) != -1)
				return 0;
		darnitSpriteDraw(sprite);
		darnitRenderBlendingDisable(handle);
		darnitRenderEnd();
		darnitLoop(handle);
	}


	return 0;
}
