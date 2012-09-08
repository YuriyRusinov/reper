TEMPLATE = subdirs

# Directories
SUBDIRS += \
	operator_app \
	http_connector 
win32{
        SUBDIRS += db_synchronizer
}

SUBDIRS += kaps_manager
