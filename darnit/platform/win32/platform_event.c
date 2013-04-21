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

unsigned short vk_translate[256];

unsigned int tpw_modifier(unsigned int vk);
unsigned int tpw_keysym_translate(unsigned int vk); 


LRESULT CALLBACK tpw_message_process(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	TPW_EVENT event;
	WORD ht;
	static int hidden_cursor = 0;
	short keys[2];

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
					return DefWindowProc(hWnd, uMsg, wParam, lParam);
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
		case WM_SETCURSOR:
			ht = LOWORD(lParam);
			if (ht == HTCLIENT && !hidden_cursor && tpw.hide_cursor) {
				hidden_cursor = 1;
				ShowCursor(FALSE);
			} else if (ht != HTCLIENT && hidden_cursor) {
				hidden_cursor = 0;
				ShowCursor(TRUE);
			}
			return 1;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	tpw_event_push(event);

	return 0;
}


void tpw_event_loop() {
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return;
}


TPW_JOYSTICK *tpw_joystick_open(int js_id) {
	#pragma warning tpw_joystick_open(): Not implemented yet
	return NULL;
}


int tpw_joystick_num() {
	#pragma warning tpw_joystick_num(): Not implemented yet
	return 0;
}


void tpw_joystick_enable(TPW_ENBOOL enable) {
	#pragma warning tpw_joystick_enable(): Not implemented yet
	return;
}


const char *tpw_joystick_name(int i) {
	#pragma warning tpw_joystick_name(): Not implemented yet
	return "Blah";
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
