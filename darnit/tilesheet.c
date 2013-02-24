/*
Copyright (c) 2011-2013 Steven Arnow
'tilesheet.c' - This file is part of libdarnit

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


int renderAddTSRef(const char *fname, TILESHEET *ts) {
	void *tmp;
	int i, len, fast_cmp;

	if (fname == NULL) return -1;
	len = strlen(fname);

	for (i = fast_cmp = 0; i < len; i++)
		fast_cmp += fname[i];
	
	for (i = 0; i < d->tsr.cnt; i++)
		if (d->tsr.tsr[i].fast_cmp == 0)
			break;
	
	if (i == d->tsr.cnt) {
		d->tsr.cnt++;
		if ((tmp = realloc(d->tsr.tsr, sizeof(TILESHEET_REF) * d->tsr.cnt)) == NULL) {
			d->tsr.cnt--;
			return -1;
		}
		d->tsr.tsr = tmp;
	}

	if ((d->tsr.tsr[i].fname = malloc(strlen(fname)+1)) == NULL) {
		d->tsr.tsr[i].fast_cmp = 0;
		return -1;
	}

	strcpy(d->tsr.tsr[i].fname, fname);
	d->tsr.tsr[i].fast_cmp = fast_cmp;
	d->tsr.tsr[i].ref = ts;

	return i;
}


void renderDelTSRef(int ref) {
	if (ref < 0 || ref >= d->tsr.cnt)
		return;
	
	d->tsr.tsr[ref].fast_cmp = 0;
	free(d->tsr.tsr[ref].fname);
	d->tsr.tsr[ref].fname = NULL;

	return;
}


TILESHEET *renderGetTilesheetFromRef(const char *fname) {
	int i, len, fast_cmp;

	if (fname == NULL) return NULL;
	len = strlen(fname);

	for (i = fast_cmp = 0; i < len; i++)
		fast_cmp += fname[i];
	
	for (i = 0; i < d->tsr.cnt; i++) {
		if (d->tsr.tsr[i].fast_cmp == fast_cmp)
			if (strcmp(d->tsr.tsr[i].fname, fname) == 0) {
				d->tsr.tsr[i].ref->ref_count++;
				return d->tsr.tsr[i].ref;
			}
	}

	return NULL;
}


void renderTilesheetAnimateAll() {
	int i;

	for (i = 0; i < d->tsr.cnt; i++) {
		if (!d->tsr.tsr[i].fast_cmp)
			continue;
		if (d->tsr.tsr[i].ref->animation.tiles > 0)
			renderTilesheetAnimate(d->tsr.tsr[i].ref);
	}

	return;
}


void renderTilesheetAnimate(TILESHEET *ts) {
	int i, j, change;
	TS_FRAME *frame;

	if (ts == NULL)
		return;


	for (i = 0; i < ts->animation.tiles; i++) {
		change = 0;
		ts->animation.tile[i].time_rest += darnitTimeLastFrameTook();
		j = ts->animation.tile[i].frame[ts->animation.tile[i].frame_at].time;
		while (ts->animation.tile[i].time_rest >= j) {
			ts->animation.tile[i].time_rest -= j;
			ts->animation.tile[i].frame_at++;
			if (ts->animation.tile[i].frame_at >= ts->animation.tile[i].frames)
				ts->animation.tile[i].frame_at = 0;
			j = ts->animation.tile[i].frame[ts->animation.tile[i].frame_at].time;
			frame = &ts->animation.tile[i].frame[ts->animation.tile[i].frame_at];
			change = 1;
		}

		if (change) {
			i = frame->tile_dst % (ts->w / ts->wsq);
			j = frame->tile_dst / (ts->w / ts->wsq);
			renderUpdateTilesheet(ts, i * ts->wsq, j * ts->hsq, &ts->animation.data[ts->animation.tile_skip * frame->tile_src], ts->wsq, ts->hsq);
		}
	}

	return;
}


int renderTilesheetAnimationApply(TILESHEET *ts, const char *fname) {
	unsigned int i, j, k, tiles, frames, pos, a, b, cc, d, e;
	unsigned int *newdata;
	char c, buf[512];
	FILESYSTEM_FILE *fp;
	IMGLOAD_DATA img;
	TS_FRAME *frame;
	
	if (ts == NULL)
		return -1;
	
	if (ts->animation.tiles > 0)
		return -1;

	if ((fp = fsFileOpen(fname, "rb")) == NULL)
		return -1;

	fsFileGetLine(buf, 512, fp);
	pos = fsFileTell(fp);
	img = imgloadLoad(buf);

	if (img.img_data == NULL) {
		fsFileClose(fp);
		return -1;
	}

	if (img.w != ts->wsq) {
		if (!(newdata = malloc(sizeof(unsigned int) * ts->wsq * ts->hsq * (img.w / ts->wsq * img.h / ts->hsq)))) {
			fsFileClose(fp);
			free(img.img_data);
			return -1;
		}

		a = img.w / ts->wsq;
		b = img.h / ts->hsq;
		cc = ts->wsq * ts->hsq;
		for (i = 0; i < a; i++) {
			for (j = 0; j < b; j++)
				for (k = 0; k < cc; k++) {
					/* Skip all the lines before us */
					d = (a * j * cc) + (k / ts->wsq * img.w);
					/* Skip all the cols before us */
					e = i * ts->wsq + k % ts->wsq;
					newdata[(j*a + i) * cc + k] = img.img_data[d + e];
				}
		}
		free(img.img_data);
		img.img_data = newdata;
		img.w = ts->wsq;
		img.h = ts->hsq * a * b;

	}

	ts->animation.animation_tiles = img.h / ts->hsq;
	tiles = frames = 0;
	
	while (!fsFileEOF(fp)) {
		fsFileRead(&c, 1, fp);
		switch (c) {
			case 'T':
				fsFileGets(buf, 512, fp);
				break;
			case 'F':
				fsFileGets(buf, 512, fp);
				frames++;
				break;
			case 'E':
				fsFileGets(buf, 512, fp);
				tiles++;
				break;
			case '\n':
				break;
			default:
				fsFileGets(buf, 512, fp);
				break;
		}
	}

	i = j = 0;
	fsFileSeek(fp, pos, SEEK_SET);

	
	frame = malloc(sizeof(TS_FRAME) * frames);
	ts->animation.tile = malloc(sizeof(TS_TILE) * tiles);

	if (frame == NULL || ts->animation.tile == NULL) {
		fsFileClose(fp);
		free(img.img_data);
		free(frame);
		free(ts->animation.tile);
		ts->animation.tile = NULL;
		return -1;
	}

	ts->animation.tiles = tiles;
	ts->animation.tile_skip = ts->wsq * ts->hsq;
	ts->animation.data = img.img_data;
	ts->animation.frame_data = frame;
	frames = 0;

	while (!fsFileEOF(fp)) {
		fsFileRead(&c, 1, fp);
		switch (c) {
			case 'T':
				fsFileGets(buf, 512, fp);
				ts->animation.tile[i].frame = &frame[frames];
				ts->animation.tile[i].frame_at = 0;
				ts->animation.tile[i].time_rest = 0;
				j = 0;
				break;
			case 'F':
				fsFileGets(buf, 512, fp);
				sscanf(buf, "%i %i %i\n", &ts->animation.tile[i].frame[j].tile_src, &ts->animation.tile[i].frame[j].tile_dst, &ts->animation.tile[i].frame[j].time);
				j++;
				break;
			case 'E':
				fsFileGets(buf, 512, fp);
				ts->animation.tile[i].frames = j;
				j = 0;
				i++;
				break;
			case '\n':
				break;
			default:
				fsFileGets(buf, 512, fp);
				break;
		}
	}

	ts->animation.tiles = i;

	fsFileClose(fp);

	return 0;
}


