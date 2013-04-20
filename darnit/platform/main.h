/*
Copyright (c) 2013 Steven Arnow
'main.h' - This file is part of libdarnit_tpw

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


#ifndef __PLATFORM_MAIN_H__
#define	__PLATFORM_MAIN_H__

typedef enum {
	TPW_INPUT_GRAB_GRAB,
	TPW_INPUT_GRAB_RELEASE
} TPW_INPUT_GRAB;


typedef enum {
	TPW_ENBOOL_DISABLE = 0,
	TPW_ENBOOL_ENABLE = 1,
} TPW_ENBOOL;


typedef struct {
	signed short	x, y;
	unsigned short	w, h;
} TPW_RECT;

#include "keysym.h"
#include "sound.h"

typedef struct {
	TPW_EVENT_S	event;
} TPW_COMMON;

#include "platform_main.h"
#include "event.h"

int tpw_init();
int tpw_window_create(const char *title, unsigned int window_w, unsigned int window_h, unsigned int fullscreen, const unsigned int bpp);
void tpw_sleep(unsigned int msec);
unsigned int tpw_ticks();
void tpw_input_grab(TPW_INPUT_GRAB grab);
const char *tpw_key_name_get(int sym);
void tpw_render_buffer_swap();
TPW_RECT **tpw_videomodes_list();
void tpw_input_unicode(int enable);
void tpw_cursor_show(unsigned int show);
int tpw_icon_set(void *pixdata_rgba32, int w, int h);
void tpw_quit();

extern TPW tpw;

#endif
