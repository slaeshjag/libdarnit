/*
Copyright (c) 2013 Axel Isaksson
'sound_win32.c' - This file is part of libdarnit_tpw

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

#include <windows.h>
#include <mmsystem.h>

#define TPW_INTERNAL
#include "../main.h"

static const unsigned int[] sample_format = {
	[TPW_SAMPLE_FORMAT_S16LE] = 16,
	[TPW_SAMPLE_FORMAT_S8] = 8,
};

static struct CALLBACK_DATA {
	void (*fetch_audio)(void *data, void *mixdata, int bytes);
	void *userdata;
} callback_data;

static struct {
	HWAVEOUT handle;
	sound.formatEX format;
	WAVEHDR header[2];
	unsigned int current;
} sound;

static void callback(HWAVEOUT hwo, UINT msg, DWORD_PTR instance, DWORD_PTR param1, DWORD_PTR param2) {
	if(msg!=WOM_DONE)
		return;
	callback_data.fetch_audio(callback_data.userdata, sound.header[sound.current].lpData, sound.header[sound.current].dwBufferLength);
	waveOutWrite(handle, &(sound.header[sound.current]), sizeof(WAVEHDR));
	
	sound.current=!sound.current;
}

int tpw_sound_open(TPW_SOUND_SETTINGS settings) {
	int res;
	sound.format.wFormatTag = WAVE_FORMAT_PCM;
	sound.format.nChannels = settings.channels;
	sound.format.nSamplesPerSec = settings.sample_rate;
	sound.format.nAvgBytesPerSec = settings.sample_rate*(sample_format[settings.format]/8)*settings.channels;
	sound.format.nBlockAlign = (sample_format[settings.format]/8)*settings.channels;
	sound.format.wBitsPerSample = sample_format[settings.format];
	sound.format.cbSize = 0;
	
	callback_data.fetch_audio=settings.callback;
	callback_data.userdata=settings.userdata;
	callback_data.samples=settings.samples;
	
	sound.header[0].dwBufferLength=sound.header[1].dwBufferLength=(sample_format[settings.format]/8)*settings.samples
	sound.header[0].lpData=malloc(sound.header[0].dwBufferLength);
	sound.header[1].lpData=malloc(sound.header[1].dwBufferLength);
	
	if(waveOutOpen(&sound.handle, WAVE_MAPPER, &sound.format, (DWORD_PTR) callback, 0, CALLBACK_FUNCTION)==MMSYSERR_NOERROR) {
		waveOutPrepareHeader(sound.handle, &(sound.handle[0]), sizeof(WAVEHDR));
		waveOutPrepareHeader(sound.handle, &(sound.handle[1]), sizeof(WAVEHDR));
		waveOutPause(sound.handle);
		return 0;
	}
	return -1;
}


void tpw_sound_control(TPW_SOUND_COMMAND command) {
	switch(command) {
		case TPW_SOUND_COMMAND_PLAY:
			waveOutRestart(sound.handle);
			callback(sound.handle, WOM_DONE, 0, 0, 0);
			break;
		case TPW_SOUND_COMMAND_PAUSE:
			waveOutPause(sound.handle);
			break;
	}
}
