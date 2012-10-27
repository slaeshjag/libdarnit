#include "darnit.h"




int audioDecodePreloaded(AUDIO_HANDLE *pb, void *buff, int buff_len, int pos) {
	if (pb->size - pos < buff_len)
		buff_len = pb->size - pos;
	memcpy(buff, pb->data + pos, buff_len);

	return buff_len;
}

#if 0
int audioCodecWAVDecode(AUDIO_HANDLE *pb, void *buff, int buff_len, int pos) {
	short sample_l, sample_r;
	int i;
	short *s_buff = buff;
	WAVE_ST_FORMAT *codec = pb->codec_handle;

	if (codec->channels == pb->channels)
		return fread(buff, 1, buff_len, pb->fp);
	
	/* We have to take the long, slow way it seems... */
	for (i = 0; i < buff_len; i++) {
		fread(&sample_l, 2, 1, pb->fp);
		if (pb->channels == 2) {
			buff[i << 1] = sample_l;
			buff[(i << 1) + 1] = sample_l;
		} else {
			fread(&sample_r, 2, 1, pb->fp);
			sample_l = audioSampleMix(sample_l, sample_r);
			buff[i] = sample_l;
		}
	}

	return i << pb->channels;
}



int audioDecodeWAV(AUDIO_HANDLE *pb, void *buff, int buff_len, int pos) {
	int i, b, j;

	if (pb->data != NULL)
		return audioDecodePreloaded(pb, buff, buf_len, pos);
	i = b = j = 0;

	do {
		i = audioCodecWAVDecode(pb->codec_handle, 2, buff + b, (buff_len - b) >> 2);
		b += i;
		if (b == buff_len)
			break;
	} while (i != 0)

	return b;
}



AUDIO_HANDLE *audioOpenWAV(const char *fname, int mode, int channels) {
/* TODO: Fix */

	WAVE_HEADER header;
	WAVE_ST_FORMAT format;
	WAVE_ST_DATA data;
	AUDIO_HANDLE *ah, *pb;
	unsigned int seek;
	void *datap;

	if ((ah = malloc(sizeof(AUDIO_HANDLE))) == NULL)
		return NULL;
	
	if ((ah->fp = fopen(fname, "rb")) == NULL) {
		free(ah);
		return NULL;
	}

	fread(&header, sizeof(WAVE_HEADER), 1, ah->fp);

	if (header.id != WAVE_ID || header.format != WAVE_FORMAT) {
		fclose(ah->fp);
		free(ah);
		return NULL;
	}

	fread(&format, sizeof(WAVE_ST_FORMAT), 1, ah->fp);

	if (format.id != WAVE_FMT || format.format != 1 || format.channels > 2 || format.samplerate != 44100 ||
	    format.bits_per_sample != 16) {
	    	fclose(ah->fp);
		free(ah);
		return NULL;
	}

	seek = format.size + 20;
	fseek(ah->fp, seek, SEEK_SET);
	
	ah->stream_data_size = seek;
	ah->channels = format.channels;
	ah->size = 0;
	ah->pos = 0;
	ah->type = AUDIO_TYPE_WAV;
	ah->data = NULL;
	ah->usage = 0;
	fseek(ah->fp, 8, SEEK_CUR);

	if (mode == ALOAD_MODE_STR)
		return ah;
	
	fseek(ah->fp, -8, SEEK_CUR);
	fread(&data, sizeof(WAVE_ST_DATA), 1, ah->fp);

	if (data.id != WAVE_DATA) {
		fclose(ah->fp);
		free(ah);
		return NULL;
	}

	if ((datap = malloc(data.size)) == NULL) {
		fclose(ah->fp);
		free(ah);
		return NULL;
	}

	audioDecodeWAV(pb, datap, ah->size, 0);

	ah->data = datap;
	fclose(ah->fp);
	ah->fp = NULL;

	return ah;
}
#endif

void *audioStopStreamed(AUDIO_HANDLE *pb) {
	pb->usage--;

	if (pb->usage > 0)
		return pb;
	if (pb->codec_handle != NULL) {
		stb_vorbis_close(pb->codec_handle);
	} if (pb->data != NULL)
		free(pb->data);
	
	free(pb->fname);
	free(pb);

	return NULL;
}


