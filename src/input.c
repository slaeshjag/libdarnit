#include "darnit.h"


void inputKeymapReset(void *handle) {
	DARNIT *m = handle;

	m->input.map.up = SDLK_UP;
	m->input.map.down = SDLK_DOWN;
	m->input.map.left = SDLK_LEFT;
	m->input.map.right = SDLK_RIGHT;
	#ifdef PANDORA
	m->input.map.x = SDLK_PAGEDOWN;
	m->input.map.y = SDLK_PAGEUP;
	m->input.map.a = SDLK_HOME;
	m->input.map.b = SDLK_END;
	m->input.map.start = SDLK_LALT;
	m->input.map.select = SDLK_LCTRL;
	m->input.map.l = SDLK_RSHIFT;
	m->input.map.r = SDLK_RCTRL;
	#else
	m->input.map.x = SDLK_s;
	m->input.map.y = SDLK_w;
	m->input.map.a = SDLK_a;
	m->input.map.b = SDLK_d;
	m->input.map.l = SDLK_q;
	m->input.map.r = SDLK_e;
	m->input.map.start = SDLK_RETURN;
	m->input.map.select = SDLK_RSHIFT;
	#endif

	m->input.key = 0;

	return;
}


int inputInitJoystick(void *handle) {
	DARNIT *m = handle;
	int i;

	m->input.js.nub0 = m->input.js.nub1 = NULL;
	m->input.js.nub0i = m->input.js.nub1i = -1;
	m->input.js.nub0_x = m->input.js.nub0_y = m->input.js.nub1_x = m->input.js.nub1_y = 0;

	for (i = 0; i < SDL_NumJoysticks(); i++) {
		if (strcmp("nub0", SDL_JoystickName(i)) == 0) {
			m->input.js.nub0 = SDL_JoystickOpen(i);
			m->input.js.nub0i = i;
		} else if (strcmp("nub1", SDL_JoystickName(i)) == 0) {
			m->input.js.nub1 = SDL_JoystickOpen(i);
			m->input.js.nub1i = i;
		}
	}

	if (m->input.js.nub0 != NULL || m->input.js.nub1 != NULL)
		SDL_JoystickEventState(SDL_ENABLE);
	
	return 0;
}
		


int inputInit(void *handle) {
	DARNIT *m = handle;

	m->input.key = 0;
	m->input.keypending = 0;
	m->input.upper = 0;
	m->input.lastkey = 0;
	m->input.mouse.x = m->input.mouse.y = m->input.mouse.wheel = 0;

	inputKeymapReset(m);
	inputInitJoystick(m);

	return 0;
}