TILESHEET *renderTilesheetLoad(const char *fname, unsigned int wsq, unsigned int hsq, unsigned int convert_to) {
	TILESHEET *ts;
	IMGLOAD_DATA data;
	void *data_t;

	if ((ts = renderGetTilesheetFromRef(fname)) != NULL) {
		return ts;
	}

	data = imgloadLoad(fname);
	data_t = data.img_data;

	if (data.img_data == NULL) {
		return NULL;
	}

	ts = renderNewTilesheet(data.w / wsq, data.h / hsq, wsq, hsq, convert_to);

	renderUpdateTilesheet(ts, 0, 0, data_t, data.w, data.h);
	renderPopulateIsometricTilesheet(ts, data.w / wsq, data.h / hsq, data_t);
	
	free(data_t);
	ts->ref = renderAddTSRef(fname, ts);

	return ts;
}


TILESHEET *renderTilesheetLoadIsometric(const char *fname, unsigned int wsq, unsigned int hsq, unsigned int convert_to) {
	TILESHEET *ts;
	IMGLOAD_DATA data;
	void *data_t;

	if ((ts = renderGetTilesheetFromRef(fname)) != NULL) {
		return ts;
	}

	data = imgloadLoad(fname);
	data_t = data.img_data;

	if (data.img_data == NULL) {
		return NULL;
	}

	ts = renderNewTilesheet(data.w / wsq, data.h / hsq, wsq, hsq, convert_to);

	renderUpdateTilesheet(ts, 0, 0, data_t, data.w, data.h);
	renderPopulateIsometricTilesheet(ts, data.w / wsq, data.h / hsq, data_t);
	
	free(data_t);
	ts->ref = renderAddTSRef(fname, ts);

	return ts;
}

