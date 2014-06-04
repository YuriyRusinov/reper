# kksgui

TEMPLATE = lib
TARGET = kksgui

DESTDIR = ../../build
include(../../ddocclient.conf)
#utils support
INCLUDEPATH *= \
               ../../corelibs/kksutils \
               ../../corelibs/kkspp \
               ../kksqwt \
               ../kksqwt/src \
               ../kksqwt/textengines \
               . \
               ./ui
              


DEPENDPATH *= \
               ../../corelibs/kksutils \
               ../../corelibs/kkspp \
               ../kksqwt

 
LIBS *= -L$$DESTDIR -lkksutils -lkkspp -lkksqwt

CONFIG += qt

CONFIG += warn_on dll

QT += core gui xml svg xmlpatterns

DEFINES += __USE_DLL_KKSGUI

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

    DEPENDPATH *= ../../kksqgis/kksqgis

    win32{
        LIBS *= -L$$OSGEODIR/lib  -lproj_i -lgdal_i -lsqlite3_i
    }
    else{
        LIBS *= -L$$OSGEODIR/lib  -lproj -lgdal -lsqlite3
    }
}


include (kksgui.pri)

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
