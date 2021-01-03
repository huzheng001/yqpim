#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "intl.h"

#include "configfile.h"
#include "stdio.h"
#include <gtk/gtk.h>
#include <string.h>
#include "yqtreeitem.h"


extern GtkWidget *nbitems1[14],*nbitems2[12],*nbitems3[9],*nbitems4[14],*nbitems5[8],*nbitems6[3];
extern gboolean titleshow[46];
extern gboolean yq_tree_removing;
extern gint clist_selectrow;
extern GtkWidget *tree_main,*treewin;
extern gint yq_tree_item_height;
extern YqTreeItem *root_leaf;
extern struct _YQList yqlist;
extern ConfigFile *rccfgfile;
extern gboolean nb_changed;

YqTreeItem *selectleaf;
gboolean yq_nowiszu;

GdkPixmap *yqpixmaps[MAX_IMAGE_TYPES];
GdkBitmap *yqmasks[MAX_IMAGE_TYPES];

GtkWidget *yq_createnulltree ();
GtkWidget *yq_readzu (YqTreeItem * upleaf);
void Popup (gchar * szMessage);
void yq_tree_item_set_pixmap (YqTreeItem * treeitem, ImageType ed);
void zu_expanded (GtkWidget * widget, gpointer data);
void zu_collapsed (GtkWidget * widget, gpointer data);
void leaf_setnb (gchar * ab);
gchar *leaf_formattext (gchar * bd);
void clist_openzu (YqTreeItem * yqitem);
void open_zu (YqTreeItem * yqitem);
void yq_set_list_type (ListType type);
GtkWidget *yq_tree_item_new (gchar * ab, ImageType ed);
void yqleaf_selectionchanged (GtkWidget * tree);
void open_rootitem();
void open_allitem();
GCompareFunc yq_list_get_sort_func();
void yq_play_sound(gint i);
YqTreeItem *yq_tree_search_next(YqTreeItem *nowleaf);
YqTreeItem *yq_tree_search_upnext(YqTreeItem *nowleaf);
void yq_tree_select_item (GtkTree *tree, gint item);
void yq_tree_select_child(GtkTree *tree, GtkWidget *tree_item);
void yq_tree_item_expand(YqTreeItem * item);
void yq_clist_add_items(GList *list);
void yq_clist_set_titles(gint sorttype,gint oldsorttype);
void yq_nb_changed(GtkWidget * widget, gpointer data);


void
yqleaf_selectionchanged (GtkWidget * tree)
{
	YqTreeItem *item;

	if (yq_tree_removing)
		return;
	if (!GTK_TREE_SELECTION (tree))
		return;
	item = YQ_TREE_ITEM (GTK_TREE_SELECTION (tree)->data);
	selectleaf = item;
	if (strcmp(item->nodename,"r")==0)
	{
		yq_nowiszu = TRUE;
		open_rootitem();
		return;
	}
	if (strcmp(item->nodename,"a")==0)
	{
		yq_nowiszu = TRUE;
		open_allitem(item);
		return;
	}

	if (strchr (item->nodename, '-'))
	{
		gchar zu[260], *ti, *ab;

		yq_nowiszu = FALSE;
		yq_set_list_type (LIST_NOTEBOOK);
		strcpy (zu, item->nodename);
		ti = strchr (zu, '-');
		*ti = '\0';
		ti++;
		yq_cfg_read_string (item->cfg, zu, ti, &ab);
		nb_changed = TRUE;
		leaf_setnb (ab);
		nb_changed = FALSE;
		g_free (ab);
	}
	else
	{
		yq_nowiszu = TRUE;
		open_zu (item);
	}
}

void
open_rootitem()
{
	yq_set_list_type (LIST_DETAILS);
	clist_openzu (root_leaf);
}
void
open_allitem(YqTreeItem *yqitem)
{
	YqTreeItem *newleaf;
	GList *list;

	yq_play_sound(3);
	yq_set_list_type (LIST_DETAILS);
	yq_clist_set_titles(yqitem->sorttype,yqlist.sorttype);
	list = NULL;
	yqlist.sorttype=yqitem->sorttype;
	newleaf=yq_tree_search_next(root_leaf);
	while (newleaf!=root_leaf)
	{
		if ((strchr (newleaf->nodename, '-'))&&(strstr(newleaf->nodename,"yq_0")!=newleaf->nodename))
			list = g_list_insert_sorted(list, newleaf, yq_list_get_sort_func());
		newleaf=yq_tree_search_next(newleaf);
	}
	yq_clist_add_items(list);
	g_list_free(list);
}

