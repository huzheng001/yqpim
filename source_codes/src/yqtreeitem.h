#ifndef __YQ_TREE_ITEM_H__
#define __YQ_TREE_ITEM_H__

#ifdef __cplusplus
extern "C"
{
#endif				/* __cplusplus */

#include <gtk/gtk.h>
#include "configfile.h"
#include "yqtype.h"


#define YQ_TREE_ITEM(obj)		GTK_CHECK_CAST (obj, yq_tree_item_get_type (), YqTreeItem)
#define YQ_TREE_CLASS(klass)	GTK_CHECK_CLASS_CAST (klass, yq_tree_item_get_type (), YqTreeItemClass)
#define IS_YQ_TREE_ITEM(obj)		GTK_CHECK_TYPE (obj, yq_tree_item_get_type ())

	typedef struct _YqTreeItem YqTreeItem;
	typedef struct _YqTreeItemClass YqTreeItemClass;

	struct _YqTreeItemClass
	{
		GtkTreeItemClass parent_class;
	};

	struct _YqTreeItem
	{
		GtkTreeItem tree_item;
		GtkTree *attree;
		gchar nodename[260];
		ConfigFile *cfg;
		gint sorttype;
	};

	guint yq_tree_item_get_type (void);
	GtkWidget *yq_tree_item_new (gchar * ab, ImageType ed);
	void yq_tree_remove_item (GtkWidget * tree, GtkWidget * item);

#ifdef __cplusplus
}
#endif				/* __cplusplus */

#endif
