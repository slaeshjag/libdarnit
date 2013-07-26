#include <darnit/darnit.h>


int callback(signed short *buff, int buff_len, int pos, void *data) {
	int i;
	buff_len >>= 1;


	for (i = 0; i < buff_len; i++)
		buff[i] = (((pos >> 1) + i) % 441 > 220) ? -8000 : 8000;

	return i << 1;
}
		


int main(int argc, char **argv) {
	DARNIT_PARTICLE *p_shower, *p_pulsar, *p_autopulsar;
	DARNIT_SOUND *sound;
	int i, a1, a2;

	d_init("paricle_test", "Particle engine test", NULL);

	sound = d_sound_callback_load(callback, NULL, 2);
	d_sound_play(sound, 0, 4, 4, 0);

	p_shower = d_particle_new(1000, DARNIT_PARTICLE_TYPE_POINT);
	p_pulsar = d_particle_new(1000, DARNIT_PARTICLE_TYPE_POINT);
	p_autopulsar = d_particle_new(1000, DARNIT_PARTICLE_TYPE_POINT);

	d_particle_color_start(p_shower, 0, 0, 255, 255);
	d_particle_color_start(p_pulsar, 255, 255, 0, 255);
	d_particle_color_start(p_autopulsar, 255, 0, 0, 255);

	d_particle_color_target(p_shower, 0, 0, 0, 255);
	d_particle_color_target(p_pulsar, 0, 0, 0, 255);
	d_particle_color_target(p_autopulsar, 0, 0, 0, 255);

	d_particle_emitter_move(p_shower, 400, 240);
	d_particle_emitter_move(p_pulsar, 200, 240);
	d_particle_emitter_move(p_autopulsar, 600, 240);

	d_particle_emitter_angle(p_shower, 0, 100);
	d_particle_emitter_angle(p_pulsar, 0, 3600);
	d_particle_emitter_angle(p_autopulsar, 0, 3600);

	d_particle_emitter_velocity(p_shower, 50, 100);
	d_particle_emitter_velocity(p_pulsar, 0, 100);
	d_particle_emitter_velocity(p_autopulsar, 0, 100);

	d_particle_emitter_gravity(p_shower, 0, 0);
	d_particle_emitter_gravity(p_pulsar, 0, 120);
	d_particle_emitter_gravity(p_autopulsar, 0, 120);

	d_particle_life(p_shower, 3000);
	d_particle_life(p_pulsar, 3000);
	d_particle_life(p_autopulsar, 3000);

	d_particle_mode(p_shower, DARNIT_PARTICLE_MODE_SHOWER);
	d_particle_mode(p_pulsar, DARNIT_PARTICLE_MODE_PULSAR);
	d_particle_mode(p_autopulsar, DARNIT_PARTICLE_MODE_AUTOPULSAR);

	for (i = 0; ; i++) {
		a1 = (i * 50) % 3600;
		a2 = (200 + i * 50) % 3600;

		if (d_keys_get().lmb) {
			d_particle_emitter_move(p_pulsar, d_mouse_get().x, d_mouse_get().y);
			d_particle_pulse(p_pulsar);
		}

		if (a2 < a1)
			a2 += 3600;
	
		d_particle_emitter_angle(p_shower, a1, a2);
		d_render_begin();
		d_particle_draw(p_shower);
		d_particle_draw(p_pulsar);
		d_particle_draw(p_autopulsar);
		d_render_end();
		d_loop();
	}

	return 0;
}

