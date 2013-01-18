#include "darnit.h"


void EXPORT_THIS *d_dynlib_open(const char *fname) {
	return dynlibOpen(fname);
}


void EXPORT_THIS *d_dynlib_get(void *lib, const char *symbol) {
	return dynlibSymbolGet(lib, symbol);
}

void EXPORT_THIS *d_dynlib_close(void *lib) {
	return dynlibClose(lib);
}
