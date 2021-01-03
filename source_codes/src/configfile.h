#ifndef YQ_CONFIGFILE_H
#define YQ_CONFIGFILE_H

#include <glib.h>

typedef struct
{
	gchar *key;
	gchar *value;
}
ConfigLine;

typedef struct
{
	gchar *name;
	GList *lines;
}
ConfigSection;


typedef struct
{
	GList *sections;
	gchar *cfgfilename;
	gboolean changed;
}
ConfigFile;

ConfigFile *yq_cfg_new (void);
ConfigFile *yq_cfg_open_file (gchar * filename);
gboolean yq_cfg_write_file (ConfigFile * cfg, gchar * filename);
void yq_cfg_free (ConfigFile * cfg);

gboolean yq_cfg_read_string (ConfigFile * cfg, gchar * section, gchar * key,
			     gchar ** value);
gboolean yq_cfg_read_int (ConfigFile * cfg, gchar * section, gchar * key,
			  gint * value);
gboolean yq_cfg_read_boolean (ConfigFile * cfg, gchar * section, gchar * key,
			      gboolean * value);
void yq_cfg_write_string (ConfigFile * cfg, gchar * section, gchar * key,
			  gchar * value);
void yq_cfg_write_int (ConfigFile * cfg, gchar * section, gchar * key,
		       gint value);
void yq_cfg_write_boolean (ConfigFile * cfg, gchar * section, gchar * key,
			   gboolean value);

void yq_cfg_rename_section (ConfigFile * cfg, gchar * section,
			    gchar * section_name);
void yq_cfg_remove_key (ConfigFile * cfg, gchar * section, gchar * key);
void yq_cfg_remove_section (ConfigFile * cfg, gchar * section);
#endif
