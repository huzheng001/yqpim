#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "intl.h"

#include <gtk/gtk.h>
#include "configfile.h"
#include "yqtreeitem.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>


gint *imindexes,imtitlecont;

extern gchar sFilename[256];
extern GtkWidget *tree_main;
extern gint yesnoreturn;
extern YqTreeItem *selectleaf, *root_leaf;
extern gchar *title[];
extern struct _YQList yqlist;
extern ConfigFile *rccfgfile;
extern gint clist_selectrow;
extern gboolean yq_nowiszu;


void yq_cfg_clean_sections (ConfigFile * cfg, gchar * section);
void yq_play_sound(gint i);
void GetFilename (char *sTitle);
void YesNo (gchar * szMessage);
void Areyousure (gchar * szMessage);
void Popup (gchar * szMessage);
GtkWidget *yq_createnulltree ();
gchar *leaf_formattext (gchar * bd);
YqTreeItem *yq_tree_search_next(YqTreeItem *nowleaf);
void toolbar_addjilu (YqTreeItem *nowleaf,gchar * ab,gboolean freeze,gboolean expand);
void toolbar_clean_trash(YqTreeItem *trashitem,gboolean ask);
void yq_tree_select_item (GtkTree *tree, gint item);
void yq_tree_select_child(GtkTree *tree, GtkWidget *tree_item);
void yq_tree_item_expand(YqTreeItem * item);
void yq_export_csv_item(YqTreeItem *leaf,FILE *file);
void yq_export_html_head(FILE * htmlfile,gchar *title);
void yq_export_html_middle(FILE *htmlfile,gchar *caption);
void yq_export_html_end(FILE * htmlfile);
void yq_export_html_leaf(FILE *htmlfile,ConfigFile * cfg,gchar *zuname,gchar *leafname);
void yq_export_html_item(YqTreeItem *exportitem);
void yq_export_html_rootitem(gchar *dirname);
void yq_export_html_allitem();
void yq_export_html_all_selection();
void yq_export_html_zu(ConfigFile * cfg,gchar *zuname,gchar *dirname,gchar *upname);
void yq_export_html_selection(ConfigFile * cfg,gchar *zuname,gchar *dirname);
void yq_import_csv_format(gchar *ab,gchar *tmp);
void yq_import_csv_index(gint n,gchar **imtitles,gint *inindexes);
void zu_expanded (GtkWidget * widget, gpointer data);
void zu_collapsed (GtkWidget * widget, gpointer data);
void open_zu (YqTreeItem * yqitem);
GtkWidget *yq_readzu (YqTreeItem * upleaf);


void
menu_file_close_func (GtkWidget * widget, gpointer data)
{
	gint n;
	YqTreeItem *closeitem;

	if (strlen(selectleaf->nodename) == 1)
		return;

	if (selectleaf->cfg->changed)
	{
		YesNo (_("It is changed,save it?"));
		if (yesnoreturn == 2)
			return;
		if (yesnoreturn == 1)
		{
			yq_cfg_write_file (selectleaf->cfg,
					   selectleaf->cfg->cfgfilename);
		}
	}

	n = g_list_length(GTK_TREE(tree_main)->children);
	closeitem=selectleaf;
	while (closeitem->attree != GTK_TREE (tree_main))
		closeitem = YQ_TREE_ITEM (closeitem->attree->tree_owner);
	if (strcmp(selectleaf->nodename,"yq"))
	{
		yq_tree_select_child(closeitem->attree,GTK_WIDGET(closeitem));
		gtk_widget_grab_focus (GTK_WIDGET(closeitem));
	} /* if don't do so,it will core when only one file open or warning when more than one file open as it is not selecting the file leaf),
		     i think it is gtk+ 's bug. */

	yq_cfg_free (selectleaf->cfg);
	g_free (selectleaf->cfg);

	yq_tree_remove_item (tree_main, GTK_WIDGET (closeitem));
	yq_tree_select_item (GTK_TREE (tree_main)->root_tree, 1);
}


void
menu_file_saveas_func (GtkWidget * widget, gpointer data)
{
	struct stat st;

	GetFilename (_("Save as..."));
	if (sFilename[0] == '\0')
		return;
	if (sFilename[strlen(sFilename)-1] == '/')
	{
		Popup(_("Please type the file name"));
		return;
	}
	if (!stat (sFilename, &st))
	{
		Areyousure (_("This file is exist,overwrite?"));
		if (!yesnoreturn)
			return;
	}
	yq_cfg_write_file (selectleaf->cfg, sFilename);
}

