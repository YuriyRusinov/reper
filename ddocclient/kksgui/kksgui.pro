# kksgui

TEMPLATE = lib
TARGET = kksgui

DESTDIR = ../build
include(../ddocclient.conf)
#utils support
INCLUDEPATH *= ../kksutils
DEPENDPATH *= ../kksutils
LIBS *= -L$$DESTDIR -lkksutils
#pp support
INCLUDEPATH *= ../kkspp
DEPENDPATH *= ../kkspp
LIBS *= -L$$DESTDIR -lkkspp

INCLUDEPATH *= ../kksqwt
DEPENDPATH *= ../kksqwt
LIBS *= -L$$DESTDIR -lkksqwt


CONFIG += qt

CONFIG += warn_on dll

QT += core gui xml svg xmlpatterns

DEFINES += __USE_DLL_KKSGUI
#DEFINES -= UNICODE

INCLUDEPATH *= . \
               ./ui


qgis{
    DEFINES *= __USE_QGIS__
    DEFINES *= __USE_EXPORTS  #for qgis export library

    LIBS *= -L$$QGISDIR/lib -lqgis_core -lqgis_gui
    LIBS *= -L$$DESTDIR -lkksqgis

    INCLUDEPATH *= \
                   $$OSGEODIR/include \
                   $$OSGEODIR/include/qwt \
                   $$QGISDIR/include \
                   ../kksqgis \
                   ../kksqgis/layerprops \
                   ../kksqgis/legend \
                   ../kksqgis/projectprops \
                   ../kksqgis/maptools

    DEPENDPATH *= ../kksqgis

    LIBS *= -L$$OSGEODIR/lib  -lproj_i -lgdal_i -lsqlite3_i
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
