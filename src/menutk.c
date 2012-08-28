#include "darnit.h"


void menutkHighlightRecalculate(MENUTK_ENTRY *menu, int w, int h) {
	menu->hl.box.x1 = menu->hl.box.x3 = 0.0f;
	menu->hl.box.x2 = menu->swgran * w;
	menu->hl.box.x4 = menu->hl.box.x2;

	menu->hl.box.y1 = menu->hl.box.y3 = 0.0f;
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


void menutkTopSelRecalc(MENUTK_ENTRY *menu) {
	int i;
	char *start, *end;

	if (menu->str != NULL && menu->scroll_threshold > 0) {
		if (menu->selection - menu->top_sel >= menu->scroll_threshold)
			menu->top_sel = menu->selection - menu->scroll_threshold + 1;
		if (menu->selection < menu->top_sel)
			menu->top_sel = menu->selection;
		for (i = 0, start = menu->str; i < menu->top_sel; i++)
			start = strstr(start, "\n") + 1;
		for (i = 0, end = start; i < menu->scroll_threshold; i++, end++)
			if ((end = strstr(end, "\n")) == NULL)
				break;
		if (end != NULL) {
			end--;
			*end = 0;
		}
		textResetSurface(menu->text);
		textSurfaceAppendString(menu->text, start);
		if (end != NULL)
			*end = '\n';
	} else if (menu->scroll_threshold == 0 && menu->str != NULL) {
		textResetSurface(menu->text);
		textSurfaceAppendString(menu->text, menu->str);
	}

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
	int i, len, w, cnt;

	len = strlen(menu->str);

	for (i = w = cnt = 0; i < len; i += utf8GetValidatedCharLength(&menu->str[i])) {
		if (menu->str[i] == '\n') {
			if (cnt == selection)
				return w;
			cnt++;
		} else if (cnt == selection)
			w += textGetGlyphWidth(menu->text->font, utf8GetChar(&menu->str[i]));
	}

	return w;
}			


int menutkSelectionPos(MENUTK_ENTRY *menu, int selection) {
	int i, len, w, cnt;

	len = strlen(menu->str);

	for (i = w = cnt = 0; i < len;) {
		if (menu->str[i] == '\n') {
			cnt++;
			w += (menu->option_space);
		} else if (cnt == selection)
			return w;
		else
			w += textGetGlyphWidth(menu->text->font, utf8GetChar(&menu->str[i]));
		i += utf8GetValidatedCharLength(&menu->str[i]);
	}

	return w;
}


void menutkSpinbuttonTextUpdate(MENUTK_ENTRY *menu) {
	int i, numl;
	char num[10];
	char *space = " ";

	sprintf(num, "%i", menu->selection);
	numl = strlen(num);
	numl = menu->skip_option - numl;

	textResetSurface(menu->text);
	textSurfaceAppendString(menu->text, menu->str);
	textSurfaceAppendString(menu->text, " ∨ ");

	for (i = 0; i < numl; i++)
		textSurfaceAppendChar(menu->text, space);
	textSurfaceAppendString(menu->text, num);
	textSurfaceAppendString(menu->text, " ∧");

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
	menu->str = malloc(strlen(options) + 1);
	strcpy(menu->str, options);
	
	len = strlen(options);

	for (i = 0, cnt = 1; i < len; i++)
		if (options[i] == '\n')
			cnt++;
	
	menu->text = textMakeRenderSurface(len + 10, menu->font, ~0, x+4, y);
	menu->option_space = menu->text->font->font_height * 2;

	for (i = 0; i < len;) {
		if (options[i] != '\n')
			i += textSurfaceAppendChar(menu->text, &options[i]);
		else {
			i++;
			textSurfaceSkip(menu->text, menu->text->font->font_height * 2); 
		}
	}

	menu->waiting = 1;
	menu->selection = 0;
	menu->options = cnt;
	menu->skip_option = -1;
	menu->swgran = m->video.swgran, menu->shgran = m->video.shgran;
	menu->change = 1;
	menu->hidden = 0;
	menu->top_sel = 0;
	menu->scroll_threshold = max;

	menutkHighlightRecalculate(menu, menutkSelectionWidth(menu, 0) + 8, menu->font->ascent - menu->font->descent + 4);
	menutkHighlightMove(menu, 0, 0);
	menutkSetColor(menu, color);

	return menu;
}


void menutkAdjustTextinputCursor(void *handle, MENUTK_ENTRY *menu) {
	DARNIT *m = handle;
	if (menu == NULL) return;

	menu->hl.color[0].a = m->video.tint_a;
	menu->hl.color[0].r = m->video.tint_r;
	menu->hl.color[0].g = m->video.tint_g;
	menu->hl.color[0].b = m->video.tint_b;
	menu->hl.color[1] = menu->hl.color[2] = menu->hl.color[3] = menu->hl.color[0];

	return;
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
	menu->top_sel = 0;

	menu->options = options;
	menu->advance = option_advance;
	menu->skip_option = skip_option;
	menu->swgran = m->video.swgran, menu->shgran = m->video.shgran;
	menu->change = 1;
	menu->hidden = 0;
	menu->text = NULL;
	menu->time = SDL_GetTicks();
	menu->autorep = 0;
	menu->str = NULL;


	menutkHighlightRecalculate(menu, menuw, shadeh);
	menutkHighlightMove(menu, 0, 0);
	menutkSetColor(menu, color);

	if (skip_option == 0) {
		menu->selection = 1;
		menutkHighlightMove(menu, 0, option_advance);
	}
	
	return menu;
}


void *menutkVerticalCreate(void *handle, const char *options, int x, int y, TEXT_FONT *font, int menuw, int textskip_x, int color, int max_h) {
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

	menu->str = malloc(strlen(options) + 1);
	strcpy(menu->str, options);

	len = strlen(options);

	for (i = 0, cnt = 1; i < len; i++) 
		if (options[i] == '\n')
			cnt++;
	
	menu->text = textMakeRenderSurface(len, menu->font, ~0, x + textskip_x, y);

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
	menu->top_sel = 0;
	menu->scroll_threshold = max_h;

	menutkTopSelRecalc(menu);

	menutkHighlightRecalculate(menu, menuw, textFontGetHS(menu->font) + 4);
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
	menu->str = malloc(strlen(comment_text) + 1);
	strcpy(menu->str, comment_text);

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

	menu->text = textMakeRenderSurface(len + menu->skip_option + 5, font, ~0, x, y);
	menutkSpinbuttonTextUpdate(menu);

	return menu;
}


void *menutkTextinputCreate(int x, int y, TEXT_FONT *font, char *buf, int buf_len, int field_len) {
	DARNIT *m = font->handle;
	MENUTK_ENTRY *menu;
	int i;

	if ((menu = malloc(sizeof(MENUTK_ENTRY))) == NULL) {
		MALLOC_ERROR
		return NULL;
	}

	menu->x = x*m->video.swgran-1.0f, menu->y = (y*m->video.shgran*-1.0f) + 1.0f;
	menu->textinput_buf = buf;
	menu->font = font;
	menu->text = textMakeRenderSurface(field_len>>1, font, ~0, x, y);
	menu->waiting = 1;
	menu->top_sel = 0;
	menu->selection = 0;
	menu->hidden = 0;
	menu->change = 1;
	menu->cursor_display = 0;
	menu->options = buf_len - 1;
	menu->swgran = m->video.swgran;
	menu->shgran = m->video.shgran;

	menu->codepoint = malloc(sizeof(unsigned int) * buf_len);

	menu->xi = x; menu->yi = y;
	menu->orientation = MENUTK_TEXTINPUT;
	menu->textinput_buf[menu->options] = 0; 
	menu->textinput_buf_use = 0;

	for (i = 0; i < menu->options; i++) {
		if (menu->textinput_buf[menu->textinput_buf_use] == 0)
			break;
		menu->codepoint[i] = utf8GetChar(&buf[menu->textinput_buf_use]);
		menu->textinput_buf_use += utf8GetValidatedCharLength(&buf[menu->textinput_buf_use]);
	}

	menu->codepoint_use = i;
	menu->codepoint[i] = 0;
	menutkAdjustTextinputCursor(m, menu);
	menutkHighlightMove(menu, 0, 0);
	menu->selection = -1;
	menu->str = NULL;

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
		menutkHighlightRecalculate(menu, menutkSelectionWidth(menu, menu->selection) + 8, menu->font->ascent - menu->font->descent + 4);
		menutkHighlightMove(menu, menutkSelectionPos(menu, menu->selection)-menu->font->ascent+menu->font->font_height-4, 0);
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
		} else if (!(m->input.key & (KEY_UP | KEY_DOWN))) {
			menu->autorep = 0;
			menu->time = SDL_GetTicks();
		}
		return;
	}

	menutkTopSelRecalc(menu);

	if (menu->autorep == 1)
		menu->time = SDL_GetTicks();
	menu->change = 1;
	if (menu->selection > -1)
		menutkHighlightMove(menu, 0, menu->advance*(menu->selection - menu->top_sel));

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


