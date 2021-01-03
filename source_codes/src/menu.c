
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gtk/gtk.h>
#include <stdio.h>
#include "configfile.h"
#include "intl.h"


GtkWidget *menu_file_new, *menu_file_open, *menu_file_openold,
	*menu_file_close, *menu_file_saveas, *menu_file_import,*menu_file_export,*menu_file_exit, *menu_edit_cut,
	*menu_edit_copy, *menu_edit_paste,*menu_edit_selectall,*menu_edit_selectelse,*menu_edit_find, *menu_jilu_add, *menu_jilu_remove,
	*menu_jilu_save, *menu_jilu_saveadd, *menu_jilu_cancel, *menu_jilu_zu,
	*menu_jilu_rename, *menu_tool_option,*menu_tool_trash;

extern ConfigFile *rccfgfile;


gint CloseAppWindow (GtkWidget * widget, gpointer * data);
void yq_readfile (GtkWidget * widget, gpointer file_name);
void menu_file_close_func (GtkWidget * widget, gpointer data);
void menu_file_saveas_func (GtkWidget * widget, gpointer data);
void menu_file_importcsv_func(GtkWidget *widget, gpointer data);
void menu_file_exportcsv_func(GtkWidget *widget, gpointer data);
void menu_file_importvcard_func(GtkWidget *widget, gpointer data);
void menu_file_exportvcard_func(GtkWidget *widget, gpointer data);
void menu_file_exporthtml_func(GtkWidget *widget, gpointer data);
void menu_edit_selectall_func(GtkWidget *widget, gpointer data);
void menu_edit_selectother_func(GtkWidget *widget, gpointer data);
void menu_tool_trash_open_func (GtkWidget * widget, gpointer data);
void menu_tool_trash_close_func (GtkWidget * widget, gpointer data);
void menu_tool_trash_clean_func (GtkWidget * widget, gpointer data);
void menu_tool_password_func (GtkWidget * widget, gpointer data);

void toolbar_save (GtkWidget * widget, gpointer data);
void toolbar_cancel (GtkWidget * widget, gpointer data);
void toolbar_remove (GtkWidget * widget, gpointer data);
void toolbar_add (GtkWidget * widget, gpointer data);
void toolbar_saveadd (GtkWidget * widget, gpointer data);
void toolbar_zu (GtkWidget * widget, gpointer data);
void toolbar_open (GtkWidget * widget, gpointer data);
void toolbar_rename (GtkWidget * widget, gpointer data);
void toolbar_new (GtkWidget * widget, gpointer data);
void toolbar_copy (GtkWidget * widget, gpointer data);
void toolbar_cut (GtkWidget * widget, gpointer data);
void toolbar_paste (GtkWidget * widget, gpointer data);
void toolbar_search (GtkWidget * widget, gpointer data);
void show_config_window ();
void show_about_window ();

GtkWidget *CreateBarSubMenu (GtkWidget * menubar, char *szName);
GtkWidget *CreateSubMenu (GtkWidget * menu, char *szName);
GtkWidget *CreateMenuItem (GtkWidget * menu,
			   char *szName,
			   GtkAccelGroup * accel,
			   guint key,
			   guint mods, GtkSignalFunc func, gpointer data);
GtkWidget *CreateMenuCheck (GtkWidget * menu,
			    char *szName, GtkSignalFunc func, gpointer data);
GtkWidget *CreateMenuRadio (GtkWidget * menu,
			    char *szName,
			    GSList ** group,
			    GtkSignalFunc func, gpointer data);

