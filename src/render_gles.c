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
/*	glEnable(GL_BLEND); */

	return 0;
}


int videoInit(void *handle, const char *wtitle, int screenw, int screenh, int fullscreen) {
	DARNIT *m = handle;
	EGLint configs_avail = 0;
	SDL_SysWMinfo sysinfo;
	unsigned int mode;

	mode = SDL_SWSURFACE;
	if (fullscreen) mode |= SDL_FULLSCREEN;
	
	/* here goes SDL init code */
	SDL_Init(SDL_INIT_EVERYTHING);
	
	if (!(m->video.XDisplay = XOpenDisplay(NULL))) {
		fprintf(stderr, "videoInit(): Fatal error: Unable to get a display handle from X\n");
		return -1;
	}
	
	if (!(m->video.eglDisplay = eglGetDisplay((EGLNativeDisplayType) m->video.XDisplay))) {
		fprintf(stderr, "videoInit(): Fatal error: Unable to get a display handle from EGL\n");
		return -1;
	}
	
	if (!eglInitialize(m->video.eglDisplay, NULL, NULL)) {
		fprintf(stderr, "videoInit(): Fatal error: Unable to initialize EGL\n");
		return -1;
	}
	
	if ((m->video.screen = SDL_SetVideoMode(screenw, screenh, 16, mode)) == NULL) {
		fprintf(stderr, "videoInit(): Fatal error: Unable to set up a window for SDL\n");
		return -1;
	}
	
	if (eglChooseConfig(m->video.eglDisplay, egl_config_attrib, &m->video.eglConfig, 1, &configs_avail) != EGL_TRUE)  {
		fprintf(stderr, "videoInit(): Fatal error: Unable to find a config for EGL (%i)\n", configs_avail);
		return -1;
	}
	
	SDL_VERSION(&sysinfo.version);
	if (SDL_GetWMInfo(&sysinfo) <= 0) {
		fprintf(stderr, "videoInit(): Fatal error: Unable to get window handle\n");
		return -1;
	}
	
	if ((m->video.eglSurface = eglCreateWindowSurface(m->video.eglDisplay, m->video.eglConfig, (EGLNativeWindowType) sysinfo.info.x11.window, 0)) == EGL_NO_SURFACE) {
		fprintf(stderr, "videoInit(): Fatal error: Unable to create a EGL surface\n");
		return -1;
	}
	
	eglBindAPI(EGL_OPENGL_ES_API);
	if ((m->video.eglContext = eglCreateContext(m->video.eglDisplay, m->video.eglConfig, NULL, NULL)) == EGL_NO_CONTEXT) {
		fprintf(stderr, "videoInit(): Fatal error: Unable to create a EGL context\n");
		return -1;
	}
	
	if (eglMakeCurrent(m->video.eglDisplay, m->video.eglSurface, m->video.eglSurface, m->video.eglContext) == EGL_FALSE) {
		fprintf(stderr, "videoInit(): Fatal error: Unable to make the EGL context current\n");
		return -1;
	}
	
	m->video.swgran = 2.0f/screenw;
	m->video.shgran = 2.0f/screenh;

	m->video.w = screenw;
	m->video.h = screenh;

	m->video.camx = 0;
	m->video.camy = 0;

	m->platform.fullscreen = fullscreen;

	m->video.offset_x = m->video.offset_y = 0;

	SDL_WM_SetCaption(wtitle, NULL);
	videoInitGL(screenw, screenh);

	m->video.tint_r = m->video.tint_g = m->video.tint_b = m->video.tint_a = 1.0f;

	#ifdef PANDORA
	if ((m->video.fbdev = open("/dev/fb0", O_RDONLY)) < 0)
		fprintf(stderr, "WARNING: Unable to open fbdev for Vsync\n");
	#else
		m->video.fbdev = -1;
	#endif

	return 0;
}



void videoSwapBuffers(void *handle) {
	DARNIT *m = handle;
	int n;
	
	if (m->video.fbdev >= 0) {
		n = 0;
		ioctl(m->video.fbdev, FBIO_WAITFORVSYNC, &n);
	}

	eglSwapBuffers(m->video.eglDisplay, m->video.eglSurface);

	return;
}


void videoClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	return;
}


int videoLoop(void *handle) {
	DARNIT *m = handle;

	videoSwapBuffers(m);
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

void videoRemoveTexture(unsigned int texture) {
	glDeleteTextures(1, &texture);
	
	return;
}


void videoDestroy(void *handle) {
	DARNIT *m = handle;

	if (m->video.eglSurface || m->video.eglContext || m->video.eglDisplay)
		eglMakeCurrent(m->video.eglDisplay, NULL, NULL, EGL_NO_CONTEXT);
		eglDestroyContext(m->video.eglDisplay, g_eglContext);
		eglDestroySurface(m->video.eglDisplay, g_eglSurface);
		eglTerminate(m->video.eglDisplay);
	}

	return;
}
