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


void tpw_event_platform_init() {
	/* Ugly hack to make MSVS happy... <.< */
	vk_translate[0x08] = 8;
	vk_translate[0x09] = 9;
	vk_translate[0x0C] = 12;
	vk_translate[0x0D] = 13;
	vk_translate[0x0E] = 19;
	vk_translate[0x14] = 301;
	vk_translate[0x1B] = 27;
	vk_translate[0x20] = 32;
	vk_translate[0x21] = 280;
	vk_translate[0x22] = 281;
	vk_translate[0x23] = 279;
	vk_translate[0x24] = 278;
	vk_translate[0x25] = 276;
	vk_translate[0x26] = 273;
	vk_translate[0x27] = 275;
	vk_translate[0x28] = 274;
	vk_translate[0x2D] = 277;
	vk_translate[0x2E] = 127;
	vk_translate[0x30] = 48;
	vk_translate[0x31] = 49;
	vk_translate[0x32] = 50;
	vk_translate[0x33] = 51;
	vk_translate[0x34] = 52;
	vk_translate[0x35] = 53;
	vk_translate[0x36] = 54;
	vk_translate[0x37] = 55;
	vk_translate[0x38] = 56;
	vk_translate[0x39] = 57;
	vk_translate[0x41] = 97;
	vk_translate[0x42] = 98;
	vk_translate[0x43] = 99;
	vk_translate[0x44] = 100;
	vk_translate[0x45] = 101;
	vk_translate[0x46] = 102;
	vk_translate[0x47] = 103;
	vk_translate[0x48] = 104;
	vk_translate[0x49] = 105;
	vk_translate[0x4A] = 106;
	vk_translate[0x4B] = 107;
	vk_translate[0x4C] = 108;
	vk_translate[0x4D] = 109;
	vk_translate[0x4E] = 110;
	vk_translate[0x4F] = 111;
	vk_translate[0x50] = 112;
	vk_translate[0x51] = 113;
	vk_translate[0x52] = 114;
	vk_translate[0x53] = 115;
	vk_translate[0x54] = 116;
	vk_translate[0x55] = 117;
	vk_translate[0x56] = 118;
	vk_translate[0x57] = 119;
	vk_translate[0x58] = 120;
	vk_translate[0x59] = 121;
	vk_translate[0x5A] = 122;
	vk_translate[0x5B] = 310;
	vk_translate[0x5C] = 309;
	vk_translate[0x5F] = 256;
	vk_translate[0x60] = 257;
	vk_translate[0x61] = 258;
	vk_translate[0x62] = 259;
	vk_translate[0x63] = 260;
	vk_translate[0x64] = 261;
	vk_translate[0x65] = 262;
	vk_translate[0x66] = 263;
	vk_translate[0x67] = 264;
	vk_translate[0x68] = 265;
	vk_translate[0x69] = 266;
	vk_translate[0x6A] = 268;
	vk_translate[0x6B] = 270;
	vk_translate[0x6C] = 271;
	vk_translate[0x6D] = 269;
	vk_translate[0x6E] = 266;
	vk_translate[0x6F] = 267;
	vk_translate[0x70] = 282;
	vk_translate[0x71] = 283;
	vk_translate[0x72] = 284;
	vk_translate[0x73] = 285;
	vk_translate[0x74] = 286;
	vk_translate[0x75] = 287;
	vk_translate[0x76] = 288;
	vk_translate[0x77] = 289;
	vk_translate[0x78] = 290;
	vk_translate[0x79] = 291;
	vk_translate[0x7A] = 292;
	vk_translate[0x7B] = 293;
	vk_translate[0x90] = 300;
	vk_translate[0x91] = 302;
	vk_translate[0xA0] = 304;
	vk_translate[0xA1] = 303;
	vk_translate[0xA2] = 306;
	vk_translate[0xA3] = 305;
	vk_translate[0xA4] = 308;
	vk_translate[0xA5] = 307;
	
	return;
}
