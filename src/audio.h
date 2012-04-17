#ifndef __AUDIO_H__
#define	__AUDIO_H__

#include <SDL/SDL_audio.h>
#include <SDL/SDL_mutex.h>
#include <modplay/modplay.h>
#include <vorbis/vorbisfile.h>

#define		AUDIO_SFX_CHANNELS		16
#define		AUDIO_SFX_FILES			64


typedef struct {
	int				sfx;
	int				lvol;
	int				rvol;
	int				pos;
	int				len;
	unsigned int			key;
} AUDIO_SFX_CHANNEL;


typedef struct {
	short				*buf;
	int				len;
} AUDIO_SFX;


typedef struct {
	int				playing;
	MODFILE				*modfile;
	OggVorbis_File			*vorbis;
} AUDIO_MUSIC;


typedef struct {
	short				*sfxbuf;
	short				*musicbuf;
	int				musicvol;
	int				sfxvol;
	SDL_mutex			*lock;
	AUDIO_SFX_CHANNEL		sfxchan[AUDIO_SFX_CHANNELS];
	AUDIO_SFX			sfx[AUDIO_SFX_FILES];
	AUDIO_MUSIC			music;
	unsigned int			cnt;
} AUDIO;


void audioMusicPlayMod(void *handle, const char *fname);
void audioMusicPlayVorbis(void *handle, const char *fname);
void audioMusicStop(void *handle);

int audioSFXLoad(void *handle, const char *fname);
void audioSFXFree(void *handle, int index);
void audioSFXClear(void *handle);
int audioSFXPlaySlotGet(void *handle);
unsigned int audioSFXPlay(void *handle, int sfx, int vol_l, int vol_r);
short audioSampleMix(short s1, short s2);
void audioFrameMix(short *target, short *source1, short *source2, int frames);
void audioMusicDecode(void *handle, int frames);
void audioSFXMix(void *handle, int channel, int frames);
void audioSFXDecode(void *handle, int frames);
void audioMix(void *handle, Uint8 *mixdata, int bytes);
int audioInit(void *handle);




#endif
