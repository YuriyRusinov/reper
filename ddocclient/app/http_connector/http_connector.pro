# http-client KKSSITO. 

TEMPLATE = app
TARGET = http_connector

DESTDIR = ../../build
include(../../ddocclient.conf)

INCLUDEPATH += 	. \
		../../dataaccess \
		../../kkscore/base_classes \
		../../kkscore \
                ../../kkspp \
                ../../kksfactory \
		../../kksutils \
                ../../kksinteractor \
                ../../kkshttpconnector \
                ./include

DEPENDPATH += 	. \
		../../dataaccess \
		../../kkscore \
		../../kkscore/base_classes \
                ../../kkspp \
                ../../kksfactory \
		../../kksutils \
                ../../kksinteractor \
                ../../kkshttpconnector \
                ./gost

LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkkscore -lkksfactory -lkkspp -lkksgui -lkksinteractor -lkkshttpconnector

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
DISTFILES *= $${DESTDIR}/interactor.ini

target.files *= $${DISTFILES}
target.path = $$KKS_EXEC_DIR
INSTALLS += target

