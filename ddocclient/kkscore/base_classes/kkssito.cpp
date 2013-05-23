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
#include <QString>

#include "kkssito.h"
#include "defines.h"
#include <kkspgdatabase.h>
#include <kksdatabase.h>

//#include "KKSRecordFactory.h"
#include "KKSLoader.h"
#include "KKSFileLoader.h"
#include "KKSPPFactory.h"
#include "KKSEIOFactory.h"
#include "KKSObjEditorFactory.h"
#include "KKSCatEditorFactory.h"
#include "KKSTemplateEditorFactory.h"
#include "KKSRubricFactory.h"
#include "KKSStuffFactory.h"
#include "KKSAttributesFactory.h"
#include "KKSIndFactory.h"

#include "kkssettings.h"
#include "cmdjournalsettingsform.h"
#include "msgjournalsettingsform.h"

#include "connectioninfoform.h"

#include "loginform.h"
#include "choosedlform.h"

#include "kksclient_name.h"

__CORE_EXPORT KKSSito * KKSSito::self = 0;

const QString kksSitoNameEng = QString(EXECUTABLE_PREFIX);

/*!\brief Конструктор класса.

Необходимо отметить, что в приложении можно создать только один экземпляр данного класса. При попытке создать второй, приложение завершится аварийно.

Устанавливаются локальная кодировка и кодировка сервера баз данных по умолчанию. В качестве первой используется текущая системная (делается вызов QTextCodec::codecForLocale()). В качестве второй используется кодировка KOI8-R (делается вызов QTextCodec::codecForName("KOI8-R")).

Кроме того загружаются необходимые данные для класса QTranslator (русификация). По умолчанию используется файл kkssito_ru.ts в корневом каталоге приложения.
*/
KKSSito::KKSSito(const QString & userName) :
    poDb (0),
    m_objf (0),
    m_catf (0),
    m_tf (0),
    m_attrf (0),
    m_indf (0)
{
    if ( self )
        qFatal("There should be only one KKSSito object");

    poSettings = 0;
    lastError = 0;
    poDb1 = 0;
    tor = 0;

    Q_INIT_RESOURCE (ddocs_icon_set);
    Q_INIT_RESOURCE (ddocs_icon_set1);

    m_savedCmdJSettings = NULL;
    m_savedMsgJSettings = NULL;

    m_loader = 0;
    m_fileLoader = 0;
    m_ppf = NULL;
    m_eiof = NULL;
    //m_awf = NULL;
    m_rf = 0;
    m_sf = 0;

    self = this;
	allowedUserName = userName;

    QDir dir;
    workingDir = dir.absolutePath();

    loadTranslator();
    loadLastOpenedDir();
    loadLastSelectedFilter();
    loadCmdJSetings();
    loadMsgJSetings();
    loadAccLevels();

    initFactories();

    getLastError();//это типа чтоп файл журнала создать в текущей папке (там где exe-шник лежЫт)
}

void KKSSito::loadTranslator()
{
    KKSSettings *kksSettings = kksSito->getKKSSettings();
    if(!kksSettings)
        return;
    
    QString transl_path = getWDir();
    
    kksSettings->beginGroup (QString("System settings/") + kksSitoNameEng);

    if ( kksSettings->getParam("Translators_path").isEmpty() )
    {
        kksSettings->endGroup();
        kksSettings->writeSettings (QString("System settings/") + kksSitoNameEng, 
                                     "Translators_path", 
                                     transl_path);
        kksSettings->beginGroup (QString("System settings/") + kksSitoNameEng);
    }
    
    transl_path = kksSettings->getParam("Translators_path");

    kksSettings->endGroup();
    
    qDebug() << "Try to load translator files from " << transl_path;

    if(tor){
        QApplication::removeTranslator(tor);
        delete tor;
    }
    tor = new QTranslator(0);
    tor->load(QString("ddocclient_ru"), transl_path);
    QApplication::installTranslator(tor);

#ifdef _FOR_LESSONS_SHEDULER_    

#endif
}


/*!\brief Деструктор класса.

Удаляется созданный экземпляр класса GISdb.*/
KKSSito::~KKSSito( )
{

    if(poDb)
        delete poDb;

    if(poDb1)
        delete poDb1;

    if ( tor )
        delete tor;

#ifdef _FOR_LESSONS_SHEDULER_
    if(tor_sh)
        delete tor_sh;
    if(tor_su)
        delete tor_su;
#endif

    if(lastError)
        delete lastError;
    
    if(poSettings)
        delete poSettings;

    //if(m_rf) 
    //    delete m_rf;

    if(m_loader) 
        delete m_loader;

    if(m_fileLoader) 
        delete m_fileLoader;

    if(m_ppf) 
        delete m_ppf;

    if(m_eiof) 
        delete m_eiof;

    if(m_objf)
        delete m_objf;

//    if(m_awf)
//        delete m_awf;

    if (m_rf)
        delete m_rf;

    if (m_catf && m_tf)
    {
        m_catf->setTemplateEditorFactory (0);
        m_tf->setCatEditorFactory (0);
    }

    if (m_catf)
        delete m_catf;

    if (m_tf)
        delete m_tf;

    if(m_savedCmdJSettings)
        delete m_savedCmdJSettings;;
    if(m_savedMsgJSettings)
        delete m_savedMsgJSettings;


    self = 0;
}


