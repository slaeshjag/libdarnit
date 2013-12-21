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


void EXPORT_THIS d_particle_color_target(PARTICLE *p, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	if (!p)
		return;
	p->target.r = 1000000 * r;
	p->target.g = 1000000 * g;
	p->target.b = 1000000 * b;
	p->target.a = 1000000 * a;

	particleColorDelta(p);

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


void EXPORT_THIS d_particle_emitter_move(PARTICLE *p, int x, int y) {
	if (!p)
		return;
	p->emit_x = x * 1000;
	p->emit_y = y * 1000;

	return;
}


void EXPORT_THIS d_particle_emitter_angle(PARTICLE *p, int angle_min, int angle_max) {
	if (!p)
		return;
	if (!(angle_min - angle_max))
		angle_max++;
	p->max_angle = angle_max;
	p->min_angle = angle_min;

	return;
}


void EXPORT_THIS d_particle_emitter_velocity(PARTICLE *p, int velocity_min, int velocity_max) {
	if (!p)
		return;
	if (!(velocity_max - velocity_min))
		velocity_max++;
	p->max_velocity = velocity_max;
	p->min_velocity = velocity_min;

	return;
}


void EXPORT_THIS d_particle_emitter_gravity(PARTICLE *p, int gravity_xvel, int gravity_yvel) {
	if (!p)
		return;
	p->gravity_x = gravity_xvel;
	p->gravity_y = gravity_yvel;

	return;
}


void EXPORT_THIS d_particle_life(PARTICLE *p, int msec) {
	if (!p)
		return;
	p->particle_life = (msec) ? msec : 1;
	particleColorDelta(p);

	return;
}


void EXPORT_THIS d_particle_mode(PARTICLE *p, PARTICLE_MODE mode) {
	if (!p)
		return;
	p->mode = mode;

	return;
}


void EXPORT_THIS d_particle_pulse(PARTICLE *p) {
	if (!p)
		return;
	p->pulse = 1;
	
	return;
}


void EXPORT_THIS d_particle_spawnrate(PARTICLE *p, int spawnrate) {
	if (!p)
		return;
	p->spawnrate_max = spawnrate;
}


void EXPORT_THIS d_particle_point_size(PARTICLE *p, int size) {
	if (!p)
		return;
	p->point_size = size;
	return;
}


int EXPORT_THIS d_particle_used(PARTICLE *p) {
	if (!p)
		return 0;
	return p->used;
}


void EXPORT_THIS d_particle_tilesheet(PARTICLE *p, const char *fname, int tile_w, int tile_h, int tile) {
	return particleSetTexture(p, fname, tile_w, tile_h, tile);
}


void EXPORT_THIS d_particle_clear_all(PARTICLE *p) {
	return particleClearAll(p);
}


PARTICLE EXPORT_THIS *d_particle_free(PARTICLE *p) {
	return particleFree(p);
}
