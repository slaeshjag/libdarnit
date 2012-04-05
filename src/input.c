#include "darner.h"


void inputInit(void *handle) {
	DARNER *m = handle;

	m->input.key = 0;
	m->input.keypending = 0;
	m->input.upper = 0;
	m->input.lastkey = 0;
	m->input.mouse.x = m->input.mouse.y = m->input.mouse.wheel = 0;

	return;
}


void inputPoll(void *handle) {
	DARNER *m = handle;
	
	while (SDL_PollEvent(&m->input.event)) {
		if (m->input.event.type == SDL_KEYDOWN) {
			switch (m->input.event.key.keysym.sym) {
				case SDLK_UP:
				case SDLK_w:
					m->input.key |= KEY_UP;
					break;
				case SDLK_DOWN:
				case SDLK_s:
					m->input.key |= KEY_DOWN;
					break;
				case SDLK_LEFT:
				case SDLK_a:
					m->input.key |= KEY_LEFT;
					break;
				case SDLK_RIGHT:
				case SDLK_d:
					m->input.key |= KEY_RIGHT;
					break;
				case SDLK_PAGEUP:
					m->input.key |= KEY_Y;
					break;
				case SDLK_PAGEDOWN:
				case SDLK_SPACE:
					m->input.key |= KEY_X;
					break;
				case SDLK_HOME:
				case SDLK_ESCAPE:
					m->input.key |= KEY_A;
					break;
				case SDLK_END:
				case SDLK_RETURN:
					m->input.key |= KEY_B;
					break;
				case SDLK_RALT:
				case SDLK_LALT:
					m->input.key |= KEY_START;
					break;
				case SDLK_LCTRL:
					m->input.key |= KEY_SELECT;
					break;
				case SDLK_RSHIFT:
					m->input.upper |= (m->input.event.key.keysym.sym == SDLK_RSHIFT) ? 1 : 0;
					m->input.key |= KEY_L;
					break;
				case SDLK_RCTRL:
					m->input.key |= KEY_R;
					break;
				case SDLK_LSHIFT:
					m->input.upper |= 2;
					break;
				default:
					break;
			}

			if (m->input.event.key.keysym.sym < 0x80) {			// ASCII
				m->input.lastkey = m->input.event.key.keysym.sym;
			}
		} else if (m->input.event.type == SDL_KEYUP) {
			switch (m->input.event.key.keysym.sym) {
				case SDLK_UP:
				case SDLK_w:
					m->input.key |= KEY_UP;
					m->input.key ^= KEY_UP;
					break;
				case SDLK_DOWN:
				case SDLK_s:
					m->input.key |= KEY_DOWN;
					m->input.key ^= KEY_DOWN;
					break;
				case SDLK_LEFT:
				case SDLK_a:
					m->input.key |= KEY_LEFT;
					m->input.key ^= KEY_LEFT;
					break;
				case SDLK_RIGHT:
				case SDLK_d:
					m->input.key |= KEY_RIGHT;
					m->input.key ^= KEY_RIGHT;
					break;
				case SDLK_PAGEUP:
					m->input.key |= KEY_Y;
					m->input.key ^= KEY_Y;
					break;
				case SDLK_PAGEDOWN:
					m->input.key |= KEY_X;
					m->input.key ^= KEY_X;
					break;
				case SDLK_HOME:
				case SDLK_ESCAPE:
					m->input.key |= KEY_A;
					m->input.key ^= KEY_A;
					break;
				case SDLK_END:
				case SDLK_SPACE:
					m->input.key |= KEY_B;
					m->input.key ^= KEY_B;
					return;
				case SDLK_RALT:
				case SDLK_LALT:
					m->input.key |= KEY_START;
					m->input.key ^= KEY_START;
					return;
				case SDLK_LCTRL:
					m->input.key |= KEY_SELECT;
					m->input.key ^= KEY_SELECT;
					return;
				case SDLK_RSHIFT:
					m->input.upper |= (m->input.event.key.keysym.sym == SDLK_RSHIFT) ? 1 : 0;
					m->input.upper ^= (m->input.event.key.keysym.sym == SDLK_RSHIFT) ? 1 : 0;
					m->input.key |= KEY_L;
					m->input.key ^= KEY_L;
					return;
				case SDLK_RCTRL:
					m->input.key |= KEY_R;
					m->input.key ^= KEY_R;
					return;
				case SDLK_LSHIFT:
					m->input.upper |= 2;
					m->input.upper ^= 2;
					break;
				default:
					break;
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
		} else if (m->input.event.type == SDL_QUIT)
			exit(0);
	}

	m->input.keypending &= m->input.key;
	
	return;
}



unsigned int inputASCIIPop(void *handle) {
	unsigned int key;
	DARNER *m = handle;

	key = m->input.lastkey;
	if (key >= 'a' && key <= 'z' && m->input.upper)
		key -= ('a' - 'A');
	m->input.lastkey = 0;

	return key;
}
