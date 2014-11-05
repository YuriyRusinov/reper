#include "kkscoreapplication.h"

#include <QtGlobal>
#include <QtDebug>
#include <QLocale>
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QMap>
#include <QTextEdit>
#include <QTextStream>
#include <QString>


#include "defines.h"
#include <kkspgdatabase.h>
#include <kksdatabase.h>

#include "kkspluginloader.h"

#include "KKSLoader.h"
#include "KKSFileLoader.h"
#include "KKSPPFactory.h"
#include "KKSEIOFactory.h"

#include "KKSDbgOutputWidget.h"
#include <kksnotifyreceiver.h>

#include "kkssettings.h"
#include "kkscommandlineopts.h"
#include "commandlineparamsform.h"

#include "connectioninfoform.h"

#include "loginform.h"
#include "choosedlform.h"

#include "kksclient_name.h"

__COREAPP_EXPORT KKSCoreApplication * KKSCoreApplication::selfCore = 0;

const QString kksAppNameEng = QString(EXECUTABLE_PREFIX);
QFile m_logFile;

void KKSDbgOutputHandler(QtMsgType type, const char *msg)
{
     if(!kksCoreApp || !kksCoreApp->dbgWidget())
         return;

     switch (type) {
     case QtDebugMsg:
         kksCoreApp->dbgWidget()->printMessage(KKSDbgOutputWidget::cDebugMsg, QString::fromLocal8Bit(msg) );
         fprintf(stdout, "Debug: %s\n", msg);
         break;
     case QtWarningMsg:
         kksCoreApp->dbgWidget()->printMessage(KKSDbgOutputWidget::cWarningMsg, QString::fromLocal8Bit(msg) );
         fprintf(stdout, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         kksCoreApp->dbgWidget()->printMessage(KKSDbgOutputWidget::cCriticalMsg, QString::fromLocal8Bit(msg) );
         fprintf(stdout, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         kksCoreApp->dbgWidget()->printMessage(KKSDbgOutputWidget::cFatalMsg, QString::fromLocal8Bit(msg) );
         fprintf(stdout, "Fatal: %s\n", msg);
         abort();
     }
}


/*!\brief ����������� ������.

���������� ��������, ��� � ���������� ����� ������� ������ ���� ��������� ������� ������. ��� ������� ������� ������, ���������� ���������� ��������.

��������������� ��������� ��������� � ��������� ������� ��� ������ �� ���������. � �������� ������ ������������ ������� ��������� (�������� ����� QTextCodec::codecForLocale()). 
� �������� ������ ������������ ��������� KOI8-R (�������� ����� QTextCodec::codecForName("KOI8-R")).

����� ���� ����������� ����������� ������ ��� ������ QTranslator (�����������). �� ��������� ������������ ���� ddocclient_ru.ts � �������� �������� ����������.
*/
KKSCoreApplication::KKSCoreApplication(KKSCommandLineOpts * opts, bool msgToWindow) :
    poDb (0),
    poDb1(0),
    poDb2(0),
    m_dbgWidget(0),
    m_notifyReceiver(0)
{
    if ( selfCore )
        qFatal("There should be only one KKSCoreApplication object");

    poSettings = 0;
    lastError = 0;
    tor = 0;

    m_pluginLoader = 0;

    Q_INIT_RESOURCE (ddocs_icon_set);
    Q_INIT_RESOURCE (ddocs_icon_set1);


    m_loader = 0;
    m_fileLoader = 0;
    m_ppf = NULL;
    m_eiof = NULL;

    selfCore = this;
    
    m_kksOpts = opts;
    if(m_kksOpts)
        setAllowedUserName(m_kksOpts->user);

    QDir dir;
    workingDir = dir.absolutePath();

    loadTranslator();
    loadLastOpenedDir();
    loadLastSelectedFilter();
    loadAccLevels();

    loadKKSPlugins();
    initCoreFactories();
    

    initLogStream();

    getLastError();//��� ���� ���� ���� ������� ������� � ������� ����� (��� ��� exe-���� �����)

    if(msgToWindow)
        qInstallMsgHandler(KKSDbgOutputHandler);

}


void KKSCoreApplication::loadKKSPlugins()
{
    
    KKSSettings *kksSettings = kksCoreApp->getKKSSettings();
    if(!kksSettings)
        return;
    
    QString plugins_path = getWDir() + "/ddocplugins";
    
    kksSettings->beginGroup (QString("System settings/") + kksAppNameEng);

    if ( kksSettings->getParam("DDoc_plugins_path").isEmpty() )
    {
        kksSettings->endGroup();
        kksSettings->writeSettings (QString("System settings/") + kksAppNameEng, 
                                     "DDoc_plugins_path", 
                                     plugins_path);
        kksSettings->beginGroup (QString("System settings/") + kksAppNameEng);
    }
    
    plugins_path = kksSettings->getParam("DDoc_plugins_path");

    kksSettings->endGroup();
    
    qDebug() << "Try to load DynamicDocs plugins from " << plugins_path;
    
    QDir d(plugins_path);

    m_pluginLoader = new KKSPluginLoader(d);
    m_pluginLoader->initPlugins();
}

void KKSCoreApplication::loadTranslator()
{
    KKSSettings *kksSettings = kksCoreApp->getKKSSettings();
    if(!kksSettings)
        return;
    
    QString transl_path = getWDir();
    
    kksSettings->beginGroup (QString("System settings/") + kksAppNameEng);

    if ( kksSettings->getParam("Translators_path").isEmpty() )
    {
        kksSettings->endGroup();
        kksSettings->writeSettings (QString("System settings/") + kksAppNameEng, 
                                     "Translators_path", 
                                     transl_path);
        kksSettings->beginGroup (QString("System settings/") + kksAppNameEng);
    }
    
    transl_path = kksSettings->getParam("Translators_path");

    kksSettings->endGroup();
    
    qDebug() << "Try to load translator files from " << transl_path;

    if(tor){
        QApplication::removeTranslator(tor);
        delete tor;
    }

    QTranslator * openRPTtor = new QTranslator(0);
    bool ok = openRPTtor->load(QString("common_ru"), transl_path);
    QApplication::installTranslator(openRPTtor);

    QTranslator * openRPTtor1 = new QTranslator(0);
    bool ok1 = openRPTtor1->load(QString("wrtembed_ru"), transl_path);
    QApplication::installTranslator(openRPTtor1);

    QTranslator * openRPTtor2 = new QTranslator(0);
    bool ok2 = openRPTtor2->load(QString("renderer_ru"), transl_path);
    QApplication::installTranslator(openRPTtor2);

    tor = new QTranslator(0);
    tor->load(QString("ddocclient_ru"), transl_path);
    QApplication::installTranslator(tor);

}


/*!\brief ���������� ������. */
KKSCoreApplication::~KKSCoreApplication( )
{

    if(m_notifyReceiver){
        m_notifyReceiver->quit();
        delete m_notifyReceiver;
    }

    if(poDb)
        delete poDb;

    if(poDb1)
        delete poDb1;

    if(poDb2)
        delete poDb2;

    if ( tor )
        delete tor;

    if(lastError)
        delete lastError;
    
    if(poSettings)
        delete poSettings;

    if(m_loader) 
        delete m_loader;

    if(m_fileLoader) 
        delete m_fileLoader;

    if(m_ppf) 
        delete m_ppf;

    if(m_eiof) 
        delete m_eiof;

    if(m_kksOpts)
        delete m_kksOpts;

    selfCore = 0;

    m_logStream.flush();
}


const KKSCommandLineOpts * KKSCoreApplication::commandLineOpts() const
{
    if(!m_kksOpts)
        m_kksOpts = new KKSCommandLineOpts();

    return m_kksOpts;
}

KKSDatabase * KKSCoreApplication::db() const 
{
    if ( ! poDb ){
        poDb = new KKSPGDatabase();
        QObject::connect(poDb, SIGNAL(disconnected()), this, SLOT(clearLoader()));
    }

    return poDb;
}

KKSDatabase * KKSCoreApplication::db1() const 
{
    if ( ! poDb1 ){
        poDb1 = new KKSPGDatabase();
    }

    return poDb1;
}

KKSDatabase * KKSCoreApplication::db2() const 
{
    if ( ! poDb2 ){
        poDb2 = new KKSPGDatabase();
    }

    return poDb2;
}

void KKSCoreApplication::clearLoader()
{
    if(!m_loader)
        return;

    m_loader->clearCache();
}

KKSDbgOutputWidget * KKSCoreApplication::dbgWidget(bool bCreateMenu, bool bForDockable) const
{
    if(!m_dbgWidget){
        m_dbgWidget = new KKSDbgOutputWidget(bForDockable);
        if(bCreateMenu)
            m_dbgWidget->initMenuEmitting();
    }

    return m_dbgWidget;
}


/*!\brief ����� ���������� ��������� ��������� ������*/
KKSError * KKSCoreApplication::getLastError()
{
    if ( ! lastError ){
        lastError = new KKSError();
        QString fileName = QString( getWDir() + "/kks_err.log" );
        lastError->setLogFile(fileName);
    }

    return lastError;
}

void KKSCoreApplication::initLogStream()
{
    QString fileName = QString( getWDir() + "/log.log" );
    m_logFile.setFileName(fileName);
    
    if (m_logFile.open(QFile::WriteOnly | QFile::Truncate)) {
        m_logStream.setDevice(&m_logFile);
        m_logStream << QObject::tr("Log started at ") + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "\n";
    }
}

/*!\brief ����� ��������� ���������� � ��������� ��������� ������. 

��������� ������� ������ ����� ����� �� �����, ��� � � ������ KKSError::init().*/
void KKSCoreApplication::updateLastError(KKSError::ErrorTypes type, QString code, QString name, QString message, QString group)
{
    lastError->init(type, 
                    code, 
                    name, 
                    message, 
                    group);
}

/*!\brief ����� ���������� ��������� �� ����� ������������ ���������.

����������� ������������ ������, ����� �������� ������ � ����������. � ���������� ����� ���� ������ ������ ���� ��������� ������� ������.

��������! ������������ ��������� �� ���������� ��������� ������. ������ ������� ��� �������������� ������ ������.*/
KKSSettings * KKSCoreApplication::getKKSSettings()
{
    if ( ! poSettings)
    {
        poSettings = new KKSSettings( ORGANIZATION , PRODUCT_FULL_NAME);
    }

    poSettings->readSettings ( );

    return poSettings;
}

/*!\brief ����� ���������� ��������� �� ����� ������������ ���������.

����������� ������������ ������, ����� �������� ������ � ����������. � ���������� ����� ���� ������ ������ ���� ��������� ������� ������.

��������! ������������ ��������� �� ���������� ��������� ������. ������ ������� ��� �������������� ������ ������.*/
KKSSettings * KKSCoreApplication::getKKSSettings (const QString & organization, const QString & application )
{
    if ( ! poSettings)
    {
        poSettings = new KKSSettings (organization, application);
    }


    poSettings->readSettings ( );
    return poSettings;
}


/*!\brief ����� ���������� ��������� �� ����� ������������ ��������� c �������� ����� ��� �� ����������.

����������� ������������ ������, ����� �������� ������ � ����������. � ���������� ����� ���� ������ ������ ���� ��������� ������� ������.

��������! ������������ ��������� �� ���������� ��������� ������. ������ ������� ��� �������������� ������ ������.*/
KKSSettings * KKSCoreApplication::getKKSSettings (const QString & fileName)
{
    if ( ! poSettings)
    {
        poSettings = new KKSSettings ( fileName );
    }

    poSettings->readSettings ( );
    return poSettings;
}

/*!\brief ����� �������� ����������� ������ ������ ������ ������������ ������ � ���������� �� �������� (������� ���������� ����) � ���� ������.

������������� ������������ ������ ������ �����, � �� ����������� ����� QFileDialog::getOpenFileNames(), ��������� ������ ����� ���������� ��������� �������� ������� � ��������� ������ ��� ������.

� ������, ���� �� ������ ����� �� ���� �������, ����� ������ ������ ������.

\param parent ��������� �� ������������ ����
\param caption �������������� ������� � ������� ����� ������� ������ �����
\param filters ������ ����� � ��������� ���������� ������. ������ ������� ����� ���� ������, ��������, ��������� �������: "X Pixmap (*.XPM *.xpm)".
\param startDir ��������� ������� ������� ������ �����. ���� �� ����� ��� ���������� ������ ������, �� ����� ������������ �������� getLastOpenedDir().
\param startFilter ������, ������� ������ ���� ������ � �������. ���� ���������� �������� �� ����� ������� � ������ �������� �������, �� ��� ����� ��������������
*/
QStringList KKSCoreApplication::getOpenFileNames(QWidget * parent, QString caption, QStringList filters, QString startDir, QString startFilter)
{
    QFileDialog * fileDialog = new QFileDialog(parent, caption);

    if(!filters.isEmpty())
        fileDialog->setFilters(filters);

    if(startFilter.isEmpty())
        fileDialog->selectFilter(lastSelectedFilter);
    else
        fileDialog->selectFilter(startFilter);

    fileDialog->setFileMode(QFileDialog::ExistingFile);

    if(startDir.isEmpty())
        fileDialog->setDirectory(getLastOpenedDir());
    else
        fileDialog->setDirectory(startDir);

    QStringList lFiles;
    fileDialog->setModal(true);
    if(fileDialog->exec() != QDialog::Accepted){
        delete fileDialog;
        return lFiles;
    }

    lastOpenedDir = fileDialog->directory().absolutePath();
    if(poSettings)
        poSettings->writeSettings (QString ("System settings/"+kksAppNameEng), "LastOpenedDir", lastOpenedDir);

    lastSelectedFilter = fileDialog->selectedFilter();
    if(poSettings)
        poSettings->writeSettings (QString ("System settings/"+kksAppNameEng), "LastSelectedFilter", lastSelectedFilter);

    lFiles = fileDialog->selectedFiles();

    delete fileDialog;

    return lFiles;
}

/*!\brief ����� �������� ����������� ������ ������ ������������� ����� � ���������� ��� �������� (������� ���������� ���� � ����).

������������� ������������ ������ ������ �����, � �� ����������� ����� QFileDialog::getOpenFileName(), ��������� ������ ����� ���������� ��������� �������� ������� � ��������� ������ ��� ������.

� ������, ���� �� ������ ����� �� ���� �������, ����� ������ ������ ������.

\param parent ��������� �� ������������ ����
\param caption �������������� ������� � ������� ����� ������� ������ �����
\param filters ������ ����� � ��������� ���������� ������. ������ ������� ����� ���� ������, ��������, ��������� �������: "X Pixmap (*.XPM *.xpm)".
\param startDir ��������� ������� ������� ������ �����. ���� �� ����� ��� ���������� ������ ������, �� ����� ������������ �������� getLastOpenedDir().
\param startFilter ������, ������� ������ ���� ������ � �������. ���� ���������� �������� �� ����� ������� � ������ �������� �������, �� ��� ����� ��������������
*/
QString KKSCoreApplication::getOpenFileName(QWidget * parent, QString caption, QStringList filters, QString startDir, QString startFilter)
{
    QFileDialog * fileDialog = new QFileDialog(parent, caption);

    if(!filters.isEmpty())
        fileDialog->setFilters(filters);

    if(startFilter.isEmpty())
        fileDialog->selectFilter(lastSelectedFilter);
    else
        fileDialog->selectFilter(startFilter);

    fileDialog->setFileMode(QFileDialog::ExistingFile);

    if(startDir.isEmpty())
        fileDialog->setDirectory(getLastOpenedDir());
    else
        fileDialog->setDirectory(startDir);

    QString lFile;
    fileDialog->setModal(true);
    if(fileDialog->exec() != QDialog::Accepted){
        delete fileDialog;
        return lFile;
    }

    lastOpenedDir = fileDialog->directory().absolutePath();
    if(poSettings)
        poSettings->writeSettings (QString ("System settings/"+kksAppNameEng), "LastOpenedDir", lastOpenedDir);

    lastSelectedFilter = fileDialog->selectedFilter();
    if(poSettings)
        poSettings->writeSettings (QString ("System settings/"+kksAppNameEng), "LastSelectedFilter", lastSelectedFilter);

    QStringList files = fileDialog->selectedFiles();
    if (!files.isEmpty())
        lFile = files[0];

    delete fileDialog;

    return lFile;
}

/*!\brief ����� �������� ����������� ������ ������ ����� ��� ���������� (�.�. ���� ����� �� ������������) � ���������� ��� �������� (������� ���������� ���� � ����).

������������� ������������ ������ ������ �����, � �� ����������� ����� QFileDialog::getSaveFileName(), ��������� ������ ����� ���������� ��������� �������� ������� � ��������� ������ ��� ������.

� ������, ���� �� ������ ����� �� ���� �������, ����� ������ ������ ������.

\param parent ��������� �� ������������ ����
\param caption �������������� ������� � ������� ����� ������� ������ �����
\param filters ������ ����� � ��������� ���������� ������. ������ ������� ����� ���� ������, ��������, ��������� �������: "X Pixmap (*.XPM *.xpm)".
\param selectedFilter ��������� �� ������ � ��������� � ������� ��������. ���� � �������� �������� ������� ��������� ����� ������ ��������� ������ �� ������, ���������� ����, �� ������ ������ � ������� ������ �������. �������� ����� ���� ����� ��������� �������: QString * filter = new QString(); ����� ������ ������� ������ �������� ����� ��������� ��������� � ������� ������.
\param startDir ��������� ������� ������� ������ �����. ���� �� ����� ��� ���������� ������ ������, �� ����� ������������ �������� getLastOpenedDir().
*/
QString KKSCoreApplication::getSaveFileName(QWidget * parent, QString caption, QStringList filters, QString * selectedFilter, QString startDir)
{
    QFileDialog * fileDialog = new QFileDialog(parent, caption);

    if(!filters.isEmpty())
        fileDialog->setFilters(filters);

    if(!selectedFilter || selectedFilter->isEmpty())
        fileDialog->selectFilter(lastSelectedFilter);
    else
        fileDialog->selectFilter(*selectedFilter);

    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);

    if(startDir.isEmpty())
        fileDialog->setDirectory(getLastOpenedDir());
    else
        fileDialog->setDirectory(startDir);

    QString lFile;
    fileDialog->setModal(true);
    if(fileDialog->exec() != QDialog::Accepted){
        delete fileDialog;
        return lFile;
    }

    lastOpenedDir = fileDialog->directory().absolutePath();
    if(poSettings)
        poSettings->writeSettings (QString ("System settings/"+kksAppNameEng), 
                                   "LastOpenedDir", 
                                   lastOpenedDir);

    lastSelectedFilter = fileDialog->selectedFilter();
    if(selectedFilter) 
        *selectedFilter = fileDialog->selectedFilter();
    if(poSettings)
        poSettings->writeSettings (QString ("System settings/"+kksAppNameEng), 
                                   "LastSelectedFilter", 
                                   lastSelectedFilter);

    QStringList files = fileDialog->selectedFiles();
    if (!files.isEmpty())
        lFile = files[0];

    delete fileDialog;

    return lFile;
}

/*!\brief ����� �������� ����������� ������ ������ ������������� �������� � ���������� ��� �������� (������� ���������� ���� � ����).

������������� ������������ ������ ������ �����, � �� ����������� ����� QFileDialog::getExistingDirectory(), ��������� ������ ����� ���������� ��������� �������� �������.

� ������, ���� �� ������ �������� �� ���� �������, ����� ������ ������ ������.

\param parent ��������� �� ������������ ����
\param caption �������������� ������� � ������� ����� ������� ������ ��������
\param startDir ��������� ������� �������. ���� �� ����� ��� ���������� ������ ������, �� ����� ������������ �������� getLastOpenedDir().
*/
QString KKSCoreApplication::getExistingDir(QWidget * parent, QString caption, QString startDir)
{
    QFileDialog * fileDialog = new QFileDialog(parent, caption);

    fileDialog->setFileMode(QFileDialog::DirectoryOnly);

    if(startDir.isEmpty())
        fileDialog->setDirectory(getLastOpenedDir());
    else
        fileDialog->setDirectory(startDir);

    QString lFile;
    fileDialog->setModal(true);
    if(fileDialog->exec() != QDialog::Accepted){
        delete fileDialog;
        return lFile;
    }

    lastOpenedDir = fileDialog->directory().absolutePath();
    
    if(poSettings)
        poSettings->writeSettings (QString ("System settings/"+kksAppNameEng), "LastOpenedDir", lastOpenedDir);

    QStringList files = fileDialog->selectedFiles();
    if (!files.isEmpty())
        lFile = files[0];

    delete fileDialog;

    if (!lFile.isEmpty() && lFile.right(1) != "/")
        lFile += "/";

    return lFile;
}

void KKSCoreApplication::loadLastOpenedDir()
{
    lastOpenedDir = getWDir();//QString(".");

    KKSSettings *kksSettings = kksCoreApp->getKKSSettings();
    if(!kksSettings)
        return;
        
    kksSettings->beginGroup (QString ("System settings/"+kksAppNameEng));

    if ( kksSettings->getParam("LastOpenedDir").isEmpty() )
    {
        kksSettings->endGroup();
        kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng), 
                                     "LastOpenedDir", 
                                     lastOpenedDir);
        kksSettings->beginGroup (QString ("System settings/"+kksAppNameEng));
    }
    
    lastOpenedDir = kksSettings->getParam("LastOpenedDir");

    kksSettings->endGroup();
}

