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
	[TPW_SAMPLE_FORMAT_S16LE] = 16;
	[TPW_SAMPLE_FORMAT_S8] = 8;
};

static WAVEFORMATEX waveformat;
static HWAVEOUT handle;

static void callback(HWAVEOUT hwo, UINT msg, DWORD_PTR instance, DWORD_PTR param1, DWORD_PTR param2) {
	
}

int tpw_sound_open(TPW_SOUND_SETTINGS settings) {
	int res;
	
	waveformat.wFormatTag = WAVE_FORMAT_PCM;
	waveformat.nChannels = settings.channels;
	waveformat.nSamplesPerSec = settings.sample_rate;
	waveformat.nAvgBytesPerSec = settings.sample_rate*(sample_format[settings.format]/8)*setting.channels;
	waveformat.nBlockAlign = (sample_format[settings.format]/8)*setting.channels;
	waveformat.wBitsPerSample = sample_format[settings.format];
	waveformat.cbSize = 0;
	if(waveOutOpen(&handle, WAVE_MAPPER, &waveformat, (DWORD_PTR) callback, NULL, CALLBACK_FUNCTION)==MMSYSERR_NOERROR) {
		waveOutPause(handle);
		return 0;
	}
	return -1;
}


void tpw_sound_control(TPW_SOUND_COMMAND command) {
	if(command==TPW_SOUND_COMMAND_PLAY)
		waveOutRestart(handle);
	else if(command==TPW_SOUND_COMMAND_PAUSE)
		waveOutPause(handle);
}
