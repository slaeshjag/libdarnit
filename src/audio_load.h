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


AUDIO_HANDLE *audioOpenWAV(const char *fname);
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
