#include "darnit.h"


void menutkHighlightRecalculate(MENUTK_ENTRY *menu, int w, int h) {
	menu->hl.box.x2 = menu->swgran * w;
	menu->hl.box.x4 = menu->hl.box.x2;

	menu->hl.box.y3 = menu->shgran * h * -1.0f;
	menu->hl.box.y4 = menu->hl.box.y3;

	menu->hl.box.x1 = menu->hl.box.x3 = menu->hl.box.y1 = menu->hl.box.y2 = 0.0f;

	return;
}


void menutkHighlightMove(MENUTK_ENTRY *menu, int x, int y) {
	menu->hl.x = menu->swgran * x;
	menu->hl.y = menu->shgran * y * -1.0f;

	return;
}


void menutkSetColor(MENUTK_ENTRY *menu, int color) {
	if (menu == NULL) return;
	if (color == 0) {
		menu->hl.color[0].a = 1.0f;
		menu->hl.color[0].r = 0.8f;
		menu->hl.color[0].g = 0.65f;
		menu->hl.color[0].b = 0.2f;
		menu->hl.color[3] = menu->hl.color[2] = menu->hl.color[1] = menu->hl.color[0];
	} else if (color == 1) {
		menu->hl.color[0].r = 0.4f;
		menu->hl.color[0].g = 0.0f;
		menu->hl.color[0].b = 0.0f;
		menu->hl.color[0].a = 1.0f;
		menu->hl.color[1] = menu->hl.color[0];
		menu->hl.color[3] = menu->hl.color[2] = menu->hl.color[1];
	}

	return;
}
	


int menutkSelectionWidth(MENUTK_ENTRY *menu, int selection) {
	int i, len, tw, w, cnt;

	len = strlen(menu->str);
	tw = textFontGetW(menu->font);

	for (i = w = cnt = 0; i < len; i++) {
		if (menu->str[i] == '\n') {
			if (cnt == selection)
				return w*tw;
			cnt++;
		} else if (cnt == selection)
			w++;
	}

	return w*tw;
}			


int menutkSelectionPos(MENUTK_ENTRY *menu, int selection) {
	int i, len, tw, w, cnt;

	len = strlen(menu->str);
	tw = textFontGetW(menu->font);

	for (i = w = cnt = 0; i < len; i++) {
		if (menu->str[i] == '\n') {
			cnt++;
			w += 4;
		} else if (cnt == selection)
			return w*tw;
		else
			w++;
	}

	return w*tw;
}


void menutkSpinbuttonTextUpdate(MENUTK_ENTRY *menu) {
	int i, numl;
	char num[10];

	sprintf(num, "%i", menu->selection);
	numl = strlen(num);
	numl = menu->skip_option - numl;

	textResetSurface(menu->text);
	textSurfaceAppendString(menu->text, menu->str);
	textSurfaceAppendString(menu->text, " \x02 ");

	for (i = 0; i < numl; i++)
		textSurfaceAppendChar(menu->text, ' ');
	textSurfaceAppendString(menu->text, num);
	textSurfaceAppendString(menu->text, " \x03");

	return;
}


void menutkVerticalShadeReduceSelectionsByOne(MENUTK_ENTRY *menu) {
	if (menu == NULL) return;
	if (menu->selection == menu->options - 1)
		menu->selection--;
	menu->options--;
	menu->change = 1;
	menutkHighlightMove(menu, 0, menu->advance*menu->selection);

	return;
}


void menutkVerticalShadeIncreaseSelectionsByOne(MENUTK_ENTRY *menu) {
	if (menu == NULL) return;
	menu->options++;
	
	return;
}


