/*
Copyright (c) 2011-2013 Steven Arnow
'render_tilemap.c' - This file is part of libdarnit

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


#include "darnit.h"

#ifndef DARNIT_HEADLESS


void renderTilemapISOCoordinates(RENDER_TILEMAP *tm, int x, int y, int *r_x, int *r_y) {
	int origin = tm->ts->wsq * tm->w / 2;

	isometricFromScreen(x, y, origin, 0, tm->ts->w, tm->r_h, r_x, r_y);

	return;
}


int renderTilemapRecalcISO(RENDER_TILEMAP *tm) {
	float x_start, y_start, x_pos, y_pos;
	float x_step, y_step;
	int i, j, k, t, x_cur, y_cur, origin_x, origin_y;


	x_start = -1.0f;
	y_start = 1.0f;
	x_step = d->video.swgran * tm->ts->wsq / 2;
	y_step = d->video.shgran * tm->r_h / 2;
	y_cur = tm->cam_yi - (tm->map_w - 1);
	renderTilemapISOCoordinates(tm, tm->cam_xi, tm->cam_yi, &origin_x, &origin_y);
	origin_y--;
	x_cur = origin_x;

	for (i = k = 0; i < tm->w; i++, x_cur++) {
		y_cur = origin_y;
		x_start = x_step * i - 1.0f + x_step * 2;
		y_start = 1.0f;
		if (y_cur & 0x1)
			y_start += y_step;

		if (x_cur < 0)
			continue;
		if (x_cur >= tm->map_w)
			break;
		for (j = 0; j < tm->h; j++, y_cur++) {
			if (y_cur < 0)
				continue;
			if (y_cur >= tm->map_h)
				break;
			t = tm->map[x_cur + y_cur * tm->map_w] & tm->mask;
			if (t > tm->ts->tiles)
				continue;
			if (tm->inv_div != 0)
				if (t % tm->inv_div == 0)
					continue;
			y_pos = y_start + tm->ts->tile[t].h_p - tm->ts->sh;
			x_pos = x_start - x_step;
			
			/* Rurgh... */
			tm->cache[k].vertex[0].coord.x = x_pos;
			tm->cache[k].vertex[0].coord.y = y_pos;
			tm->cache[k].vertex[1].coord.x = x_pos + x_step;
			tm->cache[k].vertex[1].coord.y = y_pos;
			tm->cache[k].vertex[2].coord.x = x_pos + x_step;
			tm->cache[k].vertex[2].coord.y = y_pos - tm->ts->tile[t].h_p;
			tm->cache[k].vertex[3].coord.x = x_pos + x_step;
			tm->cache[k].vertex[3].coord.y = y_pos - tm->ts->tile[t].h_p;
			tm->cache[k].vertex[4].coord.x = x_pos;
			tm->cache[k].vertex[4].coord.y = y_pos - tm->ts->tile[t].h_p;
			tm->cache[k].vertex[5].coord.x = x_pos;
			tm->cache[k].vertex[5].coord.y = y_pos;
			tm->cache[k].vertex[0].tex.u = tm->ts->tile[t].r;
			tm->cache[k].vertex[0].tex.v = tm->ts->tile[t].s;
			tm->cache[k].vertex[1].tex.u = tm->ts->tile[t].u;
			tm->cache[k].vertex[1].tex.v = tm->ts->tile[t].s;
			tm->cache[k].vertex[2].tex.u = tm->ts->tile[t].u;
			tm->cache[k].vertex[2].tex.v = tm->ts->tile[t].v;
			tm->cache[k].vertex[3].tex.u = tm->ts->tile[t].u;
			tm->cache[k].vertex[3].tex.v = tm->ts->tile[t].v;
			tm->cache[k].vertex[4].tex.u = tm->ts->tile[t].r;
			tm->cache[k].vertex[4].tex.v = tm->ts->tile[t].v;
			tm->cache[k].vertex[5].tex.u = tm->ts->tile[t].r;
			tm->cache[k].vertex[5].tex.v = tm->ts->tile[t].s;
			k++;
			
			y_start -= y_step;

		}

		origin_y++;
	}

	return k;
}