KKSDatabase * KKSSito::db() const 
{
    if ( ! poDb ){
        poDb = new KKSPGDatabase();
        QObject::connect(poDb, SIGNAL(disconnected()), this, SLOT(clearLoader()));
    }

    return poDb;
}

KKSDatabase * KKSSito::db1() const 
{
    if ( ! poDb1 ){
        poDb1 = new KKSPGDatabase();
    }

    return poDb1;
}

void KKSSito::clearLoader()
{
    if(!m_loader)
        return;

    m_loader->clearCache();
}

/*!\brief Метод возвращает последнюю возникшую ошибку*/
KKSError * KKSSito::getLastError()
{
    if ( ! lastError ){
        lastError = new KKSError();
        QString fileName = QString( getWDir() + "/kks_err.log" );
        lastError->setLogFile(fileName);
    }

    return lastError;
}

/*!\brief Метод обновляет информацию о последней возникшей ошибке. 

Параметры данного метода имеют такой же смысл, как и у метода KKSError::init().*/
void KKSSito::updateLastError(KKSError::ErrorTypes type, QString code, QString name, QString message, QString group)
{
    lastError->init(type, 
                    code, 
                    name, 
                    message, 
                    group);
}

/*!\brief Метод возвращает указатель на набор библиотечных установок.

Используйте возвращаемый объект, чтобы получить доступ к установкам. В приложении может быть создан только один экземпляр данного класса.

Внимание! Возвращается указатель на внутреннюю структуру памяти. Нельзя удалять или модифицировать данную память.*/
KKSSettings * KKSSito::getKKSSettings()
{
    if ( ! poSettings)
    {
        poSettings = new KKSSettings( ORGANIZATION , PRODUCT_FULL_NAME);
    }

    poSettings->readSettings ( );

    return poSettings;
}

/*!\brief Метод возвращает указатель на набор библиотечных установок.

Используйте возвращаемый объект, чтобы получить доступ к установкам. В приложении может быть создан только один экземпляр данного класса.

Внимание! Возвращается указатель на внутреннюю структуру памяти. Нельзя удалять или модифицировать данную память.*/
KKSSettings * KKSSito::getKKSSettings (const QString & organization, const QString & application )
{
    if ( ! poSettings)
    {
        poSettings = new KKSSettings (organization, application);
    }


    poSettings->readSettings ( );
    return poSettings;
}


/*!\brief Метод возвращает указатель на набор библиотечных установок c заданием файла для их сохранения.

Используйте возвращаемый объект, чтобы получить доступ к установкам. В приложении может быть создан только один экземпляр данного класса.

Внимание! Возвращается указатель на внутреннюю структуру памяти. Нельзя удалять или модифицировать данную память.*/
KKSSettings * KKSSito::getKKSSettings (const QString & fileName)
{
    if ( ! poSettings)
    {
        poSettings = new KKSSettings ( fileName );
    }

    poSettings->readSettings ( );
    return poSettings;
}

/*!\brief Метод вызывает стандартный диалог выбора набора существующих файлов и возвращает их названия (включая абсолютный путь) в виде списка.

Рекомендуется использовать именно данный метод, а не стандартный вызов QFileDialog::getOpenFileNames(), поскольку данный метод запоминает последний открытый каталог и выбранный фильтр для файлов.

В случае, если ни одного файла не было выбрано, метод вернет пустой список.

\param parent Указатель на родительское окно
\param caption Информационная надпись в верхней части диалога выбора файла
\param filters Список строк с фильтрами расширений файлов. Строка фильтра может быть задана, например, следующим образом: "X Pixmap (*.XPM *.xpm)".
\param startDir начальный каталог диалога выбора файла. Если не задан или передается пустая строка, то будет использовано значение getLastOpenedDir().
\param startFilter Фильтр, который должен быть выбран в диалоге. Если указанного значения не будет найдено в списке фильтров диалога, то оно будет игнорироваться
*/
QStringList KKSSito::getOpenFileNames(QWidget * parent, QString caption, QStringList filters, QString startDir, QString startFilter)
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
        poSettings->writeSettings (QString ("System settings/"+kksSitoNameEng), "LastOpenedDir", lastOpenedDir);

    lastSelectedFilter = fileDialog->selectedFilter();
    if(poSettings)
        poSettings->writeSettings (QString ("System settings/"+kksSitoNameEng), "LastSelectedFilter", lastSelectedFilter);

    lFiles = fileDialog->selectedFiles();

    delete fileDialog;

    return lFiles;
}

