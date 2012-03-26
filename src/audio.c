#include "darner.h"


void audioMusicPlayMod(void *handle, const char *fname) {
	DARNER *m = handle;
	void *modfile;

	modfile = malloc(sizeof(MODFILE));
	MODFILE_Init(modfile);
	MODFILE_Load(fname, modfile);
	MODFILE_Start(modfile);
	MODFILE_SetFormat(modfile, 44100, 2, 16, 1);
	m->audio.music.modfile = modfile; 
	m->audio.music.modfile->musicvolume = 64;
	m->audio.music.modfile->sfxvolume = 64;

	return;
}


void audioMusicPlayVorbis(void *handle, const char *fname) {
	DARNER *m = handle;
	void *vorbis;

	vorbis = malloc(sizeof(OggVorbis_File));
	ov_fopen((char *) fname, vorbis);
	m->audio.music.vorbis = vorbis;

	return;
}


void audioMusicStop(void *handle) {
	DARNER *m = handle;
	SDL_mutexP(m->audio.lock);
	
	if (m->audio.music.modfile != NULL) {
		MODFILE_Stop(m->audio.music.modfile);
		MODFILE_Free(m->audio.music.modfile);
		free(m->audio.music.modfile);
		m->audio.music.modfile = NULL;
	} else if (m->audio.music.vorbis != NULL) {
		ov_clear(m->audio.music.vorbis);
		free(m->audio.music.vorbis);
		m->audio.music.vorbis = NULL;
	}
	SDL_mutexV(m->audio.lock);

	return;
}