void
menu_file_exporthtml_func(GtkWidget *widget, gpointer data)
{
	struct stat st;

	if (strcmp(selectleaf->nodename,"a")==0)
	{
		if (clist_selectrow==-1)
			yq_export_html_allitem();
		else
			yq_export_html_all_selection();
		return;
	}	
	if (!yq_nowiszu)
	{
		yq_export_html_item(selectleaf);
	}
	else
	{
		GetFilename (_("Export to html,please choose the directory"));
		if (sFilename[0]=='\0')
			return;
		if (sFilename[strlen(sFilename)-1] != '/')
		{
			Popup(_("This is't a directory,it should end with \"/\""));
			return;
		}
		if (stat (sFilename, &st))
		{
			mkdir(sFilename,755);
		}
		if (strcmp(selectleaf->nodename,"r")==0)
		{
			yq_export_html_rootitem(sFilename);
			return;
		}
		if (clist_selectrow == -1)
			yq_export_html_zu(selectleaf->cfg,selectleaf->nodename,sFilename,NULL);
		else
			yq_export_html_selection(selectleaf->cfg,selectleaf->nodename,sFilename);
	}
}

void
yq_export_html_head(FILE * htmlfile,gchar *title)
{
	fprintf(htmlfile,_("<html>\n<head>\n<meta http-equiv=\"Content-Type\"\ncontent=\"text/html; charset= \">\n<meta name=\"GENERATOR\" content=\"huzheng--yqpim\">\n<title>"));
	fprintf(htmlfile,title);
	fprintf(htmlfile,"</title>\n</head>\n<body>\n");	
}

void
yq_export_html_middle(FILE *htmlfile,gchar *caption)
{
	gint i;
	gchar *ab,ti[5];
	
	fprintf(htmlfile,"<table border=1>\n<caption align=top>");
	fprintf(htmlfile,caption);
	fprintf(htmlfile,"</caption>\n");
	fprintf(htmlfile,"<tr bgColor=#a9d0c4>\n");
	for (i=0;i<46;i++)
	{
		fprintf(htmlfile,"    <td>");
		sprintf(ti,"%d",i+1);
		if (yq_cfg_read_string (rccfgfile, "title", ti, &ab))
		{
			fprintf(htmlfile,ab);
			g_free(ab);
		}
		else
			fprintf(htmlfile,_(title[i]));
		fprintf(htmlfile,"</td>\n");
	}
	fprintf(htmlfile,"</tr>\n");
}

void
yq_export_html_end(FILE * htmlfile)
{
	fprintf(htmlfile,"</table>\n</body>\n</html>\n");
	fclose(htmlfile);
}

void
yq_export_html_leaf(FILE *htmlfile,ConfigFile * cfg,gchar *zuname,gchar *leafname)
{
	gint i;
	gchar *aa,*ab,*bb,*cc;
	
	fprintf(htmlfile,"<tr>\n");
	yq_cfg_read_string (cfg, zuname, leafname, &ab);
	aa=ab;
	for (i=0;i<46;i++)
	{
		bb=aa;
		if (i!=45)
		{
			while (*aa!=',') aa++;
			*aa='\0';
		}
		cc=leaf_formattext(bb);
		fprintf(htmlfile,"    <td>");
		if (strlen(cc)==0)
			fprintf(htmlfile,"&nbsp");
		else
			fprintf(htmlfile,cc);
		fprintf(htmlfile,"</td>\n");
		aa++;				
	}
	g_free(ab);
	fprintf(htmlfile,"</tr>\n");
}