/*!\brief Метод вызывает стандартный диалог выбора существующего файла и возвращает его название (включая абсолютный путь к нему).

Рекомендуется использовать именно данный метод, а не стандартный вызов QFileDialog::getOpenFileName(), поскольку данный метод запоминает последний открытый каталог и выбранный фильтр для файлов.

В случае, если ни одного файла не было выбрано, метод вернет пустую строку.

\param parent Указатель на родительское окно
\param caption Информационная надпись в верхней части диалога выбора файла
\param filters Список строк с фильтрами расширений файлов. Строка фильтра может быть задана, например, следующим образом: "X Pixmap (*.XPM *.xpm)".
\param startDir начальный каталог диалога выбора файла. Если не задан или передается пустая строка, то будет использовано значение getLastOpenedDir().
\param startFilter Фильтр, который должен быть выбран в диалоге. Если указанного значения не будет найдено в списке фильтров диалога, то оно будет игнорироваться
*/
QString KKSSito::getOpenFileName(QWidget * parent, QString caption, QStringList filters, QString startDir, QString startFilter)
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
        poSettings->writeSettings (QString ("System settings/"+kksSitoNameEng), "LastOpenedDir", lastOpenedDir);

    lastSelectedFilter = fileDialog->selectedFilter();
    if(poSettings)
        poSettings->writeSettings (QString ("System settings/"+kksSitoNameEng), "LastSelectedFilter", lastSelectedFilter);

    QStringList files = fileDialog->selectedFiles();
    if (!files.isEmpty())
        lFile = files[0];

    delete fileDialog;

    return lFile;
}

/*!\brief Метод вызывает стандартный диалог выбора файла для сохранения (т.е. файл может не существовать) и возвращает его название (включая абсолютный путь к нему).

Рекомендуется использовать именно данный метод, а не стандартный вызов QFileDialog::getSaveFileName(), поскольку данный метод запоминает последний открытый каталог и выбранный фильтр для файлов.

В случае, если ни одного файла не было выбрано, метод вернет пустую строку.

\param parent Указатель на родительское окно
\param caption Информационная надпись в верхней части диалога выбора файла
\param filters Список строк с фильтрами расширений файлов. Строка фильтра может быть задана, например, следующим образом: "X Pixmap (*.XPM *.xpm)".
\param selectedFilter указатель на строку с выбранным в диалоге фильтром. Если в качестве значения данного параметра будет указан некоротый фильтр из списка, описанного выше, то данный фильтр в диалоге станет текущим. Параметр может быть задан следующим образом: QString * filter = new QString(); После вызова данного метода параметр будет содержать выбранный в диалоге фильтр.
\param startDir начальный каталог диалога выбора файла. Если не задан или передается пустая строка, то будет использовано значение getLastOpenedDir().
*/
QString KKSSito::getSaveFileName(QWidget * parent, QString caption, QStringList filters, QString * selectedFilter, QString startDir)
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
        poSettings->writeSettings (QString ("System settings/"+kksSitoNameEng), 
                                   "LastOpenedDir", 
                                   lastOpenedDir);

    lastSelectedFilter = fileDialog->selectedFilter();
    if(selectedFilter) 
        *selectedFilter = fileDialog->selectedFilter();
    if(poSettings)
        poSettings->writeSettings (QString ("System settings/"+kksSitoNameEng), 
                                   "LastSelectedFilter", 
                                   lastSelectedFilter);

    QStringList files = fileDialog->selectedFiles();
    if (!files.isEmpty())
        lFile = files[0];

    delete fileDialog;

    return lFile;
}

/*!\brief Метод вызывает стандартный диалог выбора существующего каталога и возвращает его название (включая абсолютный путь к нему).

Рекомендуется использовать именно данный метод, а не стандартный вызов QFileDialog::getExistingDirectory(), поскольку данный метод запоминает последний открытый каталог.

В случае, если ни одного каталога не было выбрано, метод вернет пустую строку.

\param parent Указатель на родительское окно
\param caption Информационная надпись в верхней части диалога выбора каталога
\param startDir начальный каталог диалога. Если не задан или передается пустая строка, то будет использовано значение getLastOpenedDir().
*/
QString KKSSito::getExistingDir(QWidget * parent, QString caption, QString startDir)
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
        poSettings->writeSettings (QString ("System settings/"+kksSitoNameEng), "LastOpenedDir", lastOpenedDir);

    QStringList files = fileDialog->selectedFiles();
    if (!files.isEmpty())
        lFile = files[0];

    delete fileDialog;

    if (!lFile.isEmpty() && lFile.right(1) != "/")
        lFile += "/";

    return lFile;
}

