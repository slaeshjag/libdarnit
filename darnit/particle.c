/*
Copyright (c) 2011-2013 Steven Arnow
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


void particleTexturedInit(PARTICLE_VERTEX_TEX *tex) {
	tex->x = 0.0f;
	tex->y = 0.0f;
	tex->r = 0.0f;
	tex->g = 0.0f;
	tex->b = 0.0f;
	tex->a = 0.0f;
	tex->u = 0.0f;
	tex->v = 0.0f;

	return;
}


void particleColoredInit(PARTICLE_VERTEX *vert) {
	vert->x = 0.0f;
	vert->y = 0.0f;
	vert->r = 0.0f;
	vert->g = 0.0f;
	vert->b = 0.0f;
	vert->a = 0.0f;

	return;
}


PARTICLE_EMITTER *particleEmitterNew(PARTICLE_CONFIG *conf) {
	PARTICLE_EMITTER *pe;
	int i, j, k;

	if (!(pe = malloc(sizeof(PARTICLE_EMITTER))))
		return NULL;
	
	pe->tint_r = CCOLOR_FLOAT(conf->start_r);
	pe->tint_g = CCOLOR_FLOAT(conf->start_g);
	pe->tint_b = CCOLOR_FLOAT(conf->start_b);
	pe->tint_a = CCOLOR_FLOAT(conf->start_a);

	pe->d_r = CCOLOR_DELTA(conf->target_r, conf->start_r, conf->ttl);
	pe->d_g = CCOLOR_DELTA(conf->target_g, conf->start_g, conf->ttl);
	pe->d_b = CCOLOR_DELTA(conf->target_b, conf->start_b, conf->ttl);
	pe->d_a = CCOLOR_DELTA(conf->target_a, conf->start_a, conf->ttl);

	pe->gravity_x = PER_MSEC(conf->gravity_x);
	pe->gravity_y = PER_MSEC(conf->gravity_y);
	pe->spread_direction = conf->spawn_direction * 10;
	pe->spread_angle = conf->spawn_spread * 10;

	pe->ttl = conf->ttl;
	pe->particle_max = conf->particles_max;
	pe->spawnrate = PER_MSEC(conf->spawnrate);
	pe->mode = conf->mode;
	pe->size = conf->size;

	pe->spawn_vel_max = PER_MSEC(conf->spawn_maxvel);
	pe->spawn_vel_min = PER_MSEC(conf->spawn_minvel);

	pe->particle_max = conf->particles_max;
	pe->ttl = conf->ttl;

	pe->pulse_interval = pe->ttl / (pe->particle_max / pe->spawnrate) + 1;
	pe->ts = NULL;

	pe->com = malloc(sizeof(PARTICLE_COMMON) * pe->particle_max);
	for (i = 0; i < pe->particle_max; i++) {
		pe->com[i].age = -1;
		pe->com[i].x_vel = 0;
		pe->com[i].y_vel = 0;
	}
	
	switch (pe->mode & 0xF) {
		case PARTICLE_MODE_TEXTURED:
			pe->tex = malloc(sizeof(PARTICLE_TEXTURE) * pe->particle_max);
			for (i = 0; i < pe->particle_max; i++)
				for (j = 0; j < 2; j++)
					for (k = 0; k < 3; k++)
						particleTexturedInit(&pe->tex[i].tri[j].vert[k]);
			pe->ts_u1 = 1.0f / conf->ts->w * conf->ts_x;
			pe->ts_v1 = 1.0f / conf->ts->h * conf->ts_y;
			pe->ts_u2 = pe->ts_u1 + 1.0f / conf->ts->w * conf->size;
			pe->ts_v2 = pe->ts_v1 + 1.0f / conf->ts->h * conf->size;
			pe->ts = conf->ts;
			break;
		case PARTICLE_MODE_TRIANGLE:
			for (i = 0; i < pe->particle_max; i++)
				for (j = 0; j < 3; j++)
					particleColoredInit(&pe->tri[i].vert[j]);
			break;
		case PARTICLE_MODE_QUAD:
			for (i = 0; i < pe->particle_max; i++)
				for (j = 0; j < 2; j++)
					for (k = 0; k < 3; k++)
						particleColoredInit(&pe->quad[i].tri[j].vert[k]);
			break;
		case PARTICLE_MODE_LINE:
			for (i = 0; i < pe->particle_max; i++)
				for (j = 0; j < 2; j++)
					particleColoredInit(&pe->line[i].vert[j]);
			break;
		default:
			fprintf(stderr, "Fixme: Unhandled mode\n");
	}

	return pe;
}

#if 0
void particleEmitterRender(PARTICLE_EMITTER *pe) {
	int i, spawn;

	if (!pe)
		return;
	
	for (i = 0; i < pe->particle_max; i++) {
		pe->com[i].x_vel += pe->gravity_x * (d->fps.time_at_flip - d->fps.time_at_last_frame);
		pe->com[i].y_vel += pe->gravity_y * (d->fps.time_at_flip - d->fps.time_at_last_frame);
	}

	spawn = pe->spawnrate * (d->fps.time_at_flip - d->fps.time_at_last_frame);

	/* Render */
	switch (pe->mode & 0xF) {
		case PARTICLE_MODE_TEXTURED:
			break;
		case PARTICLE_MODE_TRIANGLE:
			break;
		case PARTICLE_MODE_QUAD:
			for (i = 0; i < pe->particles_max; i++) {


int particleEmitterMode(PARTICLE_EMITTER *pe, unsigned int mode, void *data) {
	if (!pe)
		return -1;
	switch (mode) {
		case 

#endif

// DARNIT_HEADLESS
#endif
