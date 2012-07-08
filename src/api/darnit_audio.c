#include "darnit.h"


void EXPORT_THIS *darnitSoundLoadTracked(const char *fname, int preload, int makemono) {
	return audioOpenTracked(fname, preload, makemono);
}


void EXPORT_THIS *darnitSoundLoadStreamed(const char *fname, int preload, int makemono) {
	return audioOpenStreamed(fname, preload, makemono);
}


void EXPORT_THIS *darnitSoundUnload(void *sound_resource) {
	return audioUnload(sound_resource);
}


void EXPORT_THIS darnitSoundPlaybackStopAll(void *handle) {
	audioSoundClear(handle);

	return;
}


void EXPORT_THIS darnitSoundPlaybackStop(void *handle, int playback_key) {
	audioSoundStop(handle, playback_key);

	return;
}


int EXPORT_THIS darnitSoundPlaybackStart(void *handle, void *sound_resource, int loop, int vol_l, int vol_r, int jmpto) {
	return audioSoundStart(handle, sound_resource, 0, loop, vol_l, vol_r, jmpto);
}


int EXPORT_THIS darnitSoundPlaybackCheck(void *handle, int playback_key) {
	DARNIT *d = handle;
	int i;

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		if (d->audio.playback_chan[i].key == playback_key)
			return 0;
	return -1;
}


void EXPORT_THIS darnitSoundPlaybackVolumeSet(void *handle, unsigned int playback_key, int vol_l, int vol_r) {
	DARNIT *d = handle;
	int i;

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		if (d->audio.playback_chan[i].key == playback_key) {
			d->audio.playback_chan[i].lvol = vol_l;
			d->audio.playback_chan[i].rvol = vol_r;
			break;
		}
	
	return;
}
