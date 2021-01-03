#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "intl.h"

#include "configfile.h"
#include <gtk/gtk.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "yqtreeitem.h"


extern gchar sFilename[256];
extern gint yesnoreturn;
extern gint clist_selectrow;
extern GdkPixmap *yqpixmaps[MAX_IMAGE_TYPES];
extern GdkBitmap *yqmasks[MAX_IMAGE_TYPES];
extern YqTreeItem *selectleaf, *root_leaf;
extern GtkWidget *tree_main;
extern gboolean yq_nowiszu;
extern struct _YQList yqlist;
extern GtkWidget *new_button, *open_button,*cut_button, *copy_button,
	*paste_button, *search_button, *add_button, *remove_button,
	*save_button, *saveadd_button, *cancel_button, 
	*zu_button, *rename_button, *mima_button, *laji_button, 
	*question_button, *up_button;

extern GtkWidget *treewin;
extern GtkWidget *nbitems1[14],*nbitems2[12],*nbitems3[9],*nbitems4[14],*nbitems5[8],*nbitems6[3];
extern GtkWidget *menu_file_new, *menu_file_open, *menu_file_openold,
	*menu_file_close, *menu_file_saveas, *menu_file_import,*menu_file_export,*menu_file_exit, *menu_edit_cut,
	*menu_edit_copy, *menu_edit_paste,*menu_edit_selectall,*menu_edit_selectelse,*menu_edit_find, *menu_jilu_add, *menu_jilu_remove,
	*menu_jilu_save, *menu_jilu_saveadd, *menu_jilu_cancel, *menu_jilu_zu,
	*menu_jilu_rename, *menu_tool_option,*menu_tool_trash;



gboolean nb_changed, nb_isadd;
static gchar *search_text=NULL;
static GtkWidget *search_window=NULL;
ConfigFile *tmpcfg;

void addtotext (gchar ** a, gchar * b);
void nb_setchanged (GtkWidget * widget, gpointer data);
void leaf_setnb (gchar * ab);
void Popup (gchar * szMessage);
void Areyousure (gchar * szMessage);
void YesNo (gchar * szMessage);
void yq_set_list_type (ListType type);
void open_allitem(YqTreeItem *yqitem);
void yq_tree_item_set_pixmap (YqTreeItem * treeitem, ImageType ed);
void yq_tree_item_set_text (YqTreeItem * treeitem, gchar * ab);
GtkWidget *yq_tree_item_new (gchar * ab, ImageType ed);
gchar *leaf_formattext (gchar * bd);
void yq_import_csv_format(gchar *ab,gchar *tmp);
void toolbar_save (GtkWidget * widget, gpointer data);
void toolbar_add (GtkWidget * widget, gpointer data);
void zu_expanded (GtkWidget * widget, gpointer data);
void zu_collapsed (GtkWidget * widget, gpointer data);
void yq_cfg_remove_sections (ConfigFile * cfg, gchar * section);
void yq_cfg_rename_sections (ConfigFile * cfg, gchar * section,
			     gchar * section_name);
void yq_cfg_daiti_sections (ConfigFile * cfg, gchar * section1,
			    gchar * section2);
GtkWidget *yq_readzu (YqTreeItem * upleaf);
GtkWidget *yq_createnulltree ();
void yq_readfile (GtkWidget * widget, gpointer openfile);
void GetFilename (char *sTitle);
void InputName (gchar * szMessage, gchar * entryvalue);
void create_yqfile (gchar * filename);
void open_zu (YqTreeItem * yqitem);
void toolbar_copy (GtkWidget * widget, gpointer data);
void toolbar_clean_trash(YqTreeItem *trashitem,gboolean ask);
void toolbar_remove (GtkWidget * widget, gpointer data);
void toolbar_remove_zu(YqTreeItem *rmitem);
void toolbar_change_nodenames(YqTreeItem *cnitem,gchar *name);
void toolbar_remove_leaf(YqTreeItem *rmitem);
void toolbar_addjilu (YqTreeItem *nowleaf,gchar * ab,gboolean freeze,gboolean expand);
void toolbar_addzu (YqTreeItem *nowleaf,gboolean freeze,gboolean expand);
YqTreeItem *yq_tree_search_next(YqTreeItem *nowleaf);
void yq_tree_select_item (GtkTree *tree, gint item);
void yq_tree_select_child(GtkTree *tree, GtkWidget *tree_item);
void yq_tree_item_expand(YqTreeItem * item);
void toolbar_search_yesfunc (GtkWidget * widget, gpointer data);
void toolbar_search_nofunc (GtkWidget * widget, gpointer data);
void yq_config_copy_sections (ConfigFile *cfg1,gchar * zuname1,ConfigFile *cfg2, gchar * zuname2);
void yq_play_sound(gint i);
void yq_nb_changed(GtkWidget * widget, gpointer data);




GtkWidget * yq_createnulltree ()
{
	GtkWidget *tree;

	tree = gtk_tree_new ();
	gtk_widget_show (tree);
	return tree;
}


void
yq_cfg_remove_sections (ConfigFile * cfg, gchar * section)
{
	gint i, z;
	gchar tz[260];

	yq_cfg_read_int (cfg, section, "z", &z);
	for (i = 1; i <= z; i++)
	{
		sprintf (tz, "%s_%d", section, i);
		yq_cfg_remove_sections (cfg, tz);
	}
	yq_cfg_remove_section (cfg, section);
}

void
yq_cfg_clean_sections (ConfigFile * cfg, gchar * section)
{
	gint i, z;
	gchar tz[260];

	yq_cfg_read_int (cfg, section, "z", &z);
	for (i = 1; i <= z; i++)
	{
		sprintf (tz, "%s_%d", section, i);
		yq_cfg_remove_sections (cfg, tz);
	}
	yq_cfg_read_int (cfg, section, "a", &z);
	for (i = 1; i <= z; i++)
	{
		sprintf (tz, "%d",i);
		yq_cfg_remove_key (cfg, section,tz);
	}
	yq_cfg_write_int (cfg, section, "z", 0);
	yq_cfg_write_int (cfg, section, "a", 0);
}

void
yq_cfg_daiti_sections (ConfigFile * cfg, gchar * section1, gchar * section2)
{
	yq_cfg_remove_sections (cfg, section1);
	yq_cfg_rename_sections (cfg, section2, section1);
}

