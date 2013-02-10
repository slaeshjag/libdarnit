/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_menutk.h' - This file is part of libdarnit

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


#ifndef __DARNIT_MENUTK_H__
#define	__DARNIT_MENUTK_H__

typedef void DARNIT_MENU;

DARNIT_MENU *d_menu_horizontal_new(const char *options, int x, int y, void *font, int max_w);
DARNIT_MENU *d_menu_vertical_new(const char *options, int x, int y, void *font, int menuw, int textskip_x, int max_h);
DARNIT_MENU *d_menu_vertical_shade_new(int x, int y, int shade_w, int shade_h, int y_advance, int options, int skip_option, int max_h);
DARNIT_MENU *d_menu_spinbutton_new(const char *label, int x, int y, void *font, int step, int min, int max);
DARNIT_MENU *d_menu_textinput_new(int x, int y, void *font, char *buff, int buff_len, int field_len);
void d_menu_vertical_shade_selections_reduce(DARNIT_MENU *menu);
void d_menu_vertical_shade_selections_increase(DARNIT_MENU *menu);
void d_menu_shade_color(DARNIT_MENU *menu, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
int d_menu_loop(DARNIT_MENU *menu);
void d_menu_selection_wait(DARNIT_MENU *menu);
int d_menu_peek(DARNIT_MENU *menu, int *top_sel);
int d_menu_changed(DARNIT_MENU *menu);
void d_menu_hide(DARNIT_MENU *menu);
void d_menu_show(DARNIT_MENU *menu);
void *d_menu_free(DARNIT_MENU *menu);

#endif
