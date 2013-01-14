#include "darnit.h"


void EXPORT_THIS *d_menu_horizontal_new(const char *options, int x, int y, void *font, int max_w) {
	return menutkHorisontalCreate(options, x, y, font, 0, max_w);
}


void EXPORT_THIS *d_menu_vertical_new(const char *options, int x, int y, void *font, int menuw, int textskip_x, int max_h) {
	return menutkVerticalCreate(options, x, y, font, menuw, textskip_x, 0, max_h);
}


void EXPORT_THIS *d_menu_vertical_shade_new(int x, int y, int shade_w, int shade_h, int y_advance, int options, int skip_option, int max_h) {
	return menutkVerticalShadeCreate(x, y, shade_h, y_advance, options, shade_w, 0, skip_option, max_h);
}


void EXPORT_THIS *d_menu_spinbutton_new(const char *label, int x, int y, void *font, int step, int min, int max) {
	return menutkSpinbuttonCreate(label, x, y, font, step, min, max);
}


void EXPORT_THIS *d_menu_textinput_new(int x, int y, void *font, char *buff, int buff_len, int field_len) {
	return menutkTextinputCreate(x, y, font, buff, buff_len, field_len);
}


void EXPORT_THIS d_menu_vertical_shade_selections_reduce(void *menu) {
	menutkVerticalShadeReduceSelectionsByOne(menu);

	return;
}


void EXPORT_THIS d_menu_vertical_shade_selections_increase(void *menu) {
	menutkVerticalShadeIncreaseSelectionsByOne(menu);
	
	return;
}


void EXPORT_THIS d_menu_shade_color(void *menu, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	MENUTK_COLOR col;
	MENUTK_ENTRY *m = menu;

	col.r = 1.0f/(256-r), col.g = 1.0f/(256-g), col.b = 1.0f/(256-b), col.a = 1.0f/(256-a);

	m->hl.color[0] = col;
	m->hl.color[1] = col;
	m->hl.color[2] = col;
	m->hl.color[3] = col;

	return;
}


int EXPORT_THIS d_menu_loop(void *menu) {
	return menutkMenuRoutine(menu);
}


void EXPORT_THIS d_menu_selection_wait(void *menu) {
	menutkWaitForNewSelection(menu);

	return;
}


int EXPORT_THIS d_menu_peek(void *menu, int *top_sel) {
	return menutkPeek(menu, top_sel);
}


int EXPORT_THIS d_menu_changed(void *menu) {
	return menutkGetIfChanged(menu);
}


void EXPORT_THIS d_menu_hide(void *menu) {
	menutkMenuHide(menu);

	return;
}


void EXPORT_THIS d_menu_show(void *menu) {
	menutkMenuUnhide(menu);

	return;
}


void EXPORT_THIS *d_menu_free(void *menu) {
	return menutkDestroy(menu);
}

