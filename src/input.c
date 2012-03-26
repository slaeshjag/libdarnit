#include "darner.h"


void inputInit(void *handle) {
	DARNER *m = handle;

	m->input.key = 0;
	m->input.keypending = 0;

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
				case SDLK_i:
					m->input.key |= KEY_Y;
					break;
				case SDLK_PAGEDOWN:
				case SDLK_m:
					m->input.key |= KEY_X;
					break;
				case SDLK_HOME:
				case SDLK_ESCAPE:
				case SDLK_j:
					m->input.key |= KEY_A;
					break;
				case SDLK_END:
				case SDLK_k:
				case SDLK_SPACE:
					m->input.key |= KEY_B;
					break;
				case SDLK_RALT:
				case SDLK_LALT:
				case SDLK_y:
					m->input.key |= KEY_START;
					break;
				case SDLK_LCTRL:
				case SDLK_r:
					m->input.key |= KEY_SELECT;
					break;
				case SDLK_RSHIFT:
				case SDLK_q:
					m->input.key |= KEY_L;
					break;
				case SDLK_RCTRL:
				case SDLK_p:
					m->input.key |= KEY_R;
					break;
				default:
					break;
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
				case SDLK_i:
					m->input.key |= KEY_Y;
					m->input.key ^= KEY_Y;
					break;
				case SDLK_PAGEDOWN:
				case SDLK_m:
					m->input.key |= KEY_X;
					m->input.key ^= KEY_X;
					break;
				case SDLK_HOME:
				case SDLK_j:
				case SDLK_ESCAPE:
					m->input.key |= KEY_A;
					m->input.key ^= KEY_A;
					break;
				case SDLK_END:
				case SDLK_k:
				case SDLK_SPACE:
					m->input.key |= KEY_B;
					m->input.key ^= KEY_B;
					return;
				case SDLK_RALT:
				case SDLK_LALT:
				case SDLK_y:
					m->input.key |= KEY_START;
					m->input.key ^= KEY_START;
					return;
				case SDLK_LCTRL:
				case SDLK_r:
					m->input.key |= KEY_SELECT;
					m->input.key ^= KEY_SELECT;
					return;
				case SDLK_RSHIFT:
				case SDLK_q:
					m->input.key |= KEY_L;
					m->input.key ^= KEY_L;
					return;
				case SDLK_RCTRL:
				case SDLK_p:
					m->input.key |= KEY_R;
					m->input.key ^= KEY_R;
					return;
				default:
					break;
			}
		} else if (m->input.event.type == SDL_QUIT)
			exit(0);
	}

	m->input.keypending &= m->input.key;
	
	return;
}
