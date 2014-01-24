# -*- mode: sh -*- ###########################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
##############################################

QWT_ROOT = ../..

include( $${QWT_ROOT}/kksqwtconfig.pri )

TEMPLATE  = lib

contains(CONFIG, QwtDll ) {
    CONFIG += dll
}
else {
    CONFIG += staticlib
}

#MOC_DIR         = moc
#OBJECTS_DIR     = obj$${SUFFIX_STR}
#DESTDIR         = $${QWT_ROOT}/lib
INCLUDEPATH    += $${QWT_ROOT}/src
DEPENDPATH     += $${QWT_ROOT}/src

QWTLIB       = kksqwt

win32 {
    QwtDll {
        DEFINES += QT_DLL QWT_DLL QWT_MAKEDLL
        QWTLIB   = $${QWTLIB}
    }

    win32-msvc:LIBS  += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-msvc.net:LIBS  += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-msvc2002:LIBS += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-msvc2003:LIBS += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-msvc2005:LIBS += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-msvc2008:LIBS += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-g++:LIBS   += -L$${QWT_ROOT}/lib -l$${QWTLIB}
}
else {
    LIBS      += -L$${QWT_ROOT}/lib -l$${QWTLIB}
}