void
open_zu (YqTreeItem * yqitem)
{
	yq_set_list_type (LIST_DETAILS);
	clist_openzu (yqitem);
}

gchar * leaf_formattext (gchar * bd)
{
	gchar *ab, *bb;
	ab = bd;
	bb = bd;
	while (*bb != '\0')
	{
		if (*bb != '~')
		{
			if (ab != bb)
				*ab = *bb;
			ab++;
			bb++;
		}
		else
		{
			bb++;
			switch (*bb)
			{
			case '0':
				*ab = '\0';
				break;
			case '~':
				*ab = '~';
				break;
			case 'n':
				*ab = '\n';
				break;
			case 'd':
				*ab = ',';
				break;
			}
			ab++;
			bb++;
		}
	}
	*ab = '\0';
	return bd;
}

void
leaf_setnb (gchar * ab)
{
	gchar *bb;
	gint i;

	yq_play_sound(9);
	
	bb=leaf_formattext (strtok (ab, ","));

	gtk_entry_set_text (GTK_ENTRY (GTK_COMBO (nbitems5[0])->entry),
				    leaf_formattext (strtok (NULL, ",")));
	gtk_label_set_text(GTK_LABEL(nbitems2[0]),leaf_formattext (strtok (NULL, ",")));
	gtk_label_set_text(GTK_LABEL(nbitems2[1]),leaf_formattext (strtok (NULL, ",")));
	for (i=2;i<5;i++)
		gtk_entry_set_text (GTK_ENTRY (nbitems2[i]),
			    leaf_formattext (strtok (NULL, ",")));
	gtk_entry_set_text (GTK_ENTRY (GTK_COMBO (nbitems2[5])->entry),bb);
	yq_nb_changed(nbitems2[2],NULL);
	for (i=6;i<11;i++)
		gtk_entry_set_text (GTK_ENTRY (nbitems2[i]),
			    leaf_formattext (strtok (NULL, ",")));
	gtk_text_freeze (GTK_TEXT (nbitems2[11]));
	gtk_editable_delete_text (GTK_EDITABLE (nbitems2[11]), 0, -1);
	bb = leaf_formattext (strtok (NULL, ","));
	gtk_text_insert (GTK_TEXT (nbitems2[11]), NULL, NULL,
			 NULL, bb, strlen (bb));
	gtk_text_thaw (GTK_TEXT (nbitems2[11]));
	gtk_text_freeze (GTK_TEXT (nbitems3[0]));
	gtk_editable_delete_text (GTK_EDITABLE (nbitems3[0]), 0, -1);
	bb = leaf_formattext (strtok (NULL, ","));
	gtk_text_insert (GTK_TEXT (nbitems3[0]), NULL, NULL,
			 NULL, bb, strlen (bb));
	gtk_text_thaw (GTK_TEXT (nbitems3[0]));
	for (i=1;i<9;i++)
		gtk_entry_set_text (GTK_ENTRY (nbitems3[i]),
			    leaf_formattext (strtok (NULL, ",")));
	gtk_entry_set_text (GTK_ENTRY (nbitems4[0]),
			    leaf_formattext (strtok (NULL, ",")));
	gtk_entry_set_text (GTK_ENTRY (nbitems4[1]),
			    leaf_formattext (strtok (NULL, ",")));
	gtk_text_freeze (GTK_TEXT (nbitems4[2]));
	gtk_editable_delete_text (GTK_EDITABLE (nbitems4[2]), 0, -1);
	bb = leaf_formattext (strtok (NULL, ","));
	gtk_text_insert (GTK_TEXT (nbitems4[2]), NULL, NULL,
			 NULL, bb, strlen (bb));
	gtk_text_thaw (GTK_TEXT (nbitems4[2]));
	for (i=3;i<14;i++)
		gtk_entry_set_text (GTK_ENTRY (nbitems4[i]),
			    leaf_formattext (strtok (NULL, ",")));
	gtk_entry_set_text (GTK_ENTRY (nbitems5[1]),
			    leaf_formattext (strtok (NULL, ",")));
	gtk_entry_set_text (GTK_ENTRY (nbitems5[2]),
			    leaf_formattext (strtok (NULL, ",")));
	gtk_text_freeze (GTK_TEXT (nbitems5[3]));
	gtk_editable_delete_text (GTK_EDITABLE (nbitems5[3]), 0, -1);
	bb = leaf_formattext (strtok (NULL, ","));
	gtk_text_insert (GTK_TEXT (nbitems5[3]), NULL, NULL,
			 NULL, bb, strlen (bb));
	gtk_text_thaw (GTK_TEXT (nbitems5[3]));
	for (i=4;i<7;i++)
		gtk_entry_set_text (GTK_ENTRY (nbitems5[i]),
			    leaf_formattext (strtok (NULL, ",")));
	gtk_text_freeze (GTK_TEXT (nbitems5[7]));
	gtk_editable_delete_text (GTK_EDITABLE (nbitems5[7]), 0, -1);
	bb = leaf_formattext (strtok (NULL, ","));
	gtk_text_insert (GTK_TEXT (nbitems5[7]), NULL, NULL,
			 NULL, bb, strlen (bb));
	gtk_text_thaw (GTK_TEXT (nbitems5[7]));
	gtk_entry_set_text (GTK_ENTRY (nbitems6[0]),
			    leaf_formattext (strtok (NULL, ",")));
	gtk_text_freeze (GTK_TEXT (nbitems6[1]));
	gtk_editable_delete_text (GTK_EDITABLE (nbitems6[1]), 0, -1);
	bb = leaf_formattext (strtok (NULL, ","));
	gtk_text_insert (GTK_TEXT (nbitems6[1]), NULL, NULL,
			 NULL, bb, strlen (bb));
	gtk_text_thaw (GTK_TEXT (nbitems6[1]));
	gtk_text_freeze (GTK_TEXT (nbitems6[2]));
	gtk_editable_delete_text (GTK_EDITABLE (nbitems6[2]), 0, -1);
	bb = leaf_formattext (strtok (NULL, ","));
	gtk_text_insert (GTK_TEXT (nbitems6[2]), NULL, NULL,
			 NULL, bb, strlen (bb));
	gtk_text_thaw (GTK_TEXT (nbitems6[2]));
	gtk_widget_grab_focus (yqlist.list);
}

