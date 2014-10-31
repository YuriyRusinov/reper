#!/bin/bash

. ./user.config

VERSION=2.0.4

PROJECT_NAME=DynamicDocs

LINTER_PREFIX=$LINTER_ROOT
SYS_FILES=./system_files
INTERNALS=./internals

PASSWD=""

checkSystem()
{
    EXIST_PORT=`ps afx | grep postmaster | head -1 | grep -o $LINTER_PORT`
    if [ "$EXIST_PORT" = "$LINTER_PORT" ]; then
        printf "\n\nERROR! There are any postmaster process running on port $LINTER_PORT!\n"
        printf "Change LINTER_PORT parameter in user.config file\n"
        printf "Installation aborted!\n\n"
        exit 1
    fi
    
    EXIST_PORT=`ps afx | grep postmaster | head -1 | grep -o $LINTER_PORT`
    if [ "$EXIST_PORT" = "$LINTER_PORT" ]; then
        printf "\n\nERROR! There are any postmaster process running on port $LINTER_PORT!\n"
        printf "Change LINTER_PORT parameter in user.config file\n"
        printf "Installation aborted!\n\n"
        exit 1
    fi

    if test -e $LINTER_PREFIX; then
	#file exist
	
	if ! test -d $LINTER_PREFIX; then
	    printf "\n\nERROR! /full/path/to/$PROJECT_NAME Server MUST be a directory!\n"
            printf "Change LINTER_ROOT parameter in user.config file\n"
            printf "Installation aborted!\n\n"
            exit 1
	fi
    
        #exist and is a dir
	
        NFILES=`ls -a $LINTER_PREFIX | wc -l`
	if [ $NFILES != 2 ]; then
	    printf "/full/path/to/$PROJECT_NAME Server MUST be empty!\n"
            printf "Change LINTER_ROOT parameter in user.config file\n"
            printf "Installation aborted!\n\n"
            exit 1
	fi    
	
	PREV=`pwd -P`
	cd $LINTER_PREFIX
	LINTER_PREFIX=`pwd -P`
	cd $PREV
	
    fi
    
}

createPGPass()
{
    read -s -p "Input password for user $USER: " PASSWD
    printf "\n"
    printf "*:*:*:$USER:$PASSWD\n" > ~/.pgpass
    chmod 600 ~/.pgpass
}

deletePGPass()
{
    rm -f ~/.pgpass
}

show_complete()
{
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
    printf "                   $PROJECT_NAME Server installation complete.\n"
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
}

show_complete_full()
{
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
    printf "\nPostgreSQL Server and all libraries successfully installed.\n"
    printf "Now PostgreSQL Server is runnig on port $LINTER_PORT\n"
    printf "For automatical running PostgreSQL Server when operation system is starting up\n"
    printf "you can use ./system_files/postgresql script\n\n"
    
    printf "               $PROJECT_NAME Server full installation complete.\n"
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
}

show_error()
{
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
    printf "                   $PROJECT_NAME Server installation aborted.\n"
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
    printf "********************************************************************************\n"
}

show_usage()
{
    printf "\n\n"
    printf "$PROJECT_NAME Server $VERSION installer\n\n"
    printf "Usage: $1 [install|fullinstall|update|clean|help]"
    printf "\n\n"
    
    printf "Using without parameters is synonym for ./install.sh install\n\n\n"
    printf "install \t- \tBuild $PROJECT_NAME File Library\n"
    printf "\t\t\tInstall $PROJECT_NAME Server and create new database\n"    
    printf "\t\t\tNote that if the File Library was already compiled\n"
    printf "\t\t\tthey will be only linked and installed (make install)\n"
    printf "\n\n"
    
    printf "fullinstall \t- \tBuild and install PostgreSQL database server from sources.\n"
    printf "\t\t\tAlso next libraries will be installed from sources:\n"
    printf "\t\t\t\t- PROJ4 - cartographic projection software;\n"
    printf "\t\t\t\t- GEOS - Geometry Engine Open Source;\n"
    printf "\t\t\t\t- GDAL - GeoData Abstraction Layer;\n"
    printf "\t\t\t\t- GSL - GNU Scientific Library;\n"
    printf "\t\t\t\t- OSSP uuid - Universally Unique Identifier;\n"
    printf "\t\t\t\t- PostGIS - Geographic Information Systems Extensions to PostgreSQL.\n"
    printf "\t\t\tThen build $PROJECT_NAME File Library, install $PROJECT_NAME Server\n"
    printf "\t\t\tand create new database\n"
    printf "\n\n"
    
    printf "update \t\t- \tUpdate $PROJECT_NAME Server from previous to last version\n"
    printf "\t\t\tIf you have any data very significant to you in database of previous\n"
    printf "\t\t\tversion of $PROJECT_NAME Server, You can just update existing database.\n"
    printf "\t\t\tAll your data will be saved\n\n\n"
    
    printf "clean \t\t- \tOnly clean source of $PROJECT_NAME File Library\n\n\n"
    printf "help \t\t-\tShow this help\n"

    printf "\n\n"
}


