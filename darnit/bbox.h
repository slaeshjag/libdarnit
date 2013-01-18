#ifndef __BBOX_H__
#define	__BBOX_H__


#define			BBOX_MAX		8192
#define			TARGET_FILTER		8
#define			SORT_MODE_X		0
#define			SORT_MODE_Y		1


typedef struct {
	unsigned int			x;
	unsigned int			y;
	unsigned int			w;
	unsigned int			h;
	unsigned int			xb;
	unsigned int			yb;
	int				key;
} DARNIT_BBOX_ENTRY;


typedef struct {
	DARNIT_BBOX_ENTRY		*bbox;
	unsigned int			bboxes;
	unsigned int			sort;
	unsigned int			cnt;
	unsigned int			sortmode;
	unsigned int			max;
} BBOX;


int bboxAdd(BBOX *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void bboxDelete(BBOX *bbox, int key);
void bboxMove(BBOX *bbox, int key, unsigned int x, unsigned int y);
void bboxResize(BBOX *bbox, int key, unsigned int w, unsigned int h);
void bboxSort(BBOX *bbox);
void bboxClear(BBOX *bbox);
void *bboxNew(unsigned int size);
void *bboxFree(BBOX *bbox);
int bboxCollBoxTest(BBOX *bbox, int x, int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen);


#endif
