#!/bin/sh -xe

libtoolize                      $LT_EXTRA
aclocal                         $ACLOCAL_EXTRA
autoheader                      $AH_EXTRA
automake     --add-missing      $AM_EXTRA
autoconf                        $AC_EXTRA

