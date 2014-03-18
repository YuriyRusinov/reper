# factory

TEMPLATE = lib
TARGET = kksfactory

DESTDIR = ../build
include(../ddocclient.conf)
#libraries support
INCLUDEPATH *= ../kksutils ../dataaccess ../kkspp ../kksgui ../kksgui/ui ../kkscore ../kkscore/base_classes
DEPENDPATH *= ../kksutils ../dataaccess ../kkspp ../kksgui ../kkscore

#librenderer support
INCLUDEPATH *= ../rptrenderer \
               ../rptrenderer/src

DEPENDPATH *= ../kkspp \
              ../rptrenderer

#INCLUDEPATH *= /home/rusinov/schedule/openrpt_git/openrpt/trunk/common \
#               /home/rusinov/schedule/openrpt_git/openrpt/trunk/OpenRPT/renderer \
#               /home/rusinov/schedule/openrpt_git/openrpt/trunk/OpenRPT/bldtbls \
#               /home/rusinov/schedule/openrpt_git/openrpt/trunk/OpenRPT/common \
#               /home/rusinov/schedule/openrpt_git/openrpt/trunk/OpenRPT/wrtembed

#DEPENDPATH *=  /home/rusinov/schedule/openrpt_git/openrpt/trunk/common \
#               /home/rusinov/schedule/openrpt_git/openrpt/trunk/OpenRPT/renderer \
#               /home/rusinov/schedule/openrpt_git/openrpt/trunk/OpenRPT/bldtbls \
#               /home/rusinov/schedule/openrpt_git/openrpt/trunk/OpenRPT/common \
#               /home/rusinov/schedule/openrpt_git/openrpt/trunk/OpenRPT/wrtembed
#
#LIBS *= -L/home/rusinov/schedule/openrpt_git/openrpt/trunk/lib/librenderer.a
## -lrenderer

LIBS *= -L$$DESTDIR -lkksutils -ldataaccess -lkkspp -lkksgui -lrptrenderer

unix {
LIBS += -lcurses
}
CONFIG += qt

CONFIG += warn_on dll

QT += xml sql svg

DEFINES += __USE_DLL_KKSFACTORY
#DEFINES -= UNICODE

qgis{
    DEFINES *= __USE_QGIS__
    DEFINES *= __USE_EXPORTS  #for qgis export library

    LIBS *= -L$$QGISDIR/lib -lqgis_core -lqgis_gui
    LIBS *= -L$$DESTDIR -lkksqgis

    INCLUDEPATH *= \
                   $$OSGEODIR/include \
                   $$OSGEODIR/include/qwt \
                   $$QGISDIR/include \
                   $$QGISDIR/include/qgis \
                   ../kksqgis \
                   ../kksqgis/layerprops \
                   ../kksqgis/legend \
                   ../kksqgis/projectprops \
                   ../kksqgis/maptools

    DEPENDPATH *= ../kksqgis

    win32{
        LIBS *= -L$$OSGEODIR/lib  -lproj_i -lgdal_i -lsqlite3_i
    }
    else{
        LIBS *= -L$$OSGEODIR/lib  -lproj -lgdal -lsqlite3
    }
}


include (kksfactory.pri)

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
