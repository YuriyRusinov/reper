TEMPLATE = app

include(../../ddocclient.conf)

TARGET = reper_app

DESTDIR = ../../build

INCLUDEPATH += 	. \
		../../corelibs/dataaccess \
		../../corelibs/kksutils \
		../../corelibs/kksdatafactory \
		../../corelibs/kkspp \
                ../../corelibs/kkspluginmanager \
		../../mgrlibs/kkscoreapplication/base_classes \
		../../mgrlibs/kkscoreapplication/ui \
		../../mgrlibs/kkscoreapplication \
		../../mgrlibs/kksapplication \
                ../../ddocplugins/radio_image


DEPENDPATH += 	\
		../../corelibs/dataaccess \
		../../corelibs/kksutils \
		../../corelibs/kksdatafactory \
		../../corelibs/kkspp \
                ../../corelibs/kkspluginmanager \
		../../mgrlibs/kkscoreapplication \
		../../mgrlibs/kksapplication \
                ../../ddocplugins/radio_image


LIBS += -L$$DESTDIR -ldataaccess -lkkscoreapplication -lkksutils -lkkspp -lkksdatafactory -lkkspluginmanager  -lkksapplication
#LIBS *= -L$$DESTDIR/ddocplugins -lradio_image


qgis{
    DEFINES *= __USE_QGIS__
    DEFINES *= __USE_EXPORTS  #for qgis export library

    LIBS *= -L$$QGISDIR/lib -lqgis_core -lqgis_gui
    LIBS *= -L$$DESTDIR -lkksqgis

    INCLUDEPATH *= \
                   $$OSGEODIR/include \
                   $$OSGEODIR/include/qwt \
                   $$QGISDIR/include/qgis \
                   $$QGISDIR/include \
                   ../../kksqgis/kksqgis \
                   ../../kksqgis/kksqgis/layerprops \
                   ../../kksqgis/kksqgis/legend \
                   ../../kksqgis/kksqgis/projectprops \
                   ../../kksqgis/kksqgis/maptools

    DEPENDPATH += ../../kksqgis/kksqgis

    win32{
        LIBS *= -L$$OSGEODIR/lib  -lproj_i -lgdal_i -lsqlite3_i
    }
    else{
        LIBS *= -L$$OSGEODIR/lib  -lproj -lgdal -lsqlite3 -lgeos_c -lspatialindex -lqwt -lspatialite
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

#RC_FILE = operator.rc


include(reper_app.pri)

target.path = $$KKS_EXEC_DIR
INSTALLS += target
