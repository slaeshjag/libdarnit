/*
Copyright (c) 2013 Steven Arnow
'main_sdl.c' - This file is part of libdarnit_tpw

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
	glfwInit();

	return 1;
}


int tpw_window_create(const char *title, unsigned int window_w, unsigned int window_h, unsigned int fullscreen, const unsigned int bpp) {
	unsigned int mode = 0;
	mode = (fullscreen) ? GLFW_FULLSCREEN : GLFW_WINDOW;
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	if (glfwOpenWindow(window_w, window_h, 0, 0, 0, 0, 8, 8, mode) != GL_TRUE)
		return 0;
	glfwSetWindowTitle(title);
	
	glfwSwapInterval(1);
	
	return 1;
}


void tpw_sleep(unsigned int msec) {
	#ifdef _WIN32
	Sleep(msec);
	#endif

	return;
}


unsigned int tpw_ticks() {
	return glfwGetTime() * 1000;
}


void tpw_input_grab(TPW_INPUT_GRAB grab) {
	#warning tpw_input_grab not implemted

	return;
}


void tpw_render_buffer_swap() {
	glfwSwapBuffers();

	return;
}


const char *tpw_key_name_get(int sym) {
	#warning tpw_key_name_get not implemented
	return "NOT IMPLEMENTED";
}


TPW_RECT **tpw_videomodes_list() {
	#warning tpw_videomodes_list not implemented
	return NULL;
}


void tpw_input_unicode(int enable) {
	return;
}


void tpw_cursor_show(unsigned int show) {
	glfwDisable(GLFW_MOUSE_CURSOR);

	return;
}


int tpw_icon_set(void *pixdata_rgba32, int w, int h) {
	#warning tpw_icon_set not implemented

	return 1;
}


void tpw_quit() {
	glfwTerminate();

	return;
}
