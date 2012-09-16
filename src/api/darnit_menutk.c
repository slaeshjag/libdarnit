#include "darnit.h"


void EXPORT_THIS *darnitMenuHorizontalCreate(void *handle, const char *options, int x, int y, void *font, int max_w) {
	return menutkHorisontalCreate(handle, options, x, y, font, 0, max_w);
}


void EXPORT_THIS *darnitMenuVerticalCreate(void *handle, const char *options, int x, int y, void *font, int menuw, int textskip_x, int max_h) {
	return menutkVerticalCreate(handle, options, x, y, font, menuw, textskip_x, 0, max_h);
}


void EXPORT_THIS *darnitMenuVerticalShadeCreate(void *handle, int x, int y, int shade_w, int shade_h, int y_advance, int options, int skip_option, int max_h) {
	return menutkVerticalShadeCreate(handle, x, y, shade_h, y_advance, options, shade_w, 0, skip_option, max_h);
}


void EXPORT_THIS *darnitMenuSpinbuttonCreate(void *handle, const char *label, int x, int y, void *font, int step, int min, int max) {
	return menutkSpinbuttonCreate(handle, label, x, y, font, step, min, max);
}


void EXPORT_THIS *darnitMenuTextinputCreate(int x, int y, void *font, char *buff, int buff_len, int field_len) {
	return menutkTextinputCreate(x, y, font, buff, buff_len, field_len);
}


void EXPORT_THIS darnitMenuVerticalShadeSelectionsReduceByOne(void *menu) {
	menutkVerticalShadeReduceSelectionsByOne(menu);

	return;
}


void EXPORT_THIS darnitMenuVerticalShadeSelectionsIncreaseByOne(void *menu) {
	menutkVerticalShadeIncreaseSelectionsByOne(menu);
	
	return;
}


void EXPORT_THIS darnitMenuShadeColorSet(void *menu, float r, float g, float b, float a) {
	MENUTK_COLOR col;
	MENUTK_ENTRY *m = menu;

	col.r = r, col.g = g, col.b = b, col.a = a;

	m->hl.color[0] = col;
	m->hl.color[1] = col;
	m->hl.color[2] = col;
	m->hl.color[3] = col;

	return;
}


int EXPORT_THIS darnitMenuHandle(void *handle, void *menu) {
	return menutkMenuRoutine(handle, menu);
}


void EXPORT_THIS darnitMenuSelectionWaitForNew(void *menu) {
	menutkWaitForNewSelection(menu);

	return;
}


int EXPORT_THIS darnitMenuPeek(void *menu, int *top_sel) {
	return menutkPeek(menu, top_sel);
}


int EXPORT_THIS darnitMenuChangedCheck(void *menu) {
	return menutkGetIfChanged(menu);
}


void EXPORT_THIS darnitMenuHide(void *menu) {
	menutkMenuHide(menu);

	return;
}


void EXPORT_THIS darnitMenuUnhide(void *menu) {
	menutkMenuUnhide(menu);

	return;
}


void EXPORT_THIS *darnitMenuDestroy(void *menu) {
	return menutkDestroy(menu);
}

