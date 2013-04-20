/*
Copyright (c) 2013 Steven Arnow
'main_dummy.c' - This file is part of libdarnit_tpw

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

#define	TPW_INTERNAL
#include "../main.h"

int tpw_init_platform() {
	#warning tpw_init_platform(): Not implemented yet
	return 1;
}


int tpw_window_create(const char *title, unsigned int window_w, unsigned int window_h, unsigned int fullscreen, unsigned int bpp) {
	#warning tpw_window_create(): Not implemented yet
	return 0;
}


void tpw_sleep(unsigned int msec) {
	#warning tpw_sleep(): Not implemented yet
	return;
}


unsigned int tpw_ticks() {
	#warning tpw_ticks(): Not implemented yet
	return 0;
}


void tpw_input_grab(TPW_INPUT_GRAB grab) {
	#warning tpw_input_grab(): Not implemented yet
	return;
}


void tpw_render_buffer_swap() {
	#warning tpw_render_buffer_swap(): Not implemented yet
	return;
}


const char *tpw_key_name_get(int sym) {
	#warning tpw_key_name_get(): Not implemented yet
	return "blah";
}


TPW_RECT **tpw_videomodes_list() {
	#warning tpw_videomodes_list(): Not implemented yet
	return (void *) -1;
}


void tpw_input_unicode_enable(int enable) {
	#warning tpw_input_unicode_enable(): Not implemented yet
	return;
}


void tpw_cursor_show(int show) {
	#warning tpw_cursor_show(): Not implemented yet
	return;
}


int tpw_icon_set(void *pixdata_rpga32, int w, int h) {
	#warning tpw_icon_set(): Not implemented yet
	return 1;
}


void tpw_quit() {
	#warning tpw_quit(): Not implemented yet
	return;
}