void *menutkHorisontalCreate(void *handle, const char *options, int x, int y, TEXT_FONT *font, int color, int max) {
	DARNIT *m = handle;
	MENUTK_ENTRY *menu;
	int i, len, cnt;

	if ((menu = malloc(sizeof(MENUTK_ENTRY))) == NULL) {
		MALLOC_ERROR
		return NULL;
	}

	menu->orientation = MENUTK_ORIENT_H;
	menu->font = font;
	menu->x = x*m->video.swgran-1.0f, menu->y = y*m->video.shgran*-1.0f + 1.0f;
	menu->str = options;
	
	len = strlen(options);

	for (i = 0, cnt = 1; i < len; i++)
		if (options[i] == '\n')
			cnt++;
	
	menu->text = textMakeRenderSurface(len + cnt*3 + 10, menu->font, len + cnt*3, x+4, y);

	for (i = 0; i < len; i++) {
		if (options[i] != '\n')
			textSurfaceAppendChar(menu->text, options[i]);
		else
			textSurfaceAppendString(menu->text, "    ");
	}

	menu->waiting = 1;
	menu->selection = 0;
	menu->options = cnt;
	menu->skip_option = -1;
	menu->swgran = m->video.swgran, menu->shgran = m->video.shgran;
	menu->change = 1;
	menu->hidden = 0;
	menu->top_sel = 0;

	menutkHighlightRecalculate(menu, menutkSelectionWidth(menu, 0) + 8, textFontGetH(menu->font));
	menutkHighlightMove(menu, 0, 0);
	menutkSetColor(menu, color);

	return menu;
}


void *menutkVerticalShadeCreate(void *handle, int x, int y, int shadeh, int option_advance, int options, int menuw, int color, int skip_option) {
	DARNIT *m = handle;
	MENUTK_ENTRY *menu;

	if ((menu = malloc(sizeof(MENUTK_ENTRY))) == NULL) {
		MALLOC_ERROR
		return NULL;
	}

	menu->orientation = MENUTK_ORIENT_V_OL;
	menu->x = x*m->video.swgran-1.0f, menu->y = y*m->video.shgran*-1.0f + 1.0f;
	menu->waiting = 1;
	menu->selection = 0;
	menu->options = options;
	menu->advance = option_advance;
	menu->skip_option = skip_option;
	menu->swgran = m->video.swgran, menu->shgran = m->video.shgran;
	menu->change = 1;
	menu->hidden = 0;
	menu->text = NULL;
	menu->time = SDL_GetTicks();
	menu->autorep = 0;


	menutkHighlightRecalculate(menu, menuw, shadeh);
	menutkHighlightMove(menu, 0, 0);
	menutkSetColor(menu, color);

	if (skip_option == 0) {
		menu->selection = 1;
		menutkHighlightMove(menu, 0, option_advance);
	}
	
	return menu;
}


void *menutkVerticalCreate(void *handle, const char *options, int x, int y, TEXT_FONT *font, int menuw, int textskip_x, int color) {
	DARNIT *m = handle;
	MENUTK_ENTRY *menu;
	int i, len, cnt;

	if ((menu = malloc(sizeof(MENUTK_ENTRY))) == NULL) {
		MALLOC_ERROR
		return NULL;
	}
	
	menu->orientation = MENUTK_ORIENT_V;
	menu->font = font;
	menu->x = x*m->video.swgran-1.0f, menu->y = (y*m->video.shgran*-1.0f) + 1.0f;
	menu->str = options;

	len = strlen(options);

	for (i = 0, cnt = 1; i < len; i++) 
		if (options[i] == '\n')
			cnt++;
	
	menu->text = textMakeRenderSurface(len, menu->font, len, x + textskip_x, y);
	textSurfaceAppendString(menu->text, options);

	menu->waiting = 1;
	menu->selection = 0;
	menu->advance = textFontGetHS(menu->font);
	menu->skip_option = -1;
	menu->options = cnt;
	menu->swgran = m->video.swgran, menu->shgran = m->video.shgran;
	menu->change = 1;
	menu->hidden = 0;
	menu->time = SDL_GetTicks();
	menu->autorep = 0;

	menutkHighlightRecalculate(menu, menuw, textFontGetH(menu->font));
	menutkHighlightMove(menu, 0, 0);
	menutkSetColor(menu, color);

	return menu;
}


void *menutkSpinbuttonCreate(void *handle, const char *comment_text, int x, int y, TEXT_FONT *font, int step, int min, int max) {
	DARNIT *m = handle;
	int len;
	char tst[10];
	MENUTK_ENTRY *menu;

	if ((menu = malloc(sizeof(MENUTK_ENTRY))) == NULL) {
		MALLOC_ERROR
		return NULL;
	}

	menu->orientation = MENUTK_SPINBTN;
	menu->x = x*m->video.swgran-1.0f, menu->y = (y*m->video.shgran*-1.0f) + 1.0f;
	menu->str = comment_text;

	len = strlen(comment_text);
	sprintf(tst, "%i", max);
	menu->skip_option = strlen(tst);		/* Ugly reusage of this member. I don't feel like adding more members. */
	len += menu->skip_option;

	menu->waiting = 1;
	menu->selection = min;
	menu->advance = step;
	menu->options = max;
	menu->min = min;
	menu->change = 1;
	menu->hidden = 0;

	menu->text = textMakeRenderSurface(len + menu->skip_option + 5, font, len + menu->skip_option + 5, x, y);
	menutkSpinbuttonTextUpdate(menu);

	return menu;
}


