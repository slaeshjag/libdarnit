/*
Copyright (c) 2013 Steven Arnow <s@rdw.se>
'darnit_collision.c' - This file is part of libDarnit

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

int EXPORT_THIS d_coll_circle(int x1, int y1, int x2, int y2, int rad_1, int rad_2) {
	return collisionCircleTest(x1, y1, x2, y2, rad_1, rad_2);
}
