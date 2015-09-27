# interactor

TEMPLATE = lib
TARGET = kksinteractor

DESTDIR = ../../build
include(../../io_client.conf)
#
#utils support
#
INCLUDEPATH *= \
                ../../corelibs/kksutils \
                ../../corelibs/dataaccess \
		../../mgrlibs/kkscoreapplication/base_classes \
		../../mgrlibs/kkscoreapplication/ui \
		../../mgrlibs/kkscoreapplication \

DEPENDPATH *= \
                ../../corelibs/kksutils \
                ../../corelibs/dataaccess \
		../../mgrlibs/kkscoreapplication

LIBS *= -L$$DESTDIR -lkksutils -ldataaccess -lkkscoreapplication


INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

LIBS *= -L$${PSQL_LIB_DIR}
win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_LIB_DIR}/libpq.dll.a
}
unix: LIBS *= -lpq

#INCLUDEPATH += ../kkscore ../kkscore/base_classes
#DEPENDPATH += ../kkscore ../kkscore/base_classes
#LIBS += -L$$DESTDIR -lkkscore

unix {
#LIBS += -lcurses
}
CONFIG += qt
QT += xml

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
