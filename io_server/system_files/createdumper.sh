#!/bin/bash

DB=$1
USER=$2
PORT=$3
LINTER_PREFIX=$4
PGDATA_DIR=$5


DUMPS_DIR=./${DB}_dumps

CWD=`pwd -P`


cd $PGDATA_DIR
DIR_EXIST=0

cd $DUMPS_DIR &&
DIR_EXIST=1

if [ $DIR_EXIST = 1 ]; then
    cd ../
    rm -f -r $DUMPS_DIR
fi

DIR_EXIST=0
mkdir ./$DUMPS_DIR &&
chown postgres:postgres ./$DUMPS_DIR &&
chmod 700 $DUMPS_DIR && 
DIR_EXIST=1

if [ $DIR_EXIST = 0 ]; then
    cd $CWD
    printf "ERROR! Cannot create internal $PGDATA_DIR/$DUMPS_DIR directory!\n\n"
    exit 1
fi


#suggest that ${DB}_analyzer directory is exist
DUMPER_DIR=./${DB}_analyzer

cd $CWD

cd $PGDATA_DIR
DIR_EXIST=0

cd $DUMPER_DIR &&
DIR_EXIST=1

if [ $DIR_EXIST = 0 ]; then
    cd $CWD
    printf "ERROR! Internal $PGDATA_DIR/$DUMPER_DIR directory does not exist!\nInstallation aborted!\n\n"
    exit 1
fi


DIR_EXIST=0
touch ./dumper.sh &&
chown postgres:postgres ./dumper.sh &&
chmod 700 ./dumper.sh &&
DIR_EXIST=1

if [ $DIR_EXIST = 0 ]; then
    cd $CWD
    printf "ERROR! Cannot create dumper.sh file!\n\n"
    exit 1
fi

CWD1=`pwd -P`

DIR_EXIST=0
cd ~postgres &&
HOME_DIR=`pwd -P` &&
DIR_EXIST=1
if [ $DIR_EXIST = 0 ]; then
    cd $CWD1
    printf "ERROR! system user $USER does not have home directory! You MUST create that\n\n"
    exit 1
fi

cd $CWD1


printf "#!/bin/bash\n\n" >> ./dumper.sh
printf "LINTER_PREFIX=$LINTER_PREFIX\n" >> ./dumper.sh
printf "USER=$USER\n" >> ./dumper.sh
printf "DB=$DB\n" >> ./dumper.sh
printf "PORT=$PORT\n\n" >> ./dumper.sh


printf "DUMP_DIR=$PGDATA_DIR/$DUMPS_DIR\n" >> ./dumper.sh
printf "DUMPER_DIR=$PGDATA_DIR/$DUMPER_DIR\n" >> ./dumper.sh
printf "LOG_FILE=\$DUMPER_DIR/lastdump.log\n\n" >> ./dumper.sh

printf "CWD=\`pwd -P\`\n" >> ./dumper.sh
printf "cd \$DUMPER_DIR\n" >> ./dumper.sh
printf "cp \$DUMPER_DIR/.pgpass $HOME_DIR/.pgpass\n" >> ./dumper.sh

printf "chmod 0600 $HOME_DIR/.pgpass\n" >> ./dumper.sh
printf "export PGPASSFILE=$HOME_DIR/.pgpass\n\n" >> ./dumper.sh

STR="date +%%x"
printf "DUMP_FILE=\`$STR\`\n\n" >> ./dumper.sh

printf "\$LINTER_PREFIX/bin/pg_dump -p \$PORT -f \$DUMP_DIR/\$DUMP_FILE -U \$USER \$DB 2>\$LOG_FILE\n\n" >> ./dumper.sh


printf "rm -f $HOME_DIR/.pgpass\n" >> ./dumper.sh

printf "cd \$CWD\n" >> ./dumper.sh

#printf "0 18 * * * root $PGDATA_DIR/$DUMPER_DIR/dumper.sh\n" >> /etc/crontab
#/etc/rc.d/init.d/crond restart

cd $CWD
