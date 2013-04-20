/*
Copyright (c) 2013 Steven Arnow
'sound_sdl.c' - This file is part of libdarnit_tpw

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

#include "../main.h"


int tpw_sound_open(TPW_SOUND_SETTINGS settings) {
	SDL_AudioSpec fmt;

	tpw.sound = settings;

	fmt.freq = settings.sample_rate;
	switch (settings.format) {
		case TPW_SAMPLE_FORMAT_S16LE:
			fmt.format = AUDIO_S16;
			break;
		case TPW_SAMPLE_FORMAT_S8:
			fmt.format = AUDIO_S8;
			break;
	}
	fmt.channels = settings.channels;
	fmt.samples = settings.samples;
	fmt.callback = (void *) settings.callback;
	fmt.userdata = settings.userdata;

	return SDL_OpenAudio(&fmt, NULL);
}


void tpw_sound_control(TPW_SOUND_COMMAND command) {
	switch(command) {
		case TPW_SOUND_COMMAND_PAUSE:
			SDL_PauseAudio(1);
			break;
		case TPW_SOUND_COMMAND_PLAY:
			SDL_PauseAudio(0);
			break;
	}
	
	return;
}
