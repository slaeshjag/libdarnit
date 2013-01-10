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
	float				x;
	float				y;
	float				r;
	float				g;
	float				b;
	float				a;
} PARTICLE_VERTEX;


typedef struct {
	float				x;
	float				y;
	float				r;
	float				g;
	float				b;
	float				a;
	float				u;
	float				v;
} PARTICLE_VERTEX_TEX;


typedef struct {
	PARTICLE_VERTEX			vert[2];
} PARTICLE_LINE;


typedef struct {
	PARTICLE_VERTEX			vert[3];
} PARTICLE_TRIANGLE;


typedef struct {
	PARTICLE_TRIANGLE		tri[2];
} PARTICLE_QUAD;


typedef struct {
	PARTICLE_VERTEX_TEX		vert[3];
} PARTICLE_TRI_TEX;


typedef struct {
	PARTICLE_TRI_TEX		tri[2];
} PARTICLE_TEXTURE;


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
		PARTICLE_TEXTURE	*tex;
		PARTICLE_TRIANGLE	*tri;
		PARTICLE_QUAD		*quad;
		PARTICLE_LINE		*line;
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
