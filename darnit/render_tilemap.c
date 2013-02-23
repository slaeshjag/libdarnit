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

	isometricFromScreen(x, y, origin, 0, tm->ts->wsq, tm->r_h, r_x, r_y);

	return;
}


void renderTilemapToISOCoordinates(RENDER_TILEMAP *tm, int x, int y, int *r_x, int *r_y) {
	int origin = tm->ts->wsq * tm->w / 2;

	isometricToScreen(x, y, origin, 0, tm->ts->wsq, tm->r_h, r_x, r_y);

	return;
}


int renderTilemapRecalcISO(RENDER_TILEMAP *tm) {
	float x_start, y_start, x_pos, y_pos;
	float x_step, y_step;
	int i, j, k, t, x_cur, y_cur, x_iter, y_iter, o_x, o_y, x_last, y_last;


	x_step = d->video.swgran * tm->ts->wsq / 2;
	y_step = d->video.shgran * tm->r_h / 2;
	x_iter = d->video.w / tm->ts->wsq * 2;
	y_iter = (d->video.h / tm->r_h) + 2;
	renderTilemapISOCoordinates(tm, tm->cam_xp + d->video.w, tm->cam_yp, &o_x, &o_y);
	o_x++;
	renderTilemapToISOCoordinates(tm, o_x, o_y, &x_last, &y_last);
	x_start = d->video.swgran * (x_last - tm->cam_xp) - 1.0f - x_step;
	y_start = 1.0f - d->video.shgran * (y_last - tm->cam_yp);
	for (i = k = 0; i < y_iter; i++, y_start -= y_step * 2) {
		x_pos = x_start;
		y_pos = y_start;
		x_cur = o_x + i;
		y_cur = o_y + i;


		if (y_cur >= tm->map_w)
			break;
		if (y_cur < 0)
			continue;
		
		for (j = 0; j < x_iter; j++, x_cur--, x_pos -= x_step, y_pos += y_step) {
			if (x_cur >= tm->map_w)
				continue;
			if (x_cur < 0)
				break;

			t = tm->map[x_cur + y_cur * tm->map_w] & tm->mask;
			if (t >= tm->ts->tiles)
				continue;

			if (tm->inv_div != 0)
				if (t % tm->inv_div == 0)
					continue;

			/* Rurgh... */
			RENDER_TILEMAP_FILL(k, x_pos, y_pos, x_step * 2, tm->ts->tile[t].h_p, t);
			//fprintf(stderr, "K: %i, T: %i - (%i,%i)\n", k, t, x_cur, y_cur);
			//fprintf(stderr, "Coordinates: %f,%f   %f,%f\n", x_pos, y_pos, x_pos + x_step, y_pos - tm->ts->tile[t].h_p);
			//fprintf(stderr, "Tex. coords: %f,%f   %f,%f\n", tm->ts->tile[t].r, tm->ts->tile[t].s, tm->ts->tile[t].u, tm->ts->tile[t].v);
			k++;

		}

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
			if (y_cur < 0 || y_cur >= map_h)
				continue;
			t = tilemap[x_cur + y_cur * map_w] & mask;
			if (t >= ts->tiles)
				continue;

			if (inv_div != 0)
				if (t % inv_div == 0)
					continue;
			/* ugh.. */
			if (k == 0)
				fprintf(stderr, "Starting rendering at %i, %i (%i, %i)\n", x_cur, y_cur, i, j);
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
	tm->cam_y = tm->ts->shgran * (cam_y % tm->r_h);

	x = floorf((float) cam_x / tm->ts->wsq);
	y = floorf((float) cam_y / tm->r_h);
	
	if (x == tm->cam_xi && y == tm->cam_yi)
		return;

	w = tm->w;
	h = tm->h;
	map_w = tm->map_w;
	map_h = tm->map_h;
	tm->cam_xp = cam_x;
	tm->cam_yp = cam_y;


	if (tm->isometric)
		tm->cache_used = renderTilemapRecalcISO(tm);
	else
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
	tm->cam_xp = 0;
	tm->cam_yp = 0;

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
	tm->r_h = ts->hsq;

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

	if (tm->isometric)
		tm->cache_used = renderTilemapRecalcISO(tm);
	else
		tm->cache_used = renderTilemapRecalc(tm->cache, tm->ts, tm->cam_xi, tm->cam_yi, tm->w, tm->h, tm->map_w, tm->map_h, tm->map, tm->inv_div, tm->mask);

	return;
}


// DARNIT_HEADLESS
#endif
