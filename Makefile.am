SUBDIRS = src

EXTRA_DIST = gtk-synce.desktop	\
             gtk-synce.png	\
             depcomp		\
             build_rpm.sh	\
             gtk-synce.spec	\
             build_tgz.sh	\
             slack-desc

gnome_menudir = $(prefix)/share/applications
gnome_menu_DATA = gtk-synce.desktop

icondir = $(prefix)/share/pixmaps
icon_DATA = gtk-synce.png

docdir = $(prefix)/doc/@PACKAGE@-@VERSION@
doc_DATA = ChangeLog AUTHORS COPYING INSTALL README TODO NEWS
