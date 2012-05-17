#!/bin/bash

. ./user.config

VERSION=0.13.4

PROJECT_NAME=DynamicDocs

LINTER_PREFIX=$LINTER_ROOT
SYS_FILES=./system_files
INTERNALS=./internals

PASSWD=""

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
    printf "Usage: $1 [install|update|clean|help]"
    printf "\n\n"
    
    printf "Using without parameters is synonym for ./install.sh install\n\n\n"
    printf "install \t- \tBuild $PROJECT_NAME File Library\n"
    printf "\t\t\tInstall $PROJECT_NAME Server and create new database\n"    
    printf "\t\t\tNote that if the File Library was already compiled\n"
    printf "\t\t\tthey will be only linked and installed (make install)\n"
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

case $COMMAND in
    clean)
    
	cd $PREV
	cd $INTERNALS/libfloader/src
	make -f Makefile clean
        rm -f ./kksversion.h

	cd $PREV
	exit 0
    ;;
    install)
	createPGPass
        cd $SYS_FILES &&
	chmod a+x ./build.sh &&
	./build.sh "$VERSION" "$LINTER_PREFIX" "$PGDATA_DIR" "$FILE_ARCH" "0" "$AUTO_CREATE" "$PROJECT_NAME" &&
	chmod a+x ./createdb.sh &&
	./createdb.sh "$BASE" "$USER" "$LINTER_PORT" "$LINTER_PREFIX" "$ENCODING" "1" "$PASSWD" "$VERSION" "$LOCAL_ADDRESS" "$IO_FILES" "$USE_MODULES" "$IS_MAIN_ORG" 2>$PREV/createdb.log &&
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
	./updatedb.sh "$BASE" "$USER" "$LINTER_PORT" "$LINTER_PREFIX" "$ENCODING" "$PASSWD" "$VERSION" "$LOCAL_ADDRESS" "$IO_FILES" "$USE_MODULES" 2>$PREV/updatedb.log &&
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
