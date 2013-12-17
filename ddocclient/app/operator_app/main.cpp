/* qt */
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <Qt>
#include <QObject>
#include <QMainWindow>

/* ui */
#include "kksmainwindow.h"
#include "kkssito.h"
#include "kksclient_name.h"

int main(int argc, char *argv[])
{
    //QApplication app(argc, argv);

    KKSSito *sito = NULL;
	
    KKSMainWindow * mainWindow = NULL;

    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("cp1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp1251"));

    if(argc == 2 && strcmp(argv[1], "admin") == 0){
        //sito = KKSSito::init (false);//, QString("admin"));
        sito = KKSSito::init (argc, argv, false, QString("admin"), true);
        mainWindow = new KKSMainWindow();
		mainWindow->setWindowTitle(QObject::tr("DynamicDocs ") + QObject::tr("Administrator") + " " + KKS_VERSION);
        //mainWindow->setWindowTitle(QObject::tr("PK IR ") + QObject::tr("Administrator") + " " + KKS_VERSION);
    }
    else{
        sito = KKSSito::init (argc, argv, false, QString(), true);
        mainWindow = new KKSMainWindow();
		mainWindow->setWindowTitle(QObject::tr("DynamicDocs ") + QObject::tr("Operator") + " " + KKS_VERSION);
        //mainWindow->setWindowTitle(QObject::tr("PK IR ") + QObject::tr("Operator") + " " + KKS_VERSION);
    }

    if(!sito){
        delete mainWindow;
        return 1;
    }

    mainWindow->show();

    //int r = app.exec();
    int r = qApp->exec();

    delete mainWindow;
    return r;
}
