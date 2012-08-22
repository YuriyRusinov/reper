# This is part of KKSSITO. Task db_synchronizer
# author ksa

TEMPLATE = app
TARGET = db_synchronizer
include(../../ddocclient.conf)

DESTDIR = ../../build

QT += xml

INCLUDEPATH += 	. \
		../../dataaccess \
		../../kkscore/base_classes \
		../../kkscore \
                ../../kksfactory \
		../../kksutils \
		../../kkspp 


DEPENDPATH += 	\
		. \
		../../dataaccess \
                ../../kksfactory \
		../../kkscore \
		../../kksutils \
		../../kkspp

INCLUDEPATH += ./ext_libs/include \
               ./src 

LIBS +=  -L./ext_libs/release/ \
         -L../../build/ \
         -ldataaccess \
         -lkksfactory \
         -lkksutils\
         -lkkscore \
         -lapi_bdfz

# Input
HEADERS += src/db_synchronizer.h \
           src/siu_connect_form.h

SOURCES += src/main.cpp \
           src/db_synchronizer.cpp \
           src/db_synchronizer_dd1.cpp \
           src/db_synchronizer_dd2.cpp \
           src/db_synchronizer_siu1.cpp \
           src/siu_connect_form.cpp 

FORMS += src/db_synchronizer_form.ui \
         src/siu_connect_form.ui

target.files *= $${DISTFILES}
target.path = $$KKS_EXEC_DIR
INSTALLS += target
