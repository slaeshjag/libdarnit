#include <darnit/darnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void colorTest(DARNIT_TEXT_SURFACE *surface) {
	int i;
	char tmp[2];
	d_text_surface_reset(surface);
	srand(time(NULL));
	
	for (i = 0; i < 16; i++) {
		d_text_surface_color_next(surface, rand()&0xFF, rand()&0xFF, rand()&0xFF);
		sprintf(tmp, "%c", 'A' + i);
		d_text_surface_char_append(surface, tmp);
	}

	return;
}
	


int main(int argc, char **argv) {
	int i, j, js0_x, js0_y, js1_x, js1_y;
	void *font, *surface, *text, *mapsheet, *sprite, *textinput, *mtsprite, *fps_text, *tilebuf, *fancy_text;
	void *music;
	char test[256], fps[16];
	char *test_text;
	DARNIT_MOUSE mouse;
	DARNIT_KEYS keys;
	DARNIT_MAP *map;

	if (!d_init("TESTAPP - libDarnit", "testapp", NULL))
		return -1;

	/* COmpressed writes test */
	DARNIT_FILE *f;
	char bzval[64];

	f = d_file_open("test.bz2", "wb+");
	d_file_write_compressed(f, "Fiskpinnar", strlen("Fiskpinnar") + 1);
	d_file_write("Fiskpinnar", strlen("Fiskpinnar") + 1, f);
	d_file_close(f);
	f = d_file_open("test.bz2", "rb");
	d_file_read_compressed(f, bzval, strlen("Fiskpinnar") + 1);
	if (strcmp(bzval, "Fiskpinnar"))
		fprintf(stderr, "Compression test failed\n");
	else {
		d_file_read(bzval, strlen("Fiskpinnar") + 1, f);
		if (strcmp(bzval, "Fiskpinnar"))
			fprintf(stderr, "Post-compression test failed\n");
		else
			fprintf(stderr, "Compression test passed\n");
	}



	music = d_sound_tracked_load("latyl-greasy_duck_v1.mod", DARNIT_AUDIO_STREAM, DARNIT_AUDIO_STEREO);
	d_sound_play(music, 0, 127, 127, 0);
	d_sound_play(music, 0, 127, 127, 0);
	d_sound_play(music, 0, 127, 127, 0);
	d_sound_play(music, 0, 127, 127, 0);
	d_sound_play(music, 0, 127, 127, 0);

	test_text = malloc(64);
	font = d_font_load("dejavu_sans.ttf", 28, 512, 512);
	surface = d_menu_vertical_new("Hello\nGoodbye\nOther\nNothing\nLess than nothing", 50, 100, font, 200, 10, 3);

	sprite = d_sprite_load("test.spr", 0, DARNIT_PFORMAT_RGB5A1);
	d_sprite_move(sprite, 50, 50);
	text = d_text_surface_new(font, 80, 800, 0, 460);
	fancy_text = d_text_surface_color_new(font, 16, 800, 0, 420);
	colorTest(fancy_text);

	mapsheet = d_render_tilesheet_load("mapsheet.png", 32, 32, DARNIT_PFORMAT_RGBA8);
	if ((map = d_map_load("testmap_iso.ldmz")) == NULL)
		fprintf(stderr, "Map load failed\n");
//	tilemap = darnitRenderTilemapCreate("map.png", 10, mapsheet, DARNIT_TILEMAP_DEFAULT_MASK);
//	darnitRenderTint(handle, 0.5f, 0.5f, 0.5f, 1.0f);
	d_sprite_animate_start(sprite);
	sprintf(test_text, "Héllo, world. Modify m€! Test of offsets");
	textinput = d_menu_textinput_new(0, 0, font, test_text, 64, 200);

	mtsprite = d_mtsprite_load("testspr.mts");
	d_mtsprite_animate_start(mtsprite);

	fps_text = d_text_surface_new(font, 16, 200, 0, 40);

	tilebuf = d_render_tile_new(1, mapsheet);
	d_render_tile_move(tilebuf, 0, 64, 64);
	d_render_tile_tilesheet_coord_set(tilebuf, 0, 16, 16, 32, 32);
	fprintf(stderr, "String lenght: %i\n", d_font_string_w(font, "ASDFÅÄÖ,,"));

//	for (i = 0; i < 10; i++) 
//		darnitRenderTilemapTileSet(tilemap, i, 5, 2);

	for (i = j = 0;;) {
		keys = d_keys_get();
		d_text_surface_reset(text); d_text_surface_reset(fps_text);
		mouse = d_mouse_get();
		d_joystick_get(&js0_x, &js0_y, &js1_x, &js1_y);
		sprintf(test, "X: %i, Y: %i, W: %i;; TX: %i, TY: %i;; nub0 (%i,%i) ;; nub1(%i,%i)", mouse.x, mouse.y, mouse.wheel, i*4, j*4, js0_x, js0_y, js1_x, js1_y);
		sprintf(fps, "%i", d_last_frame_time());
		d_text_surface_string_append(text, test);
		d_text_surface_string_append(fps_text, "ASDFÅÄÖ,,");

		if (keys.lmb)
			d_render_fade_in(1000, 1.0f, 0.0f, 0.0f);
		if (keys.rmb)
			d_render_fade_out(1000);

		d_render_begin();

		if (keys.left == 1)
			i--;
		if (keys.right == 1)
			i++;
		if (keys.up == 1)
			j--;
		if (keys.down == 1)
			j++;
		if (keys.select)
			fprintf(stderr, "Blololol\n");

		if (keys.r == 1)
			d_tilemap_camera_move(map->layer->tilemap, i*4, j*4);
		d_render_blend_enable();
		d_tilemap_draw(map->layer->tilemap);
		if (keys.l == 1)
			if (d_menu_loop(surface) != -1)
				return 0;
		d_menu_loop(textinput); 
		d_sprite_draw(sprite);
		d_text_surface_draw(text);
		d_text_surface_draw(fancy_text);
		d_text_surface_draw(fps_text);

		d_render_offset(-200, -200);
		d_mtsprite_draw(mtsprite);
		d_render_offset(0, 0);

		d_render_blend_disable();
		d_render_tile_draw(tilebuf, 1);
		d_render_end();
		d_loop();
	}


	return 0;
}
