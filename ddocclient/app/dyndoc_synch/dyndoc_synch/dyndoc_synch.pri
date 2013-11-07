# dyndoc_synch
HEADERS += \
        config_dyndoc_synch.h \
        UserInterface/dyndocForm/dyndocForm.h \
        UserInterface/DataStream/DataStream.h \
        UserInterface/NetworkStream/NetworkStream.h \
        UserInterface/DataElements/viewElement.h \
        UserInterface/DataElements/dyndocModel.h \
        UserInterface/DataElements/dyndocView.h \
        UserInterface/DataElements/dyndoc_delegate.h \
        Namespaces/netStruct.h
        
SOURCES += \
        main.cpp \
        UserInterface/dyndocForm/dyndocForm.cpp \
        UserInterface/DataStream/DataStream.cpp \
        UserInterface/NetworkStream/NetworkStream.cpp \
        UserInterface/DataElements/viewElement.cpp \
        UserInterface/DataElements/dyndocModel.cpp \
        UserInterface/DataElements/dyndocView.cpp \
        UserInterface/DataElements/dyndoc_delegate.cpp \
        Namespaces/netStruct.cpp

FORMS   += \
        UserInterface/dyndocForm/dyndocForm.ui

headers.files += \
        config_scheduler.h \
        scheduler.h \
        schedulerform.h

QT += network
