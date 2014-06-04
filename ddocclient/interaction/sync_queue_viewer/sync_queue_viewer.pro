# http-client KKSSITO. 

TEMPLATE = lib
TARGET = sync_queue_viewer

DESTDIR = ../../build
include(../../ddocclient.conf)

CONFIG += qt
CONFIG += warn_on dll
QT += network

DEFINES += __USE_DLL_SYNCQUEUEVIEWER

INCLUDEPATH += 	. \
               ../../corelibs/dataaccess \
               ../../corelibs/kkspp \
               ../../corelibs/kksdatafactory \
               ../../corelibs/kksutils \
               ../../corelibs/kkspluginmanager \
               ../../mgrlibs/kkscoreapplication/base_classes \
               ../../mgrlibs/kkscoreapplication/ui \
               ../../mgrlibs/kkscoreapplication 

DEPENDPATH += 	. \
               ../../corelibs/dataaccess \
               ../../corelibs/kkspp \
               ../../corelibs/kkspluginmanager \
               ../../corelibs/kksdatafactory \
               ../../corelibs/kksutils \
               ../../mgrlibs/kkscoreapplication 

LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkkscoreapplication -lkksdatafactory -lkkspp -lkkspluginmanager

# libpq support
LIBS += -L$${PSQL_HOME}/lib
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS *= -lpq

# Input
HEADERS += \
           ./syncqueueviewerform.h \
           ./syncqueueview.h \
           ./syncqueueitemmodel.h \
           ./filtersform.h \
           ./sqv_delegate.h \
           dataelement.h \
           sqv_datastream.h \
           kkssqv_config.h

SOURCES += \
           ./syncqueueviewerform.cpp \
           ./syncqueueview.cpp \
           ./syncqueueitemmodel.cpp \
           #./main.cpp \
           ./filtersform.cpp \
           ./sqv_delegate.cpp \
           dataelement.cpp \
           sqv_datastream.cpp

#FORMS += 

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

#target.files *= $${DISTFILES}
#target.path = $$KKS_EXEC_DIR
#INSTALLS += target