void
yq_export_html_allitem()
{
	struct stat st;
	FILE *htmlfile;
	YqTreeItem *newleaf;	
	gchar *a;

	GetFilename (_("Export to html file"));
	if (sFilename[strlen(sFilename)-1] == '/')
	{
		Popup(_("Please type the html file name"));
		return;
	}
	if (!stat (sFilename, &st))
	{
		Areyousure (_("This file is exist,overwrite?"));
		if (!yesnoreturn)
			return;
	}
	if ((htmlfile = fopen (sFilename, "w")) == NULL)
		return;
	yq_export_html_head(htmlfile,_("All Contacts"));
	yq_export_html_middle(htmlfile,_("All Contacts"));
	newleaf=yq_tree_search_next(root_leaf);	
	while (newleaf!=root_leaf)
	{
		a = strchr (newleaf->nodename, '-');
		if (a)
		{
			*a = '\0';
			a++;
			yq_export_html_leaf(htmlfile,newleaf->cfg,newleaf->nodename,a);
			a--;
			*a='-';
		}
		newleaf=yq_tree_search_next(newleaf);
	}	
	yq_export_html_end(htmlfile);		
}
void
yq_export_html_rootitem(gchar *dirname)
{
	GList *list;
	struct stat st;
	FILE *htmlfile;
	gchar newdirname[512],*ab;
	gint i;

	sprintf(newdirname,"%sindex.html",dirname);
	if ((htmlfile = fopen (newdirname, "w")) == NULL)
		return;
	yq_export_html_head(htmlfile,_("yqpim"));
	
	list = gtk_container_children (GTK_CONTAINER (tree_main));
	i=0;
	while (list)
	{
		i++;
		sprintf(newdirname,"%syq%d/",dirname,i);
		if (stat (newdirname, &st))
		{
			mkdir(newdirname,755);
		}
		yq_export_html_zu(YQ_TREE_ITEM(list->data)->cfg,"yq",newdirname,"../index");
		sprintf(newdirname,"<a href=\"yq%d/yq.html\">",i);
		fprintf(htmlfile,newdirname);
		yq_cfg_read_string (YQ_TREE_ITEM(list->data)->cfg, "yq","n", &ab);
		fprintf(htmlfile,ab);
		g_free(ab);
		fprintf(htmlfile,"</a><br>\n");
		list = g_list_remove_link (list, list);
	}
	yq_export_html_end(htmlfile);
}

void
yq_export_html_item(YqTreeItem *exportitem)
{
	struct stat st;
	FILE *htmlfile;
	gchar *ab,*bb;

	GetFilename (_("Export to html file"));
	if (sFilename[0]=='\0')
		return;
	if (sFilename[strlen(sFilename)-1] == '/')
	{
		Popup(_("Please type the html file name"));
		return;
	}
	if (!stat (sFilename, &st))
	{
		Areyousure (_("This file exists,overwrite?"));
		if (!yesnoreturn)
			return;
	}
	if ((htmlfile = fopen (sFilename, "w")) == NULL)
		return;
	bb = strchr(exportitem->nodename,'-');
	*bb = '\0';
	bb++;
	yq_cfg_read_string (exportitem->cfg,exportitem->nodename,"n", &ab);
	yq_export_html_head(htmlfile,ab);
	yq_export_html_middle(htmlfile,ab);
	g_free(ab);		
	yq_export_html_leaf(htmlfile,exportitem->cfg,exportitem->nodename,bb);
	bb--;
	*bb='-';
	yq_export_html_end(htmlfile);	
}

void
yq_export_html_zu(ConfigFile * cfg,gchar *zuname,gchar *dirname,gchar *upname)
{
	FILE *htmlfile;
	gint i,z,a;
	gchar ti[4],*ab,*aa,tz[256],filename[512];

	sprintf(filename,"%s%s.html",dirname,zuname);
	if ((htmlfile = fopen (filename, "w")) == NULL)
			return;
	yq_cfg_read_string (cfg, zuname,"n", &aa);
	yq_export_html_head(htmlfile,aa);
	if (upname)
	{
		fprintf(htmlfile,"<a href=\"");
		fprintf(htmlfile,upname);
		fprintf(htmlfile,".html\">");
		fprintf(htmlfile,"..</a><br>\n");
	}
	yq_cfg_read_int (cfg, zuname, "z", &z);
	for (i=1;i<=z;i++)
	{
		sprintf(tz,"%s_%d",zuname,i);
		yq_export_html_zu(cfg,tz,dirname,zuname);
		fprintf(htmlfile,"<a href=\"");
		fprintf(htmlfile,tz);
		fprintf(htmlfile,".html\">");
		yq_cfg_read_string (cfg, tz,"n", &ab);
		fprintf(htmlfile,ab);
		g_free(ab);
		fprintf(htmlfile,"</a><br>\n");
	}
	yq_export_html_middle(htmlfile,aa);
	g_free(aa);
	yq_cfg_read_int (cfg, zuname, "a", &a);
	for (i=1;i<=a;i++)
	{
		sprintf(ti,"%d",i);
		yq_export_html_leaf(htmlfile,cfg,zuname,ti);
	}
	yq_export_html_end(htmlfile);
}