int renderTilemapRecalc(TILE_CACHE *cache, TILESHEET *ts, int x, int y, int w, int h, int map_w, int map_h, unsigned int *tilemap, int inv_div, unsigned int mask) {
	float x_start, y_start;
	float *x_adv_buf, *y_adv_buf;
	int i, j, k, t, x_cur, y_cur;

	x_adv_buf = malloc(sizeof(float) * (w + 1));
	y_adv_buf = malloc(sizeof(float) * (h + 1));
	x_start = -1.0f;
	y_start = 1.0f;

	for (i = 0; i < (w + 1); i++)
		x_adv_buf[i] = x_start + ts->sw * (float) i;
	for (i = 0; i < (h + 1); i++)
		y_adv_buf[i] = y_start - ts->sh * (float) i;
	
	for (i = k = 0, x_cur = x; i < w; i++, x_cur++) {
		if (x_cur < 0 || x_cur >= map_w)
			continue;
		for (j = 0, y_cur = y; j < h; j++, y_cur++) {
			if (y_cur < 0 || y_cur > map_h)
				continue;
			t = tilemap[x_cur + y_cur * map_w] & mask;
			if (t >= ts->tiles)
				continue;

			if (inv_div != 0)
				if (t % inv_div == 0)
					continue;
			/* ugh.. */
			cache[k].vertex[0].coord.x = x_adv_buf[i];
			cache[k].vertex[0].coord.y = y_adv_buf[j];
			cache[k].vertex[1].coord.x = x_adv_buf[i+1];
			cache[k].vertex[1].coord.y = y_adv_buf[j];
			cache[k].vertex[2].coord.x = x_adv_buf[i+1];
			cache[k].vertex[2].coord.y = y_adv_buf[j+1];
			cache[k].vertex[3].coord.x = x_adv_buf[i+1];
			cache[k].vertex[3].coord.y = y_adv_buf[j+1];
			cache[k].vertex[4].coord.x = x_adv_buf[i];
			cache[k].vertex[4].coord.y = y_adv_buf[j+1];
			cache[k].vertex[5].coord.x = x_adv_buf[i];
			cache[k].vertex[5].coord.y = y_adv_buf[j];
			cache[k].vertex[0].tex.u = ts->tile[t].r;
			cache[k].vertex[0].tex.v = ts->tile[t].s;
			cache[k].vertex[1].tex.u = ts->tile[t].u;
			cache[k].vertex[1].tex.v = ts->tile[t].s;
			cache[k].vertex[2].tex.u = ts->tile[t].u;
			cache[k].vertex[2].tex.v = ts->tile[t].v;
			cache[k].vertex[3].tex.u = ts->tile[t].u;
			cache[k].vertex[3].tex.v = ts->tile[t].v;
			cache[k].vertex[4].tex.u = ts->tile[t].r;
			cache[k].vertex[4].tex.v = ts->tile[t].v;
			cache[k].vertex[5].tex.u = ts->tile[t].r;
			cache[k].vertex[5].tex.v = ts->tile[t].s;
			k++;
		}
	}

	free(x_adv_buf);
	free(y_adv_buf);
	
	return k;
}


void renderTilemapCameraMove(RENDER_TILEMAP *tm, int cam_x, int cam_y) {
	int x, y, w, h, map_w, map_h;
	
	tm->cam_x = tm->ts->swgran * (cam_x % tm->ts->wsq) * -1;
	tm->cam_y = tm->ts->shgran * (cam_y % tm->ts->hsq);

	x = floorf((float) cam_x / tm->ts->wsq);
	y = floorf((float) cam_y / tm->ts->hsq);
	
	if (x == tm->cam_xi && y == tm->cam_yi)
		return;

	w = tm->w;
	h = tm->h;
	map_w = tm->map_w;
	map_h = tm->map_h;

	tm->cache_used = renderTilemapRecalc(tm->cache, tm->ts, x, y, w, h, map_w, map_h, tm->map, tm->inv_div, tm->mask);

	tm->cam_xi = x;
	tm->cam_yi = y;

	return;
}


void *renderTilemapCreateISO(unsigned int w, unsigned int h, unsigned int *map, int camera_x, int camera_y, unsigned int inv_div, TILESHEET *ts, unsigned int mask, int r_h) {
	RENDER_TILEMAP *tm;
	
	if ((tm = malloc(sizeof(RENDER_TILEMAP))) == NULL) {
		return NULL;
	}

	tm->map_w = w;
	tm->map_h = h;
	tm->cam_xi = ~0;
	tm->cam_yi = ~0;
	tm->map = map;
	tm->inv_div = inv_div;
	tm->ts = ts;
	tm->w = d->video.w / ts->wsq + 2;
	tm->h = d->video.h / r_h * 2 + 2;
	tm->mask = mask;
	tm->isometric = 1;
	tm->r_h = r_h;

	tm->cache = malloc(sizeof(TILE_CACHE) * tm->w * tm->h);
	renderTilemapCameraMove(tm, camera_x, camera_y);

	return tm;
}


void *renderTilemapCreate(unsigned int w, unsigned int h, unsigned int *map, int camera_x, int camera_y, unsigned int inv_div, TILESHEET *ts, unsigned int mask) {
	RENDER_TILEMAP *tm;
	
	if ((tm = malloc(sizeof(RENDER_TILEMAP))) == NULL) {
		return NULL;
	}

	tm->map_w = w;
	tm->map_h = h;
	tm->cam_xi = ~0;
	tm->cam_yi = ~0;
	tm->map = map;
	tm->inv_div = inv_div;
	tm->ts = ts;
	tm->w = d->video.w / ts->wsq + 2;
	tm->h = d->video.h / ts->hsq + 2;
	tm->mask = mask;
	tm->isometric = 0;

	tm->cache = malloc(sizeof(TILE_CACHE) * tm->w * tm->h);
	renderTilemapCameraMove(tm, camera_x, camera_y);

	return tm;
}


void renderTilemapRender(RENDER_TILEMAP *tm) {
	glLoadIdentity();
	glTranslatef(tm->cam_x, tm->cam_y, 0);
	renderCache(tm->cache, tm->ts, tm->cache_used);
	glLoadIdentity();
	glTranslatef(d->video.swgran * d->video.offset_x, d->video.shgran * d->video.offset_y, 0);

	return;
}


void renderTilemapFree(RENDER_TILEMAP *tm) {
	free(tm->cache);
	free(tm);

	return;
}




void renderTilemapForceRecalc(RENDER_TILEMAP *tm) {
	if (tm == NULL) return;

	tm->cache_used = renderTilemapRecalc(tm->cache, tm->ts, tm->cam_xi, tm->cam_yi, tm->w, tm->h, tm->map_w, tm->map_h, tm->map, tm->inv_div, tm->mask);

	return;
}


// DARNIT_HEADLESS
#endif
