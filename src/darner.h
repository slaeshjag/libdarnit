/* libdarner named after the Southern Giant Darner, "fastest insect in the world" */
#ifndef __DARNER_H__
#define __DARNER_H__


#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>


typedef struct {
	unsigned int	x	: 16;
	unsigned int	y	: 16;
	signed int	wheel	: 32;
} DARNER_MOUSE;


#include "render.h"
#include "input.h"
#include "error.h"
#include "text.h"
#include "menutk.h"
#include "audio.h"
#include "imgload.h"
#include "sprite.h"


typedef struct {
	VIDEO		video;
	INPUT		input;
	AUDIO		audio;
	SPRITE		sprite;
} DARNER;


#endif