void
yq_export_html_all_selection()
{
	struct stat st;
	FILE *htmlfile;
	YqTreeItem *exportitem;	
	GList *list;
	gchar *aa;

	GetFilename (_("Export to html file"));
	if (sFilename[strlen(sFilename)-1] == '/')
	{
		Popup(_("Please type the html file name"));
		return;
	}
	if (!stat (sFilename, &st))
	{
		Areyousure (_("This file exists,overwrite?"));
		if (!yesnoreturn)
			return;
	}
	if ((htmlfile = fopen (sFilename, "w")) == NULL)
		return;
	yq_export_html_head(htmlfile,_("All Contacts"));
	yq_export_html_middle(htmlfile,_("All Contacts"));
	list=GTK_CLIST(yqlist.list) -> selection;
	while (list!=NULL)
	{			
		exportitem = (YqTreeItem *)gtk_clist_get_row_data(GTK_CLIST(yqlist.list), (gint) list -> data);
		aa=strchr (exportitem->nodename, '-');
		*aa='\0';
		aa++;
		yq_export_html_leaf(htmlfile,exportitem->cfg,exportitem->nodename,aa);
		aa--;
		*aa='-';
		list=list->next;
	}	
	yq_export_html_end(htmlfile);	
}

void
yq_export_html_selection(ConfigFile * cfg,gchar *zuname,gchar *dirname)
{
	FILE *htmlfile;
	gchar *ab,*aa,filename[512];
	GList *list;
	YqTreeItem *exportitem;

	sprintf(filename,"%s%s.html",dirname,zuname);
	if ((htmlfile = fopen (filename, "w")) == NULL)
			return;
	yq_cfg_read_string (cfg, zuname,"n", &aa);
	yq_export_html_head(htmlfile,aa);

	
	list=GTK_CLIST(yqlist.list) -> selection;
	while (list!=NULL)
	{			
		exportitem = (YqTreeItem *)gtk_clist_get_row_data(GTK_CLIST(yqlist.list), (gint) list -> data);
		if (!strchr (exportitem->nodename, '-'))
		{
			yq_export_html_zu(cfg,exportitem->nodename,dirname,zuname);
			fprintf(htmlfile,"<a href=\"");
			fprintf(htmlfile,exportitem->nodename);
			fprintf(htmlfile,".html\">");
			yq_cfg_read_string (cfg, exportitem->nodename,"n", &ab);
			fprintf(htmlfile,ab);
			g_free(ab);
			fprintf(htmlfile,"</a><br>\n");
		}
		list=list->next;
	}	
	yq_export_html_middle(htmlfile,aa);
	g_free(aa);
	
	list=GTK_CLIST(yqlist.list) -> selection;
	while (list!=NULL)
	{			
		exportitem = (YqTreeItem *)gtk_clist_get_row_data(GTK_CLIST(yqlist.list), (gint) list -> data);
		aa=strchr (exportitem->nodename, '-');
		if (aa)
		{
			*aa='\0';
			aa++;
			yq_export_html_leaf(htmlfile,cfg,exportitem->nodename,aa);
			aa--;
			*aa='-';
		}
		list=list->next;
	}	
	yq_export_html_end(htmlfile);	
}