void
yq_cfg_rename_sections (ConfigFile * cfg, gchar * section,
			gchar * section_name)
{
	gint i, z;
	gchar tz1[260], tz2[260];
	yq_cfg_read_int (cfg, section, "z", &z);
	for (i = 1; i <= z; i++)
	{
		sprintf (tz1, "%s_%d", section, i);
		sprintf (tz2, "%s_%d", section_name, i);
		yq_cfg_rename_sections (cfg, tz1, tz2);
	}
	yq_cfg_rename_section (cfg, section, section_name);
}



void
nb_setchanged (GtkWidget * widget, gpointer data)
{
	if ((widget != save_button) == nb_changed)
		return;

	gtk_widget_set_sensitive (new_button, nb_changed);
	gtk_widget_set_sensitive (menu_file_new, nb_changed);
	gtk_widget_set_sensitive (open_button, nb_changed);
	gtk_widget_set_sensitive (menu_file_open, nb_changed);
	gtk_widget_set_sensitive (menu_file_openold, nb_changed);
	gtk_widget_set_sensitive (menu_file_close, nb_changed);
	gtk_widget_set_sensitive (menu_file_saveas, nb_changed);
	gtk_widget_set_sensitive (menu_file_import, nb_changed);
	gtk_widget_set_sensitive (menu_file_export, nb_changed);
	gtk_widget_set_sensitive (cut_button, nb_changed);
	gtk_widget_set_sensitive (menu_edit_cut, nb_changed);
	gtk_widget_set_sensitive (copy_button, nb_changed);
	gtk_widget_set_sensitive (menu_edit_copy, nb_changed);
	gtk_widget_set_sensitive (paste_button, nb_changed);
	gtk_widget_set_sensitive (menu_edit_paste, nb_changed);
	gtk_widget_set_sensitive (menu_edit_selectall, nb_changed);
	gtk_widget_set_sensitive (menu_edit_selectelse, nb_changed);
	gtk_widget_set_sensitive (menu_edit_find, nb_changed);
	gtk_widget_set_sensitive (search_button, nb_changed);
	gtk_widget_set_sensitive (add_button, nb_changed);
	gtk_widget_set_sensitive (menu_jilu_add, nb_changed);
	gtk_widget_set_sensitive (remove_button, nb_changed);
	gtk_widget_set_sensitive (menu_jilu_remove, nb_changed);
	gtk_widget_set_sensitive (zu_button, nb_changed);
	gtk_widget_set_sensitive (menu_jilu_zu, nb_changed);
	gtk_widget_set_sensitive (rename_button, nb_changed);
	gtk_widget_set_sensitive (menu_jilu_rename, nb_changed);
	gtk_widget_set_sensitive (mima_button, nb_changed);
	gtk_widget_set_sensitive (menu_tool_trash, nb_changed);
	gtk_widget_set_sensitive (laji_button, nb_changed);
	gtk_widget_set_sensitive (up_button, nb_changed);

	gtk_widget_set_sensitive (treewin, nb_changed);


	nb_changed = !nb_changed;
	
	gtk_widget_set_sensitive (menu_jilu_save, nb_changed);
	gtk_widget_set_sensitive (menu_jilu_saveadd, nb_changed);
	gtk_widget_set_sensitive (menu_jilu_cancel, nb_changed);
	gtk_widget_set_sensitive (save_button, nb_changed);
	gtk_widget_set_sensitive (saveadd_button, nb_changed);
	gtk_widget_set_sensitive (cancel_button, nb_changed);
}

void
addtotext (gchar ** a, gchar * bb)
{
	gint i;
	gchar *b;
	if (*bb == '\0')
	{
		**a = '~';
		(*a)++;
		**a = '0';
		(*a)++;
	}
	else
	{
		b = bb;
		for (i = 1; i <= strlen (bb); i++)
		{
			switch (*b)
			{
			case '~':
				**a = '~';
				(*a)++;
				**a = '~';
				break;
			case '\n':
				**a = '~';
				(*a)++;
				**a = 'n';
				break;
			case ',':
				**a = '~';
				(*a)++;
				**a = 'd';
				break;
			default:
				**a = *b;
				break;
			}
			(*a)++;
			b++;
		}
	}
	**a = ',';
	(*a)++;
}

void
toolbar_save (GtkWidget * widget, gpointer data)
{
	gchar *ab, *a, *c;
	gint i;

	if (*gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (nbitems2[5])->entry)) == '\0')
	{
		Popup (_("Please input the display name"));
		return;
	}
	ab = g_malloc (10000);
	a = ab;
	c = gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (nbitems2[5])->entry));
	if (!nb_isadd)
		yq_tree_item_set_text (selectleaf, c);
	addtotext (&a, c);
	c = gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (nbitems5[0])->entry));
	if (!nb_isadd)
	{
		if (!strcmp(c, _("Female")))
		{
			yq_tree_item_set_pixmap (selectleaf, GIRL);
		}
		else
		{
		
			yq_tree_item_set_pixmap (selectleaf, BOY);
		}
	}
	addtotext (&a, c);
	gtk_label_get(GTK_LABEL(nbitems2[0]),&c);
	addtotext (&a,c);
	gtk_label_get(GTK_LABEL(nbitems2[1]),&c);
	addtotext (&a,c);
	for (i=2;i<5;i++)
		addtotext (&a, gtk_entry_get_text (GTK_ENTRY (nbitems2[i])));
	for (i=6;i<11;i++)
		addtotext (&a, gtk_entry_get_text (GTK_ENTRY (nbitems2[i])));
	c = gtk_editable_get_chars (GTK_EDITABLE (nbitems2[11]), (gint) 0,(gint) - 1);
	addtotext (&a, c);
	g_free (c);
	c = gtk_editable_get_chars (GTK_EDITABLE (nbitems3[0]), (gint) 0,(gint) - 1);
	addtotext (&a, c);
	g_free (c);
	for (i=1;i<9;i++)
		addtotext (&a, gtk_entry_get_text (GTK_ENTRY (nbitems3[i])));
	addtotext (&a, gtk_entry_get_text (GTK_ENTRY (nbitems4[0])));
	addtotext (&a, gtk_entry_get_text (GTK_ENTRY (nbitems4[1])));
	c = gtk_editable_get_chars (GTK_EDITABLE (nbitems4[2]), (gint) 0,(gint) - 1);
	addtotext (&a, c);
	g_free (c);
	for (i=3;i<14;i++)
		addtotext (&a, gtk_entry_get_text (GTK_ENTRY (nbitems4[i])));
	addtotext (&a, gtk_entry_get_text (GTK_ENTRY (nbitems5[1])));
	addtotext (&a, gtk_entry_get_text (GTK_ENTRY (nbitems5[2])));
	c = gtk_editable_get_chars (GTK_EDITABLE (nbitems5[3]), (gint) 0,(gint) - 1);
	addtotext (&a, c);
	g_free (c);
	for (i=4;i<7;i++)
		addtotext (&a, gtk_entry_get_text (GTK_ENTRY (nbitems5[i])));
	c = gtk_editable_get_chars (GTK_EDITABLE (nbitems5[7]), (gint) 0,(gint) - 1);
	addtotext (&a, c);
	g_free (c);
	addtotext (&a, gtk_entry_get_text (GTK_ENTRY (nbitems6[0])));
	c = gtk_editable_get_chars (GTK_EDITABLE (nbitems6[1]), (gint) 0,(gint) - 1);
	addtotext (&a, c);
	g_free (c);
	c = gtk_editable_get_chars (GTK_EDITABLE (nbitems6[2]), (gint) 0,(gint) - 1);
	addtotext (&a, c);
	g_free (c);
	
	a--;
	*a = '\0';
	nb_setchanged (save_button, NULL);
	if (!nb_isadd)
	{
		a = strchr (selectleaf->nodename, '-');
		*a = '\0';
		a++;
		yq_cfg_write_string (selectleaf->cfg, selectleaf->nodename, a, ab);
		a--;
		*a = '-';
	}
	else
		toolbar_addjilu (selectleaf,ab,FALSE,TRUE);
	g_free (ab);
}

