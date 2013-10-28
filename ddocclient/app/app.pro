TEMPLATE = subdirs

# Directories
SUBDIRS += \
	operator_app \
	http_connector \
        sync_queue_viewer \
        #dyndoc_synch \
        kksdaemon

win32{
        SUBDIRS += \
                   db_synchronizer
}

#SUBDIRS += kaps_manager