void *menutkTextinputCreate(int x, int y, TEXT_FONT *font, char *buf, int buf_len, int field_len) {
	MENUTK_ENTRY *menu;
	int i;

	if ((menu = malloc(sizeof(MENUTK_ENTRY))) == NULL) {
		MALLOC_ERROR
		return NULL;
	}
	
	menu->textinput_buf = buf;
	menu->text = textMakeRenderSurface(field_len, font, field_len+1, x, y);
	menu->waiting = 1;
	menu->top_sel = 0;
	menu->selection = 0;
	menu->hidden = 0;
	menu->change = 1;
	menu->options = buf_len - 1;
	menu->xi = x; menu->yi = y;
	menu->orientation = MENUTK_TEXTINPUT;
	menu->textinput_buf[menu->options] = 0; 

	for (i = 0; i < menu->options; i++)
		if (menu->textinput_buf[i] == 0)
			break;

	menu->textinput_buf_use = i;
	renderCalcTileCache(&menu->text_cursor, font->ts, 4);		/* 4 is the assumed value for showing a white box the size of a glyph */

	return menu;
}


void menutkInputH(void *handle, MENUTK_ENTRY *menu) {
	DARNIT *m = handle;


	if ((m->input.key ^ m->input.keypending) & BUTTON_ACCEPT) {
		menu->waiting = 0;
		m->input.keypending |= BUTTON_ACCEPT;
	} else if ((m->input.key ^ m->input.keypending) & BUTTON_CANCEL) {
		menu->waiting = 0;
		menu->selection = -2;
		m->input.keypending |= BUTTON_CANCEL;
	} else if ((m->input.key ^ m->input.keypending) & KEY_RIGHT) {
		menu->selection++;
		if (menu->selection == menu->options)
			menu->selection = 0;
		m->input.keypending |= KEY_RIGHT;
	} else if ((m->input.key ^ m->input.keypending) & KEY_LEFT) {
		menu->selection--;
		if (menu->selection == -1)
			menu->selection = menu->options - 1;
		m->input.keypending |= KEY_LEFT;
	} else
		return;


	menu->change = 1;
	if (menu->selection > -1) {
		menutkHighlightRecalculate(menu, menutkSelectionWidth(menu, menu->selection) + 8, textFontGetH(menu->font));
		menutkHighlightMove(menu, menutkSelectionPos(menu, menu->selection), 0);
	}
	
	return;
}


void menutkInputV(void *handle, MENUTK_ENTRY *menu) {
	DARNIT *m = handle;
	unsigned int step = SDL_GetTicks();
	
	step -= menu->time;

	if ((m->input.key ^ m->input.keypending) & BUTTON_ACCEPT) {
		menu->waiting = 0;
		m->input.keypending |= BUTTON_ACCEPT;
	} else if ((m->input.key ^ m->input.keypending) & BUTTON_CANCEL) {
		menu->waiting = 0;
		menu->selection = -2;
		m->input.keypending |= BUTTON_CANCEL;
	} else if (((m->input.key ^ m->input.keypending) & KEY_DOWN) || ((m->input.key & KEY_DOWN) && menu->autorep && step >= MENUTK_AUTOREP_STEP)) {
		menu->selection++;
		if (menu->selection == menu->skip_option)
			menu->selection++;
		if (menu->selection == menu->options)
			menu->selection = 0;
		if (menu->selection == menu->skip_option)
			menu->selection++;
		m->input.keypending |= KEY_DOWN;
	} else if (((m->input.key ^ m->input.keypending) & KEY_UP) || ((m->input.key & KEY_UP) && menu->autorep && step >= MENUTK_AUTOREP_STEP)) {
		menu->selection--;
		if (menu->selection == menu->skip_option)
			menu->selection--;
		if (menu->selection < 0)
			menu->selection = menu->options - 1;
		if (menu->selection == menu->skip_option)
			menu->selection--;
		m->input.keypending |= KEY_UP;
	} else {
		if (menu->autorep && step >= MENUTK_AUTOREP_STEP)
			menu->autorep = 0;
		else if (menu->autorep != 1 && step >= MENUTK_AUTOREP_DELAY && (m->input.key & (KEY_UP | KEY_DOWN))) {
			menu->time = SDL_GetTicks();
			menu->autorep = 1;
		}
		return;
	}

	if (menu->autorep == 1)
		menu->time = SDL_GetTicks();
	menu->change = 1;
	if (menu->selection > -1)
		menutkHighlightMove(menu, 0, menu->advance*menu->selection);

	return;
}


