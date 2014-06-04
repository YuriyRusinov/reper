# This is part of KKSSITO. Task mail for 'Jupiter'
# author saa

TEMPLATE = app
TARGET = mail

DESTDIR = .

INCLUDEPATH += ./include . \
                /usr/local/kkssito/include\
                /usr/local/pgsql/include

LIBS +=  -L/usr/local/kkssito/lib\
         -L../gost\
         -ldataaccess\
         -lkksutils\
         -lkkscore\
         -lkksinteractor\
         -lplot\
         -lgost\
         -lkksfactory\
         -lkkspp\
         -lkksgui

# Input
HEADERS += src/plot.h \
           src/logger.h \
           src/cryptmessage.h

SOURCES += src/mail.cpp \
           src/logger.cpp \
           src/cryptmessage.cpp

MOC_DIR = ./.moc
OBJECTS_DIR = ./.obj
