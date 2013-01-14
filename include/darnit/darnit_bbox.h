#ifndef __DARNIT_BBOX_H__
#define	__DARNIT_BBOX_H__

typedef void DARNIT_BBOX;


DARNIT_BBOX *d_bbox_new(unsigned int size);
DARNIT_BBOX *d_bbox_free(DARNIT_BBOX);
int d_bbox_test(DARNIT_BBOX *bbox, int x, int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen);
int d_bbox_add(DARNIT_BBOX *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void d_bbox_delete(DARNIT_BBOX *bbox, int key);
void d_bbox_move(DARNIT_BBOX *bbox, int key, unsigned int x, unsigned int y);
void d_bbox_resize(DARNIT_BBOX *bbox, int key, unsigned int w, unsigned int h);
void d_bbox_clear(DARNIT_BBOX *bbox);

#endif
