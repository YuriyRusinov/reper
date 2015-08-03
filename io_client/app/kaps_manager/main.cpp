/* qt */
#include <QApplication>
#include <QTextStream>
#include <Qt>
#include <QObject>
#include <QMainWindow>

/* ui */
#include "kksclient_name.h"
#include "kaps_manager_form.h"
#include "kkssito.h"
#include "kksdatabase.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef WIN32
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
#else
    QTextCodec::setCodecForTr(QTextCodec::codecForName("KOI8-R"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("KOI8-R"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("KOI8-R"));
#endif

    KKSSito *sito = KKSSito::init (false);
    if(!sito)
        return 1;

    int ok = sito->GUIConnect();
    if(ok != 1)
        return 1;

    KKSDatabase * db = sito->db();
    if(!db || !db->connected())
        return 1;

    KapsManagerForm f (db); // = NULL;
	
    f.show();

    int r = app.exec();//qApp->exec();

    //delete dbSync;
    return r;
}