void
zu_expanded (GtkWidget * widget, gpointer data)
{
	YqTreeItem *leaf;
	
	leaf=YQ_TREE_ITEM (widget);
	if (strcmp(leaf->nodename,"yq")==0)
	{
		yq_tree_item_set_pixmap (YQ_TREE_ITEM (widget), YQFILE_OPEN);
		return;
	}
	if (strcmp(leaf->nodename,"yq_0")==0)
		yq_tree_item_set_pixmap (leaf, TRASH);
	else
		yq_tree_item_set_pixmap (leaf, ZU_OPEN);
}

void
zu_collapsed (GtkWidget * widget, gpointer data)
{
	YqTreeItem *leaf;

	leaf = YQ_TREE_ITEM (widget);
	if (strcmp(leaf->nodename,"yq")==0)
		yq_tree_item_set_pixmap (leaf, YQFILE_CLOSE);
	else
		if (strcmp(leaf->nodename,"yq_0")!=0)
			yq_tree_item_set_pixmap (leaf, ZU_CLOSE);
	
	if (leaf==selectleaf) return;
	if (selectleaf->cfg != leaf->cfg)
		return;
	if (strstr(selectleaf->nodename,leaf->nodename)==selectleaf->nodename)
		yq_tree_select_child (leaf->attree, widget);
}


