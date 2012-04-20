#include "darnit.h"



void renderTilemapRender(void *handle, RENDER_TILEMAP *tm) {
	DARNIT *m = handle;
	glLoadIdentity();
	glTranslatef(tm->cam_x, tm->cam_y, 0);
	renderCache(tm->cache, tm->ts, tm->tilew * tm->tileh);
	glLoadIdentity();
	glTranslatef(m->video.offset_x, m->video.offset_y, 0);

	return;
}


void renderTilemapRecalculateTile(RENDER_TILEMAP *tm, int x, int y, int view_x, int view_y) {
	int clear = 0;
	int index, ci, i;
	float *ptr;

	index = y * tm->tm_w + x;
	ci = tm->tilew*view_y + view_x;

	if (x >= tm->tm_w || x < 0)
		clear = 1;
	else if (y >= tm->tm_h || y < 0)
		clear = 1;
	else if (!tm->map[index] % tm->dns_div)
		clear = 1;
	
	if (!clear) {
		renderCalcTileCache(&tm->cache[ci], tm->ts, tm->map[index]);
		renderCalcTilePosCache(&tm->cache[ci], tm->ts, x*tm->ts->wsq, y*tm->ts->hsq);
		return;
	}
	
	ptr = (void *) &tm->cache[ci];
	for (i = 0; i < 24; i++)
		ptr[i] = 0.0f;
	
	return;
}


void renderTilemapTileSet(RENDER_TILEMAP *tm, int x, int y, int tile) {
	int i, j;

	if (x >= tm->tm_w || x < 0 || y >= tm->tm_h || y < 0)
		return;

	tm->map[tm->tm_w * y + x] = tile;

	for (i = 0; i < tm->tilew; i++)
		if (tm->col[i] == x)
			for (j = 0; j < tm->tileh; j++)
				if (tm->row[j] == y) {
					renderTilemapRecalculateTile(tm, x, y, i, j);
					return;
				}
	return;
}


void renderTilemapCameraMove(RENDER_TILEMAP *tm, int cam_x, int cam_y) {
	int i, j, k, tx, ty, bx, by, index;

	tx = cam_x / tm->ts->wsq;
	ty = cam_y / tm->ts->hsq;
	bx = tx + tm->tilew - 1;
	by = ty + tm->tileh - 1;

	tm->cam_x = tm->ts->swgran * cam_x * -1;
	tm->cam_y = tm->ts->shgran * cam_y;

	for (i = 0; i < tm->tilew - 1; i++)
		tm->needbuf_x[i] = tx + i;
	for (i = 0; i < tm->tileh - 1; i++)
		tm->needbuf_y[i] = ty + i;

	for (i = 0; i < tm->tilew; i++) {
		if (tm->col[i] < tx || tm->col[i] > bx) {	// Don't need it, free it!
			if (tm->col[i] == -1)			// Oh, never mind
				continue;
			tm->col_spare[tm->col_spares] = i;
			tm->col_spares++;
			tm->col[i] = -1;
		} else {
			for (j = 0; j < tm->tilew; j++) {
				if (tm->needbuf_x[j] == tm->col[i]) {
					tm->needbuf_x[j] = -1;
					break;
				}
			}
		}
	}
			
	
	for (i = 0; i < tm->tileh; i++) {
		if (tm->row[i] < ty || tm->row[i] > by) {
			if (tm->row[i] == -1)
				continue;
			tm->row_spare[tm->row_spares] = i;
			tm->row_spares++;
			tm->row[i] = -1;
		} else {
			for (j = 0; j < tm->tileh; j++)
				if (tm->needbuf_y[j] == tm->row[i]) {
					tm->needbuf_y[j] = -1;
					break;
				}
		}
	}

	// Now, fill in the missing rows/cols //
	for (i = 0; i < tm->tilew - 1; i++)
		if (tm->needbuf_x[i] != -1) {
			tm->col_spares--;
			tm->col[tm->col_spare[tm->col_spares]] = tm->needbuf_x[i];
			index = 0;
			for (j = 0; j < tm->tileh-1; j++) {
				for (k = 0; k < tm->tileh; k++)
					if (tm->row[k] == ty+j) {
						index = k;
						break;
					}
				if (k == tm->tileh)
					continue;
				renderTilemapRecalculateTile(tm, tm->needbuf_x[i], ty+j, tm->col_spare[tm->col_spares], index);
			}
		}
	
	for (i = 0; i < tm->tileh - 1; i++)
		if (tm->needbuf_y[i] != -1) {
			tm->row_spares--;
			tm->row[tm->row_spare[tm->row_spares]] = tm->needbuf_y[i];
			for (j = 0; j < tm->tilew-1; j++) {
				for (k = 0; k < tm->tilew; k++)
					if (tm->col[k] == tx+j) {
						index = k;
						break;
					}
				if (k == tm->tilew)
					continue;
				renderTilemapRecalculateTile(tm, tx+j, tm->needbuf_y[i], index, tm->row_spare[tm->row_spares]);
			}
		}

	

	return;
}



