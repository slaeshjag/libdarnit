#ifndef __DARNIT_H__
#define __DARNIT_H__


#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif


typedef struct {
	unsigned int	x	: 16;
	unsigned int	y	: 16;
	signed int	wheel	: 32;
} DARNIT_MOUSE;


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


typedef struct {
	TILE_CACHE		*tc;
	unsigned int		tiles;
} DARNIT_RENDER_BUFFER;


typedef struct {
	VIDEO		video;
	INPUT		input;
	AUDIO		audio;
} DARNIT;


#endif
