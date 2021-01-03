#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "intl.h"

#include <gtk/gtk.h>
#include <string.h>
#include "configfile.h"
#include "yqtreeitem.h"


extern ConfigFile *rccfgfile;
extern YqTreeItem *selectleaf;

GtkWidget *tree_main, *treewin;
YqTreeItem *root_leaf;
gint yq_tree_item_height=1;

GtkWidget *yq_tree_item_new (gchar * ab, ImageType ed);
void rootleaf_collapse ();
void yqleaf_selectionchanged (GtkWidget * tree);
static void treewin_size_changed (GtkWidget * box);
void yq_set_list_type (ListType type);
void yq_tree_select_child(GtkTree *tree, GtkWidget *tree_item);
void yq_play_sound(gint i);

static void
treewin_size_changed (GtkWidget * box)
{
	gint m;
	
	yq_cfg_read_int (rccfgfile, "setting", "paned_position",&m);
	if (m!=box->allocation.width)
		yq_cfg_write_int (rccfgfile, "setting", "paned_position",
			  box->allocation.width);
}

void
rootleaf_collapse (GtkWidget * widget, gpointer data)
{
	YqTreeItem *leaf;

	leaf = YQ_TREE_ITEM (widget);
	if (strstr(selectleaf->nodename,"yq")==selectleaf->nodename)
		yq_tree_select_child (leaf->attree, widget);
}

void
createtree (GtkWidget * parent, GtkWidget * vbox)
{
	GtkWidget *tree;
	GtkRequisition requisition;

	tree = gtk_tree_new ();
	gtk_tree_set_view_mode (GTK_TREE (tree), GTK_TREE_VIEW_ITEM);
	gtk_tree_set_selection_mode (GTK_TREE (tree), GTK_SELECTION_BROWSE);
	gtk_signal_connect (GTK_OBJECT (tree), "selection_changed",
			    GTK_SIGNAL_FUNC (yqleaf_selectionchanged), NULL);
			    
	root_leaf = YQ_TREE_ITEM(yq_tree_item_new (_("All Contacts"), YQFILE));
	gtk_tree_append (GTK_TREE (tree), GTK_WIDGET(root_leaf));
	root_leaf->attree =GTK_TREE (tree);
	strcpy(root_leaf->nodename,"a");
	root_leaf = YQ_TREE_ITEM(yq_tree_item_new (_("Contacts"), YQFILE));
	gtk_signal_connect (GTK_OBJECT (root_leaf), "collapse",
			    GTK_SIGNAL_FUNC (rootleaf_collapse), NULL);
	gtk_tree_append (GTK_TREE (tree), GTK_WIDGET(root_leaf));
	root_leaf->attree =GTK_TREE (tree);
	strcpy(root_leaf->nodename,"r");
	gtk_widget_show (tree);
	tree_main = gtk_tree_new ();
	gtk_tree_item_set_subtree (GTK_TREE_ITEM (root_leaf), tree_main);
	gtk_widget_show (tree_main);	
	gtk_tree_item_expand (GTK_TREE_ITEM (root_leaf));
	gtk_widget_size_request(GTK_WIDGET(root_leaf), &requisition);
	yq_tree_item_height = requisition.height;
	treewin = gtk_scrolled_window_new (NULL, NULL);
	gtk_signal_connect_object (GTK_OBJECT (treewin),
				   "size_allocate",
				   GTK_SIGNAL_FUNC (treewin_size_changed),
				   GTK_OBJECT (treewin));
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (treewin),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (treewin),
					       tree);
	gtk_box_pack_start (GTK_BOX (vbox), treewin, TRUE, TRUE, 0);
	gtk_widget_show (treewin);
}
