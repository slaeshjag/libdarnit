/*
Copyright (c) 2011-2013 Steven Arnow
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

#define	CCOLOR_FLOAT(x)			(1.0f / ((x) - 255))
#define	CCOLOR_DELTA(x, xn, y)		((1.0f / (((x) - (xn)) - 255)) / (y))
#define	PER_MSEC(x)			(((float) (x)) / 1000.0f)

#define	PARTICLE_MODE_UNDEF		0
#define	PARTICLE_MODE_TEXTURED		1
#define	PARTICLE_MODE_TRIANGLE		2
#define	PARTICLE_MODE_QUAD		3
#define	PARTICLE_MODE_LINE		4

#define	PARTICLE_MODE_PULSAR		0x100


typedef struct {
	int				age;
	float				x_vel;
	float				y_vel;
} PARTICLE_COMMON;



typedef struct {
	unsigned int			particle_max;
	unsigned int			ttl;
	unsigned int			size;
	float				spawnrate;
	unsigned int			mode;
	unsigned int			spread_direction;
	unsigned int			spread_angle;

	/* Only used by pulsars */
	int				pulse_interval;
	

	float				tint_r;
	float				tint_g;
	float				tint_b;
	float				tint_a;

	float				d_r;
	float				d_g;
	float				d_b;
	float				d_a;

	float				gravity_x;
	float				gravity_y;

	float				spawn_vel_max;
	float				spawn_vel_min;


	/* Some members for textured particles */
	TILESHEET			*ts;
	float				ts_u1;
	float				ts_v1;
	float				ts_u2;
	float				ts_v2;

	PARTICLE_COMMON			*com;

	union {
		TILE_COLOR_TEX_CACHE	*tex;
		TRI_COLOR_CACHE		*tri;
		TILE_COLOR_CACHE	*quad;
		LINE_COLOR_CACHE	*line;
		void			*buffer;
	};
} PARTICLE_EMITTER;


typedef struct {
	unsigned int			spawnrate;
	unsigned int			particles_max;
	unsigned int			ttl;
	unsigned int			size;

	unsigned int			mode;

	/* Only for textured particles */
	TILESHEET			*ts;
	int				ts_x;
	int				ts_y;

	/* For all particles */
	unsigned char			start_r;
	unsigned char			start_g;
	unsigned char			start_b;
	unsigned char			start_a;

	unsigned char			target_r;
	unsigned char			target_g;
	unsigned char			target_b;
	unsigned char			target_a;

	unsigned int			spawn_maxvel;
	unsigned int			spawn_minvel;

					/* In degrees */
	unsigned int			spawn_direction;
	unsigned int			spawn_spread;

	int				gravity_x;
	int				gravity_y;
} PARTICLE_CONFIG;


#endif
