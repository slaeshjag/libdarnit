/*
Copyright (c) 2011-2013 Steven Arnow
'audio.h' - This file is part of libdarnit

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
	int				loop;
	int				close_when_done;

	/* Only used by stb_vorbis */
	off_t				pos_loop;
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
//	short				*samplebuf;
	short				*scratchbuf;
	int				*samplebuf;
	SDL_mutex			*lock;
	AUDIO_PLAYBACK_CHANNEL		playback_chan[AUDIO_PLAYBACK_CHANNELS];
	unsigned int			cnt;
	int				compression;
} AUDIO;


int audioSoundStart(AUDIO_HANDLE *ah, int channels, int loop, int vol_l, int vol_h, int jmpto);
void audioSoundStop(int key);
void audioSoundClear();
short audioSampleMix(short s1, short s2);
void audioFrameMix(short *target, short *source1, short *source2, int frames);
int audioInit();




#endif
