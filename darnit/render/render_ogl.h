/*
Copyright (c) 2011-2013 Steven Arnow
'render_ogl.h' - This file is part of libdarnit

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


#ifndef		__VIDEO_OGL_H__
#define		__VIDEO_OGL_H__

#include <GL/gl.h>


/* DO NOT INCLUDE THIS FILE MANUALLY, THAT IS HANDLED BY render.h */

#define COLOURDEPTH_RED_SIZE  		5
#define COLOURDEPTH_GREEN_SIZE 		6
#define COLOURDEPTH_BLUE_SIZE 		5
#define COLOURDEPTH_DEPTH_SIZE		16



#include	<SDL/SDL.h>
#include	<SDL/SDL_opengl.h>

typedef struct {
	FADE_STRUCT			fade;
	float				swgran;
	float				shgran;
	unsigned int			w;
	unsigned int			h;
	unsigned int			camx;
	unsigned int			camy;
	unsigned int			time;
	int				offset_x;
	int				offset_y;
	float				tint_r;
	float				tint_g;
	float				tint_b;
	float				tint_a;
	int				blend;
	int				fullscreen;
} VIDEO;

unsigned int videoAddTextureRGBA4(void *data, unsigned int w, unsigned int h);
unsigned int videoAddTextureRGB5A1(void *data, unsigned int w, unsigned int h);
unsigned int videoAddTextureA8(void *data, unsigned int w, unsigned int h);
int videoInit(const char *wtitle, int screenw, int screenh, int fullscreen);
int videoLoop();
void videoSwapBuffers();
void videoClearScreen();
unsigned int videoAddTexture(void *data, unsigned int w, unsigned int h);
void videoRemoveTexture(unsigned int texture);
void videoDestroy();

#endif
