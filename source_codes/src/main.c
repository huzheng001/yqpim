/*
yqpim
program by huzheng
forlinux@263.net
http://forlinux.yeah.net

友情强档 for linux
胡正制作
forlinux@263.net
http://forlinux.yeah.net
qq: 1706932
非常感谢你阅读源代码,我会很高兴如果你与我联系!
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif


#include "configfile.h"
#include "intl.h"
#include "yqtreeitem.h"
#include <gtk/gtk.h>
#include <stdlib.h>


extern ConfigFile *rccfgfile,*tmpcfg;
extern GtkWidget *tree_main;
extern YqTreeItem *root_leaf;
extern GtkWidget *nbitems2[12];
extern gint yesnoreturn;
extern gboolean nb_changed;
extern GdkPixmap *yqpixmaps[MAX_IMAGE_TYPES];
extern GdkBitmap *yqmasks[MAX_IMAGE_TYPES];



void yq_main ();
void createmenu (GtkWidget * win_main, GtkWidget * main_vbox);
void createtoolbar (GtkWidget * parent, GtkWidget * main_vbox);
void createtoolbar1 (GtkWidget * parent, GtkWidget * main_vbox);
void createtree (GtkWidget * parent, GtkWidget * vbox);
void createlist (GtkWidget * vbox);
void yq_load_pixmaps (GtkWidget * parent);
void yqrc_init ();
void yq_readfile (GtkWidget * widget, gpointer openfile);
void toolbar_cancel (GtkWidget * widget, gpointer data);
void toolbar_save (GtkWidget * widget, gpointer data);
void YesNo (gchar * szMessage);
void yq_tree_name_search(GtkWidget * widget, gpointer data);
void yq_tree_ruby_search(GtkWidget * widget, gpointer data);
void yq_play_sound(gint i);


gint
CloseAppWindow (GtkWidget * widget, gpointer * data)
{
	GList *list = NULL;
	ConfigFile *nowcfgfile;

	if (nb_changed)
	{
		if (*gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (nbitems2[5])->entry)) != '\0')
		{
			YesNo (_("modifing the card,save it?"));
			if (yesnoreturn == 2)
				return 1;
			if (yesnoreturn == 1)
				toolbar_save (widget, NULL);
		}
	}
	if (GTK_TREE_ITEM (root_leaf)->subtree)
	{
		list = gtk_container_children (GTK_CONTAINER (tree_main));
		while (list)
		{
			nowcfgfile = YQ_TREE_ITEM (list->data)->cfg;
			if (nowcfgfile->changed)
			{
				YesNo (_("this yqpim file is changed,save it?"));
				if (yesnoreturn == 2)
					return 1;
				if (yesnoreturn == 1)
					yq_cfg_write_file (nowcfgfile,
						   nowcfgfile->cfgfilename);
			}
			yq_cfg_free (nowcfgfile);
			g_free (nowcfgfile);
			list = g_list_remove_link (list, list);
		}
	}
	yq_play_sound(2);
	if (rccfgfile->changed)
		yq_cfg_write_file (rccfgfile, rccfgfile->cfgfilename);
	yq_cfg_free (rccfgfile);
	g_free (rccfgfile);
	if (tmpcfg)
	{	
		yq_cfg_free(tmpcfg);
		g_free(tmpcfg);
	}
	gtk_main_quit ();
	return (FALSE);
}

int
main (int argc, char *argv[])
{
	gtk_set_locale ();	//有了这行就可以处理中文了
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	
	gtk_init (&argc, &argv);
	yqrc_init ();		//初始化配置文件,在createfile.c
	yq_main ();		//创建主窗口
	gtk_main ();
	return 0;
}

/*
void
kk (GtkWidget * widget, gpointer * data)
{
	g_print ("%s\n", data);
}
*/
void
yq_play_sound(gint i)
{
	gboolean b;

	yq_cfg_read_boolean (rccfgfile, "setting", "playsound", &b);
	if (!b) return;
	switch (i)
	{
	case 1:
		system("esdplay /usr/share/sounds/yq/start.wav &");
		break;
	case 2:
		system("esdplay /usr/share/sounds/yq/end.wav &");
		break;
	case 3:
		system("esdplay /usr/share/sounds/yq/look.wav &");
		break;
	case 4:
		system("esdplay /usr/share/sounds/yq/error.wav &");
		break;
	case 5:
		system("esdplay /usr/share/sounds/yq/notify.wav &");
		break;
	case 6:
		system("esdplay /usr/share/sounds/info.wav &");
		break;
	case 7:
		system("esdplay /usr/share/sounds/question.wav &");
		break;
	case 8:
		system("esdplay /usr/share/sounds/generic.wav &");
		break;
	case 9:
		system("esdplay /usr/share/sounds/panel/slide.wav &");
		break;
	case 10:
		system("esdplay /usr/share/sounds/gnobots2/splat.wav &");
		break;
	}
}

