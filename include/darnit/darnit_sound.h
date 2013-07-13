/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_sound.h' - This file is part of libdarnit

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


#ifndef __DARNIT_AUDIO_H__
#define	__DARNIT_AUDIO_H__

typedef enum {
	DARNIT_AUDIO_MONO = 1,
	DARNIT_AUDIO_STEREO = 2,
} DARNIT_AUDIO_CHANNELS;

typedef enum {
	DARNIT_AUDIO_PRELOAD = 0,
	DARNIT_AUDIO_STREAM = 1,
} DARNIT_AUDIO_MODE;

#define			DARNIT_AUDIO_NOREPEAT		-1

typedef void DARNIT_SOUND;

DARNIT_SOUND *d_sound_tracked_load(const char *fname, DARNIT_AUDIO_MODE mode, DARNIT_AUDIO_CHANNELS channels);
DARNIT_SOUND *d_sound_streamed_load(const char *fname, DARNIT_AUDIO_MODE mode, DARNIT_AUDIO_CHANNELS channels);
DARNIT_SOUND *d_sound_callback_load(int (*callback)(signed short *buff, int buff_len, int pos, void *data), void *data, DARNIT_AUDIO_CHANNELS channels);
DARNIT_SOUND *d_sound_unload(DARNIT_SOUND *sound_resource);
void d_sound_stop_all();
void d_sound_stop(int playback_key);
int d_sound_play(DARNIT_SOUND *sound_resource, int loop, int vol_l, int vol_r, int jmpto);
int d_sound_playback_status(int playback_key);
void d_sound_playback_volume_set(unsigned int playback_key, int vol_l, int vol_r);
void d_sound_master_volume(int volume);
void d_sound_mix_callback_set(void (*callback)(void *ignore, void *mixdata, int bytes));

void d_sound_compression_enable();
void d_sound_compression_disable();

#endif
