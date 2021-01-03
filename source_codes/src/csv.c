#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "intl.h"

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "configfile.h"



static GtkWidget **optmenus,*csvloadlist;



extern gint yesnoreturn;
extern gchar *title[];
extern gint *imindexes,imtitlecont;
extern ConfigFile *rccfgfile;
extern gchar sFilename[256];


void ClosingDialog2 (GtkWidget * widget, gpointer data);
void InputName (gchar * szMessage, gchar * entryvalue);
void Areyousure (gchar * szMessage);
void Popup (gchar * szMessage);
GtkWidget *CreateMenuItem (GtkWidget * menu,
			   char *szName,
			   GtkAccelGroup * accel,
			   guint key,
			   guint mods, GtkSignalFunc func, gpointer data);
void csv_window_ok_cb (GtkWidget * widget, gpointer data);
void csv_window_cancel_cb (GtkWidget * widget, gpointer data);
void csv_window_destroy(GtkWidget * widget, gpointer data);
void csv_window_load_cb (GtkWidget * widget, gpointer data);
void csv_load_window_del_cb (GtkWidget * widget, gpointer data);
void csv_load_window_ok_cb (GtkWidget * widget, gpointer data);
void csv_load_window_cancel_cb (GtkWidget * widget, gpointer data);
void csv_load_window_destroy(GtkWidget * widget, gpointer data);
void csv_window_save_cb (GtkWidget * widget, gpointer data);
void yq_import_csv_setimindex(GtkWidget * widget, gpointer data);
void yq_cfg_rename_sections (ConfigFile * cfg, gchar * section,gchar * section_name);



