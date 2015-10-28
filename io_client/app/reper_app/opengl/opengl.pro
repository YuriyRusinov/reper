QT       += core gui opengl

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = repopengl
TEMPLATE = lib

DEFINES += GOLOGRAMMA_LIBRARY
DESTDIR = ../../../build

include (opengl.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}

TRANSLATIONS = ../../../transl/gologram_ru.ts