void menutkSpinbuttonInput(void *handle, MENUTK_ENTRY *menu) {
	DARNIT *m = handle;

	
	if ((m->input.key ^ m->input.keypending) & BUTTON_ACCEPT) {
		menu->waiting = 0;
		m->input.keypending |= BUTTON_ACCEPT;
	} else if ((m->input.key ^ m->input.keypending) & BUTTON_CANCEL) {
		menu->waiting = 0;
		menu->selection = -2;
		m->input.keypending |= BUTTON_CANCEL;
	} else if ((m->input.key ^ m->input.keypending) & KEY_UP) {
		if (menu->selection == menu->options)
			menu->selection = menu->min;
		else
			menu->selection += menu->advance;
		if (menu->selection > menu->options)
			menu->selection = menu->options;
		m->input.keypending |= KEY_UP;
	} else if ((m->input.key ^ m->input.keypending) & KEY_DOWN) {
		m->input.keypending |= KEY_DOWN;
		if (menu->selection == menu->min)
			menu->selection = menu->options;
		else
			menu->selection -= menu->advance;
		if (menu->selection < menu->min)
			menu->selection = menu->min;
	} else if ((m->input.key ^ m->input.keypending) & KEY_RIGHT) {
		m->input.keypending |= KEY_RIGHT;
		menu->selection = menu->options>>1;
		if (menu->selection < menu->min)
			menu->selection = menu->min;
	} else
		return;
	
	menu->change = 1;
	menutkSpinbuttonTextUpdate(menu);

	return;
}


void menutkTextinputInput(void *handle, MENUTK_ENTRY *menu) {
	DARNIT *m = handle;
	unsigned int key, i, time;

	key = inputASCIIPop(handle);

	if (key == SDLK_RETURN) {
		menu->waiting = 0;
		menu->change = 1;
		menu->cursor_display = 0;
	} else if (key == 0);
	else if (key == '\b') {
		if (menu->selection > 0) {
			menu->selection--;
			for (i = menu->selection; i < menu->textinput_buf_use - 1; i++) 
				menu->textinput_buf[i] = menu->textinput_buf[i+1];
			menu->textinput_buf[i] = 0;
			if (menu->selection <= menu->top_sel) menu->top_sel = menu->selection - menu->text->len + 2;
			if (menu->top_sel < 0) menu->top_sel = 0;
			menu->textinput_buf_use--;
		}
	} else if (menu->textinput_buf_use == menu->options);
	else {
		if (menu->textinput_buf_use > menu->selection) {
			for (i = menu->textinput_buf_use; i > menu->selection; i--)
				menu->textinput_buf[i] = menu->textinput_buf[i-1];
		} else
			menu->textinput_buf[menu->selection + 1] = 0;

		menu->textinput_buf[menu->selection] = key;
		menu->textinput_buf_use++;
		menu->selection++;
		if (menu->selection >= (menu->top_sel + menu->text->len)) menu->top_sel = menu->selection - menu->text->len;
		menu->change = 1;
	}
	
	if (menu->top_sel < 0) menu->top_sel = 0;
	if (menu->top_sel + menu->text->len - 1 < menu->selection) menu->top_sel++;

	textResetSurface(menu->text);

	for (i = 0; i < menu->text->len; i++) {
		if (menu->textinput_buf[i + menu->top_sel] == 0)
			break;
		textSurfaceAppendChar(menu->text, menu->textinput_buf[i + menu->top_sel]);
	}

	time = SDL_GetTicks();
	if (menu->waiting == 1)
		menu->cursor_display = ((time % 1000) >= 500) ? 1 : 0;
	else
		menu->cursor_display = 0;

	menu->textinput_buf[menu->textinput_buf_use] = 0;
	renderCalcTilePosCache(&menu->text_cursor, menu->text->font->ts, menu->xi - menu->top_sel*menu->text->font->ts->wsq + menu->selection*menu->text->font->ts->wsq, menu->yi);

	if ((m->input.key ^ m->input.keypending) & BUTTON_ACCEPT) {
		menu->waiting = 0;
		m->input.keypending |= BUTTON_ACCEPT;
		menu->cursor_display = 0;
	} else if ((m->input.key ^ m->input.keypending) & BUTTON_CANCEL) {
		menu->waiting = 0;
		menu->selection = -2;
		menu->cursor_display = 0;
		m->input.keypending |= BUTTON_CANCEL;
	} else if ((m->input.key ^ m->input.keypending) & KEY_LEFT) {
		if (menu->selection > 0)
			menu->selection--;
		m->input.keypending |= KEY_LEFT;
		if (menu->selection <= menu->top_sel) menu->top_sel = menu->selection - menu->text->len/2;
		if (menu->top_sel < 0) menu->top_sel = 0;
	} else if ((m->input.key ^ m->input.keypending) & KEY_RIGHT) {
		if (menu->selection < menu->textinput_buf_use)
			menu->selection++;
		m->input.keypending |= KEY_RIGHT;
		if (menu->selection >= (menu->top_sel + menu->text->len) && menu->selection < menu->textinput_buf_use - 1) menu->top_sel = menu->selection - menu->text->len/2;
	} else
		return;

		
	
	menu->change = 1;

	return;
}

