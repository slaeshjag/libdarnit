/*
Copyright (c) 2013, 2014 Steven Arnow <s@rdw.se>
'tbbox.c' - This file is part of libdarnit

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


TBBOX *tbboxNew(int size, enum TBBOX_SORTMODE mode) {
	TBBOX *tb;
	int i;

	if (!(tb = malloc(sizeof(*tb))))
		return tb;
	tb->used = 0;
	tb->entries = size;
	tb->entry = malloc(sizeof(*tb->entry) * size);
	tb->lookup = malloc(sizeof(*tb->lookup) * size);

	for (i = 0; i < size; i++)
		tb->entry[i].mode = TBBOX_MODE_NOT_USED, tb->entry[i].id = i, tb->lookup[i] = -1;
	
	return tb;
}


TBBOX *tbboxFree(TBBOX *tb) {
	if (!tb)
		return NULL;
	
	free(tb->entry);
	free(tb->lookup);
	free(tb);

	return NULL;
}


int tbboxFindFreeSlot(TBBOX *tb) {
	int i;

	for (i = 0; i < tb->entries; i++)
		if (tb->entry[i].mode == TBBOX_MODE_NOT_USED)
			return i;
	return -1;
}


int tbboxAddCircle(TBBOX *tb, int x, int y, int radius) {
	int key;

	if (!tb) return -1;
	if ((key = tbboxFindFreeSlot(tb)) < 0)
		return -1;
	tb->used++;
	tb->entry[key].mode = TBBOX_MODE_CIRCLE;
	tb->entry[key].pos_low_x = x - radius;
	tb->entry[key].pos_low_y = y - radius;
	tb->entry[key].pos_high_x = x + radius;
	tb->entry[key].pos_high_y = y + radius;

	tb->entry[key].circ.point_x = x;
	tb->entry[key].circ.point_y = y;
	tb->entry[key].circ.radius = radius;

	tb->lookup[tb->entry[key].id] = key;

	return tb->entry[key].id;
}


int tbboxAddTriangleBox(TBBOX *tb, int x, int y, int w, int h, int angle) {
	int key;

	if (!tb) return -1;
	if ((key = tbboxFindFreeSlot(tb)) < 0)
		return -1;
	tb->used++;
	tb->entry[key].mode = TBBOX_MODE_RECTANGLE;
	tb->entry[key].pos_low_x = x;
	tb->entry[key].pos_low_y = y;
	tb->entry[key].pos_high_x = x + w;
	tb->entry[key].pos_high_y = y + h;

	tb->lookup[tb->entry[key].id] = key;

	return tb->entry[key].id;
}