void
createmenu (GtkWidget * win_main, GtkWidget * main_vbox)
{
	GtkWidget *menubar;
	GtkWidget *menu;
	GtkWidget *menuitem;
	GtkAccelGroup *accel;
	gint i, a;
	gchar *ab, ti[4];

	accel = gtk_accel_group_new ();
	gtk_accel_group_attach (accel, GTK_OBJECT (win_main));
	menubar = gtk_menu_bar_new ();
	gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, TRUE, 0);
	gtk_widget_show (menubar);
	menu = CreateBarSubMenu (menubar, _("File"));

	menu_file_new =
		CreateMenuItem (menu, _("New..."), accel, 'N', GDK_CONTROL_MASK,
				GTK_SIGNAL_FUNC (toolbar_new), NULL);

	menu_file_open =
		CreateMenuItem (menu, _("Open..."), accel, 'O', GDK_CONTROL_MASK,
				GTK_SIGNAL_FUNC (toolbar_open), NULL);

	menu_file_openold = CreateSubMenu (menu, _("Recent Files"));

	yq_cfg_read_int (rccfgfile, "file", "a", &a);
	for (i = 1; i <= a; i++)
	{
		sprintf (ti, "%d", i);
		yq_cfg_read_string (rccfgfile, "file", ti, &ab);
		menuitem = CreateMenuItem (menu_file_openold, ab, NULL, 0, 0,
					   GTK_SIGNAL_FUNC (yq_readfile),
					   g_strdup (ab));
		g_free (ab);
	}
	menu_file_close = CreateMenuItem (menu, _("Close"), NULL, 0, 0,
					  GTK_SIGNAL_FUNC (menu_file_close_func), NULL);

	menuitem = CreateMenuItem (menu, NULL, NULL, 0, 0, NULL, NULL);

	menu_file_saveas = CreateMenuItem (menu, _("Save As..."), NULL, 0, 0,
					   GTK_SIGNAL_FUNC (menu_file_saveas_func),
					   NULL);

	menuitem = CreateMenuItem (menu, NULL, NULL, 0, 0, NULL, NULL);

	menu_file_import = CreateSubMenu (menu, _("Import"));

	menuitem = CreateMenuItem (menu_file_import, _("Business card(vcard)..."), NULL, 0, 0,
				   GTK_SIGNAL_FUNC (menu_file_importvcard_func), NULL);
	menuitem =
		CreateMenuItem (menu_file_import, _("CSV File..."), NULL, 0,
				0, GTK_SIGNAL_FUNC (menu_file_importcsv_func),NULL);

	menu_file_export = CreateSubMenu (menu, _("Export"));

	menuitem = CreateMenuItem (menu_file_export, _("Business card(vcard)..."), NULL, 0, 0,
				   GTK_SIGNAL_FUNC (menu_file_exportvcard_func), NULL);
	menuitem =
		CreateMenuItem (menu_file_export, _("CSV File..."), NULL, 0, 0,
				GTK_SIGNAL_FUNC (menu_file_exportcsv_func), NULL);
	menuitem =
		CreateMenuItem (menu_file_export, _("Html File..."), NULL, 0, 0,
				GTK_SIGNAL_FUNC (menu_file_exporthtml_func), NULL);

	menuitem = CreateMenuItem (menu, NULL, NULL, 0, 0, NULL, NULL);

	menu_file_exit =
		CreateMenuItem (menu, _("Exit"), accel, 'X', GDK_MOD1_MASK,
				GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

	/* -----------------
	 * --- Edit menu ---
	 * ----------------- */
	menu = CreateBarSubMenu (menubar, _("Edit"));

	menu_edit_cut =
		CreateMenuItem (menu, _("Cut"), accel, 'X', GDK_CONTROL_MASK,
				GTK_SIGNAL_FUNC (toolbar_cut), NULL);

	menu_edit_copy =
		CreateMenuItem (menu, _("Copy"), accel, 'C', GDK_CONTROL_MASK,
				GTK_SIGNAL_FUNC (toolbar_copy), NULL);

	menu_edit_paste =
		CreateMenuItem (menu, _("Paste"), accel, 'V', GDK_CONTROL_MASK,
				GTK_SIGNAL_FUNC (toolbar_paste), NULL);

	menuitem = CreateMenuItem (menu, NULL, NULL, 0, 0, NULL, NULL);

	menu_edit_selectall = CreateMenuItem (menu, _("Select All"), accel, 'A', GDK_CONTROL_MASK,
				   GTK_SIGNAL_FUNC (menu_edit_selectall_func), NULL);
	menu_edit_selectelse = CreateMenuItem (menu, _("Select Else"), NULL, 0, 0,
				   GTK_SIGNAL_FUNC (menu_edit_selectother_func), NULL);

	menuitem = CreateMenuItem (menu, NULL, NULL, 0, 0, NULL, NULL);

	menu_edit_find = CreateMenuItem (menu, _("Find People..."), accel, 'F', GDK_CONTROL_MASK,
				   GTK_SIGNAL_FUNC (toolbar_search), NULL);

	menu = CreateBarSubMenu (menubar, _("Contacts"));

	menu_jilu_add = CreateMenuItem (menu, _("New Contact"), NULL, 0, 0,
					GTK_SIGNAL_FUNC (toolbar_add), NULL);
	menu_jilu_remove = CreateMenuItem (menu, _("Delete"), NULL, 0, 0,
					   GTK_SIGNAL_FUNC (toolbar_remove),
					   NULL);

	menuitem = CreateMenuItem (menu, NULL, NULL, 0, 0, NULL, NULL);

	menu_jilu_save =
		CreateMenuItem (menu, _("Save"), accel, 'S', GDK_CONTROL_MASK,
				GTK_SIGNAL_FUNC (toolbar_save), NULL);
	menu_jilu_saveadd =
		CreateMenuItem (menu, _("Save & New"), NULL, 0, 0,
				GTK_SIGNAL_FUNC (toolbar_saveadd), NULL);
	menu_jilu_cancel =
		CreateMenuItem (menu, _("Cancel"), NULL, 0, 0,
				GTK_SIGNAL_FUNC (toolbar_cancel), NULL);

	gtk_widget_set_sensitive (menu_jilu_save, 0);
	gtk_widget_set_sensitive (menu_jilu_saveadd, 0);
	gtk_widget_set_sensitive (menu_jilu_cancel, 0);

	menuitem = CreateMenuItem (menu, NULL, NULL, 0, 0, NULL, NULL);

	menu_jilu_zu = CreateMenuItem (menu, _("New Folder..."), NULL, 0, 0,
				       GTK_SIGNAL_FUNC (toolbar_zu), NULL);

	menu_jilu_rename = CreateMenuItem (menu, _("Rename..."), NULL, 0, 0,
					   GTK_SIGNAL_FUNC (toolbar_rename),
					   NULL);


	menu = CreateBarSubMenu (menubar, _("Tools"));

	menuitem = CreateMenuItem (menu, _("Password..."), NULL, 0, 0,
				   GTK_SIGNAL_FUNC (menu_tool_password_func), NULL);

	menuitem = CreateMenuItem (menu, NULL, NULL, 0, 0, NULL, NULL);

	menu_tool_trash = CreateSubMenu (menu, _("Trash"));

	menuitem = CreateMenuItem (menu_tool_trash, _("Open"), NULL, 0, 0,
				   GTK_SIGNAL_FUNC (menu_tool_trash_open_func), NULL);
	menuitem = CreateMenuItem (menu_tool_trash, _("Clean"), NULL, 0, 0,
				   GTK_SIGNAL_FUNC (menu_tool_trash_clean_func), NULL);
	menuitem = CreateMenuItem (menu_tool_trash, _("Close"), NULL, 0, 0,
				   GTK_SIGNAL_FUNC (menu_tool_trash_close_func), NULL);


	menuitem = CreateMenuItem (menu, NULL, NULL, 0, 0, NULL, NULL);

	menu_tool_option = CreateMenuItem (menu, _("Preferences..."), NULL, 0, 0,
					   GTK_SIGNAL_FUNC
					   (show_config_window), NULL);


	/* ---------------------
	 * --- Help sub menu ---
	 * --------------------- */
	menu = CreateBarSubMenu (menubar, _("Help"));

	menuitem = CreateMenuItem (menu, _("About..."), NULL, 0, 0,
				   GTK_SIGNAL_FUNC (show_about_window), NULL);
}
