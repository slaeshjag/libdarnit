/*
Copyright (c) 2011-2013 Steven Arnow
'tilemap.h' - This file is part of libdarnit

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


#ifndef __TILEMAP_H__
#define	__TILEMAP_H__

#define	TILEMAP_DEFAULT_INV_DIV		0xFFF


typedef struct {
	int			w;
	int			h;
	unsigned int		*data;
	RENDER_TILEMAP		*render;
} TILEMAP_ENTRY;


TILEMAP_ENTRY *tilemapNew(int invs_div, void *tilesheet, unsigned int mask, int w, int h, int iso);
TILEMAP_ENTRY *tilemapLoad(const char *fname, int invs_div, void *tilesheet, unsigned int mask, int iso);
void *tilemapFree(TILEMAP_ENTRY *tm);


#endif
