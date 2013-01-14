#ifndef	__DARNIT_SPRITE_H__
#define	__DARNIT_SPRITE_H__

#define	DARNIT_SPRITE_ANIMATION_NOREPEAT	0
#define	DARNIT_SPRITE_ANIMATION_REPEAT		1

typedef void DARNIT_SPRITE;


DARNIT_SPRITE *d_sprite_new(DARNIT_TILESHEET *tilesheet);
DARNIT_SPRITE *d_sprite_load(const char *fname, int dir, unsigned int target_format);
DARNIT_SPRITE *d_sprite_delete(DARNIT_SPRITE *sprite);
void d_sprite_direction_set(DARNIT_SPRITE *sprite, int dir);
void d_sprite_activate(DARNIT_SPRITE *sprite, int dir);
void d_sprite_frame_set(DARNIT_SPRITE *sprite, int frame);
void d_sprite_move(DARNIT_SPRITE *sprite, int x, int y);
void d_sprite_animate_start(DARNIT_SPRITE *sprite);
void d_sprite_animate_pause(DARNIT_SPRITE *sprite);
void d_sprite_animate_stop(DARNIT_SPRITE *sprite);
void d_sprite_animate_repeat(void *sprite, int repeat);
void d_sprite_draw(DARNIT_SPRITE *sprite);
void d_sprite_frame_entry(DARNIT_SPRITE *sprite, int dir, int frame, int tile, int time);

int d_sprite_width(DARNIT_SPRITE *sprite);
int d_sprite_height(DARNIT_SPRITE *sprite);

#endif
