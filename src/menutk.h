#ifndef __MENUTK_H__
#define	__MENUTK_H__


#define				MENUTK_ORIENT_H		0
#define				MENUTK_ORIENT_V		1
#define				MENUTK_ORIENT_V_OL	2
#define				MENUTK_SPINBTN		3
#define				MENUTK_TEXTINPUT	4


#define				MENUTK_COLOR_DEF	0
#define				MENUTK_COLOR_SPECIAL	1


#define				MENUTK_AUTOREP_DELAY	1000
#define				MENUTK_AUTOREP_STEP	60


typedef struct {
	float			r, g, b, a;
} MENUTK_COLOR;


typedef struct {
	float			x1, y1, x2, y2, x3, y3, x4, y4;
} MENUTK_BOX;


typedef struct {
	MENUTK_COLOR		color[4];
	MENUTK_BOX		box;
	float			x, y;
	int			entryh;
} MENUTK_HIGHLIGHT;


typedef struct {
	int			orientation;
	TEXT_FONT		*font;
	const char		*str;
	float			x;
	float			y;
	int			options;
	int			selection;
	int			waiting;
	int			advance;
	int			skip_option;
	int			change;
	int			min;
	int			hidden;
	unsigned int		time;
	int 			autorep;
	TEXT_SURFACE		*text;
	MENUTK_HIGHLIGHT	hl;
	float			swgran, shgran;
	TILE_CACHE		text_cursor;
	char			*textinput_buf;
	int			textinput_buf_use;
	int			top_sel;
	int			xi;
	int			yi;
	int			cursor_display;

	int			option_space;
} MENUTK_ENTRY;


void menutkVerticalShadeReduceSelectionsByOne(MENUTK_ENTRY *menu;);
void menutkVerticalShadeIncreaseSelectionsByOne(MENUTK_ENTRY *menu);
void *menutkHorisontalCreate(void *handle, const char *options, int x, int y, TEXT_FONT *font, int color, int max);
void *menutkVerticalShadeCreate(void *handle, int x, int y, int shadeh, int option_advance, int options, int menuw, int color, int skip_option);
void *menutkVerticalCreate(void *handle, const char *options, int x, int y, TEXT_FONT *font, int menuw, int textskip_x, int color);
void *menutkSpinbuttonCreate(void *handle, const char *comment_text, int x, int y, TEXT_FONT *font, int step, int min, int max);
void *menutkTextinputCreate(int x, int y, TEXT_FONT *font, char *buf, int buf_len, int field_len);
int menutkMenuRoutine(void *handle, MENUTK_ENTRY *menu);
void *menutkDestroy(MENUTK_ENTRY *menu);
void menutkWaitForNewSelection(MENUTK_ENTRY *menu);
int menutkPeek(MENUTK_ENTRY *menu);
int menutkGetIfChanged(MENUTK_ENTRY *menu);
void menutkMenuHide(MENUTK_ENTRY *menu);
void menutkMenuUnhide(MENUTK_ENTRY *menu);


#endif
