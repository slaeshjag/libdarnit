#include "darnit.h"


void EXPORT_THIS *darnitBBoxNew(unsigned int size) {
	return bboxNew(size);
}


void EXPORT_THIS *darnitBBoxFree(BBOX *bbox) {
	bboxFree(bbox);

	return NULL;
}


int EXPORT_THIS darnitBBoxCollTest(BBOX *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen) {
	return bboxCollBoxTest(bbox, x, y, w, h, list, listlen);
}


int EXPORT_THIS darnitBBoxEntryAdd(BBOX *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	return bboxAdd(bbox, x, y, w, h);
}


void EXPORT_THIS darnitBBoxEntryDelete(BBOX *bbox, int key) {
	bboxDelete(bbox, key);

	return;
}


void EXPORT_THIS darnitBBoxEntryMove(BBOX *bbox, int key, unsigned int x, unsigned int y) {
	bboxMove(bbox, key, x, y);

	return;
}


void EXPORT_THIS darnitBBoxEntryResize(BBOX *bbox, int key, unsigned int w, unsigned int h) {
	bboxResize(bbox, key, w, h);

	return;
}


void EXPORT_THIS darnitBBoxClear(BBOX *bbox) {
	bboxClear(bbox);

	return;
}

