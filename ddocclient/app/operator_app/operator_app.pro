TEMPLATE = app

include(../../ddocclient.conf)

TARGET = $${EXECUTABLE_PREFIX}_app

DESTDIR = ../../build

INCLUDEPATH += 	. \
		../../dataaccess \
		../../kkscore/base_classes \
		../../kkscore/ui \
		../../kkscore \
		../../kksutils \
		../../kksfactory \
		../../kkspp \
                ../../kksgui \
		../../kksgui/ui \
                ../../kksqwt \
                ../../kksqwt/src


DEPENDPATH += 	\
		. \
		../../dataaccess \
		../../kkscore \
		../../kksutils \
		../../kkspp \
		../../kksfactory \
                ../../kksgui \
                ../../kksqwt

INCLUDEPATH *= ../../rptrenderer \
               ../../rptrenderer/src

DEPENDPATH *= ../kkspp \
              ../../rptrenderer

LIBS += -L$$DESTDIR -ldataaccess -lkkscore -lkksutils -lkkspp -lkksfactory -lkksgui -lrptrenderer -lkksqwt


qgis{
    DEFINES *= __USE_QGIS__
    DEFINES *= __USE_EXPORTS  #for qgis export library

    LIBS *= -L$$QGISDIR/lib -lqgis_core -lqgis_gui
    LIBS *= -L$$DESTDIR -lkksqgis

    INCLUDEPATH *= \
                   $$OSGEODIR/include \
                   $$OSGEODIR/include/qwt \
                   $$QGISDIR/include \
                   ../../kksqgis \
                   ../../kksqgis/layerprops \
                   ../../kksqgis/legend \
                   ../../kksqgis/projectprops \
                   ../../kksqgis/maptools

    DEPENDPATH += ../../kksqgis

    LIBS *= -L$$OSGEODIR/lib  -lproj_i -lgdal_i -lsqlite3_i
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

RC_FILE = operator.rc


include(operator_app.pri)

target.path = $$KKS_EXEC_DIR
INSTALLS += target