GtkWidget *
yq_readzu (YqTreeItem * upleaf)
{
	GtkWidget *tree;
	YqTreeItem *leaf;
	gchar *ab, *bb, ti[4], tz[260];
	gint i, z;

	tree = gtk_tree_new ();
	gtk_widget_show (tree);
	yq_cfg_read_int (upleaf->cfg, upleaf->nodename, "z", &z);	//z表示本组有多少子组

	for (i = 1; i <= z; i++)
	{
		sprintf (tz, "%s_%d", upleaf->nodename, i);
		yq_cfg_read_string (upleaf->cfg, tz, "n", &ab);
		leaf = YQ_TREE_ITEM (yq_tree_item_new (ab, ZU_CLOSE));
		g_free (ab);
		leaf->attree = GTK_TREE (tree);
		leaf->cfg = upleaf->cfg;
		strcpy (leaf->nodename, tz);

		gtk_signal_connect (GTK_OBJECT (leaf), "expand",
				    GTK_SIGNAL_FUNC (zu_expanded), NULL);
		gtk_signal_connect (GTK_OBJECT (leaf), "collapse",
				    GTK_SIGNAL_FUNC (zu_collapsed), NULL);

		gtk_tree_append (GTK_TREE (tree), GTK_WIDGET (leaf));
		gtk_tree_item_set_subtree (GTK_TREE_ITEM (leaf),
					   yq_readzu (leaf));
	}

	yq_cfg_read_int (upleaf->cfg, upleaf->nodename, "a", &z);	//a表示本组有多少记录
	for (i = 1; i <= z; i++)
	{
		sprintf (ti, "%d", i);
		sprintf (tz, "%s-%s", upleaf->nodename, ti);
		yq_cfg_read_string (upleaf->cfg, upleaf->nodename, ti, &ab);

		bb = strchr (ab, ',');
		*bb = '\0';
		if (strstr((bb + 1), _("Female")) == (bb+1))
			leaf = YQ_TREE_ITEM (yq_tree_item_new
					     (leaf_formattext (ab), GIRL));
		else
			leaf = YQ_TREE_ITEM (yq_tree_item_new
					     (leaf_formattext (ab), BOY));

		leaf->attree = GTK_TREE (tree);
		leaf->cfg = upleaf->cfg;
		strcpy (leaf->nodename, tz);

		gtk_tree_append (GTK_TREE (tree), GTK_WIDGET (leaf));
		g_free (ab);
	}
	return tree;
}

void
yq_readfile (GtkWidget * widget, gpointer open_file)
{
	YqTreeItem *leaf;
	gchar *ab, *bb, ti[4];
	ConfigFile *cfgfile;
	gint i, a=0, j;

	cfgfile = yq_cfg_open_file (open_file);
	if (!cfgfile)
	{
		Popup (_("Error while reading file!"));
		return;
	}

	if (!yq_cfg_read_string (cfgfile, "yq", "n", &ab))
	{
		Popup (_("This is not a yqpim file"));
		yq_cfg_free (cfgfile);
		g_free (cfgfile);
		return;
	}
	yq_cfg_read_int (cfgfile, "yq", "v", &a);
	if (a!=2)
	{
		Popup (_("Can't open this version's yqpim file"));
		yq_cfg_free (cfgfile);
		g_free (cfgfile);
		return;
	}		
	yq_cfg_read_string (rccfgfile, "file", "1", &bb);
	if (strcmp(open_file,bb)!=0)
	{
		g_free(bb);
		yq_cfg_read_int (rccfgfile, "file", "a", &a);
		for (i = 1; i <= a; i++)	//删除重复文件,每次都扫,记录中不可能有两个相同的,只需扫一遍
		{
			sprintf (ti, "%d", i);
			yq_cfg_read_string (rccfgfile, "file", ti, &bb);
			if (!strcmp (bb, open_file))
			{
				g_free (bb);
				for (j = i; j < a; j++)
				{
					sprintf (ti, "%d", j + 1);
					yq_cfg_read_string (rccfgfile, "file", ti,
							    &bb);
					sprintf (ti, "%d", j);
					yq_cfg_write_string (rccfgfile, "file", ti,
							     bb);
					g_free (bb);
				}
				sprintf (ti, "%d", a);
				yq_cfg_remove_key (rccfgfile, "file", ti);
				a--;
				break;
			}
			g_free (bb);
		}
		for (i = a; i > 0; i--)	//只记录5个最近打开文件
		{
			if (i != 5)
			{
				sprintf (ti, "%d", i);
				yq_cfg_read_string (rccfgfile, "file", ti, &bb);
				sprintf (ti, "%d", i + 1);
				yq_cfg_write_string (rccfgfile, "file", ti, bb);
				g_free (bb);
			}
		}
		yq_cfg_write_string (rccfgfile, "file", "1", open_file);
		if (a != 5)
			a++;
		yq_cfg_write_int (rccfgfile, "file", "a", a);
	}
	else
		g_free(bb);
	leaf = YQ_TREE_ITEM (yq_tree_item_new (ab, YQFILE_CLOSE));
	if (!GTK_TREE_ITEM (root_leaf)->subtree)
	{
		tree_main = yq_createnulltree ();
		gtk_tree_item_set_subtree (GTK_TREE_ITEM (root_leaf),
					   tree_main);
	}
	leaf->attree = GTK_TREE (tree_main);
	leaf->cfg = cfgfile;

	strcpy (leaf->nodename, "yq");

	gtk_tree_prepend (GTK_TREE (tree_main), GTK_WIDGET (leaf));

	gtk_signal_connect (GTK_OBJECT (leaf), "expand",
			    GTK_SIGNAL_FUNC (zu_expanded), NULL);
	gtk_signal_connect (GTK_OBJECT (leaf), "collapse",
			    GTK_SIGNAL_FUNC (zu_collapsed), NULL);
	gtk_tree_item_set_subtree (GTK_TREE_ITEM (leaf), yq_readzu (leaf));

	g_free (ab);
	gtk_tree_item_expand (GTK_TREE_ITEM (leaf));
	if (!GTK_TREE_ITEM (root_leaf)->expanded)
		gtk_tree_item_expand (GTK_TREE_ITEM (root_leaf));
	yq_tree_select_item (GTK_TREE (tree_main), 0);
	cfgfile = NULL;
}

