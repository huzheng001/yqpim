#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "intl.h"

#include "configfile.h"
#include <string.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "yqtype.h"

extern struct _YQList yqlist;
extern gboolean nb_changed;
extern ConfigFile *rccfgfile;
extern GdkPixmap *yqpixmaps[MAX_IMAGE_TYPES];
extern GdkBitmap *yqmasks[MAX_IMAGE_TYPES];


GtkWidget *nbitems1[14],*nbitems2[12],*nbitems3[9],*nbitems4[14],*nbitems5[8],*nbitems6[3];
static GtkWidget *rubyentry1,*rubyentry2;


void CloseDialog (GtkWidget * widget, gpointer data);
void ClosingDialog (GtkWidget * widget, gpointer data);
void nb_setchanged (GtkWidget * widget, gpointer data);
void toolbar_goup (GtkWidget * widget, gpointer data);
gint yq_notebook_key_press (GtkWidget * widget, GdkEvent *event,gpointer func_data);


gint
yq_notebook_key_press (GtkWidget * widget, GdkEvent *event,
		       gpointer func_data)
{
	if (event->key.keyval == GDK_Escape)
	{
		toolbar_goup(NULL,NULL);
		return TRUE;
	}
	return FALSE;
}

GtkWidget *
yq_nb_createcombobox1 ()
{
	GList *cbitems = NULL;
	GtkWidget *combo;

	cbitems = g_list_append (cbitems, _("Unspecified"));
	cbitems = g_list_append (cbitems, _("Male"));
	cbitems = g_list_append (cbitems, _("Female"));

	combo = gtk_combo_new ();
	gtk_combo_set_popdown_strings (GTK_COMBO (combo), cbitems);
	g_list_free(cbitems);
	gtk_entry_set_editable (GTK_ENTRY (GTK_COMBO (combo)->entry), FALSE);
	gtk_widget_show (combo);
	return (combo);
}

void
yq_nb_changed(GtkWidget * widget, gpointer data)
{
	GList *cbitems = NULL;
	gchar *a,*b,*c,*d,*n[4];
	static gint i=-2,exit=0;
	gint j;
	
		if ((widget== nbitems2[2]) || (widget==nbitems2[3]))  /*last name or first name*/
		{				
			for (j=0;j<4;j++)
				n[j]=NULL;
			a=gtk_entry_get_text(GTK_ENTRY(nbitems2[2]));
			b=gtk_entry_get_text(GTK_ENTRY(nbitems2[3]));
		 	c=gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(nbitems2[5])->entry));
			if ((i==-2)||((*a=='\0')&&(*b=='\0')&&(*c=='\0')))
				yq_cfg_read_int(rccfgfile,"setting","displayname",&i);
			if (i==(-1))
				cbitems = g_list_append (cbitems, c);
			if ((*a=='\0')&&(*b=='\0'))
			{
				if (i>=0)
				{
					n[i]=g_strdup("");
					cbitems = g_list_append (cbitems,"");
				}
			}
			if ((*a!='\0')&&(*b=='\0'))
			{
				if (i>=0) n[i]=g_strdup(a);
				cbitems = g_list_append (cbitems, a);
			}
			if ((*a=='\0')&&(*b!='\0'))
			{
				if (i>=0) n[i]=g_strdup(b);
				cbitems = g_list_append (cbitems, b);
			}
			if ((*a!='\0')&&(*b!='\0'))
			{
				d=g_malloc(strlen(a)+strlen(b)+3);
				sprintf(d,"%s %s",a,b);
				n[0]=g_strdup(d);
				cbitems = g_list_append (cbitems, n[0]);
				sprintf(d,"%s%s",a,b);
				n[1]=g_strdup(d);
				cbitems = g_list_append (cbitems, n[1]);
				sprintf(d,"%s, %s",a,b);
				n[2]=g_strdup(d);
				cbitems = g_list_append (cbitems, n[2]);
				sprintf(d,"%s %s",b,a);
				n[3]=g_strdup(d);
				cbitems = g_list_append (cbitems, n[3]);
				g_free(d);
			}
			exit=1;
			gtk_combo_set_popdown_strings (GTK_COMBO (nbitems2[5]), cbitems);
			g_list_free(cbitems);
			
			if (i>=0)
				gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(nbitems2[5])->entry),n[i]);
			exit=0;
			for (j=0;j<4;j++)
				if (n[j])
					g_free(n[j]);
		}
		else if (widget== GTK_COMBO(nbitems2[5])->entry)  /*display name*/
		{
			if (exit) return;
			a=gtk_entry_get_text(GTK_ENTRY(nbitems2[2]));
				b=gtk_entry_get_text(GTK_ENTRY(nbitems2[3]));
				c=gtk_entry_get_text(GTK_ENTRY(widget));
				d=g_malloc(strlen(a)+strlen(b)+3);
				sprintf(d,"%s %s",a,b);
				if (strcmp(d,c)==0)
				{
					i=0;
				}
				else
				{		
					sprintf(d,"%s%s",a,b);
					if (strcmp(d,c)==0)
					{
						if (*c!='\0')
							i=1;
					}
					else
					{
						sprintf(d,"%s, %s",a,b);
						if (strcmp(d,c)==0)
						{
							i=2;
						}
						else
						{
							sprintf(d,"%s %s",b,a);
							if (strcmp(d,c)==0)
							{
								i=3;
							}	
							else
								i=-1;
						}
					}
				}
				g_free(d);					
				gtk_label_set_text(GTK_LABEL(nbitems1[0]),c);
				if (i>=0)
					yq_cfg_write_int(rccfgfile,"setting","displayname",i);
		}
		else if (widget== nbitems2[9] )  /*email*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[1]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems2[7] )  /*icq*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[2]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems3[1] )  /*home phone*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[3]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems4[10] )  /*pager*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[4]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems3[4] )   /*mobile*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[5]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems3[8] )   /*personal web page*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[6]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems4[6] )   /*business phone*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[7]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems4[8] )   /*business fax*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[8]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems4[1] )   /*job title*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[9]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems4[3] )   /*department*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[10]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems4[4] )    /*office*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[11]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems4[0] )   /*company name*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[12]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else if (widget== nbitems4[13] )    /*business web page*/
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[13]),gtk_entry_get_text(GTK_ENTRY(widget)));
		}
		else
		{
			
		}

	nb_setchanged (widget, data);
}

