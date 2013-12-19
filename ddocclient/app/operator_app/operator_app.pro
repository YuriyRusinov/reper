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
		../../kksgui/ui


DEPENDPATH += 	\
		. \
		../../dataaccess \
		../../kkscore \
		../../kksutils \
		../../kkspp \
		../../kksfactory \
                ../../kksgui

INCLUDEPATH *= ../../rptrenderer \
               ../../rptrenderer/src

DEPENDPATH *= ../kkspp \
              ../../rptrenderer

LIBS += -L$$DESTDIR -ldataaccess -lkkscore -lkksutils -lkkspp -lkksfactory -lkksgui -lrptrenderer


qgis{
    DEFINES *= __USE_QGIS__
    DEFINES *= __USE_EXPORTS  #for qgis export library

    LIBS *= -L$$QGISDIR/lib -lqgis_core -lqgis_gui
    LIBS *= -L$$DESTDIR -lkksgis

    INCLUDEPATH *= $$OSGEODIR/include \
                   $$OSGEODIR/include/qwt \
                   $$QGISDIR/include \
                   ../../kksgis \
                   ../../kksgis/layerprops \
                   ../../kksgis/legend

    DEPENDPATH += ../../kksgis

    LIBS *= -L$$OSGEODIR/lib  -lproj_i -lgdal_i
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
