TEMPLATE = subdirs

# Directories
SUBDIRS += \
	operator_app \
	http_connector \
        sync_queue_viewer \
        dyndoc_synch \
        sync_queue_viewer \
        kksdaemon

win32{
        SUBDIRS += \
                   db_synchronizer
}

#SUBDIRS += kaps_manager
