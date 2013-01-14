#ifndef __DARNIT_MAIN_H__
#define	__DARNIT_MAIN_H__

#define		DARNIT_PLATFORM_DESKTOP		0x100000
#define		DARNIT_PLATFORM_HANDHELD	0x200000

#define		DARNIT_PLATFORM_LINUX		0x1
#define		DARNIT_PLATFORM_WIN32		0x2
#define		DARNIT_PLATFORM_PANDORA		0x4
#define		DARNIT_PLATFORM_GCWZERO		0x8
#define		DARNIT_PLATFORM_GPHCAANOO	0x10


typedef struct {
	unsigned int		platform;
	unsigned int		screen_w;
	unsigned int		screen_h;
	unsigned int		fullscreen;
} DARNIT_PLATFORM;


typedef struct {
	signed short		x	: 16;
	signed  short		y	: 16;
	unsigned short		w	: 16;
	unsigned short		h	: 16;
} DARNIT_VIDEOMODE;


void *d_init(const char *wtitle, const char *data_dir, const char *icon);
void *d_init_custom(const char *wtitle, int win_w, int win_h, int fullscreen, const char *data_dir, const char *icon);
void d_loop();
unsigned int d_time_get();
int d_fps();
int d_last_frame_time();
DARNIT_PLATFORM d_platform_get();
void d_quit();


/* Special init funcs, in case you need to parse a config file or something before you init video */
/* If you call initPartial, you must not call d_init nor d_init_custom. You must not call any rendering or timing function either, as these are undefined before video inits */
/* When you want to init video, call d_init_rest() */

void *d_init_partial(const char *data_dir);
int d_init_rest(const char *wtitle, int win_w, int win_h, int fullscreen, const char *icon);


/* You need to call d_init_partial before calling this one */
DARNIT_VIDEOMODE **d_videomode_get();
#endif
