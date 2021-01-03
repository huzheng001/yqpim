#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "intl.h"

#include <gtk/gtk.h>
#include <string.h>

gchar sFilename[256];
gint yesnoreturn;


void yq_play_sound(gint i);



void
CloseDialog (GtkWidget * widget, gpointer data)
{
	gtk_widget_destroy (GTK_WIDGET (data));
}

void
ClosingDialog (GtkWidget * widget, gpointer data)
{
	gtk_grab_remove (GTK_WIDGET (widget));
	gtk_main_quit ();
}

void
ClosingDialog2 (GtkWidget * widget, gpointer data)
{
	gtk_main_quit ();
}


void
Popup (gchar * szMessage)
{
	static GtkWidget *label;
	GtkWidget *button;
	GtkWidget *dialog_window;

	dialog_window = gtk_dialog_new ();
	gtk_signal_connect (GTK_OBJECT (dialog_window), "destroy",
			    GTK_SIGNAL_FUNC (ClosingDialog), NULL);
	gtk_window_set_title (GTK_WINDOW (dialog_window), _("Yqpim"));
	gtk_container_border_width (GTK_CONTAINER (dialog_window), 5);
	label = gtk_label_new (szMessage);
	gtk_misc_set_padding (GTK_MISC (label), 10, 10);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->vbox),
			    label, TRUE, TRUE, 0);
	gtk_widget_show (label);
	button = gtk_button_new_with_label (_("OK"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (CloseDialog), dialog_window);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
			    button, TRUE, TRUE, 0);
	gtk_widget_grab_default (button);
	gtk_widget_show (button);
	gtk_widget_show (dialog_window);
	gtk_grab_add (dialog_window);
	yq_play_sound(4);
	gtk_main ();
}

void
YesFunc (GtkWidget * widget, gpointer data)
{
	yesnoreturn = 1;
	gtk_widget_destroy (GTK_WIDGET (data));
}


void
Areyousure (gchar * szMessage)
{
	GtkWidget *label;
	GtkWidget *button;
	GtkWidget *dialog_window;

	yq_play_sound(7);
	yesnoreturn = 0;
	dialog_window = gtk_dialog_new ();
	gtk_signal_connect (GTK_OBJECT (dialog_window), "destroy",
			    GTK_SIGNAL_FUNC (ClosingDialog), NULL);
	gtk_window_set_title (GTK_WINDOW (dialog_window), _("Yqpim"));
	gtk_container_border_width (GTK_CONTAINER (dialog_window), 5);
	label = gtk_label_new (szMessage);
	gtk_misc_set_padding (GTK_MISC (label), 10, 10);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->vbox),
			    label, TRUE, TRUE, 0);
	gtk_widget_show (label);
	button = gtk_button_new_with_label (_("OK"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (YesFunc), dialog_window);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
			    button, TRUE, TRUE, 0);
	gtk_widget_show (button);
	button = gtk_button_new_with_label (_("Cancel"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (CloseDialog), dialog_window);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
			    button, TRUE, TRUE, 0);
	gtk_widget_grab_default (button);
	gtk_widget_show (button);
	gtk_widget_show (dialog_window);
	gtk_grab_add (dialog_window);
	gtk_main ();
}

void
NoFunc (GtkWidget * widget, gpointer data)
{
	yesnoreturn = 0;
	gtk_widget_destroy (GTK_WIDGET (data));
}


