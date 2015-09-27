#kksutils
TEMPLATE = lib
TARGET = kksutils

DESTDIR = ../../build
include(../../io_client.conf)


CONFIG_FILE = configEndians.h
#win32:LITERAL_QUOTE = " "
unix:LITERAL_QUOTE = \"
#win32:LITERAL_QUOTE3 = "
#unix:LITERAL_QUOTE3 = \\\"

!exists($$CONFIG_FILE) {
	unix{
		contains(USE_BIGENDIAN, 0){
			CURRENT_STRING = $$sprintf("echo %1%2undef WORDS_BIGENDIAN%1 >> %3", $$LITERAL_QUOTE, $$LITERAL_HASH, $$CONFIG_FILE)
		}
		else{
			
			CURRENT_STRING = $$sprintf("echo %1%2define WORDS_BIGENDIAN 1%1 >> %3", $$LITERAL_QUOTE, $$LITERAL_HASH, $$CONFIG_FILE)
		}
	}
	else{
		CURRENT_STRING = $$sprintf("echo %1%2undef WORDS_BIGENDIAN%1 >> %3", $$LITERAL_QUOTE, $$LITERAL_HASH, $$CONFIG_FILE)
	}
        message ($$CURRENT_STRING)
	system($$CURRENT_STRING)
}

INCLUDEPATH += .

CONFIG -= qt

CONFIG += warn_on dll

DEFINES += __USE_DLL_KKSUTILS
DEFINES -= UNICODE

include (kksutils.pri)

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