void KKSSito::loadLastOpenedDir()
{
    lastOpenedDir = getWDir();//QString(".");

    KKSSettings *kksSettings = kksSito->getKKSSettings();
    if(!kksSettings)
        return;
        
    kksSettings->beginGroup (QString ("System settings/"+kksSitoNameEng));

    if ( kksSettings->getParam("LastOpenedDir").isEmpty() )
    {
        kksSettings->endGroup();
        kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng), 
                                     "LastOpenedDir", 
                                     lastOpenedDir);
        kksSettings->beginGroup (QString ("System settings/"+kksSitoNameEng));
    }
    
    lastOpenedDir = kksSettings->getParam("LastOpenedDir");

    kksSettings->endGroup();
}

void KKSSito::loadLastSelectedFilter()
{
    lastSelectedFilter = QString(tr("All files (*.*)"));

    KKSSettings *kksSettings = kksSito->getKKSSettings();
    if(!kksSettings)
        return;
        
    kksSettings->beginGroup (QString ("System settings/"+kksSitoNameEng));

    if ( kksSettings->getParam("LastSelectedFilter").isEmpty() )
    {
        kksSettings->endGroup();
        kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng), 
                                     "LastSelectedFilter", 
                                     lastSelectedFilter);
        kksSettings->beginGroup (QString ("System settings/"+kksSitoNameEng));
    }
    
    lastSelectedFilter = kksSettings->getParam("LastSelectedFilter");

    kksSettings->endGroup();
}


KKSSito * KKSSito::init (bool with_connect, const QString & userName)
{
    QApplication * app;
	
    if ( !QCoreApplication::instance() )
    {
        int argc = 1;
        char ** argv = new char *;
        argv[0] = new char;
        app = new QApplication (argc, argv);
    }
    else
        app = static_cast <QApplication *> (QCoreApplication::instance());

    KKSSito * xG0;
    bool bDel = false;
    if ( !self ){
        xG0 = new KKSSito (userName);
        bDel = true;
    }
    else
        xG0 = self;

    if(with_connect){
        KKSDatabase * m_db = xG0->db();

        if ( ! m_db->connected() )
        {

            int ok = GUIConnect();
            if(ok != OK_CODE)
            {
                if(bDel)
                    delete xG0;
                return 0;
            }
        }
    }

    return xG0;
}

int KKSSito::GUIConnect(QWidget * parent)
{
    if(!kksSito)
        return ERROR_CODE;

    KKSDatabase * m_db = kksSito->db();
    if(!m_db)
        return ERROR_CODE;

    KKSSettings * poSettings = kksSito->getKKSSettings();
    KKSList<KKSAccLevel*> levels = kksSito->getAccLevels();

    poSettings->beginGroup ("System settings/Database");
    QString ip = poSettings->getParam("hostName");
    QString dbname = poSettings->getParam("databaseName");
    //QString port = poSettings->getParam("databasePort");
	
	QString user;
	if(kksSito->allowedUserName.isEmpty())
		user = poSettings->getParam("userName");
	else
		user = kksSito->allowedUserName;

    QString currentLevel = poSettings->getParam("currentLevel");
    QString port;

    if(currentLevel.isEmpty()){
        if(levels.count() == 0){
            currentLevel = tr("Not Secret");
            port = "5432";
        }
        else{
            currentLevel = levels.at(0)->name();
            port = levels.at(0)->port();
        }
    }
    else{
        for(int i=0; i<levels.count(); i++){
            if(levels.at(i)->name() == currentLevel)
                port = levels.at(i)->port();
        }
    }

    KKSAccLevel currLevel(currentLevel, port);
    LoginForm * lf = new LoginForm( levels, currLevel, 0 );
    
    bool result = false;

    do
    {
        //lf->setAccLevels(m_accLevels);

		lf->setUser(user, kksSito->allowedUserName.isEmpty() ? true : false);
        lf->setHost(ip);
        lf->setName(dbname);
        //lf->setPort(port);
        //lf->setLevel(currentLevel);
        
#ifndef WIN32
        //установить список выбора уровней доступа невидимым. Уровень доступа под мсвс определяется системой
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
            //char* l_err_mess = m_db->lastError();
            //qDebug () << __PRETTY_FUNCTION__ << l_err_mess;
            QMessageBox::critical( 0, 
                                   QObject::tr( "Error!" ), 
                                   QObject::tr(m_db->lastError()) );
        }
    } while ( ! m_db->connected() );

    poSettings->endGroup (); // Database

    if(result)
    {
        poSettings->beginGroup("System settings");
        qWarning() << "result = " << result;
        if(lf->host() != ip){
        qWarning() << "HOST = " << lf->host() << ip;
            poSettings->writeSettings("Database", 
                                      "hostName", 
                                      lf->host());
        }
        if(lf->name() != dbname)
            poSettings->writeSettings("Database", 
                                      "databaseName", 
                                      lf->name());
        if(lf->user() != user)
            poSettings->writeSettings("Database", 
                                      "userName", 
                                      lf->user());
        /*
        if(lf->port() != port)
            poSettings->writeSettings("Database", 
                                      "databasePort", 
                                      lf->port());
        */
        
        //закомментировано, чтобы порт всегда обновлялся
        if(lf->level() != currentLevel)
            poSettings->writeSettings("Database",
                                      "currentLevel",
                                      lf->level());
        
        poSettings->endGroup();
    }

    delete lf;

    if(!m_db->connected())
    {
        return ERROR_CODE;
    }
    

