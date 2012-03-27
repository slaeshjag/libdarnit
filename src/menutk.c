#include "darner.h"


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
	if (color == 0) {
//		menu->hl.color[0].r = menu->hl.color[0].g = menu->hl.color[0].b = 0.2f;
		menu->hl.color[0].a = 1.0f;
		menu->hl.color[0].r = 0.8f;
		menu->hl.color[0].g = 0.65f;
		menu->hl.color[0].b = 0.2f;
//		menu->hl.color[1] = menu->hl.color[0];
		menu->hl.color[3] = menu->hl.color[2] = menu->hl.color[1] = menu->hl.color[0];
//		menu->hl.color[2].r = menu->hl.color[2].g = menu->hl.color[2].b = 0.4f;
//		menu->hl.color[2].a = 1.0f;
//		menu->hl.color[3] = menu->hl.color[2];
	} else if (color == 1) {
		menu->hl.color[0].r = 0.4f;
		menu->hl.color[0].g = 0.0f;
		menu->hl.color[0].b = 0.0f;
		menu->hl.color[0].a = 1.0f;
		menu->hl.color[1] = menu->hl.color[0];
//		menu->hl.color[2].r = 0.6f;
//		menu->hl.color[2].g = 0.2f;
//		menu->hl.color[2].b = 0.2f;
//		menu->hl.color[2].a = 1.0f;
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
	if (menu->selection == menu->options - 1)
		menu->selection--;
	menu->options--;
	menu->change = 1;
	menutkHighlightMove(menu, 0, menu->advance*menu->selection);

	return;
}


void *menutkHorisontalCreate(void *handle, const char *options, int x, int y, TEXT_FONT *font, int color) {
	DARNER *m = handle;
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

	menutkHighlightRecalculate(menu, menutkSelectionWidth(menu, 0) + 8, textFontGetH(menu->font));
	menutkHighlightMove(menu, 0, 0);
	menutkSetColor(menu, color);

	return menu;
}


void *menutkVerticalShadeCreate(void *handle, int x, int y, int shadeh, int option_advance, int options, int menuw, int color, int skip_option) {
	DARNER *m = handle;
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
	DARNER *m = handle;
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
	DARNER *m = handle;
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
	menu->skip_option = strlen(tst);		// Ugly reusage of this member. I don't feel like adding more members.
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



void menutkInputH(void *handle, MENUTK_ENTRY *menu) {
	DARNER *m = handle;


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
	DARNER *m = handle;
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
	DARNER *m = handle;

	
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




int menutkMenuRoutine(void *handle, MENUTK_ENTRY *menu) {
	DARNER *m = handle; 

	if (menu == NULL)
		return -2;

	if (menu->waiting == 1) {
		if (menu->orientation == MENUTK_ORIENT_H)
			menutkInputH(m, menu);
		else if (menu->orientation == MENUTK_ORIENT_V || menu->orientation == MENUTK_ORIENT_V_OL)
			menutkInputV(m, menu);
		else if (menu->orientation == MENUTK_SPINBTN)
			menutkSpinbuttonInput(m, menu);
	}

	if (menu->hidden == 1)
		return -1;

	glLoadIdentity();
	if (menu->orientation != MENUTK_SPINBTN) {
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
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if (menu->orientation != MENUTK_ORIENT_V_OL)
		textRender(menu->text);

	glTranslatef(m->video.swgran * m->video.offset_x, m->video.shgran * m->video.offset_y, 0.0f);
	
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
