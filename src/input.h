#ifndef __INPUT_H__
#define	__INPUT_H__

// TODO: Sara wants an escape key!
#define		KEY_LEFT		0x1
#define		KEY_RIGHT		0x2
#define		KEY_UP			0x4
#define		KEY_DOWN		0x8
#define		KEY_X			0x10
#define		KEY_Y			0x20
#define		KEY_A			0x40
#define		KEY_B			0x80
#define		KEY_START		0x100
#define		KEY_SELECT		0x200
#define		KEY_L			0x400
#define		KEY_R			0x800
#define		MB_LEFT			0x1000
#define		MB_RIGHT_D		0x2000


#define		BUTTON_ACCEPT		KEY_B
#define		BUTTON_CANCEL		KEY_A

#define DARNIT_KEYACTION_PRESS		1
#define DARNIT_KEYACTION_RELEASE	2

#define		RAW_BUFFER_LEN		8

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
} INPUT_MAP;


typedef struct {
	int				nub0_x;
	int				nub0_y;
	int				nub1_x;
	int				nub1_y;
	SDL_Joystick			*nub0;
	SDL_Joystick			*nub1;
	int				nub0i;
	int				nub1i;
} INPUT_JOYSTICK;


typedef struct {
	int				raw[RAW_BUFFER_LEN];
	int				action[RAW_BUFFER_LEN];
	unsigned int			use;
} INPUT_RAW;


typedef struct {
	SDL_Event			event;
	unsigned int			key;
	unsigned int			keypending;
	unsigned int			upper;
	unsigned int			lastkey;
	DARNIT_MOUSE			mouse;
	INPUT_MAP			map;
	INPUT_JOYSTICK			js;
	INPUT_RAW			raw;
} INPUT_STRUCT;


void inputRawPush(int sym, int action);
int inputRawPop(int *action);
void inputPoll();
int inputInit();
unsigned int inputASCIIPop();
void inputKeymapReset();

#endif
