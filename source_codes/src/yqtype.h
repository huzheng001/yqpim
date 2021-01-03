#include <gtk/gtk.h>

typedef enum
{
	GIRL,
	BOY,
	ZU_OPEN,
	ZU_CLOSE,
	YQFILE,
	YQPIM,
	YQFILE_OPEN,
	YQFILE_CLOSE,
	TRASH,
	NAME,
	HOME,
	BUSINESS,
	PERSONAL,
	NOTES,
	MAX_IMAGE_TYPES
}
ImageType;

typedef enum
{
	LIST_DETAILS = 0,
	LIST_THUMBNAILS = 1,
	LIST_SMALL_ICONS = 2,
	LIST_NOTEBOOK = 3
}
ListType;

struct _YQList
{
	GtkWidget *box, *list, *listwin;
	ListType type;
	gint sorttype;
};

