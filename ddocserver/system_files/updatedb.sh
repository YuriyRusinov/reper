#!/bin/bash

BASE=$1
USER=$2
DB_PORT=$3
LINTER_PREFIX=$4
ENCODING=$5
DB_HOST=127.0.0.1
PASSWD=$6
VERSION=$7
LOCAL_ADDRESS=$8
#REMOTE_ADDRESS=$9
IO_FILES=${9}

CWD=`pwd -P`

cd ../internals

cp -f ./functions/files/readd_files_c_lin.sql ./functions/files/readd_files_c.sql

cd ./updates

#cd $CWD

COMMAND_OK=0
$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select kkssitoversion()" -t $USER > /dev/null &&
COMMAND_OK=1
if [ $COMMAND_OK = 0 ]; then
    printf "\n\nERROR! Database $BASE does not exist!\n\n"
    exit 1
fi

PREV_VER=`$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select kkssitoversion()" -t $USER | head -1 | cut -d ' ' -f 5`

UPDATE_FILE=${PREV_VER}_to_${VERSION}.sql

if [ ! -f ./$UPDATE_FILE ]; then
    printf "\n\nFATAL ERROR! Can't find $UPDATE_FILE\n\n";
    exit 1
fi

COMMAND_OK=0

cd ../

$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -f ./updates/$UPDATE_FILE $USER &&
COMMAND_OK=1


$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select setkkssitoversion('DynamicDocs Server version $VERSION', '$VERSION')" -t $USER > /dev/null &&
#$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select setfilearchivepath('$IO_FILES')" -t $USER > /dev/null &&
#$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select usetlocaladdress('$LOCAL_ADDRESS')" -t $USER > /dev/null &&
#$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select startsync()" -t $USER > /dev/null &&

cd ../
