/*
Copyright (c) 2011-2013 Steven Arnow
'menutk.h' - This file is part of libdarnit

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
	TILE_CACHE		*tile_cache;
	TILESHEET		*ts;
} MENUTK_HIGHLIGHT;


typedef struct {
	int			orientation;
	TEXT_FONT		*font;
	char			*str;
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
	unsigned int		*codepoint;
	int			codepoint_use;
	char			*textinput_buf;
	int			textinput_buf_use;
	int			top_sel;
	int			xi;
	int			yi;
	int			cursor_display;

	int			option_space;
	int			scroll_threshold;
} MENUTK_ENTRY;


void menutkVerticalShadeReduceSelectionsByOne(MENUTK_ENTRY *menu;);
void menutkVerticalShadeIncreaseSelectionsByOne(MENUTK_ENTRY *menu);
void *menutkHorisontalCreate(const char *options, int x, int y, TEXT_FONT *font, int color, int max);
void *menutkVerticalShadeCreate(int x, int y, int shadeh, int option_advance, int options, int menuw, int color, int skip_option, int max_h);
void *menutkVerticalCreate(const char *options, int x, int y, TEXT_FONT *font, int menuw, int textskip_x, int color, int max_h);
void *menutkSpinbuttonCreate(const char *comment_text, int x, int y, TEXT_FONT *font, int step, int min, int max);
void *menutkTextinputCreate(int x, int y, TEXT_FONT *font, char *buf, int buf_len, int field_len);
int menutkMenuRoutine(MENUTK_ENTRY *menu);
void *menutkDestroy(MENUTK_ENTRY *menu);
void menutkWaitForNewSelection(MENUTK_ENTRY *menu);
int menutkPeek(MENUTK_ENTRY *menu, int *top_sel);
int menutkGetIfChanged(MENUTK_ENTRY *menu);
void menutkMenuHide(MENUTK_ENTRY *menu);
void menutkMenuUnhide(MENUTK_ENTRY *menu);


#endif