void
yq_nb_name_ruby_yesfunc(GtkWidget * widget, gpointer data)
{
	gchar *a,*b;
	gboolean c=FALSE;
	
	gtk_label_get(GTK_LABEL(nbitems2[0]),&a);
	b=gtk_entry_get_text(GTK_ENTRY(rubyentry1));
	if (strcmp(a,b)!=0)
	{
		gtk_label_set_text(GTK_LABEL(nbitems2[0]),b);
		c=TRUE;
	}
	gtk_label_get(GTK_LABEL(nbitems2[1]),&a);
	b=gtk_entry_get_text(GTK_ENTRY(rubyentry2));
	if (strcmp(a,b)!=0)
	{
		gtk_label_set_text(GTK_LABEL(nbitems2[1]),b);
		c=TRUE;
	}
	if (c)
		nb_setchanged(widget,data);
	gtk_widget_destroy (GTK_WIDGET (data));
}

void
yq_nb_name_ruby_func(GtkWidget * widget, gpointer data)
{
	GtkWidget *dialog_window,*table,*label,*entry,*button;
	char *a;
	
	dialog_window = gtk_dialog_new ();
	gtk_signal_connect (GTK_OBJECT (dialog_window), "destroy",
			    GTK_SIGNAL_FUNC (ClosingDialog), NULL);
	gtk_window_set_title (GTK_WINDOW (dialog_window), _("Ruby"));
	gtk_container_border_width (GTK_CONTAINER (dialog_window), 5);
	table=gtk_table_new(3,2,FALSE);
	label=gtk_label_new(_("Ruby Last Name:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1, GTK_FILL, 0, 10, 0);
	label=gtk_label_new(_("Ruby First Name:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table), label, 1, 2, 0, 1, GTK_FILL, 0, 10, 0);
	rubyentry1=gtk_entry_new_with_max_length(50);
	gtk_label_get(GTK_LABEL(nbitems2[0]),&a);
	gtk_entry_set_text(GTK_ENTRY(rubyentry1), a);
	gtk_widget_show(rubyentry1);
	gtk_table_attach (GTK_TABLE (table), rubyentry1, 0, 1, 1, 2, GTK_FILL, 0, 10, 5);
	rubyentry2=gtk_entry_new_with_max_length(50);
	gtk_label_get(GTK_LABEL(nbitems2[1]),&a);
	gtk_entry_set_text(GTK_ENTRY(rubyentry2), a);
	gtk_widget_show(rubyentry2);
	gtk_table_attach (GTK_TABLE (table), rubyentry2, 1, 2, 1, 2, GTK_FILL, 0, 10, 5);
	entry=gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry), gtk_entry_get_text(GTK_ENTRY(nbitems2[2])));
	gtk_widget_set_sensitive (entry, FALSE);
	gtk_widget_show(entry);
	gtk_table_attach (GTK_TABLE (table), entry, 0, 1, 2, 3, GTK_FILL, 0, 10, 5);
	entry=gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry), gtk_entry_get_text(GTK_ENTRY(nbitems2[3])));
	gtk_widget_set_sensitive (entry, FALSE);
	gtk_widget_show(entry);
	gtk_table_attach (GTK_TABLE (table), entry, 1, 2, 2, 3, GTK_FILL, 0, 10, 5);
	gtk_widget_show(table);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->vbox),
			    table, TRUE, TRUE, 10);
	button = gtk_button_new_with_label (_("OK"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (yq_nb_name_ruby_yesfunc),
			     dialog_window);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
			    button, TRUE, TRUE, 0);
	gtk_widget_show (button);
	gtk_widget_grab_default (button);
	button = gtk_button_new_with_label (_("Cancel"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (CloseDialog), dialog_window);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
			    button, TRUE, TRUE, 0);
	gtk_widget_show (button);
	gtk_widget_show (dialog_window);
	gtk_grab_add (dialog_window);
	gtk_main ();
}

