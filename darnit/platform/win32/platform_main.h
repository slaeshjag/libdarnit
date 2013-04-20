/*
Copyright (c) 2013 Steven Arnow
'main_dummy.h' - This file is part of libdarnit_tpw

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

#ifndef __PLATFORM_DUMMY_MAIN_H__
#define	__PLATFORM_DUMMY_MAIN_H__

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>

#define	WINDOW_CLASS_NAME	"TPW window"

typedef struct {
	TPW_COMMON		common;
	HMODULE			hInstance;
	HWND			hWnd;
	HDC			hDC;
	HGLRC			hRC;
	int			fullscreen;

	/* Event stuffs */
	unsigned int		unicode_key;
	char			keys[256];
	unsigned int		modifiers;
	unsigned int		hide_cursor;

	/* Screen resolution */
	TPW_RECT		**screen_res;
} TPW;


#endif
