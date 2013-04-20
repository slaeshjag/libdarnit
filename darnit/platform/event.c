/*
Copyright (c) 2013 Steven Arnow
'event.c' - This file is part of libdarnit_tpw

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

#include "main.h"

int tpw_event_init(int queue_size) {
	tpw.common.event.event_read = 0;
	tpw.common.event.event_write = 0;
	tpw.common.event.event_max = queue_size;

	if (!(tpw.common.event.event = malloc(sizeof(TPW_EVENT) * queue_size)))
		return 0;
	return 1;
}


void tpw_event_push(TPW_EVENT event) {
	if (TPW_FIFO_NEXT(tpw.common.event.event_write, tpw.common.event.event_max) == tpw.common.event.event_read)
		return;
	tpw.common.event.event[tpw.common.event.event_write] = event;
	tpw.common.event.event_write = TPW_FIFO_NEXT(tpw.common.event.event_write, tpw.common.event.event_max);

	return;
}


int tpw_event_pop(TPW_EVENT *event) {
	if (tpw.common.event.event_read == tpw.common.event.event_write)
		return 0;
	*event = tpw.common.event.event[tpw.common.event.event_read];
	tpw.common.event.event_read = TPW_FIFO_NEXT(tpw.common.event.event_read, tpw.common.event.event_max);

	return 1;
}
