#ifndef __DARNIT_AUDIO_H__
#define	__DARNIT_AUDIO_H__

#define			DARNIT_AUDIO_MONO		1
#define			DARNIT_AUDIO_STEREO		2

#define			DARNIT_AUDIO_PRELOAD		0
#define			DARNIT_AUDIO_STREAM		1

#define			DARNIT_AUDIO_NOREPEAT		-1

typedef void DARNIT_SOUND;

DARNIT_SOUND *d_sound_tracked_load(const char *fname, int preload, int makemono);
DARNIT_SOUND *d_sound_streamed_load(const char *fname, int preload, int makemono);
DARNIT_SOUND *d_sound_unload(DARNIT_SOUND *sound_resource);
void d_sound_stop_all();
void d_sound_stop(int playback_key);
int d_sound_play(DARNIT_SOUND *sound_resource, int loop, int vol_l, int vol_r, int jmpto);
int d_sound_playback_status(int playback_key);
void d_sound_playback_volume_set(unsigned int playback_key, int vol_l, int vol_r);

#endif
