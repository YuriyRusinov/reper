#include "kksapplication.h"

#include <QTranslator>
#include <QTextCodec>
#include <QApplication>

#include "KKSObjEditorFactory.h"
#include "KKSCatEditorFactory.h"
#include "KKSTemplateEditorFactory.h"
#include "KKSRubricFactory.h"
#include "KKSStuffFactory.h"
#include "KKSAttributesFactory.h"

#include "cmdjournalsettingsform.h"
#include "msgjournalsettingsform.h"

#include "kkssettings.h"
#include "kksclient_name.h"
#include <kksdatabase.h>
#include "defines.h"


#ifdef __USE_QGIS__
#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#endif


__KKSAPP_EXPORT KKSApplication * KKSApplication::self = 0;

const QString kksAppNameEng = QString(EXECUTABLE_PREFIX);

/*!\brief Конструктор класса.

Необходимо отметить, что в приложении можно создать только один экземпляр данного класса. При попытке создать второй, приложение завершится аварийно.

Устанавливаются локальная кодировка и кодировка сервера баз данных по умолчанию. В качестве первой используется текущая системная (делается вызов QTextCodec::codecForLocale()). 
В качестве второй используется кодировка KOI8-R (делается вызов QTextCodec::codecForName("KOI8-R")).

Кроме того загружаются необходимые данные для класса QTranslator (русификация). По умолчанию используется файл ddocclient_ru.ts в корневом каталоге приложения.
*/
KKSApplication::KKSApplication(const QString & userName, bool msgToWindow) :
    KKSCoreApplication(userName, msgToWindow),
    m_objf (0),
    m_catf (0),
    m_tf (0),
    m_attrf (0)
{
    if ( self )
        qFatal("There should be only one KKSApplication object");

    m_rf = 0;
    m_sf = 0;

    self = this;

    m_savedCmdJSettings = NULL;
    m_savedMsgJSettings = NULL;

    loadCmdJSetings();
    loadMsgJSetings();

    loadQGISTranslator();
    loadQGISPlugins(); //only if qgis linked

    initGUIFactories();
    
}

void KKSApplication::loadQGISPlugins()
{
    
#ifdef __USE_QGIS__
    KKSSettings *kksSettings = kksCoreApp->getKKSSettings();
    if(!kksSettings)
        return;
    
    QString plugins_path = getWDir() + "/plugins";
    
    kksSettings->beginGroup (QString("System settings/") + kksAppNameEng);

    if ( kksSettings->getParam("QGIS_plugins_path").isEmpty() )
    {
        kksSettings->endGroup();
        kksSettings->writeSettings (QString("System settings/") + kksAppNameEng, 
                                     "QGIS_plugins_path", 
                                     plugins_path);
        kksSettings->beginGroup (QString("System settings/") + kksAppNameEng);
    }
    
    plugins_path = kksSettings->getParam("QGIS_plugins_path");

    kksSettings->endGroup();
    
    qDebug() << "Try to load QGIS plugins from " << plugins_path;
    
    QgsApplication * a = static_cast<QgsApplication *>(QApplication::instance());
    if(!a)
        qFatal(tr("QgsApplication object does not exist!").toLocal8Bit().constData());

    a->setPluginPath(plugins_path);
    a->initQgis();

#endif
    
}

const QString & KKSApplication::GISHomeDir() const
{
    if(!m_GISHomeDir.isEmpty())
        return m_GISHomeDir;

    KKSSettings *kksSettings = kksCoreApp->getKKSSettings();
    if(!kksSettings)
        return m_GISHomeDir;
    
    QString homeDir = getWDir() + "/gis_home";
    
    kksSettings->beginGroup (QString("System settings/") + kksAppNameEng);

    if ( kksSettings->getParam("QGIS_files_home_path").isEmpty() )
    {
        kksSettings->endGroup();
        kksSettings->writeSettings (QString("System settings/") + kksAppNameEng, 
                                     "QGIS_files_home_path", 
                                     homeDir);
        kksSettings->beginGroup (QString("System settings/") + kksAppNameEng);
    }
    
    homeDir = kksSettings->getParam("QGIS_files_home_path");

    kksSettings->endGroup();
    
    qDebug() << "Set QGIS temporary files home dir to " << homeDir;

    m_GISHomeDir = homeDir;

    return m_GISHomeDir;
    
}

