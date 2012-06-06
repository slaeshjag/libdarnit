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
