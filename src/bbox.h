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
} DARNER_BBOX_ENTRY;


typedef struct {
	DARNER_BBOX_ENTRY		bbox[BBOX_MAX];
	unsigned int			bboxes;
	unsigned int			sort;
	unsigned int			cnt;
	unsigned int			sortmode;
} BBOX;


int bboxAdd(void *handle, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void bboxDelete(void *handle, int key);
void bboxMove(void *handle, int key, unsigned int x, unsigned int y);
void bboxSort(void *handle);
void bboxClear(void *handle);
int bboxInit(void *handle);
int bboxCollBoxTest(void *handle, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen);



#endif
