# This is part of KKSSITO. Task db_synchronizer
# author ksa

TEMPLATE = app
TARGET = kaps_manager
include(../../ddocclient.conf)

DESTDIR = ../../build

#QT += xml

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

INCLUDEPATH += . 

LIBS +=  -L../../build/ \
         -ldataaccess \
         -lkksfactory \
         -lkksutils\
         -lkkscore \
         -lkkspp \
         -lkksgui 

# Input
HEADERS += kaps_manager_form.h 

SOURCES += main.cpp \
           kaps_manager_form.cpp 

FORMS += kaps_manager_form.ui 

target.files *= $${DISTFILES}
target.path = $$KKS_EXEC_DIR
INSTALLS += target