AUDIO_HANDLE *audioPlayStreamed(AUDIO_HANDLE *ah, int loop, int channels) {
	AUDIO_HANDLE *pb;
	int error;

	if (ah->data != NULL) {
		ah->usage++;
		return ah;
	}

	if ((pb = malloc(sizeof(AUDIO_HANDLE))) == NULL)
		return NULL;

	pb->codec_handle = stb_vorbis_open_filename(ah->fname, &error, NULL);

	pb->data = NULL;
	pb->fname = NULL;
	pb->close_when_done = 0;
	pb->usage = 1;
	pb->channels = ah->channels;
	pb->type = AUDIO_TYPE_OGG;

	return pb;
}


int audioDecodeStreamed(AUDIO_HANDLE *pb, void *buff, int buff_len, int pos) {
	int i, b, j;
	short t_buff[2], *s_buff = buff;


	if (pb->data != NULL)
		return audioDecodePreloaded(pb, buff, buff_len, pos);
	i = b = j = 0;
	if (pb->channels == 2)
		do {
			i = stb_vorbis_get_samples_short_interleaved(pb->codec_handle, 2, buff + b, (buff_len - b) >> 1);
			i <<= 2;
			b += i;
			if (b == buff_len) break;
		} while (i > 0);
	else {
		for (i = 0; i < buff_len >> 1; i++) {
			b = stb_vorbis_get_samples_short_interleaved(pb->codec_handle, 2, t_buff, 2);
			if (b == 0) break;
			s_buff[i] = audioSampleMix(t_buff[0], t_buff[1]);
		}
		b = i << 1;
	}

	return b;
}



AUDIO_HANDLE *audioOpenStreamed(const char *fname, int mode, int channels) {
	AUDIO_HANDLE *ah, *pb;
	char *fname_n;

	if (fname == NULL) return NULL;
	fname_n = utilPathTranslate(fname);

	if ((ah = malloc(sizeof(AUDIO_HANDLE))) == NULL)
		return NULL;

	ah->fname = fname_n;

	ah->data = NULL;
	ah->size = 0;
	ah->codec_handle = NULL;
	ah->type = AUDIO_TYPE_OGG;
	ah->usage = 0;
	ah->channels = channels;

	if (mode == ALOAD_MODE_STR)
		return ah;

	if ((pb = audioPlayStreamed(ah, 0, channels)) == NULL) {
		free(ah->fname);
		free(ah);
		return NULL;
	}

	free(ah->fname);
	ah->fname = NULL;

	ah->size = stb_vorbis_stream_length_in_samples(pb->codec_handle);
	ah->size <<= ah->channels;
	if ((ah->data = malloc(ah->size)) == NULL) {
		audioStopStreamed(pb);
		free(ah);
		return NULL;
	}

	audioDecodeStreamed(pb, ah->data, ah->size, 0);

	audioStopStreamed(pb);
	ah->usage = 0;
	ah->close_when_done = 0;
	ah->pos = 0;

	return ah;
}

AUDIO_HANDLE *audioPlayTracked(AUDIO_HANDLE *ah, int loop, int channels) {
	ModPlug_Settings mps;
	AUDIO_HANDLE *pb;

	if (ah->data != NULL) {
		ah->usage++;
		return ah;
	}

	ModPlug_GetSettings(&mps);
	mps.mFlags = MODPLUG_ENABLE_OVERSAMPLING;
	mps.mChannels = ah->channels;
	mps.mBits = 16;
	mps.mFrequency = AUDIO_SAMPLE_RATE;
	mps.mResamplingMode = MODPLUG_RESAMPLE_LINEAR;
	#ifndef PANDORA /* modplug on the pandora is too outdated, ignore when building for pandora for now */
	mps.mStereoSeparation = 256;
	#endif
	mps.mLoopCount = 55;
	ModPlug_SetSettings(&mps);

	if ((pb = malloc(sizeof(AUDIO_HANDLE))) == NULL)
		return NULL;
	
	pb->pos = 0;
	pb->type = AUDIO_TYPE_TRACKED;
	pb->data = NULL;
	pb->size = 0;
	pb->stream_data = NULL;
	pb->stream_data_size = 0;
	pb->close_when_done = 0;
	pb->usage = 1;

	if ((pb->codec_handle = ModPlug_Load(ah->stream_data, ah->stream_data_size)) == NULL) {
		free(pb);
		return NULL;
	}

	ModPlug_SetMasterVolume(pb->codec_handle, 512);
	pb->channels = ah->channels;

	return pb;
}