void renderPopulateTilesheet(TILESHEET *ts, int tiles_w, int tiles_h) {
	float twgran, thgran;
	int i, j, p;

	twgran = 1.0f / ts->w * ts->wsq;
	thgran = 1.0f / ts->h * ts->hsq;

	ts->sw = d->video.swgran * ts->wsq;
	ts->sh = d->video.shgran * ts->hsq;
	ts->swgran = d->video.swgran;
	ts->shgran = d->video.shgran;
	ts->tiles = tiles_w * tiles_h;
	ts->ref_count = 1;
	
	for (i = 0; i < tiles_h; i++)
		for (j = 0; j < tiles_w; j++) {
			p = i * tiles_w + j;
			ts->tile[p].r = twgran * j;
			ts->tile[p].s = thgran * i;
			ts->tile[p].u = ts->tile[p].r + twgran;
			ts->tile[p].v = ts->tile[p].s + thgran;
		}
	return;
}


int renderDetectTileHeight(int x, int y, int w, int max_h, int ts_w, unsigned int *data) {
	int i, j;

	for (i = 0; i < max_h; i++)
		for (j = 0; j < w; j++)
			if ((data[(j + x) + (i + y) * ts_w] & 0xFF000000))
				return max_h - i;
	return 0;
}


void renderPopulateIsometricTilesheet(TILESHEET *ts, int tiles_w, int tiles_h, unsigned int *data) {
	float twgran, phgran;
	int i, j, p, x, y, skip;

	twgran = 1.0f / ts->w * ts->wsq;
	phgran = 1.0f / ts->h;

	ts->sw = d->video.swgran * ts->wsq;
	ts->sh = d->video.shgran * ts->hsq;
	ts->swgran = d->video.swgran;
	ts->shgran = d->video.shgran;
	ts->ref_count = 1;
	
	for (i = 0; i < tiles_h; i++)
		for (j = 0; j < tiles_w; j++) {
			x = j * ts->wsq;
			y = i * ts->hsq;
			p = i * tiles_w + j;
			skip = renderDetectTileHeight(x, y, ts->wsq, ts->hsq, tiles_w * ts->wsq, data);
//			fprintf(stderr, "Tile height %i is %i\n", p, skip);
//			skip = ts->hsq;
			ts->tile[p].r = twgran * j;
			ts->tile[p].s = phgran * ((i + 1) * ts->hsq - skip);
			ts->tile[p].u = ts->tile[p].r + twgran;
			ts->tile[p].v = ts->tile[p].s + skip * phgran;
			ts->tile[p].h_p = skip * d->video.shgran;
		}
	return;
}


