TEMPLATE = app

TRANSLATIONS +=reper_ru.ts

include(../../io_client.conf)
QT += network core gui xml

TARGET = reper_ksa

DESTDIR = ../../build

INCLUDEPATH +=  . \
                ../../corelibs/dataaccess \
                ../../corelibs/kksutils \
                ../../corelibs/kksdatafactory \
                ../../corelibs/kkspp \
                ../../corelibs/kkspluginmanager \
                ../../kksqgis/kksqgis


DEPENDPATH += 	\
                ../../corelibs/dataaccess \
                ../../corelibs/kksutils \
                ../../corelibs/kksdatafactory \
                ../../corelibs/kkspp \
                ../../corelibs/kkspluginmanager \
                ../../kksqgis/kksqgis

LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkkspp -lkkspluginmanager -lkksdatafactory

qgis{
    DEFINES *= __USE_QGIS__
    DEFINES *= __USE_EXPORTS  #for qgis export library


    KKSQGIS_LIB_NAME = kksqgis26
    KKSQGIS_DIR = kksqgis26

    LIBS *= -L$$QGISDIR/lib -lqgis_core -lqgis_gui
    LIBS *= -L$$DESTDIR -l$$KKSQGIS_LIB_NAME

    INCLUDEPATH *= \
                   $$OSGEODIR/include \
                   $$OSGEODIR/include/qwt \
                   $$QGISDIR/include/qgis \
                   $$QGISDIR/include \
                   ../../kksqgis/$$KKSQGIS_DIR \
                   ../../kksqgis/$$KKSQGIS_DIR/layerprops \
                   ../../kksqgis/$$KKSQGIS_DIR/legend \
                   ../../kksqgis/$$KKSQGIS_DIR/projectprops \
                   ../../kksqgis/$$KKSQGIS_DIR/pluginmanager \
                   ../../kksqgis/$$KKSQGIS_DIR/maptools

    DEPENDPATH *= ../../kksqgis/$$KKSQGIS_DIR

    win32{
        LIBS *= -L$$OSGEODIR/lib  -lproj_i -lgdal_i -lsqlite3_i -l$$QWTLIB -l$$QSCI
    }
    else{
        LIBS *= -L$$OSGEODIR/lib  -lproj -lgdal -lsqlite3 -l$$QWTLIB -l$$QSCI
    }

}

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