int audioDecodeTracked(AUDIO_HANDLE *pb, void *buff, int buff_len, int pos) {
	if (pb->data != NULL)
		return audioDecodePreloaded(pb, buff, buff_len, pos);
	return ModPlug_Read(pb->codec_handle, buff, buff_len);
}


AUDIO_HANDLE *audioOpenTracked(const char *fname, int mode, int channels) {
	AUDIO_HANDLE *ah, *pb;
	unsigned int *abuff, sz;
	unsigned int size, i;
	char *fname_n;
	FILE *fp;

	fname_n = utilPathTranslate(fname);
	if ((fp = fopen(fname_n, "rb")) == NULL)
		return NULL;

	free(fname_n);
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	if ((ah = malloc(sizeof(AUDIO_HANDLE))) == NULL) {
		fclose(fp);
		return NULL;
	}
	
	if ((ah->stream_data = malloc(size)) == NULL) {
		fclose(fp);
		free(ah);
		return NULL;
	}

	ah->stream_data_size = size;
	fread(ah->stream_data, size, 1, fp);
	fclose(fp);

	ah->data = NULL;
	ah->size = 0;
	ah->type = AUDIO_TYPE_TRACKED;
	ah->codec_handle = NULL;
	ah->usage = 0;
	ah->channels = channels;

	if (mode == ALOAD_MODE_STR)
		return ah;
	
	if ((pb = audioPlayTracked(ah, 0, channels)) == NULL) {
		free(ah->stream_data);
		free(ah);
		return NULL;
	}
	
	sz = ModPlug_GetLength(pb->codec_handle);
	sz /= 1000;
	sz++;
	sz *= AUDIO_SAMPLE_RATE;
	sz <<= channels;

	if ((abuff = malloc(sz)) == NULL) {
		audioStopTracked(pb);
		free(ah->stream_data);
		free(ah);
		return NULL;
	}

	for (i = 0; i < sz >> 2; i++)
		abuff[i] = 0;
	
	audioDecodeTracked(pb, (void *) abuff, sz, 0);

	audioStopTracked(pb);
	free(ah->stream_data);
	ah->stream_data = NULL;
	ah->stream_data_size = 0;
	ah->usage = 0;
	ah->close_when_done = 0;
	ah->pos = 0;
	ah->size = sz;
	ah->data = (void *) abuff;

	return ah;
}


void *audioStopTracked(AUDIO_HANDLE *pb) {
	pb->usage--;

	if (pb->usage > 0)
		return pb;
	if (pb->codec_handle != NULL)
		ModPlug_Unload(pb->codec_handle);
	if (pb->stream_data != NULL)
		free(pb->stream_data);
	if (pb->data != NULL)
		free(pb->data);
	free(pb);

	return NULL;
}


void *audioUnload(AUDIO_HANDLE *pb) {
	if (pb->usage > 0)
		return pb;
	if (pb->type == AUDIO_TYPE_WAV);
	else if (pb->type == AUDIO_TYPE_OGG)
		return audioStopTracked(pb);
	else if (pb->type == AUDIO_TYPE_TRACKED)
		return audioStopTracked(pb);
	
	return NULL;
}


AUDIO_HANDLE *audioPlay(AUDIO_HANDLE *ah, int channels, int loop) {
	if (ah == NULL) return NULL;

	if (ah->type == AUDIO_TYPE_WAV);
	else if (ah->type == AUDIO_TYPE_OGG)
		return audioPlayStreamed(ah, loop, channels);
	else if (ah->type == AUDIO_TYPE_TRACKED)
		return audioPlayTracked(ah, loop, channels);
	
	return NULL;
}


int audioDecode(AUDIO_HANDLE *ah, void *buff, int buff_len, int pos) {
	if (ah->type == AUDIO_TYPE_WAV);
	else if (ah->type == AUDIO_TYPE_OGG)
		return audioDecodeStreamed(ah, buff, buff_len, pos);
	else if (ah->type == AUDIO_TYPE_TRACKED)
		return audioDecodeTracked(ah, buff, buff_len, pos);
	
	return 0;
}
