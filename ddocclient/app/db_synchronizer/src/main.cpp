/* qt */
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <Qt>
#include <QObject>
#include <QMainWindow>

/* ui */
#include "kksclient_name.h"
#include "db_synchronizer.h"
#include "kkssito.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef WIN32
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
#else
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("KOI8-R"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("KOI8-R"));
#endif

    KKSSito *sito = KKSSito::init (false);
    if(!sito)
        return 1;

    DBSynchronizer dbSync; // = NULL;
	
    dbSync.show();

    int r = app.exec();//qApp->exec();

    //delete dbSync;
    return r;
}
