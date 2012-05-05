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



#ifdef _WIN32
	#define EXPORT_THIS
#else
	#define	EXPORT_THIS __attribute__ ((visibility ("default")))
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
#include "socket.h"


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
