#include <gtk/gtk.h>
#include "yqtype.h"


struct _YQList yqlist;

void createclist ();
void createnotebook ();

void
createlist (GtkWidget * vbox)
{
	yqlist.box = gtk_hbox_new (FALSE, 0);
	createclist ();
	gtk_widget_show (yqlist.box);
	gtk_box_pack_start (GTK_BOX (vbox), yqlist.box, TRUE, TRUE, 0);
}

void
yq_set_list_type (ListType type)
{
	if (type == yqlist.type)
	{
		if (yqlist.type == LIST_DETAILS)
			gtk_clist_clear (GTK_CLIST (yqlist.list));
		return;
	}
	switch (yqlist.type)
	{
	case LIST_DETAILS:
		gtk_clist_clear (GTK_CLIST (yqlist.list));
		gtk_container_remove (GTK_CONTAINER (yqlist.box),
				      yqlist.listwin);
		break;
	case LIST_NOTEBOOK:
		gtk_container_remove (GTK_CONTAINER (yqlist.box),
				      yqlist.list);
		break;
	}

	switch (type)
	{
	case LIST_DETAILS:
		createclist ();
		break;
	case LIST_NOTEBOOK:
		createnotebook ();
		break;
	}
}
