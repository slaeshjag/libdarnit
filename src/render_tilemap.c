#include "darner.h"



void renderTilemapRender(RENDER_TILEMAP *tm) {
	renderCache(tm->cache, tm->ts, tm->tilew * tm->tileh);

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
//	else if (tm->map[index] % tm->dns_div)
//		clear = 1;
	
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


void renderTilemapCameraMove(RENDER_TILEMAP *tm, int cam_x, int cam_y) {
	int i, j, tx, ty, bx, by;

	tx = cam_x / tm->ts->wsq;
	ty = cam_y / tm->ts->hsq;
	bx = tx + tm->tilew - 1;
	by = ty + tm->tileh - 1;

	fprintf(stderr, "%i,,;,,%i\n", tx, ty);

	for (i = 0; i < tm->tilew - 1; i++)
		tm->needbuf_x[i] = tx + i;
	for (i = 0; i < tm->tileh - 1; i++)
		tm->needbuf_y[i] = ty + i;

	for (i = 0; i < tm->tilew; i++) {
		if (tm->col[i] < tx || tm->col[i] > bx) {	// Don't need it, free it!
			if (tm->col[i] == -1)			// Oh, never mind
				continue;
			fprintf(stderr, "Freeing %i\n", i);
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

	fprintf(stderr, "Spares: %i, %i\n", tm->col_spares, tm->row_spares);


	// Now, fill in the missing rows/cols //
	for (i = 0; i < tm->tilew - 1; i++)
		if (tm->needbuf_x[i] != -1) {
			fprintf(stderr, "Eek, bad time ahead! (X), %i, %i, %i\n", tm->needbuf_x[i], i, tm->col_spare[tm->col_spares]);
			tm->col_spares--;
			tm->col[tm->col_spare[tm->col_spares]] = tm->needbuf_x[i];
			for (j = 0; j < tm->tileh; j++)
				renderTilemapRecalculateTile(tm, tm->needbuf_x[i], ty+j, tm->col_spare[tm->col_spares], j);
		}
	
	for (i = 0; i < tm->tileh - 1; i++)
		if (tm->needbuf_y[i] != -1) {
			fprintf(stderr, "Eek, bad time ahead! (Y)\n");
			tm->row_spares--;
			tm->row[tm->row_spare[tm->row_spares]] = tm->needbuf_y[i];
			for (j = 0; j < tm->tilew; j++)
				renderTilemapRecalculateTile(tm, tx+j, tm->needbuf_y[i], j, tm->row_spare[tm->row_spares]);
		}

	

	return;
}

/*
void renderTilemapCameraMove(RENDER_TILEMAP *tm, int cam_x, int cam_y) {
	int i, j, dx, dy, nx, ny, tx, ty, dir, ti, tc;

	tx = cam_x / tm->ts->wsq;
	ty = cam_y / tm->ts->hsq;
	nx = tm->cam_x / tm->ts->wsq;
	ny = tm->cam_y / tm->ts->hsq;
	dx = tx - nx;
	dy = ty - ny;
	if (dx < 0) dx *= -1;
	if (dy < 0) dy *= -1;

	if (dx > tm->tilew - 4 || dy > tm->tileh - 4) {
		renderTilemapRecalculate(tm);
		return;
	}

//	 Horisontal recalculation 
	if (dx > 0) {
		if (tm->strip_x == 0) fprintf(stderr, "H: ");
		dir = (nx > tx) ? -1 : 1;
		tc = (dir == 1) ? nx + tm->tilew: nx-1;
		if (dir == 1) { 
			nx++; 
			if (tm->lastdir_x < 1) 
				tm->strip_x++; 
			if (tm->strip_x >= tm->tilew) 
				tm->strip_x = 0; 
		}
		else { if (tm->lastdir_x > -1) tm->strip_x--; if (tm->strip_x < 0) tm->strip_x = tm->tilew - 1; }
		for (i = 0; i != dx; i++, nx += dir) {
			ti = tm->strip_y + 1;
			for (j = 0; j < tm->tileh-1; j++, ti++) {
				if (ti >= tm->tileh)
					ti = 0;
				fprintf(stderr, "H: %i %i %i %i\n", tc, ny+j, tm->strip_x, ti);
		//		renderTilemapRecalculateTile(tm, tc, ny+j, tm->strip_x, ti);
			}
			tm->strip_x += dir;
			if (tm->strip_x >= tm->tilew)
				tm->strip_x = 0;
			else if (tm->strip_x < 0)
				tm->strip_x = tm->tilew - 1;
		}
		tm->lastdir_x = dir;
	}

	tm->cam_x = cam_x;
	
//	 Vertical recalculation 
	if (dy > 0) {
		if (tm->strip_y == 0) fprintf(stderr, "\nV: ");
		dir = (ny > ty) ? -1 : 1;
		tc = (dir == 1) ? ny + tm->tileh : ny - 1;
		if (dir == 1) { 
			ny++; 
			if (tm->lastdir_y < 1) 
				tm->strip_y++; 
			if (tm->strip_y >= tm->tileh) 
				tm->strip_y = 0; 
		}
		else { if (tm->lastdir_y > -1) tm->strip_y--; if (tm->strip_y < 0) tm->strip_y = tm->tileh - 1; }
		for (i = 0; i != dy; i++, ny += dir) {
			ti = tm->strip_x + 1;
			for (j = 0; j < tm->tilew-1; j++, ti++) {
				if (ti >= tm->tilew)
					ti = 0;
				fprintf(stderr, "V: %i %i %i %i;; %i\n", nx+j, tc, ti, tm->strip_y, tm->tilew);
				renderTilemapRecalculateTile(tm, nx+j, tc, ti, tm->strip_y);
			}
			tm->strip_y += dir;
			if (tm->strip_y >= tm->tileh)
				tm->strip_y = 0;
			else if (tm->strip_y < 0)
				tm->strip_y = tm->tileh - 1;
		}
		tm->lastdir_y = dir;
	}

	tm->cam_y = cam_y;

	return;
}
*/


void *renderTilemapCreate(void *handle, unsigned int w, unsigned int h, unsigned int *map, int camera_x, int camera_y, unsigned int invisibility_divider, TILESHEET *ts) {
	DARNER *m = handle;
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

	renderTilemapCameraMove(tm, camera_x, camera_y);

	return tm;
}
