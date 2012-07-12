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

void *darnitInit(const char *wtitle);
void *darnitInitCustom(const char *wtitle, int win_w, int win_h, int fullscreen);
void darnitLoop(void *handle);
unsigned int darnitTimeGet();
int darnitFPSGet(void *handle);
int darnitTimeLastFrameTook(void *handle);
DARNIT_PLATFORM darnitPlatformGet(void *handle);

#endif
