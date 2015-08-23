## Process this file with automake to generate a Makefile.in

bin_PROGRAMS = tileSweatshop createTileset
tileSweatshop_SOURCES = tileSweatshop.cc
cli_program_SOURCES = createTileset.cc

## Only our GUI program uses GTK+ at the moment.
tileSweatshop_CPPFLAGS = @GTK_CFLAGS@
tileSweatshop_LDADD = @GTK_LIBS@

CLEANFILES = *~