void *renderTilesheetFree(TILESHEET *ts) {
	if (ts == NULL) return NULL;

	ts->ref_count--;
	if (ts->ref_count > 0) return ts;
	videoRemoveTexture(ts->texhandle);
	renderDelTSRef(ts->ref);
	free(ts->animation.frame_data);
	free(ts->animation.tile);
	free(ts->animation.data);
	free(ts->tile);
	free(ts);

	return NULL;
}


TILESHEET *renderNewTilesheet(int tiles_w, int tiles_h, int tile_w, int tile_h, unsigned int format) {
	unsigned int tilesheet_w, tilesheet_h, texture;
	TILESHEET *ts;
	
	tilesheet_w = bitwiseRoundUpToPow2(tiles_w * tile_w);
	tilesheet_h = bitwiseRoundUpToPow2(tiles_h * tile_h);

	if ((ts = malloc(sizeof(TILESHEET))) == NULL)
		return NULL;

	ts->animation.tiles = 0;
	ts->animation.frame_data = NULL;
	ts->animation.tile = NULL;
	ts->animation.data = NULL;
	ts->w = tilesheet_w;
	ts->h = tilesheet_h;
	ts->format = format;

	#ifndef _WIN32
	if (format == PFORMAT_RGBA8)
		ts->texhandle = videoAddTexture(NULL, ts->w, ts->h);
	else if (format == PFORMAT_RGBA4)
		ts->texhandle = videoAddTextureRGBA4(NULL, ts->w, ts->h);
	else if (format == PFORMAT_RGB5A1)
		ts->texhandle = videoAddTextureRGB5A1(NULL, ts->w, ts->h);
	else if (format == PFORMAT_A8)
		ts->texhandle = videoAddTextureA8(NULL, ts->w, ts->h);
	#else
	if (format == PFORMAT_A8)
		ts->texhandle = videoAddTextureA8(NULL, ts->w, ts->h);
	else
		ts->texhandle = videoAddTexture(NULL, ts->w, ts->h);
	ts->format = (format == PFORMAT_A8) ? PFORMAT_A8 : PFORMAT_RGBA8;
	#endif
	
	if ((ts->tile = malloc(sizeof(TILESHEET_TILE) * tiles_w * tiles_h)) == NULL) {
		glDeleteTextures(1, &texture);
		free(ts);
		return NULL;
	}

	ts->tiles = tiles_w * tiles_h;
	ts->wsq = tile_w;
	ts->hsq = tile_h;
	ts->ref = -1;

	renderPopulateTilesheet(ts, tiles_w, tiles_h);

	return ts;
}


void renderTilesheetGeometrics(TILESHEET *ts, int *w, int *h, int *wsq, int *hsq) {
	if (!ts)
		return;
	if (w)
		*w = ts->w;
	if (h)
		*h = ts->h;
	if (wsq)
		*wsq = ts->wsq;
	if (hsq)
		*hsq = ts->hsq;

	return;
}


void renderUpdateTilesheet(TILESHEET *ts, int pos_x, int pos_y, void *data, int w, int h) {
	unsigned int format;

	glBindTexture(GL_TEXTURE_2D, ts->texhandle);
	format = (ts->format == PFORMAT_A8) ? GL_ALPHA : GL_RGBA;

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, pos_x, pos_y, w, h, format, GL_UNSIGNED_BYTE, data);

	return;
}


void renderTilesheetScalingSet(TILESHEET *ts, unsigned int scaling) {
	unsigned int flag=GL_NEAREST;

	glBindTexture(GL_TEXTURE_2D, ts->texhandle);

	if (scaling == DARNIT_SCALE_NEAREST) {
		flag = GL_NEAREST;
	} else if (scaling == DARNIT_SCALE_LINEAR) {
		flag = GL_LINEAR;
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flag);

	return;
}
