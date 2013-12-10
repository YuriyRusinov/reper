TEMPLATE = subdirs

# Directories
SUBDIRS += \
	operator_app \
	http_connector \
#        sync_queue_viewer \
        dyndoc_synch \
        kksdaemon

include(../ddocclient.conf)

qgis{
    SUBDIRS += reper_ksa
}

#win32{
#        SUBDIRS += \
#                   db_synchronizer
#}

#SUBDIRS += kaps_manager
