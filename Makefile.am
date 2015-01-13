# octree/Makefile.am

ACLOCAL_AMFLAGS = -I m4

#AM_CFLAGS = -Wall -Wno-unused -fmax-errors=5
AM_CXXFLAGS = -Wall -Wextra -Werror -pedantic-errors


lib_LTLIBRARIES = liboctree.la
liboctree_la_SOURCES = octree.hh octree.cc

bin_PROGRAMS = demo
demo_SOURCES = demo.cc
demo_LDADD = liboctree.la

check_PROGRAMS = \
  test/create_delete

test_create_delete_SOURCES = test/create_delete.cc
test_create_delete_LDADD = liboctree.la


TESTS = $(check_PROGRAMS)