PREV=`pwd -P`
cd $PREV
INSTALL_OK=0

COMMAND=$1;

if [ "$COMMAND" = "" ]; then
    COMMAND=install;
    echo -n "Continue install? (y or n) [n] :"

	read key
	
 	if [ "$key" = "y" -o "$key" = "Y" ]; then
	    printf "Start install...\n"	
        else
	    printf "Installation process cancelled!\n "
	    exit 1
	fi;
fi;

IO_FILES=$PGDATA_DIR/$FILE_ARCH

if [ "$ORG_UID" = "" ]; then
    ORG_UID="localorg_prefix"
fi;

case $COMMAND in
    clean)
    
	cd $PREV
	cd $INTERNALS/libfloader/src
	make -f Makefile clean
        rm -f ./kksversion.h

	cd $PREV
	exit 0
    ;;
    fullinstall)
        checkSystem
	createPGPass
        cd $SYS_FILES &&
        SYS_FILES_ABS=`pwd -P` &&
        cd $SYS_FILES_ABS &&
	chmod a+x ./build-src.sh &&
	./build-src.sh "$VERSION" "$LINTER_PREFIX" "$PGDATA_DIR" "$LINTER_PORT" "$SYS_FILES_ABS/src" "template1" "$USER" "$PASSWD" &&
	chmod a+x ./build.sh &&
	./build.sh "$VERSION" "$LINTER_PREFIX" "$PGDATA_DIR" "$FILE_ARCH" "0" "$AUTO_CREATE" "$PROJECT_NAME" &&
	chmod a+x ./createdb.sh &&
	./createdb.sh "$BASE" "$USER" "$LINTER_PORT" "$LINTER_PREFIX" "$ENCODING" "1" "$PASSWD" "$VERSION" "$LOCAL_ADDRESS" "$IO_FILES" "$USE_MODULES" "$IS_MAIN_ORG" "$ORG_NAME" "$LOCAL_PORT" "$USE_GATEWAY" "$ORG_UID" 2>$PREV/createdb.log &&
	chmod a+x ./createanalyzer.sh &&
	./createanalyzer.sh "$BASE" "$USER" "$LINTER_PORT" "$LINTER_PREFIX" "$PGDATA_DIR" &&
	chmod a+x ./createdumper.sh &&
	./createdumper.sh "$BASE" "$USER" "$LINTER_PORT" "$LINTER_PREFIX" "$PGDATA_DIR" &&
	INSTALL_OK=1
	deletePGPass

        if [ $INSTALL_OK = 1 ]; then
            show_complete_full
            exit 0
        fi
    ;;
    install)
	createPGPass
        cd $SYS_FILES &&
        SYS_FILES_ABS=`pwd -P` &&
        cd $SYS_FILES_ABS &&
	chmod a+x ./build.sh &&
	./build.sh "$VERSION" "$LINTER_PREFIX" "$PGDATA_DIR" "$FILE_ARCH" "0" "$AUTO_CREATE" "$PROJECT_NAME" &&
	chmod a+x ./createdb.sh &&
	./createdb.sh "$BASE" "$USER" "$LINTER_PORT" "$LINTER_PREFIX" "$ENCODING" "1" "$PASSWD" "$VERSION" "$LOCAL_ADDRESS" "$IO_FILES" "$USE_MODULES" "$IS_MAIN_ORG" "$ORG_NAME" "$LOCAL_PORT" "$USE_GATEWAY" "$ORG_UID" 2>$PREV/createdb.log &&
	chmod a+x ./createanalyzer.sh &&
	./createanalyzer.sh "$BASE" "$USER" "$LINTER_PORT" "$LINTER_PREFIX" "$PGDATA_DIR" &&
	chmod a+x ./createdumper.sh &&
	./createdumper.sh "$BASE" "$USER" "$LINTER_PORT" "$LINTER_PREFIX" "$PGDATA_DIR" &&
	INSTALL_OK=1
	deletePGPass
    ;;
    update)
	createPGPass
        cd $SYS_FILES &&	
	chmod a+x ./build.sh &&
	./build.sh "$VERSION" "$LINTER_PREFIX" "$PGDATA_DIR" "$FILE_ARCH" "1" "$AUTO_CREATE" "$PROJECT_NAME" &&
	chmod a+x ./updatedb.sh &&
	./updatedb.sh "$BASE" "$USER" "$LINTER_PORT" "$LINTER_PREFIX" "$ENCODING" "$PASSWD" "$VERSION" "$LOCAL_ADDRESS" "$IO_FILES" "$USE_MODULES" "$ORG_NAME" "$LOCAL_PORT" 2>$PREV/updatedb.log &&
	INSTALL_OK=1
	
	deletePGPass
    ;;
    *)
	show_usage $0
	exit 1
    ;;
esac

if [ $INSTALL_OK = 0 ]; then
    show_error
    exit 1
fi

if [ $INSTALL_OK = 1 ]; then
    show_complete
    exit 0
fi

exit 0
