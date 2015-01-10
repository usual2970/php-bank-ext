dnl $Id$
dnl config.m4 for extension bank

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(bank, for bank support,
dnl Make sure that the comment is aligned:
dnl [  --with-bank             Include bank support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(bank, whether to enable bank support,
Make sure that the comment is aligned:
[  --enable-bank           Enable bank support])

if test "$PHP_BANK" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-bank -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/bank.h"  # you most likely want to change this
  dnl if test -r $PHP_BANK/$SEARCH_FOR; then # path given as parameter
  dnl   BANK_DIR=$PHP_BANK
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for bank files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       BANK_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$BANK_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the bank distribution])
  dnl fi

  dnl # --with-bank -> add include path
  dnl PHP_ADD_INCLUDE($BANK_DIR/include)

  dnl # --with-bank -> check for lib and symbol presence
  dnl LIBNAME=bank # you may want to change this
  dnl LIBSYMBOL=bank # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $BANK_DIR/lib, BANK_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_BANKLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong bank lib version or lib not found])
  dnl ],[
  dnl   -L$BANK_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(BANK_SHARED_LIBADD)

  PHP_NEW_EXTENSION(bank, bank.c, $ext_shared)
fi
