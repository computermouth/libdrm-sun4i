AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/include/drm

AM_CFLAGS = \
	@PTHREADSTUBS_CFLAGS@ \
	$(WARN_CFLAGS)

libdrm_sun4i_ladir = $(libdir)
libdrm_sun4i_la_LTLIBRARIES = libdrm_sun4i.la
libdrm_sun4i_la_LDFLAGS = -version-number 0:0:0 -no-undefined
libdrm_sun4i_la_LIBADD = ../libdrm.la @PTHREADSTUBS_LIBS@

libdrm_sun4i_la_SOURCES = \
	private.h \
	sun4i.c

libdrm_sun4iincludedir = ${includedir}/libdrm
libdrm_sun4iinclude_HEADERS = sun4i.h

pkgconfigdir = @pkgconfigdir@
pkgconfig_DATA = libdrm_sun4i.pc

TESTS = sun4i-symbol-check
EXTRA_DIST = $(TESTS)
