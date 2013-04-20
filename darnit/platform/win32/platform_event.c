/*
Copyright (c) 2013 Steven Arnow
'event_win32.c' - This file is part of libdarnit_tpw

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

unsigned short vk_translate[256] = {
	[0x08] = 8,
	[0x09] = 9,
	[0x0C] = 12,
	[0x0D] = 13,
	[0x0E] = 19,
	[0x14] = 301,
	[0x1B] = 27,
	[0x20] = 32,
	[0x21] = 280,
	[0x22] = 281,
	[0x23] = 279,
	[0x24] = 278,
	[0x25] = 276,
	[0x26] = 273,
	[0x27] = 275,
	[0x28] = 274,
	[0x2D] = 277,
	[0x2E] = 127,
	[0x30] = 48,
	[0x31] = 49,
	[0x32] = 50,
	[0x33] = 51,
	[0x34] = 52,
	[0x35] = 53,
	[0x36] = 54,
	[0x37] = 55,
	[0x38] = 56,
	[0x39] = 57,
	[0x41] = 97,
	[0x42] = 98,
	[0x43] = 99,
	[0x44] = 100,
	[0x45] = 101,
	[0x46] = 102,
	[0x47] = 103,
	[0x48] = 104,
	[0x49] = 105,
	[0x4A] = 106,
	[0x4B] = 107,
	[0x4C] = 108,
	[0x4D] = 109,
	[0x4E] = 110,
	[0x4F] = 111,
	[0x50] = 112,
	[0x51] = 113,
	[0x52] = 114,
	[0x53] = 115,
	[0x54] = 116,
	[0x55] = 117,
	[0x56] = 118,
	[0x57] = 119,
	[0x58] = 120,
	[0x59] = 121,
	[0x5A] = 122,
	[0x5B] = 310,
	[0x5C] = 309,
	[0x5F] = 256,
	[0x60] = 257,
	[0x61] = 258,
	[0x62] = 259,
	[0x63] = 260,
	[0x64] = 261,
	[0x65] = 262,
	[0x66] = 263,
	[0x67] = 264,
	[0x68] = 265,
	[0x69] = 266,
	[0x6A] = 268,
	[0x6B] = 270,
	[0x6C] = 271,
	[0x6D] = 269,
	[0x6E] = 266,
	[0x6F] = 267,
	[0x70] = 282,
	[0x71] = 283,
	[0x72] = 284,
	[0x73] = 285,
	[0x74] = 286,
	[0x75] = 287,
	[0x76] = 288,
	[0x77] = 289,
	[0x78] = 290,
	[0x79] = 291,
	[0x7A] = 292,
	[0x7B] = 293,
	[0x90] = 300,
	[0x91] = 302,
	[0xA0] = 304,
	[0xA1] = 303,
	[0xA2] = 306,
	[0xA3] = 305,
	[0xA4] = 308,
	[0xA5] = 307,
};

unsigned int tpw_modifier(unsigned int vk);
unsigned int tpw_keysym_translate(unsigned int vk); 


LRESULT CALLBACK tpw_message_process(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	TPW_EVENT event;
	short keys[2];
	#warning tpw_message_process(): Not tested yet

	switch (uMsg) {
		case WM_ACTIVATE:
			event.type = TPW_EVENT_TYPE_ACTIVE;
			event.active.gain = (!HIWORD(wParam));
			event.active.app_active = 1;
			break;
		case WM_SYSCOMMAND:
			switch (wParam) {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
				default:
					return 1;
			}
		case WM_CLOSE:
			event.type = TPW_EVENT_TYPE_QUIT;
			break;
		case WM_KEYDOWN:
			if (tpw.keys[wParam])
				return 0;
			event.type = TPW_EVENT_TYPE_KEYDOWN;
			tpw.modifiers |= tpw_modifier(wParam);
			event.key.keysym = tpw_keysym_translate(wParam);
			tpw.keys[wParam] = 0xFF;
			if (!event.key.keysym)
				return 0;
			if (tpw.unicode_key)
				event.key.unicode = (ToUnicode(wParam, lParam, (BYTE *) tpw.keys, (LPWSTR) keys, 1, 0) > 0) ? keys[0] : 0;
			else
				event.key.unicode = 0;
			break;
		case WM_KEYUP:
			tpw.keys[wParam] = 0;
			event.type = TPW_EVENT_TYPE_KEYUP;
			event.key.keysym = tpw_keysym_translate(wParam);
			tpw.modifiers |= tpw_modifier(wParam);
			tpw.modifiers ^= tpw_modifier(wParam);
			break;
		case WM_LBUTTONDOWN:
			event.type = TPW_EVENT_TYPE_MOUSEBTN_DOWN;
			event.mouse.button = TPW_MOUSE_BUTTON_LEFT;
			break;
		case WM_MBUTTONDOWN:
			event.type = TPW_EVENT_TYPE_MOUSEBTN_DOWN;
			event.mouse.button = TPW_MOUSE_BUTTON_MIDDLE;
			break;
		case WM_RBUTTONDOWN:
			event.type = TPW_EVENT_TYPE_MOUSEBTN_DOWN;
			event.mouse.button = TPW_MOUSE_BUTTON_RIGHT;
			break;
		case WM_LBUTTONUP:
			event.type = TPW_EVENT_TYPE_MOUSEBTN_UP;
			event.mouse.button = TPW_MOUSE_BUTTON_LEFT;
			break;
		case WM_MBUTTONUP:
			event.type = TPW_EVENT_TYPE_MOUSEBTN_UP;
			event.mouse.button = TPW_MOUSE_BUTTON_MIDDLE;
			break;
		case WM_RBUTTONUP:
			event.type = TPW_EVENT_TYPE_MOUSEBTN_UP;
			event.mouse.button = TPW_MOUSE_BUTTON_RIGHT;
			break;
		case WM_MOUSEWHEEL:
			event.type = TPW_EVENT_TYPE_MOUSEBTN_DOWN;
			event.mouse.button = (GET_WHEEL_DELTA_WPARAM(wParam) < 0) ? TPW_MOUSE_BUTTON_WHEEL_UP : TPW_MOUSE_BUTTON_WHEEL_DOWN;
			DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		case WM_MOUSEMOVE:
			event.type = TPW_EVENT_TYPE_MOUSEMOVE;
			event.mouse.x = GET_X_LPARAM(lParam);
			event.mouse.y = GET_Y_LPARAM(lParam);
			DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	tpw_event_push(event);

	return 0;
}


void tpw_event_loop() {
	MSG msg;
	#warning tpw_event_loop(): Probably incomplete implementation
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return;
}


TPW_JOYSTICK *tpw_joystick_open(int js_id) {
	#warning tpw_joystick_open(): Not implemented yet
	return NULL;
}


int tpw_joystick_num() {
	#warning tpw_joystick_num(): Not implemented yet
	return 0;
}


void tpw_joystick_enable(TPW_ENBOOL enable) {
	#warning tpw_joystick_enable(): Not implemented yet
	return;
}


const char *tpw_joystick_name(int i) {
	#warning tpw_joystick_name(): Not implemented yet
	return;
}


unsigned int tpw_keys_modifiers() {
	return tpw.modifiers;
}


unsigned int tpw_keysym_translate(unsigned int vk) {
	return vk_translate[vk];
}


unsigned int tpw_modifier(unsigned int vk) {
	switch (vk) {
		case 0xA0:
			return TPW_KEY_MOD_LSHIFT;
		case 0xA1:
			return TPW_KEY_MOD_RSHIFT;
		case 0xA2:
			return TPW_KEY_MOD_LCTRL;
		case 0xA3:
			return TPW_KEY_MOD_RCTRL;
		case 0xA4:
			return TPW_KEY_MOD_LALT;
		case 0xA5:
			return TPW_KEY_MOD_RALT;
		case 0x5B:
			return TPW_KEY_MOD_LMETA;
		case 0x5C:
			return TPW_KEY_MOD_RMETA;
		default:
			return 0;
	}

	return 0;
}
