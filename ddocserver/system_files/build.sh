#!/bin/bash

VERSION=$1
LINTER_PREFIX=$2
PGDATA_DIR=$3
IO_FILES=$4
UPDATE_MODE=$5
AUTO_CREATE=$6
PROJECT_NAME=$7

INSTALL_PREFIX=`$LINTER_PREFIX/bin/pg_config --pkglibdir`

printf "$INSTALL_PREFIX \n"

EXIT_ON_ERROR=1
LIBFLOADER_PATH=../internals/libfloader

CWD=`pwd -P`


makeAutoDir()
{
    DIR_EXIST=0
    OK=0

    while [ $OK = 0 ]
    do
        dirPostfix=$RANDOM
        printf "\n\nRANDOM NUMBER IS $dirPostfix\n\n";
        IO_FILES_DIR="./io_files_$dirPostfix"
        

        ABS_FILES_DIR=$IO_FILES_DIR

        cd $IO_FILES_DIR &&
        DIR_EXIST=1
        
        if [ $DIR_EXIST = 1  ]; then
            ABS_FILES_DIR=`pwd -P`
            cd ../
            continue
        fi

        DIR_EXIST=0
        mkdir $IO_FILES_DIR &&
        chown postgres:postgres $IO_FILES_DIR &&
        chmod 700 $IO_FILES_DIR &&
        cd $IO_FILES_DIR &&
        ABS_FILES_DIR=`pwd -P` &&
        cd ../
        DIR_EXIST=1

        if [ $DIR_EXIST = 0 ]; then
            printf "ERROR! Cannot create internal IO_FILES directory!\nFull creation path is $PGDATA_DIR/$IO_FILES_DIR\n"
            exit 1
        fi
        
        printf "IO_FILES_DIR=$ABS_FILES_DIR" > $CWD/io_files_name.conf
        OK=1
    done
}




LIBFLOADER_BUILD=0
cd $CWD

printf "********************************************************************************\n"
printf "********************************************************************************\n"
printf "********************************************************************************\n"
printf "********************************************************************************\n"
printf "                Start building $PROJECT_NAME Server file library\n"
printf "********************************************************************************\n"
printf "********************************************************************************\n"
printf "********************************************************************************\n"
printf "********************************************************************************\n"

cd $LIBFLOADER_PATH

cd ./src
rm -f ./kksversion.h
printf "#define KKS_VERSION \"$VERSION\"" >> ./kksversion.h

cd ../

SYSTEM_NAME=`uname -s`

WITH_PG="--with-pgsql=$LINTER_PREFIX/bin/pg_config"
LIBFLOADER_OPTIONS="$WITH_PG --libdir=$INSTALL_PREFIX"
chmod a+x ./configure
chmod a+x ./build-aux/autogen.sh
./build-aux/autogen.sh &&
./configure $LIBFLOADER_OPTIONS &&
make -f Makefile &&
LIBFLOADER_BUILD=1


if [ $LIBFLOADER_BUILD = 1 ]; then
    LIBFLOADER_BUILD=0
    
    if [ "$SYSTEM_NAME" = "SunOS" ]; then
        sudo cp ./bld/libfloader.so $INSTALL_PREFIX &&
        LIBFLOADER_BUILD=1
    else
        sudo make -f Makefile install &&
        LIBFLOADER_BUILD=1
    fi
fi
#make install DESTDIR=$TMP_PKG &&

if [ $LIBFLOADER_BUILD = 0 ] && [ $EXIT_ON_ERROR = 1 ]; then
    printf "$PROJECT_NAME Server file library build failed!\n"
    exit 1
fi

cd $PGDATA_DIR

DIR_EXIST=0

cd ./$IO_FILES &&
DIR_EXIST=1

if [ $DIR_EXIST = 1 ]; then
    cd ../
    if [ $UPDATE_MODE = 0 ]; then
        if [ $AUTO_CREATE != 1 ]; then
            printf "\n\nERROR! Internal IO_FILES directory exist! ($PGDATA_DIR/$IO_FILES)\nMaybe $PROJECT_NAME Server was already installed on this host?\n"
            printf "Try to set AUTO_CREATE parameter to 1 in user.config file\n\n"
            exit 1
        fi

        makeAutoDir
    else
        printf "IO_FILES_DIR=$IO_FILES" >> $CWD/io_files_name.conf
    fi
else
    
    DIR_EXIST=0
    mkdir ./$IO_FILES &&
    chown postgres:postgres ./$IO_FILES &&
    chmod 700 ./$IO_FILES &&
    DIR_EXIST=1
    if [ $DIR_EXIST = 0 ]; then
        printf "ERROR! Cannot create internal IO_FILES directory!\nFull creation path is $PGDATA_DIR/$IO_FILES\n"
        exit 1
    fi
    
    printf "IO_FILES_DIR=$IO_FILES" >> $CWD/io_files_name.conf
fi


printf "********************************************************************************\n"
printf "********************************************************************************\n"
printf "********************************************************************************\n"
printf "********************************************************************************\n"
printf "                            Build process complete!\n"
printf "********************************************************************************\n"
printf "********************************************************************************\n"
printf "********************************************************************************\n"
printf "********************************************************************************\n"

cd $CWD

exit 0