void
menu_file_exportcsv_func(GtkWidget *widget, gpointer data)
{
	struct stat st;
	FILE *csvfile;
	gint i;
	GList *list;
	YqTreeItem *leaf;
	gchar *ab,ti[5];

	GetFilename (_("Export to csv file"));
	if (sFilename[0] == '\0')
		return;
	if (sFilename[strlen(sFilename)-1] == '/')
	{
		Popup(_("Please type the csv file name"));
		return;
	}
	if (!stat (sFilename, &st))
	{
		Areyousure (_("This file exists,overwrite?"));
		if (!yesnoreturn)
			return;
	}
	if ((csvfile = fopen (sFilename, "w")) == NULL)
			return;
	for (i=0;i<45;i++)
	{
		sprintf(ti,"%d",i+1);
		if (yq_cfg_read_string (rccfgfile, "title", ti, &ab))
		{
			fprintf(csvfile,ab);
			g_free(ab);
		}
		else
			fprintf(csvfile,_(title[i]));
		fprintf(csvfile,",");
	}
	if (yq_cfg_read_string (rccfgfile, "title", "46", &ab))
	{
		fprintf(csvfile,ab);
		g_free(ab);
	}
	else
		fprintf(csvfile,_(title[45]));
	fprintf(csvfile,"\n");
	
	if (!yq_nowiszu)
	{
		yq_export_csv_item(selectleaf,csvfile);
	}
	else
	{
		if (clist_selectrow == -1)
		{
			if (strcmp(selectleaf->nodename,"a")==0)
			{				
				for (i=0;i<GTK_CLIST(yqlist.list) -> rows;i++)
				{
					leaf = (YqTreeItem *)gtk_clist_get_row_data(GTK_CLIST(yqlist.list), i);
					yq_export_csv_item(leaf,csvfile);
				}
			}
			else
			{
				list = gtk_container_children (GTK_CONTAINER (GTK_TREE_ITEM(selectleaf)->subtree));
				while (list)
				{
					yq_export_csv_item(YQ_TREE_ITEM(list->data),csvfile);
					list = g_list_remove_link (list, list);
				}
			}
		}
		else
		{			
			list=GTK_CLIST(yqlist.list) -> selection;
			while (list!=NULL)
			{
				leaf = (YqTreeItem *)gtk_clist_get_row_data(GTK_CLIST(yqlist.list), (gint) list -> data);
				yq_export_csv_item(leaf,csvfile);
				list=list->next;
			}
		}
	}
	fclose(csvfile);
}

void
yq_export_csv_item(YqTreeItem *leaf,FILE *file)
{
	gint i,n;
	gchar *ab,*a,*b,*c,*d,tz[255];

	
	a = strchr (leaf->nodename, '-');
	if (!a) return;
	*a ='\0';
	a++;
	strcpy(tz,a);
	yq_cfg_read_string (leaf->cfg, leaf->nodename, tz, &ab);
	a--;
	*a='-';
	a=ab;
	for (i=0;i<46;i++)
	{
		b=a;
		if (i!=45)
		{
			while (*a!=',') a++;
			*a='\0';
		}
		c=leaf_formattext(b);
		d=c;
		while (*d!='\0')
		{
			if (*d=='\"') *d='\'';  /* the " can't in csv file*/
				d++;
		}
		if ((strchr(c,','))||(strchr(c,'\n'))) 
			n=1;
		else
			n=0;
		if (n) fprintf(file,"\"");
		fprintf(file,c);
		if (n) fprintf(file,"\"");
			if (i!=45) fprintf(file,",");
		a++;				
	}
	fprintf(file,"\n");
	g_free(ab);
}

void
menu_edit_selectall_func(GtkWidget *widget, gpointer data)
{
	gint row;
	
	if (yq_nowiszu)
	{
		for (row = 0; row < GTK_CLIST(yqlist.list) -> rows; row++)
		{
			gtk_clist_select_row(GTK_CLIST(yqlist.list),row,0);
		}
	}
}

void
menu_edit_selectother_func(GtkWidget *widget, gpointer data)
{
	GList *list;
	gint i,j,n,*rows,*row;
	gboolean c;
	
	if (!yq_nowiszu) return;
	list=GTK_CLIST(yqlist.list) -> selection;
	if (!list)
	{
		menu_edit_selectall_func(NULL,NULL);
		return;
	}
	n=0;
	while (list)
	{
		n++;
		list=list->next;
	}
	list=GTK_CLIST(yqlist.list) -> selection;
	rows= g_malloc (n * sizeof (gint));
	row=rows;
	while (list)
	{
		*row=(gint)list->data;
		list=list->next;
		row++;
	}
	for (i=0;i<GTK_CLIST(yqlist.list) -> rows;i++)
	{
		row=rows;
		c=FALSE;
		for (j=0;j<n;j++)
		{
			if (i==*row)
			{
				c=TRUE;
				break;
			}
			else
			{
				row++;
			}
		}
		if (c)
			gtk_clist_unselect_row(GTK_CLIST(yqlist.list),i,0);
		else
			gtk_clist_select_row(GTK_CLIST(yqlist.list),i,0);
	}
	g_free(rows);
}


