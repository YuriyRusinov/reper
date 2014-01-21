# kksqgis

TEMPLATE = lib
TARGET = kksqgis

DESTDIR = ../build
include(../ddocclient.conf)

CONFIG += qt

CONFIG += warn_on dll

QT += core gui xml svg xmlpatterns webkit network

DEFINES += __USE_DLL_KKSQGIS
#DEFINES -= UNICODE

#INCLUDEPATH *= . 
INCLUDEPATH *=  . \
                dn \
                legend \
                images \
                layerprops \
                projectprops \
                maptools


qgis{
    DEFINES *= __USE_QGIS__
    DEFINES *= __USE_EXPORTS  #for qgis export library

    LIBS *= -L$$QGISDIR/lib -lqgis_core -lqgis_gui

    INCLUDEPATH *= $$OSGEODIR/include \
                   $$OSGEODIR/include/qwt \
                   $$QGISDIR/include


    LIBS *= -L$$OSGEODIR/lib  -lproj_i -lgdal_i -lsqlite3_i
}


include (kksqgis.pri)

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
