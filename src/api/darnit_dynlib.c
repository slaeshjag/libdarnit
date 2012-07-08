#include "darnit.h"


void EXPORT_THIS *darnitDynlibOpen(const char *fname) {
	return dynlibOpen(fname);
}


void EXPORT_THIS *darnitDynlibGet(void *lib, const char *symbol) {
	return dynlibSymbolGet(lib, symbol);
}

void EXPORT_THIS *darnitDynlibClose(void *lib) {
	return dynlibClose(lib);
}
