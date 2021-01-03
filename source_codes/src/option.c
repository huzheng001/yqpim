#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "intl.h"

#include <gtk/gtk.h>
#include "yqtreeitem.h"
#include "configfile.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


static GtkWidget *configwindow = NULL;
static GtkWidget *option_1_title_entry[46];
static GtkWidget *option_1_width_entry[46];
static GtkWidget *option_1_check_button[46];
static GtkWidget *sndbutton;
/*static GtkWidget *option_2_oldname_entry[46];
static GtkWidget *option_2_newname_entry[46];*/

extern struct _YQList yqlist;
extern ConfigFile *rccfgfile;
extern gboolean titleshow[46];
extern gchar *title[];
extern gint yesnoreturn;
extern GtkWidget *tree_main;
extern YqTreeItem *selectleaf;
extern gboolean yq_nowiszu;
extern GdkPixmap *yqpixmaps[MAX_IMAGE_TYPES];
extern GdkBitmap *yqmasks[MAX_IMAGE_TYPES];


void Areyousure (gchar * szMessage);
void createclist ();
void Popup (gchar * szMessage);
GtkWidget *CreateWidgetFromXpm (GtkWidget * parent, gchar ** xpm_data);
static void config_window_ok_cb (GtkWidget * widget, gpointer data);
void open_allitem(YqTreeItem *yqitem);
void open_rootitem();
void open_zu (YqTreeItem * yqitem);
void yq_play_sound(gint i);


static void
config_window_close_cb (GtkWidget * widget, gpointer data)
{
	gtk_widget_destroy (configwindow);
	configwindow = NULL;
}

static void
config_window_huifu_cb (GtkWidget * widget, gpointer data)
{
	gint i;

	Areyousure (_("Are you sure you want to set to default?"));
	if (!yesnoreturn)
		return;
	for (i = 0; i < 46; i++)
	{
		gtk_entry_set_text (GTK_ENTRY (option_1_title_entry[i]),
				    _(title[i]));
		gtk_entry_set_text (GTK_ENTRY (option_1_width_entry[i]), "0");
		if ((i == 0) || (i == 8) || (i == 10) || (i == 14)
		    || (i == 21) || (i == 45))
			gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON
						     (option_1_check_button
						      [i]), 1);
		else
			gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON
						     (option_1_check_button
						      [i]), 0);
	}
}

static void
config_window_destroy (GtkWidget * w, GdkEvent * event, gpointer data)
{
	config_window_close_cb (NULL, NULL);
}

static void
config_window_ok_cb (GtkWidget * widget, gpointer data)
{
	gchar *ab, ti[4], *bb, tz[256];
	gint i, a, b;
	gboolean c;

	for (i = 0; i < 46; i++)
	{
		if (*gtk_entry_get_text (GTK_ENTRY (option_1_title_entry[i]))
		    == '\0')
		{
			sprintf (tz, _("Number %d 's name is NULL!"), i + 1);
			Popup (tz);
			return;
		}
	}

	for (i = 0; i < 46; i++)
	{
		bb = gtk_entry_get_text (GTK_ENTRY (option_1_width_entry[i]));
		if (*bb == '\0')
		{
			sprintf (tz, _("Number %d 's width is NULL!"), i + 1);
			Popup (tz);
			return;
		}
		a = atoi (bb);
		if (a < 0)
		{
			sprintf (tz, "Number %d 's width is not a number!", i + 1);
			Popup (tz);
			return;
		}
	}


	for (i = 1; i <= 46; i++)
	{
		sprintf (ti, "%d", i);
		if (yq_cfg_read_string (rccfgfile, "title", ti, &ab))
		{
			c=TRUE;
		}
		else
		{
			c=FALSE;
			ab=_(title[i-1]);
		}
		bb = gtk_entry_get_text (GTK_ENTRY
					 (option_1_title_entry[i - 1]));
		if (strcmp (ab, bb)!=0)
			yq_cfg_write_string (rccfgfile, "title", ti, bb);
		if (c)
			g_free(ab);
	}
	for (i = 2; i <= 46; i++)
	{
		sprintf (ti, "%d", i);
		if (gtk_toggle_button_get_active
		    (GTK_TOGGLE_BUTTON (option_1_check_button[i - 1])) !=
		    titleshow[i - 1])
		{
			titleshow[i - 1] = !titleshow[i - 1];
			yq_cfg_write_boolean (rccfgfile, "titleshow", ti,
					      titleshow[i - 1]);
		}
	}
	for (i = 1; i <= 46; i++)
	{
		sprintf (ti, "%d", i);
		yq_cfg_read_int (rccfgfile, "titlewidth", ti, &a);
		b = atoi (gtk_entry_get_text
			  (GTK_ENTRY (option_1_width_entry[i - 1])));
		if (a != b)
			yq_cfg_write_int (rccfgfile, "titlewidth", ti, b);
	}
	yq_cfg_read_boolean(rccfgfile,"setting","playsound",&c);
	if (c!=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (sndbutton)))
		yq_cfg_write_boolean(rccfgfile,"setting","playsound",!c);
	
	if (yq_nowiszu)
	{
		gtk_clist_clear (GTK_CLIST (yqlist.list));
		gtk_container_remove (GTK_CONTAINER (yqlist.box),
				      yqlist.listwin);
		createclist ();
		if (strcmp(selectleaf->nodename,"r")==0)
		{
			open_rootitem();
		}
		else
		{
			if (strcmp(selectleaf->nodename,"a")==0)
			{
				open_allitem(selectleaf);
			}
			else
			{
				open_zu (selectleaf);
			}
		}
	}
	config_window_close_cb (NULL, NULL);
}




