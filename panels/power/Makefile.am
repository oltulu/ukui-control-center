cappletname = power
saverdir = $(libexecdir)/ukui-screensaver
#libexecdir = /usr/lib
AM_CPPFLAGS = 					\
	-DUIDIR=\""$(uidir)"\"			\
	@PACKAGE_CFLAGS@			\
	-DUKUIMENU_I_KNOW_THIS_IS_UNSTABLE	\
	-fPIC					\
        $(PANEL_CFLAGS)				\
	${DBUS_CFLAGS}				\
	$(UPOWER_CFLAGS)			\
	$(UNIQUE_CFLAGS)			\
	-DSBINDIR=\"$(sbindir)\"		\
	-DUKUILOCALEDIR=\""$(datadir)/locale"\"	\
	-DEGG_LOG_FILE=\""/tmp/kpm.log"\"       \
        -DEGG_VERBOSE="\"GPM_VERBOSE\""         \
        -DEGG_LOGGING="\"GPM_LOGGING\""         \
        -DEGG_CONSOLE="\"GPM_CONSOLE\""         \
	-DSAVERDIR=\""$(saverdir)"\"		\
	-DLIBEXECDIR=\""$(libexecdir)"\"	\
        $(NULL)

ccpanelsdir = $(PANELS_DIR)
ccpanels_LTLIBRARIES = libpower.la

libpower_la_SOURCES =                           \
        egg-color.c                                     \
        egg-color.h                                     \
        egg-debug.h                                     \
        egg-debug.c                                     \
        egg-precision.h                                 \
        egg-precision.c                                 \
        egg-array-float.c                               \
        egg-array-float.h                               \
        egg-idletime.h                                  \
        egg-idletime.c                                  \
        egg-dbus-proxy.h                                \
        egg-dbus-proxy.c                                \
        egg-dbus-monitor.h                              \
        egg-dbus-monitor.c                              \
        egg-discrete.h                                  \
        egg-discrete.c                                  \
        egg-string.h                                    \
        egg-string.c                                    \
        egg-console-kit.h                               \
        egg-console-kit.c                               \
        kpm-common.h                                    \
        kpm-common.c                                    \
        kpm-brightness.h                                \
        kpm-brightness.c                                \
        kpm-marshal.h                                   \
        kpm-marshal.c                                   \
        kpm-upower.c                                    \
        kpm-upower.h                                    \
	kpm-prefs-core.c				\
	kpm-prefs-core.h				\
	ukui-screensaver-preferences.c			\
	subprocs.c					\
	subprocs.h					\
	gs-job.c					\
	gs-job.h					\
	gs-theme-manager.c				\
	gs-theme-manager.h				\
	gs-prefs.c					\
	gs-prefs.h					\
	copy-theme-dialog.c				\
	copy-theme-dialog.h				\
	$(NULL)

libpower_la_LIBADD = 				\
			@PACKAGE_LIBS@		\
			$(DBUS_LIBS)		\
			$(UNIQUE_LIBS)		\
			$(UPOWER_LIBS)		\
			-lukui-menu		\
			$(NULL)

libpower_la_LDFLAGS = -export-dynamic @PACKAGE_LDFLAGS@

uidir = $(pkgdatadir)/ui
ui_DATA = ukui-fullscreen-preview.ui

@INTLTOOL_XML_NOMERGE_RULE@
#gsettings_schemas_in_files = org.ukui.power-manager.gschema.xml.in
#gsettings_SCHEMAS = $(gsettings_schemas_in_files:.gschema.xml.in=.gschema.xml)
@GSETTINGS_RULES@

CLEANFILES = \
        $(gsettings_SCHEMAS_in) \
        $(gsettings_SCHEMAS) \
        *.gschema.valid

BUILT_SOURCES =                                         \
        org.ukui.PowerManager.h                 \
        org.ukui.PowerManager.Backlight.h               \
        kpm-marshal.c                                   \
        kpm-marshal.h                                   \
        $(NULL)

kpm-marshal.c: kpm-marshal.list
	echo "#include \"kpm-marshal.h\"" > $@ && \
	@GLIB_GENMARSHAL@ $< --prefix=kpm_marshal --body >> $@

kpm-marshal.h: kpm-marshal.list
	@GLIB_GENMARSHAL@ $< --prefix=kpm_marshal --header > $@

org.ukui.PowerManager.h: org.ukui.PowerManager.xml
	libtool --mode=execute dbus-binding-tool        \
		--prefix=kpm_manager                    \
		--mode=glib-server                      \
                --output=org.ukui.PowerManager.h        \
                $(srcdir)/org.ukui.PowerManager.xml

org.ukui.PowerManager.Backlight.h: org.ukui.PowerManager.Backlight.xml
	libtool --mode=execute dbus-binding-tool        \
		--prefix=kpm_backlight                  \
		--mode=glib-server                      \
		--output=org.ukui.PowerManager.Backlight.h      \
                $(srcdir)/org.ukui.PowerManager.Backlight.xml


clean-local :
	rm -f *~ Makefile.in Makefile
	rm -f kpm-marshal.c kpm-marshal.h org.ukui.PowerManager.h org.ukui.PowerManager.Backlight.h
