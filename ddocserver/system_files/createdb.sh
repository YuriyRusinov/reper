#!/bin/bash

BASE=$1
USER=$2
DB_PORT=$3
LINTER_PREFIX=$4
ENCODING=$5
WITH_DROP=$6
DB_HOST=127.0.0.1
PASSWD=$7
VERSION=$8
LOCAL_ADDRESS=$9
#REMOTE_ADDRESS=${10}
IO_FILES=${10}
USE_MODULES=${11}
IS_MAIN_ORG=${12}
ORG_NAME=${13}
LOCAL_PORT=${14}
USE_GATEWAY=${15}
ORG_EMAIL_PREFIX=${16}

. ./io_files_name.conf

if [ "$WITH_DROP" != "" ]; then
    OLD_DB=`$LINTER_PREFIX/bin/psql -U $USER -p $DB_PORT -h $DB_HOST -l | grep $BASE | head -1 | cut -d ' ' -f 2`
    if [ "$OLD_DB" = "$BASE" ]; then
        printf "\n\nDATABASE $BASE exist in current cluster! That will be dropped if you continue. Proceed? (y or n) [n] :";

	read key
	
 	if [ "$key" = "y" -o "$key" = "Y" ]; then
	    $LINTER_PREFIX/bin/dropdb -p $DB_PORT -h $DB_HOST -U $USER $BASE
        else
	    printf "Installation process cancelled!\n "
	    exit 1
	fi;
    fi
fi

CWD=`pwd -P`

cd ../internals

cp -f ./functions/files/readd_files_c_lin.sql ./functions/files/readd_files_c.sql

#cd $CWD

CREATED=0
THE_LOG=0

$LINTER_PREFIX/bin/createdb -h $DB_HOST -p $DB_PORT -E $ENCODING -U $USER $BASE  &&
CREATED=1

if [ $CREATED = 0 ]; then
    printf "\n\nERROR! Cannot create database $BASE\n"
    printf "Maybe no left space on device or database template1 is accessed by another user\n"
    printf "logout from database template1 and verify that that there are enough space available (1 Mb minimum).\n"
    printf "For details see log file ./createdb.log\n\n"
    
    exit 1
fi

$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -f ./initdb.sql $USER

$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select setkkssitoversion('DynamicDocs Server version $VERSION', '$VERSION')" -t $USER > /dev/null &&
$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select setfilearchivepath('$IO_FILES_DIR')" -t $USER > /dev/null &&

$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select startsync()" -t $USER > /dev/null &&
$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select setorgasmain($IS_MAIN_ORG)" -t $USER > /dev/null


#if [ "$IS_MAIN_ORG" = "1" ]; then
#    $LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "update organization set name = '$ORG_NAME', short_name = '$ORG_NAME', code_name = '$ORG_NAME'" -t $USER > /dev/null &&
#    $LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "insert into ORGANIZATION_TRANSPORT (id_organization, id_transport, address, is_active) values(1, 1, '$LOCAL_ADDRESS', TRUE);" -t $USER > /dev/null
#fi


$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -f ./initprivs.sql $USER

$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -f ./initdata.sql $USER

$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -f ./initiotriggers.sql $USER

$LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select usetlocaladdress('$LOCAL_ADDRESS', $LOCAL_PORT, $USE_GATEWAY)" -t $USER > /dev/null &&

if [ "$USE_MODULES" != "0" ]; then
    $LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -f ./initmodules.sql $USER
fi

# !!! localorg_prefix_slave is used in trigger on table organization_transport !!!
if [ "$IS_MAIN_ORG" = "1" ]; then
    $LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select createTempTables(); select setCurrentDl(4); select insertlocalorg('$ORG_NAME', '$ORG_NAME', '$ORG_NAME', '$ORG_EMAIL_PREFIX', 1, NULL, 1, '$LOCAL_ADDRESS', $LOCAL_PORT, $USE_GATEWAY)" -t $USER > /dev/null
    $LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "create unique index ot_addr_port on organization_transport (address, port);" -t $USER > /dev/null
fi

if [ "$IS_MAIN_ORG" != "1" ]; then
    $LINTER_PREFIX/bin/psql -h $DB_HOST -p $DB_PORT -d $BASE -c "select createTempTables(); select setCurrentDl(4); select insertlocalorg('$ORG_NAME', '$ORG_NAME', '$ORG_NAME', 'localorg_prefix_slave', 1, NULL, 1, '$LOCAL_ADDRESS', $LOCAL_PORT, $USE_GATEWAY)" -t $USER > /dev/null
fi

exit 0
