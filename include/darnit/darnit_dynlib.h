#ifndef __DARNIT_DYNLIB_H__
#define	__DARNIT_DYNLIB_H__

typedef void DARNIT_DYNLIB;

DARNIT_DYNLIB *d_dynlib_open(const char *fname);
void *d_dynlib_get(DARNIT_DYNLIB *lib, const char *symbol);
DARNIT_DYNLIB *d_dynlib_close(DARNIT_DYNLIB *lib);

#endif
