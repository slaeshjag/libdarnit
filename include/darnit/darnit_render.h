/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_render.h' - This file is part of libdarnit

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/


#ifndef __DARNIT_RENDER_H__
#define	__DARNIT_RENDER_H__

typedef enum {
	DARNIT_RENDER_LOGIC_OP_NONE	= 0,
	DARNIT_RENDER_LOGIC_OP_AND	= 1,
	DARNIT_RENDER_LOGIC_OP_NAND	= 2,
	DARNIT_RENDER_LOGIC_OP_OR	= 3,
	DARNIT_RENDER_LOGIC_OP_NOR	= 4,
	DARNIT_RENDER_LOGIC_OP_XOR	= 5,
} DARNIT_RENDER_LOGIC_OP;


typedef enum {
	DARNIT_SCALE_NEAREST		= 1,
	DARNIT_SCALE_LINEAR		= 2,
} DARNIT_RENDER_SCALE;

typedef void DARNIT_TILESHEET;
typedef void DARNIT_TILE;
typedef void DARNIT_LINE;
typedef void DARNIT_CIRCLE;
typedef void DARNIT_RECT;


DARNIT_TILESHEET *d_render_tilesheet_load(const char *fname, unsigned int wsq, unsigned int hsq, DARNIT_PFORMAT target_format);
DARNIT_TILESHEET *d_render_tilesheet_new(int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format);
DARNIT_TILESHEET *d_render_tilesheet_isom_load(const char *fname, unsigned int wsq, unsigned int hsq, DARNIT_PFORMAT target_format);
void d_render_tilesheet_update(DARNIT_TILESHEET *tilesheet, int sheet_x, int sheet_y, int change_w, int change_h, void *data);
void d_render_tilesheet_geometrics(DARNIT_TILESHEET *tilesheet, int *w, int *h, int *tile_w, int *tile_h);
DARNIT_TILESHEET *d_render_tilesheet_free(DARNIT_TILESHEET *tilesheet);

void d_render_tile_init(DARNIT_TILE *buf, unsigned int tile, unsigned int tile_ts, int x, int y); 
void d_render_tile_move(DARNIT_TILE *tile_p, unsigned int tile, int x, int y);
void d_render_tile_tilesheet_coord_set(DARNIT_TILE *tile_p, unsigned int tile, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void d_render_tile_size_set(DARNIT_TILE *buf, unsigned int tile, int w, int h);
void d_render_tile_set(DARNIT_TILE *tile_p, unsigned int tile, unsigned int tile_ts);
void d_render_tile_tilesheet(DARNIT_TILE *tile_p, DARNIT_TILESHEET *tilesheet);
void d_render_tile_clear(DARNIT_TILE *tile_p, unsigned int tile);
void d_render_tile_draw(DARNIT_TILE *tile_p, unsigned int tiles);
DARNIT_TILE *d_render_tile_new(unsigned int tiles, DARNIT_TILESHEET *tilesheet);
DARNIT_TILE *d_render_tile_free(DARNIT_TILE *tile_p);

DARNIT_LINE *d_render_line_new(unsigned int lines, unsigned int line_w);
void d_render_line_move(DARNIT_LINE *line_p, unsigned int line, int x1, int y1, int x2, int y2);
void d_render_line_draw(DARNIT_LINE *line_p, int lines);
DARNIT_LINE *d_render_line_free(DARNIT_LINE *line_p);

DARNIT_RECT *d_render_rect_new(unsigned int rects);
void d_render_rect_move(DARNIT_RECT *rect_p, unsigned int rect, int x1, int y1, int x2, int y2);
void d_render_rect_draw(DARNIT_RECT *rect_p, int rects);
void d_render_rect_get(DARNIT_RECT *buf, unsigned int rect, int *x1, int *y1, int *x2, int *y2);
DARNIT_RECT *d_render_rect_free(DARNIT_RECT *rect_p);

DARNIT_CIRCLE *d_render_circle_new(unsigned int lines, unsigned int line_w);
void d_render_circle_move(DARNIT_CIRCLE *circle_p, int x, int y, int radius);
void d_render_circle_draw(DARNIT_CIRCLE *circle_p);
DARNIT_CIRCLE *d_render_circle_free(DARNIT_CIRCLE *circle_p);

void d_render_line_get(DARNIT_LINE *buf, unsigned int line, int *x, int *y, int *x2, int *y2);
void d_render_begin();
void d_render_end();
void d_render_blend_enable();
void d_render_blend_disable();
void d_render_tint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void d_render_tint_get(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a);
void d_render_clearcolor_set(unsigned char r, unsigned char g, unsigned char b);
void d_render_offset(int x, int y);

void d_render_fade_in(unsigned int time, unsigned char r, unsigned char g, unsigned char b);
void d_render_fade_out(unsigned int time);
int d_render_fade_status();
void d_render_state_restore();

int d_render_tilesheet_animation_apply(DARNIT_TILESHEET *tilesheet, const char *fname);
void d_render_tilesheet_animate(DARNIT_TILESHEET *tilesheet);

void d_render_logic_op(DARNIT_RENDER_LOGIC_OP logicop);
void d_render_tilesheet_scale_algorithm(DARNIT_TILESHEET *tilesheet, DARNIT_RENDER_SCALE scaling);










/* Please don't use this function. I hate it <.< */
void d_render_tile_blit(DARNIT_TILESHEET *tilesheet, unsigned int tile, int x, int y);


#endif
