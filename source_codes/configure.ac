dnl Process this file with autoconf to produce a configure script.

AC_INIT(src/main.c)
AM_INIT_AUTOMAKE(yqpim, 0.8.0)
AM_CONFIG_HEADER(config.h)

dnl
dnl Checks for programs.
dnl ====================
AC_PROG_CC
AC_ISC_POSIX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB

CFLAGS=""

dnl
dnl Only use -Wall if we have gcc
dnl =============================
if test "x$GCC" = "xyes"; then
  if test -z "`echo "$CFLAGS" | grep "\-Wall" 2> /dev/null`" ; then
    CFLAGS="$CFLAGS -Wall"
  fi
fi

dnl
dnl Checks for GTK+ version, use this
dnl =================================
AM_PATH_GTK(1.2.5,
            [LIBS="$LIBS $GTK_LIBS"
            CFLAGS="$CFLAGS $GTK_CFLAGS"],
            AC_MSG_ERROR(Cannot find GTK: Is gtk-config in path?))

gtk_version=`$GTK_CONFIG --version`
gtk_major_version=`echo $gtk_version | awk -F '.' '{print $1}'`
gtk_minor_version=`echo $gtk_version | awk -F '.' '{print $2}'`
gtk_micro_version=`echo $gtk_version | awk -F '.' '{print $3}'`

if test $gtk_major_version -gt 1 -o \
	$gtk_major_version -eq 1 -a $gtk_minor_version -gt 2 -o \
	$gtk_major_version -eq 1 -a $gtk_minor_version -eq 2 -a $gtk_micro_version -ge 5; then
	gtk_stat="up-to-date"
else
	gtk_stat="out-of-date"
fi

dnl CHECK_GNOME
  AC_PATH_PROG(GNOME_CONFIG,gnome-config,no)
  AM_CONDITIONAL(HAVE_GNOME, test $GNOME_CONFIG != no)
  if test "$GNOME_CONFIG" = no; then
    AC_MSG_RESULT(GNOME icon and menu entry will NOT be installed)
  else
    AC_MSG_RESULT(will install a GNOME icon and menu entry)
    AC_SUBST(GNOME_DATADIR)
    GNOME_DATADIR="`$GNOME_CONFIG --datadir`"
  fi

dnl
dnl Checks for header files.
dnl ========================
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(unistd.h)

dnl
dnl Checks for typedefs, structures, and compiler characteristics.
dnl ==============================================================
AC_C_CONST
AC_STRUCT_TM

dnl
dnl Checks for library functions.
dnl =============================
AC_CHECK_FUNCS(getcwd)

dnl
dnl Checks for i18n support
dnl =======================
ALL_LINGUAS="zh_CN"
AM_GNU_GETTEXT
LIBS="$LIBS $INTLLIBS"
localedir='${prefix}/share/locale'
AC_SUBST(localedir)

AC_OUTPUT(src/Makefile yqpim.spec src/pixmaps/Makefile src/wave/Makefile Makefile intl/Makefile po/Makefile.in)

dnl
dnl Tell you what optional libraries you've installed
dnl =================================================

echo
echo "YqPim configure result:"
echo
echo "GTK+ version: $gtk_version ($gtk_stat)"
echo "==============================================================="
echo "Type \"make\" to compile yqpim."
echo

