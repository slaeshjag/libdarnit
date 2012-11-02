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

void *darnitInit(const char *wtitle, const char *data_dir);
void *darnitInitCustom(const char *wtitle, int win_w, int win_h, int fullscreen, const char *data_dir);
void darnitLoop();
unsigned int darnitTimeGet();
int darnitFPSGet();
int darnitTimeLastFrameTook();
DARNIT_PLATFORM darnitPlatformGet();
void darnitQuit();


/* Special init funcs, in case you need to parse a config file or something before you init video */
/* If you call initPartial, you must not call darnitInit nor darnitInitCustom. You must not call any rendering or timing function either, as these are undefined before video inits */
/* When you want to init video, call darnitInitRest() */

void *darnitInitPartial(const char *data_dir);
int darnitInitRest(const char *wtitle, int win_w, int win_h, int fullscreen);

#endif
