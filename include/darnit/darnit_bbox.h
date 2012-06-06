#ifndef __DARNIT_BBOX_H__
#define	__DARNIT_BBOX_H__

void *darnitBBoxNew(unsigned int size);
void *DarnerBBoxFree(void *bbox);
int darnitBBoxCollTest(void *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen);
int darnitBBoxEntryAdd(void *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void darnitBBoxEntryDelete(void *bbox, int key);
void darnitBBoxEntryMove(void *bbox, int key, unsigned int x, unsigned int y);
void darnitBBoxEntryResize(void *bbox, int key, unsigned int w, unsigned int h);
void darnitBBoxClear(void *bbox);

#endif
