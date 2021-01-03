#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "intl.h"

#include "configfile.h"
#include "stdio.h"
#include "yqtreeitem.h"
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>

extern struct _YQList yqlist;
extern GdkPixmap *yqpixmaps[MAX_IMAGE_TYPES];
extern GdkBitmap *yqmasks[MAX_IMAGE_TYPES];
extern ConfigFile *rccfgfile;
extern GtkWidget *tree_main;
extern YqTreeItem *selectleaf;

gint clist_selectrow;
gint yq_sorttype;
gboolean titleshow[46];
const gchar *title[] =
	{ N_("Name"), N_("Gender"), N_("Last name's ruby"), N_("First name's ruby"), N_("Last name"), N_("First name"),N_("Title"), N_("nickname"),
 N_("ICQ"), N_("AIM"), N_("Email"), N_("Email2"), N_("Comment"), N_("Home street address"), N_("Home phone"),
 N_("Home Fax"), N_("Home city"), N_("Mobile"), N_("Home state/province"), N_("Home Zip Code"), N_("Home Country/Region"), N_("Home Web Page"), N_("Company"),
 N_("Job Title"), N_("Business Street Address"), N_("Department"), N_("Office"), N_("Business City"), N_("Business Phone"), N_("Business Stage/province"), N_("Business Fax"),
 N_("Business Zip Code"), N_("Pager") ,N_("Business Country/Region"),N_("Ip Phone"),N_("Business Web Page"),N_("Spouse"),N_("Birthday"),N_("Children"),N_("Anniversary"),
 N_("Hobbies"),N_("Speciality"),N_("Saying"),N_("Type"),N_("Public Key"),N_("Notes")};



void clist_readjilu (gchar ** strvalue, gchar * ab);
gchar *leaf_formattext (gchar * bd);
void yq_clist_set_titles(gint column,gboolean scend);
void yq_clist_resort();
GCompareFunc yq_list_get_sort_func();
void yq_clist_add_items(GList *list);
gint yq_cmp_ascend_by_name(YqTreeItem *c1, YqTreeItem *c2);
void yq_play_sound(gint i);
void yq_clist_popup();
void yq_clist_open_item(GtkWidget * widget, gpointer data);
void yq_tree_select_child(GtkTree *tree, GtkWidget *tree_item);
void yq_tree_item_expand(YqTreeItem * item);
void toolbar_copy (GtkWidget * widget, gpointer data);
void toolbar_cut (GtkWidget * widget, gpointer data);
void toolbar_paste (GtkWidget * widget, gpointer data);
void toolbar_remove (GtkWidget * widget, gpointer data);
void toolbar_goup (GtkWidget * widget, gpointer data);
void menu_edit_selectall_func(GtkWidget *widget, gpointer data);
GtkWidget *CreateMenuItem (GtkWidget * menu,
			   char *szName,
			   GtkAccelGroup * accel,
			   guint key,
			   guint mods, GtkSignalFunc func, gpointer data);


void
yq_clist_select_row (GtkCList * clist, gint row, gint column)
{
	clist_selectrow = row;
}


gint
yq_clist_key_press (GtkWidget * widget, GdkEvent *event,
		       gpointer func_data)
{
	if (event->key.keyval == GDK_Return)
	{
		yq_clist_open_item(NULL,NULL);
		return TRUE;
	}
	if (event->key.keyval == GDK_Delete)
	{
		toolbar_remove(widget,NULL);
		return TRUE;
	}
	if (event->key.keyval == GDK_BackSpace)
	{
		toolbar_goup(NULL,NULL);
		return TRUE;
	}
	return FALSE;

}

gint
yq_clist_button_press (GtkWidget * widget, GdkEvent *event,
		       gpointer func_data)
{
	if (event->type == GDK_2BUTTON_PRESS)
	{
		yq_clist_open_item(NULL,NULL);
		return TRUE;
	}
	if (event->button.button == 3)
	{
		yq_clist_popup(event);
		return TRUE;
	}
	return FALSE;
}

