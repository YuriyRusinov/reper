# Radio Image for IO plugin

TEMPLATE = lib
CONFIG += plugin dll
TARGET = radio_image

DESTDIR = ../../build/ddocplugins
DEPLIBS_DIR = ../../build

include(../../io_client.conf)

INCLUDEPATH += 	. \
		../../corelibs/dataaccess \
		../../corelibs/kksutils \
		../../corelibs/kksdatafactory \
		../../corelibs/kkspp \
                ../../corelibs/kkspluginmanager \
		../../mgrlibs/kkscoreapplication/base_classes \
		../../mgrlibs/kkscoreapplication/ui \
		../../mgrlibs/kkscoreapplication

DEPENDPATH += 	\
		../../corelibs/dataaccess \
		../../corelibs/kksutils \
		../../corelibs/kksdatafactory \
		../../corelibs/kkspp \
                ../../corelibs/kkspluginmanager \
		../../mgrlibs/kkscoreapplication

LIBS += -L$$DEPLIBS_DIR -ldataaccess -lkkscoreapplication -lkksutils -lkkspp -lkksdatafactory -lkkspluginmanager

include(radio_image.pri)

win32 {
    # lib
    target.path = $$KKS_PLUGIN_DIR
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
    target.path = $$KKS_PLUGIN_DIR
}

INSTALLS += target

#win32 {
#    INSTALLS += implib
#}
