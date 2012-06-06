#ifndef __DARNIT_DYNLIB_H__
#define	__DARNIT_DYNLIB_H__

void *darnitDynlibOpen(const char *fname);
void *darnitDynlibGet(void *lib, const char *symbol);
void *darnitDynlibClose(void *lib);

#endif
