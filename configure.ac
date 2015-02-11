# octree/configure.ac

AC_PREREQ([2.68])
AC_INIT([octree],
        [m4_esyscmd_s([git describe --always --dirty])],
        [rubicks00@gmail.com],
        [octree],
        [https://github.com/rubicks/octree])
AC_LANG_PUSH([C++])
AC_CONFIG_SRCDIR([octree.hh])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([subdir-objects])
LT_PREREQ([2.4])
#LT_INIT([dlopen])
LT_INIT([shared])

# Checks for programs.
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11
#AC_PROG_CC


# Checks for libraries.

dnl AC_CHECK_LIB([z],[gzopen],[],
dnl   [AC_MSG_ERROR([libz missing])])


dnl Checks for header files.

AC_CHECK_HEADER([cstdlib],[],[AC_MSG_ERROR([cstdlib missing])])
dnl AC_CHECK_HEADER([endian.h],[],[AC_MSG_ERROR([endian.h missing])])
dnl AC_CHECK_HEADER([stdint.h],[],[AC_MSG_ERROR([stdint.h missing])])


# Checks for typedefs, structures, and compiler characteristics.

AC_C_INLINE

AC_C_BIGENDIAN([AC_DEFINE([IS_BIG_ENDIAN],[1],[define if big endian])],
               [AC_DEFINE([IS_LITTLE_ENDIAN],[1],[define if little endian])],
               [AC_MSG_ERROR([unknown endianness])])

AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T


# Checks for library functions.

dnl AC_CHECK_FUNCS([pow sqrt strcasecmp])


dnl Checks for symbol declarations

dnl AC_CHECK_DECLS([M_PI],
dnl   [],
dnl   [AC_MSG_ERROR([M_PI missing])],
dnl   [[#include <math.h>]])


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
