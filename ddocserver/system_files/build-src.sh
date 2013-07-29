#!/bin/bash

VERSION=$1
PREFIX=$2
PGDATA=$3
PORT=$4
SRC_DIR=$5
DB_NAME=$6
USER=$7
PASSWORD=$8


#SRC_DIR=/home/ddoc/src


PROJ_VER=4.8.0
GEOS_VER=3.3.8
PGSQL_VER=9.2.3
POSTGIS_VER=2.0.3
GDAL_VER=1.9.2
UUID_VER=1.6.2
GSL_VER=1.14

PG_SRC_DIR=$SRC_DIR/postgresql-$PGSQL_VER
UUID_SRC_DIR=$SRC_DIR/uuid-$UUID_VER
GSL_SRC_DIR=$SRC_DIR/gsl-$GSL_VER
PROJ4_SRC_DIR=$SRC_DIR/proj-$PROJ_VER
GEOS_SRC_DIR=$SRC_DIR/geos-$GEOS_VER
GDAL_SRC_DIR=$SRC_DIR/gdal-$GDAL_VER
POSTGIS_SRC_DIR=$SRC_DIR/postgis-$POSTGIS_VER

SYS_FILES_ABS=`pwd -P`

cd $SRC_DIR
PREV=`pwd -P`

if [ ! -f ./proj-$PROJ_VER.tar.gz ]; then 
    printf "Can't find ./proj-$PROJ_VER.tar.gz in $SRC_DIR\n";
    exit 1
fi

if [ ! -f ./geos-$GEOS_VER.tar.bz2 ]; then 
    printf "Can't find ./geos-$GEOS_VER.tar.gz in $SRC_DIR\n";
    exit 1
fi

if [ ! -f ./postgresql-$PGSQL_VER.tar.bz2 ]; then 
    printf "Can't find ./postgresql-$PGSQL_VER.tar.bz2 in $SRC_DIR\n";
    exit 1
fi

if [ ! -f ./postgis-$POSTGIS_VER.tar.gz ]; then 
    printf "Can't find ./postgis-$POSTGIS_VER.tar.gz in $SRC_DIR\n";
    exit 1
fi

if [ ! -f ./gdal-$GDAL_VER.tar.gz ]; then 
    printf "Can't find ./gdal-$GDAL_VER.tar.gz in $SRC_DIR\n";
    exit 1
fi

if [ ! -f ./uuid-$UUID_VER.tar.gz ]; then 
    printf "Can't find ./uuid-$UUID_VER.tar.gz in $SRC_DIR\n";
    exit 1
fi

if [ ! -f ./gsl-$GSL_VER.tar.gz ]; then 
    printf "Can't find ./gsl-$GSL_VER.tar.gz in $SRC_DIR\n";
    exit 1
fi


#PREFIX=$LINTER_PREFIX

PG_PREFIX=$PREFIX
UUID_PREFIX=$PREFIX
PROJ4_PREFIX=$PREFIX
GEOS_PREFIX=$PREFIX
GDAL_PREFIX=$PREFIX
POSTGIS_PREFIX=$PREFIX

GSL_PREFIX=/usr/

adduser postgres



export LD_LIBRARY_PATH=$UUID_PREFIX/lib:$PREFIX/lib:$GSL_PREFIX

gunzip < uuid-$UUID_VER.tar.gz | tar xf - &&
cd $UUID_SRC_DIR && 
./configure --prefix=$UUID_PREFIX --with-pgsql=no --with-cxx && 
make && 
make install && 

cd $PREV &&
bunzip2 < postgresql-$PGSQL_VER.tar.bz2 | tar xf - &&
cd $PG_SRC_DIR && 
./configure LDFLAGS="-L$UUID_PREFIX/lib" CPPFLAGS="-I$UUID_PREFIX/include" --with-ossp-uuid --prefix=$PG_PREFIX --with-libxml --with-libxslt && 
make world && 
make install && 
cd ./contrib && 
make && 
make install &&


mkdir $PGDATA && 
chown -R postgres:postgres  $PGDATA && 
su - postgres -c "$PG_PREFIX/bin/initdb -D $PGDATA -E UTF8 --locale=ru_RU.UTF8" &&

cd $PREV &&
gunzip < gsl-$GSL_VER.tar.gz | tar xf - &&
cd $GSL_SRC_DIR &&
./configure --prefix=$GSL_PREFIX &&
make &&
make install &&

