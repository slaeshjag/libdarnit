#include "darnit.h"


unsigned int EXPORT_THIS d_keys_get() {
	return (d->input.key ^ d->input.keypending);
}


unsigned int EXPORT_THIS d_gets_zero() {
	return 0;
}


void EXPORT_THIS d_keys_set(unsigned int button) {
	d->input.keypending |= button;

	return;
}


void EXPORT_THIS d_keymapping_reset() {
	inputKeymapReset();

	return;
}


void EXPORT_THIS d_keymapping_set(INPUT_MAP map) {
	d->input.map = map;

	return;
}


INPUT_MAP EXPORT_THIS d_keymapping_get() {
	return d->input.map;
}


void EXPORT_THIS d_input_grab() {
	SDL_WM_GrabInput(SDL_GRAB_ON);

	return;
}


void EXPORT_THIS d_input_release() {
	#ifndef MAEMO
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	#endif

	return;
}


DARNIT_MOUSE EXPORT_THIS d_mouse_get() {
	DARNIT_MOUSE m = d->input.mouse;
	d->input.mouse.wheel = 0;
	return m;
}


void EXPORT_THIS d_joystick_get(int *js0_x, int *js0_y, int *js1_x, int *js1_y) {
	if (js0_x != NULL)
		*js0_x = d->input.js.nub0_x;
	if (js0_y != NULL)
		*js0_y = d->input.js.nub0_y;
	if (js1_x != NULL)
		*js1_x = d->input.js.nub1_x;
	if (js1_y != NULL)
		*js1_y = d->input.js.nub1_y;
	
	return;
}


void EXPORT_THIS d_key_raw_push(int sym, int action, int unicode) {
	inputRawPush(sym, action, unicode);

	return;
}


INPUT_RAW_KEY EXPORT_THIS d_key_raw_pop() {
	return inputRawPop();
}


void EXPORT_THIS d_key_raw_clear() {
	d->input.raw.use = 0;
	return;
}


const char EXPORT_THIS *d_key_name_get(int sym) {
	return SDL_GetKeyName(sym);
}


void EXPORT_THIS d_input_unicode(int enable) {
	SDL_EnableUNICODE(enable);

	return;
}
