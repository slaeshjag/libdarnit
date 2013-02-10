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
	char *fname_n;
	char path[DARNIT_PATH_MAX];
	FILESYSTEM_FILE *f;
	if (fname == NULL) return NULL;
	
	/* Find a file that matches the path */
	if (strlen(d->fs.data_dir) + 2 + strlen(fname) > DARNIT_PATH_MAX)
		return NULL;
	if (strlen(d->fs.write_dir) + 2 + strlen(fname) > DARNIT_PATH_MAX)
		return NULL;
	
	/* First, look in write-dir */
	sprintf(path, "%s/%s", d->fs.write_dir, fname);
	if (!(f = fsFileOpen(path, "r"))) {
		/* Nope. Look in data dir instead */
		sprintf(path, "%s/%s", d->fs.data_dir, fname);
		if (!(f = fsFileOpen(path, "r")))
			/* File doesn't seem to exist */
			return NULL;
	}

	fsFileClose(f);


	#ifdef _WIN32
		fname_n = utilPathTranslate(path);
		HINSTANCE *lib;

		lib = malloc(sizeof(HINSTANCE));
		*lib = LoadLibrary(fname_n);
	#else
		void *lib;

		fname_n = malloc(strlen(path) + 1);
		sprintf(fname_n, "%s", path);
		lib = dlopen(fname_n, RTLD_NOW | RTLD_GLOBAL);
	#endif
	free(fname_n);
	return lib;
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

		FreeLibrary(*libh);
		free(lib);
		return NULL;
	#else
		dlclose(lib);
		return NULL;
	#endif
}
