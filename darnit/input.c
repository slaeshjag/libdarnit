/*
Copyright (c) 2011-2013 Steven Arnow
'input.c' - This file is part of libdarnit

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


#include "darnit.h"

#ifndef DARNIT_HEADLESS

void inputRawPush(int sym, int action, int unicode) {
	if (d->input.raw.use == RAW_BUFFER_LEN)
		return;
	d->input.raw.raw[d->input.raw.use].keysym = sym;
	d->input.raw.raw[d->input.raw.use].action = action;
	d->input.raw.raw[d->input.raw.use].unicode = unicode;
	d->input.raw.use++;

	return;
}


INPUT_RAW_KEY inputRawPop() {
	INPUT_RAW_KEY key;

	key.keysym = key.action = key.unicode = 0;
	if (d->input.raw.use == 0)
		return key;
	d->input.raw.use--;

	return d->input.raw.raw[d->input.raw.use];
}


void inputKeymapReset() {
	d->input.map.up = TPW_KEY_UP;
	d->input.map.down = TPW_KEY_DOWN;
	d->input.map.left = TPW_KEY_LEFT;
	d->input.map.right = TPW_KEY_RIGHT;
	#ifdef PANDORA
	d->input.map.x = TPW_KEY_PAGEDOWN;
	d->input.map.y = TPW_KEY_PAGEUP;
	d->input.map.a = TPW_KEY_HOME;
	d->input.map.b = TPW_KEY_END;
	d->input.map.start = TPW_KEY_LALT;
	d->input.map.select = TPW_KEY_LCTRL;
	d->input.map.l = TPW_KEY_RSHIFT;
	d->input.map.r = TPW_KEY_RCTRL;
	#elif defined(MAEMO)
	d->input.map.x = TPW_KEY_s;
	d->input.map.y = TPW_KEY_w;
	d->input.map.a = TPW_KEY_a;
	d->input.map.b = TPW_KEY_d;
	d->input.map.start = TPW_KEY_KP_ENTER;
	d->input.map.select = TPW_KEY_BACKSPACE;
	d->input.map.l = TPW_KEY_q;
	d->input.map.r = TPW_KEY_e;
	#elif defined(GCW_ZERO)
	d->input.map.x = TPW_KEY_LSHIFT;
	d->input.map.y = TPW_KEY_SPACE;
	d->input.map.a = TPW_KEY_LCTRL;
	d->input.map.b = TPW_KEY_LALT;
	d->input.map.start = TPW_KEY_ENTER;
	d->input.map.select = TPW_KEY_ESCAPE;
	d->input.map.l = TPW_KEY_TAB;
	d->input.map.r = TPW_KEY_BACKSPACE;
	#else
	d->input.map.x = TPW_KEY_s;
	d->input.map.y = TPW_KEY_w;
	d->input.map.a = TPW_KEY_LCTRL;
	d->input.map.b = TPW_KEY_LALT;
	d->input.map.l = TPW_KEY_a;
	d->input.map.r = TPW_KEY_d;
	d->input.map.start = TPW_KEY_RETURN;
	d->input.map.select = TPW_KEY_ESCAPE;
	#endif

	d->input.key = 0;

	return;
}


int inputInitJoystick() {
	int i, any;
	const char *js0="";

	any = 0;

	if (d->platform.platform & DARNIT_PLATFORM_PANDORA)
		js0 = "nub0";
	else if (d->platform.platform & DARNIT_PLATFORM_GCWZERO)
		js0 = "analog joystick";
	else
		any = 1;

	d->input.js.nub0 = d->input.js.nub1 = NULL;
	d->input.js.nub0i = d->input.js.nub1i = -1;
	d->input.js.nub0_x = d->input.js.nub0_y = d->input.js.nub1_x = d->input.js.nub1_y = 0;

	
	for (i = 0; i < tpw_joystick_num(); i++) {
		if (!any) {
			if (strcmp(js0, tpw_joystick_name(i)) == 0) {
				d->input.js.nub0 = tpw_joystick_open(i);
				d->input.js.nub0i = i;
			} else if (strcmp("nub1", tpw_joystick_name(i)) == 0) {
				d->input.js.nub1 = tpw_joystick_open(i);
				d->input.js.nub1i = i;
			}
		} else {
			if (any == 1) {
				d->input.js.nub0 = tpw_joystick_open(i);
				d->input.js.nub0i = i;
				any++;
			} else if (any == 2) {
				d->input.js.nub1 = tpw_joystick_open(i);
				d->input.js.nub1i = i;
				any++;
			}
		}
	}

	if (d->input.js.nub0 != NULL || d->input.js.nub1 != NULL)
		tpw_joystick_enable(TPW_ENBOOL_ENABLE);	
	return 0;
}
		


int inputInit() {
	d->input.key = 0;
	d->input.keypending = 0;
	d->input.upper = 0;
	d->input.lastkey = 0;
	d->input.raw.use = 0;
	d->input.key = 0;
	d->input.mouse.x = d->input.mouse.y = d->input.mouse.wheel = d->input.mouse.mb_l = d->input.mouse.mb_r = 0;
	
	inputKeymapReset();
	inputInitJoystick();

	return 0;
}


void inputPoll() {
	TPW_EVENT event;

	tpw_event_loop();

	while (tpw_event_pop(&event)) {
		if (event.type == TPW_EVENT_TYPE_KEYDOWN) {
			if (event.key.keysym == d->input.map.up)
				d->input.key |= KEY_UP;
			else if (event.key.keysym == d->input.map.down)
				d->input.key |= KEY_DOWN;
			else if (event.key.keysym == d->input.map.left)
				d->input.key |= KEY_LEFT;
			else if (event.key.keysym == d->input.map.right)
				d->input.key |= KEY_RIGHT;
			else if (event.key.keysym == d->input.map.x)
				d->input.key |= KEY_X;
			else if (event.key.keysym == d->input.map.y)
				d->input.key |= KEY_Y;
			else if (event.key.keysym == d->input.map.a)
				d->input.key |= KEY_A;
			else if (event.key.keysym == d->input.map.b)
				d->input.key |= KEY_B;
			else if (event.key.keysym == d->input.map.start)
				d->input.key |= KEY_START;
			else if (event.key.keysym == d->input.map.select)
				d->input.key |= KEY_SELECT;
			else if (event.key.keysym == d->input.map.l)
				d->input.key |= KEY_L;
			else if (event.key.keysym == d->input.map.r)
				d->input.key |= KEY_R;
			/* I'm sorry Sara... */
			#ifdef PANDORA
			else if (event.key.keysym == TPW_KEY_ESCAPE)
				d_quit();
			#elif defined(MAEMO)
			else if (event.key.keysym == TPW_KEY_BACKSPACE && (tpw_keys_modifiers&TPW_KEY_MOD_CTRL))
				tpw_input_grab(TPW_INPUT_GRAB_GRAB);
			#endif
			if (event.key.keysym == TPW_KEY_LSHIFT)
				d->input.upper |= 2;
			else if (event.key.keysym == TPW_KEY_RSHIFT)
				d->input.upper |= 1;
			if (event.key.keysym < 0x80)	/* ASCII */
				d->input.lastkey = event.key.keysym;
			inputRawPush(event.key.keysym, DARNIT_KEYACTION_PRESS, event.key.unicode);

		} else if (event.type == TPW_EVENT_TYPE_KEYUP) {
			if (event.key.keysym == d->input.map.up) {
				d->input.key |= KEY_UP;
				d->input.key ^= KEY_UP;
			} else if (event.key.keysym == d->input.map.down) {
				d->input.key |= KEY_DOWN;
				d->input.key ^= KEY_DOWN;
			} else if (event.key.keysym == d->input.map.left) {
				d->input.key |= KEY_LEFT;
				d->input.key ^= KEY_LEFT;
			} else if (event.key.keysym == d->input.map.right) {
				d->input.key |= KEY_RIGHT;
				d->input.key ^= KEY_RIGHT;
			} else if (event.key.keysym == d->input.map.x) {
				d->input.key |= KEY_X;
				d->input.key ^= KEY_X;
			} else if (event.key.keysym == d->input.map.y) {
				d->input.key |= KEY_Y;
				d->input.key ^= KEY_Y;
			} else if (event.key.keysym == d->input.map.a) {
				d->input.key |= KEY_A;
				d->input.key ^= KEY_A;
			} else if (event.key.keysym == d->input.map.b) {
				d->input.key |= KEY_B;
				d->input.key ^= KEY_B;
			} else if (event.key.keysym == d->input.map.start) {
				d->input.key |= KEY_START;
				d->input.key ^= KEY_START;
			} else if (event.key.keysym == d->input.map.select) {
				d->input.key |= KEY_SELECT;
				d->input.key ^= KEY_SELECT;
			} else if (event.key.keysym == d->input.map.l) {
				d->input.key |= KEY_L;
				d->input.key ^= KEY_L;
			} else if (event.key.keysym == d->input.map.r) {
				d->input.key |= KEY_R;
				d->input.key ^= KEY_R;
			}

			if (event.key.keysym == TPW_KEY_LSHIFT) {
				d->input.upper |= 2;
				d->input.upper ^= 2;
			} else if (event.key.keysym == TPW_KEY_RSHIFT) {
				d->input.upper |= 1;
				d->input.upper ^= 1;
			}
			inputRawPush(event.key.keysym, DARNIT_KEYACTION_RELEASE, event.key.unicode);
			
		} else if (event.type == TPW_EVENT_TYPE_MOUSEMOVE) {
			d->input.mouse.x = event.mouse.x;
			d->input.mouse.y = event.mouse.y;
		} else if (event.type == TPW_EVENT_TYPE_MOUSEBTN_DOWN) {
			if (event.mouse.button == TPW_MOUSE_BUTTON_LEFT) {
				d->input.key |= MB_LEFT;
				d->input.mouse.mb_l = 1;
			} else if (event.mouse.button == TPW_MOUSE_BUTTON_RIGHT) {
				d->input.key |= MB_RIGHT_D;
				d->input.mouse.mb_r = 1;
			} else if (event.mouse.button == TPW_MOUSE_BUTTON_WHEEL_UP)
				d->input.mouse.wheel--;
			else if (event.mouse.button == TPW_MOUSE_BUTTON_WHEEL_DOWN)
				d->input.mouse.wheel++;
		} else if (event.type == TPW_EVENT_TYPE_MOUSEBTN_UP) {
			if (event.mouse.button == TPW_MOUSE_BUTTON_LEFT) {
				d->input.key |= MB_LEFT;
				d->input.key ^= MB_LEFT;
				d->input.mouse.mb_l = 0;
			} else if (event.mouse.button == TPW_MOUSE_BUTTON_RIGHT) {
				d->input.key |= MB_RIGHT_D;
				d->input.key ^= MB_RIGHT_D;
				d->input.mouse.mb_r = 0;
			}
		} else if (event.type == TPW_EVENT_TYPE_JOYSTICK_MOVE) {
			if (event.joystick.js_id == d->input.js.nub0i) {
				if (event.joystick.axis == 0)
					d->input.js.nub0_x = event.joystick.value;
				else
					d->input.js.nub0_y = event.joystick.value;
			} else {
				if (event.joystick.axis == 0)
					d->input.js.nub1_x = event.joystick.value;
				else
					d->input.js.nub1_y = event.joystick.value;
			}
		#ifdef MAEMO
		} else if (event.type == TPW_EVENT_TYPE_ACTIVE && event.active.gain == 1 && event.active.app_active) {
			tpw_input_grab(TPW_INPUT_GRAB_GRAB);
		#endif
		}  else if (event.type == TPW_EVENT_TYPE_QUIT)
			exit(0);
	}

	d->input.keypending &= d->input.key;

	d->input.key |= ((tpw_keys_modifiers()) << 16);
	
	return;
}



unsigned int inputASCIIPop() {
	unsigned int key;
	
	key = d->input.lastkey;
	if (key >= 'a' && key <= 'z' && d->input.upper)
		key -= ('a' - 'A');
	d->input.lastkey = 0;

	return key;
}

// DARNIT_HEADLESS
#endif