#ifndef _FOR_LESSONS_SHEDULER_ //Если выполняется проект составления расписаний (для ВКА) 
                               //необходимо сократить трафик по сети и нагрузку на сервер.
                               //для этого мы убираем это соединение.
    KKSDatabase * m_db1 = kksSito->db1();
    m_db1->connect(m_db->getHost(), m_db->getName(), m_db->getUser(), m_db->getPass(), m_db->getPort());
    if(!m_db1->connected()){
        m_db->disconnect();
        return ERROR_CODE;
    }
#else
    KKSDatabase * m_db1 = NULL;
#endif 

    KKSResult * res = NULL;

#ifndef _FOR_LESSONS_SHEDULER_
    res = m_db->execute("select kkssitoversion1()");
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();
        
        QMessageBox::critical(parent, 
                              tr("Database version mismatch"), 
                              tr("Cannot get information about current version of server software you connected to.\n\n"
                              "Further work is impossible. You should use equal versions of client and server software"),
                              QMessageBox::Ok, QMessageBox::NoButton);

        return ERROR_CODE;
    }
    
    QString serverVersion = res->getCellAsString(0, 0);
    if(serverVersion != QString(KKS_VERSION)){
        if(res)
            delete res;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();
        
        QMessageBox::critical(parent, 
                              tr("Database version mismatch"), 
                              tr("Current version of your client software is %1,\n"
                              "Current version of server you connected to is %2.\n\n"
                              "Further work is impossible. You should use equal versions of client and server software").arg(KKS_VERSION).arg(serverVersion),
                              QMessageBox::Ok, QMessageBox::NoButton);
        return ERROR_CODE;
    }

    delete res;
