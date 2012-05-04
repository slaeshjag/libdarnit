#ifndef		__VIDEO_OGL_H__
#define		__VIDEO_OGL_H__


#define COLOURDEPTH_RED_SIZE  		5
#define COLOURDEPTH_GREEN_SIZE 		6
#define COLOURDEPTH_BLUE_SIZE 		5
#define COLOURDEPTH_DEPTH_SIZE		16



#include	<SDL/SDL.h>
#include	<SDL/SDL_opengl.h>


typedef struct {
	SDL_Surface			*screen;
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
} VIDEO;

int videoInit(void *handle, const char *wtitle, int screenw, int screenh, int fullscreen);
int videoLoop(void *handle);
void videoSwapBuffers(void *handle);
void videoClearScreen();
unsigned int videoAddTexture(void *data, unsigned int w, unsigned int h);
void videoRemoveTexture(unsigned int texture);

#endif