void
yq_clist_open_item(GtkWidget * widget, gpointer data)
{
	YqTreeItem *openitem;
	
	if (strcmp(selectleaf->nodename,"a")==0)
	{	
		if (clist_selectrow!=-1)
		{
			openitem=(YqTreeItem *)
				       gtk_clist_get_row_data
				       (GTK_CLIST (yqlist.list),
					clist_selectrow);
			yq_tree_item_expand(YQ_TREE_ITEM(openitem->attree->tree_owner));
		}
	}
	else
	{
		if (!GTK_TREE_ITEM (selectleaf)->expanded)
			gtk_tree_item_expand (GTK_TREE_ITEM (selectleaf));
		if (clist_selectrow!=-1)
			openitem=(YqTreeItem *)
			       gtk_clist_get_row_data
				       (GTK_CLIST (yqlist.list),
					clist_selectrow);
	}
	if (clist_selectrow!=-1)
		yq_tree_select_child (openitem->attree,
				       GTK_WIDGET(openitem));
}

void
yq_clist_popup(GdkEvent *event)
{
	GtkWidget *menu, *menu_item;

	GdkEventButton *bevent = (GdkEventButton *) event; 

	menu = gtk_menu_new();
	
	menu_item = gtk_menu_item_new();
	gtk_menu_append(GTK_MENU(menu), menu_item);
	gtk_widget_show(menu_item);
	
	menu_item =	CreateMenuItem (menu, _("open"),NULL, 0,0,
				GTK_SIGNAL_FUNC (yq_clist_open_item), NULL);
	menu_item =	CreateMenuItem (menu, _("cut"), NULL, 0, 0,
				GTK_SIGNAL_FUNC (toolbar_cut), NULL);
	menu_item =	CreateMenuItem (menu, _("copy"), NULL, 0, 0,
				GTK_SIGNAL_FUNC (toolbar_copy), NULL);
	menu_item =	CreateMenuItem (menu, _("paste"), NULL, 0, 0,
				GTK_SIGNAL_FUNC (toolbar_paste), NULL);
	menu_item =	CreateMenuItem (menu, _("select all"), NULL, 0, 0,
				GTK_SIGNAL_FUNC (menu_edit_selectall_func), NULL);
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, bevent->button,bevent->time);
}

void
yq_clist_click_column (GtkCList * clist, gint column)
{
	gint lcolumn;
	gint rcolumn,i,m;

	yq_play_sound(3);

	rcolumn=0;
	for (i=0;i<=column;i++)
	{
		while (!titleshow[rcolumn]) rcolumn++;
		rcolumn++;
	}
	rcolumn--;
	if (selectleaf->sorttype!=-1)
	{
		lcolumn=(selectleaf->sorttype)/2;
		m=(selectleaf->sorttype)%2;
	}
	else
		lcolumn=-1;
	if (lcolumn==rcolumn)	
		yqlist.sorttype=rcolumn*2+(!m);
	else
		yqlist.sorttype=rcolumn*2+1;
	yq_clist_set_titles(yqlist.sorttype,selectleaf->sorttype);
	selectleaf->sorttype=yqlist.sorttype;
	yq_clist_resort();
}

void
yq_clist_set_titles(gint sorttype,gint oldsorttype)
{
	gint a,b,c,i;
	gchar tz[256],ti[4],*list_title;
	gboolean bl;
	
	if (sorttype!=-1)
	{
		a=sorttype/2;
		c=-1;
		for (i=0;i<=a;i++)
		{
			if (titleshow[i]) c++;
		}	
		b=sorttype%2;
		sprintf(ti,"%d",a+1);
		if (yq_cfg_read_string (rccfgfile, "title",ti, &list_title))
		{
			bl=TRUE;
		}
		else
		{
			bl=FALSE;
			list_title=_(title[a]);
		}
		if (b)
			sprintf(tz,_("%s +"),list_title);
		else
			sprintf(tz,_("%s -"),list_title);
		if (bl)
			g_free(list_title);
		gtk_clist_set_column_title(GTK_CLIST(yqlist.list), c,tz);
	}
	else
		a=-1;

	b=a;
	a=oldsorttype/2;
	if (a==b)
		return;
	c=-1;
	for (i=0;i<=a;i++)
	{
		if (titleshow[i]) c++;
	}	
	sprintf(ti,"%d",a+1);
	if (yq_cfg_read_string (rccfgfile, "title",ti, &list_title))
	{
		bl=TRUE;
	}
	else
	{
		bl=FALSE;
		list_title=_(title[a]);
	}
	gtk_clist_set_column_title(GTK_CLIST(yqlist.list), c,list_title);
	if (bl)
		g_free(list_title);
}

