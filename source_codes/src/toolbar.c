#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "intl.h"

#include <gtk/gtk.h>
#include "yqtype.h"

#include "pixmaps/new.xpm"
#include "pixmaps/open.xpm"
#include "pixmaps/cut.xpm"
#include "pixmaps/copy.xpm"
#include "pixmaps/paste.xpm"
#include "pixmaps/search.xpm"
#include "pixmaps/add.xpm"
#include "pixmaps/remove.xpm"
#include "pixmaps/save.xpm"
#include "pixmaps/saveadd.xpm"
#include "pixmaps/cancel.xpm"
#include "pixmaps/zu.xpm"
#include "pixmaps/rename.xpm"
#include "pixmaps/mima.xpm"
#include "pixmaps/question.xpm"
#include "pixmaps/up.xpm"

extern GdkPixmap *yqpixmaps[MAX_IMAGE_TYPES];
extern GdkBitmap *yqmasks[MAX_IMAGE_TYPES];



GtkWidget *new_button, *open_button,*cut_button, *copy_button,
	*paste_button, *search_button, *add_button, *remove_button,
	*save_button, *saveadd_button, *cancel_button, 
	*zu_button, *rename_button, *mima_button, *laji_button, 
	*question_button, *up_button;

void kk (GtkWidget * widget, gpointer data);
GtkWidget *CreateWidgetFromXpm (GtkWidget * parent, gchar ** xpm_data);
void toolbar_save (GtkWidget * widget, gpointer data);
void toolbar_cancel (GtkWidget * widget, gpointer data);
void toolbar_remove (GtkWidget * widget, gpointer data);
void toolbar_add (GtkWidget * widget, gpointer data);
void toolbar_saveadd (GtkWidget * widget, gpointer data);
void toolbar_zu (GtkWidget * widget, gpointer data);
void toolbar_open (GtkWidget * widget, gpointer data);
void toolbar_rename (GtkWidget * widget, gpointer data);
void toolbar_new (GtkWidget * widget, gpointer data);
void toolbar_cut (GtkWidget * widget, gpointer data);
void toolbar_copy (GtkWidget * widget, gpointer data);
void toolbar_paste (GtkWidget * widget, gpointer data);
void toolbar_goup (GtkWidget * widget, gpointer data);
void toolbar_search (GtkWidget * widget, gpointer data);
void show_about_window ();
void menu_tool_trash_open_func (GtkWidget * widget, gpointer data);
void menu_tool_password_func (GtkWidget * widget, gpointer data);

void
createtoolbar (GtkWidget * parent, GtkWidget * main_vbox)
{
	GtkWidget *toolbar;

	/* --- Create the toolbar and add it to the window --- */
	toolbar =
		gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL,
				 GTK_TOOLBAR_ICONS);
	gtk_box_pack_start (GTK_BOX (main_vbox), toolbar, FALSE, FALSE, 2);
	gtk_widget_show (toolbar);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

	new_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
					      NULL, _("New Yqpim File"), NULL,
					      CreateWidgetFromXpm (parent,
								   (gchar **)
								   new_xpm),
					      (GtkSignalFunc) toolbar_new,
					      NULL);

	open_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
					       NULL, _("Open Yqpim File"), NULL,
					       CreateWidgetFromXpm (parent,
								    (gchar **)
								    open_xpm),
					       (GtkSignalFunc) toolbar_open,
					       NULL);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

	cut_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
					      NULL, _("Cut"), NULL,
					      CreateWidgetFromXpm (parent,
								   (gchar **)
								   cut_xpm),
					      (GtkSignalFunc) toolbar_cut, NULL);

	copy_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
					       NULL, _("Copy"), NULL,
					       CreateWidgetFromXpm (parent,
								    (gchar **)
								    copy_xpm),
					       (GtkSignalFunc) toolbar_copy,
					       NULL);

	paste_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
						NULL, _("Paste"), NULL,
						CreateWidgetFromXpm (parent,
								     (gchar
								      **)
								     paste_xpm),
						(GtkSignalFunc) toolbar_paste,
						NULL);

	search_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
						 NULL, _("Find People"), NULL,
						 CreateWidgetFromXpm (parent,
								      (gchar
								       **)
								      search_xpm),
						 (GtkSignalFunc) toolbar_search,
						 NULL);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

	add_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
					      NULL, _("New Contact"), NULL,
					      CreateWidgetFromXpm (parent,
								   (gchar **)
								   add_xpm),
					      (GtkSignalFunc) toolbar_add,
					      NULL);

	remove_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
						 NULL, _("Delete"), NULL,
						 CreateWidgetFromXpm (parent,
								      (gchar
								       **)
								      remove_xpm),
						 (GtkSignalFunc)
						 toolbar_remove, NULL);
	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));
	zu_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
					     NULL, _("New Folder"), NULL,
					     CreateWidgetFromXpm (parent,
								  (gchar **)
								  zu_xpm),
					     (GtkSignalFunc) toolbar_zu,
					     NULL);
	rename_button =
		gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), NULL,
					 _("Rename"), NULL,
					 CreateWidgetFromXpm (parent,
							      (gchar **)
							      rename_xpm),
					 (GtkSignalFunc) toolbar_rename,
					 NULL);
	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));
	mima_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
					       NULL, _("Setup Password"), NULL,
					       CreateWidgetFromXpm (parent,
								    (gchar **)
								    mima_xpm),
					       (GtkSignalFunc) menu_tool_password_func, NULL);
	laji_button =
		gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), NULL,
					 _("Open Trash"), NULL,
					 gtk_pixmap_new (yqpixmaps[TRASH], yqmasks[TRASH]),
					 (GtkSignalFunc) menu_tool_trash_open_func, NULL);
	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));
	question_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
						   NULL, _("About"), NULL,
						   CreateWidgetFromXpm
						   (parent,
						    (gchar **) question_xpm),
						   (GtkSignalFunc)
						   show_about_window, NULL);
}

void
createtoolbar1 (GtkWidget * parent, GtkWidget * main_vbox)
{
	GtkWidget *toolbar;

	toolbar =
		gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL,
				 GTK_TOOLBAR_ICONS);

	gtk_box_pack_start (GTK_BOX (main_vbox), toolbar, FALSE, FALSE, 0);
	gtk_widget_show (toolbar);
	up_button = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
					     NULL, _("Go Up"), NULL,
					     CreateWidgetFromXpm (parent,
								  (gchar **)
								  up_xpm),
					     (GtkSignalFunc) toolbar_goup, NULL);
	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));
	save_button =
		gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), NULL, _("Save Contact"),
					 NULL, CreateWidgetFromXpm (parent,
								    (gchar **)
								    save_xpm),
					 (GtkSignalFunc) toolbar_save, NULL);
	saveadd_button =
		gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), NULL,
					 _("Save & New Contact"), NULL,
					 CreateWidgetFromXpm (parent,
							      (gchar **)
							      saveadd_xpm),
					 (GtkSignalFunc) toolbar_saveadd,
					 NULL);
	cancel_button =
		gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), NULL, _("Cancel"),
					 NULL, CreateWidgetFromXpm (parent,
								    (gchar **)
								    cancel_xpm),
					 (GtkSignalFunc) toolbar_cancel,
					 NULL);
	gtk_widget_set_sensitive (save_button, 0);
	gtk_widget_set_sensitive (saveadd_button, 0);
	gtk_widget_set_sensitive (cancel_button, 0);
	
}
