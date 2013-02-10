/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_bbox.c' - This file is part of libdarnit

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


void EXPORT_THIS *d_bbox_new(unsigned int size) {
	return bboxNew(size);
}


void EXPORT_THIS *d_bbox_free(BBOX *bbox) {
	bboxFree(bbox);

	return NULL;
}


int EXPORT_THIS d_bbox_test(BBOX *bbox, int x, int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen) {
	return bboxCollBoxTest(bbox, x, y, w, h, list, listlen);
}


int EXPORT_THIS d_bbox_add(BBOX *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	return bboxAdd(bbox, x, y, w, h);
}


void EXPORT_THIS d_bbox_delete(BBOX *bbox, int key) {
	bboxDelete(bbox, key);

	return;
}


void EXPORT_THIS d_bbox_move(BBOX *bbox, int key, unsigned int x, unsigned int y) {
	bboxMove(bbox, key, x, y);

	return;
}


void EXPORT_THIS d_bbox_resize(BBOX *bbox, int key, unsigned int w, unsigned int h) {
	bboxResize(bbox, key, w, h);

	return;
}


void EXPORT_THIS d_bbox_Clear(BBOX *bbox) {
	bboxClear(bbox);

	return;
}

