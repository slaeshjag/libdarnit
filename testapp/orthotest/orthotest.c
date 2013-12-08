#include <darnit/darnit.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv) {
	DARNIT_TILESHEET *ts;
	DARNIT_TILEMAP *tm;
	int i;

	d_init("orthotest", "orthotest", NULL);
	ts = d_render_tilesheet_load("grid.png", 24, 24, DARNIT_PFORMAT_RGB5A1);
	tm = d_tilemap_new(0xFFF, ts, 0xFFF, 10, 18);
	for (i = 0; i < 180; i++)
		tm->data[i] = 1;
	d_tilemap_recalc(tm);
	d_tilemap_camera_move(tm, -1, 0);


	for (;;) {
		d_render_begin();
		d_tilemap_draw(tm);
		d_render_end();
		d_loop();
	}

	d_quit();
}
