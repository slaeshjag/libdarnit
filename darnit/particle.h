/*
Copyright (c) 2013 Steven Arnow
'particle.h' - This file is part of libdarnit

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


#ifndef __PARTICLE_H__
#define	__PARTICLE_H__


typedef enum {
	PARTICLE_TYPE_POINT,
	PARTICLE_TYPE_TEXTURED,
	PARTICLE_TYPE_TRIANGLE,
	PARTICLE_TYPE_QUAD,
} PARTICLE_TYPE;

typedef enum {
	PARTICLE_MODE_OFF,
	PARTICLE_MODE_SHOWER,
	PARTICLE_MODE_PULSAR,
	PARTICLE_MODE_AUTOPULSAR
} PARTICLE_MODE;


typedef struct {
	int				r;
	int				g;
	int				b;
	int				a;
} PARTICLE_COLOR;


typedef struct {
	int				age;
	int				x_vel;
	int				y_vel;

	int				x_pos;
	int				y_pos;

	int				used;

	PARTICLE_COLOR			color;
} PARTICLE_COMMON;


typedef struct {
	PARTICLE_COLOR			source;
	PARTICLE_COLOR			delta_c;
	PARTICLE_COLOR			target;
	PARTICLE_MODE			mode;
	PARTICLE_TYPE			type;

	int				particle_life;

	int				emit_x;
	int				emit_y;

	int				min_angle;
	int				max_angle;

	int				min_velocity;
	int				max_velocity;

	int				gravity_x;
	int				gravity_y;

	int				particles_max;
	int				spawnrate_max;

	unsigned int			last_frame_time;
	int				pulse;

	PARTICLE_COMMON			*particle;

	union {
		TILE_COLOR_TEX_CACHE	*tex;
		RECT_CACHE		*quad;
		TRI_COLOR_CACHE		*tri;
		POINT_COLOR_CACHE	*point;
	};

	/* Only used by PARTICLE_TYPE_POINT */
	int				point_size;

	/* Only used by PARTICLE_TYPE_TEXTURED */
	TILESHEET			*ts;
	float				tile_w;
	float				tile_h;
} PARTICLE;



PARTICLE *particleNew(int max_particles, PARTICLE_TYPE type);
void particleLoop(PARTICLE *p);
void particleColorDelta(PARTICLE *p);
void particleSetTexture(PARTICLE *p, const char *ts_fname, int tile_w, int tile_h, int tile);
PARTICLE *particleFree(PARTICLE *p);


#endif
