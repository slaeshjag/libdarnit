#ifndef __DARNIT_AUDIO_H__
#define	__DARNIT_AUDIO_H__

#define			DARNIT_AUDIO_MONO		1
#define			DARNIT_AUDIO_STEREO		2

#define			DARNIT_AUDIO_PRELOAD		0
#define			DARNIT_AUDIO_STREAM		1


void *darnitSoundLoadTracked(const char *fname, int preload, int makemono);
void *darnitSoundLoadStreamed(const char *fname, int preload, int makemono);
void *darnitSoundUnload(void *sound_resource);
void darnitSoundPlaybackStopAll();
void darnitSoundPlaybackStop(int playback_key);
int darnitSoundPlaybackStart(void *sound_resource, int loop, int vol_l, int vol_r, int jmpto);
int darnitSoundPlaybackCheck(int playback_key);
void darnitSoundPlaybackVolumeSet(unsigned int playback_key, int vol_l, int vol_r);

#endif
