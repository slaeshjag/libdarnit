#ifndef __DARNIT_AUDIO_H__
#define	__DARNIT_AUDIO_H__

void *darnitSoundLoadTracked(const char *fname, int preload, int makemono);
void *darnitSoundLoadStreamed(const char *fname, int preload, int makemono);
void *darnitSoundUnload(void *sound_resource);
void darnitSoundPlaybackStopAll(void *handle);
void darnitSoundPlaybackStop(void *handle, int playback_key);
int darnitSoundPlaybackStart(void *handle, void *sound_resource, int channels, int loop, int vol_l, int vol_r, int jmpto);
int darnitSoundPlaybackCheck(void *handle, int playback_key);
void darnitSoundPlaybackVolumeSet(void *handle, unsigned int playback_key, int vol_l, int vol_r);

#endif
