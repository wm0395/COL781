# utils/Makefile.in --- xscreensaver, Copyright © 1997-2021 Jamie Zawinski.
# the `../configure' script generates `jwxyz/Makefile' from this file.

# JWXYZ Is Not Xlib.
#
# But it's a bunch of function definitions that bear some resemblance to
# Xlib and that kinda sorta implement Xlib in terms of the native graphics
# substrate (Cocoa, OpenGL, GLES, Java).


.SUFFIXES:
.SUFFIXES: .c .o

srcdir		= .

prefix		= /usr
datarootdir	= ${prefix}/share

CC		= gcc -std=gnu89 -pedantic -Wall -Wnested-externs -Wstrict-prototypes -Wmissing-prototypes -Wdeclaration-after-statement -Wno-overlength-strings -Wno-language-extension-token -Wno-variadic-macros
CFLAGS		= -g -O2 -ffile-prefix-map=/home/user/Desktop/code/COL781_Assignments/xscreensaver-6.06+dfsg1=. -fstack-protector-strong -fstack-clash-protection -Wformat -Werror=format-security -fcf-protection -DLOG_CPU_TIME
DEFS		= -DHAVE_CONFIG_H

DEPEND		= makedepend
DEPEND_FLAGS	= 
DEPEND_DEFINES	= 

SHELL		= /bin/sh
INSTALL		= /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA	= ${INSTALL} -m 644
INSTALL_DIRS	= ${INSTALL} -d

X_CFLAGS	= 

INCLUDES_1	= -I$(srcdir) -I.. -I../utils
INCLUDES	= $(INCLUDES_1)   -I/usr/include/gtk-3.0 -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/harfbuzz -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/fribidi -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/x86_64-linux-gnu -I/usr/include/gio-unix-2.0 -I/usr/include/atk-1.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/libxml2 -pthread  -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/libpng16 -I/usr/include/x86_64-linux-gnu -I/usr/include/libmount -I/usr/include/blkid -pthread  -I/usr/include/freetype2 -I/usr/include/libpng16 

SRCS		= jwxyz-android.c jwxyz-cocoa.m jwxyz-common.c jwxyz-gl.c \
		  jwxyz-timers.c jwxyz-image.c jwxyz.m jwzgles.c
OBJS		= 
HDRS		= jwxyz-android.h jwxyz-cocoa.h jwxyz-timers.h jwxyz.h \
		  jwxyzI.h jwzgles.h jwzglesI.h
EXTRAS		= README Makefile.in

TARFILES	= $(EXTRAS) $(SRCS) $(HDRS) $(LOGOS)


default: all
all: $(OBJS)

install:   install-program   install-man
uninstall: uninstall-program uninstall-man

install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install

install-program:
install-man:
uninstall-program:
uninstall-man:

clean:
	-rm -f *.o a.out core

distclean: clean
	-rm -f Makefile TAGS *~ "#"*

# Adds all current dependencies to Makefile
depend:
	$(DEPEND) -s '# DO NOT DELETE: updated by make depend'		    \
	$(DEPEND_FLAGS) --						    \
	$(INCLUDES) $(DEFS) $(DEPEND_DEFINES) $(CFLAGS) $(X_CFLAGS) --	    \
	$(SRCS)

# Adds some dependencies to Makefile.in -- not totally accurate, but pretty
# close.  This excludes dependencies on files in /usr/include, etc.  It tries
# to include only dependencies on files which are themselves a part of this
# package.
distdepend::
	@echo updating dependencies in `pwd`/Makefile.in... ;		    \
	$(DEPEND) -w 0 -f - 						    \
	-s '# DO NOT DELETE: updated by make distdepend' $(DEPEND_FLAGS) -- \
	$(INCLUDES_1) $(DEFS) $(DEPEND_DEFINES) $(CFLAGS) $(X_CFLAGS) --    \
	$(SRCS) 2>/dev/null |						    \
	sort -d |							    \
	(								    \
	  awk '/^# .*Makefile.in ---/,/^# DO .*distdepend/' < Makefile.in ; \
	  sed -e '/^#.*/d'						    \
	      -e 's@ \./@ @g;s@ /[^ ]*@@g;/^.*:$$/d'			    \
	      -e 's@ \([^$$]\)@ $$(srcdir)/\1@g'			    \
	      -e 's@ $$(srcdir)/\(.*config.h\)@ \1@g' ;			    \
	  echo ''							    \
	) > /tmp/distdepend.$$$$ &&					    \
	mv /tmp/distdepend.$$$$ Makefile.in

TAGS: tags
tags:
	find $(srcdir) -name '*.[chly]' -print | xargs etags -a

list_tarfiles:
	@find $(TARFILES) -type f -print | sort


# How we build object files in this directory.
.c.o:
	$(CC) -c $(INCLUDES) $(DEFS) $(CPPFLAGS) $(CFLAGS) $(X_CFLAGS) $<


##############################################################################
#
# DO NOT DELETE: updated by make distdepend

jwxyz-common.o: ../config.h
jwxyz-image.o: ../config.h
jwxyz-timers.o: ../config.h
jwzgles.o: ../config.h

