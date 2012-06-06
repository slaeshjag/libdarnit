#ifndef __DARNIT_MAIN_H__
#define	__DARNIT_MAIN_H__

void *darnitInit(const char *wtitle);
void *darnitInitCustom(const char *wtitle, int win_w, int win_h, int fullscreen);
void darnitLoop(void *handle);
unsigned int darnitTimeGet();
int darnitFPSGet(void *handle);
int darnitTimeLastFrameTook(void *handle);

#endif