void
yq_import_csv_index(gint n,gchar **imtitles,gint *inindexes)
{
	GtkWidget *csvwindow,*csvwin;
	GtkWidget *win_vbox,*vbox;
	GtkWidget *hbox,*table,*menu,*item;
	GtkWidget *button,*frame,*label;
	gint i,j;
	gchar buf[256],**imtitle,tt[10],*aa,*ab,ti[5];

	yesnoreturn=0;
	csvwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect (GTK_OBJECT (csvwindow), "delete_event",
			    (GtkSignalFunc) csv_window_destroy, NULL);
	gtk_window_set_policy (GTK_WINDOW (csvwindow), FALSE, FALSE,
			       FALSE);
	gtk_window_set_title (GTK_WINDOW (csvwindow), _("import csv file"));
	gtk_window_set_wmclass (GTK_WINDOW (csvwindow), "csv",
				"yq");
	gtk_container_border_width (GTK_CONTAINER (csvwindow), 5);
	
	win_vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add (GTK_CONTAINER (csvwindow), win_vbox);
	gtk_widget_show (win_vbox);

	hbox = gtk_hbox_new (TRUE, 0);
	gtk_box_pack_start (GTK_BOX (win_vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show (hbox);

	button = gtk_button_new_with_label (_("load"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    (GtkSignalFunc) csv_window_load_cb, optmenus);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 20);
	gtk_widget_show (button);

	button = gtk_button_new_with_label (_("save"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    (GtkSignalFunc) csv_window_save_cb, NULL);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 20);
	gtk_widget_show (button);

	hbox = gtk_hbox_new (TRUE, 0);
	gtk_box_pack_end (GTK_BOX (win_vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show (hbox);

	button = gtk_button_new_with_label (_("OK"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    (GtkSignalFunc) csv_window_ok_cb, csvwindow);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 20);
	gtk_widget_show (button);

	button = gtk_button_new_with_label (_("Cancel"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    (GtkSignalFunc) csv_window_cancel_cb, csvwindow);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 20);
	gtk_widget_show (button);

	frame = gtk_frame_new (NULL);
	gtk_container_border_width (GTK_CONTAINER (frame), 5);
	gtk_frame_set_label( GTK_FRAME(frame), _("import to index") );
	gtk_box_pack_start (GTK_BOX (win_vbox), frame, TRUE, TRUE, 0);
	gtk_widget_show (frame);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (frame), vbox);						   
	gtk_container_border_width (GTK_CONTAINER (vbox), 5);
	gtk_widget_show (vbox);
	table = gtk_table_new (3, n+1, FALSE);
	gtk_widget_show (table);

	csvwin = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_usize (csvwin, 280, 380);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (csvwin),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (csvwin),
					       table);
	gtk_box_pack_start (GTK_BOX (vbox), csvwin, TRUE, TRUE, 0);

	gtk_widget_show (csvwin);
	
	label = gtk_label_new ("#");
	gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 0, 1);
	gtk_widget_show (label);
	label = gtk_label_new (_("import index"));
	gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 0, 1);
	gtk_widget_show (label);
	label = gtk_label_new (_("import as"));
	gtk_table_attach_defaults (GTK_TABLE (table), label, 2, 3, 0, 1);
	gtk_widget_show (label);
	
	imtitle=imtitles;
	optmenus=g_malloc(n*sizeof(GtkWidget *));
	for (i = 0; i < n; i++)
	{
		sprintf(buf,"%d",i+1);
		label = gtk_label_new (buf);
		gtk_widget_show (label);
		gtk_table_attach_defaults (GTK_TABLE (table),
		   label, 0, 1,
		   i + 1, i + 2);
		label = gtk_label_new (*imtitle);
		imtitle++;
		gtk_widget_show (label);
		gtk_table_attach_defaults (GTK_TABLE (table),
					   label, 1, 2,
					   i + 1, i + 2);

		*(optmenus+i)=gtk_option_menu_new();
		menu=gtk_menu_new();
		for (j=0;j<46;j++)
		{
			sprintf(tt,"%d",i*1000+j);
			aa=g_strdup(tt);
			sprintf(ti,"%d",j+1);
			if (yq_cfg_read_string (rccfgfile, "title", ti, &ab))
			{
				item =	CreateMenuItem (menu, ab, NULL,0,0,
				GTK_SIGNAL_FUNC (yq_import_csv_setimindex), aa);
				g_free(ab);
			}
			else
				item =	CreateMenuItem (menu, _(title[j]), NULL,0,0,
				GTK_SIGNAL_FUNC (yq_import_csv_setimindex), aa);
		}
		sprintf(tt,"%d",i*1000+46);
		aa=g_strdup(tt);
		item =	CreateMenuItem (menu, _("import to end"), NULL,0,0,
				GTK_SIGNAL_FUNC (yq_import_csv_setimindex), aa);
		sprintf(tt,"%d",i*1000+34);
		aa=g_strdup(tt);
		item =	CreateMenuItem (menu, _("don\'t import"), NULL,0,0,
				GTK_SIGNAL_FUNC (yq_import_csv_setimindex), aa);
		gtk_widget_show (menu);
		gtk_option_menu_set_menu(GTK_OPTION_MENU(*(optmenus+i)),menu);
		if (i<46)
			gtk_option_menu_set_history(GTK_OPTION_MENU(*(optmenus+i)),i);
		else
			gtk_option_menu_set_history(GTK_OPTION_MENU(*(optmenus+i)),46);
		gtk_widget_show (*(optmenus+i));		
		gtk_table_attach_defaults (GTK_TABLE (table),
					   *(optmenus+i), 2, 3,
					   i + 1, i + 2);
	}
	gtk_widget_show (csvwindow);
	gtk_main ();
	g_free(optmenus);
}

void
yq_import_csv_setimindex(GtkWidget * widget, gpointer data)
{
	gint i,j,a;
	
	a=atoi(data);
	i=a/1000;
	j=a%1000;
	
	if (j==46)
	{
		*(imindexes+i)=-1;
	}
	else
	{
		if (j==34)
			*(imindexes+i)=-2;
		else
			*(imindexes+i)=j;
	}
}