void
toolbar_addjilu (YqTreeItem *nowleaf,gchar * ab,gboolean freeze,gboolean expand)
{
	YqTreeItem *leaf;
	gchar bb[260], *a, zu[256];
	gint i, n;

	a = strchr (ab, ',');
	*a = '\0';
	strcpy (bb, ab);
	leaf_formattext (bb);
	*a = ',';
	if (strstr((a + 1), _("Female")) == (a+1))
		leaf = YQ_TREE_ITEM (yq_tree_item_new (bb, GIRL));
	else
		leaf = YQ_TREE_ITEM (yq_tree_item_new (bb, BOY));

	leaf->cfg = nowleaf->cfg;
	if (!strchr(nowleaf->nodename,'-'))
	{
		strcpy (zu, nowleaf->nodename);
		if (!GTK_TREE_ITEM (nowleaf)->subtree)
			gtk_tree_item_set_subtree (GTK_TREE_ITEM (nowleaf),
						   yq_createnulltree ());
		if ((expand)&&(!GTK_TREE_ITEM (nowleaf)->expanded))
			gtk_tree_item_expand (GTK_TREE_ITEM (nowleaf));
		gtk_tree_append (GTK_TREE
				 (GTK_TREE_ITEM (nowleaf)->subtree),
				 GTK_WIDGET (leaf));
		leaf->attree = GTK_TREE (GTK_TREE_ITEM (nowleaf)->subtree);
	}
	else
	{
		strcpy (zu, nowleaf->nodename);
		a = strchr (zu, '-');
		*a = '\0';
		gtk_tree_append (nowleaf->attree, GTK_WIDGET (leaf));
		leaf->attree = nowleaf->attree;
	}

	yq_cfg_read_int (nowleaf->cfg, zu, "a", &i);
	i++;
	sprintf (bb, "%s-%d", zu, i);
	strcpy (leaf->nodename, bb);
	sprintf (bb, "%d", i);
	yq_cfg_write_string (leaf->cfg, zu, bb, ab);
	yq_cfg_write_int (leaf->cfg, zu, "a", i);
	nb_isadd = 0;
	if (freeze)
		return;
	yq_cfg_read_int (leaf->cfg, zu, "z", &n);
	n = i + n - 1;
	yq_tree_select_item (GTK_TREE (YQ_TREE_ITEM (leaf)->attree), n);
}

void
toolbar_saveadd (GtkWidget * widget, gpointer data)
{
	toolbar_save (save_button, NULL);
	toolbar_add (add_button, NULL);
}


void
toolbar_cancel (GtkWidget * widget, gpointer data)
{
	gchar *ab, zu[260], *a, tj[4];

	if (yq_nowiszu)
	{
		open_zu (selectleaf);
		gtk_clist_select_row (GTK_CLIST (yqlist.list),
				      clist_selectrow, 0);
	}
	else
	{
		strcpy (zu, selectleaf->nodename);
		a = strchr (zu, '-');
		*a = '\0';
		a++;
		strcpy (tj, a);
		yq_cfg_read_string (selectleaf->cfg, zu, tj, &ab);
		leaf_setnb (ab);
		g_free (ab);
	}
	nb_setchanged (save_button, NULL);
	nb_isadd = 0;
}

void
toolbar_clean_trash(YqTreeItem *trashitem,gboolean ask)
{
	GList *list;
	YqTreeItem *rmitem;
	
	if (ask)
	{
		Areyousure (_("Are you sure to clean the trash?"));
		if (!yesnoreturn)
			return;
		yq_play_sound(10);
	}
		
	list = gtk_container_children (GTK_CONTAINER (GTK_TREE_ITEM(trashitem)->subtree));
    while (list)
	{
		rmitem = YQ_TREE_ITEM(list -> data);
		if (strchr (rmitem->nodename, '-'))
		{
			toolbar_remove_leaf(rmitem);
		}
		else
		{
			toolbar_remove_zu(rmitem);
		}
		list = g_list_remove_link (list,list);
	}
}

