#include "darnit.h"

/* DO NOT FEED THIS FILE TO YOUR COMPILER MANUALLY, THAT IS HANDLED BY render.c */


int videoInitGL(int w, int h) {
	glEnable(GL_TEXTURE_2D);
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}


int videoInit(const char *wtitle, int screenw, int screenh, int fullscreen) {
	unsigned int mode;

	mode = SDL_OPENGL;
	if (fullscreen) mode |= SDL_FULLSCREEN;

	
	/* here goes SDL init code */
	SDL_Init(SDL_INIT_EVERYTHING);
	
	if ((d->video.screen = SDL_SetVideoMode(screenw, screenh, 16, mode)) == NULL) {
		fprintf(stderr, "videoInit(): Fatal error: Unable to set up a window for SDL\n");
		return -1;
	}
	
	d->video.swgran = 2.0f/screenw;
	d->video.shgran = 2.0f/screenh;

	d->video.w = screenw;
	d->video.h = screenh;

	d->video.camx = 0;
	d->video.camy = 0;
	d->video.offset_x = d->video.offset_y = 0;
	d->video.time = SDL_GetTicks();
	
	SDL_WM_SetCaption(wtitle, NULL);
	videoInitGL(screenw, screenh);

	d->platform.fullscreen = fullscreen;
	d->video.tint_r = d->video.tint_g = d->video.tint_b = d->video.tint_a = 1.0f;
	
	return 0;
}



int videoLoop() {
	int delay;

	videoSwapBuffers();
	videoClearScreen();

	delay = 16 - (SDL_GetTicks() - d->video.time);

	if (delay < 0)
		delay = 0;

	SDL_Delay(delay);
	d->video.time = SDL_GetTicks();

	
	return 0;
}


void videoSwapBuffers() {
	
	SDL_GL_SwapBuffers();
	
	return;
}


void videoClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	return;
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);

	return texhandle;
}


unsigned int videoAddTextureRGB5A1(void *data, unsigned int w, unsigned int h) {
	unsigned int texhandle;

	glGenTextures(1, &texhandle);
	glBindTexture(GL_TEXTURE_2D,texhandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);

	return texhandle;
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


void videoRemoveTexture(unsigned int texture) {
	glDeleteTextures(1, &texture);
	
	return;
}


void videoDestroy() {
  // Something to remember me by!
  // WHY HAX?!?!?
/*  DARNIT_UNUSED(handle);*/
	return;
}
