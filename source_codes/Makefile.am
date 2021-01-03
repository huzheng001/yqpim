SUBDIRS = intl po src
EXTRA_DIST = \
	yqpim.desktop \
	yqpim.png \
	yqpim.spec.in

if HAVE_GNOME	
desktopdir = $(GNOME_DATADIR)/gnome/apps/Applications
desktop_DATA = yqpim.desktop
pixmapdir = $(GNOME_DATADIR)/pixmaps
pixmap_DATA = yqpim.png
endif

dist-hook: yqpim.spec
	cp $(top_builddir)/yqpim.spec $(distdir)