int audioSFXLoad(void *handle, const char *fname) {
	DARNER *m = handle;
	int i, bytes;
	FILE *fp;

	for (i = 0; i < AUDIO_SFX_FILES; i++) {
		if (m->audio.sfx[i].len == -1)
			break;
	}

	if (i == AUDIO_SFX_FILES)
		return -1;
	
	if ((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "libDarner: Unable to open SFX file %s\n", fname);
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	m->audio.sfx[i].len = ftell(fp) >> 1;
	bytes = ftell(fp);
	if ((m->audio.sfx[i].buf = malloc(bytes)) == NULL) {
		fprintf(stderr, "libDarner: Unable to malloc(%i)\n", bytes);
		fclose(fp);
		m->audio.sfx[i].len = -1;
		return -1;
	}

	fseek(fp, 0, SEEK_SET);
	fread(m->audio.sfx[i].buf, bytes, 1, fp);
	fclose(fp);

	return i;
}


void audioSFXFree(void *handle, int index) {
	DARNER *m = handle;

	SDL_mutexP(m->audio.lock);

	free(m->audio.sfx[index].buf);
	m->audio.sfx[index].buf = NULL;
	m->audio.sfx[index].len = -1;

	SDL_mutexV(m->audio.lock);

	return;
}


void audioSFXClear(void *handle) {
	int i;


	for (i = 0; i < AUDIO_SFX_FILES; i++)
		audioSFXFree(handle, i);

	
	return;
}


int audioSFXPlaySlotGet(void *handle) {
	DARNER *m = handle;
	int i;

	for (i = 0; i < AUDIO_SFX_CHANNELS; i++)
		if (m->audio.sfxchan[i].pos == -1)
			return i;
	
	return -1;
}


int audioSFXPlay(void *handle, int sfx, int vol_l, int vol_r) {
	DARNER *m = handle;
	int chan;

	if ((chan = audioSFXPlaySlotGet(m)) == -1) {
		fprintf(stderr, "libDarner: Unable to play SFX: Out of playback channels\n");
		return -1;
	}

	m->audio.sfxchan[chan].sfx = sfx;
	m->audio.sfxchan[chan].lvol = vol_l;
	m->audio.sfxchan[chan].rvol = vol_r;
	m->audio.sfxchan[chan].pos = 0;
	m->audio.sfxchan[chan].len = m->audio.sfx[sfx].len;
	
	return chan;
};



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


void audioMusicDecode(void *handle, int frames) {
	DARNER *m = handle;
	int i, bytes, bt, b;
	char *buf = (void *) m->audio.musicbuf;

	bytes = frames << 2;

	for (i = 0; i < frames<<1; i++)
		m->audio.musicbuf[i] = 0;

	if (m->audio.music.modfile != NULL) {
		m->audio.music.modfile->mixingbuf = (void *) m->audio.musicbuf;
		m->audio.music.modfile->mixingbuflen = frames<<2;
		MODFILE_Player(m->audio.music.modfile);
	} else if (m->audio.music.vorbis != NULL) {
		for (bt = b = 0; bt < bytes; ) {
			i = 0;
			if ((b = ov_read(m->audio.music.vorbis, &buf[bt], bytes-bt, 0, 2, 1, &i)) <= 0) {
				audioMusicStop(handle);
				break;
			}
			bt += b;
		}
	}
	/* TODO: Implement rest */

	return;
}


void audioSFXMix(void *handle, int channel, int frames) {
	DARNER *m = handle;
	int i, sample;
	short buf[8192], *tst;

	for (i = 0; i < 8192; i++)
		buf[i] = 0;

	for (i = 0; i < frames; i++) {
		if (m->audio.sfxchan[channel].pos >= m->audio.sfxchan[channel].len) {
			m->audio.sfxchan[channel].pos = -1;
			break;
		}
		sample = m->audio.sfx[m->audio.sfxchan[channel].sfx].buf[m->audio.sfxchan[channel].pos] * m->audio.sfxchan[channel].lvol;
		sample >>= 7;
		buf[i<<1] = sample;
		sample = m->audio.sfx[m->audio.sfxchan[channel].sfx].buf[m->audio.sfxchan[channel].pos] * m->audio.sfxchan[channel].rvol;
		sample >>= 7;
		buf[(i<<1)+1] = sample;
		m->audio.sfxchan[channel].pos++;
	}

	tst = m->audio.sfxbuf;
	audioFrameMix(tst, m->audio.sfxbuf, buf, frames);

	return;
}


void audioSFXDecode(void *handle, int frames) {
	DARNER *m = handle;
	int i, samples;

	samples = frames << 1;
	for (i = 0; i < samples; i++)
		m->audio.sfxbuf[i] = 0;
	
	for (i = 0; i < AUDIO_SFX_CHANNELS; i++) {
		if (m->audio.sfxchan[i].pos == -1)
			continue;
		audioSFXMix(m, i, frames);
	}

	return;
}



void audioMix(void *handle, Uint8 *mixdata, int bytes) {
	DARNER *m = handle;
	int frames;
	short *stream = (void *) mixdata;

	frames = bytes >>2;
	SDL_mutexP(m->audio.lock);
	audioMusicDecode(m, frames);
	audioSFXDecode(m, frames);
	audioFrameMix(stream, m->audio.sfxbuf, m->audio.musicbuf, frames*2);
	SDL_mutexV(m->audio.lock);

	return;
}


int audioInit(void *handle) {
	DARNER *m = handle;
	SDL_AudioSpec fmt;
	int i;

	fmt.freq = 44100;
	fmt.format = AUDIO_S16;
	fmt.channels = 2;
	fmt.samples = 1024;
	fmt.callback = audioMix;
	fmt.userdata = handle;

	m->audio.lock = SDL_CreateMutex();

	if ((m->audio.sfxbuf = malloc(1024*4*4)) == NULL) {
		fprintf(stderr, "libDarner: Unable to malloc(%i)\n", 4096);
		return -1;
	}

	if ((m->audio.musicbuf = malloc(1024*4*4)) == NULL) {
		fprintf(stderr, "libDarner: Unable to malloc(%i)\n", 4096);
		return -1;
	}

	for (i = 0; i < AUDIO_SFX_CHANNELS; i++)
		m->audio.sfxchan[i].pos = -1;
	for (i = 0; i < AUDIO_SFX_FILES; i++) {
		m->audio.sfx[i].len = -1;
		m->audio.sfx[i].buf = NULL;
	}

	m->audio.music.modfile = NULL;
	m->audio.music.vorbis = NULL;

	if (SDL_OpenAudio(&fmt, NULL) < 0) {
		fprintf(stderr, "libDarner: Unable to open audio: %s\n", SDL_GetError());
		return -1;
	}

	m->audio.sfxvol = 127;
	m->audio.musicvol = 127;

	SDL_PauseAudio(0);

	return 0;
}
