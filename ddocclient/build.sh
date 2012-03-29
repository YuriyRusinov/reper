#!/bin/bash
QTDIR=/usr/lib/qt4

PROJECT_NAME=DynamicDocs

QTINC=$QTDIR/include
QTLIB=$QTDIR/lib

QTBINDIR=$QTDIR/bin

cd ./transl
QM_OK=0
$QTBINDIR/lupdate ../ -ts ./ddocclient_ru.ts &&
$QTBINDIR/lrelease ./ddocclient_ru.ts -qm ./ddocclient_ru.qm &&
QM_OK=1

if [ $QM_OK != 1 ]; then
    printf "\nWARNING! Russian translation for $PROJECT_NAME Client build failed!\n"
fi

cd ../


rm -f ./kkscore/kksclient_name.h
rm -f ./kksutils/configEndians.h

BUILD_OK=0

$QTBINDIR/qmake -recursive &&
make > ./errors.log &&
sudo make install &&
BUILD_OK=1

if [ $BUILD_OK = 0 ]; then
    printf "\n======================================="
    printf "\n\nDynamicDocs Client build failed!\n\n"
    printf "=======================================\n"
    exit 1
fi


#####

. ./install_prefix.conf

CWD=`pwd -P`

OK=0

cd $KKS_PREFIX &&
OK=1
if [ $OK = 0 ]; then
    printf "\n\nERROR!Cannot find installation directory!\n\n"
    exit 1
fi


mkdir -p ./sbin
#chown $USER:$GROUP $PREFIX/sbin

printf "LD_LIBRARY_PATH=../lib:$QTDIR/lib;export LD_LIBRARY_PATH" > ./ddocclient.ld.so.conf


cd ./bin
for i in * ; 
do 
    if [ $i != ddocclient_ru.qm ] && [ $i != data ]; then
        mv -f $i ../sbin
        printf "#!/bin/bash\n\n" > $i
        printf ". ../ddocclient.ld.so.conf\n\n" >> $i
        printf "../sbin/$i \"$" >> $i
        printf "@\"\n" >> $i
        chmod a+x $i
    fi
done

cd $CWD

printf "\n\n******************************************************"
printf "\n\n$PROJECT_NAME Client and all external libraries installed OK!"
printf "\n\n******************************************************\n"

exit 0
