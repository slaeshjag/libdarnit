#ifndef __DARNIT_INPUT_H__
#define	__DARNIT_INPUT_H__

#define		BUTTON_ACCEPT		b
#define		BUTTON_CANCEL		a


typedef struct {
	unsigned int	left	: 1;
	unsigned int	right	: 1;
	unsigned int	up	: 1;
	unsigned int	down	: 1;
	unsigned int	x	: 1;
	unsigned int	y	: 1;
	unsigned int	a	: 1;
	unsigned int	b	: 1;
	unsigned int	start	: 1;
	unsigned int	select	: 1;
	unsigned int	l	: 1;
	unsigned int	r	: 1;
	unsigned int	lmb	: 1;
	unsigned int	rmb	: 1;
} DARNIT_KEYS;


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



DARNIT_KEYS darnitButtonGet();
void darnitButtonSet(DARNIT_KEYS buttons);
DARNIT_KEYS darnitButtonZero();
void darnitInputGrab();
void darnitInputUngrab();
DARNIT_MOUSE darnitMouseGet();
void darnitButtonMappingReset();
void darnitButtonMappingSet(DARNIT_INPUT_MAP map);
DARNIT_INPUT_MAP darnitButtonMappingGet();
void darnitJoystickGet(int *js0_x, int *js0_y, int *js1_x, int *js1_y);
void darnitKeyboardRawPush(int sym);
int darnitKeyboardRawPop();
void darnitKeyboardRawClear();

#endif