void KKSCoreApplication::loadLastSelectedFilter()
{
    lastSelectedFilter = QString(tr("All files (*.*)"));

    KKSSettings *kksSettings = kksCoreApp->getKKSSettings();
    if(!kksSettings)
        return;
        
    kksSettings->beginGroup (QString ("System settings/"+kksAppNameEng));

    if ( kksSettings->getParam("LastSelectedFilter").isEmpty() )
    {
        kksSettings->endGroup();
        kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng), 
                                     "LastSelectedFilter", 
                                     lastSelectedFilter);
        kksSettings->beginGroup (QString ("System settings/"+kksAppNameEng));
    }
    
    lastSelectedFilter = kksSettings->getParam("LastSelectedFilter");

    kksSettings->endGroup();
}


KKSCoreApplication * KKSCoreApplication::init (int argc, 
                                               char *argv[], 
                                               //bool with_connect, 
                                               //const QString & userName, 
                                               bool msgToWindow)
{
    if ( selfCore )
        qFatal(tr("There should be only one KKSCoreApplication object").toLocal8Bit().constData());

//    if ( QCoreApplication::instance() ){
//        qFatal(tr("There are already exist a QApplication object. You did not create QApplication objects manually!").toLocal8Bit().constData());
//    }

    QApplication * app (0);
    if (QCoreApplication::instance() )
        app = qobject_cast<QApplication *>(QCoreApplication::instance() );

    if (!app)
        app = new QApplication(argc, argv);

    KKSCoreApplication * xG0;
    bool bDel = false;
    if ( !selfCore ){
        KKSCommandLineOpts * opts = KKSCoreApplication::parseCommandLineOptions(argc, argv);
        xG0 = new KKSCoreApplication (opts, msgToWindow);
        bDel = true;
    }
    else
        xG0 = selfCore;

    
    int ok = connectIfReady();
    if(ok != OK_CODE){
        if(bDel)
            delete xG0;
        return 0;
    }

    return xG0;

    /*
    if(kksCoreApp->commandLineOpts() && kksCoreApp->commandLineOpts()->withAutoConnect){
        KKSDatabase * m_db = xG0->db();

        if ( ! m_db->connected() )
        {
            int ok = 0;
            if(kksCoreApp->commandLineOpts()->readyToAutoConnect()){
                ok = autoConnect();
                if(ok != OK_CODE)
                    ok = GUIConnect();
            }
            else{
                ok = GUIConnect();
            }

            if(ok != OK_CODE){
                if(bDel)
                    delete xG0;
                return 0;
            }

        }
    }

    return xG0;
    */
}

