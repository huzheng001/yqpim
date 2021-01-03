#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "intl.h"

#include <gtk/gtk.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include "configfile.h"
#include <string.h>

extern gboolean titleshow[46];
ConfigFile *rccfgfile;


void create_yqfile (gchar * filename);

void
yqrc_init ()
{
	gchar yq_dir[256];
	gchar yqrc_file[256];
	gchar yq_file[256];
	gchar *home;
	struct stat st;
	FILE *file;
	gint i;
	
	if ((home = getenv ("HOME")) == NULL)
		return;

	strcpy (yq_dir, home);
	if (yq_dir[strlen (yq_dir) - 1] != '/')
		strcat (yq_dir, "/");

	strcpy (yqrc_file, yq_dir);
	strcat (yqrc_file, ".yqrc");
	sprintf (yq_file, "1=%syqfile\n", yq_dir);
	if (stat (yqrc_file, &st))
	{
		if ((file = fopen (yqrc_file, "w")) == NULL)
			return;
		fprintf (file, "[setting]\n");
		fprintf (file, "paned_position=124\n");
		fprintf (file, "window_width=660\n");
		fprintf (file, "window_height=430\n");
		fprintf (file, "playsound=1\n");
		fprintf (file, "[file]\n");
		fprintf (file, "a=1\n");
		fprintf (file, yq_file);
		fprintf (file,"[csvindex_0]\n");
		fprintf (file,"a=1\n");
		fprintf (file,_("n=default cvsindex\n"));
		fprintf (file, "[titleshow]\n");
		for (i = 1; i <= 46; i++)
		{
			if ((i == 1) || (i == 9) || (i == 11) || (i == 15) || (i == 22) || (i == 46))
			{
				sprintf (yq_file, "%d=1\n", i);
				fprintf (file, yq_file);
			}
		}
		fclose (file);
	}
	
	sprintf (yq_file, "%syqfile", yq_dir);
	if (stat (yq_file, &st))
		create_yqfile (yq_file);
	rccfgfile = yq_cfg_open_file (yqrc_file);

	for (i = 1; i <= 46; i++)
	{
		sprintf (yq_file, "%d", i);
		yq_cfg_read_boolean (rccfgfile, "titleshow", yq_file,
				     &titleshow[i - 1]);
	}
}

void
create_yqfile (gchar * filename)
{
	FILE *file;

	if ((file = fopen (filename, "w")) == NULL)
		return;
	fprintf (file, "[yq]\n");
	fprintf (file, "v=2\n"); /*version 文件格式版本*/
	fprintf (file, "z=0\n");
	fprintf (file, "a=0\n");
	fprintf (file, _("n=My Contacts\n"));

	fclose (file);
}
