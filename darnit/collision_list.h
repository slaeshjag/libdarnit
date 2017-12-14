/*
Copyright (c) 2015 Steven Arnow <s@rdw.se>
'collision_list.h' - This file is part of libdarnit

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

#ifndef __COLLISION_LIST_H__
#define	__COLLISION_LIST_H__

enum CollisionEntryType {
	COLLISION_TYPE_TRIANGLE,
	COLLISION_TYPE_CIRCLE,
};

enum CollisionEntryIndexType {
	COLLISION_TYPE_KEY,
	COLLISION_TYPE_RAW_INDEX,
};

enum CollisionSortMode {
	COLLISION_SORT_MODE_X,
	COLLISION_SORT_MODE_Y,
};

struct CollisionCoordinate {
	int				x;
	int				y;
};

struct CollisionElement {
	struct CollisionCoordinate	shape[4];
	struct CollisionCoordinate	origin;
	struct CollisionCoordinate	rotated_shape[4];

	enum CollisionEntryType		type;
	int				angle;
	int				used;
	int				key;

	/* Bounding box test for quicker lookup */
	int				x;
	int				y;
	int				xb;
	int				yb;
	int				w;
	int				h;
};

struct CollisionList {
	enum CollisionEntryIndexType	itype;
	enum CollisionSortMode		mode;
	struct CollisionElement		*list;
	int				listlen;
	int				key_cnt;
	int				needs_sorting;
};

#endif
