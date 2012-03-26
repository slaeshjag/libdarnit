#ifndef __IMGLOAD_H__
#define	__IMGLOAD_H__


#include <png.h>

#define	IMG_IS_RAW		1
#define	IMG_IS_PNG		0
#define	IMG_IS_BAD		-1


typedef struct {
	unsigned int		w;
	unsigned int		h;
	unsigned int		*img_data;
} IMGLOAD_DATA;


IMGLOAD_DATA imgloadLoad(const char *fname);

#endif
