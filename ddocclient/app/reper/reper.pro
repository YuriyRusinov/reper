TEMPLATE = app

include(../../ddocclient.conf)
QT += network core gui xml

TARGET = reper

DESTDIR = ../../build
QGISDIR = "C:/Program Files (x86)/Quantum GIS 1.7.0"

DEFINES *= __USE_EXPORTS

INCLUDEPATH +=  . \
                $$QGISDIR/include \
                ../../dataaccess \
                ../../kksutils \
                ../../kksrunservice


DEPENDPATH += 	\
                . \
                ../../dataaccess \
                ../../kksutils \
                ../../kksrunservice

#LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkksrunservice
LIBS += -L$$QGISDIR/lib -lqgis_core -lqgis_gui

# libpq support
LIBS += -L$${PSQL_HOME}/lib
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libs/libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS += -lpq

include(reper.pri)

target.path = $$KKS_EXEC_DIR
INSTALLS += target


