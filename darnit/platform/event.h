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

#include "platform_event.h"
#include "threads.h"

/* Modifier keys */
#define	TPW_KEY_MOD_LSHIFT		0x0001
#define	TPW_KEY_MOD_RSHIFT		0x0002
#define	TPW_KEY_MOD_LCTRL		0x0004
#define	TPW_KEY_MOD_RCTRL		0x0008
#define	TPW_KEY_MOD_LALT		0x0010
#define	TPW_KEY_MOD_RALT		0x0020
#define	TPW_KEY_MOD_LMETA		0x0040
#define	TPW_KEY_MOD_RMETA		0x0080

#define	TPW_KEY_MOD_SHIFT		0x0003
#define	TPW_KEY_MOD_CTRL		0x000C
#define	TPW_KEY_MOD_ALT			0x0030
#define	TPW_KEY_MOD_META		0x00C0

#define	TPW_FIFO_NEXT(cur, max)		(((cur) + 1 == (max)) ? 0 : (cur + 1))
typedef void TPW_JOYSTICK;


typedef enum {
	TPW_EVENT_TYPE_KEYDOWN,
	TPW_EVENT_TYPE_KEYUP,
	TPW_EVENT_TYPE_MOUSEMOVE,
	TPW_EVENT_TYPE_MOUSEBTN_UP,
	TPW_EVENT_TYPE_MOUSEBTN_DOWN,
	TPW_EVENT_TYPE_JOYSTICK_MOVE,
	TPW_EVENT_TYPE_ACTIVE,
	TPW_EVENT_TYPE_QUIT,
} TPW_EVENT_TYPE;


typedef enum {
	TPW_MOUSE_BUTTON_LEFT		= 1,
	TPW_MOUSE_BUTTON_MIDDLE		= 2,
	TPW_MOUSE_BUTTON_RIGHT		= 3,
	TPW_MOUSE_BUTTON_WHEEL_UP	= 4,
	TPW_MOUSE_BUTTON_WHEEL_DOWN	= 5,
} TPW_MOUSE_BUTTON;


typedef struct {
	unsigned int			keysym;
	unsigned int			unicode;
} TPW_EVENT_KEY;


typedef struct {
	int				x;
	int				y;
	TPW_MOUSE_BUTTON		button;
} TPW_EVENT_MOUSE;


typedef struct {
	int				axis;
	int				value;
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
	TPW_MUTEX			*mutex;
	TPW_EVENT			*event;
	int				event_max;
	int				event_read;
	int				event_write;
} TPW_EVENT_S;

int tpw_event_init(int queue_size);
void tpw_event_push(TPW_EVENT event);
int tpw_event_pop(TPW_EVENT *event);

void tpw_event_loop();
TPW_JOYSTICK *tpw_joystick_open(int js_id);
const char *tpw_joystick_name(int i);
int tpw_joystick_num();
void tpw_joystick_enable(TPW_ENBOOL enable);
unsigned int tpw_keys_modifiers();


#endif
