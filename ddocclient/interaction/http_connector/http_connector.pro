# http-client KKSSITO. 

TEMPLATE = app
TARGET = http_connector

DESTDIR = ../../build
include(../../ddocclient.conf)

INCLUDEPATH += 	. \
		../../corelibs/dataaccess \
                ../../corelibs/kkspp \
                ../../corelibs/kksdatafactory \
		../../corelibs/kksutils \
		../../corelibs/kkspluginmanager \
		../../mgrlibs/kkscoreapplication/base_classes \
		../../mgrlibs/kkscoreapplication/ui \
		../../mgrlibs/kkscoreapplication \
                ../kksinteractor \
                ../kkshttpconnector \
                ./include

DEPENDPATH += 	. \
		../../corelibs/dataaccess \
                ../../corelibs/kkspp \
                ../../corelibs/kksdatafactory \
		../../corelibs/kksutils \
		../../corelibs/kkspluginmanager \
		../../mgrlibs/kkscoreapplication \
                ../kksinteractor \
                ../kkshttpconnector

LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkkscoreapplication -lkksdatafactory -lkkspp -lkksinteractor -lkkshttpconnector -lkkspluginmanager

# libpq support
LIBS += -L$${PSQL_HOME}/lib
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS *= -lpq

include (./http_connector.pri)

QT += network

!exists ($${DESTDIR}/http.ini) {
system (cp http.ini.example $${DESTDIR}/http.ini)
}

DISTFILES = $${DESTDIR}/http_connector $${DESTDIR}/http.ini

target.files = $${DISTFILES}
target.path = $$KKS_EXEC_DIR
INSTALLS += target
