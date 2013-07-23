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

#include "../main.h"

#define BUFFERS 8

static const unsigned int sample_format[] = {
	16,	/* TPW_SAMPLE_FORMAT_S16LE */
	8,	/* TPW_SAMPLE_FORMAT_S8 */
};

static struct CALLBACK_DATA {
	void (*fetch_audio)(void *data, void *mixdata, int bytes);
	void *userdata;
} callback_data;

static struct {
	HWAVEOUT handle;
	WAVEFORMATEX format;
	WAVEHDR header[BUFFERS];
	unsigned int current;
} sound;

void tpw_sound_callback(void (*callback)(void *data, void *mixdata, int bytes)) {
	callback_data.fetch_audio = callback;
	
	return;
}
	
static void CALLBACK callback(HWAVEOUT hwo, UINT msg, DWORD_PTR instance, DWORD_PTR param1, DWORD_PTR param2) {
	if(msg!=WOM_DONE)
		return;
	callback_data.fetch_audio(callback_data.userdata, sound.header[sound.current].lpData, sound.header[sound.current].dwBufferLength);
	waveOutWrite(hwo, &(sound.header[sound.current]), sizeof(WAVEHDR));
	
	sound.current=(sound.current+1)%BUFFERS;
}

	
int tpw_sound_open(TPW_SOUND_SETTINGS settings) {
	int res;
	unsigned int buflen;
	int i;
	sound.format.wFormatTag = WAVE_FORMAT_PCM;
	sound.format.nChannels = settings.channels;
	sound.format.nSamplesPerSec = settings.sample_rate;
	sound.format.nAvgBytesPerSec = settings.sample_rate*(sample_format[settings.format]/8)*settings.channels;
	sound.format.nBlockAlign = (sample_format[settings.format]/8)*settings.channels;
	sound.format.wBitsPerSample = sample_format[settings.format];
	sound.format.cbSize = 0;
	
	callback_data.fetch_audio=settings.callback;
	callback_data.userdata=settings.userdata;
	
	buflen=(sample_format[settings.format]/8)*settings.samples;
	
	for(i=0; i<BUFFERS; i++) {
		sound.header[i].dwBufferLength=buflen;
		sound.header[i].lpData=(LPSTR)malloc(buflen);
	}
	
	if((res=waveOutOpen(&sound.handle, WAVE_MAPPER, &sound.format, (DWORD) callback, (DWORD) callback, CALLBACK_FUNCTION))==MMSYSERR_NOERROR) {
//		fprintf(stderr, "arne\n");
		for(i=0; i<BUFFERS; i++)
			waveOutPrepareHeader(sound.handle, &(sound.header[i]), sizeof(WAVEHDR));
		waveOutPause(sound.handle);
		return 0;
	}
//	fprintf(stderr, "res %i\n", res);
	return -1;
}


void tpw_sound_control(TPW_SOUND_COMMAND command) {
	int i;
	switch(command) {
		case TPW_SOUND_COMMAND_PLAY:
			waveOutRestart(sound.handle);
			for(i=0; i<BUFFERS; i++)
				callback(sound.handle, WOM_DONE, 0, 0, 0);
			break;
		case TPW_SOUND_COMMAND_PAUSE:
			waveOutPause(sound.handle);
			break;
	}
}
