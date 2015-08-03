dnl ---------------------------------------------------------------------------
dnl Message output
dnl ---------------------------------------------------------------------------
AC_DEFUN([LOC_MSG],[echo "$1"])

dnl ------------------------------------------------------------------------
dnl Detect GDAL/OGR
dnl
dnl use AC_CHECK_GDAL to detect GDAL and OGR
dnl it sets:
dnl   GDAL_CFLAGS
dnl   GDAL_LDADD
dnl ------------------------------------------------------------------------


dnl ---------------------------------------------------------------------------
dnl PostgreSQL
dnl ---------------------------------------------------------------------------
AC_DEFUN([AC_CHECK_PGSQL],
[
AC_ARG_WITH(pgsql,
	AS_HELP_STRING([--with-pgsql=path],
                 [PostgreSQL Support (full path to pg_config)]),
	[ac_pgsql_config_path=$withval])

if test x"$ac_pgsql_config_path" = x ; then
  ac_pgsql_config_path=`which pg_config 2>/dev/null`
fi

PG_PATH="/usr/pgsql/bin:/usr/local/pgsql/bin:/usr/lib/pgsql/bin:/opt/pgsql/bin"

ac_pgsql_config_path=`AS_DIRNAME( [$ac_pgsql_config_path] )`
AC_PATH_PROG([PG_CONFIG], [pg_config], [no], [$ac_pgsql_config_path $PG_PATH])

if test x${PG_CONFIG} = xno ; then
  AC_MSG_ERROR([pg_config not found! Supply it with --with-pgsql=PATH])
else
  AC_MSG_CHECKING([PG_CFLAGS])
  PG_CFLAGS="-I`$PG_CONFIG --includedir-server`"
  AC_MSG_RESULT($PG_CFLAGS)

  AC_MSG_CHECKING([PG_LIBDIR])
  PG_LIBDIR=`$PG_CONFIG --libdir`
  AC_MSG_RESULT($PG_LIBDIR)

  AC_MSG_CHECKING([PG_PKGLIBDIR])
  PG_PKGLIBDIR=`$PG_CONFIG --pkglibdir`
  AC_MSG_RESULT($PG_PKGLIBDIR)
fi

AC_SUBST(PG_CONFIG)
AC_SUBST(PG_CFLAGS)
AC_SUBST(PG_LIBDIR)
AC_SUBST(PG_PKGLIBDIR)
])
