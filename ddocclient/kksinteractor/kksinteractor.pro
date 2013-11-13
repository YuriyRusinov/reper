# interactor

TEMPLATE = lib
TARGET = kksinteractor

DESTDIR = ../build
include(../ddocclient.conf)
#
#utils support
#
INCLUDEPATH *= ../kksutils
DEPENDPATH *= ../kksutils
LIBS *= -L$$DESTDIR -lkksutils
#
#dataaccess support
#
INCLUDEPATH *= ../dataaccess
DEPENDPATH *= ../dataaccess
LIBS *= -L$$DESTDIR -ldataaccess

INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

LIBS *= -L$${PSQL_LIB_DIR}
win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_LIB_DIR}/libpq.dll.a
}
unix: LIBS *= -lpq

#
#kksgui support
#
#INCLUDEPATH += ../kksgui ../kksgui/ui
#DEPENDPATH += ../kksgui
#LIBS += -L$$DESTDIR -lkksgui

#
#kkscore support
#
INCLUDEPATH += ../kkscore ../kkscore/base_classes
DEPENDPATH += ../kkscore ../kkscore/base_classes
LIBS += -L$$DESTDIR -lkkscore

unix {
LIBS += -lcurses
}
CONFIG += qt

CONFIG += warn_on dll

DEFINES += __USE_DLL_KKSINTERACTOR
#DEFINES -= UNICODE

include (kksinteractor.pri)

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
