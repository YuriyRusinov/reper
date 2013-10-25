# http-client KKSSITO. 

TEMPLATE = app
TARGET = dds_http_connector

DESTDIR = ../../../build
include(../../../ddocclient.conf)

INCLUDEPATH += 	. \
		../../../dataaccess \
		../../../kkscore/base_classes \
		../../../kkscore \
                ../../../kkspp \
                ../../../kksfactory \
		../../../kksutils \
                ../../../kksinteractor \
                ./include
#                /usr/local/kkssito/include\
#                /usr/local/pgsql/include

DEPENDPATH += 	. \
		../../../dataaccess \
		../../../kkscore \
		../../../kkscore/base_classes \
                ../../../kkspp \
                ../../../kksfactory \
		../../../kksutils \
                ../../../kksinteractor \
                ./gost

LIBS += -L/usr/local/kkssito/lib\
#        -L./gost\ 
        -L$$DESTDIR -ldataaccess -lkksutils -lkkscore -lkksfactory -lkkspp -lkksgui -lkksinteractor

# libpq support
LIBS += -L$${PSQL_HOME}/lib
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS *= -lpq
#unix: LIBS *= -L./gost -lgost

# Input
HEADERS += src/httpwindow.h \
           src/timerform.h \
           src/transportsettingsform.h \ 
           src/netThread.h
#           src/cryptmessage.h

SOURCES += src/httpwindow.cpp \
           src/timerform.cpp \	
           src/transportsettingsform.cpp \
           src/netThread.cpp \
           src/main.cpp #\
#           src/cryptmessage.cpp

FORMS += src/authenticationdialog.ui \
         src/transport_settings_form.ui \
	   src/timer_form.ui	

QT += network

!exists ($${DESTDIR}/http.ini) {
system (cp http.ini.example $${DESTDIR}/http.ini)
}
DISTFILES *= $${DESTDIR}/interactor.ini

target.files *= $${DISTFILES}
target.path = $$KKS_EXEC_DIR
INSTALLS += target