void
toolbar_remove (GtkWidget * widget, gpointer data)
{
	gint n, j=0, z;
	gchar *ab,aa[260],*bb;
	YqTreeItem *rmitem,*tmpitem;
	GtkTree *tmptree;
	GList *list;
	gboolean trashopend;
	
	if (strcmp(selectleaf->nodename,"r") ==0)
		return;

	if (!yq_nowiszu)
	{
		rmitem=selectleaf;
		bb = strchr (rmitem->nodename, '-');
		*bb = '\0';
		bb++;
		j = atoi (bb);
		yq_cfg_read_int (rmitem->cfg, rmitem->nodename, "z", &z);
		yq_cfg_read_int (rmitem->cfg, rmitem->nodename, "a", &n);
		bb--;
		*bb='-';
		tmpitem=YQ_TREE_ITEM(rmitem->attree->tree_owner);
		tmptree=rmitem->attree;
		trashopend=(strcmp(YQ_TREE_ITEM(rmitem->attree->children->data)->nodename,"yq_0")==0);
		toolbar_remove_leaf(rmitem);
		if (z + n == 1)
		{
			yq_tree_select_child (tmpitem->attree,GTK_WIDGET(tmpitem));
		}
		else
		{
			if (j == n)
				j = j + z - 2;
			else
				j = j + z-1;
			if (trashopend)
				j++;
			yq_tree_select_item (tmptree, j);
		}
	}
	else
	{
		if (clist_selectrow == -1)
		{
			if (strcmp (selectleaf->nodename, "yq") == 0) return;
			if (strcmp (selectleaf->nodename, "a") == 0) return;
			if (strcmp (selectleaf->nodename, "yq_0") == 0)
			{
				if (GTK_TREE(GTK_TREE_ITEM(selectleaf)->subtree)->children)
				{
					toolbar_clean_trash(selectleaf,widget!=NULL);
					open_zu (selectleaf);				
				}
				else
				{
					tmpitem=YQ_TREE_ITEM(selectleaf->attree->tree_owner);
					yq_tree_remove_item (GTK_WIDGET(selectleaf->attree), GTK_WIDGET (selectleaf));
					if (GTK_TREE(GTK_TREE_ITEM(tmpitem)->subtree)->children)
						yq_tree_select_item (GTK_TREE(GTK_TREE_ITEM(tmpitem)->subtree), 0);
					else
						yq_tree_select_child (tmpitem->attree,GTK_WIDGET(tmpitem));
				}
				return;
			}
			rmitem = selectleaf;
			
			if (widget)  /*don't ask when cut 删除时询问,剪切时不询问*/
			{
				yq_cfg_read_string (rmitem->cfg,rmitem->nodename, "n", &ab);
				sprintf (aa, _("Are you sure to delete folder \"%s\"?"), ab);
				g_free (ab);
				Areyousure (aa);
				if (!yesnoreturn)
					return;
				yq_play_sound(10);
			}

			
			bb = strrchr (rmitem->nodename, '_');			
			*bb = '\0';
			bb++;
			j = atoi (bb);
			yq_cfg_read_int (rmitem->cfg, rmitem->nodename, "z", &z);
			yq_cfg_read_int (rmitem->cfg, rmitem->nodename, "a", &n);
			bb--;
			*bb='_';
			
			trashopend=(strcmp(YQ_TREE_ITEM(rmitem->attree->children->data)->nodename,"yq_0")==0);
			tmpitem=YQ_TREE_ITEM(rmitem->attree->tree_owner);
			tmptree=rmitem->attree;
			toolbar_remove_zu(rmitem);
			
			if (z + n == 1)
			{
				yq_tree_select_child (tmpitem->attree,GTK_WIDGET(tmpitem));
			}
			else
			{
				if (j!=0)
				{
				if ((j == z) && (z != 1))
					j = j - 2;
				else    j--;
				if (trashopend)
					j++;
				}
				else
					j=1;
				yq_tree_select_item (tmptree, j);
			}
		}
		else
		{
			list=GTK_CLIST(yqlist.list) -> selection;
			if (list->next)
			{
				if (widget)
				{
					Areyousure (_("Are you sure to delete?"));
					if (!yesnoreturn)
						return;
					yq_play_sound(10);
				}
				while (list!=NULL)
				{
					rmitem = (YqTreeItem *)gtk_clist_get_row_data(GTK_CLIST(yqlist.list), (gint) list -> data);
					if (strchr (rmitem->nodename, '-'))
					{
						toolbar_remove_leaf(rmitem);
					}
					else
					{
						if (strcmp(rmitem->nodename,"yq_0")!=0)
							toolbar_remove_zu(rmitem);
					}
					list=list->next;
				}
				if (strcmp (selectleaf->nodename, "a") == 0)
				{
					open_allitem(selectleaf);
				}
				else
				{
					open_zu (selectleaf);
				}
			}
			else
			{
				if (strcmp(selectleaf->nodename,"a")==0)
					trashopend=FALSE;
				else
					trashopend=(strcmp(YQ_TREE_ITEM(GTK_TREE(GTK_TREE_ITEM(selectleaf)->subtree)->children->data)->nodename,"yq_0")==0);
				rmitem = (YqTreeItem *)
					gtk_clist_get_row_data (GTK_CLIST
								(yqlist.list),
								clist_selectrow);
				if (strchr (rmitem->nodename, '-'))
				{
					bb = strchr (rmitem->nodename, '-');
					*bb = '\0';
					bb++;
					j = atoi (bb);
					yq_cfg_read_int (rmitem->cfg, rmitem->nodename, "z", &z);
					yq_cfg_read_int (rmitem->cfg, rmitem->nodename, "a", &n);
					bb--;
					*bb='-';
					if (!strcmp (selectleaf->nodename, "a") == 0)
						tmpitem=YQ_TREE_ITEM(rmitem->attree->tree_owner);
					toolbar_remove_leaf(rmitem);
					if (strcmp (selectleaf->nodename, "a") == 0)
					{
						j=clist_selectrow;
						n=GTK_CLIST(yqlist.list) -> rows;
						open_allitem(selectleaf);
						if (n!=1)
						{
							if (j==n-1)
								j--;
							gtk_clist_select_row (GTK_CLIST (yqlist.list), j, 0);
						}						
					}
					else
					{
						open_zu (selectleaf);
						if (j == n)
							j = j + z - 2;
						else
							j = j + z - 1;
						if (trashopend)
							j++;
						gtk_clist_select_row (GTK_CLIST (yqlist.list), j, 0);
					}
				}
				else
				{
					if (strcmp (rmitem->nodename, "yq_0") == 0)
					{
						if (GTK_TREE(GTK_TREE_ITEM(rmitem)->subtree)->children)
							toolbar_clean_trash(rmitem,widget!=NULL);
						else
							yq_tree_remove_item (GTK_WIDGET(rmitem->attree), GTK_WIDGET (rmitem));
					}
					else
					{
						if (widget)
						{
							yq_cfg_read_string (rmitem->cfg,rmitem->nodename, "n", &ab);
							sprintf (aa, _("Are you sure to delete folder \"%s\"?"), ab);
							g_free (ab);
							Areyousure (aa);
							if (!yesnoreturn)
								return;
							yq_play_sound(10);
						}

						bb = strrchr (rmitem->nodename, '_');			
						*bb = '\0';
						bb++;
						j = atoi (bb);
						yq_cfg_read_int (rmitem->cfg, rmitem->nodename, "z", &z);
						yq_cfg_read_int (rmitem->cfg, rmitem->nodename, "a", &n);
						bb--;
						*bb='_';

						tmpitem=YQ_TREE_ITEM(rmitem->attree->tree_owner);
						toolbar_remove_zu(rmitem);
					}
					open_zu (selectleaf);
					if (j!=0)
					{
						if ((j == z) && (z != 1))
							j = j - 2;
						else
							j--;
						if (trashopend)
							j++;
					}
					gtk_clist_select_row (GTK_CLIST (yqlist.list), j, 0);
				}				
			}
		}
	}
}

