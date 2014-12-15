QT       += gui

TARGET = gologramma
TEMPLATE = lib

DESTDIR = ../../../build

DEFINES += GOLOGRAMMA_LIBRARY

SOURCES += gologramma.cpp \
           model3Dconst.cpp \
           objloader.cpp \
           cuboid.cpp

HEADERS += gologramma.h \
           gologramma_global.h \
           model3Dconst.h \
           objloader.h \
           cuboid.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
