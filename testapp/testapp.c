#include "darner.h"


int main(int argc, char **argv) {
	int i, sfx;
	void *font, *surface, *handle;
	handle = darnerInit("TESTAPP - libDarner");
	
	font = darnerFontLoad(handle, "font.png", 10, 16, 4);
	surface = darnerMenuVerticalCreate(handle, "Hello\nGoodbye\nOther\nNothing\nLess than nothing", 50, 100, font, 200, 10);
//	sfx = darnerSFXLoad(handle, "test.sfx");
//	darnerMusicPlayMod(handle, "latyl-greasy_duck_v1.mod");
	darnerMusicPlayVorbis(handle, "test.ogg");
//	darnerSFXPlay(handle, sfx, 0, 80);


	for (;;) {
		darnerRenderBegin();
		if (darnerMenuHandle(handle, surface) != -1)
			return 0;
		darnerRenderEnd();
		darnerLoop(handle);
	}


	return 0;
}
