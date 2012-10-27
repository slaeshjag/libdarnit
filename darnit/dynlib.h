#ifndef __DYNLIB_H__
#define __DYNLIB_H__


void *dynlibOpen(const char *fname);
void *dynlibSymbolGet(void *lib, const char *symbol);
void *dynlibClose(void *lib);


#endif
