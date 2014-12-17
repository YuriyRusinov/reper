#-------------------------------------------------
#
# Project created by QtCreator 2014-12-11T16:55:51
#
#-------------------------------------------------

QT       += core gui

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gologram
TEMPLATE = lib

DEFINES += GOLOGRAMMA_LIBRARY
DESTDIR = ../../../build

SOURCES += gologramma.cpp \
           model3Dconst.cpp \
           objloader.cpp \
           cuboid.cpp \
           imagecreatorform.cpp

HEADERS += gologramma.h\
           gologramma_global.h \
           model3Dconst.h \
           objloader.h \
           cuboid.h \
           imagecreatorform.h

FORMS    += imagecreatorform.ui

unix {
    target.path = /usr/lib
    INSTALLS += target
}