void *renderTilemapCreate(void *handle, unsigned int w, unsigned int h, unsigned int *map, int camera_x, int camera_y, unsigned int invisibility_divider, TILESHEET *ts) {
	DARNIT *m = handle;
	RENDER_TILEMAP *tm;
	int i;

	if ((tm = malloc(sizeof(RENDER_TILEMAP))) == NULL) {
		fprintf(stderr, "libDarner: renderTilemapCreate(): Unable to malloc(%i)\n", (int) sizeof(RENDER_TILEMAP));
		return NULL;
	}

	tm->tilew = m->video.w / ts->wsq;
	tm->tilew += (m->video.w % ts->wsq) ? 3 : 2;	/* We need it to be 1 tile bigger than the amount of on-screen tiles */
	tm->tileh = m->video.h / ts->hsq;
	tm->tileh += (m->video.h % ts->hsq) ? 3 : 2; 	/* -||- */
	tm->row = malloc(sizeof(int) * tm->tileh);
	tm->col = malloc(sizeof(int) * tm->tilew);
	tm->row_spare = malloc(tm->tileh * sizeof(int));
	tm->col_spare = malloc(tm->tilew * sizeof(int));
	tm->needbuf_x = malloc(tm->tilew * sizeof(int));
	tm->needbuf_y = malloc(tm->tileh * sizeof(int));

	tm->tm_w = w;
	tm->tm_h = h;

	tm->ts = ts;
	tm->dns_div = invisibility_divider;
	tm->map = map;
	

	for (i = 0; i < tm->tilew; i++) {
		tm->col[i] = -1;
		tm->col_spare[i] = i;
	}
	for (i = 0; i < tm->tileh; i++) {
		tm->row[i] = -1;
		tm->row_spare[i] = i;
	}
	
	tm->row_spares = tm->tileh;
	tm->col_spares = tm->tilew;

	if ((tm->cache = malloc(sizeof(TILE_CACHE) * (tm->tilew * tm->tileh))) == NULL) {
		free(tm);
		fprintf(stderr, "libDarner: renderTilemapCreate(): Unable to malloc(%i)\n", (int) sizeof(TILE_CACHE)*tm->tilew*tm->tileh);
		return NULL;
	}

	for (i = 0; i < tm->tilew * tm->tileh; i++)
		renderTilemapRecalculateTile(tm, -1, -1, -1, -1);
	renderTilemapCameraMove(tm, tm->tm_w, tm->tm_h);			// This "fixes" a weird rendering bug
	renderTilemapCameraMove(tm, 0, 0);

	return tm;
}


void renderTilemapFree(RENDER_TILEMAP *tm) {
	free(tm->cache);
	free(tm->row);
	free(tm->col);
	free(tm->row_spare);
	free(tm->col_spare);
	free(tm->needbuf_x);
	free(tm->needbuf_y);
	free(tm);

	return;
}
