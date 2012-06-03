#include "darnit.h"


void audioSoundStop(void *handle, int key) {
	DARNIT *m = handle;
	int i;
	
	if (key == -1)
		return;

	SDL_mutexP(m->audio.lock);

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		if (m->audio.playback_chan[i].key == key)
			break;
	if (i == AUDIO_PLAYBACK_CHANNELS) {
		SDL_mutexV(m->audio.lock);
		return;
	}

	
	audioUnload(m->audio.playback_chan[i].res);
	m->audio.playback_chan[i].key = -1;
	
	SDL_mutexV(m->audio.lock);
	
	return;
}


void audioSoundClear(void *handle) {
	DARNIT *m = handle;
	int i;

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		audioSoundStop(handle, m->audio.playback_chan[i].key);

	return;
}


int audioPlaySlotGet(void *handle) {
	DARNIT *m = handle;
	int i;

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		if (m->audio.playback_chan[i].key == -1)
			return i;
	
	return -1;
}


int audioSoundStart(void *handle, AUDIO_HANDLE *ah, int channels, int loop, int vol_l, int vol_r, int jmpto) {
	DARNIT *m = handle;
	int i;

	if ((i = audioPlaySlotGet(handle)) == -1)
		return -1;

	m->audio.playback_chan[i].pos = 0;
	m->audio.playback_chan[i].lvol = vol_l;
	m->audio.playback_chan[i].rvol = vol_r;
	m->audio.playback_chan[i].len = 0;
	if ((m->audio.playback_chan[i].res = audioPlay(ah, channels, loop)) == NULL)
		return -1;
	
	m->audio.playback_chan[i].key = m->audio.cnt;
	m->audio.cnt++;

	return m->audio.playback_chan[i].key;
}


short audioSampleMix(short s1, short s2) {
	return s1 + s2 - ((s1*s2) >> 15);
}


void audioFrameMix(short *target, short *source1, short *source2, int frames) {
	int i;

	frames <<=1;

	for (i = 0; i < frames; i++)
		target[i] = audioSampleMix(source1[i], source2[i]);
	
	return;
}


void audioMixDecoded(void *handle, int channel, int frames, void *mixdata) {
	DARNIT *m = handle;
	int i, sample, decoded, loop;


	if (m->audio.playback_chan[channel].res->channels == 1) {
		decoded = audioDecode(m->audio.playback_chan[channel].res, m->audio.scratchbuf, frames<<1, m->audio.playback_chan[channel].pos);
		loop = decoded >> 1;

		for (i = 0; i < decoded >> 1; i++) {
			m->audio.samplebuf[i<<1] = m->audio.scratchbuf[m->audio.playback_chan[channel].pos];
			m->audio.samplebuf[(i<<1)+1] = m->audio.scratchbuf[m->audio.playback_chan[channel].pos];
		}
	} else {
		decoded = audioDecode(m->audio.playback_chan[channel].res, m->audio.samplebuf, frames<<2, m->audio.playback_chan[channel].pos);
		loop = decoded >> 2;
	}
	
	i = loop << 1;

	for (; i < frames<<1; i++)
		m->audio.samplebuf[i] = 0;
	for (i = 0; i < loop; i++) {
		sample = m->audio.samplebuf[i<<1];
		sample *= m->audio.playback_chan[channel].lvol;
		sample >>= 7;
		m->audio.samplebuf[i<<1] = sample;
		
		sample = m->audio.samplebuf[(i<<1)+1];
		sample *= m->audio.playback_chan[channel].rvol;
		sample >>= 7;
		m->audio.samplebuf[(i<<1)+1] = sample;
	}

	m->audio.playback_chan[channel].pos += decoded;
	
	if (decoded == 0)
		m->audio.playback_chan[channel].key = -1;

	audioFrameMix(mixdata, m->audio.samplebuf, mixdata, frames);

	return;
}


void audioDecodeAndMix(void *handle, int frames, void *mixdata) {
	DARNIT *m = handle;
	int i, samples;
	short *mixbuf = mixdata;

	samples = frames << 1;
	for (i = 0; i < samples; i++)
		mixbuf[i] = 0;
	
	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++) {
		if (m->audio.playback_chan[i].key == -1)
			continue;
		audioMixDecoded(m, i, frames, mixdata);
	}

	return;
}


void audioMix(void *handle, Uint8 *mixdata, int bytes) {
	DARNIT *m = handle;
	int frames;

	frames = bytes >>2;
	SDL_mutexP(m->audio.lock);
	
	audioDecodeAndMix(m, frames, mixdata);
	
	SDL_mutexV(m->audio.lock);

	return;
}


int audioInit(void *handle) {
	DARNIT *m = handle;
	SDL_AudioSpec fmt;
	int i;

	fmt.freq = AUDIO_SAMPLE_RATE;
	fmt.format = AUDIO_S16;
	fmt.channels = 2;
	fmt.samples = 1024;
	fmt.callback = audioMix;
	fmt.userdata = handle;

	m->audio.lock = SDL_CreateMutex();

	if ((m->audio.samplebuf = malloc(1024*4*4)) == NULL) {
		fprintf(stderr, "libDarnit: Unable to malloc(%i)\n", 4096);
		return -1;
	}
	if ((m->audio.scratchbuf = malloc(1024*4*4)) == NULL) {
		free(m->audio.samplebuf);
		fprintf(stderr, "libDarnit: Unable to malloc(%i)\n", 4096);
		return -1;
	}

	for (i = 0; i < AUDIO_PLAYBACK_CHANNELS; i++)
		m->audio.playback_chan[i].key = -1;

	if (SDL_OpenAudio(&fmt, NULL) < 0) {
		fprintf(stderr, "libDarnit: Unable to open audio: %s\n", SDL_GetError());
		return -1;
	}

	m->audio.cnt = 0;

	SDL_PauseAudio(0);

	return 0;
}