cd $PREV &&
gunzip < proj-$PROJ_VER.tar.gz | tar xf - &&
cd $PROJ4_SRC_DIR &&
./configure --prefix=$PROJ4_PREFIX &&
make &&
make install &&

cd $PREV &&
bunzip2 < geos-$GEOS_VER.tar.bz2 | tar xf - &&
cd $GEOS_SRC_DIR &&
./configure --prefix=$GEOS_PREFIX &&
make &&
make install &&

cd $PREV &&
gunzip < gdal-$GDAL_VER.tar.gz | tar xf - &&
cd $GDAL_SRC_DIR &&
./configure LDFLAGS="-L$PREFIX/lib" CPPFLAGS="-I$PREFIX/include" --prefix=$GDAL_PREFIX --with-geos=$GEOS_PREFIX/bin/geos-config &&
make &&
make install &&

cd $PREV &&
gunzip < postgis-$POSTGIS_VER.tar.gz | tar xf - &&
cd $POSTGIS_SRC_DIR &&
./configure --prefix=$POSTGIS_PREFIX --with-projdir=$PROJ4_PREFIX --with-geosconfig=$GEOS_PREFIX/bin/geos-config --with-pgconfig=$PG_PREFIX/bin/pg_config --with-gdalconfig=$GDAL_PREFIX/bin/gdal-config &&
make &&
make install &&

cd $SYS_FILES_ABS &&
CONF_FILES="$SYS_FILES_ABS/pg_hba.conf $SYS_FILES_ABS/postgresql.conf"
sudo cp -f $CONF_FILES $PGDATA

printf "#!/bin/sh\n" > ./ddocserver-$VERSION &&
printf "# ddocserver-$VERSION This is the init script for starting up the PostgreSQL\n" >> ./ddocserver-$VERSION &&
printf "#               server\n" >> ./ddocserver-$VERSION &&
printf "#\n" >> ./ddocserver-$VERSION &&
printf "# chkconfig: - 64 36\n" >> ./ddocserver-$VERSION &&
printf "# description: Starts and stops the PostgreSQL backend daemon that handles \\\n" >> ./ddocserver-$VERSION &&
printf "#              all database requests.\n" >> ./ddocserver-$VERSION &&
printf "# processname: postmaster\n" >> ./ddocserver-$VERSION &&
printf "# pidfile: /var/run/postmaster.pid\n" >> ./ddocserver-$VERSION &&

printf "\nPGVERSION=$PGSQL_VER\nPG_PREFIX=$PG_PREFIX\nPGPORT=$PORT\n\n" >> ./ddocserver-$VERSION &&
cat ./postgresql.orig >> ./ddocserver-$VERSION &&
chmod a+x ./ddocserver-$VERSION &&
cp ./ddocserver-$VERSION /etc/init.d/ 

OS_NAME=`uname -a | grep -o gentoo`
SERVICE_ADDED=0
if [ "$OS_NAME" = "gentoo" ]; then
    rc_update add ddocserver-$VERSION default &&
    SERVICE_ADDED=1
else
    chkconfig --add ddocserver-$VERSION &&
    SERVICE_ADDED=1
fi

SERVICE_STARTED=0
if [ $SERVICE_ADDED = 1 ]; then
    service ddocserver-$VERSION start &&
    SERVICE_STARTED=1
else
    ./pg_start.sh start $PGSQL_VER $PG_PREFIX $PORT &&
    SERVICE_STARTED=1
fi

if [ $SERVICE_STARTED = 0 ]; then
    printf "\n\nERROR! Could not start DynamicDocs-$VERSION Server!\n\n"
    exit 1
fi

sleep 5 &&

$PG_PREFIX/bin/psql -U postgres -p $PORT -c "create role $USER with superuser createdb createrole login password '$PASSWORD'" -d template1

$PG_PREFIX/bin/psql -U $USER -p $PORT -f $POSTGIS_SRC_DIR/postgis/postgis.sql -d $DB_NAME &&
$PG_PREFIX/bin/psql -U $USER -p $PORT -f $POSTGIS_SRC_DIR/raster/rt_pg/rtpostgis.sql -d $DB_NAME &&
$PG_PREFIX/bin/psql -U $USER -p $PORT -f $POSTGIS_SRC_DIR/spatial_ref_sys.sql -d $DB_NAME &&

#./pg_start.sh stop $PGSQL_VER $PG_PREFIX $PORT &&

cd $SYS_FILES_ABS
