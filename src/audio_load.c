#include "darnit.h"


AUDIO_HANDLE *audioOpenWAV(const char *fname) {
	WAVE_HEADER header;
	WAVE_ST_FORMAT format;
	AUDIO_HANDLE *ah;
	unsigned int seek;

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
	
	ah->channels = format.channels;
	ah->size = 0;
	ah->pos = 0;
	ah->type = ALOAD_WAV;
	ah->data = NULL;
	ah->usage = 1;

	return ah;
}
