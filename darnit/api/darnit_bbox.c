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

