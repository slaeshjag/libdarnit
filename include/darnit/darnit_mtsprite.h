#ifndef	__DARNIT_MTSPRITE_H__
#define	__DARNIT_MTSPRITE_H__

typedef void DARNIT_MTSPRITE;

DARNIT_MTSPRITE *d_mtsprite_new(void *tilesheet);
DARNIT_MTSPRITE *d_mtsprite_load(const char *fname);
void d_mtsprite_animate_start(DARNIT_MTSPRITE *mtsprite);
void d_mtsprite_animate_pause(DARNIT_MTSPRITE *mtsprite);
void d_mtsprite_tile_add(DARNIT_MTSPRITE *sprite, int x, int y, int w, int h, int rx, int ry);
void d_mtsprite_frame_set(DARNIT_MTSPRITE *sprite, int time);
void d_mtsprite_animate_stop(DARNIT_MTSPRITE *mtsprite);
void d_mtsprite_animate_repeat(DARNIT_MTSPRITE *mtsprite, int repeat);
void d_mtsprite_draw(DARNIT_MTSPRITE *mtsprite);
void *d_mtsprite_delete(DARNIT_MTSPRITE *mtsprite);

#endif