void
yq_clist_add_items(GList *list)
{
	GList *t_pos;
	gint row,i,n;
	YqTreeItem *leaf;
	gchar **strvalue,*ab,*a;
	gboolean bb;

	n = 0;
	for (i = 0; i <= 45; i++)
		if (titleshow[i])
			n++;
	strvalue = g_malloc (n * sizeof (gchar *));
	for (i = 0; i < n; i++)
		strvalue[i] = NULL;
	gtk_clist_freeze(GTK_CLIST(yqlist.list));	
	gtk_clist_clear(GTK_CLIST(yqlist.list));
	t_pos = g_list_first(list);
	while (t_pos != NULL)
	{
		leaf = t_pos -> data;
		if (strchr(leaf->nodename,'-'))
		{
			a = strchr (leaf->nodename, '-');
			*a = '\0';
			a++;
			yq_cfg_read_string (leaf->cfg,leaf->nodename,a, &ab);
			a--;
			*a= '-';
			bb = ((strstr((strchr(ab,',')+1),_("Female")))==(strchr(ab,',')+1));
			clist_readjilu (strvalue, ab);
			row = gtk_clist_append (GTK_CLIST (yqlist.list), strvalue);
			leaf_formattext (ab);
			if (bb)
				gtk_clist_set_pixtext (GTK_CLIST (yqlist.list), row,
						       0, ab, 1, yqpixmaps[GIRL],
						       yqmasks[GIRL]);
			else
				gtk_clist_set_pixtext (GTK_CLIST (yqlist.list), row,
						       0, ab, 1, yqpixmaps[BOY],
						       yqmasks[BOY]);
		}
		else
		{
			for (i = 0; i < n; i++)
				strvalue[i] = NULL;
			row = gtk_clist_append (GTK_CLIST (yqlist.list), strvalue);
			yq_cfg_read_string (leaf->cfg,leaf->nodename, "n", &ab);
			if (strcmp(leaf->nodename,"yq")==0)
				gtk_clist_set_pixtext (GTK_CLIST (yqlist.list), row, 0,
					       ab, 1,
					       yqpixmaps[YQFILE_CLOSE],
				 	      yqmasks[YQFILE_CLOSE]);
			else
			{	
				if (strcmp(leaf->nodename,"yq_0")==0)
				{
					gtk_clist_set_pixtext (GTK_CLIST (yqlist.list), row, 0,
					       ab, 1,
					       yqpixmaps[TRASH],
				 	      yqmasks[TRASH]);					
				}
				else
				{
					gtk_clist_set_pixtext (GTK_CLIST (yqlist.list), row, 0,
					       ab, 1,
					       yqpixmaps[ZU_CLOSE],
				 	      yqmasks[ZU_CLOSE]);
				}
			}

		}
		g_free (ab);
		gtk_clist_set_row_data (GTK_CLIST (yqlist.list), row, leaf);
				
		t_pos = t_pos -> next;
	}
	g_free (strvalue);
	gtk_clist_thaw(GTK_CLIST(yqlist.list));
	clist_selectrow = -1;
}

