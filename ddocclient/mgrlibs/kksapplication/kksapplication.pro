# kkscore

TEMPLATE = lib
TARGET = kksapplication

DESTDIR = ../../build
include(../../ddocclient.conf)

#utils support
INCLUDEPATH *= \
               ../../corelibs/kksutils \
               ../../corelibs/dataaccess \
               ../../corelibs/kkspp \
               ../../corelibs/kkspluginmanager \
               ../../corelibs/kksdatafactory \
               ../../guilibs/kksguifactory \
               ../../guilibs/kksgui \
               ../../guilibs/kksgui/ui \
               ../kkscoreapplication \
               ../kkscoreapplication/base_classes \
               ../kkscoreapplication/ui

DEPENDPATH *= \
               ../../corelibs/kksutils \
               ../../corelibs/dataaccess \
               ../../corelibs/kkspp \
               ../../corelibs/kkspluginmanager \
               ../../corelibs/kksdatafactory \
               ../../guilibs/kksguifactory \
               ../kkscoreapplication \
               ../../guilibs/kksgui
               

LIBS *= -L$$DESTDIR -lkksutils -ldataaccess -lkksdatafactory -lkksguifactory -lkkspp -lkksgui -lkkspluginmanager -lkkscoreapplication

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
        LIBS *= -L$$OSGEODIR/lib  -lproj -lgdal -lsqlite3
    }
}


# libpq support
LIBS += -L$${PSQL_LIB_DIR}
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS += -lpq

CONFIG += qt

CONFIG += warn_on dll

DEFINES *= __USE_DLL_KKSAPP

win32:LITERAL_QUOTE = " "
unix:LITERAL_QUOTE = \'
win32:LITERAL_QUOTE3 = \"
unix:LITERAL_QUOTE3 = \"

CONFIG_NAME_FILE = kksclient_name.h

!exists($$CONFIG_NAME_FILE) {

	CURRENT_STRING = $$sprintf("echo %1%2ifndef KKSCLIENT_NAME_H%1 >> %3", $$LITERAL_QUOTE, $$LITERAL_HASH, $$CONFIG_NAME_FILE)
	system($$CURRENT_STRING)
	CURRENT_STRING = $$sprintf("echo %1%2define KKSCLIENT_NAME_H%1 >> %3", $$LITERAL_QUOTE, $$LITERAL_HASH, $$CONFIG_NAME_FILE)
	system($$CURRENT_STRING)
	CURRENT_STRING = $$sprintf("echo %1%2define EXECUTABLE_PREFIX %5%3%5 %1 >> %4", $$LITERAL_QUOTE, $$LITERAL_HASH, $$EXECUTABLE_PREFIX, $$CONFIG_NAME_FILE, $$LITERAL_QUOTE3)
	system($$CURRENT_STRING)
	CURRENT_STRING = $$sprintf("echo %1%2define ORGANIZATION %5%3%5 %1 >> %4", $$LITERAL_QUOTE, $$LITERAL_HASH, $$ORGANIZATION, $$CONFIG_NAME_FILE, $$LITERAL_QUOTE3)
	system($$CURRENT_STRING)
        CURRENT_STRING = $$sprintf("echo %1%2define PRODUCT_FULL_NAME %6%3 %4%6 %1 >> %5", $$LITERAL_QUOTE, $$LITERAL_HASH, $$PRODUCT_NAME, $$KKS_VERSION, $$CONFIG_NAME_FILE, $$LITERAL_QUOTE3)
        system($$CURRENT_STRING)
        CURRENT_STRING = $$sprintf("echo %1%2define PRODUCT_NAME %5%3%5 %1 >> %4", $$LITERAL_QUOTE, $$LITERAL_HASH, $$PRODUCT_NAME, $$CONFIG_NAME_FILE, $$LITERAL_QUOTE3)
        system($$CURRENT_STRING)
	CURRENT_STRING = $$sprintf("echo %1%2define KKS_VERSION %5%3%5 %1 >> %4", $$LITERAL_QUOTE, $$LITERAL_HASH, $${KKS_VERSION}, $$CONFIG_NAME_FILE, $$LITERAL_QUOTE3)
#	system($$CURRENT_STRING)
	CURRENT_STRING = $$sprintf("echo %1%2define KKS_VERSION %3", $$LITERAL_QUOTE, $$LITERAL_HASH, $$LITERAL_QUOTE3)
	CURRENT_STRING = $$sprintf("%1%2%4 %5>> %3", $$CURRENT_STRING, $$KKS_VERSION, $$CONFIG_NAME_FILE, $$LITERAL_QUOTE3, $$LITERAL_QUOTE)
	system($$CURRENT_STRING)
	CURRENT_STRING = $$sprintf("echo %1%2endif%1 >> %3", $$LITERAL_QUOTE, $$LITERAL_HASH, $$CONFIG_NAME_FILE)
	system($$CURRENT_STRING)
}

include (kksapplication.pri)

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
