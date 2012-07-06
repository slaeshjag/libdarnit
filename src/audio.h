#ifndef __AUDIO_H__
#define	__AUDIO_H__

#include <SDL/SDL_audio.h>
#include <SDL/SDL_mutex.h>
#include <libmodplug/modplug.h>
#include <stb_vorbis.h>

#define		AUDIO_PLAYBACK_CHANNELS		16
#define		AUDIO_SFX_FILES			64


#define		AUDIO_TYPE_WAV			0
#define		AUDIO_TYPE_OGG			1
#define		AUDIO_TYPE_TRACKED		2

#define		AUDIO_SAMPLE_RATE		44100


typedef struct {
	char				*fname;
	unsigned int			type;
	unsigned int			pos;
	void				*codec_handle;
	FILE				*fp;
	char				*data;
	unsigned int			size;
	char				*stream_data;
	unsigned int			stream_data_size;
	unsigned int			usage;
	unsigned int			channels;
	int				close_when_done;
} AUDIO_HANDLE;


typedef struct {
	int				lvol;
	int				rvol;
	int				pos;
	int				len;
	unsigned int			key;
	AUDIO_HANDLE			*res;
} AUDIO_PLAYBACK_CHANNEL;


typedef struct {
	short				*buf;
	int				len;
} AUDIO_SFX;


typedef struct {
	int				playing;
	ModPlugFile			*modfile;
	void				*modplugdata;
	int				modplugdata_len;
	stb_vorbis			*vorbis;
} AUDIO_MUSIC;


typedef struct {
	short				*samplebuf;
	short				*scratchbuf;
	SDL_mutex			*lock;
	AUDIO_PLAYBACK_CHANNEL		playback_chan[AUDIO_PLAYBACK_CHANNELS];
	unsigned int			cnt;
} AUDIO;


int audioSoundStart(void *handle, AUDIO_HANDLE *ah, int channels, int loop, int vol_l, int vol_h, int jmpto);
void audioSoundStop(void *handle, int key);
void audioSoundClear(void *handle);
short audioSampleMix(short s1, short s2);
void audioFrameMix(short *target, short *source1, short *source2, int frames);
int audioInit(void *handle);




#endif
