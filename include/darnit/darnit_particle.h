/*
Copyright (c) 2013 Steven Arnow
'darnit_particle.h' - This file is part of libdarnit

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


#ifndef __DARNIT_PARTICLE_H__
#define	__DARNIT_PARTICLE_H__

typedef void			DARNIT_PARTICLE;

typedef enum {
	DARNIT_PARTICLE_TYPE_POINT
} DARNIT_PARTICLE_TYPE;

typedef enum {
	DARNIT_PARTICLE_MODE_OFF,
	DARNIT_PARTICLE_MODE_SHOWER,
	DARNIT_PARTICLE_MODE_PULSAR,
	DARNIT_PARTICLE_MODE_AUTOPULSAR
} DARNIT_PARTICLE_MODE;


DARNIT_PARTICLE *d_particle_new(int max_particles, DARNIT_PARTICLE_TYPE type);
void d_particle_draw(DARNIT_PARTICLE *p);
void d_particle_color_target(DARNIT_PARTICLE *p, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void d_particle_color_start(DARNIT_PARTICLE *p, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void d_particle_emitter_move(DARNIT_PARTICLE *p, int x, int y);
void d_particle_emitter_angle(DARNIT_PARTICLE *p, int angle_min, int angle_max);
void d_particle_emitter_velocity(DARNIT_PARTICLE *p, int velocity_min, int velocity_max);
void d_particle_emitter_gravity(DARNIT_PARTICLE *p, int gravity_xvel, int gravity_yvel);
void d_particle_life(DARNIT_PARTICLE *p, int msec);
void d_particle_mode(DARNIT_PARTICLE *p, DARNIT_PARTICLE_MODE mode);
void d_particle_spawnrate(DARNIT_PARTICLE *p, int spawnrate);
void d_particle_point_size(DARNIT_PARTICLE *p, int point_size);
void d_particle_pulse(DARNIT_PARTICLE *p);
DARNIT_PARTICLE *d_particle_free(DARNIT_PARTICLE *p);

#endif
