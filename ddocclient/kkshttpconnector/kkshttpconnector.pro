# kkspp

TEMPLATE = lib
TARGET = kkshttpconnector

DESTDIR = ../build
include(../ddocclient.conf)

CONFIG += qt
CONFIG += warn_on dll
QT += network

DEFINES *= __USE_DLL_KKSHTTPCONNECTOR

INCLUDEPATH += 	. \
		../dataaccess \
		../kkscore/base_classes \
		../kkscore \
                ../kkspp \
                ../kksfactory \
		../kksutils \
                ../kksinteractor

DEPENDPATH += 	. \
		../dataaccess \
		../kkscore \
		../kkscore/base_classes \
                ../kkspp \
                ../kksfactory \
		../kksutils \
                ../kksinteractor

LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkkscore -lkksfactory -lkkspp -lkksinteractor

# libpq support
LIBS += -L$${PSQL_LIB_DIR}
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS += -lpq

include (./kkshttpconnector.pri)

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

##############
#!exists ($${DESTDIR}/http.ini) {
#system (cp http.ini.example $${DESTDIR}/http.ini)
#}
#DISTFILES *= $${DESTDIR}/interactor.ini