GtkWidget *
createpage1 ()
{
	GtkWidget *label,*hbox,*vbox,*pixmap_widget,*separator,*table;
	
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
	hbox=gtk_hbox_new(FALSE,0);
	pixmap_widget = gtk_pixmap_new (yqpixmaps[YQPIM], yqmasks[YQPIM]);
	gtk_widget_show (pixmap_widget);
	gtk_box_pack_start (GTK_BOX (hbox), pixmap_widget, FALSE, TRUE, 10);
	label=gtk_label_new(_("Summary of information about this contact"));
	gtk_widget_show(label);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);
	gtk_widget_show(hbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
	separator = gtk_hseparator_new ();
	gtk_widget_show(separator);
	gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, TRUE, 10);
	table = gtk_table_new (14, 2, FALSE);
	label=gtk_label_new(_("Name:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[0]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[0]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[0], 1, 2, 0, 1, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("E-Mail Address:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[1]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[1]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[1], 1, 2, 1, 2, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("ICQ:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[2]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[2]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 2, 3, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[2], 1, 2, 2, 3, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Home Phone:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[3]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[3]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 3, 4, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[3], 1, 2, 3, 4, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Pager:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[4]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[4]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 4, 5, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[4], 1, 2, 4, 5, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Mobile:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[5]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[5]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 5, 6, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[5], 1, 2, 5, 6, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Personal Web Page:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[6]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[6]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 6, 7, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[6], 1, 2, 6, 7, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Business Phone:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[7]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[7]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 7, 8, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[7], 1, 2, 7, 8, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Business Fax:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[8]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[8]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 8, 9, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[8], 1, 2, 8, 9, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Job Title:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[9]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[9]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 9, 10, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[9], 1, 2, 9, 10, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Department:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[10]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[10]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 10, 11, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[10], 1, 2, 10, 11, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Office:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[11]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[11]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 11, 12, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[11], 1, 2, 11, 12, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Company Name:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[12]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[12]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 12, 13, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[12], 1, 2, 12, 13, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Business Web page:"));
	gtk_misc_set_alignment (GTK_MISC (label), 1, .5);
	gtk_widget_show(label);
	nbitems1[13]=gtk_label_new(NULL);
	gtk_widget_show(nbitems1[13]);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 13, 14, GTK_FILL, 0, 10, 0);
	gtk_table_attach (GTK_TABLE (table), nbitems1[13], 1, 2, 13, 14, GTK_FILL, 0, 0, 0);
	gtk_widget_show (table);
	gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, TRUE, 0);
	gtk_widget_show(vbox);
	return vbox;
}