void
toolbar_copyto_trash(YqTreeItem *leaf)
{
	gint n;
	gchar *a,*ab,tz[256];
	YqTreeItem *openitem,*trashitem;
	gboolean trashopen;
	
	if (strstr(leaf->nodename,"yq_0")==leaf->nodename)
		return;
	openitem=leaf;
	while (openitem->attree != GTK_TREE (tree_main))
		openitem = YQ_TREE_ITEM (openitem->attree->tree_owner);
	trashitem=YQ_TREE_ITEM(GTK_TREE(GTK_TREE_ITEM(openitem)->subtree)->children->data);
	if (strcmp(trashitem->nodename,"yq_0")==0)
		trashopen=TRUE;
	else
		trashopen=FALSE;
	if (strchr(leaf->nodename,'-'))
	{
		a = strchr (leaf->nodename, '-');
		*a = '\0';
		a++;
		strcpy(tz,a);						
		yq_cfg_read_string (leaf->cfg, leaf->nodename, tz, &ab);
		a--;
		*a='-';
		if (trashopen)
			toolbar_addjilu (trashitem,ab,TRUE,FALSE);
		else
		{
			yq_cfg_read_int (leaf->cfg,"yq_0", "a", &n);						
			yq_cfg_write_int (leaf->cfg, "yq_0", "a", n+1);
			sprintf(tz,"%d",n+1);
			yq_cfg_write_string (leaf->cfg, "yq_0", tz, ab);
		}
		g_free(ab);
	}
	else
	{
		yq_cfg_read_int (leaf->cfg,"yq_0", "z", &n);						
		yq_cfg_write_int (leaf->cfg, "yq_0", "z", n+1);
		sprintf(tz,"%s_%d","yq_0",n+1);
		yq_config_copy_sections(leaf->cfg,leaf->nodename,leaf->cfg,tz);
		if (trashopen)
			toolbar_addzu (trashitem,TRUE,FALSE);
	}
}

void
toolbar_remove_zu(YqTreeItem *rmitem)
{
	gint j,n,z,i;
	gchar ti[4], tz[260], aa1[260],aa2[260],*bb;
	GList *children;

	toolbar_copyto_trash(rmitem);
	strcpy (tz, rmitem->nodename);		
	bb = strrchr (tz, '_');
	*bb = '\0';
	bb++;
	strcpy (ti, bb);
	j = atoi (ti);
	yq_cfg_read_int (rmitem->cfg, tz, "z", &z);
	yq_cfg_read_int (rmitem->cfg, tz, "a", &n);
	if (strcmp(YQ_TREE_ITEM(rmitem->attree->children->data)->nodename,"yq_0")==0)
		children =
			g_list_nth (gtk_container_children
			    (GTK_CONTAINER (rmitem->attree)), j);
	else
		children =
			g_list_nth (gtk_container_children
			    (GTK_CONTAINER (rmitem->attree)), j - 1);
	yq_cfg_write_int (rmitem->cfg, tz, "z", z - 1);
	yq_cfg_remove_sections (rmitem->cfg, rmitem->nodename);
	for (i = j; i < z; i++)
	{
		sprintf (aa1, "%s_%d", tz, i);
		sprintf (aa2, "%s_%d", tz, i + 1);
		yq_cfg_rename_sections (rmitem->cfg, aa2,
					aa1);
		children = children->next;
		toolbar_change_nodenames(YQ_TREE_ITEM (children->data),aa1);
	}
	yq_tree_remove_item (GTK_WIDGET(rmitem->attree), GTK_WIDGET (rmitem));
}

void
toolbar_change_nodenames(YqTreeItem *cnitem,gchar *name)
{
	GList *children;
	gint i;
	
	children = gtk_container_children (GTK_CONTAINER (GTK_TREE_ITEM(cnitem)->subtree));
    while (children)
	{
      if (strchr(YQ_TREE_ITEM (children->data)->nodename,'-'))
	  {
		  for (i=0;i<strlen(name);i++)
		  {
			  YQ_TREE_ITEM (children->data)->nodename[i]=name[i];
		  }
	  }
	  else
	  {
		  toolbar_change_nodenames(YQ_TREE_ITEM (children->data),name);
	  }
      children = g_list_remove_link (children, children);
	}
	for (i=0;i<strlen(name);i++)
	{
	cnitem->nodename[i]=name[i];
	}
}

void
toolbar_remove_leaf(YqTreeItem *rmitem)
{
	gint j,n,z,i;
	gchar *ab,*bb,ti[4], tj[4], tz[260], aa[260];
	GList *children;
	YqTreeItem *treeitem;
	toolbar_copyto_trash(rmitem);
	strcpy (tz, rmitem->nodename);	
	bb = strchr (tz, '-');
	*bb = '\0';
	bb++;
	strcpy (ti, bb);
	j = atoi (ti);
	yq_cfg_read_int (rmitem->cfg, tz, "z", &z);
	yq_cfg_read_int (rmitem->cfg, tz, "a", &n);
	if (strcmp(YQ_TREE_ITEM(rmitem->attree->children->data)->nodename,"yq_0")==0)
		children =g_list_nth (gtk_container_children
			    (GTK_CONTAINER (rmitem->attree)),
				    z + j);
	else
		children =g_list_nth (gtk_container_children
			    (GTK_CONTAINER (rmitem->attree)),
				    z + j - 1);
	for (i = j; i < n; i++)
	{
		sprintf (ti, "%d", i);
		sprintf (tj, "%d", i + 1);
		yq_cfg_read_string (rmitem->cfg, tz, tj, &ab);
		yq_cfg_write_string (rmitem->cfg, tz, ti, ab);
		g_free(ab);
		children = children->next;
		treeitem = YQ_TREE_ITEM (children->data);
		sprintf (aa, "%s-%d", tz, i);
		strcpy (treeitem->nodename, aa);
	}
	yq_cfg_write_int (rmitem->cfg, tz, "a", n - 1);
	sprintf (tj, "%d", n);
	yq_cfg_remove_key (rmitem->cfg, tz, tj);
	yq_tree_remove_item (GTK_WIDGET(rmitem->attree), GTK_WIDGET (rmitem));
}

