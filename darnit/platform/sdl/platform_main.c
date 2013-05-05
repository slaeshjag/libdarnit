/*
Copyright (c) 2013 Steven Arnow
'main_sdl.c' - This file is part of libdarnit_tpw

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

#define	TPW_INTERNAL
#include "../main.h"


int tpw_init_platform() {
	#ifdef _WIN32
	void *libtmp;
	#endif

	#ifdef _WIN32
	SDL_Init(SDL_INIT_NOPARACHUTE);
	if (!(libtmp = LoadLibrary(TEXT("DDRAW.DLL"))))
		FreeLibrary(libtmp);

	SDL_SetModuleHandle(GetModuleHandle(NULL));
	#endif

	SDL_Init(SDL_INIT_EVERYTHING);

	return 1;
}


int tpw_window_create(const char *title, unsigned int window_w, unsigned int window_h, unsigned int fullscreen, const unsigned int bpp) {
	unsigned int mode;
	#ifdef HAVE_GLES
	EGLint configs_avail = 0;
	SDL_SysWMinfo sysinfo;
	#endif

	#ifndef HAVE_GLES
	mode = SDL_OPENGL;
	#else
	mode = SDL_SWSURFACE;
	#endif

	if (fullscreen) mode |= SDL_FULLSCREEN;

	SDL_WM_SetCaption(title, title);

	#ifdef HAVE_GLES
	if (!(tpw.XDisplay = XOpenDisplay(NULL))) {
		fprintf(stderr, "tpw_window_create(): Unable to open the default display\n");
		return 0;
	}

	if (!(tpw.eglDisplay = eglGetDisplay((EGLNativeDisplayType) tpw.XDisplay))) {
		fprintf(stderr, "tpw_window_create(): Unable to get a display handle from EGL\n");
		return 0;
	}

	if (!eglInitialize(tpw.eglDisplay, NULL, NULL)) {
		fprintf(stderr, "tpw_window_create(): Unable to initialize EGL\n");
		return 0;
	}
	#endif

	if (!(tpw.screen = SDL_SetVideoMode(window_w, window_h, bpp, mode)))
		return 0;
	
	#ifdef HAVE_GLES
	if (eglChooseConfig(tpw.eglDisplay, egl_config_attrib, &tpw.eglConfig, 1, &configs_avail) != EGL_TRUE) {
		fprintf(stderr, "tpw_window_create(): Unable to find a config for EGL(%i)\n", configs_avail);
		return 0;
	}

	SDL_VERSION(&sysinfo.version);
	if (SDL_GetWMInfo(&sysinfo) <= 0) {
		fprintf(stderr, "tpw_window_create(): Unable to get a window handle\n");
		return 0;
	}

	if ((tpw.eglSurface = eglCreateWindowSurface(tpw.eglDisplay, tpw.eglConfig, NULL, NULL)) == EGL_NO_SURFACE) {
		fprintf(stderr, "tpw_window_create(): Unable to create an EGL surface\n");
		return 0;
	}

	eglBindAPI(EGL_OPENGL_ES_API);
	if ((tpw.eglContext = eglCreateContext(tpw.eglDisplay, tpw.eglConfig, NULL, NULL)) == EGL_NO_CONTEXT) {
		fprintf(stderr, "tpw_window_create(): Unable to create an EGL context\n");
		return 0;
	}

	if (eglMakeCurrent(tpw.eglDisplay, tpw.eglSurface, tpw.eglSurface, tpw.eglContext) == EGL_FALSE) {
		fprintf(stderr, "tpw_window_create(): Unable to make the EGL context current\n");
		return 0;
	}
	#endif


	return 1;
}


void tpw_sleep(unsigned int msec) {
	SDL_Delay(msec);

	return;
}


unsigned int tpw_ticks() {
	return SDL_GetTicks();
}


void tpw_input_grab(TPW_INPUT_GRAB grab) {
	switch (grab) {
		case TPW_INPUT_GRAB_GRAB:
			SDL_WM_GrabInput(SDL_GRAB_ON);
			break;
		case TPW_INPUT_GRAB_RELEASE:
			SDL_WM_GrabInput(SDL_GRAB_OFF);
			break;
	}

	return;
}


void tpw_render_buffer_swap() {
	#ifndef HAVE_GLES
	SDL_GL_SwapBuffers();
	#else
	eglSwapBuffers(tpw.eglDisplay, tpw.eglSurface);
	#endif

	return;
}


const char *tpw_key_name_get(int sym) {
	return SDL_GetKeyName(sym);
}


TPW_RECT **tpw_videomodes_list() {
	return (TPW_RECT **) SDL_ListModes(NULL, SDL_HWSURFACE | SDL_FULLSCREEN);
}


void tpw_input_unicode(int enable) {
	SDL_EnableUNICODE(enable);
	
	return;
}


void tpw_cursor_show(unsigned int show) {
	SDL_ShowCursor(0);

	return;
}


int tpw_icon_set(void *pixdata_rgba32, int w, int h) {
	SDL_Surface *surface;

	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	memcpy(surface->pixels, pixdata_rgba32, w * h * 4);

	SDL_WM_SetIcon(surface, NULL);

	return 1;
}


void tpw_quit() {
	SDL_Quit();

	return;
}