void
YesNo (gchar * szMessage)
{
	GtkWidget *label;
	GtkWidget *button;
	GtkWidget *dialog_window;

	yq_play_sound(8);
	yesnoreturn = 2;
	dialog_window = gtk_dialog_new ();
	gtk_signal_connect (GTK_OBJECT (dialog_window), "destroy",
			    GTK_SIGNAL_FUNC (ClosingDialog), NULL);
	gtk_window_set_title (GTK_WINDOW (dialog_window), _("Yqpim"));
	gtk_container_border_width (GTK_CONTAINER (dialog_window), 5);
	label = gtk_label_new (szMessage);
	gtk_misc_set_padding (GTK_MISC (label), 10, 10);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->vbox),
			    label, TRUE, TRUE, 0);
	gtk_widget_show (label);
	button = gtk_button_new_with_label (_("YES"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (YesFunc), dialog_window);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
			    button, TRUE, TRUE, 0);
	gtk_widget_show (button);
	button = gtk_button_new_with_label (_("NO"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (NoFunc), dialog_window);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
			    button, TRUE, TRUE, 0);
	gtk_widget_show (button);
	button = gtk_button_new_with_label (_("Cancel"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (CloseDialog), dialog_window);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
			    button, TRUE, TRUE, 0);
	gtk_widget_grab_default (button);
	gtk_widget_show (button);
	gtk_widget_show (dialog_window);
	gtk_grab_add (dialog_window);
	gtk_main ();
}


void
InputNameYesFunc (GtkWidget * widget, gpointer data)
{
	yesnoreturn = 1;
	strcpy (sFilename,
		gtk_entry_get_text (GTK_ENTRY
				    (gtk_container_children
				     (GTK_CONTAINER
				      (GTK_DIALOG (data)->vbox))->next->
				     data)));
	gtk_widget_destroy (GTK_WIDGET (data));
}

void
InputName (gchar * szMessage, gchar * entryvalue)
{
	GtkWidget *label;
	GtkWidget *entry;
	GtkWidget *button;
	GtkWidget *dialog_window;

	yq_play_sound(7);
	yesnoreturn = 0;
	dialog_window = gtk_dialog_new ();
	gtk_signal_connect (GTK_OBJECT (dialog_window), "destroy",
			    GTK_SIGNAL_FUNC (ClosingDialog), NULL);
	gtk_window_set_title (GTK_WINDOW (dialog_window), _("Yqpim"));
	gtk_container_border_width (GTK_CONTAINER (dialog_window), 5);
	label = gtk_label_new (szMessage);
	gtk_misc_set_padding (GTK_MISC (label), 10, 10);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->vbox),
			    label, TRUE, TRUE, 0);
	gtk_widget_show (label);
	entry = gtk_entry_new ();
	gtk_signal_connect (GTK_OBJECT(entry), "activate",
                      GTK_SIGNAL_FUNC(InputNameYesFunc),
                      dialog_window);
	if (entryvalue)
	{
		gtk_entry_set_text (GTK_ENTRY (entry), entryvalue);
		gtk_entry_select_region(GTK_ENTRY(entry),0,-1);
	}
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->vbox),
			    entry, TRUE, TRUE, 5);
	gtk_widget_show (entry);
	button = gtk_button_new_with_label (_("OK"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (InputNameYesFunc),
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
	gtk_widget_grab_focus (entry);
	gtk_widget_show (dialog_window);
	gtk_grab_add (dialog_window);
	gtk_main ();
}


void
file_ok_sel (GtkWidget * widget, gpointer data)
{
	strcpy (sFilename,
		gtk_file_selection_get_filename (GTK_FILE_SELECTION (data)));
/*	if (sFilename[strlen (sFilename) - 1] == '/')
		sFilename[0] = '\0';*/
	gtk_widget_destroy (GTK_WIDGET (data));
}

void
GetFilename (char *sTitle)
{
	GtkWidget *filew;

	yq_play_sound(8);
	sFilename[0] = '\0';
	filew = gtk_file_selection_new (sTitle);
	/*gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (filew));*/
	gtk_signal_connect (GTK_OBJECT (filew), "destroy",
			    (GtkSignalFunc) ClosingDialog2, NULL);
	gtk_signal_connect (GTK_OBJECT
			    (GTK_FILE_SELECTION (filew)->ok_button),
			    "clicked", (GtkSignalFunc) file_ok_sel, filew);
	gtk_signal_connect (GTK_OBJECT
			    (GTK_FILE_SELECTION (filew)->cancel_button),
			    "clicked", (GtkSignalFunc) CloseDialog, filew);
	gtk_file_selection_set_filename (GTK_FILE_SELECTION (filew), "");
	gtk_widget_show (filew);
	/*gtk_grab_add (filew);*/

	gtk_main ();
}
