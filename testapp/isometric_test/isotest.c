#include <darnit/darnit.h>

int main(int argc, char **argv) {
	DARNIT_TILEMAP *tm;
	DARNIT_TILESHEET *ts;
	int i, j;

	d_init("isotest", "isotest", NULL);
	ts = d_render_tilesheet_isom_load("isosheet.png", 64, 64, DARNIT_PFORMAT_RGB5A1);
	tm = d_tilemap_isom_new(0, ts, 0xFF, 32, 16, 32);

	d_render_blend_enable();
//	for (i = 0; i < 32*8; i++)
//		tm->data[i] = 67;
		tm->data[0] = 50;
	d_tilemap_recalc(tm);
//	d_tilemap_camera_move(tm, -500, -500);
	
	for (i = 0;; i++) {
		j = i % 1200;
		if (j > 600)
			j = j + (2*(j - 600)) * -1;
		j *= -1;
		j += 300;
		d_tilemap_camera_move(tm, 0, 0);
		d_render_begin();
		d_render_blend_enable();
		d_tilemap_draw(tm);
		d_render_end();
		d_loop();
	}

	d_quit();
}
