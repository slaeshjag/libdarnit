/*
Copyright (c) 2013 Steven Arnow
'particle.c' - This file is part of libdarnit

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

#ifndef DARNIT_HEADLESS


PARTICLE *particleFree(PARTICLE *p) {
	if (!p)
		return p;
	free(p->tex);
	free(p->particle);
	free(p);

	return NULL;
}


void particleMove(PARTICLE *p, int index, int x, int y) {
	p->particle[index].x_pos = x;
	p->particle[index].y_pos = y;

	switch (p->type) {
		case PARTICLE_TYPE_POINT:
			renderColorPointCalc(&p->point[index], p->particle[index].x_pos / 1000, p->particle[index].y_pos / 1000);
			break;
		default:
			break;
	}

	return;
}


void particleColor(PARTICLE *p, int index) {
	switch (p->type) {
		case PARTICLE_TYPE_POINT:
			renderPointColor(&p->point[index], p->particle[index].color.r / 1000000, p->particle[index].color.g / 1000000, p->particle[index].color.b / 1000000, p->particle[index].color.a / 1000000);
			break;
		default:
			break;
	}

	return;
}


void particleClear(PARTICLE *p, int index) {
	p->particle[index].color.r = p->particle[index].color.g = p->particle[index].color.b = p->particle[index].color.a = 0;
	particleMove(p, index, ~0, ~0);
	p->particle[index].used = 0;

	return;
}


void particleSpawn(PARTICLE *p, int index) {
	int velocity, angle;

	p->particle[index].color = p->source;
	p->particle[index].used = 1;
	p->particle[index].age = 0;

	velocity = rand() % (p->max_velocity - p->min_velocity) + p->min_velocity;
	angle = rand() % (p->max_angle - p->min_angle) + p->min_angle;

	p->particle[index].x_vel = (utilSine(angle + 900) * velocity) >> 16;
	p->particle[index].y_vel = (utilSine(angle) * velocity) >> 16;

	particleMove(p, index, p->emit_x, p->emit_y);
	particleColor(p, index);

	return;
}


PARTICLE *particleNew(int max_particles, PARTICLE_TYPE type) {
	PARTICLE *p;
	int i;

	if (!(p = malloc(sizeof(PARTICLE))))
		return NULL;

	if (!(p->particle = malloc(sizeof(PARTICLE_COMMON) * max_particles))) {
		free(p);
		return NULL;
	}

	p->mode = PARTICLE_MODE_OFF;
	p->type = type;

	switch (type) {
		case PARTICLE_TYPE_POINT:
			p->point = malloc(sizeof(POINT_COLOR_CACHE) * max_particles);
			break;
		default:
			p->tex = NULL;
			fprintf(stderr, "Unimplemented mode %i\n", type);
			return particleFree(p);
	}

	for (i = 0; i < max_particles; particleClear(p, i++));
	
	p->emit_x = p->emit_y = ~0;
	p->min_angle = p->min_velocity = 0;
	p->max_angle = p->max_velocity = 1;
	p->gravity_x = p->gravity_y = 0;
	p->particle_life = 1;

	p->particles_max = max_particles;
	p->spawnrate_max = -1;

	p->ts = NULL;
	p->source.r = p->source.g = p->source.b = p->source.a = 0;
	p->delta_c = p->source;
	p->target = p->source;
	p->pulse = 0;
	p->point_size = 1;

	p->last_frame_time = tpw_ticks();
	
	return p;
}


int particleHandle(PARTICLE *p, int index, int d_t) {
	if (!p->particle[index].used)
		return 0;
	
	p->particle[index].x_vel += p->gravity_x * d_t / 1000;
	p->particle[index].y_vel += p->gravity_y * d_t / 1000;

	p->particle[index].age += d_t;
	if (p->particle[index].age >= p->particle_life) {
		particleClear(p, index);
		return 0;
	}

	p->particle[index].x_pos += p->particle[index].x_vel * d_t;
	p->particle[index].y_pos += p->particle[index].y_vel * d_t;
	
	p->particle[index].color.r += p->delta_c.r * d_t;
	p->particle[index].color.g += p->delta_c.g * d_t;
	p->particle[index].color.b += p->delta_c.b * d_t;
	p->particle[index].color.a += p->delta_c.a * d_t;
	
	particleMove(p, index, p->particle[index].x_pos, p->particle[index].y_pos);
	particleColor(p, index);

	return 1;
}



void particleLoop(PARTICLE *p) {
	int d_t, spawn, i;

	if (!p)
		return;
	d_t = tpw_ticks() - p->last_frame_time;
	p->last_frame_time = tpw_ticks();

	switch (p->mode) {
		case PARTICLE_MODE_OFF:
			spawn = 0;
			break;
		case PARTICLE_MODE_PULSAR:
			spawn = p->pulse * p->particles_max;
			break;
		case PARTICLE_MODE_SHOWER:
			spawn = (p->spawnrate_max == -1) ? p->particles_max * 1000 / p->particle_life * d_t / 1000 : p->spawnrate_max * d_t / 1000;
			break;
		case PARTICLE_MODE_AUTOPULSAR:
			spawn = p->particles_max;
			break;
	}
	
	p->pulse = 0;

	for (i = 0; i < p->particles_max; i++)
		if (!particleHandle(p, i, d_t))
			if (spawn-- > 0)
				particleSpawn(p, i);

	switch (p->type) {
		case PARTICLE_TYPE_POINT:
			renderColorPointCache(p->point, p->particles_max, p->point_size);
			break;
		default:
			break;
	}


	return;
}


void particleColorDelta(PARTICLE *p) {
	p->delta_c.r = (p->target.r - p->source.r) / p->particle_life;
	p->delta_c.g = (p->target.g - p->source.g) / p->particle_life;
	p->delta_c.b = (p->target.b - p->source.b) / p->particle_life;
	p->delta_c.a = (p->target.a - p->source.a) / p->particle_life;

	return;
}
	

// DARNIT_HEADLESS
#endif