int menutkMenuRoutine(void *handle, MENUTK_ENTRY *menu) {
	DARNIT *m = handle; 

	if (menu == NULL)
		return -2;

	if (menu->waiting == 1) {
		if (menu->orientation == MENUTK_ORIENT_H)
			menutkInputH(m, menu);
		else if (menu->orientation == MENUTK_ORIENT_V || menu->orientation == MENUTK_ORIENT_V_OL)
			menutkInputV(m, menu);
		else if (menu->orientation == MENUTK_SPINBTN)
			menutkSpinbuttonInput(m, menu);
		else if (menu->orientation == MENUTK_TEXTINPUT)
			menutkTextinputInput(m, menu);
	}

	if (menu->hidden == 1)
		return -1;

	if (menu->orientation == MENUTK_TEXTINPUT && menu->cursor_display == 1)
		renderCache(&menu->text_cursor, menu->text->font->ts, 1);

	/* glLoadIdentity(); */
	if (menu->orientation != MENUTK_SPINBTN && menu->orientation != MENUTK_TEXTINPUT) {
		glTranslatef(menu->x + menu->hl.x, menu->y + menu->hl.y, 0.0f);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &menu->hl.box);
		glColorPointer(4, GL_FLOAT, 0, menu->hl.color);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glLoadIdentity();
		glTranslatef(m->video.swgran * m->video.offset_x, m->video.shgran * m->video.offset_y, 0.0f);
		glColor4f(m->video.tint_r, m->video.tint_g, m->video.tint_b, m->video.tint_a);
	}


	if (menu->orientation != MENUTK_ORIENT_V_OL)
		textRender(menu->text);

	if (menu->waiting == 1)
		return -1;
		
	return menu->selection;
}


void *menutkDestroy(MENUTK_ENTRY *menu) {
	if (menu == NULL)
		return NULL;

	textSurfaceDestroy(menu->text);
	free(menu);
	
	return NULL;
}


void menutkWaitForNewSelection(MENUTK_ENTRY *menu) {
	if (menu == NULL)
		return;

	menu->waiting = 1;

	return;
}

int menutkPeek(MENUTK_ENTRY *menu) {
	if (menu == NULL)
		return -2;

	return menu->selection;
}


int menutkGetIfChanged(MENUTK_ENTRY *menu) {
	if (menu == NULL)
		return 0;

	if (menu->change == 1) {
		menu->change = 0;
		return 1;
	}

	return menu->change;
}


void menutkMenuHide(MENUTK_ENTRY *menu) {
	if (menu == NULL)
		return;
	
	menu->hidden = 1;

	return;
}


void menutkMenuUnhide(MENUTK_ENTRY *menu) {
	if (menu == NULL)
		return;
	
	menu->hidden = 0;
	
	return;
}
