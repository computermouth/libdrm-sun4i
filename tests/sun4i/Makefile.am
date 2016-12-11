AM_CPPFLAGS = \
	-I$(top_srcdir)/include/drm \
	-I$(top_srcdir)/sun4i \
	-I$(top_srcdir)

AM_CFLAGS = $(WARN_CFLAGS)

LDADD = \
	../../sun4i/libdrm_sun4i.la \
	../../libdrm.la

noinst_PROGRAMS = \
	openclose
