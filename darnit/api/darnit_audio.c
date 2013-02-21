/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_audio.c' - This file is part of libdarnit

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


#include "darnit.h"


void EXPORT_THIS *d_sound_tracked_load(const char *fname, int preload, int makemono) {
	return audioOpenTracked(fname, preload, makemono);
}


void EXPORT_THIS *d_sound_streamed_load(const char *fname, int preload, int makemono) {
	return audioOpenStreamed(fname, preload, makemono);
}


void EXPORT_THIS *d_sound_unload(void *sound_resource) {
	return audioUnload(sound_resource);
}


void EXPORT_THIS d_sound_stop_all() {
	audioSoundClear();

	return;
}


void EXPORT_THIS d_sound_stop(int playback_key) {
	audioSoundStop(playback_key);

	return;
}


int EXPORT_THIS d_sound_play(void *sound_resource, int loop, int vol_l, int vol_r, int jmpto) {
	return audioSoundStart(sound_resource, 0, loop, vol_l, vol_r, jmpto);
}


int EXPORT_THIS d_sound_playback_status(int playback_key) {
	int i;

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		if (d->audio.playback_chan[i].key == playback_key)
			return 0;
	return -1;
}


void EXPORT_THIS d_sound_playback_volume_set(unsigned int playback_key, int vol_l, int vol_r) {
	int i;

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		if (d->audio.playback_chan[i].key == playback_key) {
			d->audio.playback_chan[i].lvol = vol_l;
			d->audio.playback_chan[i].rvol = vol_r;
			break;
		}
	
	return;
}


void EXPORT_THIS d_sound_compression_enable() {
	d->audio.compression_enabled = 1;
	
	return;
}


void EXPORT_THIS d_sound_compression_disable() {
	d->audio.compression_enabled = 0;

	return;
}