void menutkUpdateCursorBlink(MENUTK_ENTRY *menu) {
	unsigned int time;

	time = SDL_GetTicks();
	if (menu->waiting == 1)
		menu->cursor_display = ((time % 1000) >= 500) ? 1 : 0;
	else
		menu->cursor_display = 0;

	return;
}


void menutkTextinputInput(void *handle, MENUTK_ENTRY *menu) {
	DARNIT *m = handle;
	unsigned int key, i, tmp, change;

	key = inputASCIIPop(handle);
	change = 1;

	if (key == 127 && menu->selection >= 0) {
		if (menu->selection < menu->codepoint_use) {
			key = '\b';
			menu->selection++;
		} else
			key = 0;
	}

	if (menu->selection == -1);
	else if (key == SDLK_RETURN) {
		menu->waiting = 0;
		menu->cursor_display = 0;
	} else if (key == 0) {
		menutkUpdateCursorBlink(menu);
	} else if (key == '\b') {
		if (menu->selection > 0) {
			menu->textinput_buf_use -= utf8EncodedLength(menu->codepoint[menu->selection]);
			menu->selection--;
			for (i = menu->selection; i < menu->codepoint_use; i++) 
				menu->codepoint[i] = menu->codepoint[i+1];
			menu->codepoint[i-1] = 0;
			menu->codepoint_use -= 1;
		}
	} else if (menu->textinput_buf_use == menu->options);
	else if (key < 0x20);
	else {
		if (menu->codepoint_use > menu->selection) {
			for (i = menu->codepoint_use; i > menu->selection; i--)
				menu->codepoint[i] = menu->codepoint[i-1];
		} else
			menu->codepoint[menu->selection + 1] = 0;

		menu->codepoint[menu->selection] = key;
		menu->codepoint_use++;
		menu->codepoint[menu->codepoint_use] = 0;
		menu->textinput_buf_use += utf8EncodedLength(key);
		menu->selection++;
	}


	if (menu->selection == -1);
	else if ((m->input.key ^ m->input.keypending) & BUTTON_ACCEPT) {
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
	} else if ((m->input.key ^ m->input.keypending) & KEY_RIGHT) {
		if (menu->selection < menu->codepoint_use)
			menu->selection++;
		m->input.keypending |= KEY_RIGHT;
	} else
		change = 0;

	if (menu->selection <= menu->top_sel && menu->selection != 0) {
		if (menu->selection == -1)
			menu->selection = 0;
		for (i = menu->top_sel, tmp = 0; i > 0; i--) {
			tmp += textGetGlyphWidth(menu->font, menu->codepoint[i]);
			if (tmp >= menu->text->len)
				break;
		}
		menu->top_sel = i;
		change = 1;
	}
	
	
	for (i = menu->top_sel, tmp = 0; i < menu->codepoint_use; i++) {
		if (tmp + textGetGlyphWidth(menu->font, menu->codepoint[i]) >= (menu->text->len << 1))
			break;
		tmp += textGetGlyphWidth(menu->font, menu->codepoint[i]);
	}
	if (i == menu->codepoint_use)
		if (tmp + textGetGlyphWidth(menu->font, 0) < (menu->text->len << 1))
			i++;

	if (menu->selection >= i && menu->selection < menu->codepoint_use+1 && i != menu->codepoint_use + 1)
		menu->top_sel = menu->top_sel + ((i - menu->top_sel) >> 1);
	

	menutkHighlightRecalculate(menu, textGetGlyphWidth(menu->font, menu->codepoint[menu->selection]), textFontGetH(menu->font));
	for (i = tmp = 0; i < (menu->selection - menu->top_sel); i++)
		tmp += textGetGlyphWidth(menu->font, menu->codepoint[menu->top_sel + i]);
	tmp++;
	
	menutkHighlightMove(menu, tmp+1, 0);
	
	if (change == 0 && key == 0)
		return;

	textResetSurface(menu->text);
	
	tmp = 0;
	for (i = 0; i < menu->text->len; i++) {
		if (i + menu->top_sel >= menu->codepoint_use)
			break;
		if (menu->codepoint[menu->top_sel + i] == 0)
			break;
		tmp += textGetGlyphWidth(menu->font, menu->codepoint[menu->top_sel + i]);
		if (tmp >= menu->text->len << 1)
			break;
		textSurfaceAppendCodepoint(menu->text, menu->codepoint[i + menu->top_sel]);

	}

	if (key != 0)			/* Text buffer actually changed! */
		for (i = tmp = 0; i < menu->codepoint_use; i++)
			tmp += utf8Encode(menu->codepoint[i], &menu->textinput_buf[tmp], menu->options - tmp);

	menu->textinput_buf[tmp] = 0;

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

	/* glLoadIdentity(); */
	if ((menu->orientation != MENUTK_SPINBTN && menu->orientation != MENUTK_TEXTINPUT) || (menu->orientation == MENUTK_TEXTINPUT && menu->cursor_display)) {
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
		glTranslatef(-menu->hl.x, -menu->hl.y, 0.0f);
		glColor4f(m->video.tint_r, m->video.tint_g, m->video.tint_b, m->video.tint_a);
	}


	glLoadIdentity();
	glTranslatef(m->video.swgran * m->video.offset_x, m->video.shgran * m->video.offset_y, 0.0f);
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
	free(menu->str);
	free(menu);
	
	return NULL;
}


void menutkWaitForNewSelection(MENUTK_ENTRY *menu) {
	if (menu == NULL)
		return;

	menu->waiting = 1;
	menu->changed = 1;
	if (menu->selection == -2)
		menu->selection = 0;
	menutkTopSelRecalc(menu);
	menutkHighlightMove(menu, 0, menu->advance*(menu->selection - menu->top_sel));

	return;
}

int menutkPeek(MENUTK_ENTRY *menu, int *top_sel) {
	if (menu == NULL)
		return -2;

	if (top_sel != NULL)
		*top_sel = menu->top_sel;

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
