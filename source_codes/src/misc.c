#include <gtk/gtk.h>
#include <string.h>

GtkWidget *
CreateWidgetFromXpm (GtkWidget * parent, gchar ** xpm_data)
{
	GdkBitmap *mask;
	GdkPixmap *pixmap_data;
	GtkWidget *pixmap_widget;

	pixmap_data = gdk_pixmap_create_from_xpm_d (parent->window,
						    &mask,
						    NULL,
						    (gchar **) xpm_data);
	pixmap_widget = gtk_pixmap_new (pixmap_data, mask);
	gtk_widget_show (pixmap_widget);

	return (pixmap_widget);
}

/*
 * CreateMenuItem
 *
 * Creates an item and puts it in the menu and returns the item.
 *
 * menu - container menu
 * szName - Name of the menu - NULL for a separator
 * szAccel - Acceleration string - "^C" for Control-C
 * szTip - Tool tips
 * func - Call back function
 * data - call back function data
 *
 * returns new menuitem
 */
GtkWidget *
CreateMenuItem (GtkWidget * menu,
		char *szName,
		GtkAccelGroup * accel,
		guint key, guint mods, GtkSignalFunc func, gpointer data)
{
	GtkWidget *menuitem;

	/* --- If there's a name, create the item and put a
	 *     Signal handler on it.
	 */
	if (szName && strlen (szName))
	{
		menuitem = gtk_menu_item_new_with_label (szName);
		gtk_signal_connect (GTK_OBJECT (menuitem), "activate",
				    GTK_SIGNAL_FUNC (func), data);
	}
	else
	{
		/* --- Create a separator --- */
		menuitem = gtk_menu_item_new ();
	}

	/* --- Add menu item to the menu and show it. --- */
	gtk_menu_append (GTK_MENU (menu), menuitem);
	gtk_widget_show (menuitem);

	/* --- If there was an accelerator --- */
	if (accel != NULL)
	{
		gtk_widget_add_accelerator (menuitem, "activate",
					    accel, key, mods,
					    GTK_ACCEL_VISIBLE);
	}
	return (menuitem);
}


/*
 * CreateMenuCheck
 *
 * Create a menu checkbox
 *
 * menu - container menu
 * szName - name of the menu
 * func - Call back function.
 * data - call back function data
 *
 * returns new menuitem
 */
GtkWidget *
CreateMenuCheck (GtkWidget * menu,
		 char *szName, GtkSignalFunc func, gpointer data)
{
	GtkWidget *menuitem;

	/* --- Create menu item --- */
	menuitem = gtk_check_menu_item_new_with_label (szName);

	/* --- Add it to the menu --- */
	gtk_menu_append (GTK_MENU (menu), menuitem);
	gtk_widget_show (menuitem);

	/* --- Listen for "toggled" messages --- */
	gtk_signal_connect (GTK_OBJECT (menuitem), "toggled",
			    GTK_SIGNAL_FUNC (func), data);

	return (menuitem);
}



/*
 * CreateMenuRadio
 *
 * Create a menu radio
 *
 * menu - container menu
 * szName - name of the menu
 * func - Call back function.
 * data - call back function data
 *
 * returns new menuitem
 */
GtkWidget *
CreateMenuRadio (GtkWidget * menu,
		 char *szName,
		 GSList ** group, GtkSignalFunc func, gpointer data)
{
	GtkWidget *menuitem;

	/* --- Create menu item --- */
	menuitem = gtk_radio_menu_item_new_with_label (*group, szName);
	*group = gtk_radio_menu_item_group (GTK_RADIO_MENU_ITEM (menuitem));

	/* --- Add it to the menu --- */
	gtk_menu_append (GTK_MENU (menu), menuitem);
	gtk_widget_show (menuitem);

	/* --- Listen for "toggled" messages --- */
	gtk_signal_connect (GTK_OBJECT (menuitem), "toggled",
			    GTK_SIGNAL_FUNC (func), data);

	return (menuitem);
}

GtkWidget *
CreateSubMenu (GtkWidget * menu, char *szName)
{
	GtkWidget *menuitem;
	GtkWidget *submenu;

	/* --- Create menu --- */
	menuitem = gtk_menu_item_new_with_label (szName);

	/* --- Add it to the menubar --- */
	gtk_widget_show (menuitem);
	gtk_menu_append (GTK_MENU (menu), menuitem);

	/* --- Get a menu and attach to the menuitem --- */
	submenu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), submenu);

	/* --- Viola! --- */
	return (submenu);
}

GtkWidget *
CreateBarSubMenu (GtkWidget * menubar, char *szName)
{
	GtkWidget *menuitem;
	GtkWidget *menu;

	menu = gtk_menu_new ();

	menuitem = gtk_tearoff_menu_item_new ();
	gtk_widget_show (menuitem);
	gtk_menu_append (GTK_MENU (menu), menuitem);

	/* --- Create menu --- */
	menuitem = gtk_menu_item_new_with_label (szName);

	/* --- Add it to the menubar --- */
	gtk_menu_bar_append (GTK_MENU_BAR (menubar), menuitem);
	gtk_widget_show (menuitem);

	/* --- Get a menu and attach to the menuitem --- */
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);

	/* --- Viola! --- */
	return (menu);
}
