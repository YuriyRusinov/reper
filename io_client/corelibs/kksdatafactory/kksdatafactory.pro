# factory

TEMPLATE = lib
TARGET = kksdatafactory

DESTDIR = ../../build
include(../../ddocclient.conf)
#libraries support
INCLUDEPATH *= ../kksutils ../dataaccess ../kkspp 
DEPENDPATH *= ../kksutils ../dataaccess ../kkspp 

LIBS *= -L$$DESTDIR -lkksutils -ldataaccess -lkkspp 

CONFIG += qt

CONFIG += warn_on dll

QT += xml sql svg
#QT -= gui

DEFINES += __USE_DLL_KKSDATAFACTORY
#DEFINES -= UNICODE

include (kksdatafactory.pri)

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
