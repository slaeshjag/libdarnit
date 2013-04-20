/*
Copyright (c) 2011-2013 Steven Arnow
'render_gles.h' - This file is part of libdarnit

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


#ifndef __VIDEO_GLES_H__
#define	__VIDEO_GLES_H__

/* DO NOT INCLUDE THIS FILE MANUALLY, THAT IS HANDLED BY render.h */


#define COLOURDEPTH_RED_SIZE  		5
#define COLOURDEPTH_GREEN_SIZE 		6
#define COLOURDEPTH_BLUE_SIZE 		5
#define COLOURDEPTH_DEPTH_SIZE		16


#ifdef PANDORA
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#endif


#ifdef PANDORA
	#define FBIO_WAITFORVSYNC _IOW('F', 0x20, __u32)
#else
	#define FBIO_WAITFORVSYNC 0
#endif


static const EGLint egl_config_attrib[] = {
	#ifndef MAEMO
	EGL_RED_SIZE,			COLOURDEPTH_RED_SIZE,
	EGL_GREEN_SIZE,			COLOURDEPTH_GREEN_SIZE,
	EGL_BLUE_SIZE,			COLOURDEPTH_BLUE_SIZE,
	EGL_DEPTH_SIZE,			COLOURDEPTH_DEPTH_SIZE,
	#endif
	EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
	EGL_RENDERABLE_TYPE,		EGL_OPENGL_ES_BIT,
//	EGL_BIND_TO_TEXTURE_RGBA,	EGL_TRUE,
	EGL_NONE
};


typedef struct {
	FADE_STRUCT			fade;
	float				swgran;
	float				shgran;
	unsigned int			w;
	unsigned int			h;
	unsigned int			camx;
	unsigned int			camy;
	int				offset_x;
	int				offset_y;
	float				tint_r;
	float				tint_g;
	float				tint_b;
	float				tint_a;
	int				blend;
	int				fbdev;
} VIDEO;


int videoInit(const char *wtitle, int screenw, int screenh, int fullscreen);
int videoLoop();
void videoSwapBuffers();
void videoClearScreen();
unsigned int videoAddTexture(void *data, unsigned int w, unsigned int h);
unsigned int videoAddTextureA8(void *data, unsigned int w, unsigned int h);
unsigned int videoAddTextureRGBA4(void *data, unsigned int w, unsigned int h);
unsigned int videoAddTextureRGB5A1(void *data, unsigned int w, unsigned int h);
void videoRemoveTexture(unsigned int texture);
void videoDestroy();


#endif
