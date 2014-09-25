dnl $Id$
dnl config.m4 for extension filebase

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(filebase, for filebase support,
dnl Make sure that the comment is aligned:
dnl [  --with-filebase             Include filebase support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(filebase, whether to enable filebase support,
dnl Make sure that the comment is aligned:
dnl [  --enable-filebase           Enable filebase support])

if test "$PHP_FILEBASE" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-filebase -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/filebase.h"  # you most likely want to change this
  dnl if test -r $PHP_FILEBASE/$SEARCH_FOR; then # path given as parameter
  dnl   FILEBASE_DIR=$PHP_FILEBASE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for filebase files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       FILEBASE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$FILEBASE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the filebase distribution])
  dnl fi

  dnl # --with-filebase -> add include path
  dnl PHP_ADD_INCLUDE($FILEBASE_DIR/include)

  dnl # --with-filebase -> check for lib and symbol presence
  dnl LIBNAME=filebase # you may want to change this
  dnl LIBSYMBOL=filebase # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $FILEBASE_DIR/lib, FILEBASE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_FILEBASELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong filebase lib version or lib not found])
  dnl ],[
  dnl   -L$FILEBASE_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(FILEBASE_SHARED_LIBADD)

  PHP_NEW_EXTENSION(filebase, filebase.c, $ext_shared)
fi