void
yq_clist_resort()
{
	GList *list,*t_pos;
	gint row,counter,*sel_rows,*sel_row,i,m;
	YqTreeItem *leaf,**selecteds,**selected;
	gboolean c;

	if (GTK_CLIST(yqlist.list) -> rows < 2) return;
	if (GTK_CLIST(yqlist.list) -> selection == NULL)
	{
		c = FALSE;
	}
	else
	{
		c= TRUE;
		m=0;
		list=GTK_CLIST(yqlist.list) -> selection;
		while (list)
		{
			m++;
			list=list->next;
		}
		selecteds= g_malloc (m * sizeof (YqTreeItem *));
		selected=selecteds;
		sel_rows= g_malloc (m * sizeof (gint));
		list=GTK_CLIST(yqlist.list) -> selection;
		while (list)
		{
			*selected=(YqTreeItem *)gtk_clist_get_row_data(GTK_CLIST(yqlist.list),(gint)list->data);
			list=list->next;
			selected++;
		}
	}

	list = NULL;
	for (row = 0; row < GTK_CLIST(yqlist.list) -> rows; row++)
	{
		leaf = (YqTreeItem *)gtk_clist_get_row_data(GTK_CLIST(yqlist.list), row);
		list = g_list_insert_sorted(list, leaf, yq_list_get_sort_func());
	}
	
	if (c)
	{
		t_pos = g_list_first(list);
		counter = 0;
		sel_row=sel_rows;
		while (t_pos != NULL)
		{
			leaf = t_pos -> data;
			selected=selecteds;
			for (i=0;i<m;i++)
			{
				if (leaf==*selected)
				{
					*sel_row=counter;
					sel_row++;
					break;
				}
				else
				{
					selected++;
				}
			}
			t_pos = t_pos -> next;
			counter++;
		}
	}
	yq_clist_add_items(list);
	g_list_free(list);
	if (c)
	{
		sel_row=sel_rows;
		for (i=0;i<m;i++)
		{
			gtk_clist_select_row (GTK_CLIST (yqlist.list),*sel_row, 0);
			sel_row++;
		}
		g_free(sel_rows);
		g_free(selecteds);
	}
}

GCompareFunc
yq_list_get_sort_func()
{
	return (GCompareFunc)yq_cmp_ascend_by_name;
}

gint
yq_cmp_ascend_by_name(YqTreeItem *c1, YqTreeItem *c2)
{
	gchar *a1,*a2,*b1,*b2,*c;
	gint jilu,accend,i,value;

	if (yqlist.sorttype==-1)
		return 1;
	accend=yqlist.sorttype%2;
	value=-1;
	if ((strchr(c1->nodename,'-'))&&(strchr(c2->nodename,'-')))
	{
		b1 = strchr (c1->nodename, '-');
		*b1 = '\0';
		b1++;
		yq_cfg_read_string (c1->cfg,c1->nodename,b1, &a1);
		b1--;
		*b1='-';
		jilu=yqlist.sorttype/2;
		b1=a1;
		for (i=0;i<jilu;i++)
		{
			while (*b1!=',') b1++;
			b1++;
		}
		c=b1;
		while (*c!=',') c++;
		*c='\0';

		b2 = strchr (c2->nodename, '-');
		*b2 = '\0';
		b2++;
		yq_cfg_read_string (c2->cfg,c2->nodename,b2, &a2);
		b2--;
		*b2='-';
		b2=a2;
		for (i=0;i<jilu;i++)
		{
			while (*b2!=',') b2++;
			b2++;
		}
		c=b2;
		while (*c!=',') c++;
		*c='\0';
		if (accend) 
			i=strcmp(b1, b2);
		else
			i=strcmp(b2, b1);
		g_free(a1);
		g_free(a2);
		value=i;
		return value;
	}
	if ((!strchr(c1->nodename,'-'))&&(!strchr(c2->nodename,'-')))
	{
		if (strcmp(c1->nodename,"yq_0")==0)
			return -1;
		if (strcmp(c2->nodename,"yq_0")==0)
			return 1;
		yq_cfg_read_string (c1->cfg,c1->nodename,"n", &a1);
		yq_cfg_read_string (c2->cfg,c2->nodename,"n", &a2);
		if (accend) 
			i=strcmp(a1,a2);
		else
			i=strcmp(a2,a1);
		g_free(a1);
		g_free(a2);
		value=i;
		return value;		
	}
	if (strchr(c1->nodename,'-')) value=1;
	return value;
}