int KKSCoreApplication::connectIfReady()
{
    if(kksCoreApp->commandLineOpts() && kksCoreApp->commandLineOpts()->withAutoConnect){
        KKSDatabase * m_db = kksCoreApp->db();

        if ( ! m_db->connected() )
        {
            int ok = 0;
            if(kksCoreApp->commandLineOpts()->readyToAutoConnect()){
                ok = autoConnect();
                if(ok != OK_CODE){
                    int b = QMessageBox::warning(0, 
                                                 tr("Connect to database"), 
                                                 tr("Cannot automatically connect to database!\nDo you want to connect with another connection params?"), 
                                                 QMessageBox::Yes | QMessageBox::No, 
                                                 QMessageBox::Yes);
                    if(b == QMessageBox::Yes)
                        ok = GUIConnect();
                    else
                        return OK_CODE;
                }
            }
            else{
                ok = GUIConnect();
            }

            if(ok != OK_CODE){
                return ERROR_CODE;
            }

        }
    }

    return OK_CODE;
}

int KKSCoreApplication::GUIConnect(QWidget * parent)
{
    if(!kksCoreApp)
        return ERROR_CODE;

    KKSDatabase * m_db = kksCoreApp->db();
    if(!m_db)
        return ERROR_CODE;

    const KKSCommandLineOpts * opts = kksCoreApp->commandLineOpts();

    KKSSettings * poSettings = kksCoreApp->getKKSSettings();
    KKSList<KKSAccLevel*> levels = kksCoreApp->getAccLevels();

    poSettings->beginGroup ("System settings/Database");
    
    QString ip;
    QString dbname;
    QString user;
    QString port;

    if(opts->host.isEmpty())
        ip = poSettings->getParam("hostName");
    else
        ip = opts->host;

    if(opts->database.isEmpty())
        dbname = poSettings->getParam("databaseName");
    else
        dbname = opts->database;

    if(opts->user.isEmpty())
		user = poSettings->getParam("userName");
	else
        user = opts->user;

    //QString port = poSettings->getParam("databasePort");
	
	

    QString currentLevel = poSettings->getParam("currentLevel");
    int currentLevelIndex = 0; //������������ ��� ����������� �������� ���������� ������� ������ ������� � ����������

    if(currentLevel.isEmpty()){
        if(levels.count() == 0){
            currentLevel = tr("Not Secret");
            if(opts->port.isEmpty())
                port = "5432";
            else
                port = opts->port;
        }
        else{
            currentLevel = levels.at(0)->name();
            if(opts->port.isEmpty())
                port = levels.at(0)->port();
            else
                port = opts->port;
        }
    }
    else{
        for(int i=0; i<levels.count(); i++){
            if(levels.at(i)->name() == currentLevel){
                currentLevelIndex = i;
                
                if(opts->port.isEmpty())
                    port = levels.at(i)->port();
                else
                    port = opts->port;
            }
        }
    }

    KKSAccLevel currLevel(currentLevel, port);

    LoginForm * lf = new LoginForm( levels, currLevel, opts->port.isEmpty() ? true:false, 0 );
    
    bool result = false;

    do
    {
        //lf->setAccLevels(m_accLevels);

        lf->setUser(user, opts->user.isEmpty() ? true : false);
        lf->setHost(ip, opts->host.isEmpty() ? true : false);
        lf->setName(dbname, opts->database.isEmpty() ? true : false);
        lf->setPort(port);
        //lf->setLevel(currentLevel);
        
#ifndef WIN32
        //���������� ������ ������ ������� ������� ���������. ������� ������� ��� ���� ������������ ��������
#endif
        result = ( lf->exec() == QDialog::Accepted );
        qWarning("result = %d\n", result);

        if ( ! result ) 
            break;

        if (! m_db->connect( lf->host(), 
                             lf->name(), 
                             lf->user(), 
                             lf->pass(),
                             lf->port() ) )
        {
            qCritical() << m_db->lastError();
            QMessageBox::critical( 0, 
                                   QObject::tr( "Error!" ), 
                                   QObject::tr(m_db->lastError()) );
        }
    } while ( ! m_db->connected() );

    poSettings->endGroup (); // System settings/Database
    //QString a = poSettings->group(); //a = ""

    if(result)
    {
        //poSettings->beginGroup("System settings");
        //qWarning() << "result = " << result;
        if(lf->host() != ip){
        //qWarning() << "HOST = " << lf->host() << ip;
            poSettings->writeSettings("System settings/Database", 
                                      "hostName", 
                                      lf->host());
        }
        if(lf->name() != dbname)
            poSettings->writeSettings("System settings/Database", 
                                      "databaseName", 
                                      lf->name());
        if(lf->user() != user)
            poSettings->writeSettings("System settings/Database", 
                                      "userName", 
                                      lf->user());
        
        //��������� ���������� � ������� ������ ������� � �����
        currLevel.setName(lf->level());
        currLevel.setPort(lf->port());
        levels.at(lf->levelIndex())->setName(currLevel.name());
        levels.at(lf->levelIndex())->setPort(currLevel.port());

        saveAccLevels(levels);//poSetings->group() ����� ������ ���� ����� ""
        
        poSettings->writeSettings("System settings/Database",
                                  "currentLevel",
                                  currLevel.name());
        

        
        //poSettings->endGroup();
    }

    delete lf;

    if(!m_db->connected())
    {
        return ERROR_CODE;
    }
    

    KKSDatabase * m_db1 = kksCoreApp->db1();
    m_db1->connect(m_db->getHost(), m_db->getName(), m_db->getUser(), m_db->getPass(), m_db->getPort());
    if(!m_db1->connected()){
        m_db->disconnect();
        return ERROR_CODE;
    }

    if(verifyConnection(parent) == ERROR_CODE)
        return ERROR_CODE;

    if(createNotifyReceiver() == ERROR_CODE)
        return ERROR_CODE;

    return OK_CODE;

    /*
    KKSResult * res = NULL;

    res = m_db->execute("select kkssitoversion1()");
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();
        
        qCritical() <<        tr("Cannot get information about current version of server software you connected to.\n\n"
                              "Further work is impossible. You should use equal versions of client and server software");
        QMessageBox::critical(parent, 
                              tr("Database version mismatch"), 
                              tr("Cannot get information about current version of server software you connected to.\n\n"
                              "Further work is impossible. You should use equal versions of client and server software"),
                              QMessageBox::Ok, QMessageBox::NoButton);

        return ERROR_CODE;
    }
    
    QString serverVersion = res->getCellAsString(0, 0);
    QString clientVersion = QString(KKS_VERSION);
    clientVersion = clientVersion.split("-").at(0);
    if(serverVersion != clientVersion){
        if(res)
            delete res;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();
        
        qCritical() <<        tr("Current version of your client software is %1,\n"
                              "Current version of server you connected to is %2.\n\n"
                              "Further work is impossible. You should use equal versions of client and server software").arg(clientVersion).arg(serverVersion);
        QMessageBox::critical(parent, 
                              tr("Database version mismatch"), 
                              tr("Current version of your client software is %1,\n"
                              "Current version of server you connected to is %2.\n\n"
                              "Further work is impossible. You should use equal versions of client and server software").arg(clientVersion).arg(serverVersion),
                              QMessageBox::Ok, QMessageBox::NoButton);
        return ERROR_CODE;
    }

    delete res;
    
    res = m_db->execute("select * from getUserDls();");
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();

        qCritical() <<        tr("You successfully connect with DB,\n"
                              "but no registered positions responsed.\n\n"
                              "Further work is impossible.");
        QMessageBox::critical(parent, 
                              tr("Database authorization error"), 
                              tr("You successfully connect with DB,\n"
                              "but no registered positions responsed.\n\n"
                              "Further work is impossible."),
                              QMessageBox::Ok, QMessageBox::NoButton);

        return ERROR_CODE;
    }

    int cnt = res->getRowCount();

    if(cnt == 1){
        int idDl = res->getCellAsInt(0, 0);
        QString sql = QString("select setCurrentDl(%1);").arg(idDl);
        KKSResult * r = m_db->execute(sql);
        if(!r || r->getCellAsInt(0, 0) != 1){
            if(r->getCellAsInt(0, 0) == -1){
                qCritical() << tr("Cannot make connection between\n"
                                      "current user and current position.\n"
                                      "Current user does not have rights of current position.");
                QMessageBox::critical(parent, 
                                      tr("Database authorization error"), 
                                      tr("Cannot make connection between\n"
                                      "current user and current position.\n"
                                      "Current user does not have rights of current position."), 
                                      QMessageBox::Ok, QMessageBox::NoButton);
            }
            else if(r->getCellAsInt(0, 0) == -2){
                qCritical() << tr("You are authorized for this position but not at current object.\nMake sure that you are at your home object!");
                QMessageBox::critical(parent, 
                                      tr("Database authorization error"), 
                                      tr("You are authorized for this position but not at current object.\nMake sure that you are at your home object!"), 
                                      QMessageBox::Ok, QMessageBox::NoButton);
            }

            if(r)
                delete r;
            delete res;
            m_db->disconnect();
            if(m_db1)
                m_db1->disconnect();
            return ERROR_CODE;
        }
        delete r;
        
        if(m_db1){
            r = m_db1->execute(sql);
            if(!r || r->getCellAsInt(0, 0) != 1){
                if(r)
                    delete r;
                delete res;
                m_db->disconnect();
                m_db1->disconnect();
                return ERROR_CODE;
            }
            delete r;
        }
        
        delete res;
        m_db->setCurrentDl(idDl);
        if(m_db1)
            m_db1->setCurrentDl(idDl);
        
        return OK_CODE;
    }
    
    ChooseDlForm * f = new ChooseDlForm(parent);
    for(int i=0; i<cnt; i++){
        int id = res->getCellAsInt(i, 0);
        QString dlName = res->getCellAsString(i, 4);
        QString unitName = res->getCellAsString(i, 5);
        f->addDl(id, dlName, dlName, unitName);
    }
    
    delete res;

    if(f->exec() != QDialog::Accepted ){
        qCritical() << tr("You haven't selected current position.\n"
                                                     "Further work is impossible.");
        QMessageBox::critical(parent, 
                              tr("Select position"),
                              tr("You haven't selected current position.\n"
                                                     "Further work is impossible."),
                              QMessageBox::Ok, QMessageBox::NoButton);

        delete f;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();
        return ERROR_CODE;
    }

    int idDl = f->currentDl();
    QString sql = QString("select setCurrentDl(%1);").arg(idDl);
    KKSResult * r = m_db->execute(sql);
    if(!r || r->getCellAsInt(0, 0) != 1){
        
        if(r->getCellAsInt(0, 0) == -1){
            qCritical() <<        tr("Cannot make connection between\n"
                                  "current user and current position.\n"
                                  "Current user does not have rights of current position.");
            QMessageBox::critical(parent, 
                                  tr("Database authorization error"), 
                                  tr("Cannot make connection between\n"
                                  "current user and current position.\n"
                                  "Current user does not have rights of current position."), 
                                  QMessageBox::Ok, QMessageBox::NoButton);
        }
        else if(r->getCellAsInt(0, 0) == -2){
            qCritical() << tr("You are authorized for this position but not at current object.\nMake sure that you are at your home object!");
            QMessageBox::critical(parent, 
                                  tr("Database authorization error"), 
                                  tr("You are authorized for this position but not at current object.\nMake sure that you are at your home object!"), 
                                  QMessageBox::Ok, QMessageBox::NoButton);
        }

        if(r)
            delete r;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();
        return ERROR_CODE;
    }
    
    delete r;

    if(m_db1){
        r = m_db1->execute(sql);
        if(!r || r->getCellAsInt(0, 0) != 1){
            if(r)
                delete r;
            m_db->disconnect();
            m_db1->disconnect();
            return ERROR_CODE;
        }
        delete r;
    }

    m_db->setCurrentDl(idDl);
    if(m_db1)
        m_db1->setCurrentDl(idDl);

    return OK_CODE;
    */
}


