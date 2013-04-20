/*
Copyright (c) 2013 Steven Arnow
'main_win32.c' - This file is part of libdarnit_tpw

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


int tpw_init_platform() {
	#warning tpw_init_platform(): Not tested yet

	ZeroMemory(tpw.keys, 256);
	tpw.unicode_key = 0;
	tpw.modifiers = 0;
	return 1;
}


int tpw_window_create(const char *title, unsigned int window_w, unsigned int window_h, unsigned int fullscreen, const unsigned int bpp) {
	#warning tpw_window_create(): Not tested yet

	GLuint PixelFormat;
	WNDCLASS wc;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT WindowRect;
	DEVMODE dmScreenSettings;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0 ,0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
	};

	pfd.cColorBits = bpp;

	WindowRect.left = 0;
	WindowRect.right = window_w;
	WindowRect.top = 0;
	WindowRect.bottom = window_h;

	tpw.fullscreen = fullscreen;

	tpw.hInstance = GetModuleHandle(NULL);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = tpw_message_process;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = tpw.hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;

	if (!RegisterClass(&wc)) {
		fprintf(stderr, "Failed to register a window class\n");
		return 0;
	}
	
	
	if (fullscreen) {
		ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));
		dmScreenSettings.dmSize = sizeof(DEVMODE);
		dmScreenSettings.dmPelsWidth = window_w;
		dmScreenSettings.dmPelsHeight = window_h;
		dmScreenSettings.dmBitsPerPel = bpp;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			fprintf(stderr, "WARNING: Unable to enter fullscreen, will run in a window..\n");
			fullscreen = tpw.fullscreen = 0;
			goto nofullscreen;
		} else {
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;
			ShowCursor(FALSE);
		}
	} else {
		nofullscreen:
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);
	if (!(tpw.hWnd = CreateWindowEx(dwExStyle, WINDOW_CLASS_NAME, title, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle, 0, 0, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top,
			NULL, NULL, tpw.hInstance, NULL))) {
		fprintf(stderr, "Unable to create a window\n");
		tpw_quit();
		return 0;
	}

	if (!(tpw.hDC = GetDC(tpw.hWnd))) {
		tpw_quit();
		fprintf(stderr, "Unable to get a OpenGL device context\n");
		return 0;
	}

	if (!(PixelFormat = ChoosePixelFormat(tpw.hDC, &pfd))) {
		tpw_quit();
		fprintf(stderr, "The requested pixel format is not available\n");
		return 0;
	}

	if (!SetPixelFormat(tpw.hDC, PixelFormat, &pfd)) {
		tpw_quit();
		fprintf(stderr, "Unable to set the requested pixel format\n");
		return 0;
	}

	if (!(tpw.hRC = wglCreateContext(tpw.hDC))) {
		tpw_quit();
		fprintf(stderr, "Unable to create an OpenGL context\n");
		return 0;
	}

	if (!wglMakeCurrent(tpw.hDC, tpw.hRC)) {
		tpw_quit();
		fprintf(stderr, "Unable to activate the OpenGL context\n");
		return 0;
	}

	ShowWindow(tpw.hWnd, SW_SHOW);
	SetForegroundWindow(tpw.hWnd);
	SetFocus(tpw.hWnd);

	return 1;
}


void tpw_sleep(unsigned int msec) {
	Sleep(msec);
	return;
}


unsigned int tpw_ticks() {
	#warning tpw_ticks(): Not implemented yet
	return 0;
}


void tpw_input_grab(TPW_INPUT_GRAB grab) {
	switch (grab) {
		case TPW_INPUT_GRAB_GRAB:
			SetCapture(tpw.hWnd);
			break;
		case TPW_INPUT_GRAB_RELEASE:
			ReleaseCapture();
			break;
	}
	
	return;
}


void tpw_render_buffer_swap() {
	SwapBuffers(tpw.hDC);
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


void tpw_input_unicode(int enable) {
	tpw.unicode_key = enable;
	return;
}


void tpw_cursor_show(unsigned int show) {
	//ShowCursor((show) ? TRUE : FALSE);
	return;
}


int tpw_icon_set(void *pixdata_rpga32, int w, int h) {
	#warning tpw_icon_set(): Not implemented yet
	return 1;
}


void tpw_quit() {
	ChangeDisplaySettings(NULL, 0);
	ShowCursor(TRUE);

	if (tpw.hRC) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tpw.hRC);
		tpw.hRC = NULL;
	}

	if (tpw.hDC) {
		ReleaseDC(tpw.hWnd, tpw.hDC);
		tpw.hDC = NULL;
	}

	if (tpw.hWnd)
		DestroyWindow(tpw.hWnd);
	UnregisterClass(WINDOW_CLASS_NAME, tpw.hInstance);

	#warning tpw_quit(): Not tested yet
	return;
}
