#ifndef __DARNIT_DYNLIB_H__
#define	__DARNIT_DYNLIB_H__

typedef void DARNIT_DYNLIB;

DARNIT_DYNLIB *darnitDynlibOpen(const char *fname);
void *darnitDynlibGet(DARNIT_DYNLIB *lib, const char *symbol);
DARNIT_DYNLIB *darnitDynlibClose(DARNIT_DYNLIB *lib);

#endif