void
yq_tree_name_search(GtkWidget * widget, gpointer data)
{
	gchar *aa,*ab,*a,ti[4];
	YqTreeItem *oldleaf,*newleaf;
	gboolean found;
	
	aa=gtk_entry_get_text (GTK_ENTRY (data));
	if (aa[0]=='\0')
	{
		yq_tree_select_item (GTK_TREE (tree_main)->root_tree, 0);
		return;
	}
	if (strcmp("a",selectleaf->nodename)==0)
		oldleaf=root_leaf;
	else
		oldleaf=selectleaf;
	found=FALSE;
	newleaf=yq_tree_search_next(oldleaf);
	while ((newleaf!=oldleaf)&&(!found))
	{
		if (strcmp(newleaf->nodename,"r")==0)
			ab=strdup(_("Contacts"));
		else
		{
			a=strchr(newleaf->nodename,'-');
			if (a)
			{
				*a='\0';
				a++;
				strcpy(ti,a);
				yq_cfg_read_string (newleaf->cfg, newleaf->nodename, ti, &ab);
				a--;
				*a='-';
				a=strchr(ab,',');
				*a='\0';
				leaf_formattext(ab);
			}
			else
			{
				yq_cfg_read_string (newleaf->cfg, newleaf->nodename, "n", &ab);
			}
		}
		if (strncasecmp(ab,aa,strlen(aa))==0)
		{
			found=TRUE;
			if (newleaf!=root_leaf)
				yq_tree_item_expand(YQ_TREE_ITEM(newleaf->attree->tree_owner));				
			yq_tree_select_child(newleaf->attree,GTK_WIDGET(newleaf));
			gtk_widget_grab_focus (GTK_WIDGET(data));
		}
		else
		{
			newleaf=yq_tree_search_next(newleaf);
		}
		g_free(ab);
	}
}

gchar
*yq_tree_get_ruby(YqTreeItem *item)
{
	gchar *a,ti[5],*ab,*a1,*a2;

	a=strchr(item->nodename,'-');
	if (a==NULL)
		return NULL;
	*a='\0';
	a++;
	strcpy(ti,a);
	yq_cfg_read_string (item->cfg, item->nodename, ti, &ab);
	a--;
	*a='-';
	ab=leaf_formattext(strtok (ab, ","));
	strtok (NULL, ",");
	a1=leaf_formattext(strtok (NULL, ","));
	a2=leaf_formattext(strtok (NULL, ","));
	if ((*a1=='\0')&&(*a2=='\0'))
	{
		*a1=*ab;
		a=strchr(ab,' ');
		if (a!=NULL)
		{
			*a2=*(a+1);
		}
	}
	*(a1+1)='\0';
	*(a2+1)='\0';
	a=g_malloc(3);
	sprintf(a,"%s%s",a1,a2);
	g_free(ab);
	return a;
}

void
yq_tree_ruby_search(GtkWidget * widget, gpointer data)
{
	gchar *aa,*ab;
	YqTreeItem *oldleaf,*newleaf;
	gboolean found;
	
	aa=gtk_entry_get_text (GTK_ENTRY (data));
	if (aa[0]=='\0')
	{
		return;
	}
	if (strcmp("a",selectleaf->nodename)==0)
		oldleaf=root_leaf;
	else
		oldleaf=selectleaf;
	found=FALSE;
	newleaf=yq_tree_search_next(oldleaf);
	while ((newleaf!=oldleaf)&&(!found))
	{
		ab=yq_tree_get_ruby(newleaf);
		if ((ab)&&(strncasecmp(ab,aa,strlen(aa))==0))
		{
			found=TRUE;
			if (newleaf!=root_leaf)
				yq_tree_item_expand(YQ_TREE_ITEM(newleaf->attree->tree_owner));				
			yq_tree_select_child(newleaf->attree,GTK_WIDGET(newleaf));
			gtk_widget_grab_focus (GTK_WIDGET(data));
		}
		else
		{
			newleaf=yq_tree_search_next(newleaf);
		}
		if (ab)
			g_free(ab);
	}
}

