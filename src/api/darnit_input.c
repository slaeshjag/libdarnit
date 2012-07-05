#include "darnit.h"


unsigned int EXPORT_THIS darnitButtonGet(void *handle) {
	DARNIT *d = handle;

	return (d->input.key ^ d->input.keypending);
}


unsigned int EXPORT_THIS darnitButtonZero() {
	return 0;
}


void EXPORT_THIS darnitButtonSet(void *handle, unsigned int button) {
	DARNIT *d = handle;

	d->input.keypending |= button;

	return;
}


void EXPORT_THIS darnitButtonMappingReset(void *handle) {
	inputKeymapReset(handle);

	return;
}


void EXPORT_THIS darnitButtonMappingSet(void *handle, INPUT_MAP map) {
	DARNIT *d = handle;

	d->input.map = map;

	return;
}


INPUT_MAP EXPORT_THIS darnitButtonMappingGet(void *handle) {
	DARNIT *d = handle;

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


DARNIT_MOUSE EXPORT_THIS darnitMouseGet(void *handle) {
	DARNIT *d = handle;

	return d->input.mouse;
}


void EXPORT_THIS darnitJoystickGet(void *handle, int *js0_x, int *js0_y, int *js1_x, int *js1_y) {
	DARNIT *d = handle;

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
