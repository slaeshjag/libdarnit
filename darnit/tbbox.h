/*
Copyright (c) 2014 Steven Arnow <s@rdw.se>
'darnit/tbbox.h' - This file is part of libdarnit

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

#ifndef __TBBOX_H__
#define	__TBBOX_H__

enum TBBOX_SORTMODE {
	TBBOX_SORTMODE_X,
	TBBOX_SORTMODE_Y,
};

enum tbbox_mode {
	TBBOX_MODE_NOT_USED,
	TBBOX_MODE_CIRCLE,
	TBBOX_MODE_RECTANGLE,
};


typedef struct {
	int			point_x;
	int			point_y;
	int			radius;
} TBBOX_ENTRY_CIRCLE;


typedef struct {
	int			id;
	enum tbbox_mode		mode;
	int			pos_low_x;
	int			pos_low_y;
	int			pos_high_x;
	int			pos_high_y;
	TBBOX_ENTRY_CIRCLE	circ;
} TBBOX_ENTRY;


typedef struct {
	int			size;
	int			used;
	TBBOX_ENTRY		*entry;
	int			entries;
	int			*lookup;
} TBBOX;


#endif
