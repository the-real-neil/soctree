# github.com/rubicks/soctree/Makefile.am


ACLOCAL_AMFLAGS = -I m4

AM_CXXFLAGS =          \
  -Wall                \
  -Wconversion         \
  -Werror              \
  -Wextra              \
  -pedantic-errors


lib_LTLIBRARIES = liboctree.la
liboctree_la_SOURCES = \
  morton.c             \
  morton.h             \
  octree.c             \
  octree.h


# noinst_lib_LTLIBRARIES = liboctree.la
# liboctree_la_SOURCES = octree.hh octree.cc

# bin_PROGRAMS = demo
# demo_SOURCES = demo.cc
# demo_LDADD = liboctree.la


# When check_PROGRAMS are linked against dynamic lib_LTLIBRARIES, automake
# generates them as posix shell scripts.
#
# When check_PROGRAMS are linked against static noinst_LTLIBRARIES, automake
# generates them as stand-alone binaries.

noinst_LTLIBRARIES = libcommon.la
libcommon_la_SOURCES = $(liboctree_la_SOURCES)

check_PROGRAMS = \
  test_create_delete \
  test_morton

test_create_delete_SOURCES = test/create_delete.c
test_create_delete_LDADD = libcommon.la

test_morton_SOURCES = test/morton.c
test_morton_LDADD = libcommon.la

TESTS = $(check_PROGRAMS)
