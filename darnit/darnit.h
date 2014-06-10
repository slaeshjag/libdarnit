/*
Copyright (c) 2011-2013 Steven Arnow
'darnit.h' - This file is part of libdarnit

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/


#ifndef __DARNIT_H__
#define __DARNIT_H__


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>


#ifdef _WIN32
	#define TARGET_IS_RETARDED
	#include <winsock2.h>
	#include <windows.h>
#else
	#include <dlfcn.h>
	#include <unistd.h>
	#include <netdb.h>
	#include <sys/types.h>
	#include <fcntl.h>
#endif

#include "platform/tpw.h"

#define		DARNIT_UNUSED(x) (void)x

#define		DARNIT_PLATFORM_DESKTOP		0x100000
#define		DARNIT_PLATFORM_HANDHELD	0x200000

#define		DARNIT_PLATFORM_LINUX		0x1
#define		DARNIT_PLATFORM_WIN32		0x2
#define		DARNIT_PLATFORM_PANDORA		0x4
#define		DARNIT_PLATFORM_GCWZERO		0x8
#define		DARNIT_PLATFORM_GPHCAANOO	0x10
#define		DARNIT_PLATFORM_MACOSX		0x20
#define		DARNIT_PLATFORM_FREEBSD		0x40
#define		DARNIT_PLATFORM_MAEMO		0x80
#define		DARNIT_PLATFORM_DBPYRA		0x100
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



#include "../deps/stb_truetype.h"
#include "filesystem.h"
#include "bitwise.h"
#include "tilesheet.h"
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
#include "particle.h"
#include "mt_sprite.h"
#include "audio_load.h"
#include "utf8.h"
#include "util.h"
#include "stringtable.h"
#include "ldmz.h"
#include "isometric.h"
#include "collision.h"
#include "compression.h"
#include "tbbox.h"

int darnitTimeLastFrameTook();


typedef struct {
	unsigned int		platform;
	unsigned int		screen_w;
	unsigned int		screen_h;
	unsigned int		fullscreen;
} DARNIT_PLATFORM;


typedef struct {
	TILE_CACHE		*tc;
	unsigned int		tiles;
	TILESHEET		*ts;
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
	POINT_CACHE		*pc;
	unsigned int		points;
	unsigned int		point_w;
} DARNIT_RENDER_POINT_BUFFER;


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
	SOCKET_LIST		*connect_list;
	UTIL			util;
	RENDER_PARAMS		render_params;
} DARNIT;


/* It has finally happened. I made that annoying handle global. I know <.< */
extern DARNIT *darnit_main_struct_that_nothing_should_use;
#define	d	darnit_main_struct_that_nothing_should_use


#endif