int KKSCoreApplication::autoConnect(QWidget * parent)
{
    if(!kksCoreApp)
        return ERROR_CODE;

    KKSDatabase * m_db = kksCoreApp->db();
    if(!m_db)
        return ERROR_CODE;

    if(!kksCoreApp->commandLineOpts() || !kksCoreApp->commandLineOpts()->readyToAutoConnect())
        return ERROR_CODE;

    KKSSettings * poSettings = kksCoreApp->getKKSSettings();

    poSettings->beginGroup ("System settings/Database");
	
    const KKSCommandLineOpts * opts = kksCoreApp->commandLineOpts();
    if (! m_db->connect( opts->host, 
                         opts->database, 
                         opts->user, 
                         opts->passwd,
                         opts->port.isEmpty() ? QString("5432") : opts->port ) )
    {
            qCritical() << m_db->lastError();
            QMessageBox::critical( 0, 
                                   QObject::tr( "Error!" ), 
                                   QObject::tr(m_db->lastError()) );

            poSettings->endGroup (); // System settings/Database
            return ERROR_CODE;
    }
    

    poSettings->endGroup (); // System settings/Database

    poSettings->writeSettings("System settings/Database", 
                              "hostName", 
                              opts->host);
    poSettings->writeSettings("System settings/Database", 
                              "databaseName", 
                              opts->database);

    poSettings->writeSettings("System settings/Database", 
                              "userName", 
                              opts->user);

    if(!m_db->connected())
        return ERROR_CODE;
    

    KKSDatabase * m_db1 = kksCoreApp->db1();
    m_db1->connect(m_db->getHost(), m_db->getName(), m_db->getUser(), m_db->getPass(), m_db->getPort());
    if(!m_db1->connected()){
        m_db->disconnect();
        return ERROR_CODE;
    }

    if(verifyConnection(parent) == ERROR_CODE)
        return ERROR_CODE;

    if(createNotifyReceiver() == ERROR_CODE)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSCoreApplication::connectToDb(const QString & host,
                                    const QString & database,
                                    const QString & user,
                                    const QString & passwd,
                                    const QString & port,
                                    QWidget * parent)
{
    if(!kksCoreApp)
        return ERROR_CODE;

    KKSDatabase * m_db = kksCoreApp->db();
    if(!m_db)
        return ERROR_CODE;

    KKSSettings * poSettings = kksCoreApp->getKKSSettings();

    poSettings->beginGroup ("System settings/Database");
	
    if (! m_db->connect( host, 
                         database, 
                         user, 
                         passwd,
                         port.isEmpty() ? QString("5432") : port ) )
    {
            qCritical() << m_db->lastError();
            QMessageBox::critical( 0, 
                                   QObject::tr( "Error!" ), 
                                   QObject::tr(m_db->lastError()) );

            poSettings->endGroup (); // System settings/Database
            return ERROR_CODE;
    }
    

    poSettings->endGroup (); // System settings/Database

    poSettings->writeSettings("System settings/Database", 
                              "hostName", 
                              host);
    poSettings->writeSettings("System settings/Database", 
                              "databaseName", 
                              database);

    poSettings->writeSettings("System settings/Database", 
                              "userName", 
                              user);

    if(!m_db->connected())
        return ERROR_CODE;
    

    KKSDatabase * m_db1 = kksCoreApp->db1();
    m_db1->connect(m_db->getHost(), m_db->getName(), m_db->getUser(), m_db->getPass(), m_db->getPort());
    if(!m_db1->connected()){
        m_db->disconnect();
        return ERROR_CODE;
    }

    if(verifyConnection(parent) == ERROR_CODE)
        return ERROR_CODE;

    
    if(createNotifyReceiver() == ERROR_CODE)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSCoreApplication::verifyConnection(QWidget * parent)
{
    KKSDatabase * m_db = kksCoreApp->db();
    KKSDatabase * m_db1 = kksCoreApp->db1();
    
    if(!m_db || !m_db->connected() || !m_db1 || !m_db1->connected())
        return ERROR_CODE;

    KKSResult * res = NULL;

    res = m_db->execute("select kkssitoversion1()");
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();
        
        qCritical() <<        tr("Cannot get information about current version of server software you connected to.\n\n"
                              "Further work is impossible. You should use equal versions of client and server software");
        QMessageBox::critical(parent, 
                              tr("Database version mismatch"), 
                              tr("Cannot get information about current version of server software you connected to.\n\n"
                              "Further work is impossible. You should use equal versions of client and server software"),
                              QMessageBox::Ok, QMessageBox::NoButton);

        return ERROR_CODE;
    }
    
    QString serverVersion = res->getCellAsString(0, 0);
    QString clientVersion = QString(KKS_VERSION);
    clientVersion = clientVersion.split("-").at(0);
    if(serverVersion != clientVersion){
        if(res)
            delete res;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();
        
        qCritical() <<        tr("Current version of your client software is %1,\n"
                              "Current version of server you connected to is %2.\n\n"
                              "Further work is impossible. You should use equal versions of client and server software").arg(clientVersion).arg(serverVersion);
        QMessageBox::critical(parent, 
                              tr("Database version mismatch"), 
                              tr("Current version of your client software is %1,\n"
                              "Current version of server you connected to is %2.\n\n"
                              "Further work is impossible. You should use equal versions of client and server software").arg(clientVersion).arg(serverVersion),
                              QMessageBox::Ok, QMessageBox::NoButton);
        return ERROR_CODE;
    }

    delete res;
    res = 0;
    
    int idDl = 0;

    int allowedDlCount = 0;//���� ����� �������� ���������� ����������, �� ������� �������� ������������

    if(kksCoreApp->commandLineOpts()->idDl > 0){
        idDl = kksCoreApp->commandLineOpts()->idDl;
    }
    else{
        res = m_db->execute("select * from getUserDls();");
        if(!res || res->getRowCount() == 0){
            if(res)
                delete res;
            m_db->disconnect();
            if(m_db1)
                m_db1->disconnect();

            qCritical() <<        tr("You successfully connect with DB,\n"
                                  "but no registered positions responsed.\n\n"
                                  "Further work is impossible.");
            QMessageBox::critical(parent, 
                                  tr("Database authorization error"), 
                                  tr("You successfully connect with DB,\n"
                                  "but no registered positions responsed.\n\n"
                                  "Further work is impossible."),
                                  QMessageBox::Ok, QMessageBox::NoButton);

            return ERROR_CODE;
        }

        allowedDlCount = res->getRowCount();
        if(allowedDlCount == 1)
            idDl = res->getCellAsInt(0, 0);
    }

    if(idDl > 0){
        //int idDl = res->getCellAsInt(0, 0);
        QString sql = QString("select setCurrentDl(%1);").arg(idDl);
        KKSResult * r = m_db->execute(sql);
        if(!r || r->getCellAsInt(0, 0) != 1){
            if(r->getCellAsInt(0, 0) == -1){
                qCritical() << tr("Cannot make connection between\n"
                                      "current user and current position.\n"
                                      "Current user does not have rights of current position.");
                QMessageBox::critical(parent, 
                                      tr("Database authorization error"), 
                                      tr("Cannot make connection between\n"
                                      "current user and current position.\n"
                                      "Current user does not have rights of current position."), 
                                      QMessageBox::Ok, QMessageBox::NoButton);
            }
            else if(r->getCellAsInt(0, 0) == -2){
                qCritical() << tr("You are authorized for this position but not at current object.\nMake sure that you are at your home object!");
                QMessageBox::critical(parent, 
                                      tr("Database authorization error"), 
                                      tr("You are authorized for this position but not at current object.\nMake sure that you are at your home object!"), 
                                      QMessageBox::Ok, QMessageBox::NoButton);
            }

            if(r)
                delete r;
            delete res;
            m_db->disconnect();
            if(m_db1)
                m_db1->disconnect();
            
            return ERROR_CODE;
        }

        delete r;
        
        if(m_db1){
            r = m_db1->execute(sql);
            if(!r || r->getCellAsInt(0, 0) != 1){
                if(r)
                    delete r;
                delete res;
                m_db->disconnect();
                m_db1->disconnect();
                return ERROR_CODE;
            }
            delete r;
        }
        
        delete res;
        m_db->setCurrentDl(idDl);
        if(m_db1)
            m_db1->setCurrentDl(idDl);
        
        return OK_CODE;
    }
    
    ChooseDlForm * f = new ChooseDlForm(parent);
    for(int i=0; i<allowedDlCount; i++){
        int id = res->getCellAsInt(i, 0);
        QString dlName = res->getCellAsString(i, 4);
        QString unitName = res->getCellAsString(i, 5);
        f->addDl(id, dlName, dlName, unitName);
    }
    
    delete res;

    if(f->exec() != QDialog::Accepted ){
        qCritical() << tr("You haven't selected current position.\n"
                                                     "Further work is impossible.");
        QMessageBox::critical(parent, 
                              tr("Select position"),
                              tr("You haven't selected current position.\n"
                                                     "Further work is impossible."),
                              QMessageBox::Ok, QMessageBox::NoButton);

        delete f;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();
        return ERROR_CODE;
    }

    idDl = f->currentDl();
    QString sql = QString("select setCurrentDl(%1);").arg(idDl);
    KKSResult * r = m_db->execute(sql);
    if(!r || r->getCellAsInt(0, 0) != 1){
        
        if(r->getCellAsInt(0, 0) == -1){
            qCritical() <<        tr("Cannot make connection between\n"
                                  "current user and current position.\n"
                                  "Current user does not have rights of current position.");
            QMessageBox::critical(parent, 
                                  tr("Database authorization error"), 
                                  tr("Cannot make connection between\n"
                                  "current user and current position.\n"
                                  "Current user does not have rights of current position."), 
                                  QMessageBox::Ok, QMessageBox::NoButton);
        }
        else if(r->getCellAsInt(0, 0) == -2){
            qCritical() << tr("You are authorized for this position but not at current object.\nMake sure that you are at your home object!");
            QMessageBox::critical(parent, 
                                  tr("Database authorization error"), 
                                  tr("You are authorized for this position but not at current object.\nMake sure that you are at your home object!"), 
                                  QMessageBox::Ok, QMessageBox::NoButton);
        }

        if(r)
            delete r;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();
        return ERROR_CODE;
    }
    
    delete r;

    if(m_db1){
        r = m_db1->execute(sql);
        if(!r || r->getCellAsInt(0, 0) != 1){
            if(r)
                delete r;
            m_db->disconnect();
            m_db1->disconnect();
            return ERROR_CODE;
        }
        delete r;
    }

    m_db->setCurrentDl(idDl);
    if(m_db1)
        m_db1->setCurrentDl(idDl);

    return OK_CODE;
}

const QString & KKSCoreApplication::getWDir() const
{
    return workingDir;
}

void KKSCoreApplication::initCoreFactories()
{
    if(!m_loader)
        m_loader = new KKSLoader();
    
    m_loader->setDb(db());

    if(!m_fileLoader)
        m_fileLoader = new KKSFileLoader();
    
    m_fileLoader->setDb(db());

    if(!m_eiof)
        m_eiof = new KKSEIOFactory();

    if(!m_ppf)
        m_ppf = new KKSPPFactory();

    m_eiof->setParams( m_fileLoader, m_ppf, db());
    m_ppf->setParams(m_loader, m_fileLoader, m_eiof, db());

}

KKSLoader * KKSCoreApplication::loader() const
{
    return m_loader;
}

KKSFileLoader * KKSCoreApplication::fileLoader() const
{
    return m_fileLoader;
}

KKSPPFactory * KKSCoreApplication::ppf() const
{
    return m_ppf;
}

KKSEIOFactory * KKSCoreApplication::eiof() const
{
    return m_eiof;
}


const KKSList<KKSAccLevel*> & KKSCoreApplication::getAccLevels() const
{
    return m_accLevels;
}


void KKSCoreApplication::loadAccLevels()
{
    KKSSettings *kksSettings = kksCoreApp->getKKSSettings();
    if(!kksSettings)
        return;
    
    //��������� ������� ������
    QString currentGroup = kksSettings->group();
    kksSettings->beginGroup("");

    int size = kksSettings->beginReadArray("System settings/Database/AccLevels");
    if(size == 0)
    {
        kksSettings->endArray();
        loadDefAccLevels();
        saveAccLevels(m_accLevels);
        size = kksSettings->beginReadArray("System settings/Database/AccLevels");
    }
    
    m_accLevels.clear();
    
    for (int i=0; i<size; i++) {
        kksSettings->setArrayIndex(i);
        KKSAccLevel * level = new KKSAccLevel();
        level->setName(kksSettings->value("levelName").toString());
        level->setPort(kksSettings->value("portName").toString());
        if(level->port().isEmpty())
            level->setPort("5432");
        m_accLevels.append(level);
        level->release();
    }

    kksSettings->endArray();

    kksSettings->beginGroup(currentGroup);
}

void KKSCoreApplication::saveAccLevels(const KKSList<KKSAccLevel*> & levels)
{
    KKSSettings *kksSettings = kksCoreApp->getKKSSettings();
    if(!kksSettings)
        return;

    int cnt = levels.count();
    
    //��������� ������� ������
    QString currentGroup = kksSettings->group();
    kksSettings->beginGroup("");

     kksSettings->beginWriteArray("System settings/Database/AccLevels");
     for (int i=0; i<cnt; i++) {
         kksSettings->setArrayIndex(i);
         kksSettings->setValue("levelName", levels.at(i)->name());
         kksSettings->setValue("portName", levels.at(i)->port());
     }
     
     kksSettings->endArray();

     kksSettings->beginGroup(currentGroup);
}

void KKSCoreApplication::loadDefAccLevels()
{
    QStringList levels;
    levels << tr("Not Secret") 
           << tr("Office Using")
           << tr("Secret")
           << tr("Top Secret")
           << tr("High Level");
    
    QStringList ports;
    ports  << tr("5432") 
           << tr("5433")
           << tr("5434")
           << tr("5435")
           << tr("5436");

    int cnt = levels.count();
    for(int i=0; i<cnt; i++){
        KKSAccLevel * level = new KKSAccLevel();
        level->setName(levels.at(i));
        level->setPort(ports.at(i));
        m_accLevels.append(level);
        level->release();
    }
}


void KKSCoreApplication::showConnectionInfo(QWidget * parent) const
{
    if(!m_loader || !m_loader->getDb()){
        qCritical() << tr("You are not connected to database");
        QMessageBox::critical(parent, tr("Warning"), tr("You are not connected to database"), QMessageBox::Ok);
        return;
    }

    ConnectionInfoForm * cif = new ConnectionInfoForm(parent);
    
    cif->setUserInfo(m_loader->getUserName(), m_loader->getDlName(), m_loader->getCurrMacLabelName());
    cif->setDatabaseInfo(m_loader->getDb()->getHost(), m_loader->getDb()->getPort(), m_loader->getDb()->getName());
    cif->exec();
    delete cif;
}

void KKSCoreApplication::showCommandLineParamsHelp(QWidget * parent)
{
    CommandLineParamsForm * f = new CommandLineParamsForm(parent);
    
    f->exec();
    delete f;
}


void KKSCoreApplication::showConnectionInfo(KKSDatabase * db, const QString & userName, const QString & dlName, const QString & macLabel, QWidget * parent) const
{
    if(!db){
        qCritical() << tr("You are not connected to database");
        QMessageBox::critical(parent, tr("Warning"), tr("You are not connected to database"), QMessageBox::Ok);
        return;
    }

    ConnectionInfoForm * cif = new ConnectionInfoForm(parent);
    
    cif->setUserInfo(userName, dlName, macLabel);
    cif->setDatabaseInfo(db->getHost(), db->getPort(), db->getName());
    cif->exec();
    delete cif;
}

QTextStream & KKSCoreApplication::logStream()
{
    return m_logStream;
}

KKSPluginLoader * KKSCoreApplication::pluginLoader() const
{
    return m_pluginLoader;
}

/**************************************************/
#include <stdio.h>
#include <string.h>

int   opterr = 1,    // if error message should be printed
      optind = 1,    // index into parent argv vector
      optopt,        // character checked for validity
      optreset;      // reset getopt
char *optarg;        // argument associated with option 

#define  BADCH (int)'?'
#define  BADARG   (int)':'
#define  EMSG  ""

/*
 * getopt --
 * Parse argc/argv argument vector.
 */
int getopt(int argc, char * const argv[], const char *optstring)
{
    static char *place = EMSG;    // option letter processing 
    const char *oli;                    // option letter list index 

    if (optreset || *place == 0) {// update scanning pointer 
        optreset = 0;
        place = argv[optind];
        if (optind >= argc || *place++ != '-') {
            // Argument is absent or is not an option
            place = EMSG;
            return (-1);
        }

        optopt = *place++;
        
        if (optopt == '-' && *place == 0) {
            // "--" => end of options
            ++optind;
            place = EMSG;
            return (-1);
        }
        if (optopt == 0) {
            // Solitary '-', treat as a '-' option if the program (eg su) is looking for it.
            place = EMSG;
            if (strchr(optstring, '-') == NULL)
                return -1;
            optopt = '-';
        }
    } 
    else
      optopt = *place++;

    // See if option letter is one the caller wanted... 
    if (optopt == ':' || (oli = strchr(optstring, optopt)) == NULL) {
        if (*place == 0)
            ++optind;
        if (opterr && *optstring != ':')
            (void)fprintf(stderr, "unknown option -- %cn", optopt);
        
        return (BADCH);
    }

    // Does this option need an argument?
    if (oli[1] != ':') {
        // don't need argument
        optarg = NULL;
        if (*place == 0)
            ++optind;
    } 
    else {
        // Option-argument is either the rest of this argument or the entire next argument.
        if (*place)
            optarg = place;
        else if (argc > ++optind)
            optarg = argv[optind];
        else {
            // option-argument absent
            place = EMSG;
            if (*optstring == ':')
                return (BADARG);
            if (opterr)
                (void)fprintf(stderr, "option requires an argument -- %cn", optopt);
            
            return (BADCH);
        }
      
        place = EMSG;
        ++optind;
    }

    return (optopt);// return option letter
}

/**************************************************/


KKSCommandLineOpts * KKSCoreApplication::parseCommandLineOptions(int argc, char *argv[])
{
    KKSCommandLineOpts * kksOpts = new KKSCommandLineOpts();

    if(argc == 1) { // ���� ��������� ��� ����������, ������� �������
        return kksOpts;
    }

    char *opts = "hH:p:U:x:b:o:cwd:"; // ��������� �����, ������ ��������� ��������
    int opt; // ������ ��������� ����� �������� ����
    while((opt = getopt(argc, argv, opts)) != -1) { // �������� getopt ���� ��� �� ������ -1
        switch(opt) {
            case 'h':
                kksOpts->showHelp = true;
                break;
            case 'H': 
                kksOpts->host = QString::fromLocal8Bit(optarg);
                break;
            case 'p': 
                kksOpts->port = QString::fromLocal8Bit(optarg);
                break;
            case 'U': 
                kksOpts->user = QString::fromLocal8Bit(optarg);
                break;
            case 'x': 
                kksOpts->passwd = QString::fromLocal8Bit(optarg);
                break;
            case 'b': 
                kksOpts->database = QString::fromLocal8Bit(optarg);
                break;
            case 'o': 
                kksOpts->idObject = QString::fromLocal8Bit(optarg).toInt();
                break;
            case 'c': 
                kksOpts->withAutoConnect = true;
                break;
            case 'w': 
                kksOpts->showLoginWindow = true;
                break;
            case 'd': 
                kksOpts->idDl = QString::fromLocal8Bit(optarg).toInt();
                break;
        }
    }

    optreset = 1;
    optind = 1;

    return kksOpts;
}


int KKSCoreApplication::createNotifyReceiver()
{
    if(!kksCoreApp)
        return ERROR_CODE;

    KKSDatabase * m_db = kksCoreApp->db();

    if(!m_db->connected())
    {
        return ERROR_CODE;
    }
    

    KKSDatabase * m_db2 = kksCoreApp->db2();

    m_db2->connect(m_db->getHost(), m_db->getName(), m_db->getUser(), m_db->getPass(), m_db->getPort());
    if(!m_db2->connected()){
        //m_db->disconnect();
        return ERROR_CODE;
    }
    m_db2->startListen();

    kksCoreApp->m_notifyReceiver = new KKSNotifyReceiver(m_db2, 0);
    kksCoreApp->m_notifyReceiver->setKKSCoreApp(kksCoreApp);
    connect(kksCoreApp->m_notifyReceiver, SIGNAL(databaseNotifyReceived(const QString &, const QString &, const QString &)), kksCoreApp->loader(), SIGNAL(databaseNotifyReceived(const QString &, const QString &, const QString &)));

    kksCoreApp->m_notifyReceiver->start();

    return OK_CODE;
}