#endif
    
    res = m_db->execute("select * from getUserDls();");
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        m_db->disconnect();
        if(m_db1)
            m_db1->disconnect();

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
                QMessageBox::critical(parent, 
                                      tr("Database authorization error"), 
                                      tr("Cannot make connection between\n"
                                      "current user and current position.\n"
                                      "Current user does not have rights of current position."), 
                                      QMessageBox::Ok, QMessageBox::NoButton);
            }
            else if(r->getCellAsInt(0, 0) == -2){
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
            QMessageBox::critical(parent, 
                                  tr("Database authorization error"), 
                                  tr("Cannot make connection between\n"
                                  "current user and current position.\n"
                                  "Current user does not have rights of current position."), 
                                  QMessageBox::Ok, QMessageBox::NoButton);
        }
        else if(r->getCellAsInt(0, 0) == -2){
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

QString & KKSSito::getWDir()
{
    return workingDir;
}

void KKSSito::initFactories()
{
    if(!m_loader)
        m_loader = new KKSLoader();
    
    m_loader->setDb(db());

    if(!m_fileLoader)
        m_fileLoader = new KKSFileLoader();
    
    m_fileLoader->setDb(db());

    //if(!m_ppf)
    //    m_ppf = new KKSPPFactory();

    //m_ppf->setDb(db());

    if(!m_eiof)
        m_eiof = new KKSEIOFactory();

    if(!m_ppf)
        m_ppf = new KKSPPFactory();

    m_eiof->setParams( m_fileLoader, m_ppf, db());
    m_ppf->setParams(m_loader, m_fileLoader, m_eiof, db());

    if (!m_attrf)
        m_attrf = new KKSAttributesFactory (m_loader, m_eiof, NULL, m_ppf);

    if (!m_objf)
        m_objf = new KKSObjEditorFactory(m_ppf, m_loader, m_fileLoader, m_attrf, m_eiof);

    m_attrf->setOEF(m_objf);

    if (!m_sf)
        m_sf = new KKSStuffFactory (m_loader, m_ppf, m_objf, db());

    if (!m_rf)
        m_rf = new KKSRubricFactory (m_loader, m_ppf, m_eiof, m_objf, m_sf);

    if (m_objf && m_rf)
        m_objf->setRubrFactory (m_rf);

    if (!m_catf)
        m_catf = new KKSCatEditorFactory (m_loader, m_objf, m_ppf, m_eiof, m_attrf, m_sf);

    if (!m_tf)
        m_tf = new KKSTemplateEditorFactory (m_loader, m_ppf);

    if (!m_indf)
        m_indf = new KKSIndFactory (db(), m_loader);

    if (m_objf && m_indf)
    {
        m_objf->setIndicesFactory (m_indf);
        m_indf->setOEF (m_objf);
    }

    if (m_catf && m_tf)
    {
        m_catf->setTemplateEditorFactory (m_tf);
        m_tf->setCatEditorFactory (m_catf);
    }

    if (m_sf && m_objf)
        m_objf->setStuffFactory (m_sf);
}

KKSLoader * KKSSito::loader() const
{
    return m_loader;
}

KKSFileLoader * KKSSito::fileLoader() const
{
    return m_fileLoader;
}

KKSPPFactory * KKSSito::ppf() const
{
    return m_ppf;
}

KKSEIOFactory * KKSSito::eiof() const
{
    return m_eiof;
}

KKSObjEditorFactory *KKSSito::oef() const
{
    return m_objf;
}

KKSRubricFactory * KKSSito::rf () const
{
    return m_rf;
}

KKSStuffFactory *KKSSito::sf () const
{
    return m_sf;
}

KKSIndFactory * KKSSito::indf () const
{
    return m_indf;
}

KKSCmdJSettings * KKSSito::getCmdJSettings()
{
    return m_savedCmdJSettings;
}

KKSMsgJSettings * KKSSito::getMsgJSettings()
{
    return m_savedMsgJSettings;
}

KKSAttributesFactory * KKSSito::attrf () const
{
    return m_attrf;
}

void KKSSito::loadCmdJSetings()
{
    KKSSettings *kksSettings = kksSito->getKKSSettings();
    if(!kksSettings)
        return;
    
    m_savedCmdJSettings = new KKSCmdJSettings();
    
    kksSettings->beginGroup (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"));

    if ( kksSettings->getParam("bMyInput").isEmpty() )
    {
        kksSettings->endGroup();
        saveCmdJSettings(m_savedCmdJSettings);
        kksSettings->beginGroup (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"));
    }
    
    if(kksSettings->getParam("bMyInput") == "TRUE")
        m_savedCmdJSettings->bMyInput = true;
    else
        m_savedCmdJSettings->bMyInput = false;

    if(kksSettings->getParam("bAllInput") == "TRUE")
        m_savedCmdJSettings->bAllInput= true;
    else
        m_savedCmdJSettings->bAllInput = false;

    if(kksSettings->getParam("bAllOutput") == "TRUE")
        m_savedCmdJSettings->bAllOutput = true;
    else
        m_savedCmdJSettings->bAllOutput = false;

    if(kksSettings->getParam("bInputArchived") == "TRUE")
        m_savedCmdJSettings->bInputArchived = true;
    else
        m_savedCmdJSettings->bInputArchived = false;

    if(kksSettings->getParam("bInputExecuted") == "TRUE")
        m_savedCmdJSettings->bInputExecuted = true;
    else
        m_savedCmdJSettings->bInputExecuted = false;

    if(kksSettings->getParam("bMyOutput") == "TRUE")
        m_savedCmdJSettings->bMyOutput = true;
    else
        m_savedCmdJSettings->bMyOutput = false;

    if(kksSettings->getParam("bOutputArchived") == "TRUE")
        m_savedCmdJSettings->bOutputArchived = true;
    else
        m_savedCmdJSettings->bOutputArchived = false;

    if(kksSettings->getParam("bOutputExecuted") == "TRUE")
        m_savedCmdJSettings->bOutputExecuted = true;
    else
        m_savedCmdJSettings->bOutputExecuted = false;

    if(kksSettings->getParam("bUseInputDate") == "TRUE")
        m_savedCmdJSettings->bUseInputDate = true;
    else
        m_savedCmdJSettings->bUseInputDate = false;

    if(kksSettings->getParam("bUseOutputDate") == "TRUE")
        m_savedCmdJSettings->bUseOutputDate = true;
    else
        m_savedCmdJSettings->bUseOutputDate = false;

    m_savedCmdJSettings->dInputDate = QDate::fromString(kksSettings->getParam("dInputDate"), "dd.MM.yyyy");
    m_savedCmdJSettings->dOutputDate = QDate::fromString(kksSettings->getParam("dOutputDate"), "dd.MM.yyyy");
    
    m_savedCmdJSettings->queryInterval = kksSettings->getParam("queryInterval").toInt();
    m_savedCmdJSettings->beepFile = kksSettings->getParam("beepFile");

    kksSettings->endGroup();
}

void KKSSito::saveCmdJSettings(KKSCmdJSettings * settings)
{
    KKSSettings *kksSettings = kksSito->getKKSSettings();
    if(!kksSettings)
        return;

    KKSCmdJSettings * s = NULL;
    if(settings)
        s = new KKSCmdJSettings(*settings);
    else
        s = new KKSCmdJSettings();

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"), 
                                "bMyInput", 
                                s->bMyInput ? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"), 
                                "bAllInput", 
                                s->bAllInput ? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "bAllOutput", 
                                s->bAllOutput ? "TRUE" : "FALSE");
    
    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "bInputArchived", 
                                s->bInputArchived? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "bInputExecuted", 
                                s->bInputExecuted ? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "bMyOutput", 
                                s->bMyOutput? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "bOutputArchived", 
                                s->bOutputArchived? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "bOutputExecuted", 
                                s->bOutputExecuted? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "bUseInputDate", 
                                s->bUseInputDate? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "bUseOutputDate", 
                                s->bUseOutputDate? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "dInputDate", 
                                s->dInputDate.toString("dd.MM.yyyy"));
    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "dOutputDate", 
                                s->dOutputDate.toString("dd.MM.yyyy"));

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "queryInterval", 
                                QString::number(s->queryInterval));

    if(s->beepFile.isEmpty()){
        s->beepFile = workingDir + "/cmdbeep.wav";
    }
    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/CmdJournal"),
                                "beepFile", 
                                s->beepFile);

    delete s;
}

