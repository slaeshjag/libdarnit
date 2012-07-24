#include "darnit.h"

void renderTilemapCalcPosMap(TILE_CACHE *cache, TILESHEET *ts, int x, int y, int w, int h) {
	float x_start, y_start;
	float *x_adv_buf, *y_adv_buf;
	int i, j, k;
	

	/* Because of the load/store delay, interleaving makes more efficient use of the cycles */
	x_start = x;
	y_start = y;
	x_adv_buf = malloc(sizeof(float) * (w + 1));
	x_start *= ts->sw;
	y_start *= ts->sh;
	y_adv_buf = malloc(sizeof(float) * (h + 1));
	x_start -= 1.0f;
	y_start = 1.0f - y_start;


	for (i = 0; i < (w + 1); i++) {
		x_adv_buf[i] = x_start + ts->sw*(float)i;
	}
	for (i = 0; i < (h + 1); i++) {
		y_adv_buf[i] = y_start - ts->sh*(float)i;
	}
	
	for (i = k = 0; i < w; i++) {
		for (j = 0; j < h; j++, k++) {
			cache[k].x = x_adv_buf[i];
			cache[k].y = y_adv_buf[j];
			cache[k].x2 = x_adv_buf[i+1];
			cache[k].y2 = y_adv_buf[j];
			cache[k].x3 = x_adv_buf[i+1];
			cache[k].y3 = y_adv_buf[j+1];
			cache[k].x4 = x_adv_buf[i+1];
			cache[k].y4 = y_adv_buf[j+1];
			cache[k].x5 = x_adv_buf[i];
			cache[k].y5 = y_adv_buf[j+1];
			cache[k].x6 = x_adv_buf[i];
			cache[k].y6 = y_adv_buf[j];
		}
	}

	free(x_adv_buf);
	free(y_adv_buf);

	return;
}


void renderTilemapCalcMap(TILE_CACHE *cache, TILESHEET *ts, int x, int y, int w, int h, int map_w, int map_h, unsigned int *tilemap, int inv_div, unsigned int mask) {
	int i, j, k, l;
	int x_cur, y_cur, blank;
	int t;
	float *nullbuf;

	x_cur = x;
	for (i = k = 0; i < w; i++, x_cur++) {
		y_cur = y;
		if (x_cur < 0 || x_cur >= map_w)
			blank = 1;
		else
			blank = 0;
		for (j = 0; j < h; j++, k++, y_cur++) {
			if (y_cur >= 0 && y_cur < map_h && blank == 0)
				t = tilemap[y_cur*map_w + x_cur] & mask;
			else
				t = -1;
			if (t > -1 && (t % inv_div)) {
				cache[k].u = ts->tile[t].r;
				cache[k].v = ts->tile[t].s;
				cache[k].u2 = ts->tile[t].u;
				cache[k].v2 = ts->tile[t].s;
				cache[k].u3 = ts->tile[t].u;
				cache[k].v3 = ts->tile[t].v;
				cache[k].u4 = ts->tile[t].u;
				cache[k].v4 = ts->tile[t].v;
				cache[k].u5 = ts->tile[t].r;
				cache[k].v5 = ts->tile[t].v;
				cache[k].u6 = ts->tile[t].r;
				cache[k].v6 = ts->tile[t].s;
			} else {
				nullbuf = (float *) &cache[k];
				for (l = 0; l < 24; l++)
					nullbuf[l] = -1.0f;
			}
		}
	}

	return;
}



void renderTilemapCameraMove(RENDER_TILEMAP *tm, int cam_x, int cam_y) {
	int x, y, w, h, map_w, map_h;

	tm->cam_x = tm->ts->swgran * cam_x * -1;
	tm->cam_y = tm->ts->shgran * cam_y;

	x = cam_x / tm->ts->wsq;
	y = cam_y / tm->ts->hsq;
	
	if (x == tm->cam_xi && y == tm->cam_yi)
		return;

	w = tm->w;
	h = tm->h;
	map_w = tm->map_w;
	map_h = tm->map_h;

	renderTilemapCalcPosMap(tm->cache, tm->ts, x, y, w, h);
	renderTilemapCalcMap(tm->cache, tm->ts, x, y, w, h, map_w, map_h, tm->map, tm->inv_div, tm->mask);

	tm->cam_xi = x;
	tm->cam_yi = y;

	return;
}


void *renderTilemapCreate(void *handle, unsigned int w, unsigned int h, unsigned int *map, int camera_x, int camera_y, unsigned int inv_div, TILESHEET *ts, unsigned int mask) {
	DARNIT *m = handle;
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
	tm->w = m->video.w / ts->wsq + 2;
	tm->h = m->video.h / ts->hsq + 2;
	tm->mask = mask;

	tm->cache = malloc(sizeof(TILE_CACHE) * tm->w * tm->h);
	renderTilemapCameraMove(tm, camera_x, camera_y);

	return tm;
}


void renderTilemapRender(void *handle, RENDER_TILEMAP *tm) {
	DARNIT *m = handle;
	glLoadIdentity();
	glTranslatef(tm->cam_x, tm->cam_y, 0);
	renderCache(tm->cache, tm->ts, tm->w * tm->h);
	glLoadIdentity();
	glTranslatef(m->video.swgran * m->video.offset_x, m->video.shgran * m->video.offset_y, 0);

	return;
}


void renderTilemapFree(RENDER_TILEMAP *tm) {
	free(tm->cache);
	free(tm);

	return;
}


void renderTilemapTileSet(RENDER_TILEMAP *tm, int x, int y, int tile) {
	int ci;

	if (x >= tm->map_w || x < 0 || y >= tm->map_h || y < 0)
		return;
	
	tm->map[tm->map_w * y + x] = tile;

	x -= tm->cam_xi;
	y -= tm->cam_yi;

	if (x < 0 || x >= tm->w || y < 0 || y >= tm->h)
		return;
	
	ci = x * tm->h + y;	/* For some reason, I made the cache indexing in cols instead of rows */

	renderCalcTileCache(&tm->cache[ci], tm->ts, tm->map[tm->map_w * y + x] & tm->mask);
	renderCalcTilePosCache(&tm->cache[ci], tm->ts, x * tm->ts->wsq, y * tm->ts->hsq);

	return;
}


void renderTilemapForceRecalc(RENDER_TILEMAP *tm) {
	if (tm == NULL) return;
	
	renderTilemapCalcPosMap(tm->cache, tm->ts, tm->cam_xi, tm->cam_yi, tm->w, tm->h);
	renderTilemapCalcMap(tm->cache, tm->ts, tm->cam_xi, tm->cam_yi, tm->w, tm->h, tm->map_w, tm->map_h, tm->map, tm->inv_div, tm->mask);

	return;
}
