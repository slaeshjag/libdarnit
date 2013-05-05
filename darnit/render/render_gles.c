/*
Copyright (c) 2011-2013 Steven Arnow
'render_gles.c' - This file is part of libdarnit

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


/* DO NOT FEED THIS FILE TO YOUR COMPILER MANUALLY, THAT IS HANDLED BY render.c */

#include <sys/ioctl.h>

int videoInitGL(int w, int h) {
	glEnable(GL_TEXTURE_2D);
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
/*	glEnable(GL_BLEND); */

	return 0;
}


int videoInit(const char *wtitle, int screenw, int screenh, int fullscreen) {
	if (!tpw_window_create(wtitle, screenw, screenh, fullscreen, 16))
		return -1;
	
	d->video.swgran = 2.0f/screenw;
	d->video.shgran = 2.0f/screenh;

	d->video.w = screenw;
	d->video.h = screenh;

	d->video.camx = 0;
	d->video.camy = 0;

	d->platform.fullscreen = fullscreen;

	d->video.offset_x = d->video.offset_y = 0;

	videoInitGL(screenw, screenh);

	d->video.tint_r = d->video.tint_g = d->video.tint_b = d->video.tint_a = 1.0f;

	#ifdef PANDORA
	if ((d->video.fbdev = open("/dev/fb0", O_RDONLY)) < 0)
		fprintf(stderr, "WARNING: Unable to open fbdev for Vsync\n");
	#else
		d->video.fbdev = -1;
	#endif
	
	#ifdef MAEMO
		tpw_input_grab(TPW_INPUT_GRAB_GRAB);
	#endif

	return 0;
}



void videoSwapBuffers() {
	int n;

	tpw_render_buffer_swap();
	
	if (d->video.fbdev >= 0) {
		n = 0;
		ioctl(d->video.fbdev, FBIO_WAITFORVSYNC, &n);
	}

	return;
}


void videoClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	return;
}


int videoLoop() {
	videoSwapBuffers(d);
	videoClearScreen();

	
	return 0;
}


unsigned int videoAddTexture(void *data, unsigned int w, unsigned int h) {
	unsigned int texhandle;

	glGenTextures(1, &texhandle);
	glBindTexture(GL_TEXTURE_2D,texhandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texhandle;
}


unsigned int videoAddTextureA8(void *data, unsigned int w, unsigned int h) {
	unsigned int texhandle;

	glGenTextures(1, &texhandle);
	glBindTexture(GL_TEXTURE_2D,texhandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);

	return texhandle;
}


unsigned int videoAddTextureRGBA4(void *data, unsigned int w, unsigned int h) {
	unsigned int texhandle;

	glGenTextures(1, &texhandle);
	glBindTexture(GL_TEXTURE_2D,texhandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);

	return texhandle;
}


unsigned int videoAddTextureRGB5A1(void *data, unsigned int w, unsigned int h) {
	unsigned int texhandle;

	glGenTextures(1, &texhandle);
	glBindTexture(GL_TEXTURE_2D,texhandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);

	return texhandle;
}

void videoRemoveTexture(unsigned int texture) {
	glDeleteTextures(1, &texture);
	
	return;
}


void videoDestroy() {
	#ifdef PANDORA
	close(d->video.fbdev);
	#endif

	return;
}
