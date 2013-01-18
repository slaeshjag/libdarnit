#ifndef __DARNIT_INPUT_H__
#define	__DARNIT_INPUT_H__

#define		BUTTON_ACCEPT		b
#define		BUTTON_CANCEL		a

#define DARNIT_KEYACTION_PRESS		1
#define DARNIT_KEYACTION_RELEASE	2


typedef struct {
	unsigned int	left		: 1;
	unsigned int	right		: 1;
	unsigned int	up		: 1;
	unsigned int	down		: 1;
	unsigned int	x		: 1;
	unsigned int	y		: 1;
	unsigned int	a		: 1;
	unsigned int	b		: 1;
	unsigned int	start		: 1;
	unsigned int	select		: 1;
	unsigned int	l		: 1;
	unsigned int	r		: 1;
	unsigned int	lmb		: 1;
	unsigned int	rmb		: 1;
	unsigned int	padding		: 2;

	/* Modifier keys (SHIFT, CTRL, ALT etc.. */
	unsigned int	mod_lshift	: 1;
	unsigned int	mod_rshift	: 1;
	unsigned int	mod_lctrl	: 1;
	unsigned int	mod_rctrl	: 1;
	unsigned int	mod_lalt	: 1;
	unsigned int	mod_ralt	: 1;
	unsigned int	mod_lmeta	: 1;
	unsigned int	mod_rmeta	: 1;
	unsigned int	mod_num		: 1;
	unsigned int	mod_caps	: 1;
	unsigned int	mod_mode	: 1;
} DARNIT_KEYS;


typedef struct {
	int		keysym;
	int		action;
	int		unicode;
} DARNIT_KEY_RAW;


typedef struct {
	unsigned int	x	: 16;
	unsigned int	y	: 16;
	signed int	wheel	: 32;
	unsigned int	lmb	: 1;
	unsigned int	rmb	: 1;
} DARNIT_MOUSE;


typedef struct {
	unsigned int			up;
	unsigned int			down;
	unsigned int			left;
	unsigned int			right;
	unsigned int			x;
	unsigned int			y;
	unsigned int			a;
	unsigned int			b;
	unsigned int			start;
	unsigned int			select;
	unsigned int			l;
	unsigned int			r;
} DARNIT_INPUT_MAP;

DARNIT_KEYS d_keys_get();
void d_keys_set(DARNIT_KEYS keys);
DARNIT_KEYS d_keys_zero();
void d_input_grab();
void d_input_release();
DARNIT_MOUSE d_mouse_get();
void d_keymapping_reset();
void d_keymapping_set(DARNIT_INPUT_MAP map);
DARNIT_INPUT_MAP d_keymapping_get();
void d_joystick_get(int *js0_x, int *js0_y, int *js1_x, int *js1_y);
void d_key_raw_push(int sym, int action, int unicode);
DARNIT_KEY_RAW d_key_raw_pop();
void d_key_raw_clear();

/* The returned pointer is only valid until you call this function again */
const char *d_key_name_get(int sym);

#endif
