#ifndef __DARNIT_MENUTK_H__
#define	__DARNIT_MENUTK_H__

typedef void DARNIT_MENU;

DARNIT_MENU *darnitMenuHorizontalCreate(const char *options, int x, int y, void *font, int max_w);
DARNIT_MENU *darnitMenuVerticalCreate(const char *options, int x, int y, void *font, int menuw, int textskip_x, int max_h);
DARNIT_MENU *darnitMenuVerticalShadeCreate(int x, int y, int shade_w, int shade_h, int y_advance, int options, int skip_option, int max_h);
DARNIT_MENU *darnitMenuSpinbuttonCreate(const char *label, int x, int y, void *font, int step, int min, int max);
DARNIT_MENU *darnitMenuTextinputCreate(int x, int y, void *font, char *buff, int buff_len, int field_len);
void darnitMenuVerticalShadeSelectionsReduceByOne(DARNIT_MENU *menu);
void darnitMenuVerticalShadeSelectionsIncreaseByOne(DARNIT_MENU *menu);
void darnitMenuShadeColorSet(DARNIT_MENU *menu, float r, float g, float b, float a);
int darnitMenuHandle(DARNIT_MENU *menu);
void darnitMenuSelectionWaitForNew(DARNIT_MENU *menu);
int darnitMenuPeek(DARNIT_MENU *menu, int *top_sel);
int darnitMenuChangedCheck(DARNIT_MENU *menu);
void darnitMenuHide(DARNIT_MENU *menu);
void darnitMenuUnhide(DARNIT_MENU *menu);
void *darnitMenuDestroy(DARNIT_MENU *menu);

#endif
