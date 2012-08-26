#ifndef __DARNIT_H__
#define __DARNIT_H__


#include <stdio.h>
#include <stdlib.h>
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


#ifdef _WIN32
	#define EXPORT_THIS __declspec(dllexport)
#else
	#define	EXPORT_THIS __attribute__ ((visibility ("default")))
#endif


typedef struct {
	unsigned int	x	: 16;
	unsigned int	y	: 16;
	signed int	wheel	: 32;
} DARNIT_MOUSE;


typedef struct {
	unsigned int		platform;
	unsigned int		screen_w;
	unsigned int		screen_h;
	unsigned int		fullscreen;
} DARNIT_PLATFORM;


#include "../deps/stb_truetype.h"
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

int darnitTimeLastFrameTook(void *handle);


typedef struct {
	TILE_CACHE		*tc;
	unsigned int		tiles;
} DARNIT_RENDER_BUFFER;


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
	DARNIT_FPS		fps;
	TILESHEET_REF_CONT	tsr;
	DARNIT_PLATFORM		platform;
} DARNIT;


#endif
