#include "darnit.h"


unsigned int EXPORT_THIS darnitButtonGet() {
	return (d->input.key ^ d->input.keypending);
}


unsigned int EXPORT_THIS darnitButtonZero() {
	return 0;
}


void EXPORT_THIS darnitButtonSet(unsigned int button) {
	d->input.keypending |= button;

	return;
}


void EXPORT_THIS darnitButtonMappingReset() {
	inputKeymapReset();

	return;
}


void EXPORT_THIS darnitButtonMappingSet(INPUT_MAP map) {
	d->input.map = map;

	return;
}


INPUT_MAP EXPORT_THIS darnitButtonMappingGet() {
	return d->input.map;
}


void EXPORT_THIS darnitInputGrab() {
	SDL_WM_GrabInput(SDL_GRAB_ON);

	return;
}


void EXPORT_THIS darnitInputUngrab() {
	SDL_WM_GrabInput(SDL_GRAB_OFF);

	return;
}


DARNIT_MOUSE EXPORT_THIS darnitMouseGet() {
	return d->input.mouse;
}


void EXPORT_THIS darnitJoystickGet(int *js0_x, int *js0_y, int *js1_x, int *js1_y) {
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


void EXPORT_THIS darnitKeyboardRawPush(int sym, int action) {
	inputRawPush(sym, action);

	return;
}


int EXPORT_THIS darnitKeyboardRawPop(int *action) {
	return inputRawPop(action);
}


void EXPORT_THIS darnitKeyboardRawClear() {
	d->input.raw.use = 0;
	return;
}


const char EXPORT_THIS darnitKeyboardSymNameGet(int sym) {
	return SDL_KeyName(sym);
}