void
createclist ()
{
	gint i, n, t, k;
	gchar *list_title, ti[4];
	gboolean bl;
	/*GtkTooltips *tooltips;*/

	n = 0;
	t = 0;
	for (i = 0; i <= 45; i++)
		if (titleshow[i])
			n++;
	yqlist.list = gtk_clist_new (n);
	for (i = 0; i < n; i++)
	{
		while (!titleshow[t])
			t++;
		sprintf (ti, "%d", t + 1);
		if (yq_cfg_read_string (rccfgfile, "title", ti, &list_title))
		{
			bl=TRUE;
		}
		else
		{
			bl=FALSE;
			list_title=_(title[t]);
		}
		gtk_clist_set_column_title (GTK_CLIST (yqlist.list), i,
					    list_title);
		if (bl)
			g_free (list_title);
		yq_cfg_read_int (rccfgfile, "titlewidth", ti, &k);
		if (k)
			gtk_clist_set_column_width (GTK_CLIST (yqlist.list),
						    i, k);
		else
			gtk_clist_set_column_auto_resize (GTK_CLIST
							  (yqlist.list), i,
							  1);
		t++;
	}
	gtk_clist_set_selection_mode (GTK_CLIST (yqlist.list),
				      GTK_SELECTION_EXTENDED);
	gtk_widget_show (yqlist.list);
	gtk_clist_column_titles_show (GTK_CLIST (yqlist.list));
	gtk_signal_connect (GTK_OBJECT (yqlist.list),
			    "select_row",
			    GTK_SIGNAL_FUNC (yq_clist_select_row), NULL);
	gtk_signal_connect (GTK_OBJECT (yqlist.list),
			    "button_press_event",
			    GTK_SIGNAL_FUNC (yq_clist_button_press), NULL);
	gtk_signal_connect (GTK_OBJECT (yqlist.list),
			    "key_press_event",
			    GTK_SIGNAL_FUNC (yq_clist_key_press), NULL);
	gtk_signal_connect (GTK_OBJECT (yqlist.list),
			    "click_column",
			    GTK_SIGNAL_FUNC (yq_clist_click_column), NULL);

	yqlist.listwin = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (yqlist.listwin),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (yqlist.listwin),
			   GTK_WIDGET (yqlist.list));
	gtk_box_pack_start (GTK_BOX (yqlist.box), yqlist.listwin, TRUE, TRUE,
			    0);
	gtk_widget_show (yqlist.listwin);
	/*tooltips=gtk_tooltips_new();
	gtk_tooltips_enable(tooltips);
	gtk_tooltips_set_tip(tooltips,yqlist.list,"sdfs",NULL);*/

	yqlist.type = LIST_DETAILS;
}

void
clist_readjilu (gchar ** strvalue, gchar * ab)
{
	gint i, n;
	gchar *p;

	n = 0;
	p = strtok (ab, ",");
	for (i = 1; p ; i++)
	{
		p = strtok (NULL, ",");
		if ((titleshow[i])&&(p))
		{
			n++;
			strvalue[n] = leaf_formattext (p);
		}
	}
}

void
clist_openzu (YqTreeItem * yqitem)
{
	YqTreeItem *leaf;
	GList *list;
	gint row,rows;

	if ((strcmp(yqitem->nodename,"yq")==0)||(strcmp(yqitem->nodename,"r")==0))
		yq_play_sound(5);
	else
		yq_play_sound(3);

	yq_clist_set_titles(yqitem->sorttype,yqlist.sorttype);
	list = NULL;
	yqlist.sorttype=yqitem->sorttype;
	rows=g_list_length(GTK_TREE(GTK_TREE_ITEM(yqitem)->subtree)->children);
	for (row = 0; row < rows; row++)
	{
		leaf = (YqTreeItem *) g_list_nth(GTK_TREE(GTK_TREE_ITEM(yqitem)->subtree)->children,row)->data;
		list = g_list_insert_sorted(list, leaf, yq_list_get_sort_func());
	}
	yq_clist_add_items(list);
	g_list_free(list);
}
