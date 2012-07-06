#include "darnit.h"


static void cleanup(void) {
	#ifdef _WIN32
	SDL_Quit();
	#endif
}


void darnit_init_common() {
	#ifdef _WIN32
	void *libtmp;

	SDL_Init(SDL_INIT_NOPARACHUTE);
	libtmp = LoadLibrary(TEXT("DDRAW.DLL"));
	if (libtmp != NULL)
		FreeLibrary(libtmp);
	#endif


	atexit(cleanup);

	#ifdef _WIN32
	SDL_SetModuleHandle(GetModuleHandle(NULL));
	#endif


	return;
}


void EXPORT_THIS *darnitInit(const char *wtitle) {
	DARNIT *d;

	if ((d = malloc(sizeof(DARNIT))) == NULL) {
		fprintf(stderr, "libDarnit: Error: Unable to malloc(%i)\n", (int) sizeof(DARNIT));
		return d;
	}

#ifdef _WIN32
	darnit_init_common();
#endif
	renderInit(d);
	d->fps.time_at_last_frame = d->fps.time_at_flip = SDL_GetTicks();
	d->fps.time = SDL_GetTicks() / 1000;
	
	#ifdef PANDORA
	if (videoInit(d, wtitle, 800, 480, 1) < 0);
	#else
	if (videoInit(d, wtitle, 800, 480, 0) < 0);
	#endif
	else if (inputInit(d) < 0);
	else if (audioInit(d) < 0); 
	else return d;
	
	free(d);

	return NULL;
}


void EXPORT_THIS *darnitInitCustom(const char *wtitle, int win_w, int win_h, int fullscreen) {
	DARNIT *d;

	if ((d = malloc(sizeof(DARNIT))) == NULL) {
		fprintf(stderr, "libDarnit: Error: Unable to malloc(%i)\n", (int) sizeof(DARNIT));
		return d;
	}

	darnit_init_common();
	renderInit(d);
	d->fps.time_at_last_frame = d->fps.time_at_flip = SDL_GetTicks();
	d->fps.time = SDL_GetTicks() / 1000;
	
	if (videoInit(d, wtitle, win_w, win_h, fullscreen) < 0);
	else if (inputInit(d) < 0);
	else if (audioInit(d) < 0);
	else return d;
	
	free(d);
	return NULL;
}


void EXPORT_THIS darnitLoop(void *handle) {
	DARNIT *d = handle;
	int time = SDL_GetTicks();
	
	if (time / 1000 != d->fps.time) {
		d->fps.frames_last = d->fps.frames_counting;
		d->fps.frames_counting = 0;
		d->fps.time = time / 1000;
	}

	d->fps.frames_counting++;
	
	renderFadeLoop(d);
	videoLoop(d);
	inputPoll(d);

	d->fps.time_at_last_frame = d->fps.time_at_flip;
	d->fps.time_at_flip = SDL_GetTicks();

	return;
}


unsigned int EXPORT_THIS darnitTimeGet() {
	return SDL_GetTicks();
}


int EXPORT_THIS darnitFPSGet(void *handle) {
	DARNIT *d = handle;

	return d->fps.frames_last;
}


int EXPORT_THIS darnitTimeLastFrameTook(void *handle) {
	DARNIT *d = handle;

	return d->fps.time_at_flip - d->fps.time_at_last_frame;
}
