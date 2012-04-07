#include "darner.h"


int bboxAdd(void *handle, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	DARNER *m = handle;
	int i;

	for (i = 0; i < BBOX_MAX; i++)
		if (m->bbox.bbox[i].key == -1) {
			m->bbox.bbox[i].x = x, m->bbox.bbox[i].y = y, m->bbox.bbox[i].w = w;
			m->bbox.bbox[i].h = h, m->bbox.bbox[i].xb = x+w, m->bbox.bbox[i].yb = y+h;
			m->bbox.bbox[i].key = m->bbox.cnt;
			m->bbox.cnt++;
			m->bbox.bbox[i].bboxes++;
			m->bbox.sort = 1;
			return m->bbox.bbox[i].key;
		}
	
	return -1;
}


void bboxDelete(void *handle, int key) {
	DARNER *m = handle;
	int i;

	for (i = 0; i < BBOX_MAX; i++) 
		if (m->bbox.bbox[i].key == key) {
			m->bbox.bbox[i].key = -1;
			m->bbox.sort = 1;
			return;
		}
	
	return;
}


void bboxMove(void *handle, int key, unsigned int x, unsigned int y) {
	DARNER *m = handle;
	int i;

	for (i = 0; i < BBOX_MAX; i++)
		if (m->bbox.bbox[i].key == key) {
			m->bbox.bbox[i].x = x, m->bbox.bbox[i].y = y;
			m->bbox.bbox[i].xb = x + m->bbox.bbox[i].w;
			m->bbox.bbox[i].yb = y + m->bbox.bbox[i].h;
			m->bbox.sort = 1;
			return;
		}
	
	return;
}


void bboxSort(void *handle) {
	DARNER *m = handle;
	int i, j;

	for (i = 1; i < m->bbox.bboxes; i++) {
		if (m->bbox.bbox[i].key == -1)
			continue;
		for (j = i; m->bbox.sortmode == SORT_MODE_X && j > 0 
		  && (m->bbox.bbox[j].x < m->bbox.bbox[j-1].x 
		  || m->bbox.bbox[j-1].key == -1); j--) {
			tmp = m->bbox.bbox[j];
			m->bbox.bbox[j] = m->bbox.bbox[j-1];
			m->bbox.bbox[j-1] = tmp;
		}

		for (j = i; m->bbox.sortmode == SORT_MODE_Y && j > 0
		  && (m->bbox.bbox[j].y < m->bbox.bbox[j-1].y
		  || m->bbox.bbox[j-1].key == -1); j--) {
			tmp = m->bbox.bbox[j];
			m->bbox.bbox[j] = m->bbox.bbox[j-1];
			m->bbox.bbox[j-1] = tmp;
		}
	}

	return;
}
		


void bboxClear(void *handle) {
	DARNER *m = handle;
	int i;

	for (i = 0; i < BBOX_MAX; i++)
		m->bbox.bbox[i].key = -1;
	
	m->bbox.bboxes = 0;
	m->bbox.cnt = 0;
	m->bbox.sort = 0;

	return;
}


int bboxInit(void *handle) {
	DARNER *m = handle;

	bboxClear();
	m->bbox.sortmode = SORT_MODE_X;
	
	return 0;
}



int bboxCollBoxTest(void *handle, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen) {
	DARNER *m = handle;
	DARNER_BBOX_ENTRY tmp;
	int i, test, no;

	if (m->bbox.sort)
		bboxSort(m);
	
	test = i = m->bbox.bboxes>>1;
	i >>= 1;

	if (m->bbox.sortmode == SORT_MODE_X) {
		for (; i > TARGET_FILTER; i>>=1) {
			if (m->bbox.bbox[test+i].xb < x)
				test += i;
			else if (m->bbox.bbox[test].xb < x);
			else
				test -= i;
		}

		if (m->bbox.bbox[test].xb > x)
			test = 0;
	} else {
		for (; i > TARGET_FILTER; i>>=1) {
			if (m->bbox.bbox[test+i].yb < y)
				test += i;
			else if (m->bbox.bbox[test].yb < y);
			else
				test -= i;
		}

		if (m->bbox.bbox[test].yb > y)
			test = 0;
	}
	
	no = 0;

	for (i = test; i < m->bbox.bboxes; i++) {
		if (m->bbox.sort == SORT_MODE_X && m->bbox.bbox[i].x > x+w)
			break;
		if (m->bbox.sort == SORT_MODE_Y && m->bbox.bbox[i].y > y+h)
			break;
		if (m->bbox.bbox[i].xb <= x || m->bbox.bbox[i].x >= x+w)
			continue;
		if (m->bbox.bbox[i].yb <= y || m->bbox.bbox[i].y >= y+h)
			continue;
		if (no == listlen)
			break;
		list[no] = m->bbox.bbox[i].key;
		no++;
	}

	return no;
}

