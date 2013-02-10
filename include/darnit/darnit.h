/*
Copyright (c) 2011-2013 Steven Arnow
'darnit.h' - This file is part of libdarnit

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


#ifndef __DARNIT_H__
#define	__DARNIT_H__

#ifdef __cplusplus
extern "C" {
#endif


#define		DARNIT_PFORMAT_RGBA8		0
#define		DARNIT_PFORMAT_RGBA4		1
#define		DARNIT_PFORMAT_RGB5A1		2
#define		DARNIT_PFORMAT_ALHPA		3



#include <darnit/darnit_main.h>
#include <darnit/darnit_input.h>
#include <darnit/darnit_sound.h>
#include <darnit/darnit_render.h>
#include <darnit/darnit_sprite.h>
#include <darnit/darnit_mtsprite.h>
#include <darnit/darnit_text.h>
#include <darnit/darnit_menutk.h>
#include <darnit/darnit_tilemap.h>
#include <darnit/darnit_bbox.h>
#include <darnit/darnit_dynlib.h>
#include <darnit/darnit_socket.h>
#include <darnit/darnit_utf8.h>
#include <darnit/darnit_util.h>
#include <darnit/darnit_stringtable.h>
#include <darnit/darnit_filesystem.h>
#include <darnit/darnit_map.h>


#ifdef __cplusplus
}
#endif

#endif
