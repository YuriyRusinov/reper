TEMPLATE = subdirs

include(../io_client.conf)

# Directories
SUBDIRS += \
        kksutils \
	dataaccess \
        kkspp \
        kkspluginmanager \
        kksjsonparser \
	kksdatafactory
