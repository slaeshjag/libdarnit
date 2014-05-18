/*
Copyright (c) 2011-2013 Steven Arnow
'bbox.c' - This file is part of libdarnit

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


int bboxAdd(BBOX *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	if (bbox == NULL) return -1;
	int i;

	for (i = 0; i < bbox->max; i++)
		if (bbox->bbox[i].key == -1)
			break;
	if (i == bbox->max) {
		bbox->bbox = realloc(bbox->bbox, sizeof(*bbox->bbox) * bbox->max);
		bbox->bbox[bbox->max].a_key = bbox->max;
		bbox->max++;
	}
	
	bbox->bbox[i].x = x, bbox->bbox[i].y = y, bbox->bbox[i].w = w;
	bbox->bbox[i].h = h, bbox->bbox[i].xb = x+w, bbox->bbox[i].yb = y+h;
	if (bbox->keymode)
		bbox->bbox[i].key = bbox->bbox[i].a_key;
	else
		bbox->bbox[i].key = bbox->cnt++;
	bbox->bboxes++;
	bbox->sort = 1;
	return bbox->bbox[i].key;
}


void bboxDelete(BBOX *bbox, int key) {
	if (bbox == NULL) return;
	int i;

	for (i = 0; i < bbox->max; i++) 
		if (bbox->bbox[i].key == key) {
			bbox->bbox[i].key = -1;
			bbox->sort = 1;
			bbox->bboxes--;
			return;
		}
	
	return;
}


void bboxMove(BBOX *bbox, int key, unsigned int x, unsigned int y) {
	if (bbox == NULL) return; 
	int i;

	for (i = 0; i < bbox->max; i++)
		if (bbox->bbox[i].key == key) {
			bbox->bbox[i].x = x, bbox->bbox[i].y = y;
			bbox->bbox[i].xb = x + bbox->bbox[i].w;
			bbox->bbox[i].yb = y + bbox->bbox[i].h;
			bbox->sort = 1;
			return;
		}
	
	return;
}


void bboxResize(BBOX *bbox, int key, unsigned int w, unsigned int h) {
	if (bbox == NULL) return;
	int i;
	
	for (i = 0; i < bbox->max; i++) 
		if (bbox->bbox[i].key == key) {
			bbox->bbox[i].xb = bbox->bbox[i].x + w;
			bbox->bbox[i].yb = bbox->bbox[i].y + h;
			bbox->bbox[i].w = w;
			bbox->bbox[i].h = h;
			return;
		}
	
	return;
}


void bboxSort(BBOX *bbox) {
	if (bbox == NULL) return;
	DARNIT_BBOX_ENTRY tmp;
	int i, j;

	for (i = 1; i < bbox->max; i++) {
		if (bbox->bbox[i].key == -1)
			continue;
		for (j = i; bbox->sortmode == SORT_MODE_X && j > 0 
		  && (bbox->bbox[j].x < bbox->bbox[j-1].x || 
		  (bbox->bbox[j].x == bbox->bbox[j-1].x && bbox->bbox[j].y < bbox->bbox[j-1].y)
		  || bbox->bbox[j-1].key == -1); j--) {
			tmp = bbox->bbox[j];
			bbox->bbox[j] = bbox->bbox[j-1];
			bbox->bbox[j-1] = tmp;
		}

		for (j = i; bbox->sortmode == SORT_MODE_Y && j > 0
		  && (bbox->bbox[j].y < bbox->bbox[j-1].y
		  || bbox->bbox[j-1].key == -1); j--) {
			tmp = bbox->bbox[j];
			bbox->bbox[j] = bbox->bbox[j-1];
			bbox->bbox[j-1] = tmp;
		}
	}

	bbox->sort = 0;

	return;
}
		


void bboxClear(BBOX *bbox) {
	if (bbox == NULL) return;
	int i;

	for (i = 0; i < bbox->max; i++)
		bbox->bbox[i].key = -1;
	
	bbox->bboxes = 0;
	bbox->cnt = bbox->max;
	bbox->sort = 0;

	return;
}


int bboxCollBoxTest(BBOX *bbox, int x, int y, int w, int h, unsigned int *list, unsigned int listlen) {
	if (bbox == NULL) return 0;
	int i, test, no;

	if (bbox->sort)
		bboxSort(bbox);
	
	test = i = bbox->bboxes>>1;
	i >>= 1;

	if (bbox->sortmode == SORT_MODE_X) {
		for (; i > TARGET_FILTER; i>>=1) {
			if (bbox->bbox[test+i].xb > x)
				test += i;
			else if (bbox->bbox[test].xb > x);
			else
				test -= i;
		}

		if (bbox->bbox[test].xb > x || bbox->bbox[test].key == -1)
			test = 0;
	} else {
		for (; i > TARGET_FILTER; i>>=1) {
			if (bbox->bbox[test+i].yb > y)
				test += i;
			else if (bbox->bbox[test].yb > y);
			else
				test -= i;
		}

		if (bbox->bbox[test].yb > y || bbox->bbox[test].key == -1)
			test = 0;
	}
	
	no = 0;

	for (i = test; i < bbox->bboxes; i++) {
		if (bbox->sortmode == SORT_MODE_X && bbox->bbox[i].x > x+w)
			break;
		if (bbox->sortmode == SORT_MODE_Y && bbox->bbox[i].y > y+h)
			break;
		if (bbox->bbox[i].xb <= x || bbox->bbox[i].x >= x+w)
			continue;
		if (bbox->bbox[i].yb <= y || bbox->bbox[i].y >= y+h)
			continue;
		if (no == listlen)
			break;
		list[no] = bbox->bbox[i].key;
		no++;
	}

	return no;
}



void *bboxNew(unsigned int size) {
	BBOX *bbox;
	int i;

	if ((bbox = malloc(sizeof(BBOX))) == NULL)
		return NULL;
	
	if ((bbox->bbox = malloc(sizeof(DARNIT_BBOX_ENTRY)*size)) == NULL) {
		free(bbox);
		return NULL;
	}

	bbox->max = size;

	for (i = 0; i < bbox->max; i++) {
		bbox->bbox[i].xb = -1;
		bbox->bbox[i].yb = -1;
		bbox->bbox[i].key = -1;
		bbox->bbox[i].a_key = i;
	}

	bboxSort(bbox);
	bbox->sortmode = SORT_MODE_X;
	bbox->bboxes = 0;
	bbox->cnt = i;
	bbox->keymode = 0;

	return bbox;
}



void *bboxFree(BBOX *bbox) {
	if (bbox == NULL) return NULL;

	free(bbox->bbox);
	free(bbox);

	return NULL;
}
