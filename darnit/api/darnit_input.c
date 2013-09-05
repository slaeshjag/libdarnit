/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_input.c' - This file is part of libdarnit

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


DARNIT_KEYS convert_keys(unsigned int keys) {
	DARNIT_KEYS dk;

	dk.left = keys & KEY_LEFT;
	dk.right = (keys >> 1) & 1;
	dk.up = (keys >> 2) & 1;
	dk.down = (keys >> 3) & 1;
	dk.x = (keys >> 4) & 1;
	dk.y = (keys >> 5) & 1;
	dk.a = (keys >> 6) & 1;
	dk.b = (keys >> 7) & 1;
	dk.start = (keys >> 8) & 1;
	dk.select = (keys >> 9) & 1;
	dk.l = (keys >> 10) & 1;
	dk.r = (keys >> 11) & 1;
	dk.lmb = (keys >> 12) & 1;
	dk.rmb = (keys >> 13) & 1;
	dk.padding = 0;
	dk.mod_lshift = (keys >> 16) & 1;
	dk.mod_rshift = (keys >> 17) & 1;
	dk.mod_lctrl = (keys >> 18) & 1;
	dk.mod_rctrl = (keys >> 19) & 1;
	dk.mod_lalt = (keys >> 20) & 1;
	dk.mod_ralt = (keys >> 21) & 1;
	dk.mod_lmeta = (keys >> 22) & 1;
	dk.mod_rmeta = (keys >> 23) & 1;
	dk.mod_num = (keys >> 24) & 1;
	dk.mod_caps = (keys >> 25) & 1;
	dk.mod_mode = (keys >> 26) & 1;
	
	return dk;
}


DARNIT_KEYS EXPORT_THIS d_keys_get() {
	return convert_keys((d->input.key ^ d->input.keypending));
}


unsigned int EXPORT_THIS d_keys_zero() {
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
	tpw_input_grab(TPW_INPUT_GRAB_GRAB);

	return;
}


void EXPORT_THIS d_input_release() {
	#ifndef MAEMO
	tpw_input_grab(TPW_INPUT_GRAB_RELEASE);
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
	return tpw_key_name_get(sym);
}


void EXPORT_THIS d_input_unicode(int enable) {
	tpw_input_unicode(enable);

	return;
}
