#ifndef __IMGLOAD_H__
#define	__IMGLOAD_H__


#include <png.h>

#define	IMG_IS_RAW		1
#define	IMG_IS_PNG		0
#define	IMG_IS_BAD		-1


#define PFORMAT_RGBA8		0
#define	PFORMAT_RGBA4		1
#define	PFORMAT_RGB5A1		2


typedef struct {
	unsigned int		w;
	unsigned int		h;
	unsigned int		*img_data;
} IMGLOAD_DATA;


IMGLOAD_DATA imgloadLoad(const char *fname);
void imgloadDownsample(IMGLOAD_DATA *img, unsigned int target_format);

#endif