void
menu_file_importcsv_func(GtkWidget *widget, gpointer data)
{
	struct stat stats;
	FILE *csvfile;
	gint i,n,*imindex;
	gchar *buffer,*a,*b,*c,*tmp1,*tmp2,*tmp3,**imtitles,**imtitle,*tmpindex[46];
	gboolean m;


	if (strlen(selectleaf->nodename) == 1)
	{
		Popup(_("please choose yqfile"));
		return;
	}

	GetFilename(_("Import csv file"));
	if (sFilename[0]=='\0') return;
	if (sFilename[strlen(sFilename)-1] == '/')
	{
		Popup(_("Please type the csv file name"));
		return;
	}
	if (lstat(sFilename,&stats)==-1) return;
	if (!(csvfile=fopen(sFilename,"r"))) return;
	buffer=g_malloc(stats.st_size+1);
	if(fread(buffer,1,stats.st_size,csvfile)!=stats.st_size)
	{
		g_free(buffer);
		fclose(csvfile);
		return;
	}
	fclose(csvfile);
	buffer[stats.st_size]='\0';
	a=buffer;
	n=0;
	while (*a!='\n')
	{
		if (*a==',') n++;
		a++;
	}
	*a='\0';
	n++;
	imtitlecont=n;
	imtitles=g_malloc(n* sizeof(gchar *));
	imtitle=imtitles;
	b=buffer;
	*imtitle=b;
	for (i=1;i<n;i++)
	{
		while (*b!=',') b++;
		*b='\0';
		b++;
		imtitle++;
		*imtitle=b;
	}
	imindexes=g_malloc(n* sizeof(gint));
	imindex=imindexes;
	if (n>=46)
	{
		for (i=0;i<n;i++)
		{
			if (i<=45)
				*imindex=i;
			else
				*imindex=-1;
			imindex++;			
		}
	}
	else
	{
		for (i=0;i<n;i++)
		{
			*imindex=i;
			imindex++;
		}
		for (i=n;i<46;i++)
		{
			*imindex=-2;
			imindex++;
		}
	}
	yq_import_csv_index(n,imtitles,imindexes);
	if (!yesnoreturn) 
	{
		g_free(imtitles);
		g_free(buffer);
		g_free(imindexes);
		return;
	}
	a++;
	tmp1=g_malloc(10000);
	tmp2=g_malloc(5000);
	while (*a!='\0')
	{
		while ((*a=='\n')||(*a=='\r')) a++;
		*tmp1='\0';
		*tmp2='\0';
		imindex=imindexes;
		for (i=0;i<46;i++)
		{
			tmpindex[i]=NULL;
		}
		for (i=0;i<(n-1);i++)
		{			
			b=a;
			if (*b=='"')
			{
				c=strstr(a,"\",");
				*c='\0';
				if (*imindex!=-1)
				{
					if (*imindex!=-2)
					{
						tmpindex[*imindex]=g_malloc(2*(c-b)+1);
						yq_import_csv_format(b,tmpindex[*imindex]);
					}
				}
				else
				{
					tmp3=g_malloc(2*(c-b)+1);
					yq_import_csv_format(b,tmp3);
					strcat(tmp2,"~n");
					strcat(tmp2,*(imtitles+i));
					strcat(tmp2,"=");
					strcat(tmp2,tmp3);
					g_free(tmp3);
				}				
				c++;
			}
			else
			{
				if (*b==',')
				{
					c=b;
				}
				else
				{
					c=strchr(b,',');
					*c='\0';
					if (*imindex!=-1)
					{
						if (*imindex!=-2)
						{
							tmpindex[*imindex]=g_malloc(2*(c-b)+1);
							yq_import_csv_format(b,tmpindex[*imindex]);
						}
					}
					else
					{
						tmp3=g_malloc(2*(c-b)+1);
						yq_import_csv_format(b,tmp3);
						strcat(tmp2,"~n");
						strcat(tmp2,*(imtitles+i));
						strcat(tmp2,"=");
						strcat(tmp2,tmp3);
						g_free(tmp3);
					}				
				}	
			}
			c++;
			a=c;
			imindex++;
		}
		b=a;		
		if (*b=='\n')
		{
			m=0;
			c=b;
		}
		else
		{
			if (*b=='\"')
			{
				b++;
				c=strstr(b,"\"\n");
				m=1;
			}
			else
			{
				c=strchr(b,'\n');
				m=0;
			}
			*c='\0';
			if (*(imindexes+n-1)>=0)
			{
				tmpindex[*(imindexes+n-1)]=g_malloc(2*(c-b)+1);
				yq_import_csv_format(b,tmpindex[*(imindexes+n-1)]);
			}
			else
			{
				if (*(imindexes+n-1)==-1)
				{
					tmp3=g_malloc(2*(c-b)+1);
					yq_import_csv_format(b,tmp3);
					strcat(tmp2,"~n");
					strcat(tmp2,*(imtitles+i));
					strcat(tmp2,"=");
					strcat(tmp2,tmp3);
					g_free(tmp3);
				}
			}
		}
		for (i=0;i<45;i++)
		{
			if (tmpindex[i])
			{
				strcat(tmp1,tmpindex[i]);
				strcat(tmp1,",");
				g_free(tmpindex[i]);
			}
			else
				strcat(tmp1,"~0,");
		}
		if (tmpindex[45])
		{
			strcat(tmp1,tmpindex[45]);
			g_free(tmpindex[45]);
		}
		else
		{
			if (tmp2[0]=='\0')
				strcat(tmp1,"~0");
		}
		strcat(tmp1,tmp2);
		toolbar_addjilu (selectleaf,tmp1,TRUE,FALSE);
		if (m) c++;
		c++;
		a=c;
		while ((*a=='\n')||(*a=='\r')) a++;
	}
	g_free(tmp1);
	g_free(tmp2);
	g_free(imtitles);
	g_free(imindexes);
	g_free(buffer);
	if (yq_nowiszu)
		open_zu (selectleaf);
}

