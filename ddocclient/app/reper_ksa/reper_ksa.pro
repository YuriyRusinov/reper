TEMPLATE = app

TRANSLATIONS +=reper_ru.ts

include(../../ddocclient.conf)
QT += network core gui xml

TARGET = reper_ksa

DESTDIR = ../../build

INCLUDEPATH +=  . \
                ../../dataaccess \
                ../../kksutils \
                ../../kksqgis


DEPENDPATH += 	\
                . \
                ../../dataaccess \
                ../../kksutils \
                ../../kksqgis

#LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkksrunservice
qgis{
    DEFINES *= __USE_QGIS__
    DEFINES *= __USE_EXPORTS  #for qgis export library

    INCLUDEPATH += $$OSGEODIR/include \
                   $$QGISDIR/include/qgis \
                   $$QGISDIR/include

    LIBS += -L$$QGISDIR/lib -lqgis_core -lqgis_gui

    win32{
        LIBS *= -L$$OSGEODIR/lib  -lproj_i -lgdal_i -lsqlite3_i
    }
    else{
        LIBS *= -L$$OSGEODIR/lib  -lproj -lgdal -lsqlite3
    }
}

LIBS *= -L$$DESTDIR -lkksqgis

# libpq support
LIBS += -L$${PSQL_HOME}/lib
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS += -lpq

include(reper_ksa.pri)

target.path = $$KKS_EXEC_DIR
INSTALLS += target