void KKSApplication::loadQGISTranslator()
{
#ifdef __USE_QGIS__
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
    
    qDebug() << "Try to load QGIS translator files from " << transl_path;

    QTranslator * qgistor = new QTranslator(0);
    bool ok = qgistor->load(QString("qgis_ru"), transl_path);

    QgsApplication * a = static_cast<QgsApplication *>(QApplication::instance());
    if(!a)
        qFatal(tr("QgsApplication object does not exist!").toLocal8Bit().constData());

    a->installTranslator(qgistor);
#endif

}


/*!\brief Деструктор класса.

Удаляется созданный экземпляр класса GISdb.*/
KKSApplication::~KKSApplication( )
{

    if(m_objf)
        delete m_objf;

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


KKSApplication * KKSApplication::init (int argc, 
                                       char *argv[], 
                                       bool with_connect, 
                                       const QString & userName, 
                                       bool msgToWindow)
{
    if ( self )
        qFatal(tr("There should be only one KKSApplication object").toLocal8Bit().constData());

    QApplication * app (0);
    if (QCoreApplication::instance() )
        app = qobject_cast<QApplication *>(QCoreApplication::instance() );

#ifdef __USE_QGIS__
    if (!app)
        app = new QgsApplication(argc, argv, true);
    //app->installTranslator(qgistor);

    //QgsApplication * a = static_cast <QgsApplication *> (app);
    // ---- invoked in KKSApplication::loadQGISPlugins ----
    //a->setPluginPath(pluginPath);
    //a->initQgis();
#else
    if (!app)
        app = new QApplication(argc, argv);
#endif

    KKSApplication * xG0;
    bool bDel = false;
    if ( !self ){
        xG0 = new KKSApplication (userName, msgToWindow);
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

void KKSApplication::initGUIFactories()
{
    if (!m_attrf){
        m_attrf = new KKSAttributesFactory (loader(), eiof(), NULL, ppf());
        m_attrf->setGISHomeDir(GISHomeDir());
    }


    if (!m_objf){
        m_objf = new KKSObjEditorFactory(ppf(), loader(), fileLoader(), m_attrf, eiof());
        m_objf->setPluginLoader(pluginLoader());
    }

    m_attrf->setOEF(m_objf);

    if (!m_sf)
        m_sf = new KKSStuffFactory (loader(), ppf(), m_objf, db());

    if (!m_rf)
        m_rf = new KKSRubricFactory (loader(), ppf(), eiof(), m_objf, m_sf);

    if (m_objf && m_rf)
        m_objf->setRubrFactory (m_rf);

    if (!m_catf)
        m_catf = new KKSCatEditorFactory (loader(), m_objf, ppf(), eiof(), m_attrf, m_sf);

    if (!m_tf)
        m_tf = new KKSTemplateEditorFactory (loader(), ppf());

    if (m_catf && m_tf)
    {
        m_catf->setTemplateEditorFactory (m_tf);
        m_tf->setCatEditorFactory (m_catf);
    }

    if (m_sf && m_objf)
        m_objf->setStuffFactory (m_sf);
}

KKSObjEditorFactory *KKSApplication::oef() const
{
    return m_objf;
}

KKSRubricFactory * KKSApplication::rf () const
{
    return m_rf;
}

KKSStuffFactory *KKSApplication::sf () const
{
    return m_sf;
}

KKSAttributesFactory * KKSApplication::attrf () const
{
    return m_attrf;
}

KKSCatEditorFactory * KKSApplication::catf () const
{
    return m_catf;
}

KKSTemplateEditorFactory * KKSApplication::tf () const
{
    return m_tf;
}

KKSCmdJSettings * KKSApplication::getCmdJSettings()
{
    return m_savedCmdJSettings;
}

KKSMsgJSettings * KKSApplication::getMsgJSettings()
{
    return m_savedMsgJSettings;
}

void KKSApplication::loadCmdJSetings()
{

    KKSSettings *kksSettings = kksApp->getKKSSettings();
    if(!kksSettings)
        return;
    
    m_savedCmdJSettings = new KKSCmdJSettings();
    
    kksSettings->beginGroup (QString ("System settings/"+kksAppNameEng+"/CmdJournal"));

    if ( kksSettings->getParam("bMyInput").isEmpty() )
    {
        kksSettings->endGroup();
        saveCmdJSettings(m_savedCmdJSettings);
        kksSettings->beginGroup (QString ("System settings/"+kksAppNameEng+"/CmdJournal"));
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

void KKSApplication::saveCmdJSettings(KKSCmdJSettings * settings)
{
    KKSSettings *kksSettings = kksApp->getKKSSettings();
    if(!kksSettings)
        return;

    KKSCmdJSettings * s = NULL;
    if(settings)
        s = new KKSCmdJSettings(*settings);
    else
        s = new KKSCmdJSettings();

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"), 
                                "bMyInput", 
                                s->bMyInput ? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"), 
                                "bAllInput", 
                                s->bAllInput ? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "bAllOutput", 
                                s->bAllOutput ? "TRUE" : "FALSE");
    
    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "bInputArchived", 
                                s->bInputArchived? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "bInputExecuted", 
                                s->bInputExecuted ? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "bMyOutput", 
                                s->bMyOutput? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "bOutputArchived", 
                                s->bOutputArchived? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "bOutputExecuted", 
                                s->bOutputExecuted? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "bUseInputDate", 
                                s->bUseInputDate? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "bUseOutputDate", 
                                s->bUseOutputDate? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "dInputDate", 
                                s->dInputDate.toString("dd.MM.yyyy"));
    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "dOutputDate", 
                                s->dOutputDate.toString("dd.MM.yyyy"));

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "queryInterval", 
                                QString::number(s->queryInterval));

    if(s->beepFile.isEmpty()){
        s->beepFile = getWDir();
        s->beepFile += "/cmdbeep.wav";
    }
    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/CmdJournal"),
                                "beepFile", 
                                s->beepFile);

    delete s;
}

