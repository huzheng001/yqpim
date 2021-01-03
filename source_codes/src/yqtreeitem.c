#include <gtk/gtk.h>

#include "yqtreeitem.h"


#include "pixmaps/boy.xpm"
#include "pixmaps/girl.xpm"
#include "pixmaps/zu_open.xpm"
#include "pixmaps/zu_close.xpm"
#include "pixmaps/yqfile.xpm"
#include "pixmaps/yqfile_open.xpm"
#include "pixmaps/yqfile_close.xpm"
#include "pixmaps/laji.xpm"

#include "pixmaps/yqpim.xpm"
#include "pixmaps/name.xpm"
#include "pixmaps/home.xpm"
#include "pixmaps/business.xpm"
#include "pixmaps/personal.xpm"
#include "pixmaps/notes.xpm"

extern gboolean yq_nowiszu;


GdkPixmap *yqpixmaps[MAX_IMAGE_TYPES];
GdkBitmap *yqmasks[MAX_IMAGE_TYPES];
gboolean yq_tree_removing=FALSE;


static void yq_tree_item_init (YqTreeItem * yti);
static void yq_tree_item_class_init (YqTreeItemClass * class);
GtkWidget *yq_tree_item_new (gchar * ab, ImageType ed);
void yq_tree_item_doubleselect(GtkWidget * widget, GdkEventButton * event,gpointer func_data);


void
yq_load_pixmaps (GtkWidget * parent)
{
	static gboolean loaded = FALSE;
	static GtkStyle *style;

	if (!loaded)
	{
		loaded = TRUE;
		style = gtk_widget_get_style (parent);
		yqpixmaps[BOY] = gdk_pixmap_create_from_xpm_d (parent->window,
							       &yqmasks[BOY],
							       &style->
							       bg
							       [GTK_STATE_NORMAL],
							       (gchar **)
							       boy_xpm);
		yqpixmaps[GIRL] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[GIRL],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **) girl_xpm);
		yqpixmaps[ZU_OPEN] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[ZU_OPEN],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **) zu_open_xpm);
		yqpixmaps[ZU_CLOSE] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[ZU_CLOSE],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **)
						      zu_close_xpm);
		yqpixmaps[YQFILE_OPEN] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[YQFILE_OPEN],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **)
						      yqfile_open_xpm);
		yqpixmaps[YQFILE] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[YQFILE],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **) yqfile_xpm);
		yqpixmaps[YQPIM] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[YQPIM],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **)
						      yqpim_xpm);
		yqpixmaps[YQFILE_CLOSE] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[YQFILE_CLOSE],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **)
						      yqfile_close_xpm);
		yqpixmaps[TRASH] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[TRASH],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **)
						      laji_xpm);
		yqpixmaps[NAME] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[NAME],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **)
						      name_xpm);
		yqpixmaps[HOME] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[HOME],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **)
						      home_xpm);
		yqpixmaps[BUSINESS] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[BUSINESS],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **)
						      business_xpm);
		yqpixmaps[PERSONAL] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[PERSONAL],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **)
						      personal_xpm);
		yqpixmaps[NOTES] =
			gdk_pixmap_create_from_xpm_d (parent->window,
						      &yqmasks[NOTES],
						      &style->
						      bg[GTK_STATE_NORMAL],
						      (gchar **)
						      notes_xpm);
	}
}

void
yq_tree_item_set_pixmap (YqTreeItem * yti, ImageType ed)
{
	GtkPixmap *hbox, *pixmap;

	hbox = gtk_container_children (GTK_CONTAINER (yti))->data;
	pixmap = gtk_container_children (GTK_CONTAINER (hbox))->data;
	gtk_pixmap_set (GTK_PIXMAP (pixmap), yqpixmaps[ed], yqmasks[ed]);
}

void
yq_tree_item_set_text (YqTreeItem * yti, gchar * ab)
{
	GtkWidget *hbox, *label;

	hbox = gtk_container_children (GTK_CONTAINER (yti))->data;
	label = gtk_container_children (GTK_CONTAINER (hbox))->next->data;
	gtk_label_set_text (GTK_LABEL (label), ab);
}

void
yq_tree_item_doubleselect(GtkWidget * widget, GdkEventButton * event,
		       gpointer func_data)
{
	if (event->type == GDK_2BUTTON_PRESS)
	{
		if (yq_nowiszu)
		{
			if (GTK_TREE_ITEM(widget)->expanded)
				gtk_tree_item_collapse(GTK_TREE_ITEM(widget));
			else
				gtk_tree_item_expand(GTK_TREE_ITEM(widget));
		}
	}
}


guint
yq_tree_item_get_type ()
{
	static guint yti_type = 0;

	if (!yti_type)
	{
		GtkTypeInfo yti_info = {
			"YqTreeItem",
			sizeof (YqTreeItem),
			sizeof (YqTreeItemClass),
			(GtkClassInitFunc) yq_tree_item_class_init,
			(GtkObjectInitFunc) yq_tree_item_init,
			(GtkArgSetFunc) NULL,
			(GtkArgGetFunc) NULL
		};
		yti_type =
			gtk_type_unique (gtk_tree_item_get_type (),
					 &yti_info);
	}
	return yti_type;
}

static void
yq_tree_item_class_init (YqTreeItemClass * class)
{
}

static void
yq_tree_item_init (YqTreeItem * yti)
{
	yti->attree = NULL;
	yti->nodename[0] = '\0';
	yti->cfg = NULL;
	yti->sorttype=-1;
	gtk_signal_connect (GTK_OBJECT (yti),
			    "button_press_event",
			    GTK_SIGNAL_FUNC (yq_tree_item_doubleselect), NULL);
}

GtkWidget *
yq_tree_item_new (gchar * ab, ImageType ed)
{
	YqTreeItem *yti;
	GtkWidget *hbox, *pixmap_widget, *label;

	yti = YQ_TREE_ITEM (gtk_type_new (yq_tree_item_get_type ()));
	hbox = gtk_hbox_new (FALSE, 1);
	gtk_widget_show (hbox);
	pixmap_widget = gtk_pixmap_new (yqpixmaps[ed], yqmasks[ed]);
	gtk_widget_show (pixmap_widget);
	gtk_box_pack_start (GTK_BOX (hbox), pixmap_widget, FALSE, FALSE, 0);
	label = gtk_label_new (ab);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	gtk_widget_show (label);
	gtk_container_add (GTK_CONTAINER (yti), hbox);
	gtk_widget_show (GTK_WIDGET (yti));
	return GTK_WIDGET (yti);
}

void
yq_tree_remove_item (GtkWidget * tree,GtkWidget * item)
{
	GtkWidget * owner;

	yq_tree_removing=TRUE;  //when removing treeitem,the tree's "selection_changed" even will call for two time,i use this to disable the not real selection changed.
	gtk_widget_ref (tree);
	owner = GTK_TREE(tree)->tree_owner;
	gtk_container_remove (GTK_CONTAINER(tree), item);
	if (tree->parent == NULL)
	{
  		gtk_tree_item_expand (GTK_TREE_ITEM(owner));
  		gtk_tree_item_set_subtree (GTK_TREE_ITEM(owner), tree);
	}
	else
	gtk_widget_unref (tree);
	yq_tree_removing=FALSE;
}
