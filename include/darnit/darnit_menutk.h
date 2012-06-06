#ifndef __DARNIT_MENUTK_H__
#define	__DARNIT_MENUTK_H__

void *darnitMenuHorizontalCreate(void *handle, const char *options, int x, int y, void *font, int max_w);
void *darnitMenuVerticalCreate(void *handle, const char *options, int x, int y, void *font, int menuw, int textskip_x, int max_h);
void *darnitMenuVerticalShadeCreate(void *handle, int x, int y, int shade_w, int shade_h, int y_advance, int options, int skip_option, int max_h);
void *darnitMenuSpinbuttonCreate(void *handle, const char *label, int x, int y, void *font, int step, int min, int max);
void *darnitMenuTextinputCreate(int x, int y, void *font, char *buff, int buff_len, int field_len);
void darnitMenuVerticalShadeSelectionsReduceByOne(void *menu);
void darnitMenuVerticalShadeSelectionsIncreaseByOne(void *menu);
void darnitMenuShadeColorSet(void *menu, float r, float g, float b, float a);
int darnitMenuHandle(void *handle, void *menu);
void darnitMenuSelectionWaitForNew(void *menu);
int darnitMenuPeek(void *menu);
int darnitMenuChangedCheck(void *menu);
void darnitMenuHide(void *menu);
void darnitMenuUnhide(void *menu);
void *darnitMenuDestroy(void *menu);

#endif
