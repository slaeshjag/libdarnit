#ifndef __UI_H__
#define	__UI_H__


typedef struct {
	GtkWidget		*window;
	GtkWidget		*vbox_main;

	GtkWidget		*toolbar;
	GtkToolItem		*new;
	GtkToolItem		*open;
	GtkToolItem		*save;

	GtkWidget		*hbox;

	/*****/
	GtkWidget		*hbox;
	GtkWidget		*add_section;
	GtkWidget		*del_section;
	GtkWidget		*ren_section;

	GtkWidget		*new_string;
	GtkWidget		*chg_string;
	GtkWidget		*del_string;
} UI;


#endif