void
csv_window_load_cb (GtkWidget * widget, gpointer data)
{
	GtkWidget *csvloadwin,*hbox,*button,*label,*win_vbox,*list_item,*scrolled_window;
	gchar buffer[256],*ab;
	gint i,n;
	
	csvloadwin = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect (GTK_OBJECT (csvloadwin), "delete_event",
			    (GtkSignalFunc) csv_load_window_destroy, NULL);
	gtk_window_set_policy (GTK_WINDOW (csvloadwin), FALSE, FALSE,
			       FALSE);
	gtk_window_set_title (GTK_WINDOW (csvloadwin), _("load import csv index"));
	gtk_window_set_wmclass (GTK_WINDOW (csvloadwin), "load csv index",
				"yq");
	gtk_container_border_width (GTK_CONTAINER (csvloadwin), 5);

	win_vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add (GTK_CONTAINER (csvloadwin), win_vbox);
	gtk_widget_show (win_vbox);

	hbox = gtk_hbox_new (TRUE, 0);
	gtk_box_pack_end (GTK_BOX (win_vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show (hbox);

	button = gtk_button_new_with_label (_("delete"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    (GtkSignalFunc) csv_load_window_del_cb, csvloadwin);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 25);
	gtk_widget_show (button);

	button = gtk_button_new_with_label (_("OK"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    (GtkSignalFunc) csv_load_window_ok_cb, csvloadwin);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 10);
	gtk_widget_show (button);

	button = gtk_button_new_with_label (_("Cancel"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    (GtkSignalFunc) csv_load_window_cancel_cb, csvloadwin);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 5);
	gtk_widget_show (button);
	
    scrolled_window=gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_usize(scrolled_window, 250, 150);
    gtk_container_add(GTK_CONTAINER(win_vbox), scrolled_window);
    gtk_widget_show(scrolled_window);

	csvloadlist=gtk_list_new();
    gtk_scrolled_window_add_with_viewport( GTK_SCROLLED_WINDOW(scrolled_window),csvloadlist);
    gtk_widget_show(csvloadlist);
	yq_cfg_read_int(rccfgfile,"csvindex_0","a",&n);
	for (i=0;i<n;i++)
	{
		sprintf(buffer, "csvindex_%d", i);
		yq_cfg_read_string(rccfgfile,buffer,"n",&ab);
	    label=gtk_label_new(ab);
		g_free(ab);
    	list_item=gtk_list_item_new();
	    gtk_container_add(GTK_CONTAINER(list_item), label);
    	gtk_widget_show(label);
		gtk_container_add(GTK_CONTAINER(csvloadlist), list_item);
	    gtk_widget_show(list_item);
	}
	gtk_list_select_item(GTK_LIST(csvloadlist),0);
	gtk_widget_show (csvloadwin);
	gtk_grab_add (csvloadwin);
}

void
csv_load_window_del_cb (GtkWidget * widget, gpointer data)
{
	gint n,a,i;
	gchar tz[256],*ab,tz2[256];
	
	if (!GTK_LIST(csvloadlist) -> selection)
		return;
	gtk_grab_remove (GTK_WIDGET(data));
	n=gtk_list_child_position(GTK_LIST(csvloadlist),GTK_WIDGET(GTK_LIST(csvloadlist) -> selection->data));
	if (n==0)
	{
		Popup(_("can't delete the default"));
	}
	else
	{
		sprintf(tz,"csvindex_%d",n);
		yq_cfg_read_string (rccfgfile,tz,"n", &ab);
		sprintf (tz, _("Are you sure to delete \"%s\" ?"), ab);
		g_free (ab);
		Areyousure (tz);
		if (!yesnoreturn)
			return;
		yq_cfg_read_int (rccfgfile, "csvindex_0", "a", &a);
		yq_cfg_write_int (rccfgfile, "csvindex_0", "a", a-1);
		sprintf(tz,"csvindex_%d",n);
		yq_cfg_remove_section(rccfgfile, tz);
		for (i = n+1; i <= a; i++)
		{
			sprintf (tz, "csvindex_%d",i);
			sprintf (tz2, "csvindex_%d",i-1);
			yq_cfg_rename_sections (rccfgfile, tz, tz2);
		}
		gtk_list_clear_items(GTK_LIST(csvloadlist),n,n+1);
	}
	gtk_grab_add (GTK_WIDGET(data));
}

void
csv_load_window_ok_cb (GtkWidget * widget, gpointer data)
{
	gint i,n,a;
	gchar tz[256],ti[4];

	if (!GTK_LIST(csvloadlist) -> selection)
		n=0;
	else
		n=gtk_list_child_position(GTK_LIST(csvloadlist),GTK_WIDGET(GTK_LIST(csvloadlist) -> selection->data));
	if (n==0)
	{
		if (imtitlecont>=46)
		{
			for (i=0;i<imtitlecont;i++)
			{
				if (i<=45)
				{
					*(imindexes+i)=i;
					a=i;
				}
				else
				{
					*(imindexes+i)=-1;
					a=46;
				}
				gtk_option_menu_set_history(GTK_OPTION_MENU(*(optmenus+i)),a);
			}
		}
		else
		{
			for (i=0;i<imtitlecont;i++)
			{
				*(imindexes+i)=i;
				gtk_option_menu_set_history(GTK_OPTION_MENU(*(optmenus+i)),i);
			}
			for (i=n;i<46;i++)
			{
				*(imindexes+i)=-2;
			}
		}	
	}
	else
	{
		sprintf(tz,"csvindex_%d",n);
		for (i=0;i<imtitlecont;i++)
		{
			sprintf(ti,"%d",i+1);
			if (!yq_cfg_read_int(rccfgfile,tz,ti,&a))
			{
				if (i>45) 
				{
					a=34;
					*(imindexes+i)=-1;
				}
				else
				{
					a=i+1;
					*(imindexes+i)=i;
				}
			}
			else
			{
				if (a==34)
					*(imindexes+i)=-1;
				if (a==35)
					*(imindexes+i)=-2;
				if (a<34)
					*(imindexes+i)=a-1;
			}
			gtk_option_menu_set_history(GTK_OPTION_MENU(*(optmenus+i)),a-1);
		}
		if (imtitlecont<46)
		{
			for (i=imtitlecont;i<46;i++)
				*(imindexes+i)=-2;
		}
	}
	gtk_grab_remove (GTK_WIDGET(data));
	gtk_widget_destroy (GTK_WIDGET(data));
}

void
csv_load_window_cancel_cb (GtkWidget * widget, gpointer data)
{
	gtk_grab_remove (widget);
	gtk_widget_destroy (GTK_WIDGET(data));
}

void
csv_window_destroy(GtkWidget * widget, gpointer data)
{
	gtk_main_quit ();
	gtk_widget_destroy (widget);	
}

void
csv_load_window_destroy(GtkWidget * widget, gpointer data)
{
	gtk_grab_remove (widget);
	gtk_widget_destroy (widget);
}

void
csv_window_save_cb (GtkWidget * widget, gpointer data)
{
	gint a,i;
	gchar tz[256],ti[4];
	
	yq_cfg_read_int(rccfgfile,"csvindex_0","a",&a);
	sprintf(tz,_("my csv index_%d"),a);
	InputName(_("save csv index as:"),tz);
	if (!yesnoreturn)
		return;
	if (sFilename[0] == '\0')
	{
		Popup (_("the name can't be null !"));
		return;
	}
	sprintf(tz,"csvindex_%d",a);
	yq_cfg_write_int(rccfgfile,"csvindex_0","a",a+1);
	yq_cfg_write_string(rccfgfile,tz,"n",sFilename);
	for (i=0;i<imtitlecont;i++)
	{
		sprintf(ti,"%d",i+1);
		switch (*(imindexes+i))
		{
			case -1:
			{
				a=34;
				break;
			}
			case -2:
			{
				a=35;
				break;
			}
			default:
			{
				a=*(imindexes+i)+1;
			}		
		}
		yq_cfg_write_int(rccfgfile,tz,ti,a);
	}
}

void
csv_window_ok_cb (GtkWidget * widget, gpointer data)
{
	gint i,j;
	gboolean m;
	gchar tt[256];
	
	m=FALSE;
	for (i=0;i<imtitlecont;i++)
	{
		if (*(imindexes+i)==0)
		{
			m=TRUE;
			break;
		}			
	}
	if (!m)
	{
		Popup(_("need one import as name"));
		return;
	}
	for (i=0;i<imtitlecont;i++)
	{
		for (j=i+1;j<imtitlecont;j++)
		{
			if (*(imindexes+i)>=0)
				if (*(imindexes+i)==*(imindexes+j))
				{
					sprintf(tt,_("the import to index%d is the same as index%d"),i+1,j+1);
					Popup(tt);
					return;
				}
		}
	}
	yesnoreturn=1;
	gtk_main_quit ();
	gtk_widget_destroy (GTK_WIDGET(data));
}
void
csv_window_cancel_cb (GtkWidget * widget, gpointer data)
{
	yesnoreturn=0;
	gtk_main_quit ();
	gtk_widget_destroy (GTK_WIDGET(data));
}
