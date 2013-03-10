

    HEADERS       += ./qtservice.h \
                     ./qtservice_p.h
    SOURCES       += ./qtservice.cpp

    win32:SOURCES += ./qtservice_win.cpp
    unix:HEADERS  += ./qtunixsocket.h ./qtunixserversocket.h
    unix:SOURCES  += ./qtservice_unix.cpp ./qtunixsocket.cpp ./qtunixserversocket.cpp

