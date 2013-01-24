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
	d->input.map.up = SDLK_UP;
	d->input.map.down = SDLK_DOWN;
	d->input.map.left = SDLK_LEFT;
	d->input.map.right = SDLK_RIGHT;
	#ifdef PANDORA
	d->input.map.x = SDLK_PAGEDOWN;
	d->input.map.y = SDLK_PAGEUP;
	d->input.map.a = SDLK_HOME;
	d->input.map.b = SDLK_END;
	d->input.map.start = SDLK_LALT;
	d->input.map.select = SDLK_LCTRL;
	d->input.map.l = SDLK_RSHIFT;
	d->input.map.r = SDLK_RCTRL;
	#elif defined(MAEMO)
	d->input.map.x = SDLK_s;
	d->input.map.y = SDLK_w;
	d->input.map.a = SDLK_a;
	d->input.map.b = SDLK_d;
	d->input.map.start = SDLK_KP_ENTER;
	d->input.map.select = SDLK_BACKSPACE;
	d->input.map.l = SDLK_q;
	d->input.map.r = SDLK_e;
	#elif defined(GCW_ZERO)
	d->input.map.x = SDLK_LSHIFT;
	d->input.map.y = SDLK_SPACE;
	d->input.map.a = SDLK_LCTRL;
	d->input.map.b = SDLK_LALT;
	d->input.map.start = SDLK_ENTER;
	d->input.map.select = SDLK_ESCAPE;
	d->input.map.l = SDLK_TAB;
	d->input.map.r = SDLK_BACKSPACE;
	#else
	d->input.map.x = SDLK_s;
	d->input.map.y = SDLK_w;
	d->input.map.a = SDLK_LCTRL;
	d->input.map.b = SDLK_LALT;
	d->input.map.l = SDLK_a;
	d->input.map.r = SDLK_d;
	d->input.map.start = SDLK_RETURN;
	d->input.map.select = SDLK_ESCAPE;
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

	
	for (i = 0; i < SDL_NumJoysticks(); i++) {
		if (!any) {
			if (strcmp(js0, SDL_JoystickName(i)) == 0) {
				d->input.js.nub0 = SDL_JoystickOpen(i);
				d->input.js.nub0i = i;
			} else if (strcmp("nub1", SDL_JoystickName(i)) == 0) {
				d->input.js.nub1 = SDL_JoystickOpen(i);
				d->input.js.nub1i = i;
			}
		} else {
			if (any == 1) {
				d->input.js.nub0 = SDL_JoystickOpen(i);
				d->input.js.nub0i = i;
				any++;
			} else if (any == 2) {
				d->input.js.nub1 = SDL_JoystickOpen(i);
				d->input.js.nub1i = i;
				any++;
			}
		}
	}

	if (d->input.js.nub0 != NULL || d->input.js.nub1 != NULL)
		SDL_JoystickEventState(SDL_ENABLE);
	
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

	memset(&d->input.event, 0, sizeof(d->input.event));

	inputKeymapReset();
	inputInitJoystick();

	return 0;
}


