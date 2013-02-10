/*
Copyright (c) 2011-2013 Steven Arnow
'imgload.h' - This file is part of libdarnit

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


#ifndef __IMGLOAD_H__
#define	__IMGLOAD_H__


#include <stb_image.h>

#define	IMG_IS_RAW		1
#define	IMG_IS_PNG		0
#define	IMG_IS_BAD		-1


#define PFORMAT_RGBA8		0
#define	PFORMAT_RGBA4		1
#define	PFORMAT_RGB5A1		2
#define	PFORMAT_A8		3


typedef struct {
	unsigned int		w;
	unsigned int		h;
	unsigned int		*img_data;
} IMGLOAD_DATA;


IMGLOAD_DATA imgloadLoad(const char *fname);
void imgloadDownsample(IMGLOAD_DATA *img, unsigned int target_format);

#endif
