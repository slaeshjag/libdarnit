#include <darnit/darnit.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	DARNIT_SPRITE *sprite;

	if (argc <3) {
		fprintf(stdout, "Usage: %s <sprite> <direction>\n", argv[0]);
		return -1;
	}

	if (darnitInit("libDarnit spriteview", "spriteview") == NULL) {
		fprintf(stderr, "libDarnit failed to init\n");
		return -1;
	}

	if ((sprite = darnitSpriteLoad(argv[1], atoi(argv[2]), DARNIT_PFORMAT_RGB5A1)) == NULL) {
		fprintf(stderr, "Unable to open sprite %s\n", argv[1]);
		darnitQuit();
		return -1;
	}

	darnitSpriteMove(sprite, 0, 0);
	darnitSpriteAnimationEnable(sprite);

	for (;;) {
		darnitRenderBegin();
		darnitSpriteDraw(sprite);
		darnitRenderEnd();
		darnitLoop();
	}

	return 0;
}
