#include "darnit.h"

void *d_init_partial(const char *data_dir);
int d_init_rest(const char *wtitle, int win_w, int win_h, int fullscreen, const char *icon);


static void cleanup(void) {
	#ifdef _WIN32
//	SDL_Quit();
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


void darnitSetPlatform(int partial) {
	DARNIT_ENDIAN_CONVERT end;

	if (!partial) {
		d->platform.screen_w = d->video.w;
		d->platform.screen_h = d->video.h;
	} else {
		#if defined PANDORA
			d->platform.screen_w = 800;
			d->platform.screen_h = 480;
			d->platform.fullscreen = 1;
		#elif defined GCW_ZERO
			d->platform.screen_w = 320;
			d->platform.screen_h = 240;
			d->platform.fullscreen = 1;
		#else
			d->platform.screen_w = 800;
			d->platform.screen_h = 480;
			d->platform.fullscreen = 0;
		#endif
	}

	#if defined PANDORA
		d->platform.platform = DARNIT_PLATFORM_HANDHELD | DARNIT_PLATFORM_PANDORA;
	#elif defined _WIN32
		d->platform.platform = DARNIT_PLATFORM_DESKTOP | DARNIT_PLATFORM_WIN32;
	#elif defined GCW_ZERO
		d->platform.platform = DARNIT_PLATFORM_DESKTOP | DARNIT_PLATFORM_GCWZERO;
	#else
		d->platform.platform = DARNIT_PLATFORM_DESKTOP | DARNIT_PLATFORM_LINUX;
	#endif

	end.i = 0xFF000000;
	if (end.c[0] == 0xFF)
		d->platform.platform |= DARNIT_PLATFORM_BIGENDIAN;

	return;
}


void EXPORT_THIS *d_init(const char *wtitle, const char *data_dir, const char *icon) {
	if (d_init_partial(data_dir) == NULL)
		return NULL;
	#ifdef PANDORA
	if (d_init_rest(wtitle, 800, 480, 1, icon) < 0)
	#elif defined GCW_ZERO
	if (d_init_rest(wtitle, 320, 240, 1, icon) < 0)
	#else
	if (d_init_rest(wtitle, 800, 480, 0, icon) < 0)
	#endif
		return NULL;

	return d;
}


void EXPORT_THIS *d_init_custom(const char *wtitle, int win_w, int win_h, int fullscreen, const char *data_dir, const char *icon) {
	if (d_init_partial(data_dir) == NULL)
		return NULL;
	if (d_init_rest(wtitle, win_w, win_h, fullscreen, icon) < 0)
		return NULL;
	return d;
}


int EXPORT_THIS d_init_rest(const char *wtitle, int win_w, int win_h, int fullscreen, const char *icon) {
	int t;

	videoSetIcon(icon);
	renderInit();
	SDL_ShowCursor(0);
	d->fps.time_at_last_frame = d->fps.time_at_flip = SDL_GetTicks();
	d->fps.time = SDL_GetTicks() / 1000;

	t = videoInit(wtitle, win_w, win_h, fullscreen);
	darnitSetPlatform(0);

	return t;
}


void EXPORT_THIS *d_init_partial(const char *data_dir) {
	if ((d = malloc(sizeof(DARNIT))) == NULL) {
		fprintf(stderr, "libDarnit: Error: Unable to malloc(%i)\n", (int) sizeof(DARNIT));
		return d;
	}

	#ifdef _WIN32
	darnit_init_common();
	#endif
	
	darnitSetPlatform(1);
	utilInit();

	if (videoInitPartial());
	else if (inputInit() < 0);
	else if (audioInit() < 0);
	else if (socketInit() < 0);
	else {
		if (fsInit(data_dir) < 0)
			return NULL;
		return d;
	}

	free(d);
	return NULL;
}


void EXPORT_THIS d_loop() {
	int time = SDL_GetTicks();
	
	if (time / 1000 != d->fps.time) {
		d->fps.frames_last = d->fps.frames_counting;
		d->fps.frames_counting = 0;
		d->fps.time = time / 1000;
	}

	d->fps.frames_counting++;
	
	renderFadeLoop(d);
	renderTilesheetAnimateAll();
	videoLoop(d);
	inputPoll(d);

	d->fps.time_at_last_frame = d->fps.time_at_flip;
	d->fps.time_at_flip = SDL_GetTicks();
	socketConnectLoop();

	return;
}


unsigned int EXPORT_THIS d_time_get() {
	return SDL_GetTicks();
}


int EXPORT_THIS d_fps() {
	return d->fps.frames_last;
}


int EXPORT_THIS d_last_frame_time() {
	return d->fps.time_at_flip - d->fps.time_at_last_frame;
}


int darnitTimeLastFrameTook() {
	return d_last_frame_time();
}


DARNIT_PLATFORM EXPORT_THIS d_platform_get() {
	return d->platform;
}


void EXPORT_THIS d_quit() {
	videoDestroy();

	exit(0);

	return;
}


void EXPORT_THIS **d_videomode_get() {
	void **nnn = (void **) SDL_ListModes(NULL, SDL_HWSURFACE | SDL_FULLSCREEN);
	if (nnn == (void **) -1)
		return NULL;
	return nnn;
}