static void
config_window_create (gint start_tab)
{
	GtkWidget *win_vbox;
	GtkWidget *hbox;
	GtkWidget *notebook;
	GtkWidget *frame;
	GtkWidget *label;
	GtkWidget *pixmap_widget;
	GtkWidget *vbox;
	GtkWidget *button;
	GtkWidget *table;
	GtkWidget *opwin;
	gchar buf[255], tmp[5],*ab;
	gint i,width;
	gboolean m;

	yq_play_sound(6);
	configwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect (GTK_OBJECT (configwindow), "delete_event",
			    (GtkSignalFunc) config_window_destroy, NULL);
	gtk_window_set_policy (GTK_WINDOW (configwindow), FALSE, FALSE,
			       FALSE);
	gtk_window_set_title (GTK_WINDOW (configwindow), _("Proferences"));
	gtk_window_set_wmclass (GTK_WINDOW (configwindow), "Proferences",
				"Yqpim");
	gtk_widget_realize (configwindow);

	gtk_container_border_width (GTK_CONTAINER (configwindow), 5);

	win_vbox = gtk_vbox_new (FALSE, 5);
	gtk_container_add (GTK_CONTAINER (configwindow), win_vbox);
	gtk_widget_show (win_vbox);

	hbox = gtk_hbox_new (TRUE, 0);
	gtk_box_pack_end (GTK_BOX (win_vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show (hbox);

	button = gtk_button_new_with_label (_("OK"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    (GtkSignalFunc) config_window_ok_cb, NULL);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 20);
	gtk_widget_show (button);

	button = gtk_button_new_with_label (_("Reset All"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    (GtkSignalFunc) config_window_huifu_cb, NULL);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 20);
	gtk_widget_show (button);

	button = gtk_button_new_with_label (_("Cancel"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    (GtkSignalFunc) config_window_close_cb, NULL);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 20);
	gtk_widget_show (button);

	notebook = gtk_notebook_new ();
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
	gtk_box_pack_start (GTK_BOX (win_vbox), notebook, TRUE, TRUE, 0);

	/* list tab */

	frame = gtk_frame_new (NULL);
	gtk_container_border_width (GTK_CONTAINER (frame), 5);
	gtk_widget_show (frame);
	label = gtk_label_new (_("Custom the details"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_border_width (GTK_CONTAINER (vbox), 5);
	gtk_container_add (GTK_CONTAINER (frame), vbox);
	gtk_widget_show (vbox);

	table = gtk_table_new (3, 34, FALSE);
	gtk_widget_show (table);

	opwin = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (opwin),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (opwin),
					       table);
	gtk_box_pack_start (GTK_BOX (vbox), opwin, TRUE, TRUE, 0);
	gtk_widget_show (opwin);

	label = gtk_label_new (_("Ifshow"));
	gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 0, 1);
	gtk_widget_show (label);
	label = gtk_label_new (_("List Name"));
	gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 0, 1);
	gtk_widget_show (label);
	label = gtk_label_new (_("Width"));
	gtk_table_attach_defaults (GTK_TABLE (table), label, 2, 3, 0, 1);
	gtk_widget_show (label);


	for (i = 0; i < 46; i++)
	{
		sprintf (buf, "%d", i + 1);
		option_1_check_button[i] =
			gtk_check_button_new_with_label (buf);
		gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON
					     (option_1_check_button[i]),
					     titleshow[i]);
		gtk_table_attach_defaults (GTK_TABLE (table),
					   option_1_check_button[i], 0, 1,
					   i + 1, i + 2);
		gtk_widget_show (option_1_check_button[i]);

		gtk_widget_set_sensitive (option_1_check_button[0], 0);

		option_1_title_entry[i] = gtk_entry_new_with_max_length (32);
		if (yq_cfg_read_string (rccfgfile, "title", buf, &ab))
		{
			gtk_entry_set_text (GTK_ENTRY (option_1_title_entry[i]), ab);
			g_free (ab);
		}
		else
			gtk_entry_set_text (GTK_ENTRY (option_1_title_entry[i]), _(title[i]));
		gtk_table_attach_defaults (GTK_TABLE (table),
					   option_1_title_entry[i], 1, 2,
					   i + 1, i + 2);
		gtk_widget_show (option_1_title_entry[i]);

		option_1_width_entry[i] = gtk_entry_new_with_max_length (3);
		gtk_widget_set_usize (option_1_width_entry[i], 28, -1);
		yq_cfg_read_int (rccfgfile, "titlewidth", buf, &width);
		sprintf(tmp,"%d",width);
		gtk_entry_set_text (GTK_ENTRY (option_1_width_entry[i]), tmp);
		gtk_table_attach_defaults (GTK_TABLE (table),
					   option_1_width_entry[i], 2, 3,
					   i + 1, i + 2);
		gtk_widget_show (option_1_width_entry[i]);
	}
	
	frame = gtk_frame_new (NULL);
	gtk_container_border_width (GTK_CONTAINER (frame), 5);
	gtk_widget_show (frame);
	label = gtk_label_new (_("Misc setting"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (frame), vbox);
	gtk_widget_show (vbox);

	sndbutton=gtk_check_button_new_with_label (_("Play Sound"));
	yq_cfg_read_boolean(rccfgfile,"setting","playsound",&m);
	gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (sndbutton),m);
	gtk_box_pack_start (GTK_BOX (vbox), sndbutton, TRUE, TRUE, 0);
	gtk_widget_show (sndbutton);
	
	
	/* about tab */

	frame = gtk_frame_new (NULL);
	gtk_container_border_width (GTK_CONTAINER (frame), 5);
	gtk_widget_show (frame);
	label = gtk_label_new (_("About"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (frame), vbox);
	gtk_widget_show (vbox);
	gtk_widget_realize (configwindow);

	pixmap_widget = gtk_pixmap_new (yqpixmaps[YQPIM], yqmasks[YQPIM]);
	gtk_widget_show (pixmap_widget);

	gtk_box_pack_start (GTK_BOX (vbox),
			    pixmap_widget, TRUE,
			    TRUE, 0);

	strcpy (buf,
		_("Yqpim 0.80\n\nAuthor: huzheng\nhttp://yqpim.sourceforge.net\nforlinux@263.net\n\n    Released under the GNU Public License    "));
	label = gtk_label_new (buf);
	gtk_box_pack_start (GTK_BOX (vbox), label, TRUE, TRUE, 0);
	gtk_widget_show (label);
	
	gtk_notebook_set_page (GTK_NOTEBOOK (notebook), start_tab);
	gtk_widget_show (notebook);

	gtk_widget_unrealize (configwindow);
	gtk_widget_show (configwindow);
}

void
show_config_window ()
{
	if (configwindow)
		return;
	config_window_create (0);
}

void
show_about_window ()
{
	if (configwindow)
		return;
	config_window_create (2);
}
