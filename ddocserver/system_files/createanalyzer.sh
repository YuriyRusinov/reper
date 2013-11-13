#!/bin/bash

DB=$1
USER=$2
PORT=$3
LINTER_PREFIX=$4
PGDATA_DIR=$5

A_DIR=./${DB}_analyzer

CWD=`pwd -P`


cd $PGDATA_DIR
DIR_EXIST=0

cd $A_DIR &&
DIR_EXIST=1

if [ $DIR_EXIST = 1 ]; then
    cd ../
    rm -f -r $A_DIR
    #cd $CWD
    #printf "ERROR! Internal $PGDATA_DIR/$A_DIR directory exist!\nMaybe KKSSITO Server was already installed on this host?\nInstallation aborted!\n\n"
    #exit 1
fi

DIR_EXIST=0
mkdir ./$A_DIR &&
chown postgres:postgres ./$A_DIR &&
chmod 700 $A_DIR && 
DIR_EXIST=1

if [ $DIR_EXIST = 0 ]; then
    cd $CWD
    printf "ERROR! Cannot create internal $PGDATA_DIR/$A_DIR directory!\n\n"
    exit 1
fi

DIR_EXIST=0

cd $A_DIR &&
touch ./analyzer.sh &&
chown postgres:postgres ./analyzer.sh &&
chmod 700 ./analyzer.sh &&
DIR_EXIST=1

if [ $DIR_EXIST = 0 ]; then
    cd $CWD
    printf "ERROR! Cannot create analyzer file!\n\n"
    exit 1
fi

CWD1=`pwd -P`

DIR_EXIST=0
cp ~/.pgpass $PGDATA_DIR/$A_DIR &&
chown postgres:postgres $PGDATA_DIR/$A_DIR/.pgpass &&
chmod 0600 $PGDATA_DIR/$A_DIR/.pgpass &&
DIR_EXIST=1

if [ $DIR_EXIST = 0 ]; then
    cd $CWD
    printf "ERROR! Cannot create .pgpass file!\n\n"
    exit 1
fi


DIR_EXIST=0
cd ~postgres &&
HOME_DIR=`pwd -P` &&
DIR_EXIST=1
if [ $DIR_EXIST = 0 ]; then
    cd $CWD1
    printf "ERROR! system user postgres does not have home directory! You MUST create that\n\n"
    exit 1
fi
cd $CWD1


printf "#!/bin/bash\n\n" >> ./analyzer.sh
printf "LINTER_PREFIX=$LINTER_PREFIX\n" >> ./analyzer.sh
printf "USER=$USER\n" >> ./analyzer.sh
printf "DB=$DB\n" >> ./analyzer.sh
printf "PORT=$PORT\n" >> ./analyzer.sh
printf "A_DIR=$PGDATA_DIR/$A_DIR\n" >> ./analyzer.sh
printf "LOG_FILE=\$A_DIR/analyzer.log\n\n" >> ./analyzer.sh

printf "CWD=\`pwd -P\`\n" >> ./analyzer.sh
printf "cd \$A_DIR\n" >> ./analyzer.sh
printf "cp \$A_DIR/.pgpass $HOME_DIR \n\n" >> ./analyzer.sh

printf "chmod 0600 $HOME_DIR/.pgpass\n" >> ./analyzer.sh
printf "export PGPASSFILE=$HOME_DIR/.pgpass\n" >> ./analyzer.sh

printf "\$LINTER_PREFIX/bin/psql -p \$PORT -d \$DB -c \"set session authorization admin; select createtemptables(); select setCurrentDl(4); select cmdanalyzejournal();\" -t \$USER  2>\$LOG_FILE\n\n" >> ./analyzer.sh
printf "\$LINTER_PREFIX/bin/psql -p \$PORT -d \$DB -c \"set session authorization admin; select createtemptables(); select setCurrentDl(4); select putIOIntoRubric();\" -t \$USER  2>\$LOG_FILE\n\n" >> ./analyzer.sh

printf "rm -f $HOME_DIR/.pgpass\n" >> ./analyzer.sh

printf "cd \$CWD\n" >> ./analyzer.sh

#once per minute
#printf "* * * * * root $PGDATA_DIR/$A_DIR/analyzer.sh\n" >> /etc/crontab
#/etc/rc.d/init.d/crond restart

cd $CWD