void
yq_import_csv_format(gchar *ab,gchar *tmp)
{
	gchar *a,*c;
	
	a=ab;
	c=tmp;
	while (*a!='\0')
	{
		switch (*a)
		{
			case '~':
			{
				*c='~';
				c++;
				*c='~';
				break;
			}
			case '\n':
			{
				*c='~';
				c++;
				*c='n';
				break;
			}
			case '\r':
			{
				c--;
				break;
			}
			case ',':
			{
				*c='~';
				c++;
				*c='d';
				break;
			}
			default:
			{
				*c=*a;
				break;
			}	
		}
		c++;
		a++;
	}
	*c='\0';
}

void
menu_tool_trash_open_func (GtkWidget * widget, gpointer data)
{
	YqTreeItem *trashitem,*openitem;
	gint n;
	GList *children;
	gchar *ab;


	n = g_list_length(GTK_TREE(tree_main)->children);
	if (n==0)
		return;
	if ((strlen(selectleaf->nodename) == 1)&&(n>1))
	{
		Popup(_("Please select the yqpim file"));
		return;
	}	
	if (n==1)
	{
		openitem=YQ_TREE_ITEM(GTK_TREE(tree_main)->children->data);
	}
	else
	{
		openitem=selectleaf;
		while (openitem->attree != GTK_TREE (tree_main))
			openitem = YQ_TREE_ITEM (openitem->attree->tree_owner);
	}
	children = gtk_container_children (GTK_CONTAINER (GTK_TREE_ITEM(openitem)->subtree));
    while (children)
	{
		trashitem=YQ_TREE_ITEM (children->data);
		if (strcmp(trashitem->nodename,"yq_0")==0)
		{
			g_list_free(children);
			yq_tree_item_expand(openitem);
			yq_tree_select_child (trashitem->attree,GTK_WIDGET(trashitem));
			return;
		}
    	children = g_list_remove_link (children, children);
	}
	n=yq_cfg_read_string(openitem->cfg, "yq_0", "n", &ab);
	if (n!=0)
	{
		if (strlen(ab)==0)
			n=0;
		g_free(ab);
	}
	if (n==0)
	{
		yq_cfg_write_string(openitem->cfg,"yq_0","n",_("Trash"));
	}
	yq_cfg_read_string(openitem->cfg,"yq_0","n",&ab);
	trashitem = YQ_TREE_ITEM (yq_tree_item_new (ab, TRASH));
	g_free (ab);
	trashitem->attree = GTK_TREE (GTK_TREE_ITEM(openitem)->subtree);
	trashitem->cfg = openitem->cfg;
	strcpy (trashitem->nodename,"yq_0");
	gtk_signal_connect (GTK_OBJECT (trashitem), "expand",
			    GTK_SIGNAL_FUNC (zu_expanded), NULL);
	gtk_signal_connect (GTK_OBJECT (trashitem), "collapse",
			    GTK_SIGNAL_FUNC (zu_collapsed), NULL);
	gtk_tree_prepend (GTK_TREE(GTK_TREE_ITEM(openitem)->subtree), GTK_WIDGET (trashitem));
	gtk_tree_item_set_subtree (GTK_TREE_ITEM (trashitem),
					   yq_readzu (trashitem));
	yq_tree_item_expand(openitem);				
	yq_tree_select_child (trashitem->attree,GTK_WIDGET(trashitem));
}