GtkWidget *
createpage2 ()
{
	GtkWidget *label,*button,*hbox,*vbox,*pixmap_widget,*separator,*vscroll,*table;
	
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
	hbox=gtk_hbox_new(FALSE,0);
	pixmap_widget = gtk_pixmap_new (yqpixmaps[NAME], yqmasks[NAME]);
	gtk_widget_show (pixmap_widget);
	gtk_box_pack_start (GTK_BOX (hbox), pixmap_widget, FALSE, TRUE, 10);
	label=gtk_label_new(_("Entry name and e-mail information about this contact here."));
	gtk_widget_show(label);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);
	gtk_widget_show(hbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
	separator = gtk_hseparator_new ();
	gtk_widget_show(separator);
	gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, TRUE, 10);
	table = gtk_table_new (8, 7, FALSE);
	nbitems2[0]=gtk_label_new("");
	gtk_misc_set_alignment (GTK_MISC (nbitems2[0]), 0, .5);
	gtk_widget_show(nbitems2[0]);
	gtk_table_attach (GTK_TABLE (table), nbitems2[0], 1, 3, 0, 1, GTK_FILL, 0, 5, 0);
	nbitems2[1]=gtk_label_new("");
	gtk_misc_set_alignment (GTK_MISC (nbitems2[1]), 0, .5);
	gtk_widget_show(nbitems2[1]);
	gtk_table_attach (GTK_TABLE (table), nbitems2[1], 4, 5, 0, 1, GTK_FILL, 0, 5, 0);
	button=gtk_button_new_with_label(_("Ruby..."));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (yq_nb_name_ruby_func), NULL);
	gtk_widget_show(button);
	gtk_table_attach (GTK_TABLE (table), button, 6, 7, 0, 1, GTK_FILL, 0, 5, 0);
	label=gtk_label_new(_("Last:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2, GTK_FILL, 0, 10, 5);
	nbitems2[2]=gtk_entry_new_with_max_length(50);
	gtk_signal_connect (GTK_OBJECT (nbitems2[2]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_set_usize (nbitems2[2], 100, -1);
	gtk_widget_show(nbitems2[2]);
	gtk_table_attach (GTK_TABLE (table),nbitems2[2] , 1, 3, 1, 2, GTK_FILL, 0, 5, 5);
	label=gtk_label_new(_("First:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table), label, 3, 4, 1, 2, GTK_FILL, 0, 5, 5);
	nbitems2[3]=gtk_entry_new_with_max_length(50);
	gtk_signal_connect (GTK_OBJECT (nbitems2[3]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_set_usize (nbitems2[3], 100, -1);
	gtk_widget_show(nbitems2[3]);
	gtk_table_attach (GTK_TABLE (table),nbitems2[3] , 4, 5, 1, 2, GTK_FILL | GTK_EXPAND, 0, 5, 5);
	label=gtk_label_new(_("Title:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 2, 3, GTK_FILL, 0, 10, 0);
	nbitems2[4]=gtk_entry_new_with_max_length(50);
	gtk_signal_connect (GTK_OBJECT (nbitems2[4]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_set_usize (nbitems2[4], 50, -1);
	gtk_widget_show(nbitems2[4]);
	gtk_table_attach (GTK_TABLE (table),nbitems2[4] , 1, 2, 2, 3, GTK_FILL | GTK_EXPAND, 0, 5, 0);
	label=gtk_label_new(_("Display:"));
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table), label, 2, 3, 2, 3, GTK_FILL, 0, 5, 0);
	nbitems2[5]=gtk_combo_new();
	gtk_widget_set_usize (nbitems2[5], 100, -1);
	gtk_signal_connect (GTK_OBJECT (GTK_COMBO (nbitems2[5])->entry), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems2[5]);
	gtk_table_attach (GTK_TABLE (table),nbitems2[5] , 3, 5, 2, 3, GTK_FILL, 0, 5, 0);
	label=gtk_label_new(_("Nickname:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table), label, 5, 6, 2, 3, GTK_FILL, 0, 5, 0);
	nbitems2[6]=gtk_entry_new_with_max_length(50);
	gtk_signal_connect (GTK_OBJECT (nbitems2[6]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_set_usize (nbitems2[6], 60, -1);
	gtk_widget_show(nbitems2[6]);
	gtk_table_attach (GTK_TABLE (table),nbitems2[6] , 6, 7, 2, 3, GTK_FILL | GTK_EXPAND, 0, 5, 0);
	label=gtk_label_new("");  /*just make some space*/
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 7, 3, 4, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("ICQ:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 4, 5, GTK_FILL, 0, 10, 5);
	nbitems2[7]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems2[7], 100, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems2[7]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems2[7]);
	gtk_table_attach (GTK_TABLE (table),nbitems2[7] , 1, 3, 4, 5, GTK_FILL, 0, 5, 5);
	label=gtk_label_new(_("AIM:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 3, 4, 4, 5, GTK_FILL, 0, 5, 5);
	nbitems2[8]=gtk_entry_new_with_max_length(50);
	gtk_signal_connect (GTK_OBJECT (nbitems2[8]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems2[8]);
	gtk_table_attach (GTK_TABLE (table),nbitems2[8] , 4, 7, 4, 5, GTK_FILL, 0, 5, 5);
	label=gtk_label_new(_("Email:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 5, 6, GTK_FILL, 0, 10, 0);
	nbitems2[9]=gtk_entry_new_with_max_length(100);
	gtk_signal_connect (GTK_OBJECT (nbitems2[9]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems2[9]);
	gtk_table_attach (GTK_TABLE (table),nbitems2[9] , 1, 7, 5, 6, GTK_FILL, 0, 5, 0);
	label=gtk_label_new(_("Email2:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 6, 7, GTK_FILL, 0, 10, 5);
	nbitems2[10]=gtk_entry_new_with_max_length(100);
	gtk_signal_connect (GTK_OBJECT (nbitems2[10]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems2[10]);
	gtk_table_attach (GTK_TABLE (table),nbitems2[10] , 1, 7, 6, 7, GTK_FILL, 0, 5, 5);
	label=gtk_label_new(_("Comment:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 7, 8, GTK_FILL, 0, 10, 0);
	nbitems2[11]=gtk_text_new(NULL,NULL);
	gtk_signal_connect (GTK_OBJECT (nbitems2[11]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_text_set_editable (GTK_TEXT (nbitems2[11]), TRUE);
	gtk_widget_set_usize (nbitems2[11], -1, 40);
	gtk_widget_show(nbitems2[11]);
	vscroll = gtk_vscrollbar_new (GTK_TEXT (nbitems2[11])->vadj);
	gtk_widget_show (vscroll);
	hbox=gtk_hbox_new(FALSE,0);
	gtk_box_pack_start (GTK_BOX (hbox), nbitems2[11], TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), vscroll, FALSE, FALSE, 0);
	gtk_widget_show(hbox);
	gtk_table_attach (GTK_TABLE (table),hbox , 1, 7, 7, 8, GTK_FILL, GTK_FILL | GTK_EXPAND, 5, 0);
	gtk_widget_show (table);
	gtk_box_pack_start (GTK_BOX (vbox), table, TRUE, TRUE, 0);
	gtk_widget_show(vbox);
	return vbox;
}

GtkWidget *
createpage3 ()
{
	GtkWidget *label,*button,*hbox,*vbox,*pixmap_widget,*separator,*table,*vscroll;
	
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
	hbox=gtk_hbox_new(FALSE,0);
	pixmap_widget = gtk_pixmap_new (yqpixmaps[HOME], yqmasks[HOME]);
	gtk_widget_show (pixmap_widget);
	gtk_box_pack_start (GTK_BOX (hbox), pixmap_widget, FALSE, TRUE, 10);
	label=gtk_label_new(_("Entry home-related information about this contact here."));
	gtk_widget_show(label);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);
	gtk_widget_show(hbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
	separator = gtk_hseparator_new ();
	gtk_widget_show(separator);
	gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, TRUE, 10);
	table = gtk_table_new (7, 6, FALSE);
	label=gtk_label_new(_("Street Address:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 0, 1, GTK_FILL, 0, 10, 0);
	nbitems3[0]=gtk_text_new (NULL, NULL);
	gtk_signal_connect (GTK_OBJECT (nbitems3[0]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_text_set_editable (GTK_TEXT (nbitems3[0]), TRUE);
	gtk_widget_set_usize (nbitems3[0], -1, 30);
	gtk_widget_show(nbitems3[0]);
	vscroll = gtk_vscrollbar_new (GTK_TEXT (nbitems3[0])->vadj);
	gtk_widget_show (vscroll);
	hbox=gtk_hbox_new(FALSE,0);
	gtk_widget_set_usize (hbox, 50, -1);
	gtk_box_pack_start (GTK_BOX (hbox), nbitems3[0], TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), vscroll, FALSE, FALSE, 0);
	gtk_widget_show(hbox);
	gtk_table_attach (GTK_TABLE (table),hbox , 1, 3, 0, 2, GTK_FILL, 0, 5, 0);
	label=gtk_label_new(_("Phone:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 3, 4, 0, 1, GTK_FILL, 0, 5, 0);
	nbitems3[1]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems3[1], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems3[1]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems3[1]);
	gtk_table_attach (GTK_TABLE (table),nbitems3[1] , 4, 6, 0, 1, GTK_FILL | GTK_EXPAND, 0, 0, 0);
	label=gtk_label_new(_("Fax:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 3, 4, 1, 2, GTK_FILL, 0, 5, 5);
	nbitems3[2]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems3[2], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems3[2]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems3[2]);
	gtk_table_attach (GTK_TABLE (table),nbitems3[2] , 4, 6, 1, 2, GTK_FILL, 0, 0, 5);
	label=gtk_label_new(_("City:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 2, 3, GTK_FILL, 0, 10, 0);
	nbitems3[3]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems3[3], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems3[3]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems3[3]);
	gtk_table_attach (GTK_TABLE (table),nbitems3[3] , 1, 3, 2, 3, GTK_FILL | GTK_EXPAND, 0, 5, 0);
	label=gtk_label_new(_("Mobile:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 3, 4, 2, 3, GTK_FILL, 0, 5, 0);
	nbitems3[4]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems3[4], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems3[4]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems3[4]);
	gtk_table_attach (GTK_TABLE (table),nbitems3[4] , 4, 6, 2, 3, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("State/Province:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 3, 4, GTK_FILL, 0, 10, 5);
	nbitems3[5]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems3[5], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems3[5]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems3[5]);
	gtk_table_attach (GTK_TABLE (table),nbitems3[5] , 1, 3, 3, 4, GTK_FILL, 0, 5, 5);
	label=gtk_label_new(_("Zip Code:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 4, 5, GTK_FILL, 0, 10, 0);
	nbitems3[6]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems3[6], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems3[6]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems3[6]);
	gtk_table_attach (GTK_TABLE (table),nbitems3[6] , 1, 3, 4, 5, GTK_FILL, 0, 5, 0);
	label=gtk_label_new(_("Country/Region:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 5, 6, GTK_FILL, 0, 10, 5);
	nbitems3[7]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems3[7], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems3[7]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems3[7]);
	gtk_table_attach (GTK_TABLE (table),nbitems3[7] , 1, 3, 5, 6, GTK_FILL, 0, 5, 5);
	label=gtk_label_new(_("Web Page:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 6, 7, GTK_FILL, 0, 10, 0);
	nbitems3[8]=gtk_entry_new_with_max_length(100);
	gtk_widget_set_usize (nbitems3[8], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems3[8]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems3[8]);
	gtk_table_attach (GTK_TABLE (table),nbitems3[8] , 1, 5, 6, 7, GTK_FILL, 0, 5, 0);
	button=gtk_button_new_with_label("Go");
	gtk_widget_set_usize (button, 15, -1);
	gtk_widget_show(button);
	gtk_table_attach (GTK_TABLE (table),button , 5, 6, 6, 7, GTK_FILL, 0, 0, 0);
	gtk_widget_show (table);
	gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, TRUE, 0);
	gtk_widget_show(vbox);
	return vbox;
}

GtkWidget *
createpage4 ()
{
	GtkWidget *label,*button,*hbox,*vbox,*pixmap_widget,*separator,*table,*vscroll;
	
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
	hbox=gtk_hbox_new(FALSE,0);
	pixmap_widget = gtk_pixmap_new (yqpixmaps[BUSINESS], yqmasks[BUSINESS]);
	gtk_widget_show (pixmap_widget);
	gtk_box_pack_start (GTK_BOX (hbox), pixmap_widget, FALSE, TRUE, 10);
	label=gtk_label_new(_("Entry business-related information about this contact here."));
	gtk_widget_show(label);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);
	gtk_widget_show(hbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
	separator = gtk_hseparator_new ();
	gtk_widget_show(separator);
	gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, TRUE, 10);
	table = gtk_table_new (8, 6, FALSE);
	label=gtk_label_new(_("Company:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 0, 1, GTK_FILL, 0, 10, 0);
	nbitems4[0]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[0], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[0]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[0]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[0] , 1, 3, 0, 1, GTK_FILL, 0, 5, 0);
	label=gtk_label_new(_("Job Title:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 3, 4, 0, 1, GTK_FILL, 0, 5, 0);
	nbitems4[1]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[1], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[1]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[1]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[1] , 4, 6, 0, 1, GTK_FILL | GTK_EXPAND, 0, 0, 0);
	label=gtk_label_new(_("Street Address:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 1, 2, GTK_FILL, 0, 10, 0);
	nbitems4[2]=gtk_text_new (NULL, NULL);
	gtk_signal_connect (GTK_OBJECT (nbitems4[2]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_text_set_editable (GTK_TEXT (nbitems4[2]), TRUE);
	gtk_widget_set_usize (nbitems4[2], -1, 30);
	gtk_widget_show(nbitems4[2]);
	vscroll = gtk_vscrollbar_new (GTK_TEXT (nbitems4[2])->vadj);
	gtk_widget_show (vscroll);
	hbox=gtk_hbox_new(FALSE,0);
	gtk_widget_set_usize (hbox, 50, -1);
	gtk_box_pack_start (GTK_BOX (hbox), nbitems4[2], TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), vscroll, FALSE, FALSE, 0);
	gtk_widget_show(hbox);
	gtk_table_attach (GTK_TABLE (table),hbox , 1, 3, 1, 3, GTK_FILL | GTK_EXPAND, 0, 5, 0);
	label=gtk_label_new(_("Department:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 3, 4, 1, 2, GTK_FILL, 0, 5, 5);
	nbitems4[3]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[3], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[3]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[3]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[3] , 4, 6, 1, 2, GTK_FILL, 0, 0, 5);
	label=gtk_label_new(_("Office:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 3, 4, 2, 3, GTK_FILL, 0, 5, 0);
	nbitems4[4]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[4], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[4]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[4]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[4] , 4, 6, 2, 3, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("City:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 3, 4, GTK_FILL, 0, 10, 5);
	nbitems4[5]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[5], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[5]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[5]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[5] , 1, 3, 3, 4, GTK_FILL, 0, 5, 5);
	label=gtk_label_new(_("Phone:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 3, 4, 3, 4, GTK_FILL, 0, 5, 5);
	nbitems4[6]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[6], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[6]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[6]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[6] , 4, 6, 3, 4, GTK_FILL, 0, 0, 5);
	label=gtk_label_new(_("State/Province:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 4, 5, GTK_FILL, 0, 10, 0);
	nbitems4[7]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[7], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[7]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[7]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[7] , 1, 3, 4, 5, GTK_FILL, 0, 5, 0);
	label=gtk_label_new(_("Fax:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 3, 4, 4, 5, GTK_FILL, 0, 5, 0);
	nbitems4[8]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[8], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[8]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[8]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[8] , 4, 6, 4, 5, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Zip Code:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 5, 6, GTK_FILL, 0, 10, 5);
	nbitems4[9]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[9], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[9]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[9]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[9] , 1, 3, 5, 6, GTK_FILL, 0, 5, 5);
	label=gtk_label_new(_("Pager:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 3, 4, 5, 6, GTK_FILL, 0, 5, 5);
	nbitems4[10]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[10], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[10]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[10]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[10] , 4, 6, 5, 6, GTK_FILL, 0, 0, 5);
	label=gtk_label_new(_("Country/Region:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 6, 7, GTK_FILL, 0, 10, 0);
	nbitems4[11]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[11], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[11]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[11]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[11] , 1, 3, 6, 7, GTK_FILL, 0, 5, 0);
	label=gtk_label_new(_("IP Phone:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 3, 4, 6, 7, GTK_FILL, 0, 5, 0);
	nbitems4[12]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems4[12], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems4[12]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[12]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[12] , 4, 6, 6, 7, GTK_FILL, 0, 0, 0);
	label=gtk_label_new(_("Web Page:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 7, 8, GTK_FILL, 0, 10, 5);
	nbitems4[13]=gtk_entry_new_with_max_length(100);
	gtk_signal_connect (GTK_OBJECT (nbitems4[13]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems4[13]);
	gtk_table_attach (GTK_TABLE (table),nbitems4[13] , 1, 5, 7, 8, GTK_FILL, 0, 5, 5);
	button=gtk_button_new_with_label("Go");
	gtk_widget_show(button);
	gtk_table_attach (GTK_TABLE (table),button , 5, 6, 7, 8, GTK_FILL, 0, 0, 5);
	gtk_widget_show (table);
	gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, TRUE, 0);
	gtk_widget_show(vbox);
	return vbox;
}

GtkWidget *
createpage5 ()
{
	GtkWidget *label,*hbox,*vbox,*pixmap_widget,*separator,*table,*vscroll;
	
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
	hbox=gtk_hbox_new(FALSE,0);
	pixmap_widget = gtk_pixmap_new (yqpixmaps[PERSONAL], yqmasks[PERSONAL]);
	gtk_widget_show (pixmap_widget);
	gtk_box_pack_start (GTK_BOX (hbox), pixmap_widget, FALSE, TRUE, 10);
	label=gtk_label_new(_("Entry personal information about this contact here."));
	gtk_widget_show(label);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);
	gtk_widget_show(hbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
	separator = gtk_hseparator_new ();
	gtk_widget_show(separator);
	gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, TRUE, 10);
	table = gtk_table_new (6, 4, FALSE);
	label=gtk_label_new(_("Gender:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 0, 1, GTK_FILL, 0, 10, 0);
	nbitems5[0]=yq_nb_createcombobox1 ();
	gtk_widget_set_usize (nbitems5[0], 50, -1);
	gtk_signal_connect (GTK_OBJECT (GTK_COMBO(nbitems5[0])->entry), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_table_attach (GTK_TABLE (table),nbitems5[0] , 1, 2, 0, 1, GTK_FILL | GTK_EXPAND, 0, 5, 0);
	label=gtk_label_new(_("Spouse:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 2, 3, 0, 1, GTK_FILL, 0, 5, 0);
	nbitems5[1]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems5[1], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems5[1]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems5[1]);
	gtk_table_attach (GTK_TABLE (table),nbitems5[1] , 3, 4, 0, 1, GTK_FILL | GTK_EXPAND, 0, 5, 0);
	label=gtk_label_new(_("Birthday:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 1, 2, GTK_FILL, 0, 10, 5);
	nbitems5[2]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems5[2], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems5[2]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems5[2]);
	gtk_table_attach (GTK_TABLE (table),nbitems5[2] , 1, 2, 1, 2, GTK_FILL, 0, 5, 5);
	label=gtk_label_new(_("Children:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 2, 3, 1, 2, GTK_FILL, 0, 5, 0);
	nbitems5[3]=gtk_text_new(NULL,NULL);
	gtk_signal_connect (GTK_OBJECT (nbitems5[3]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_text_set_editable (GTK_TEXT (nbitems5[3]), TRUE);
	gtk_widget_set_usize (nbitems5[3], -1, 30);
	gtk_widget_show(nbitems5[3]);
	vscroll = gtk_vscrollbar_new (GTK_TEXT (nbitems5[3])->vadj);
	gtk_widget_show (vscroll);
	hbox=gtk_hbox_new(FALSE,0);
	gtk_widget_set_usize (hbox, 50, -1);
	gtk_box_pack_start (GTK_BOX (hbox), nbitems5[3], TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), vscroll, FALSE, FALSE, 0);
	gtk_widget_show(hbox);
	gtk_table_attach (GTK_TABLE (table),hbox , 3, 4, 1, 3, GTK_FILL, 0, 5, 0);
	label=gtk_label_new(_("Anniversary:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 2, 3, GTK_FILL, 0, 10, 0);
	nbitems5[4]=gtk_entry_new_with_max_length(50);
	gtk_widget_set_usize (nbitems5[4], 50, -1);
	gtk_signal_connect (GTK_OBJECT (nbitems5[4]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems5[4]);
	gtk_table_attach (GTK_TABLE (table),nbitems5[4] , 1, 2, 2, 3, GTK_FILL, 0, 5, 0);
	label=gtk_label_new(_("Hobbies:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 3, 4, GTK_FILL, 0, 10, 5);
	nbitems5[5]=gtk_entry_new_with_max_length(100);
	gtk_signal_connect (GTK_OBJECT (nbitems5[5]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems5[5]);
	gtk_table_attach (GTK_TABLE (table),nbitems5[5] , 1, 4, 3, 4, GTK_FILL, 0, 5, 5);
	label=gtk_label_new(_("Speciality:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 4, 5, GTK_FILL, 0, 10, 0);
	nbitems5[6]=gtk_entry_new_with_max_length(100);
	gtk_signal_connect (GTK_OBJECT (nbitems5[6]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems5[6]);
	gtk_table_attach (GTK_TABLE (table),nbitems5[6] , 1, 4, 4, 5, GTK_FILL, 0, 5, 0);
	label=gtk_label_new(_("Saying:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 5, 6, GTK_FILL, 0, 10, 5);
	nbitems5[7]=gtk_text_new(NULL,NULL);
	gtk_signal_connect (GTK_OBJECT (nbitems5[7]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_text_set_editable (GTK_TEXT (nbitems5[7]), TRUE);
	gtk_widget_set_usize (nbitems5[7], -1, 80);
	gtk_widget_show(nbitems5[7]);
	vscroll = gtk_vscrollbar_new (GTK_TEXT (nbitems5[7])->vadj);
	gtk_widget_show (vscroll);
	hbox=gtk_hbox_new(FALSE,0);
	gtk_box_pack_start (GTK_BOX (hbox), nbitems5[7], TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), vscroll, FALSE, FALSE, 0);
	gtk_widget_show(hbox);
	gtk_table_attach (GTK_TABLE (table), hbox, 1, 4, 5, 6, GTK_FILL, GTK_FILL | GTK_EXPAND, 5, 5);
	gtk_widget_show (table);
	gtk_box_pack_start (GTK_BOX (vbox), table, TRUE, TRUE, 0);
	gtk_widget_show(vbox);
	return vbox;	
}

GtkWidget *
createpage6 ()
{
	GtkWidget *label,*hbox,*vbox,*pixmap_widget,*separator,*vscroll,*table;
	
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
	hbox=gtk_hbox_new(FALSE,0);
	pixmap_widget = gtk_pixmap_new (yqpixmaps[NOTES], yqmasks[NOTES]);
	gtk_widget_show (pixmap_widget);
	gtk_box_pack_start (GTK_BOX (hbox), pixmap_widget, FALSE, TRUE, 10);
	label=gtk_label_new(_("Additional information about this contact."));
	gtk_widget_show(label);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);
	gtk_widget_show(hbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
	separator = gtk_hseparator_new ();
	gtk_widget_show(separator);
	gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, TRUE, 10);
	
	table = gtk_table_new (5, 2, FALSE);
	label=gtk_label_new(_("Type:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 1, 0, 1, GTK_FILL, 0, 10, 0);
	nbitems6[0]=gtk_entry_new_with_max_length(100);
	gtk_signal_connect (GTK_OBJECT (nbitems6[0]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_widget_show(nbitems6[0]);
	gtk_table_attach (GTK_TABLE (table),nbitems6[0] , 1, 2, 0, 1, GTK_FILL | GTK_EXPAND, 0, 10, 0);
	label=gtk_label_new(_("Public Key:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 2, 1, 2, GTK_FILL, 0, 10, 5);
	nbitems6[1]=gtk_text_new(NULL,NULL);
	gtk_signal_connect (GTK_OBJECT (nbitems6[1]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_text_set_editable (GTK_TEXT (nbitems6[1]), TRUE);
	gtk_widget_set_usize (nbitems6[1], -1, 60);
	gtk_widget_show(nbitems6[1]);
	vscroll = gtk_vscrollbar_new (GTK_TEXT (nbitems6[1])->vadj);
	gtk_widget_show (vscroll);
	hbox=gtk_hbox_new(FALSE,0);
	gtk_box_pack_start (GTK_BOX (hbox), nbitems6[1], TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), vscroll, FALSE, FALSE, 0);
	gtk_widget_show(hbox);
	gtk_table_attach (GTK_TABLE (table), hbox, 0, 2, 2, 3, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 10, 0);
	label=gtk_label_new(_("Notes:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0, .5);
	gtk_widget_show(label);
	gtk_table_attach (GTK_TABLE (table),label , 0, 2, 3, 4, GTK_FILL, 0, 10, 5);
	nbitems6[2]=gtk_text_new(NULL,NULL);
	gtk_signal_connect (GTK_OBJECT (nbitems6[2]), "changed",
			    GTK_SIGNAL_FUNC (yq_nb_changed), NULL);
	gtk_text_set_editable (GTK_TEXT (nbitems6[2]), TRUE);
	gtk_widget_set_usize (nbitems6[2], -1, 80);
	gtk_widget_show(nbitems6[2]);
	vscroll = gtk_vscrollbar_new (GTK_TEXT (nbitems6[2])->vadj);
	gtk_widget_show (vscroll);
	hbox=gtk_hbox_new(FALSE,0);
	gtk_box_pack_start (GTK_BOX (hbox), nbitems6[2], TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), vscroll, FALSE, FALSE, 0);
	gtk_widget_show(hbox);
	gtk_table_attach (GTK_TABLE (table), hbox, 0, 2, 4, 5, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 10, 0);
	gtk_widget_show(table);
	gtk_box_pack_start (GTK_BOX (vbox), table, TRUE, TRUE, 0);
	gtk_widget_show(vbox);
	return vbox;
}


void
createnotebook ()
{
	GtkWidget *label;
	nb_changed = TRUE;
	yqlist.list = gtk_notebook_new ();
	gtk_box_pack_start (GTK_BOX (yqlist.box), yqlist.list, TRUE, TRUE, 0);
	gtk_widget_show (yqlist.list);
	gtk_signal_connect (GTK_OBJECT (yqlist.list),
		    "key_press_event",
		    GTK_SIGNAL_FUNC (yq_notebook_key_press), NULL);

	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (yqlist.list), GTK_POS_TOP);
	label = gtk_label_new (_("Summary"));
	gtk_widget_show (label);
	gtk_notebook_append_page (GTK_NOTEBOOK (yqlist.list), createpage1 (),
				  label);
	label = gtk_label_new (_("Name"));
	gtk_widget_show (label);
	gtk_notebook_append_page (GTK_NOTEBOOK (yqlist.list), createpage2 (),
				  label);
	label = gtk_label_new (_("Home"));
	gtk_widget_show (label);
	gtk_notebook_append_page (GTK_NOTEBOOK (yqlist.list), createpage3 (),
				  label);
	label = gtk_label_new (_("Business"));
	gtk_widget_show (label);
	gtk_notebook_append_page (GTK_NOTEBOOK (yqlist.list), createpage4 (),
				  label);
	label = gtk_label_new (_("Personal"));
	gtk_widget_show (label);
	gtk_notebook_append_page (GTK_NOTEBOOK (yqlist.list), createpage5 (),
				  label);
	label = gtk_label_new (_("Other"));
	gtk_widget_show (label);
	gtk_notebook_append_page (GTK_NOTEBOOK (yqlist.list), createpage6 (),
				  label);

	yqlist.listwin = NULL;
	yqlist.type = LIST_NOTEBOOK;
	nb_changed = FALSE;
}
