/*
Copyright (c) 2011-2013 Steven Arnow
'audio.c' - This file is part of libdarnit

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

#ifndef DARNIT_HEADLESS
void audioSoundStop(int key) {
	int i;
	
	if (key == -1)
		return;

	SDL_mutexP(d->audio.lock);

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		if (d->audio.playback_chan[i].key == key)
			break;
	if (i == AUDIO_PLAYBACK_CHANNELS) {
		SDL_mutexV(d->audio.lock);
		return;
	}

	
	audioUnload(d->audio.playback_chan[i].res);
	d->audio.playback_chan[i].key = -1;
	
	SDL_mutexV(d->audio.lock);
	
	return;
}


void audioSoundClear() {
	int i;

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		audioSoundStop(d->audio.playback_chan[i].key);

	return;
}


int audioPlaySlotGet() {
	int i;

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		if (d->audio.playback_chan[i].key == -1)
			return i;
	
	return -1;
}


int audioSoundStart(AUDIO_HANDLE *ah, int channels, int loop, int vol_l, int vol_r, int jmpto) {
	int i;

	if ((i = audioPlaySlotGet()) == -1)
		return -1;
	
	d->audio.playback_chan[i].pos = 0;
	d->audio.playback_chan[i].lvol = vol_l;
	d->audio.playback_chan[i].rvol = vol_r;
	d->audio.playback_chan[i].len = 0;
	if ((d->audio.playback_chan[i].res = audioPlay(ah, channels, loop)) == NULL)
		return -1;
	
	d->audio.playback_chan[i].key = d->audio.cnt;
	d->audio.cnt++;

	return d->audio.playback_chan[i].key;
}


short audioSampleMix(short s1, short s2) {
	int s1_, s2_, min_, out;
	
	s1_ = abs(s1);
	s2_ = abs(s2);
	min_ = (s1_ > s2_) ? s2_ : s1_;
	out = s1 + s2;
	out -= ((out * min_) >> 16);
	
	return out;
}


void audioFrameMix(short *target, short *source1, short *source2, int frames) {
	int i;

	frames <<=1;

	for (i = 0; i < frames; i++)
		target[i] = audioSampleMix(source1[i], source2[i]);
	
	return;
}

#if 0
void audioMixDecoded(int channel, int frames, void *mixdata) {
	int i, sample, decoded, loop;


	if (d->audio.playback_chan[channel].res->channels == 1) {
		decoded = audioDecode(d->audio.playback_chan[channel].res, d->audio.scratchbuf, frames<<1, d->audio.playback_chan[channel].pos);
		loop = decoded >> 1;

		for (i = 0; i < decoded >> 1; i++) {
			d->audio.samplebuf[i<<1] = d->audio.scratchbuf[i];
			d->audio.samplebuf[(i<<1)+1] = d->audio.scratchbuf[i];
		}
	} else {
		decoded = audioDecode(d->audio.playback_chan[channel].res, d->audio.samplebuf, frames<<2, d->audio.playback_chan[channel].pos);
		loop = decoded >> 2;
	}
	
	i = loop << 1;

	for (; i < frames<<1; i++)
		d->audio.samplebuf[i] = 0;
	for (i = 0; i < loop; i++) {
		sample = d->audio.samplebuf[i<<1];
		sample *= d->audio.playback_chan[channel].lvol;
		sample >>= 7;
		d->audio.samplebuf[i<<1] = sample;
		
		sample = d->audio.samplebuf[(i<<1)+1];
		sample *= d->audio.playback_chan[channel].rvol;
		sample >>= 7;
		d->audio.samplebuf[(i<<1)+1] = sample;
	}

	d->audio.playback_chan[channel].pos += decoded;
	
	if (decoded == 0)
		d->audio.playback_chan[channel].key = -1;

	audioFrameMix(mixdata, d->audio.samplebuf, mixdata, frames);

	return;
}


void audioDecodeAndMix(int frames, void *mixdata) {
	int i, samples;
	short *mixbuf = mixdata;

	samples = frames << 1;
	for (i = 0; i < samples; i++)
		mixbuf[i] = 0;
	
	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++) {
		if (d->audio.playback_chan[i].key == -1)
			continue;
		audioMixDecoded(i, frames, mixdata);
	}

	return;
}
#endif

void audioDecodeMixNew(int frames, void *mixdata) {
	int i, j, decoded, samples, deflection;
	short *mixbuf = mixdata;
	samples = frames << 1;

	for (i = 0; i < samples; i++)
		d->audio.samplebuf[i] = 0;
	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++) {
		if (d->audio.playback_chan[i].key == -1)
			continue;
		if (d->audio.playback_chan[i].res->channels == 1) {
			decoded = audioDecode(d->audio.playback_chan[i].res, d->audio.scratchbuf, frames<<1, d->audio.playback_chan[i].pos);
			for (j = 0; j < (decoded >> 1); j++) {
				d->audio.samplebuf[j<<1] += (((int)d->audio.scratchbuf[j] * d->audio.playback_chan[i].lvol) >> 7);
				d->audio.samplebuf[(j<<1)+1] += (((int)d->audio.scratchbuf[j] * d->audio.playback_chan[i].rvol) >> 7);
			}
		} else {
			decoded = audioDecode(d->audio.playback_chan[i].res, d->audio.scratchbuf, frames<<2, d->audio.playback_chan[i].pos);
			for (j = 0; j < (decoded >> 2); j++) {
				d->audio.samplebuf[j<<1] += (((int)d->audio.scratchbuf[j<<1] * d->audio.playback_chan[i].lvol) >> 7);
				d->audio.samplebuf[(j<<1)+1] += (((int)d->audio.scratchbuf[(j<<1)+1] * d->audio.playback_chan[i].rvol) >> 7);
			}
		}
	}

	if (d->audio.compression_enabled) {
		deflection = 0;
	
		for (i = 0; i < samples; i++)
			deflection = (abs(d->audio.samplebuf[i]) > deflection) ? abs(d->audio.samplebuf[i]) : deflection;
	
		deflection = (deflection > 0x7fff) ? (deflection >> 8) + 1: 1;
		if (d->audio.compression != deflection) {
			d->audio.compression += (d->audio.compression > deflection) ? ((deflection - d->audio.compression) >> 6) - 1: deflection - d->audio.compression;
			if (deflection > d->audio.compression)
				d->audio.compression = deflection;
		}
	} else
		d->audio.compression = 1;

	if (d->audio.compression < 128)
		d->audio.compression = 1;

	if (d->audio.compression > 1)
		for (i = 0; i < samples; i++)
			mixbuf[i] = (d->audio.samplebuf[i] << 7) / d->audio.compression;
	
	if (d->audio.compression == 1)
		for (i = 0; i < samples; i++)
			mixbuf[i] = d->audio.samplebuf[i];
	return;
}


void audioMix(void *data, Uint8 *mixdata, int bytes) {
	int frames;

	frames = bytes >>2;
	SDL_mutexP(d->audio.lock);
	
	audioDecodeMixNew(frames, mixdata);
	
	SDL_mutexV(d->audio.lock);

	return;
}


int audioInit() {
	SDL_AudioSpec fmt;
	int i;

	fmt.freq = AUDIO_SAMPLE_RATE;
	fmt.format = AUDIO_S16;
	fmt.channels = 2;
	fmt.samples = 1024;
	fmt.callback = audioMix;
	fmt.userdata = NULL;

	d->audio.lock = SDL_CreateMutex();

	if ((d->audio.samplebuf = malloc(1024*4*4*2)) == NULL) {
		fprintf(stderr, "libDarnit: Unable to malloc(%i)\n", 4096);
		return -1;
	}
	if ((d->audio.scratchbuf = malloc(1024*4*4)) == NULL) {
		free(d->audio.samplebuf);
		fprintf(stderr, "libDarnit: Unable to malloc(%i)\n", 4096);
		return -1;
	}

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		d->audio.playback_chan[i].key = -1;

	if (SDL_OpenAudio(&fmt, NULL) < 0) {
		fprintf(stderr, "libDarnit: Unable to open audio: %s\n", SDL_GetError());
		return -1;
	}

	d->audio.cnt = 0;
	d->audio.compression = 1;
	d->audio.compression_enabled = 1;

	SDL_PauseAudio(0);

	return 0;
}

// DARNIT_HEADLESS
#endif
