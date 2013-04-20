/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_main.c' - This file is part of libdarnit

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

void *d_init_partial(const char *data_dir);
int d_init_rest(const char *wtitle, int win_w, int win_h, int fullscreen, const char *icon);


<<<<<<< HEAD
=======
void darnit_init_common() {
	#ifdef _WIN32
	void *libtmp;

	SDL_Init(SDL_INIT_NOPARACHUTE);
	libtmp = LoadLibrary(TEXT("DDRAW.DLL"));
	if (libtmp != NULL)
		FreeLibrary(libtmp);
	#endif

	#ifdef _WIN32
	SDL_SetModuleHandle(GetModuleHandle(NULL));
	#endif


	return;
}


>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
void darnitSetPlatform(int partial) {
	DARNIT_ENDIAN_CONVERT end;

	if (!partial) {
		d->platform.screen_w = d->video.w;
		d->platform.screen_h = d->video.h;
	} else {
		#if defined(PANDORA) || defined(MAEMO)
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
	#elif defined MAEMO
		d->platform.platform = DARNIT_PLATFORM_HANDHELD | DARNIT_PLATFORM_MAEMO | DARNIT_PLATFORM_LINUX;
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
	darnitSetPlatform(1);
	if (d_init_rest(wtitle, d->platform.screen_w, d->platform.screen_h, d->platform.fullscreen, icon) < 0)
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
<<<<<<< HEAD
	tpw_cursor_show(0);
	d->fps.time_at_last_frame = d->fps.time_at_flip = tpw_ticks();
	d->fps.time = tpw_ticks() / 1000;
=======
	SDL_ShowCursor(0);
	d->fps.time_at_last_frame = d->fps.time_at_flip = SDL_GetTicks();
	d->fps.time = SDL_GetTicks() / 1000;
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f

	t = videoInit(wtitle, win_w, win_h, fullscreen);
	darnitSetPlatform(0);
	renderLineTest();

	return t;
}


void EXPORT_THIS *d_init_partial(const char *data_dir) {
	if ((d = malloc(sizeof(DARNIT))) == NULL) {
		fprintf(stderr, "libDarnit: Error: Unable to malloc(%i)\n", (int) sizeof(DARNIT));
		return d;
	}

<<<<<<< HEAD
	tpw_init();
=======
	#ifdef _WIN32
	darnit_init_common();
	#endif
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f

	d->platform.platform = 0;
	darnitSetPlatform(1);
	utilInit();

<<<<<<< HEAD
	if (inputInit() < 0);
=======
	if (videoInitPartial());
	else if (inputInit() < 0);
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
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
<<<<<<< HEAD
	int time = tpw_ticks();
=======
	int time = SDL_GetTicks();
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
	
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
<<<<<<< HEAD
	d->fps.time_at_flip = tpw_ticks();
=======
	d->fps.time_at_flip = SDL_GetTicks();
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
	socketConnectLoop();

	return;
}


unsigned int EXPORT_THIS d_time_get() {
<<<<<<< HEAD
	return tpw_ticks();
=======
	return SDL_GetTicks();
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
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
	
	#ifndef PANDORA
<<<<<<< HEAD
	tpw_quit();
=======
	SDL_Quit();
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
	#endif

	exit(0);

	return;
}


void EXPORT_THIS **d_videomode_get() {
<<<<<<< HEAD
	void **nnn = (void **) tpw_videomodes_list();
=======
	void **nnn = (void **) SDL_ListModes(NULL, SDL_HWSURFACE | SDL_FULLSCREEN);
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
	if (nnn == (void **) -1)
		return NULL;
	return nnn;
}
