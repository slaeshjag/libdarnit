/*
Copyright (c) 2013 Steven Arnow
'main_sdl.h' - This file is part of libdarnit_tpw

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

#ifndef __PLATFORM_SDL_MAIN_H__
#define	__PLATFORM_SDL_MAIN_H__

#include <SDL/SDL.h>

#ifdef HAVE_GLES
#include <X11/xutil.h>
#ifndef RASPBERRYPI
	#include <GLES/egl.h>
#endif
#include <EGL/egl.h>
#include <GLES/glext.h>
#include <SDL/SDL_syswm.h>
#else
#include <SDL/SDL_opengl.h>
#endif



#ifdef TPW_INTERNAL
#ifdef HAVE_GLES
static const EGLint egl_config_attrib[] = {
	#ifndef MAEMO
	EGL_RED_SIZE,		5,
	EGL_GREEN_SIZE		6,
	EGL_BLUE_SIZE,		5,
	EGL_DEPTH_SIZE,		16,
	#endif
	EGL_SURFACE_TYPE	EGL_WINDOW_BIT,
	EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES_BIT,
	EGL_NON
}
#endif
#endif


typedef struct {
	SDL_Surface		*screen;
	TPW_SOUND_SETTINGS	sound;
	TPW_COMMON		common;
	#ifdef HAVE_GLES
	Display			*XDisplay;
	eglConfig		eglConfig;
	EGLCOntext		eglContext;
	EGLSurface		eglSurface;
	EGLDisplay		eglDisplay;
	#endif
} TPW;



#endif
