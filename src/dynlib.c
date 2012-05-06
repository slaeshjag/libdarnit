#include "darnit.h"


void *dynlibOpen(const char *fname) {
	if (fname == NULL) return NULL;
	#ifdef _WIN32
		HINSTANCE *lib;

		lib = malloc(sizeof(HINSTANCE));
		*lib = LoadLibrary(fname);
		return lib;
	#else
		char *fname_n;
		void *lib;

		fname_n = malloc(strlen(fname + 3));
		sprintf(fname_n, "./%s", fname);
		lib = dlopen(fname_n, RTLD_NOW | RTLD_GLOBAL);
		free(fname_n);
		return lib;
	#endif
}


void *dynlibSymbolGet(void *lib, const char *symbol) {
	if (lib == NULL || symbol == NULL) return NULL;
	#ifdef _WIN32
		HINSTANCE *libh = lib;
		void *sym;

		sym = GetProcAddress(*libh, symbol);
		return sym;
	#else
		void *sym;
		
		sym = dlsym(lib, symbol);
		return sym;
	#endif
}


void *dynlibClose(void *lib) {
	if (lib == NULL) return NULL;
	#ifdef _WIN32
		HINSTANCE *libh = lib;

		freeLibrary(*libh);
		free(lib);
		return NULL;
	#else
		dlclose(lib);
		return NULL;
	#endif
}
