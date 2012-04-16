#include "darnit.h"


int videoInitGL(int w, int h) {
	glEnable(GL_TEXTURE_2D);
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_BLEND);

	return 0;
}


int videoInit(void *handle, const char *wtitle, int screenw, int screenh, int fullscreen) {
	DARNIT *m = handle;
	unsigned int mode;

	mode = SDL_OPENGL;
	if (fullscreen) mode |= SDL_FULLSCREEN;

	
	/* here goes SDL init code */
	SDL_Init(SDL_INIT_EVERYTHING);
	
	if ((m->video.screen = SDL_SetVideoMode(screenw, screenh, 16, mode)) == NULL) {
		fprintf(stderr, "videoInit(): Fatal error: Unable to set up a window for SDL\n");
		return -1;
	}
	
	m->video.swgran = 2.0f/screenw;
	m->video.shgran = 2.0f/screenh;

	m->video.w = screenw;
	m->video.h = screenh;

	m->video.camx = 0;
	m->video.camy = 0;
	m->video.offset_x = m->video.offset_y = 0;
	m->video.time = SDL_GetTicks();
	
	SDL_WM_SetCaption(wtitle, NULL);
	videoInitGL(screenw, screenh);
	
	return 0;
}



int videoLoop(void *handle) {
	DARNIT *m = handle;
	int delay;

	videoSwapBuffers(m);
	videoClearScreen();

	delay = 20 - (SDL_GetTicks() - m->video.time);

	if (delay < 0)
		delay = 0;

	SDL_Delay(delay);
	m->video.time = SDL_GetTicks();

	
	return 0;
}


void videoSwapBuffers(void *handle) {
	
	SDL_GL_SwapBuffers();
	
	return;
}


void videoClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	return;
}


unsigned int videoAddTexture(void *data, unsigned int w, unsigned int h) {
	unsigned int texhandle;

	glGenTextures(1, &texhandle);
	glBindTexture(GL_TEXTURE_2D,texhandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texhandle;
}


void videoRemoveTexture(unsigned int texture) {
	glDeleteTextures(1, &texture);
	
	return;
}
