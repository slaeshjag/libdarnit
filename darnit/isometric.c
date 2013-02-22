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


//void isometricToScreen(int x, int y, int o_x, int o_y, int w, int h, int *res_x, int *res_y) {
	
