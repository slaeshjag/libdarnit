/*
Copyright (c) 2013 Steven Arnow
'isometric.c' - This file is part of libdarnit

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


#include <stdlib.h>
#include <stdio.h>

void isometricFromScreen(int s_x, int s_y, int o_x, int o_y, int w, int h, int *iso_x, int *iso_y) {
	/*	This is the transformation matrix I use

*	/	h	w	-o_x * h - o_y * w	\*         1
*	|*	-h	w	o_x * h - o_y * w	|   × -----------
*	\*	0	0	2 * w * h		/      2 * w * h
*/

	/* After this: w is half the tile width, h is half the tile height. */
	int t, div;

	w >>= 1;
	h >>= 1;
	t = o_x * h - o_y * w;
	div = 2 * w * h;

	if (iso_x)
		(*iso_x) = (s_x * h  + s_y * w - t) / div;
	if (iso_y)
		(*iso_y) = (s_x * (-h) + s_y * w + t) / div;
	return;
}


void isometricToScreen(int x, int y, int o_x, int o_y, int w, int h, int *res_x, int *res_y) {
	w >>= 1;
	h >>= 1;

	if (res_x)
		(*res_x) = x * w + y * (-w) + o_x;
	if (res_y)
		(*res_y) = x * h + y * h + o_y;

	return;
}
