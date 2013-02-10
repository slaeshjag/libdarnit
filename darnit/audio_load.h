/*
Copyright (c) 2011-2013 Steven Arnow
'audio_load.h' - This file is part of libdarnit

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


#ifndef __AUDIO_LOAD_H__
#define	__AUDIO_LOAD_H__


#define			WAVE_ID		0x46464952
#define			WAVE_FORMAT	0x45564157
#define			WAVE_FMT	0x20746D66
#define			WAVE_DATA	0x61746164


#define			ALOAD_MODE_PRE	0
#define			ALOAD_MODE_STR	1


typedef struct {
	unsigned int		id;
	unsigned int		size;
	unsigned int		format;
} WAVE_HEADER;


typedef struct {
	unsigned int		id;
	unsigned int		size;
	unsigned int		format		: 16;
	unsigned int		channels	: 16;
	unsigned int		samplerate;
	unsigned int		byterate;
	unsigned int		block_align	: 16;
	unsigned int		bits_per_sample	: 16;
} WAVE_ST_FORMAT;


typedef struct {
	unsigned int		id;
	unsigned int		size;
} WAVE_ST_DATA;


AUDIO_HANDLE *audioOpenWAV(const char *fname, int mode, int channels);
AUDIO_HANDLE *audioPlayTracked(AUDIO_HANDLE *ah, int loop, int channels);
int audioDecodeTracked(AUDIO_HANDLE *pb, void *buff, int buff_len, int pos);
AUDIO_HANDLE *audioOpenTracked(const char *fname, int mode, int channels);
void *audioStopTracked(AUDIO_HANDLE *pb);
void *audioStopStreamed(AUDIO_HANDLE *pb);
AUDIO_HANDLE *audioPlayStreamed(AUDIO_HANDLE *ah, int loop, int channels);
int audioDecodeStreamed(AUDIO_HANDLE *pb, void *buff, int buff_len, int pos);
AUDIO_HANDLE *audioOpenStreamed(const char *fname, int mode, int channels);
void *audioUnload(AUDIO_HANDLE *pb);
AUDIO_HANDLE *audioPlay(AUDIO_HANDLE *ah, int channels, int loop);
int audioDecode(AUDIO_HANDLE *ah, void *buff, int buff_len, int pos);


#endif
