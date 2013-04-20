/*
Copyright (c) 2013 Steven Arnow
'sound_dummy.h' - This file is part of libdarnit_tpw

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

#ifndef __TPW_SOUND_H__
#define	__TPW_SOUND_H__

#include "platform_sound.h"

typedef enum {
	TPW_SAMPLE_FORMAT_S16LE,
	TPW_SAMPLE_FORMAT_S8
} TPW_SAMPLE_FORMAT;

typedef enum {
	TPW_SOUND_COMMAND_PLAY,
	TPW_SOUND_COMMAND_PAUSE,
} TPW_SOUND_COMMAND;

typedef struct {
	unsigned int		sample_rate;
	TPW_SAMPLE_FORMAT	format;
	unsigned int		channels;
	unsigned int		samples;
	void			(*callback)(void *data, void *mixdata, int bytes);
	void			*userdata;
} TPW_SOUND_SETTINGS;

int tpw_sound_open(TPW_SOUND_SETTINGS settings);
void tpw_sound_control(TPW_SOUND_COMMAND command);

#endif
