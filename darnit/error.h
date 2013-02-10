/*
Copyright (c) 2011-2013 Steven Arnow
'error.h' - This file is part of libdarnit

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


#ifndef __ERROR_H__
#define __ERROR_H__

#define		FILE_OPEN_ERROR		fprintf(stderr, "Unable to open %s\n", fname);
#define		MALLOC_ERROR		fprintf(stderr, "Unable to malloc, probably out of RAM\n");
#define		SPRITE_ERROR		fprintf(stderr, "Unable to allocate a sprite\n");
#define		NPC_ERROR		fprintf(stderr, "Unable to allocate a npc\n");

#endif
