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

CONFIG += qt

CONFIG += warn_on dll

QT += xml svg xmlpatterns

DEFINES += __USE_DLL_KKSGUI
#DEFINES -= UNICODE

INCLUDEPATH *= . \
               ./ui

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
