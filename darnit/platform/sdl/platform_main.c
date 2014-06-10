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


// Hack for Vsync on linux
#ifndef NO_GLX
#ifndef HAVE_GLES
void glXSwapIntervalEXT(int mode);
#endif
#endif

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
	#ifdef RASPBERRYPI
	bcm_host_init();
	if (!(tpw.eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY))) {
		fprintf(stderr, "tpw_window_create(): Unable to get a display handle from EGL!!\n");
		return 0;
	}
	#else
	if (!(tpw.eglDisplay = eglGetDisplay((EGL_DEFAULT_DISPLAY)))) {
		fprintf(stderr, "tpw_window_create(): Unable to get a display handle from EGL\n");
		return 0;
	}
	#endif

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

	#ifndef RASPBERRYPI
	if ((tpw.eglSurface = eglCreateWindowSurface(tpw.eglDisplay, tpw.eglConfig, (EGLNativeWindowType) NULL, NULL)) == EGL_NO_SURFACE) {
		fprintf(stderr, "tpw_window_create(): Unable to create an EGL surface\n");
		return 0;
	}
	
	#else
	/* RaspberryPi's videosystem är helt jävla efterblivet. Den som designade det här och tyckte att det var en bra idé kan gå och knulla sig själv med en motorsåg... Det finns inte ord som kan beskriva hur mycket jag hatar härket. */
	XWindowAttributes xwa;
	VC_RECT_T dst_rect, src_rect;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;

	XGetWindowAttributes(tpw.XDisplay, sysinfo.info.x11.window, &xwa);
	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = window_w << 16;
	src_rect.height = window_h << 16;
	
	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = window_w;
	dst_rect.height = window_h;


	dispman_display = vc_dispmanx_display_open(0);
	dispman_update = vc_dispmanx_update_start(0);
	tpw.nativewindow.element = vc_dispmanx_element_add(dispman_update, dispman_display, 0, &dst_rect, 0, &src_rect, DISPMANX_PROTECTION_NONE, 0, 0, 0);
	tpw.nativewindow.width = window_w;
	tpw.nativewindow.height = window_h;
	vc_dispmanx_update_submit_sync(dispman_update);

	if ((tpw.eglSurface = eglCreateWindowSurface(tpw.eglDisplay, tpw.eglConfig, (EGLNativeWindowType) &tpw.nativewindow, NULL)) == EGL_NO_SURFACE) {
		fprintf(stderr, "tpw_window_create(): Unable to create an EGL surface %X\n", eglGetError());
		return 0;
	}
	#endif
	
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


	#ifndef NO_GLX
	#ifndef HAVE_GLES
//	glXSwapIntervalEXT(1);
	#endif
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
	SDL_ShowCursor(show);

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