void
menu_tool_trash_close_func (GtkWidget * widget, gpointer data)
{
	YqTreeItem *trashitem,*closeitem;
	gint n;


	n = g_list_length(GTK_TREE(tree_main)->children);
	if (n==0)
		return;
	if ((strlen(selectleaf->nodename) == 1)&&(n>1))
	{
		Popup(_("Please select the yqpim file"));
		return;
	}	
	if (n==1)
	{
		closeitem=YQ_TREE_ITEM(GTK_TREE(tree_main)->children->data);
	}
	else
	{
		closeitem=selectleaf;
		while (closeitem->attree != GTK_TREE (tree_main))
			closeitem = YQ_TREE_ITEM (closeitem->attree->tree_owner);
	}
	if (!GTK_TREE(GTK_TREE_ITEM(closeitem)->subtree)->children)
		return;
	trashitem=YQ_TREE_ITEM(GTK_TREE(GTK_TREE_ITEM(closeitem)->subtree)->children->data);
	if (strcmp(trashitem->nodename,"yq_0")==0)
	{
		yq_tree_remove_item (GTK_WIDGET(trashitem->attree), GTK_WIDGET (trashitem));
		if (strcmp(selectleaf->nodename,"yq")==0)
			open_zu (selectleaf);
	}
}

void
menu_tool_trash_clean_func (GtkWidget * widget, gpointer data)
{
	YqTreeItem *trashitem,*closeitem;
	gint n,a,z;


	n = g_list_length(GTK_TREE(tree_main)->children);
	if (n==0)
		return;
	if ((strlen(selectleaf->nodename) == 1)&&(n>1))
	{
		Popup(_("Please select the yqpim file"));
		return;
	}	
	if (n==1)
	{
		closeitem=YQ_TREE_ITEM(GTK_TREE(tree_main)->children->data);
	}
	else
	{
		closeitem=selectleaf;
		while (closeitem->attree != GTK_TREE (tree_main))
			closeitem = YQ_TREE_ITEM (closeitem->attree->tree_owner);
	}
	yq_cfg_read_int (closeitem->cfg, "yq_0", "z", &z);
	yq_cfg_read_int (closeitem->cfg, "yq_0", "a", &a);
	if (z==0&&a==0) return;
	if (GTK_TREE(GTK_TREE_ITEM(closeitem)->subtree)->children)
	{
		trashitem=YQ_TREE_ITEM(GTK_TREE(GTK_TREE_ITEM(closeitem)->subtree)->children->data);
		if (strcmp(trashitem->nodename,"yq_0")==0)
		{
			toolbar_clean_trash(trashitem,1);
			yq_tree_select_child (trashitem->attree,GTK_WIDGET(trashitem));
			return;
		}
	}
	Areyousure (_("Are you sure you want to clean the trash?"));
	if (!yesnoreturn)
		return;
	yq_play_sound(10);
	yq_cfg_clean_sections(closeitem->cfg,"yq_0");
}

void
menu_file_exportvcard_func(GtkWidget *widget, gpointer data)
{
		Popup("please wait for next version");
}

void
menu_file_importvcard_func(GtkWidget *widget, gpointer data)
{
	Popup("please wait for next version");
}

void
menu_tool_password_func (GtkWidget * widget, gpointer data)
{
	Popup("please wait for next version");
}
