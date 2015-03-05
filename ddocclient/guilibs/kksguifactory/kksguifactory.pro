# factory

TEMPLATE = lib
TARGET = kksguifactory

DESTDIR = ../../build
include(../../ddocclient.conf)
#libraries support
INCLUDEPATH *= \
                ../../corelibs/kksutils \
                ../../corelibs/dataaccess \
                ../../corelibs/kkspp \
                ../../corelibs/kkspluginmanager \
                ../../corelibs/kksdatafactory \
                ../kksgui \
                ../kksgui/ui 

DEPENDPATH *= \
                ../../corelibs/kksutils \
                ../../corelibs/dataaccess \
                ../../corelibs/kkspp \
                ../../corelibs/kksdatafactory \
                ../../corelibs/kkspluginmanager \
                ../kksgui 

LIBS *= -L$$DESTDIR -lkksutils -ldataaccess -lkkspp -lkksgui -lkkspluginmanager -lkksdatafactory

unix {
#LIBS += -lcurses
}
CONFIG += qt

CONFIG += warn_on dll

QT += xml sql svg

DEFINES += __USE_DLL_KKSGUIFACTORY
#DEFINES -= UNICODE

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


include (kksguifactory.pri)

# install targets
# headers
headers.path = $$KKS_INCLUDE_DIR

# import lib
win32 {
	implib.path = $$KKS_LIB_DIR
	implib.files = $$sprintf("%1/%2.lib", $$DESTDIR, $$TARGET)

	# lib
	target.path = $$KKS_BIN_DIR
	debug {
		target.files = \
			$$sprintf("%1/%2.dll", $$DESTDIR, $$TARGET) \
			$$sprintf("%1/%2.pdb", $$DESTDIR, $$TARGET) \
			$$sprintf("%1/%2.ilk", $$DESTDIR, $$TARGET) \
			$$sprintf("%1/%2.exp", $$DESTDIR, $$TARGET)
	}
	release {
		target.files = \
			$$sprintf("%1/%2.dll", $$DESTDIR, $$TARGET)
	}
}
unix {
	target.path = $$KKS_LIB_DIR 
}


INSTALLS += target headers
win32{
	INSTALLS += implib
}