void KKSSito::loadMsgJSetings()
{
    KKSSettings *kksSettings = kksSito->getKKSSettings();
    if(!kksSettings)
        return;
    
    m_savedMsgJSettings = new KKSMsgJSettings();
    
    kksSettings->beginGroup (QString ("System settings/"+kksSitoNameEng+"/MsgJournal"));

    if ( kksSettings->getParam("bMyInput").isEmpty() )
    {
        kksSettings->endGroup();
        saveMsgJSettings(m_savedMsgJSettings);
        kksSettings->beginGroup (QString ("System settings/"+kksSitoNameEng+"/MsgJournal"));
    }
    
    if(kksSettings->getParam("bMyInput") == "TRUE")
        m_savedMsgJSettings->bMyInput = true;
    else
        m_savedMsgJSettings->bMyInput = false;

    //if(kksSettings->getParam("bAllInput") == "TRUE")
    //    m_savedCmdJSettings->bAllInput= true;
    //else
    //    m_savedCmdJSettings->bAllInput = false;

    //if(kksSettings->getParam("bAllOutput") == "TRUE")
    //    m_savedCmdJSettings->bAllOutput = true;
    //else
    //    m_savedCmdJSettings->bAllOutput = false;

    //if(kksSettings->getParam("bInputArchived") == "TRUE")
    //    m_savedCmdJSettings->bInputArchived = true;
    //else
    //    m_savedCmdJSettings->bInputArchived = false;

    //if(kksSettings->getParam("bInputExecuted") == "TRUE")
    //    m_savedCmdJSettings->bInputExecuted = true;
    //else
    //    m_savedCmdJSettings->bInputExecuted = false;

    if(kksSettings->getParam("bMyOutput") == "TRUE")
        m_savedMsgJSettings->bMyOutput = true;
    else
        m_savedMsgJSettings->bMyOutput = false;

    //if(kksSettings->getParam("bOutputArchived") == "TRUE")
    //    m_savedCmdJSettings->bOutputArchived = true;
    //else
    //    m_savedCmdJSettings->bOutputArchived = false;

    //if(kksSettings->getParam("bOutputExecuted") == "TRUE")
    //    m_savedCmdJSettings->bOutputExecuted = true;
    //else
    //    m_savedCmdJSettings->bOutputExecuted = false;

    if(kksSettings->getParam("bUseInputDate") == "TRUE")
        m_savedMsgJSettings->bUseInputDate = true;
    else
        m_savedMsgJSettings->bUseInputDate = false;

    if(kksSettings->getParam("bUseOutputDate") == "TRUE")
        m_savedMsgJSettings->bUseOutputDate = true;
    else
        m_savedMsgJSettings->bUseOutputDate = false;

    m_savedMsgJSettings->dInputDate = QDate::fromString(kksSettings->getParam("dInputDate"), "dd.MM.yyyy");
    m_savedMsgJSettings->dOutputDate = QDate::fromString(kksSettings->getParam("dOutputDate"), "dd.MM.yyyy");
    
    m_savedMsgJSettings->queryInterval = kksSettings->getParam("queryInterval").toInt();
    m_savedMsgJSettings->beepFile = kksSettings->getParam("beepFile");

    kksSettings->endGroup();
}

