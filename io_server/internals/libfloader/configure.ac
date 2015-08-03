AC_INIT(libfloader, 0.5.1, sergey.karin@gmail.com)
#(Makefile.opt.in)
AC_CONFIG_SRCDIR(src/rgetfile.c)
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([include/config.h])
AM_INIT_AUTOMAKE([foreign dist-bzip2])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_PGSQL

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h malloc.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([mkdir pow sqrt strerror])

AC_CONFIG_FILES([
         Makefile

         src/Makefile
         include/Makefile
])    

AC_OUTPUT
