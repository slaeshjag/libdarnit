/*
Copyright (c) 2011-2013 Steven Arnow
'dynlib.c' - This file is part of libdarnit

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/


#include "darnit.h"


void *dynlibOpen(const char *fname) {
	#ifdef _WIN32
	char *fname_n;
	#endif
	DYNLIB *dl = malloc(sizeof(DYNLIB));
	if (fname == NULL) return NULL;
	
	if (!(dl->tmp = fsGetRealFile(fname)))
		return NULL;

	#ifdef _WIN32
		
		fname_n = utilPathTranslate(dl->tmp->file);
		fname_n = realloc(fname_n, strlen(fname_n) + 2);
		strcat(fname_n, ".");
		fprintf(stderr, "Loading %s\n", fname_n);
		dl->handle = LoadLibrary(fname_n);
		free(fname_n);
	#else
		dl->handle = dlopen(dl->tmp->file, RTLD_NOW | RTLD_GLOBAL);
	#endif
	return dl;
}


void *dynlibSymbolGet(DYNLIB *lib, const char *symbol) {
	if (lib == NULL || symbol == NULL) return NULL;
	#ifdef _WIN32
		void *sym;

		sym = GetProcAddress(lib->handle, symbol);
		return sym;
	#else
		void *sym;
		
		sym = dlsym(lib->handle, symbol);
		return sym;
	#endif
}


void *dynlibClose(DYNLIB *lib) {
	if (lib == NULL) return NULL;
	#ifdef _WIN32
		FreeLibrary(lib->handle);
	#else
		dlclose(lib->handle);
	#endif
	
	fsFileClose(lib->tmp);
	free(lib);

	return NULL;
}
