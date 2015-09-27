# kkspp

TEMPLATE = lib
TARGET = kksrunservice

DESTDIR = ../../build
include(../../io_client.conf)
#utils support
#INCLUDEPATH *= ../kksutils ../dataaccess
#DEPENDPATH *= ../kksutils ../dataaccess
#LIBS *= -L$$DESTDIR -lkksutils -ldataaccess

CONFIG += qt
QT -= gui
!win32:QT += network
win32:LIBS += -luser32

CONFIG += warn_on dll

#DEFINES += __USE_DLL_KKSRUNSERVICE
DEFINES += QT_QTSERVICE_EXPORT

INCLUDEPATH += . \
               ./src

include (./kksrunservice.pri)

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