void
winmain_size_changed (GtkWidget * box)
{
	gint m;
	
	yq_cfg_read_int (rccfgfile, "setting", "window_width",&m);
	if (m!=box->allocation.width)
		yq_cfg_write_int (rccfgfile, "setting", "window_width",
			  box->allocation.width);
	yq_cfg_read_int (rccfgfile, "setting", "window_height",&m);
	if (m!=box->allocation.height)
		yq_cfg_write_int (rccfgfile, "setting", "window_height",
			  box->allocation.height);
}

void
yq_main ()
{
	GtkWidget *main_vbox, *win_main;
	GtkWidget *hpaned, *vbox, *hbox;
	GtkWidget *label, *entry;
	gint a, b;
	gchar *ab;

	win_main = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (win_main), _("Yqpim"));
	gtk_window_set_wmclass(GTK_WINDOW(win_main), "main", "Yqpim");
	gtk_widget_set_usize (win_main, 300, 200);
	gtk_container_border_width (GTK_CONTAINER (win_main), 2);
	gtk_signal_connect (GTK_OBJECT (win_main), "delete_event",
			    GTK_SIGNAL_FUNC (CloseAppWindow), NULL);
	gtk_widget_realize (win_main);
	yq_load_pixmaps (win_main);
	gdk_window_set_icon(win_main->window, NULL, yqpixmaps[YQPIM], yqmasks[YQPIM]);

	main_vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (win_main), main_vbox);
	gtk_widget_show (main_vbox);
	createmenu (win_main, main_vbox);
	createtoolbar (win_main, main_vbox);
	label = gtk_label_new (_("Name: "));
	gtk_widget_show (label);
	entry = gtk_entry_new ();
	gtk_widget_set_usize (entry, 60, -1);
	gtk_widget_show (entry);
	gtk_signal_connect (GTK_OBJECT (entry), "activate",
		    GTK_SIGNAL_FUNC (yq_tree_name_search), entry);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), entry, TRUE, TRUE, 0);
	gtk_widget_show (hbox);
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
	createtree (win_main, vbox);
	hpaned = gtk_hpaned_new ();
	gtk_paned_pack1 (GTK_PANED (hpaned), vbox, TRUE, TRUE);
	label = gtk_label_new (_("Ruby Search: "));
	gtk_widget_show (label);
	entry = gtk_entry_new_with_max_length (10);
	gtk_signal_connect (GTK_OBJECT (entry), "activate",
	    GTK_SIGNAL_FUNC (yq_tree_ruby_search), entry);
	gtk_widget_set_usize (entry, 35, -1);
	gtk_widget_show (entry);
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), entry, FALSE, FALSE, 0);
	createtoolbar1 (win_main, hbox);
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
	createlist (vbox);
	gtk_paned_pack2 (GTK_PANED (hpaned), vbox, TRUE, TRUE);
	yq_cfg_read_int (rccfgfile, "setting", "paned_position", &b);
	yq_cfg_read_int (rccfgfile, "setting", "window_width", &a);
	gtk_paned_compute_position (GTK_PANED (hpaned), a - 4, b + 2,
				    a - b - 6);
	gtk_paned_set_position (GTK_PANED (hpaned), b + 2);
	gtk_widget_show (hpaned);
	gtk_box_pack_start (GTK_BOX (main_vbox), hpaned, TRUE, TRUE, 0);
	yq_cfg_read_int (rccfgfile, "setting", "window_height", &b);
	gtk_window_set_default_size (GTK_WINDOW (win_main), a, b);
	gtk_signal_connect_object (GTK_OBJECT (win_main),
				   "size_allocate",
				   GTK_SIGNAL_FUNC (winmain_size_changed),
				   GTK_OBJECT (win_main));
/*	gtk_widget_unrealize (win_main);*/  //if add this line,the window's icon can't show
	gtk_widget_show (win_main);
	yq_play_sound(1);
	yq_cfg_read_string (rccfgfile, "file", "1", &ab);	//ab为上次打开的记录文件
	yq_readfile (win_main, ab);	//打看了^_^, treefunc.c
	g_free (ab);
}
