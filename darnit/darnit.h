#ifndef __DARNIT_H__
#define __DARNIT_H__


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
#include <errno.h>


#ifdef _WIN32
	#include <windows.h>
	#include <winsock2.h>
#else
	#include <dlfcn.h>
	#include <unistd.h>
	#include <netdb.h>
	#include <sys/types.h>
	#include <fcntl.h>
#endif

#define					DARNIT_UNUSED(x) (void)x

#define         DARNIT_PLATFORM_DESKTOP         0x100000
#define         DARNIT_PLATFORM_HANDHELD        0x200000

#define         DARNIT_PLATFORM_LINUX           0x1
#define         DARNIT_PLATFORM_WIN32           0x2
#define         DARNIT_PLATFORM_PANDORA         0x4
#define         DARNIT_PLATFORM_GCWZERO         0x8
#define         DARNIT_PLATFORM_GPHCAANOO       0x10
#define		DARNIT_PLATFORM_BIGENDIAN	0x80000000


#ifdef _WIN32
	#define EXPORT_THIS __declspec(dllexport)
	#define	off_t				long long int
#else
	#define	EXPORT_THIS __attribute__ ((visibility ("default")))
#endif


typedef union {
	unsigned int	i;
	unsigned char	c[4];
} DARNIT_ENDIAN_CONVERT;


typedef struct {
	unsigned int	x	: 16;
	unsigned int	y	: 16;
	signed int	wheel	: 32;
	unsigned int	mb_l	: 1;
	unsigned int	mb_r	: 1;
} DARNIT_MOUSE;


typedef struct {
	unsigned int		platform;
	unsigned int		screen_w;
	unsigned int		screen_h;
	unsigned int		fullscreen;
} DARNIT_PLATFORM;


#include "../deps/stb_truetype.h"
#include "filesystem.h"
#include "bitwise.h"
#include "render.h"
#include "input.h"
#include "error.h"
#include "text.h"
#include "menutk.h"
#include "audio.h"
#include "imgload.h"
#include "sprite.h"
#include "bbox.h"
#include "render_tilemap.h"
#include "tilemap.h"
#include "dynlib.h"
#include "socket.h"
#include "mt_sprite.h"
#include "audio_load.h"
#include "utf8.h"
#include "util.h"
#include "stringtable.h"
#include "ldmz.h"

int darnitTimeLastFrameTook();


typedef struct {
	TILE_CACHE		*tc;
	unsigned int		tiles;
} DARNIT_RENDER_BUFFER;


typedef struct {
	LINE_CACHE		*lc;
	unsigned int		lines;
	unsigned int		line_w;
} DARNIT_RENDER_LINE_BUFFER;


typedef struct {
	RECT_CACHE		*rc;
	unsigned int		rects;
} DARNIT_RENDER_RECT_BUFFER;


typedef struct {
	int			frames_counting;
	int			frames_last;
	int			time;
	int			time_at_flip;
	int			time_at_last_frame;
} DARNIT_FPS;


typedef struct {
	VIDEO			video;
	INPUT_STRUCT		input;
	AUDIO			audio;
	FILESYSTEM		fs;
	DARNIT_FPS		fps;
	TILESHEET_REF_CONT	tsr;
	DARNIT_PLATFORM		platform;
} DARNIT;


/* It has finally happened. I made that annoying handle global. I know <.< */
DARNIT *d;


#endif
