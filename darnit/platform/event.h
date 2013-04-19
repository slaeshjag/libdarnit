/*
Copyright (c) 2013 Steven Arnow
'event.h' - This file is part of libdarnit_tpw

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


#ifndef __PLATFORM_INPUT_H__
#define	__PLATFORM_INPUT_H__

#ifdef TPW_INTERNAL
#ifdef PLATFORM_SDL
#include "sdl/event_sdl.h"
#endif
#endif

#define	TPW_FIFO_NEXT(cur, max)		(((cur) + 1 == (max)) ? 0 : (cur + 1))


typedef enum {
	TPW_EVENT_TYPE_KEYDOWN,
	TPW_EVENT_TYPE_KEYUP,
	TPW_EVENT_TYPE_MOUSEMOVE,
	TPW_EVENT_TYPE_MOUSEBTN_UP,
	TPW_EVENT_TYPE_MOUSEBTN_DOWN,
	TPW_EVENT_TYPE_JOYSTICK_MOVE,
	TPW_EVENT_QUIT,
} TPW_EVENT_TYPE;


typedef struct {
	unsigned int			keysym;
	unsigned int			unicode;
} TPW_EVENT_KEY;


typedef struct {
	int				x;
	int				y;
	unsigned int			button;
} TPW_EVENT_MOUSE;


typedef struct {
	int				js_x;
	int				js_y;
	int				js_id;
	int				button;
} TPW_EVENT_JOYSTICK;

typedef struct {
	int				gain;
	int				app_active;
} TPW_EVENT_WIN_ACTIVE;

typedef struct {
	TPW_EVENT_TYPE			type;
	union {
		TPW_EVENT_KEY		key;
		TPW_EVENT_MOUSE		mouse;
		TPW_EVENT_JOYSTICK	joystick;
		TPW_EVENT_WIN_ACTIVE	active;
	};
} TPW_EVENT;


typedef struct {
	TPW_EVENT			*event;
	int				event_max;
	int				event_read;
	int				event_write;
} TPW_EVENT_S;

int tpw_event_init(int queue_size);
void tpw_event_push(TPW_EVENT event);
int tpw_event_pop(TPW_EVENT *event);

void tpw_event_loop();

#endif
