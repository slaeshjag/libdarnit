#ifndef __VIDEO_GLES_H__
#define	__VIDEO_GLES_H__


#define COLOURDEPTH_RED_SIZE  		5
#define COLOURDEPTH_GREEN_SIZE 		6
#define COLOURDEPTH_BLUE_SIZE 		5
#define COLOURDEPTH_DEPTH_SIZE		16


#ifdef PANDORA
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#endif


#include	<X11/Xutil.h>
#ifndef RASPBERRYPI
#include	<GLES/egl.h>
#endif
#include	<EGL/egl.h>
#include	<GLES/gl.h>
#include	<GLES/glext.h>
#include	<SDL/SDL.h>
#include	<SDL/SDL_syswm.h>

#ifdef PANDORA
	#define FBIO_WAITFORVSYNC _IOW('F', 0x20, __u32)
#else
	#define FBIO_WAITFORVSYNC 0
#endif


static const EGLint egl_config_attrib[] = {
	EGL_RED_SIZE,			COLOURDEPTH_RED_SIZE,
	EGL_GREEN_SIZE,			COLOURDEPTH_GREEN_SIZE,
	EGL_BLUE_SIZE,			COLOURDEPTH_BLUE_SIZE,
	EGL_DEPTH_SIZE,			COLOURDEPTH_DEPTH_SIZE,
	EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
	EGL_RENDERABLE_TYPE,		EGL_OPENGL_ES_BIT,
//	EGL_BIND_TO_TEXTURE_RGBA,	EGL_TRUE,
	EGL_NONE
};


typedef struct {
	Display				*XDisplay;
	EGLConfig			eglConfig;
	EGLContext			eglContext;
	EGLSurface			eglSurface;
	EGLDisplay			eglDisplay;
	SDL_Surface			*screen;
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


int videoInit(void *handle, const char *wtitle, int screenw, int screenh, int fullscreen);
int videoLoop();
void videoSwapBuffers(void *handle);
void videoClearScreen();
unsigned int videoAddTexture(void *data, unsigned int w, unsigned int h);
unsigned int videoAddTextureRGBA4(void *data, unsigned int w, unsigned int h);
unsigned int videoAddTextureRGB5A1(void *data, unsigned int w, unsigned int h);
void videoRemoveTexture(unsigned int texture);


#endif
