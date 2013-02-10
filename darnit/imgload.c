/*
Copyright (c) 2011-2013 Steven Arnow
'imgload.c' - This file is part of libdarnit

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


IMGLOAD_DATA imgloadBAD(const char *fname) {
	IMGLOAD_DATA data;

	data.w = data.h = 0;
	data.img_data = NULL;

	fprintf(stderr, "Unable to load image %s\n", fname);

	return data;
}


int imgloadRead(void *user, char *data, int size) {
	return fsFileRead(data, size, user);
}


void imgloadSkip(void *user, unsigned n) {
	fsFileSeek(user, n, SEEK_CUR);

	return;
}


int imgloadEOF(void *user) {
	return fsFileEOF(user);
}


IMGLOAD_DATA imgloadSTB(const char *fname) {
	int t;
	static stbi_io_callbacks callbacks = { imgloadRead, imgloadSkip, imgloadEOF };
	IMGLOAD_DATA img;
	FILESYSTEM_FILE *file;

	if ((file = fsFileOpen(fname, "rb")) == NULL)
		return imgloadBAD(fname);
	
	img.w = img.h = 0;
	img.img_data = (void *) stbi_load_from_callbacks(&callbacks, file, (int *) &img.w, (int *) &img.h, &t, 4);

	fsFileClose(file);
	
	return img;
}


IMGLOAD_DATA imgloadLoad(const char *fname) {
	IMGLOAD_DATA data;
	if (fname == NULL) return imgloadBAD(fname);
	
	data = imgloadSTB(fname);
	return data;
}


void imgloadDownsample(IMGLOAD_DATA *img, unsigned int target_format) {
	unsigned short *data;
	unsigned short pixel;
	int i;

	if (target_format == PFORMAT_RGBA8)
		return;
	
	data = malloc(sizeof(unsigned short) * img->w * img->h);

	if (target_format == PFORMAT_RGBA4) {
		for (i = 0; i < img->w * img->h; i++) {
			pixel = ((img->img_data[i] & 0xF0) << 8);
			pixel |= ((img->img_data[i] & 0xF000) >> 4);
			pixel |= ((img->img_data[i] & 0xF00000) >> 16);
			pixel |= ((img->img_data[i] & 0xF0000000) >> 28);
			data[i] = pixel;
		}
	} else if (target_format == PFORMAT_RGB5A1) {
		for (i = 0; i < img->w * img->h; i++) {
			pixel = ((img->img_data[i] & 0xF8) << 8);
			pixel |= ((img->img_data[i] & 0xF800) >> 5);
			pixel |= ((img->img_data[i] & 0xF80000) >> 18);
			pixel |= ((img->img_data[i] & 0x80000000) >> 31);
			data[i] = pixel;
		}
	} else
		return;
	
	free(img->img_data);
	img->img_data = (void *) data;

	return;
}