void
toolbar_add (GtkWidget * widget, gpointer data)
{
	gint i;
	
	if (strlen(selectleaf->nodename) ==1)
		return;
	if (yqlist.type == LIST_DETAILS)
		yq_set_list_type (LIST_NOTEBOOK);
	else
	{
		nb_changed = 1;
		
		for (i=0;i<14;i++)
		{
			gtk_label_set_text(GTK_LABEL(nbitems1[i]),"");
		}
		gtk_label_set_text(GTK_LABEL(nbitems2[0]),"");
		gtk_label_set_text(GTK_LABEL(nbitems2[1]),"");
		for (i=2;i<5;i++)
		{
			gtk_entry_set_text(GTK_ENTRY(nbitems2[i]),"");
		}
		gtk_entry_set_text(GTK_ENTRY(GTK_COMBO (nbitems2[5])->entry),"");
		yq_nb_changed(nbitems2[2],NULL);
		for (i=6;i<11;i++)
		{
			gtk_entry_set_text(GTK_ENTRY(nbitems2[i]),"");
		}
		gtk_editable_delete_text (GTK_EDITABLE (nbitems2[11]), 0, -1);
		gtk_editable_delete_text (GTK_EDITABLE (nbitems3[0]), 0, -1);
		for (i=1;i<9;i++)
		{
			gtk_entry_set_text(GTK_ENTRY(nbitems3[i]),"");
		}
		gtk_entry_set_text(GTK_ENTRY(nbitems4[0]),"");
		gtk_entry_set_text(GTK_ENTRY(nbitems4[1]),"");
		gtk_editable_delete_text (GTK_EDITABLE (nbitems4[2]), 0, -1);
		for (i=3;i<14;i++)
		{
			gtk_entry_set_text(GTK_ENTRY(nbitems4[i]),"");
		}
		gtk_entry_set_text(GTK_ENTRY(GTK_COMBO (nbitems5[0])->entry),"Unspecified");
		gtk_entry_set_text(GTK_ENTRY(nbitems5[1]),"");
		gtk_entry_set_text(GTK_ENTRY(nbitems5[2]),"");
		gtk_editable_delete_text (GTK_EDITABLE (nbitems5[3]), 0, -1);
		gtk_entry_set_text(GTK_ENTRY(nbitems5[4]),"");
		gtk_entry_set_text(GTK_ENTRY(nbitems5[5]),"");
		gtk_entry_set_text(GTK_ENTRY(nbitems5[6]),"");
		gtk_editable_delete_text (GTK_EDITABLE (nbitems5[7]), 0, -1);
		gtk_entry_set_text(GTK_ENTRY(nbitems6[0]),"");
		gtk_editable_delete_text (GTK_EDITABLE (nbitems6[1]), 0, -1);
		gtk_editable_delete_text (GTK_EDITABLE (nbitems6[2]), 0, -1);

		nb_changed = 0;
	}
	nb_isadd = 1;
	nb_setchanged (widget, NULL);
	gtk_notebook_set_page (GTK_NOTEBOOK (yqlist.list), 1);
	gtk_widget_grab_focus (nbitems2[2]);
}

void
toolbar_zu (GtkWidget * widget, gpointer data)
{
	gint z;
	gchar tz[260], *a;

	if (strlen(selectleaf->nodename) == 1)
		return;
	strcpy (tz, selectleaf->nodename);
	a = strchr (tz, '-');
	if (a)
		*a = '\0';
	yq_cfg_read_int (selectleaf->cfg, tz, "z", &z);
	InputName (_("Please input the folder name:"), _("New Folder"));
	if (!yesnoreturn)
		return;
	if (sFilename[0] == '\0')
	{
		Popup (_("Folder's name can't be NULL!"));
		return;
	}
	yq_cfg_write_int (selectleaf->cfg, tz, "z", z + 1);
	sprintf (tz, "%s_%d", tz, z + 1);
	yq_cfg_write_string (selectleaf->cfg, tz, "n", sFilename);
	yq_cfg_write_int (selectleaf->cfg, tz, "z", 0);
	yq_cfg_write_int (selectleaf->cfg, tz, "a", 0);
	toolbar_addzu (selectleaf,FALSE,TRUE);
}

void
toolbar_addzu (YqTreeItem *nowleaf,gboolean freeze,gboolean expand)
{
	gint z;
	gchar tz[260], zu[260], *zuname;
	YqTreeItem *newleaf,*zuitem;

	strcpy (tz, nowleaf->nodename);
	zuname = strchr (tz, '-');
	if (zuname)
	{
		*zuname = '\0';
		zuitem=YQ_TREE_ITEM(nowleaf->attree->tree_owner);
	}
	else
		zuitem=nowleaf;
	yq_cfg_read_int (zuitem->cfg, tz, "z", &z);
	sprintf (zu, "%s_%d", tz, z);
	yq_cfg_read_string (zuitem->cfg, zu, "n", &zuname);
	newleaf = YQ_TREE_ITEM (yq_tree_item_new (zuname, ZU_CLOSE));
	strcpy (newleaf->nodename, zu);
	g_free (zuname);
	newleaf->cfg = zuitem->cfg;
	if (!GTK_TREE_ITEM (zuitem)->subtree)
		gtk_tree_item_set_subtree (GTK_TREE_ITEM (zuitem),
				   yq_createnulltree ());
	if ((expand)&&(!GTK_TREE_ITEM (zuitem)->expanded))
		gtk_tree_item_expand (GTK_TREE_ITEM (zuitem));
	newleaf->attree = GTK_TREE (GTK_TREE_ITEM (zuitem)->subtree);
	gtk_signal_connect (GTK_OBJECT (newleaf), "expand",
			    GTK_SIGNAL_FUNC (zu_expanded), NULL);
	gtk_signal_connect (GTK_OBJECT (newleaf), "collapse",
			    GTK_SIGNAL_FUNC (zu_collapsed), NULL);
	gtk_tree_insert (GTK_TREE
			 (GTK_TREE_ITEM (zuitem)->subtree),
			 GTK_WIDGET (newleaf), z - 1);
	gtk_tree_item_set_subtree (GTK_TREE_ITEM (newleaf),
				   yq_readzu (newleaf));
	if (freeze) return;
	if (yq_nowiszu)
		open_zu (zuitem);
	else
		yq_tree_select_child (zuitem->attree,GTK_WIDGET(zuitem));
	gtk_clist_select_row (GTK_CLIST (yqlist.list), z - 1, 0);
}

void
toolbar_open (GtkWidget * widget, gpointer data)
{
	GetFilename (_("Open Yqpim File"));
	if (sFilename[0] == '\0') return;
	if (sFilename[strlen(sFilename)-1] == '/')
	{
		Popup(_("Please type the file name"));
		return;
	}
	yq_readfile (widget, sFilename);
}

