/*
Copyright (c) 2013 Steven Arnow
'event_win32.h' - This file is part of libdarnit_tpw

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

#ifndef __TPW_WIN32_EVENT_H__
#define	__TPW_WIN32_EVENT_H__

#include <windows.h>
LRESULT CALLBACK tpw_message_process(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
MSG msg;

extern unsigned short vk_translate[256];


#endif