void inputPoll(void *handle) {
	DARNIT *m = handle;
	
	while (SDL_PollEvent(&m->input.event)) {
		if (m->input.event.type == SDL_KEYDOWN) {
			if (m->input.event.key.keysym.sym == m->input.map.up)
				m->input.key |= KEY_UP;
			else if (m->input.event.key.keysym.sym == m->input.map.down)
				m->input.key |= KEY_DOWN;
			else if (m->input.event.key.keysym.sym == m->input.map.left)
				m->input.key |= KEY_LEFT;
			else if (m->input.event.key.keysym.sym == m->input.map.right)
				m->input.key |= KEY_RIGHT;
			else if (m->input.event.key.keysym.sym == m->input.map.x)
				m->input.key |= KEY_X;
			else if (m->input.event.key.keysym.sym == m->input.map.y)
				m->input.key |= KEY_Y;
			else if (m->input.event.key.keysym.sym == m->input.map.a)
				m->input.key |= KEY_A;
			else if (m->input.event.key.keysym.sym == m->input.map.b)
				m->input.key |= KEY_B;
			else if (m->input.event.key.keysym.sym == m->input.map.start)
				m->input.key |= KEY_START;
			else if (m->input.event.key.keysym.sym == m->input.map.select)
				m->input.key |= KEY_SELECT;
			else if (m->input.event.key.keysym.sym == m->input.map.l)
				m->input.key |= KEY_L;
			else if (m->input.event.key.keysym.sym == m->input.map.r)
				m->input.key |= KEY_R;
			if (m->input.event.key.keysym.sym == SDLK_LSHIFT)
				m->input.upper |= 2;
			else if (m->input.event.key.keysym.sym == SDLK_RSHIFT)
				m->input.upper |= 1;
			if (m->input.event.key.keysym.sym < 0x80)	/* ASCII */
				m->input.lastkey = m->input.event.key.keysym.sym;

		} else if (m->input.event.type == SDL_KEYUP) {
			if (m->input.event.key.keysym.sym == m->input.map.up) {
				m->input.key |= KEY_UP;
				m->input.key ^= KEY_UP;
			} else if (m->input.event.key.keysym.sym == m->input.map.down) {
				m->input.key |= KEY_DOWN;
				m->input.key ^= KEY_DOWN;
			} else if (m->input.event.key.keysym.sym == m->input.map.left) {
				m->input.key |= KEY_LEFT;
				m->input.key ^= KEY_LEFT;
			} else if (m->input.event.key.keysym.sym == m->input.map.right) {
				m->input.key |= KEY_RIGHT;
				m->input.key ^= KEY_RIGHT;
			} else if (m->input.event.key.keysym.sym == m->input.map.x) {
				m->input.key |= KEY_X;
				m->input.key ^= KEY_X;
			} else if (m->input.event.key.keysym.sym == m->input.map.y) {
				m->input.key |= KEY_Y;
				m->input.key ^= KEY_Y;
			} else if (m->input.event.key.keysym.sym == m->input.map.a) {
				m->input.key |= KEY_A;
				m->input.key ^= KEY_A;
			} else if (m->input.event.key.keysym.sym == m->input.map.b) {
				m->input.key |= KEY_B;
				m->input.key ^= KEY_B;
			} else if (m->input.event.key.keysym.sym == m->input.map.start) {
				m->input.key |= KEY_START;
				m->input.key ^= KEY_START;
			} else if (m->input.event.key.keysym.sym == m->input.map.select) {
				m->input.key |= KEY_SELECT;
				m->input.key ^= KEY_SELECT;
			} else if (m->input.event.key.keysym.sym == m->input.map.l) {
				m->input.key |= KEY_L;
				m->input.key ^= KEY_L;
			} else if (m->input.event.key.keysym.sym == m->input.map.r) {
				m->input.key |= KEY_R;
				m->input.key ^= KEY_R;
			}

			if (m->input.event.key.keysym.sym == SDLK_LSHIFT) {
				m->input.upper |= 2;
				m->input.upper ^= 2;
			} else if (m->input.event.key.keysym.sym == SDLK_RSHIFT) {
				m->input.upper |= 1;
				m->input.upper ^= 1;
			}
		} else if (m->input.event.type == SDL_MOUSEMOTION) {
			m->input.mouse.x = m->input.event.motion.x;
			m->input.mouse.y = m->input.event.motion.y;
		} else if (m->input.event.type == SDL_MOUSEBUTTONDOWN) {
			if (m->input.event.button.button == SDL_BUTTON_LEFT)
				m->input.key |= MB_LEFT;
			else if (m->input.event.button.button == SDL_BUTTON_RIGHT)
				m->input.key |= MB_RIGHT;
			else if (m->input.event.button.button == SDL_BUTTON_WHEELUP)
				m->input.mouse.wheel--;
			else if (m->input.event.button.button == SDL_BUTTON_WHEELDOWN)
				m->input.mouse.wheel++;
		} else if (m->input.event.type == SDL_MOUSEBUTTONUP) {
			if (m->input.event.button.button == SDL_BUTTON_LEFT) {
				m->input.key |= MB_LEFT;
				m->input.key ^= MB_LEFT;
			} else if (m->input.event.button.button == SDL_BUTTON_RIGHT) {
				m->input.key |= MB_RIGHT;
				m->input.key ^= MB_RIGHT;
			}
		} else if (m->input.event.type == SDL_JOYAXISMOTION) {
			if (m->input.event.jaxis.which == m->input.js.nub0i) {
				if (m->input.event.jaxis.axis == 0)
					m->input.js.nub0_x = m->input.event.jaxis.value;
				else
					m->input.js.nub0_y = m->input.event.jaxis.value;
			} else {
				if (m->input.event.jaxis.axis == 0)
					m->input.js.nub1_x = m->input.event.jaxis.value;
				else
					m->input.js.nub1_y = m->input.event.jaxis.value;
			}
		}  else if (m->input.event.type == SDL_QUIT)
			exit(0);
	}

	m->input.keypending &= m->input.key;
	
	return;
}



unsigned int inputASCIIPop(void *handle) {
	unsigned int key;
	DARNIT *m = handle;

	key = m->input.lastkey;
	if (key >= 'a' && key <= 'z' && m->input.upper)
		key -= ('a' - 'A');
	m->input.lastkey = 0;

	return key;
}