void KKSSito::saveMsgJSettings(KKSMsgJSettings * settings)
{
    KKSSettings *kksSettings = kksSito->getKKSSettings();
    if(!kksSettings)
        return;

    KKSMsgJSettings * s = NULL;
    if(settings)
        s = new KKSMsgJSettings(*settings);
    else
        s = new KKSMsgJSettings();

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/MsgJournal"),
                                "bMyInput", 
                                s->bMyInput ? "TRUE" : "FALSE");

    //kksSettings->writeSettings ("System settings/KKSSITO/MsgJournal", 
    //                            "bAllInput", 
    //                            s->bAllInput ? "TRUE" : "FALSE");

    //kksSettings->writeSettings ("System settings/KKSSITO/CmdJournal", 
    //                            "bAllOutput", 
    //                            s->bAllOutput ? "TRUE" : "FALSE");
    
    //kksSettings->writeSettings ("System settings/KKSSITO/CmdJournal", 
    //                            "bInputArchived", 
    //                            s->bInputArchived? "TRUE" : "FALSE");

    //kksSettings->writeSettings ("System settings/KKSSITO/CmdJournal", 
    //                            "bInputExecuted", 
    //                            s->bInputExecuted ? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString("System settings/") + kksSitoNameEng + QString("/MsgJournal"), 
                                "bMyOutput", 
                                s->bMyOutput? "TRUE" : "FALSE");

    //kksSettings->writeSettings ("System settings/KKSSITO/CmdJournal", 
    //                            "bOutputArchived", 
    //                            s->bOutputArchived? "TRUE" : "FALSE");

    //kksSettings->writeSettings ("System settings/KKSSITO/CmdJournal", 
    //                            "bOutputExecuted", 
    //                            s->bOutputExecuted? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/MsgJournal"),
                                "bUseInputDate", 
                                s->bUseInputDate? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/MsgJournal"),
                                "bUseOutputDate", 
                                s->bUseOutputDate? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/MsgJournal"),
                                "dInputDate", 
                                s->dInputDate.toString("dd.MM.yyyy"));
    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/MsgJournal"),
                                "dOutputDate", 
                                s->dOutputDate.toString("dd.MM.yyyy"));

    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/MsgJournal"),
                                "queryInterval", 
                                QString::number(s->queryInterval));

    if(s->beepFile.isEmpty()){
        s->beepFile = workingDir + "/cmdbeep.wav";
    }
    kksSettings->writeSettings (QString ("System settings/"+kksSitoNameEng+"/MsgJournal"),
                                "beepFile", 
                                s->beepFile);

    delete s;
}

const KKSList<KKSAccLevel*> & KKSSito::getAccLevels() const
{
    return m_accLevels;
}


void KKSSito::loadAccLevels()
{
    KKSSettings *kksSettings = kksSito->getKKSSettings();
    if(!kksSettings)
        return;
    
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
}

void KKSSito::saveAccLevels(const KKSList<KKSAccLevel*> & levels)
{
    KKSSettings *kksSettings = kksSito->getKKSSettings();
    if(!kksSettings)
        return;

    int cnt = levels.count();

     kksSettings->beginWriteArray("System settings/Database/AccLevels");
     for (int i=0; i<cnt; i++) {
         kksSettings->setArrayIndex(i);
         kksSettings->setValue("levelName", levels.at(i)->name());
         kksSettings->setValue("portName", levels.at(i)->port());
     }
     kksSettings->endArray();
}

void KKSSito::loadDefAccLevels()
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

KKSCatEditorFactory * KKSSito::catf () const
{
    return m_catf;
}

KKSTemplateEditorFactory * KKSSito::tf () const
{
    return m_tf;
}

void KKSSito::showConnectionInfo(QWidget * parent) const
{
    if(!m_loader || !m_loader->getDb()){
        QMessageBox::warning(parent, tr("Warning"), tr("You are not connected to database"), QMessageBox::Ok);
        return;
    }

    ConnectionInfoForm * cif = new ConnectionInfoForm(parent);
    
    cif->setUserInfo(m_loader->getUserName(), m_loader->getDlName(), m_loader->getCurrMacLabelName());
    cif->setDatabaseInfo(m_loader->getDb()->getHost(), m_loader->getDb()->getPort(), m_loader->getDb()->getName());
    cif->exec();
    delete cif;
}