void inputPoll() {
	while (SDL_PollEvent(&d->input.event)) {
		if (d->input.event.type == SDL_KEYDOWN) {
			if (d->input.event.key.keysym.sym == d->input.map.up)
				d->input.key |= KEY_UP;
			else if (d->input.event.key.keysym.sym == d->input.map.down)
				d->input.key |= KEY_DOWN;
			else if (d->input.event.key.keysym.sym == d->input.map.left)
				d->input.key |= KEY_LEFT;
			else if (d->input.event.key.keysym.sym == d->input.map.right)
				d->input.key |= KEY_RIGHT;
			else if (d->input.event.key.keysym.sym == d->input.map.x)
				d->input.key |= KEY_X;
			else if (d->input.event.key.keysym.sym == d->input.map.y)
				d->input.key |= KEY_Y;
			else if (d->input.event.key.keysym.sym == d->input.map.a)
				d->input.key |= KEY_A;
			else if (d->input.event.key.keysym.sym == d->input.map.b)
				d->input.key |= KEY_B;
			else if (d->input.event.key.keysym.sym == d->input.map.start)
				d->input.key |= KEY_START;
			else if (d->input.event.key.keysym.sym == d->input.map.select)
				d->input.key |= KEY_SELECT;
			else if (d->input.event.key.keysym.sym == d->input.map.l)
				d->input.key |= KEY_L;
			else if (d->input.event.key.keysym.sym == d->input.map.r)
				d->input.key |= KEY_R;
			/* I'm sorry Sara... */
			#ifdef PANDORA
			else if (d->input.event.key.keysym.sym == SDLK_ESCAPE)
				d_quit();
			#elif defined(MAEMO)
			else if (d->input.event.key.keysym.sym == SDLK_BACKSPACE && (d->input.event.key.keysym.mod&KMOD_CTRL))
				SDL_WM_GrabInput(SDL_GRAB_OFF);
			#endif
			if (d->input.event.key.keysym.sym == SDLK_LSHIFT)
				d->input.upper |= 2;
			else if (d->input.event.key.keysym.sym == SDLK_RSHIFT)
				d->input.upper |= 1;
			if (d->input.event.key.keysym.sym < 0x80)	/* ASCII */
				d->input.lastkey = d->input.event.key.keysym.sym;
			inputRawPush(d->input.event.key.keysym.sym, DARNIT_KEYACTION_PRESS, d->input.event.key.keysym.unicode);

		} else if (d->input.event.type == SDL_KEYUP) {
			if (d->input.event.key.keysym.sym == d->input.map.up) {
				d->input.key |= KEY_UP;
				d->input.key ^= KEY_UP;
			} else if (d->input.event.key.keysym.sym == d->input.map.down) {
				d->input.key |= KEY_DOWN;
				d->input.key ^= KEY_DOWN;
			} else if (d->input.event.key.keysym.sym == d->input.map.left) {
				d->input.key |= KEY_LEFT;
				d->input.key ^= KEY_LEFT;
			} else if (d->input.event.key.keysym.sym == d->input.map.right) {
				d->input.key |= KEY_RIGHT;
				d->input.key ^= KEY_RIGHT;
			} else if (d->input.event.key.keysym.sym == d->input.map.x) {
				d->input.key |= KEY_X;
				d->input.key ^= KEY_X;
			} else if (d->input.event.key.keysym.sym == d->input.map.y) {
				d->input.key |= KEY_Y;
				d->input.key ^= KEY_Y;
			} else if (d->input.event.key.keysym.sym == d->input.map.a) {
				d->input.key |= KEY_A;
				d->input.key ^= KEY_A;
			} else if (d->input.event.key.keysym.sym == d->input.map.b) {
				d->input.key |= KEY_B;
				d->input.key ^= KEY_B;
			} else if (d->input.event.key.keysym.sym == d->input.map.start) {
				d->input.key |= KEY_START;
				d->input.key ^= KEY_START;
			} else if (d->input.event.key.keysym.sym == d->input.map.select) {
				d->input.key |= KEY_SELECT;
				d->input.key ^= KEY_SELECT;
			} else if (d->input.event.key.keysym.sym == d->input.map.l) {
				d->input.key |= KEY_L;
				d->input.key ^= KEY_L;
			} else if (d->input.event.key.keysym.sym == d->input.map.r) {
				d->input.key |= KEY_R;
				d->input.key ^= KEY_R;
			}

			if (d->input.event.key.keysym.sym == SDLK_LSHIFT) {
				d->input.upper |= 2;
				d->input.upper ^= 2;
			} else if (d->input.event.key.keysym.sym == SDLK_RSHIFT) {
				d->input.upper |= 1;
				d->input.upper ^= 1;
			}
			inputRawPush(d->input.event.key.keysym.sym, DARNIT_KEYACTION_RELEASE, d->input.event.key.keysym.unicode);
			
		} else if (d->input.event.type == SDL_MOUSEMOTION) {
			d->input.mouse.x = d->input.event.motion.x;
			d->input.mouse.y = d->input.event.motion.y;
		} else if (d->input.event.type == SDL_MOUSEBUTTONDOWN) {
			if (d->input.event.button.button == SDL_BUTTON_LEFT) {
				d->input.key |= MB_LEFT;
				d->input.mouse.mb_l = 1;
			} else if (d->input.event.button.button == SDL_BUTTON_RIGHT) {
				d->input.key |= MB_RIGHT_D;
				d->input.mouse.mb_r = 1;
			} else if (d->input.event.button.button == SDL_BUTTON_WHEELUP)
				d->input.mouse.wheel--;
			else if (d->input.event.button.button == SDL_BUTTON_WHEELDOWN)
				d->input.mouse.wheel++;
		} else if (d->input.event.type == SDL_MOUSEBUTTONUP) {
			if (d->input.event.button.button == SDL_BUTTON_LEFT) {
				d->input.key |= MB_LEFT;
				d->input.key ^= MB_LEFT;
				d->input.mouse.mb_l = 0;
			} else if (d->input.event.button.button == SDL_BUTTON_RIGHT) {
				d->input.key |= MB_RIGHT_D;
				d->input.key ^= MB_RIGHT_D;
				d->input.mouse.mb_r = 0;
			}
		} else if (d->input.event.type == SDL_JOYAXISMOTION) {
			if (d->input.event.jaxis.which == d->input.js.nub0i) {
				if (d->input.event.jaxis.axis == 0)
					d->input.js.nub0_x = d->input.event.jaxis.value;
				else
					d->input.js.nub0_y = d->input.event.jaxis.value;
			} else {
				if (d->input.event.jaxis.axis == 0)
					d->input.js.nub1_x = d->input.event.jaxis.value;
				else
					d->input.js.nub1_y = d->input.event.jaxis.value;
			}
		#ifdef MAEMO
		} else if (d->input.event.type == SDL_ACTIVEEVENT && d->input.event.active.state == 1) {
			SDL_WM_GrabInput(SDL_GRAB_ON);
		#endif
		}  else if (d->input.event.type == SDL_QUIT)
			exit(0);
	}

	d->input.keypending &= d->input.key;

	d->input.key |= (((unsigned int) SDL_GetModState()) << 16);
	
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
