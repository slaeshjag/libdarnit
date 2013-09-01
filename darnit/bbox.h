/*
Copyright (c) 2011-2013 Steven Arnow
'bbox.h' - This file is part of libdarnit

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


#ifndef __BBOX_H__
#define	__BBOX_H__


#define			BBOX_MAX		8192
#define			TARGET_FILTER		8
#define			SORT_MODE_X		0
#define			SORT_MODE_Y		1


typedef struct {
	int				x;
	int				y;
	int				w;
	int				h;
	int				xb;
	int				yb;
	int				key;
} DARNIT_BBOX_ENTRY;


typedef struct {
	DARNIT_BBOX_ENTRY		*bbox;
	unsigned int			bboxes;
	unsigned int			sort;
	unsigned int			cnt;
	unsigned int			sortmode;
	unsigned int			max;
} BBOX;


int bboxAdd(BBOX *bbox, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void bboxDelete(BBOX *bbox, int key);
void bboxMove(BBOX *bbox, int key, unsigned int x, unsigned int y);
void bboxResize(BBOX *bbox, int key, unsigned int w, unsigned int h);
void bboxSort(BBOX *bbox);
void bboxClear(BBOX *bbox);
void *bboxNew(unsigned int size);
void *bboxFree(BBOX *bbox);
int bboxCollBoxTest(BBOX *bbox, int x, int y, int w, int h, unsigned int *list, unsigned int listlen);


#endif
