/*
Copyright (c) 2013 Steven Arnow
'event_glfw.c' - This file is part of libdarnit_tpw

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

#include "../main.h"



void tpw_event_callback_mousebtn(int button, int event) {
	TPW_EVENT event_s;

	memset(&event_s, 0, sizeof(event_s));
	
	event_s.type = (event == GLFW_PRESS) ? TPW_EVENT_TYPE_MOUSEBTN_DOWN : TPW_EVENT_TYPE_MOUSEBTN_UP;
	event_s.mouse.button = button;

	tpw_event_push(event_s);
	return;
}


void tpw_event_callback_mousepos(int x, int y) {
	TPW_EVENT event;

	memset(&event, 0, sizeof(event));

	event.type = TPW_EVENT_TYPE_MOUSEMOVE;
	event.mouse.x = x;
	event.mouse.y = y;
	
	tpw_event_push(event);
	return;
}



void tpw_event_callback_unicode(int unicode, int event) {
	TPW_EVENT event_s;

	memset(&event_s, 0, sizeof(event_s));

	event_s.type = (event == GLFW_PRESS) ? TPW_EVENT_TYPE_KEYDOWN : TPW_EVENT_TYPE_KEYUP;
	event_s.key.unicode = unicode;
	event_s.key.keysym = 0;

	tpw_event_push(event_s);
	return;
}


void tpw_event_callback(int keysym, int event) {
	TPW_EVENT event_s;

	memset(&event_s, 0, sizeof(event_s));
	
	event_s.type = (event == GLFW_PRESS) ? TPW_EVENT_TYPE_KEYDOWN : TPW_EVENT_TYPE_KEYUP;
	event_s.key.unicode = 0;
	event_s.key.keysym = tpw_event_keysym_convert(keysym);

	tpw_event_push(event_s);
	return;
}


void tpw_event_loop() {
	TPW_EVENT event;

	memset(&event, 0, sizeof(event));
	#if 0

	event.key.unicode = event_sdl.key.keysym.unicode = 0;
	while (SDL_PollEvent(&event_sdl)) {
		switch (event_sdl.type) {
			case SDL_JOYAXISMOTION:
				event.type = TPW_EVENT_TYPE_JOYSTICK_MOVE;
				event.joystick.value = event_sdl.jaxis.value;
				event.joystick.axis = event_sdl.jaxis.axis;
				event.joystick.js_id = event_sdl.jaxis.which;
				break;
			case SDL_ACTIVEEVENT:
				event.type = TPW_EVENT_TYPE_ACTIVE;
				event.active.gain = event_sdl.active.gain;
				event.active.app_active = (event_sdl.active.type == SDL_APPACTIVE);
				break;
			case SDL_QUIT:
				event.type = TPW_EVENT_TYPE_QUIT;
				break;
			default:
				break;
		}	

		tpw_event_push(event);
		event.key.unicode = event_sdl.key.keysym.unicode = 0;
	}

	#endif

	return;
}


TPW_JOYSTICK *tpw_joystick_open(int js_id) {
//	return SDL_JoystickOpen(js_id);
	return NULL;
}


const char *tpw_joystick_name(int js_id) {
//	return SDL_JoystickName(js_id);
	return NULL;
}


int tpw_joystick_num() {
	return 0;
//	return SDL_NumJoysticks();
}


void tpw_joystick_enable(TPW_ENBOOL enable) {
	return;
//	SDL_JoystickEventState((enable == TPW_ENBOOL_ENABLE) ? SDL_ENABLE : SDL_DISABLE);
	
	return;
}


unsigned int tpw_keys_modifiers() {
//	return (unsigned int) SDL_GetModState();
}