void
toolbar_rename (GtkWidget * widget, gpointer data)
{
	gchar *ab, tz[256], zu[260];
	YqTreeItem *renameitem;

	if (!yq_nowiszu)
		return;
	if (strcmp(selectleaf->nodename,"a") ==0)
		return;
	if ((strcmp(selectleaf->nodename,"r") ==0)&&(clist_selectrow==-1))
		return;
	if (clist_selectrow==-1)
		renameitem=selectleaf;
	else
		renameitem = (YqTreeItem *)
			gtk_clist_get_row_data (GTK_CLIST
						(yqlist.list),
						clist_selectrow);
	if (strchr(renameitem->nodename,'-')) return;
	strcpy (tz, renameitem->nodename);
	yq_cfg_read_string (renameitem->cfg, tz, "n", &ab);
	sprintf (zu, _("Rename \"%s\" As:"), ab);
	InputName (zu, ab);
	if (!yesnoreturn)
		return;
	strcpy (zu, ab);
	g_free (ab);
	if (sFilename[0] == '\0')
	{
		Popup (_("File name can't be NULL!"));
		return;
	}
	if (!strcmp (sFilename, zu))
	{
		Popup (_("The name is the same!"));
		return;
	}
	yq_tree_item_set_text (renameitem, sFilename);
	yq_cfg_write_string (renameitem->cfg, tz, "n", sFilename);
	if (clist_selectrow!=-1)
			gtk_clist_set_pixtext (GTK_CLIST (yqlist.list), clist_selectrow,0, sFilename, 1, yqpixmaps[ZU_CLOSE],
					       yqmasks[ZU_CLOSE]);
}

void
toolbar_new (GtkWidget * widget, gpointer data)
{
	struct stat st;

	GetFilename (_("New Yqpim File"));
	if (sFilename[0] == '\0')
		return;
	if (sFilename[strlen(sFilename)-1] == '/')
	{
		Popup(_("Please type the file name"));
		return;
	}
	if (!stat (sFilename, &st))
	{
		Areyousure (_("This file exist,overwrite?"));
		if (!yesnoreturn)
			return;
	}
	create_yqfile (sFilename); /*createfile.c*/
	yq_readfile (widget, sFilename); /*treefunc.c*/
}

void
toolbar_copy (GtkWidget * widget, gpointer data)
{
	gchar tz[260], *a,*ab;
	GList *list;
	YqTreeItem *leaf;
	gint n,i;


	if ((strcmp(selectleaf->nodename,"r") == 0)&&(clist_selectrow==-1))
		return;

	if (!tmpcfg) tmpcfg=yq_cfg_new();
	else
		yq_cfg_free (tmpcfg);
	if (yq_nowiszu)
	{
			if (clist_selectrow==-1)
			{
				if (strcmp(selectleaf->nodename,"a") == 0)
				{
					n=GTK_CLIST(yqlist.list) -> rows;
					yq_cfg_write_int (tmpcfg, "yq", "z", 0);
					yq_cfg_write_int (tmpcfg, "yq", "a", n);										
					for (i=0;i<n;i++)
					{
						leaf = (YqTreeItem *)gtk_clist_get_row_data(GTK_CLIST(yqlist.list),i);
						a = strchr (leaf->nodename, '-');
						*a = '\0';
						a++;
						strcpy(tz,a);						
						yq_cfg_read_string (leaf->cfg, leaf->nodename, tz, &ab);
						a--;
						*a='-';
						sprintf(tz,"%d",i+1);
						yq_cfg_write_string (tmpcfg, "yq", tz, ab);
						g_free(ab);						
					}
				}
				else
				{
					yq_cfg_write_string (tmpcfg, "yq", "z", "1");
					yq_cfg_write_string (tmpcfg, "yq", "a", "0");
					yq_config_copy_sections(selectleaf->cfg,selectleaf->nodename,tmpcfg,"yq_1");
				}
			}
			else
			{
				yq_cfg_write_string (tmpcfg, "yq", "z", "0");
				yq_cfg_write_string (tmpcfg, "yq", "a", "0");
				list=GTK_CLIST(yqlist.list) -> selection;
				while (list!=NULL)
				{				
					leaf = (YqTreeItem *)gtk_clist_get_row_data(GTK_CLIST(yqlist.list), (gint) list -> data);
					if (strchr(leaf->nodename,'-'))
					{
						yq_cfg_read_int (tmpcfg,"yq", "a", &n);						
						yq_cfg_write_int (tmpcfg, "yq", "a", n+1);
						a = strchr (leaf->nodename, '-');
						*a = '\0';
						a++;
						strcpy(tz,a);						
						yq_cfg_read_string (leaf->cfg, leaf->nodename, tz, &ab);
						a--;
						*a='-';
						sprintf(tz,"%d",n+1);
						yq_cfg_write_string (tmpcfg, "yq", tz, ab);
						g_free(ab);
					}
					else
					{
						yq_cfg_read_int (tmpcfg,"yq", "z", &n);						
						yq_cfg_write_int (tmpcfg, "yq", "z", n+1);
						sprintf(tz,"%s_%d","yq",n+1);
						yq_config_copy_sections(leaf->cfg,leaf->nodename,tmpcfg,tz);
					}
					list=list->next;
				}
			}
	}
	else
	{
		a = strchr (selectleaf->nodename, '-');
		*a = '\0';
		a++;
		strcpy(tz,a);
		yq_cfg_read_string (selectleaf->cfg, selectleaf->nodename, tz, &ab);
		a--;
		*a='-';
		yq_cfg_write_string (tmpcfg, "yq", "z", "0");
		yq_cfg_write_string (tmpcfg, "yq", "a", "1");
		yq_cfg_write_string (tmpcfg, "yq", "1", ab);
		g_free(ab);
	}
}

void
yq_config_copy_sections (ConfigFile *cfg1,gchar * zuname1,ConfigFile *cfg2, gchar * zuname2)
{
	gchar *ab, tz[260], tz2[260];
	gint i, z;

	yq_cfg_read_string (cfg1, zuname1, "n", &ab);
	yq_cfg_write_string (cfg2, zuname2, "n", ab);
	g_free (ab);
	yq_cfg_read_int (cfg1, zuname1, "a", &z);
	yq_cfg_write_int (cfg2, zuname2, "a", z);
	for (i = 1; i <= z; i++)
	{
		sprintf (tz, "%d", i);
		yq_cfg_read_string (cfg1, zuname1, tz, &ab);
		yq_cfg_write_string (cfg2, zuname2, tz, ab);
		g_free (ab);
	}
	yq_cfg_read_int (cfg1, zuname1, "z", &z);
	yq_cfg_write_int (cfg2, zuname2, "z", z);
	for (i = 1; i <= z; i++)
	{
		sprintf (tz, "%s_%d", zuname1, i);
		sprintf (tz2, "%s_%d", zuname2, i);
		yq_config_copy_sections (cfg1,tz,cfg2,tz2);
	}
}

