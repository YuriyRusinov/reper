TEMPLATE = app

include(../../ddocclient.conf)

TARGET = $${EXECUTABLE_PREFIX}_app

DESTDIR = ../../build

INCLUDEPATH += 	. \
		../../corelibs/dataaccess \
		../../corelibs/kksutils \
		../../corelibs/kkspp \
		../../corelibs/kksdatafactory \
		../../corelibs/kkspluginmanager \
		../../mgrlibs/kkscoreapplication/base_classes \
		../../mgrlibs/kkscoreapplication/ui \
		../../mgrlibs/kkscoreapplication \
                ../../guilibs/kksgui \
		../../guilibs/kksgui/ui \
                ../../guilibs/kksqwt \
                ../../guilibs/kksqwt/src \
                ../../guilibs/kksguifactory \
		../../mgrlibs/kksapplication \
                ../../openreport/kksorpcommon \
                ../../openreport/kksorprenderer \
                ../../openreport/kksorpwrtembed


DEPENDPATH += 	\
		. \
		../../corelibs/dataaccess \
		../../corelibs/kksutils \
		../../corelibs/kkspp \
		../../corelibs/kksdatafactory \
		../../corelibs/kkspluginmanager \
		../../mgrlibs/kkscoreapplication \
		../../mgrlibs/kksapplication \
                ../../guilibs/kksgui \
                ../../guilibs/kksqwt \
                ../../guilibs/kksguifactory \
                ../../openreport/kksorpcommon \
                ../../openreport/kksorprenderer \
                ../../openreport/kksorpwrtembed


LIBS += -L$$DESTDIR 
#����� ������� ���������� ��������� � ������!
LIBS += \
        -lkksorpwrtembed \
        -lkksorpcommon \
        -lkksorprenderer \
        -ldataaccess \
        -lkkscoreapplication \
        -lkksapplication \
        -lkksutils \
        -lkkspp \
        -lkksdatafactory \
        -lkksguifactory \
        -lkksgui \
        -lkksqwt \
        -lkkspluginmanager


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

QT += xml sql

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS += -lpq

RC_FILE = operator.rc


include(operator_app.pri)

target.path = $$KKS_EXEC_DIR
INSTALLS += target