void KKSApplication::loadMsgJSetings()
{
    KKSSettings *kksSettings = kksApp->getKKSSettings();
    if(!kksSettings)
        return;
    
    m_savedMsgJSettings = new KKSMsgJSettings();
    
    kksSettings->beginGroup (QString ("System settings/"+kksAppNameEng+"/MsgJournal"));

    if ( kksSettings->getParam("bMyInput").isEmpty() )
    {
        kksSettings->endGroup();
        saveMsgJSettings(m_savedMsgJSettings);
        kksSettings->beginGroup (QString ("System settings/"+kksAppNameEng+"/MsgJournal"));
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

void KKSApplication::saveMsgJSettings(KKSMsgJSettings * settings)
{
    KKSSettings *kksSettings = kksApp->getKKSSettings();
    if(!kksSettings)
        return;

    KKSMsgJSettings * s = NULL;
    if(settings)
        s = new KKSMsgJSettings(*settings);
    else
        s = new KKSMsgJSettings();

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/MsgJournal"),
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

    kksSettings->writeSettings (QString("System settings/") + kksAppNameEng + QString("/MsgJournal"), 
                                "bMyOutput", 
                                s->bMyOutput? "TRUE" : "FALSE");

    //kksSettings->writeSettings ("System settings/KKSSITO/CmdJournal", 
    //                            "bOutputArchived", 
    //                            s->bOutputArchived? "TRUE" : "FALSE");

    //kksSettings->writeSettings ("System settings/KKSSITO/CmdJournal", 
    //                            "bOutputExecuted", 
    //                            s->bOutputExecuted? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/MsgJournal"),
                                "bUseInputDate", 
                                s->bUseInputDate? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/MsgJournal"),
                                "bUseOutputDate", 
                                s->bUseOutputDate? "TRUE" : "FALSE");

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/MsgJournal"),
                                "dInputDate", 
                                s->dInputDate.toString("dd.MM.yyyy"));
    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/MsgJournal"),
                                "dOutputDate", 
                                s->dOutputDate.toString("dd.MM.yyyy"));

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/MsgJournal"),
                                "queryInterval", 
                                QString::number(s->queryInterval));

    if(s->beepFile.isEmpty()){
        s->beepFile = getWDir();
        s->beepFile += "/cmdbeep.wav";
    }

    kksSettings->writeSettings (QString ("System settings/"+kksAppNameEng+"/MsgJournal"),
                                "beepFile", 
                                s->beepFile);

    delete s;
}
