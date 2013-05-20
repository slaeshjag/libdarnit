/*
Copyright (c) 2013 Steven Arnow
'darnit_particle.c' - This file is part of libdarnit

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


#include "darnit.h"

PARTICLE EXPORT_THIS *d_particle_new(int max_particles, PARTICLE_TYPE type) {
	return particleNew(max_particles, type);
}


void EXPORT_THIS d_particle_draw(PARTICLE *p) {
	particleLoop(p);

	return;
}


void EXPORT_THIS d_particle_color_start(PARTICLE *p, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	if (!p)
		return;
	p->source.r = 1000000 * r;
	p->source.g = 1000000 * g;
	p->source.b = 1000000 * b;
	p->source.a = 1000000 * a;

	particleColorDelta(p);

	return;
}


PARTICLE EXPORT_THIS *d_particle_free(PARTICLE *p) {
	return particleFree(p);
}
