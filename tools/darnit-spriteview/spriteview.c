#include <stdlib.h>
#include <stdio.h>
#include <darnit/darnit.h>

int main(int argc, char **argv) {
	DARNIT_SPRITE *sprite;

	if (argc <3) {
		fprintf(stdout, "Usage: %s <sprite> <direction>\n", argv[0]);
		return -1;
	}

	if (d_init("libDarnit spriteview", "spriteview", NULL) == NULL) {
		fprintf(stderr, "libDarnit failed to init\n");
		return -1;
	}

	if ((sprite = d_sprite_load(argv[1], atoi(argv[2]), DARNIT_PFORMAT_RGB5A1)) == NULL) {
		fprintf(stderr, "Unable to open sprite %s\n", argv[1]);
		d_quit();
		return -1;
	}

	d_sprite_move(sprite, 0, 0);
	d_sprite_animate_start(sprite);
	d_render_blend_enable();
	
	for (;;) {
		d_render_begin();
		d_sprite_draw(sprite);
		d_render_end();
		d_loop();
	}

	return 0;
}
