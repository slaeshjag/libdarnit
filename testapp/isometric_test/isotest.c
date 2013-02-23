#include <darnit/darnit.h>

int main(int argc, char **argv) {
	DARNIT_TILEMAP *tm;
	DARNIT_TILESHEET *ts;
	int i;

	d_init("isotest", "isotest", NULL);
	ts = d_render_tilesheet_isom_load("isosheet.png", 64, 32, DARNIT_PFORMAT_RGB5A1);
	tm = d_tilemap_isom_new(0, ts, 0xFF, 4, 4, 32);

	d_render_blend_enable();
	for (i = 0; i < 16; i++)
		tm->data[i] = i;
	d_tilemap_recalc(tm);
//	d_tilemap_camera_move(tm, -500, -500);
	
	for (i = 0;; i++) {
		d_tilemap_camera_move(tm, 0, 200 - i);
		d_render_begin();
		d_render_blend_enable();
		d_tilemap_draw(tm);
		d_render_end();
		d_loop();
	}

	d_quit();
}