void
toolbar_paste (GtkWidget * widget, gpointer data)
{
	gchar *ab, tz1[256],tz2[256];
	gint a,z,i;

	if (!tmpcfg) 
		return;
	if (strlen(selectleaf->nodename) == 1)
		return;
	
	yq_cfg_read_int (tmpcfg,"yq","z",&a);
	for (i=1;i<=a;i++)
	{
		sprintf(tz1,"%s_%d","yq",i);
		strcpy (tz2, selectleaf->nodename);
		ab = strchr (tz2, '-');
		if (ab != NULL)
			*ab = '\0';
		yq_cfg_read_int (selectleaf->cfg, tz2, "z", &z);
		yq_cfg_write_int (selectleaf->cfg, tz2, "z", z + 1);
		sprintf (tz2, "%s_%d", tz2, z + 1);
		yq_config_copy_sections (tmpcfg,tz1,selectleaf->cfg,tz2);
		toolbar_addzu (selectleaf,TRUE,TRUE);	
	}
	yq_cfg_read_int(tmpcfg,"yq","a",&a);
	for (i=1;i<=a;i++)
	{
		sprintf(tz1,"%d",i);
		yq_cfg_read_string
		    (tmpcfg,"yq",tz1,
		     &ab);
		toolbar_addjilu (selectleaf,ab,TRUE,TRUE);
		g_free (ab);
	}
	if (yq_nowiszu)
		open_zu (selectleaf);

}

void
toolbar_cut (GtkWidget * widget, gpointer data)
{	
	toolbar_copy(NULL,NULL);
	toolbar_remove(NULL,NULL);
}

void
toolbar_goup (GtkWidget * widget, gpointer data)
{
	gint row;
	YqTreeItem *leaf;

	if ((yqlist.type==LIST_DETAILS)&&(strlen(selectleaf->nodename)== 1)) return;
	
	if (nb_changed)
	{
		toolbar_cancel(NULL,NULL);
		return;
	}
	leaf=selectleaf;
	yq_tree_select_child (YQ_TREE_ITEM
			       (selectleaf->attree->
				tree_owner)->attree,
			       selectleaf->attree->
			       tree_owner);
	for (row = 0; row < GTK_CLIST(yqlist.list) -> rows; row++)
	{
		if (leaf == (YqTreeItem *)gtk_clist_get_row_data(GTK_CLIST(yqlist.list), row))
			break;
	}
	gtk_clist_select_row (GTK_CLIST (yqlist.list),row, 0);
	gtk_widget_grab_focus (yqlist.list);
}

void
toolbar_search (GtkWidget * widget, gpointer data)
{
	GtkWidget *hbox,*button,*label,*entry;

	if (search_window)
		return;
	search_window = gtk_dialog_new ();
	gtk_signal_connect (GTK_OBJECT (search_window), "destroy",
			    GTK_SIGNAL_FUNC (toolbar_search_nofunc), search_window);
	gtk_window_set_title (GTK_WINDOW (search_window), _("search"));
	gtk_container_border_width (GTK_CONTAINER (search_window), 5);
	hbox=gtk_hbox_new(FALSE,0);
	label = gtk_label_new (_("Search:"));
	gtk_widget_show (label);
	entry = gtk_entry_new ();
	gtk_signal_connect (GTK_OBJECT(entry), "activate",
                      GTK_SIGNAL_FUNC(toolbar_search_yesfunc),
                      entry);
	gtk_widget_show (entry);
	/*GTK_WIDGET_SET_FLAGS (entry, GTK_CAN_FOCUS);*/
	if (search_text)
	{
		gtk_entry_set_text(GTK_ENTRY(entry),search_text);
		gtk_entry_select_region(GTK_ENTRY(entry),0,-1);
	}
	gtk_box_pack_start (GTK_BOX(hbox),label,FALSE,TRUE,0);
	gtk_box_pack_start (GTK_BOX(hbox),entry,TRUE,TRUE,0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (search_window)->vbox),
			    hbox, TRUE, TRUE, 5);
	button = gtk_button_new_with_label (_("search"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (toolbar_search_yesfunc),
			    entry);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (search_window)->action_area),
			    button, TRUE, TRUE, 0);
	gtk_widget_show (button);
	gtk_widget_grab_default (button);
	button = gtk_button_new_with_label (_("close"));
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
			    GTK_SIGNAL_FUNC (toolbar_search_nofunc), search_window);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (search_window)->action_area),
			    button, TRUE, TRUE, 0);
	gtk_widget_show (button);
	gtk_widget_grab_focus (entry);
	gtk_widget_show (search_window);
}

void
toolbar_search_yesfunc (GtkWidget * widget, gpointer data)
{
	gchar *a,*ab,ti[4],*tmptext;
	YqTreeItem *oldleaf,*newleaf;
	gboolean found;

	
	a=gtk_entry_get_text(GTK_ENTRY(data));
	if (*a=='\0')
		return;
	if (!search_text)
		search_text=g_strdup(a);
	else
	{
		if (strcmp(a,search_text)!=0)
		{
			g_free(search_text);
			search_text=g_strdup(a);
		}
	}
	if (strcmp("a",selectleaf->nodename)==0)
		oldleaf=root_leaf;
	else
		oldleaf=selectleaf;
	found=FALSE;
	newleaf=yq_tree_search_next(oldleaf);
	while ((newleaf!=oldleaf)&&(!found))
	{
		a=strchr(newleaf->nodename,'-');
		ab=NULL;
		tmptext=NULL;
		if (a)
		{
			*a='\0';
			a++;
			strcpy(ti,a);
			yq_cfg_read_string (newleaf->cfg, newleaf->nodename, ti, &ab);
			a--;
			*a='-';
		}
		if (ab)
		{
			tmptext=g_malloc(2*(strlen(search_text))+1);
			yq_import_csv_format(search_text,tmptext);
		}
		if (tmptext && (strstr(ab,tmptext)))
		{
			found=TRUE;
			g_free(ab);
			g_free(tmptext);
			yq_tree_item_expand(YQ_TREE_ITEM(newleaf->attree->tree_owner));				
			yq_tree_select_child(newleaf->attree,GTK_WIDGET(newleaf));
		}
		else
		{
			newleaf=yq_tree_search_next(newleaf);
		}
	}	
}

void
toolbar_search_nofunc (GtkWidget * widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
	search_window = NULL;
}
