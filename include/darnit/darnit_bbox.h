#ifndef __DARNIT_BBOX_H__
#define	__DARNIT_BBOX_H__

typedef void DARNIT_BBOX;


DARNIT_BBOX *darnitBBoxNew(unsigned int size);
DARNIT_BBOX *darnitBBoxFree(void *bbox);
int darnitBBoxCollTest(DARNIT_BBOX *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen);
int darnitBBoxEntryAdd(DARNIT_BBOX *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void darnitBBoxEntryDelete(DARNIT_BBOX *bbox, int key);
void darnitBBoxEntryMove(DARNIT_BBOX *bbox, int key, unsigned int x, unsigned int y);
void darnitBBoxEntryResize(DARNIT_BBOX *bbox, int key, unsigned int w, unsigned int h);
void darnitBBoxClear(DARNIT_BBOX *bbox);

#endif
