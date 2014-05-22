/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_bbox.h' - This file is part of libdarnit

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


#ifndef __DARNIT_BBOX_H__
#define	__DARNIT_BBOX_H__

typedef void DARNIT_BBOX;

typedef enum {
	DARNIT_BBOX_SORT_X,
	DARNIT_BBOX_SORT_Y,
} DARNIT_BBOX_SORT;


DARNIT_BBOX *d_bbox_new(unsigned int size);
DARNIT_BBOX *d_bbox_free(DARNIT_BBOX *bbox);
void d_bbox_sortmode(DARNIT_BBOX *bbox, DARNIT_BBOX_SORT mode);
void d_bbox_set_indexkey(DARNIT_BBOX *bbox);
int d_bbox_test(DARNIT_BBOX *bbox, int x, int y, unsigned int w, unsigned int h, unsigned int *list, unsigned int listlen);
int d_bbox_add(DARNIT_BBOX *bbox, int x, int y, unsigned int w, unsigned int h);
void d_bbox_delete(DARNIT_BBOX *bbox, int key);
void d_bbox_move(DARNIT_BBOX *bbox, int key, int x, int y);
void d_bbox_resize(DARNIT_BBOX *bbox, int key, unsigned int w, unsigned int h);
void d_bbox_clear(DARNIT_BBOX *bbox);

#endif
