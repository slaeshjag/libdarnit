#ifndef __AUDIO_LOAD_H__
#define	__AUDIO_LOAD_H__


#define			WAVE_ID		0x46464952
#define			WAVE_FORMAT	0x45564157
#define			WAVE_FMT	0x20746D66
#define			WAVE_DATA	0x61746164


#define			ALOAD_WAV	0


typedef struct {
	unsigned int		id;
	unsigned int		size;
	unsigned int		format;
} WAVE_HEADER;


typedef struct {
	unsigned int		id;
	unsigned int		size;
	unsigned int		format : 16;
	unsigned int		channels : 16;
	unsigned int		samplerate;
	unsigned int		byterate;
	unsigned int		block_align : 16;
	unsigned int		bits_per_sample	: 16;
} WAVE_ST_FORMAT;


typedef struct {
	unsigned int		id;
	unsigned int		size;
} WAVE_ST_DATA;


typedef struct {
	unsigned int		type;
	unsigned int		pos;
	unsigned int		size;
	FILE			*fp;
	char			*data;
	unsigned int		usage;
	unsigned int		channels;
} AUDIO_HANDLE;


#endif