void
yq_tree_item_expand(YqTreeItem * item)
{
	if (item!=root_leaf)
		yq_tree_item_expand(YQ_TREE_ITEM(item->attree->tree_owner));
	if (!GTK_TREE_ITEM(item)->expanded)
		gtk_tree_item_expand(GTK_TREE_ITEM(item));
}

gint
yq_tree_item_position(GtkTree *tree, GtkWidget *treeitem, gint level)
{
	GList *children;
	gint pos;
	static gboolean found;
	
	children = gtk_container_children (GTK_CONTAINER (tree));
	pos = 0;
	found = FALSE;
	while (children)
	{
		if (children->data == treeitem)
		{
			found = TRUE;
			return pos;
		}
		pos ++;
		if (GTK_TREE_ITEM(children->data)->expanded)
		{
			pos += yq_tree_item_position(
				GTK_TREE(
				GTK_TREE_ITEM_SUBTREE(
				GTK_TREE_ITEM(children->data))),
				treeitem, level+1);
			if (found) return pos;
		}
		children = g_list_remove_link (children, children);
	}
	if (level == 0 && !found) return -1;
	return pos;
}

void
yq_tree_select_item (GtkTree   *tree,
		      gint       item)
{
  GList *tmp_list;
  
  g_return_if_fail (tree != NULL);
  g_return_if_fail (GTK_IS_TREE (tree));
  
  tmp_list = g_list_nth (tree->children, item);
  if (tmp_list)
    yq_tree_select_child (tree, GTK_WIDGET (tmp_list->data)); 
}

void
yq_tree_select_child(GtkTree *tree, GtkWidget *tree_item)
{
	GtkAdjustment *vadj;
	gint pos,i;
	
	pos=yq_tree_item_position(GTK_TREE_ROOT_TREE(tree_main), tree_item, 0);
	if (pos >= 0)
	{
		vadj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(treewin));
		i=(yq_tree_item_height * pos);
		if ((i<vadj->value)||(i>(vadj->value+vadj->page_size)))
		{
			if ((vadj->upper - vadj->page_size)<i)
				i=vadj->upper - vadj->page_size;
			gtk_adjustment_set_value(vadj, i);
		}
	}
	gtk_tree_select_child(tree,tree_item);
}

YqTreeItem *
yq_tree_search_next(YqTreeItem *nowleaf)
{
	gint i;

	if ((GTK_TREE_ITEM(nowleaf)->subtree)&&(GTK_TREE(GTK_TREE_ITEM(nowleaf)->subtree)->children))
	{
		return YQ_TREE_ITEM(GTK_TREE(GTK_TREE_ITEM(nowleaf)->subtree)->children->data);
	}
	i=gtk_tree_child_position( nowleaf->attree,GTK_WIDGET(nowleaf));
	if (i!=g_list_length(nowleaf->attree->children)-1)
	{
		return YQ_TREE_ITEM(g_list_nth(nowleaf->attree->children,i+1)->data);
	}
	return yq_tree_search_upnext(nowleaf);
}

YqTreeItem *
yq_tree_search_upnext(YqTreeItem *nowleaf)
{
	gint i;

	if (nowleaf==root_leaf)
	{
		return root_leaf;
	}
	if (YQ_TREE_ITEM(nowleaf->attree->tree_owner)==root_leaf)
		return root_leaf;
	
	i=gtk_tree_child_position(YQ_TREE_ITEM(nowleaf->attree->tree_owner)->attree,GTK_WIDGET(nowleaf->attree->tree_owner));
	if (i!=g_list_length(YQ_TREE_ITEM(nowleaf->attree->tree_owner)->attree->children)-1)
	{
		return (YqTreeItem *)g_list_nth(YQ_TREE_ITEM(nowleaf->attree->tree_owner)->attree->children,i+1)->data;
	}
	else
	{
		return yq_tree_search_upnext(YQ_TREE_ITEM(nowleaf->attree->tree_owner)); 
	}
}
