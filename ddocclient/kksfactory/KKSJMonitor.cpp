/***********************************************************************
 * Module:  KKSJMonitor.cpp
 * Author:  sergey
 * Modified: 28 декабря 2008 г. 12:00:39
 * Purpose: Implementation of the class KKSJMonitor
 * Comment: Предназначен для цикличного опроса содержимого ЖВС, ЖИС. ЖВР, ЖИР.
 ***********************************************************************/

#include "KKSJMonitor.h"
#include <QtDebug>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QSound>
#include "KKSLoader.h"
#include "kksdatabase.h"
#include "KKSObject.h"
#include "KKSCategory.h"
#include "defines.h"
#include "KKSObjEditorFactory.h"
#include "KKSCatEditorFactory.h"
#include "KKSTemplateEditorFactory.h"
#include "KKSEIOFactory.h"
#include "KKSObjectExemplar.h"

#include <commandform.h>
#include <cmdjournalform.h>
#include <cmdjournalsettingsform.h>
#include <messageform.h>
#include <msgjournalform.h>
#include <msgjournalsettingsform.h>

#include <KKSJournalWidget.h>

#include <KKSObjEditor.h>
#include <KKSFile.h>

#ifndef WIN32
#include <curses.h>
#include <unistd.h>
#endif

KKSJMonitor::KKSJMonitor(KKSCmdJSettings * settings, 
                         KKSDatabase * db1, 
                         KKSObjEditorFactory * oef,
                         KKSEIOFactory * eiof,
                         int interval, 
                         QWidget * parent) 
: QObject(),
    m_loader (new KKSLoader()),
    m_oef (oef),
    m_eiof (eiof),
    m_cmdSettings (settings),
    m_parent (parent),
    m_modeName(NULL),
    m_orgLogo(NULL),
    cmdJ(NULL),
    msgJ(NULL)
{
    Q_UNUSED (interval);

    if(!db1 || !db1->connected()){
        qWarning() << "Connection to database is not established!";
        return;
    }

    m_loader->setDb(db1);
//    cmdJF = NULL;
    
//    m_cmdSettings = settings;

    init();
    //query();//!!!

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(query()));
}

KKSJMonitor::KKSJMonitor(KKSCmdJSettings * cmdSettings, 
                         KKSMsgJSettings * msgSettings, 
                         KKSDatabase * db1, 
                         KKSObjEditorFactory * oef, 
                         KKSEIOFactory * eiof,
                         int interval, 
                         QWidget * parent) 
: QObject(),
    m_loader (new KKSLoader()),
    m_oef (oef),
    m_eiof (eiof),
    m_cmdSettings (cmdSettings),
    m_msgSettings (msgSettings),
    m_parent (parent),
    m_modeName(NULL),
    m_orgLogo(NULL),
    cmdJ(NULL),
    msgJ(NULL)
{
    Q_UNUSED (interval);
//    m_loader = NULL;
//    m_oef = oef;

    if(!db1 || !db1->connected()){
        qWarning() << "Connection to database is not established!";
        return;
    }

    m_loader->setDb(db1);
//    cmdJF = NULL;
//    msgJF = NULL;
    journalW = 0;
    
//    m_cmdSettings = cmdSettings;
//    m_msgSettings = msgSettings;

    init();
    //query();//!!!

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(query()));
}

KKSJMonitor::~KKSJMonitor()
{
    if(m_loader)
        delete m_loader;

    if(cmdJ)
        cmdJ->release();

    if(msgJ)
        msgJ->release();

//    if(cmdJF)
//        delete cmdJF;

    if(m_cmdSettings)
        delete m_cmdSettings;

//    if(msgJF)
//        delete msgJF;

    if(m_msgSettings)
        delete m_msgSettings;

    if (journalW)
        delete journalW;
}

void KKSJMonitor::setInterval(int interval)
{
    m_timer.stop();
    m_timer.setInterval(interval);
    m_timer.start();
}

int KKSJMonitor::interval() const
{
    return m_timer.interval();
}

void KKSJMonitor::queryWorkModeInfo()
{
    if(!m_orgLogo || !m_modeName)
        return;
    if(!m_loader->getDb())
        return;

    QString oldMode = m_modeName->text();

    QString modeName = m_loader->getOrgModeName ();
    m_modeName->setText (modeName);
    
    QByteArray orgSymbol = m_loader->getOrgLogo ();
    //QByteArray ba = orgSymbol.toLocal8Bit();
    QPixmap logo;
    //bool ok = logo.loadFromData(orgSymbol, "XPM");
    bool ok = logo.loadFromData(orgSymbol);
    if (ok){
        logo = logo.scaled(QSize(32, 32));
        m_orgLogo->setPixmap (logo);
    }

    if(oldMode != modeName){
        /*
        if(QSound::isAvailable() && !m_cmdSettings->beepFile.isEmpty()){
            QSound snd(m_cmdSettings->beepFile);
            snd.play();
        }
        else{
        */
            #ifdef WIN32
                Beep(750, 100);
                Sleep(100);
                Beep(850, 100);
                Sleep(100);
                Beep(950, 100);
            #else
                beep();
                usleep(100*1000);
                beep();
                usleep(100*1000);
                beep();

            #endif
        /*}*/
    }
}

void KKSJMonitor::queryCmdJournal()
{
    QList<int> oldIds = m_commands.keys();
    bool needBeep = false;

    m_commands.clear();
    emit newCommandsFound(0);

    //emit cmdJournalQueried();

    if(m_sqlCmd.isEmpty()){
        emit cmdJournalQueried();
        return;
    }

    KKSDatabase * db = m_loader->getDb();
    KKSResult * res = db->execute(m_sqlCmd);
    if(!res){
        emit cmdJournalQueried();
        return;
    }

    int count = 0;
    if((count = res->getRowCount()) == 0){
        delete res;
        emit cmdJournalQueried();
        return;
    }

    int columns = res->getColumnCount();
    for(int row=0; row<count; row++){
        KKSEIOData * eio = new KKSEIOData();
        for(int column=0; column<columns; column++){
            QString code = QString(res->getColumnName(column));
            QString value = res->getCellAsString(row, column);

            //проверим на тип ИНТЕРВАЛ
            //полагаем здесь, что тип данных int4array в ЖР возможен только для типа данных интервал
            if(res->getColumnDataType(column) == KKSResult::dtInt4Array){
                KKSValue v;
                v.setValue(value, KKSAttrType::atInterval);
                value = v.value();
            }
            if(res->getColumnDataType(column) == KKSResult::dtBool){
                QString s = value.toLower();
                if(s == "t" || s == "true" || s == "1")
                    value = QObject::tr("Yes");
                else
                    value = QObject::tr("No");
            }

            if(res->getColumnDataType(column) == KKSResult::dtTimestamp){
                KKSValue v(value, KKSAttrType::atDateTime);
                QDateTime dt = v.valueVariant().toDateTime();
                value = dt.toString("dd.MM.yyyy hh:mm:ss");
            }
            
            //в качестве кода используется код атрибута, 
            //который является глобально уникальным (в таблице attributes).
            //поэтому в KKSMap запись должна добавиться всегда.
            //далее ее можно находить путем поиска по коду имеющегося атрибута
            //и таким образом заполнять таблицу, где колонками будут названия (title) атрибутов
            //подчиненной категории (таблицы), а значениями - значения из соответствующих
            //записей KKSEIOData.
            eio->addField(code, value);
        }
        //ID всегда идет первым в списке
        int id = res->getCellAsInt(row, 0);
        if(!oldIds.contains(id))
            needBeep = true;
        m_commands.insert(id, eio);
        eio->release();
    }
    
    delete res;
    
    emit newCommandsFound(inputCmdCount());

    if(needBeep){
        /*
        if(QSound::isAvailable() && !m_cmdSettings->beepFile.isEmpty()){
            QSound snd(m_cmdSettings->beepFile);
            snd.play();
        }
        else{
        */
#ifdef WIN32
            Beep(750, 500);
#else
            beep();
#endif
        /*}*/

        //emit newCommandsFound(inputCmdCount());
    }

    emit cmdJournalQueried();
}

void KKSJMonitor::queryMsgJournal()
{
    QList<int> oldIds = m_messages.keys();
    bool needBeep = false;

    m_messages.clear();
    emit newMessagesFound(0);
    //emit msgJournalQueried();

    if(m_sqlMsg.isEmpty()){
        emit msgJournalQueried();
        return;
    }

    KKSDatabase * db = m_loader->getDb();
    KKSResult * res = db->execute(m_sqlMsg);
    if(!res){
        emit msgJournalQueried();
        return;
    }

    int count = 0;
    if((count = res->getRowCount()) == 0){
        delete res;
        emit msgJournalQueried();
        return;
    }

    int columns = res->getColumnCount();
    for(int row=0; row<count; row++){
        KKSEIOData * eio = new KKSEIOData();
        for(int column=0; column<columns; column++){
            QString code = QString(res->getColumnName(column));
            QString value = res->getCellAsString(row, column);
            if(res->getColumnDataType(column) == KKSResult::dtBool){
                QString s = value.toLower();
                if(s == "t" || s == "true" || s == "1")
                    value = QObject::tr("Yes");
                else
                    value = QObject::tr("No");
            }
            if(res->getColumnDataType(column) == KKSResult::dtTimestamp){
                KKSValue v(value, KKSAttrType::atDateTime);
                QDateTime dt = v.valueVariant().toDateTime();
                value = dt.toString("dd.MM.yyyy hh:mm:ss");
            }
            
            //в качестве кода используется код атрибута, 
            //который является глобально уникальным (в таблице attributes).
            //поэтому в KKSMap запись должна добавиться всегда.
            //далее ее можно находить путем поиска по коду имеющегося атрибута
            //и таким образом заполнять таблицу, где колонками будут названия (title) атрибутов
            //подчиненной категории (таблицы), а значениями - значения из соответствующих
            //записей KKSEIOData.
            eio->addField(code, value);
        }
        //ID всегда идет первым в списке
        int id = res->getCellAsInt(row, 0);
        if(!oldIds.contains(id))
            needBeep = true;
        m_messages.insert(id, eio);
        eio->release();
    }
    
    delete res;
    
    emit newMessagesFound(inputMsgCount());

    if(needBeep){
        /*
        if(QSound::isAvailable() && !m_cmdSettings->beepFile.isEmpty()){
            QSound snd(m_cmdSettings->beepFile);
            snd.play();
        }
        else{
        */
#ifdef WIN32
            Beep(750, 500);
#else
            beep();
#endif
        /*}*/

        //emit newMessagesFound(inputMsgCount());
    }

    emit msgJournalQueried();
}

void KKSJMonitor::query()
{
    if(!m_loader){
        qWarning() << "Connection to database is not established!";
        return;
    }

#ifndef _FOR_LESSONS_SHEDULER_ //Если выполняется проект составления расписаний (для ВКА) 
                               //необходимо сократить трафик по сети и нагрузку на сервер.
                               //для этого мы убираем это соединение.
                               //(см. также класс kkssito)
    queryCmdJournal();
    queryMsgJournal();

    queryWorkModeInfo();
#endif
}

const KKSMap<int, KKSEIOData*> & KKSJMonitor::commands() const
{
    return m_commands;
}

const KKSMap<int, KKSEIOData*> & KKSJMonitor::messages() const
{
    return m_messages;
}

void KKSJMonitor::init()
{
    cmdJ = m_loader->loadIO(IO_CMDJ_ID, true);
    if(!cmdJ){
        qWarning() << "Command Journal does not exist in database!";
        return;
    }

    msgJ = m_loader->loadIO(IO_MSGJ_ID, true);
    if(!msgJ){
        qWarning() << "Message Journal does not exist in database!";
        return;
    }
    
    QString cmdTableName = cmdJ->tableName();
    if(cmdTableName.isEmpty()){
        qWarning() << "Command Journal does not exist in database!";
        return;
    }
    
    QString msgTableName = msgJ->tableName();
    if(msgTableName.isEmpty()){
        qWarning() << "Message Journal does not exist in database!";
        return;
    }

    idDl = m_loader->getDlId();

    m_sqlCmd = QString("select * from jGetCmdsEx(%1, %2, %3)")
                    .arg("NULL")
                    .arg("NULL")
                    .arg("FALSE");

    m_sqlMsg = QString("select * from jGetMsgsEx(%1, %2)")
                    .arg("NULL")
                    .arg("NULL");

    //журнал сообщений
    //m_msgFilters.clear();

    //здесть происходит применение настроек журнала и вызов запроса к БД
    KKSCmdJSettings * copy = NULL;
    if(m_cmdSettings)
        copy = new KKSCmdJSettings(*m_cmdSettings);
    else
        copy = new KKSCmdJSettings();

    setSettings(copy);


    KKSMsgJSettings * msgCopy = NULL;
    if(m_msgSettings)
        msgCopy = new KKSMsgJSettings(*m_msgSettings);
    else
        msgCopy = new KKSMsgJSettings();

    setMsgSettings(msgCopy);
/*
    cmdJF = new KKSCmdJournal(m_cmdSettings, m_commands, idDl, m_parent);
    connect(this, SIGNAL(cmdJournalQueried()), this, SLOT(setCmdJFData()));
    connect(cmdJF, SIGNAL(settingsChanged()), this, SLOT(slotCmdSettingsChanged()));
    connect(cmdJF, SIGNAL(executeClicked(int, int)), this, SLOT(slotExecuteCmd(int, int)));
    connect(cmdJF, SIGNAL(executeExClicked(int, int)), this, SLOT(slotExecuteExCmd(int, int)));
    connect(cmdJF, SIGNAL(setAsExecuting(int)), this, SLOT(slotSetAsExecuting(int)));
    connect(cmdJF, SIGNAL(dropPeriodCmd(int)), this, SLOT(slotDropPeriodCmd(int)));
    connect(cmdJF, SIGNAL(setAsArchived(int, bool)), this, SLOT(slotSetAsArchived(int, bool)));
    connect(cmdJF, SIGNAL(editOutCmd(int)), this, SLOT(slotEditOutCmd(int)));
    connect(cmdJF, SIGNAL(createOutCmd()), this, SLOT(slotCreateOutCmd()));
    connect(cmdJF, SIGNAL(viewResultCmd(int,bool)), this, SLOT(slotViewResult(int,bool)));
    connect(cmdJF, SIGNAL(viewCmd(int)), this, SLOT(slotViewCmd(int)));
    connect(cmdJF, SIGNAL(viewCmdDoc(int)), this, SLOT(slotViewCmdDoc(int)));
    //connect(cmdJF, SIGNAL(markAsReaded(int)), this, SLOT(slotMarkCmdAsReaded(int)));

    msgJF = new KKSMsgJournal(m_msgSettings, m_messages, idDl, m_parent);
    connect(this, SIGNAL(msgJournalQueried()), this, SLOT(setMsgJFData()));
    connect(msgJF, SIGNAL(settingsChanged()), this, SLOT(slotMsgSettingsChanged()));
    connect(msgJF, SIGNAL(createOutMsg()), this, SLOT(slotCreateOutMsg()));
    connect(msgJF, SIGNAL(viewMsg(int)), this, SLOT(slotViewMsg(int)));
    connect(msgJF, SIGNAL(viewResultMsg(int)), this, SLOT(slotViewResultMsg(int)));
    connect(msgJF, SIGNAL(markAsReaded(int)), this, SLOT(slotMarkMsgAsReaded(int)));

    //
    // добавлено 20.11.2010 Юрой в связи с необходимостью просматривать присоединенные файлы
    //
    connect (msgJF, SIGNAL (loadMsgFiles(int)), this, SLOT (slotLoadFiles(int)));
    connect (msgJF, SIGNAL (downloadFile(KKSFile *)), m_oef, SLOT (slotDownloadFile (KKSFile *)) );
    connect (this, SIGNAL (filesAreLoaded (const KKSList<KKSFile *>&)), msgJF, SLOT (isFilesEnabled (const KKSList<KKSFile *>&)) );
    connect(this, SIGNAL(signalMsgError(QString)), msgJF, SLOT(slotShowError(QString)));
*/
    journalW = new KKSJournalWidget (m_cmdSettings, m_msgSettings, m_commands, m_messages, idDl, m_parent);
    connect (this, SIGNAL(cmdJournalQueried()), this, SLOT(setCmdJFData()));
    connect (journalW, SIGNAL(cmdSettingsChanged()), this, SLOT(slotCmdSettingsChanged()));
    connect (journalW, SIGNAL(executeClicked(int, int)), this, SLOT(slotExecuteCmd(int, int)));
    connect (journalW, SIGNAL(executeExClicked(int, int)), this, SLOT(slotExecuteExCmd(int, int)));
    connect (journalW, SIGNAL(setAsExecuting(int)), this, SLOT(slotSetAsExecuting(int)));
    connect (journalW, SIGNAL(dropPeriodCmd(int)), this, SLOT(slotDropPeriodCmd(int)));
    connect (journalW, SIGNAL(setAsArchived(int, bool)), this, SLOT(slotSetAsArchived(int, bool)));
    connect (journalW, SIGNAL(editOutCmd(int)), this, SLOT(slotEditOutCmd(int)));
    connect (journalW, SIGNAL(createOutCmd()), this, SLOT(slotCreateOutCmd()));
    connect (journalW, SIGNAL(viewResultCmd(int,bool)), this, SLOT(slotViewResult(int,bool)));
    connect (journalW, SIGNAL(viewCmd(int)), this, SLOT(slotViewCmd(int)));
    connect (journalW, SIGNAL(viewCmdDoc(int)), this, SLOT(slotViewCmdDoc(int)));

    connect (this, SIGNAL(msgJournalQueried()), this, SLOT(setMsgJFData()));
    connect (journalW, SIGNAL(msgSettingsChanged()), this, SLOT(slotMsgSettingsChanged()));
    connect (journalW, SIGNAL(msgCreateOutMsg()), this, SLOT(slotCreateOutMsg()));
    connect (journalW, SIGNAL(viewMsg(int)), this, SLOT(slotViewMsg(int)));
    connect (journalW, SIGNAL(msgViewResultMsg(int)), this, SLOT(slotViewResultMsg(int)));
    connect (journalW, SIGNAL(msgMarkAsRead(int)), this, SLOT(slotMarkMsgAsReaded(int)));

    //
    // добавлено 20.11.2010 Юрой в связи с необходимостью просматривать присоединенные файлы
    //
    connect (journalW, SIGNAL (loadMsgFiles(int)), this, SLOT (slotLoadFiles(int)));
    connect (journalW, SIGNAL (downloadFile(KKSFile *, QWidget *)), m_oef, SLOT (slotDownloadFile (KKSFile *, QWidget *)) );
    connect (this, SIGNAL (filesAreLoaded (const KKSList<KKSFile *>&)), journalW, SLOT (isFilesEnabled (const KKSList<KKSFile *>&)) );
    connect(this, SIGNAL(signalMsgError(QString)), journalW, SLOT(slotShowMsgError(QString)));
}

void KKSJMonitor::setCmdJFData()
{
    if (journalW)
        journalW->setCmdData (m_commands);
//    if(!cmdJF)
//        return;

//    cmdJF->setData(m_commands);
}

void KKSJMonitor::setMsgJFData()
{
    if (journalW)
        journalW->setMsgData (m_messages);
//    if(!msgJF)
//        return;
//
//    msgJF->setData(m_messages);
}
/*
void KKSJMonitor::showCmdJournal(QWidget * parent)
{
    QWidget * prnt = NULL;
    if(parent == NULL)
        prnt = m_parent;

    if(!cmdJF)
        return;

    cmdJF->setParent(prnt);
    cmdJF->show();
}

void KKSJMonitor::showMsgJournal(QWidget * parent)
{
    QWidget * prnt = NULL;
    if(parent == NULL)
        prnt = m_parent;

    if(!msgJF)
        return;

    msgJF->setParent(prnt);
    msgJF->show();
}
 */

void KKSJMonitor :: showJournal (QWidget *parent)
{
    QWidget * prnt = NULL;
    if(parent == NULL)
        prnt = m_parent;
    
    if (!journalW)
        return;

    journalW->setParent (prnt);
    journalW->show ();
}
/*
KKSCmdJournal * KKSJMonitor::cmdJournal()
{
    if(!cmdJF)
        return NULL;

    return cmdJF;
}
 */
/*
KKSMsgJournal * KKSJMonitor::msgJournal()
{
    if(!msgJF)
        return NULL;

    return msgJF;
}
*/

KKSJournalWidget * KKSJMonitor :: journal ()
{
    if (!journalW)
        return 0;
    return journalW;
}

void KKSJMonitor::setSettings(KKSCmdJSettings * settings)
{
    if(m_cmdSettings)
        delete m_cmdSettings;

    if(!settings)
        m_cmdSettings = new KKSCmdJSettings();
    else
        m_cmdSettings = settings;

    //
    // тут происходит применение настроек журнала команд и выполнение запроса к БД
    //
    applySettings();
}

void KKSJMonitor::setMsgSettings(KKSMsgJSettings * settings)
{
    if(m_msgSettings)
        delete m_msgSettings;

    if(!settings)
        m_msgSettings = new KKSMsgJSettings();
    else
        m_msgSettings = settings;
    
    //тут происходит применение настроек журнала команд и выполнение запроса к БД
    applyMsgSettings();
}

void KKSJMonitor::slotCmdSettingsChanged()
{
    applySettings();
    emit settingsChanged(m_cmdSettings);
}

void KKSJMonitor::slotMsgSettingsChanged()
{
    applyMsgSettings();
    emit msgSettingsChanged(m_msgSettings);
}

void KKSJMonitor::applySettings()
{
    if(!m_cmdSettings)
        return;

    m_timer.stop();
    
    m_sqlCmd.clear();

    if(!m_cmdSettings->usedInput() && !m_cmdSettings->usedOutput()){
        queryCmdJournal();
        m_timer.setInterval(m_cmdSettings->queryInterval*60000);//минут
        return;
    }

    if(m_cmdSettings->usedInput()){
        bool bUseSubordinated = false;
        QString sInsertDate("NULL");
        
        if(m_cmdSettings->bMyInput && !m_cmdSettings->bAllInput){
            bUseSubordinated = false;
        }

        if(m_cmdSettings->bAllInput){
            bUseSubordinated = true;
        }

        if(m_cmdSettings->bUseInputDate){
            QDate dt = m_cmdSettings->dInputDate;
            sInsertDate = dt.toString("dd.MM.yyyy");
            sInsertDate = QString("to_date('%1', 'DD.MM.YYYY')::date").arg(sInsertDate);
        }
        
        m_sqlCmd += QString(" select * from jGetCmdsEx(%1, %2, %3) ")
                           .arg(sInsertDate)
                           .arg(sInsertDate)
                           .arg(m_cmdSettings->bInputExecuted ? "TRUE" : "FALSE");
    }
    
   
    queryCmdJournal();

    queryWorkModeInfo();

    m_timer.setInterval(m_cmdSettings->queryInterval*60000);//минут
    m_timer.start();
}

void KKSJMonitor::applyMsgSettings()
{
    if(!m_msgSettings)
        return;

    m_timer.stop();
    
    m_sqlMsg.clear();

    if(!m_msgSettings->usedInput() && !m_msgSettings->usedOutput()){
        queryMsgJournal();
        m_timer.setInterval(m_cmdSettings->queryInterval*60000);//минут
        return;
    }

    if(m_msgSettings->usedInput()){
        QString sInsertDate("NULL");
        
        if(m_msgSettings->bUseInputDate){
            QDate dt = m_msgSettings->dInputDate;
            sInsertDate = dt.toString("dd.MM.yyyy");
            sInsertDate = QString("to_date('%1', 'DD.MM.YYYY')::date").arg(sInsertDate);
        }
        
        m_sqlMsg += QString(" select * from jGetMsgsEx(%1, %2) ")
                           .arg(sInsertDate)
                           .arg(sInsertDate);
    }
    /*
    //если в фильтрах задействован ЖИР, то делаем UNION
    if(m_msgSettings->usedOutput()){

        QString sInsertDate("NULL");

        if(m_msgSettings->usedInput())
            m_sqlMsg += QString(" UNION ");
        
        if(m_msgSettings->bUseOutputDate){
            QDate dt = m_msgSettings->dOutputDate;
            sInsertDate = dt.toString("dd.MM.yyyy");
            sInsertDate = QString("to_date('%1', 'DD.MM.YYYY')::date").arg(sInsertDate);
        }
        
        m_sqlMsg += QString(" select * from jGetOutMsgs(%1, %2) ")
                   .arg(idDl)
                   .arg(sInsertDate);
    }
    */
    
    queryMsgJournal();

    m_timer.setInterval(m_cmdSettings->queryInterval*60000);//минут
    m_timer.start();
}

void KKSJMonitor::slotExecuteCmd(int idJournal, int idCategory)
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//cmdJF;
    
    KKSCategory * c = m_loader->loadCategory(idCategory);
    if(!c)
        return;

    KKSTemplate * t = new KKSTemplate(c->defTemplate());
    KKSObjEditor * editor = m_oef->createObjEditor(IO_IO_ID, 
                                                   -1, 
                                                   KKSList<const KKSFilterGroup*>(), 
                                                   "",
                                                   c,
                                                   false, 
                                                   QString(),
                                                   true,
                                                   Qt::NonModal,
                                                   parent);
    editor->setIdJournal(idJournal);
    editor->setAsDraft(true);

    c->release();

    c->release();
    t->release();

    emit editorCreated(editor);
}

void KKSJMonitor::slotExecuteExCmd(int idJournal, int idCategory)
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//cmdJF;
    
    KKSList<const KKSFilter*> filters;
    KKSObject * o = m_loader->loadIO(IO_IO_ID, true);
    if(!o)
        return;
    KKSCategory * c = o->category()->tableCategory();
    if(!c)
        return;

    //KKSFilter * filter = c->createFilter("id_io_category", QString::number(idCategory), KKSFilter::foEq);
    KKSFilter * filter = c->createFilter(ATTR_ID_IO_CATEGORY, QString::number(idCategory), KKSFilter::foEq);
    if(!filter)
        return;

    filters.append(filter);
    filter->release();
    KKSList<const KKSFilterGroup *> filterGroups;
    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();
    
    KKSObjEditor *objEditor = m_oef->createObjRecEditor(IO_IO_ID, 
                                                        IO_IO_ID, 
                                                        filterGroups, 
                                                        "",
                                                        o->category(),
                                                        true, false,
                                                        Qt::WindowModal,
                                                        parent, 
                                                        Qt::Dialog);
    int idObject = 0;
    if (objEditor->exec () == QDialog::Accepted){
        idObject = objEditor->getID();
        if(idObject <= 0)
            return;
    }
    else
        return;

    KKSCategory * cc = m_loader->loadCategory(idCategory);
    if(!cc)
        return;

    KKSTemplate * t = new KKSTemplate(cc->defTemplate());
    KKSObjEditor * editor = m_oef->createObjEditor(IO_IO_ID, 
                                                   idObject, 
                                                   KKSList<const KKSFilterGroup*>(), 
                                                   "",
                                                   cc,
                                                   false, 
                                                   QString(),
                                                   true,
                                                   Qt::NonModal,
                                                   parent);


    editor->setIdJournal(idJournal);
    editor->setAsDraft(true);

    cc->release();

    //cc->release();
    t->release();

    emit editorCreated(editor);
}

void KKSJMonitor::slotSetAsExecuting(int idJournal)
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//cmdJF;
    Q_UNUSED (parent);
    
    QString sql = QString("select cmdSetAsExecuting(%1)").arg(idJournal);
    KKSResult * res = m_loader->getDb()->execute(sql);
    if(!res || res->getCellAsInt(0, 0) != 1)
        return;

    m_timer.stop();
    
    queryCmdJournal();

    m_timer.start();
}

void KKSJMonitor::slotSetAsArchived(int idJournal, bool yes)
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//cmdJF;
    Q_UNUSED (parent);
    
    QString sql = QString("update command_journal set is_archived = %2 where id = %1").arg(idJournal).arg(yes? "TRUE" : "FALSE");
    int ok = m_loader->getDb()->executeCmd(sql);
    if(ok != OK_CODE)
        return;

    m_timer.stop();
    
    queryCmdJournal();

    m_timer.start();
}

void KKSJMonitor::slotDropPeriodCmd(int idJournal)
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//cmdJF;
    Q_UNUSED (parent);
    
    QString sql = QString("update command_journal set exec_period = NULL where id = %1").arg(idJournal);
    int ok = m_loader->getDb()->executeCmd(sql);
    if(ok != OK_CODE)
        return;

    m_timer.stop();
    
    queryCmdJournal();

    m_timer.start();
}

void KKSJMonitor::slotEditOutCmd(int idJournal)
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//cmdJF;
    
    KKSObjEditor * editor = m_oef->createObjEditor(IO_CMDJ_ID, 
                                                   idJournal, 
                                                   KKSList<const KKSFilterGroup*>(), 
                                                   "",
                                                   NULL,
                                                   false,
                                                   QString(),
                                                   false,
                                                   Qt::NonModal,
                                                   parent);

    emit editorCreated(editor);
}

void KKSJMonitor::createCmd()
{
    slotCreateOutCmd();
}

void KKSJMonitor::createMsg()
{
    slotCreateOutMsg();
}

void KKSJMonitor::slotCreateOutCmd()
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//(qobject_cast <QMainWindow* >(cmdJF->parentWidget()))->centralWidget ();
    
    KKSObject * commandIO = m_loader->loadIO (IO_CMDJ_ID, true);
    if (!commandIO)
        return;

    KKSObjectExemplar * commandEIO = new KKSObjectExemplar (-1, tr ("New command"), commandIO);
    if (!commandEIO)
    {
        commandIO->release ();
        return;
    }

    KKSCategory * tc0 = commandEIO->io()->category()->tableCategory();
    
    KKSCategoryAttr * ca = tc0->attribute(1);
    commandEIO->addAttrValue(QString::number(-1), ca);

    ca = tc0->attribute(12);//id_io_category
    commandEIO->addAttrValue(QString::number(-1), ca);

    ca = tc0->attribute(99);//input_number
    commandEIO->addAttrValue(QString::null, ca);

    ca = tc0->attribute(100);//output_number
    commandEIO->addAttrValue(QString::null, ca);

    ca = tc0->attribute(42);//id_dl_from
    commandEIO->addAttrValue(QString::number (m_loader->getDlId()), ca);

    ca = tc0->attribute(34);//id_dl_to
    commandEIO->addAttrValue(QString::number (-1), ca);
    
//    ca = tc0->attribute("id_dl_executor");
//    commandEIO->addAttrValue(QString::number (-1), ca);
    
    ca = tc0->attribute(41);//id_jr_state
    commandEIO->addAttrValue(QString::number (1), ca);

    ca = tc0->attribute(16);//insert_time
    commandEIO->addAttrValue(QDateTime::currentDateTime().toString(Qt::ISODate), ca);

    ca = tc0->attribute(44);//is_automated
    commandEIO->addAttrValue(QString("true"), ca);

    ca = tc0->attribute(37);//exec_datetime
    commandEIO->addAttrValue(QDateTime::currentDateTime().toString(Qt::ISODate), ca);

    ca = tc0->attribute(38);//exec_period
    commandEIO->addAttrValue(QString::null, ca);

    ca = tc0->attribute(45);//message_body
    commandEIO->addAttrValue(QString::null, ca);

    ca = tc0->attribute(40);//is_archived
    commandEIO->addAttrValue(QString("false"), ca);

    ca = tc0->attribute(165);//id_urgency_level
    commandEIO->addAttrValue(QString::number (1), ca);

    ca = tc0->attribute(167);//id_io_object_att
    commandEIO->addAttrValue(QString::null, ca);

    ca = tc0->attribute(55);//receive_datetime
    commandEIO->addAttrValue(QDateTime::currentDateTime().toString(Qt::ISODate), ca);

    ca = tc0->attribute(188);//accepted_datetime
    commandEIO->addAttrValue(QDateTime::currentDateTime().toString(Qt::ISODate), ca);


    QMap<int, QString> pos;// = m_loader->getDls();
    QMap<int, QString> urgencyLevels = m_loader->getUrgencyLevels ();
    CommandForm * cForm = new CommandForm (pos, urgencyLevels, true, parent, Qt::Dialog);
    
    cForm->setCommand (commandEIO);
    connect (cForm, 
             SIGNAL (loadReference(QString, QWidget *, int)), 
             m_oef, 
             SLOT (loadAttributeReference (QString, QWidget *, int)) );

    connect (cForm,
             SIGNAL (loadExecutors (QAbstractItemModel *)),
             m_oef,
             SLOT (loadExecReference (QAbstractItemModel *)) );

    connect (cForm,
             SIGNAL (addExecutor (QString, QAbstractItemModel *, int)),
             m_oef,
             SLOT (addExecReference (QString, QAbstractItemModel *, int)) );

    if (cForm->getCommand () && cForm->exec () == QDialog::Accepted)
    {
        KKSObjectExemplar *cEIO = cForm->getCommand ();
        QList<int> id_executors = cForm->getExecutors ();
 //       cEIO->removeAttrValue (5);//ca->id());

        for (int i=0; i<id_executors.count(); i++)
        {
            ca = tc0->attribute (35);//id_dl_executor
            if (i>0)
                cEIO->removeAttrValue (cEIO->attrValues().count()-1);
            cEIO->addAttrValue (QString::number (id_executors[i]), ca);
            int res = m_eiof->insertCommand (cEIO);
            if (res != OK_CODE)
                continue;
            queryCmdJournal();

            queryWorkModeInfo();
        }
    }

    cForm->setParent (0);
    delete cForm;

    commandEIO->release ();
    commandIO->release ();
}

void KKSJMonitor::slotViewCmd(int idJournal)
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//(qobject_cast <QMainWindow* >(cmdJF->parentWidget()))->centralWidget ();
    //KKSObject * commandIO = m_loader->loadIO (IO_CMDJ_ID);
    //if (!commandIO)
    //    return;

    KKSObjectExemplar * commandEIO = m_loader->getCommand (idJournal);
    if (!commandEIO)
    {
        //commandIO->release ();
        return;
    }

    QMap<int, QString> pos;// = m_loader->getDls();
    QMap<int, QString> urgencyLevels = m_loader->getUrgencyLevels ();
    CommandForm * cmdForm = new CommandForm (pos, urgencyLevels, false, parent, Qt::Dialog);
    connect (cmdForm, 
             SIGNAL (loadReference(QString, QWidget *, int)), 
             m_oef, 
             SLOT (loadAttributeReference (QString, QWidget *, int)) );
    connect (cmdForm,
             SIGNAL (loadExecutors (QAbstractItemModel *)),
             m_oef,
             SLOT (loadExecReference (QAbstractItemModel *)) );

    connect (cmdForm,
             SIGNAL (addExecutor (QString, QAbstractItemModel *, int)),
             m_oef,
             SLOT (addExecReference (QString, QAbstractItemModel *, int)) );
    
    cmdForm->setCommand(commandEIO);
    
    
    KKSList<KKSAttrValue *> avals = commandEIO->attrValues ();
    int idCat = commandEIO->attrValue(ATTR_ID_IO_CATEGORY)->value().value().toInt ();
    KKSCategory * cat = m_loader->loadCategory (idCat);
    cmdForm->setCategory (cat);
    if (cat)
        cat->release ();

    QString inputNumber = commandEIO->attrValue(ATTR_INPUT_NUMBER)->value().value();
    cmdForm->setInputNumber(inputNumber);

    QString outputNumber = commandEIO->attrValue(ATTR_OUTPUT_NUMBER)->value().value();
    cmdForm->setOutputNumber(outputNumber);

    QDateTime dt = QDateTime::fromString (commandEIO->attrValue (ATTR_INSERT_TIME)->value().value(), Qt::ISODate);
    cmdForm->setInsertDateTime (dt);

    int id_dl_to = commandEIO->attrValue (ATTR_ID_DL_TO)->value().value().toInt ();
    
    /*KKSCategory * cmdJCat = commandIO->category();
    if (!cmdJCat)
        return;

    cmdJCat = cmdJCat->tableCategory();
    if (!cmdJCat)
        return;
    */
    //KKSMap<int, KKSCategoryAttr*>::const_iterator pa = cmdJCat->attributes().constFind (commandEIO->attrValue ("id_dl_to")->attribute()->id());
    QMap<int, QString> refValues;
    QMap<int, QString> values = m_loader->loadAttributeValues (commandEIO->attrValue (ATTR_ID_DL_TO)->attribute(), refValues);
    QMap<int, QString>::const_iterator pv = values.constFind (id_dl_to);
    cmdForm->setController (pv.value());

    int id_dl_executor = commandEIO->attrValue (ATTR_ID_DL_EXECUTOR)->value().value().toInt ();
    //pa = cmdJCat->attributes().constFind (commandEIO->attrValue ("id_dl_executor")->attribute()->id());
    values = m_loader->loadAttributeValues (commandEIO->attrValue (ATTR_ID_DL_EXECUTOR)->attribute(), refValues);
    pv = values.constFind (id_dl_executor);
    if (pv != values.constEnd())
    {
        QMap<int, QString> ex;
        ex.insert (pv.key(), pv.value());
        cmdForm->setExecutors (ex);
        cmdForm->selectExecutor (pv.key());
    }
    //cmdForm->setExecutor (pv.value());

    QDateTime dt_exec = QDateTime::fromString (commandEIO->attrValue (ATTR_EXEC_DATETIME)->value().value(), Qt::ISODate);
    cmdForm->setDeadLineT (dt_exec);

    QDateTime dt_recv = QDateTime::fromString (commandEIO->attrValue (ATTR_RECEIVE_DATETIME)->value().value(), Qt::ISODate);
    cmdForm->setReceiveDateTime (dt_recv);

    QDateTime dt_acc = QDateTime::fromString (commandEIO->attrValue (ATTR_ACCEPTED_DATETIME)->value().value(), Qt::ISODate);
    cmdForm->setAcceptDateTime (dt_acc);

    QString period = commandEIO->attrValue (ATTR_EXEC_PERIOD)->value().value();
    if (!period.isEmpty())
    {
        int period_value = period.mid (0, period.indexOf (",")).toInt();
        QString period_unit = period.mid (period.indexOf (",")+1, period.indexOf ("}"));
        cmdForm->setPeriod (period_value, period_unit);
    }

    int id_jr_state = commandEIO->attrValue (ATTR_ID_JR_STATE)->value().value().toInt ();
    //pa = cmdJCat->attributes().constFind (commandEIO->attrValue ("id_jr_state")->attribute()->id());
    values = m_loader->loadAttributeValues (commandEIO->attrValue (ATTR_ID_JR_STATE)->attribute(), refValues);
    pv = values.constFind (id_jr_state);
    cmdForm->setStatus (pv.value());

    int id_dl_from = commandEIO->attrValue (ATTR_ID_DL_FROM)->value().value().toInt ();
    //pa = cmdJCat->attributes().constFind (commandEIO->attrValue ("id_dl_from")->attribute()->id());
    values = m_loader->loadAttributeValues (commandEIO->attrValue (ATTR_ID_DL_FROM)->attribute(), refValues);
    pv = values.constFind (id_dl_from);
    if (pv != values.constEnd())
        cmdForm->setSender (pv.value());

    int idUrgency = commandEIO->attrValue (ATTR_ID_URGENCY_LEVEL)->value().value().toInt ();
    //pa = cmdJCat->attributes().constFind (commandEIO->attrValue ("id_urgency_level")->attribute()->id());
    values = m_loader->loadAttributeValues (commandEIO->attrValue (ATTR_ID_URGENCY_LEVEL)->attribute(), refValues);
    pv = values.constFind (idUrgency);
    if (pv != values.constEnd())
        cmdForm->setUrgency (pv.key());

    int idObject = commandEIO->attrValue (ATTR_ID_IO_OBJECT_ATT)->value().value().toInt ();
    //pa = cmdJCat->attributes().constFind (commandEIO->attrValue ("id_io_object_att")->attribute()->id());
    values = m_loader->loadAttributeValues (commandEIO->attrValue (ATTR_ID_IO_OBJECT_ATT)->attribute(), refValues);
    pv = values.constFind (idObject);
    if (pv != values.constEnd())
        cmdForm->setAttachment (pv.value());

    QString messageBody = commandEIO->attrValue (ATTR_MESSAGE_BODY)->value().value();
    cmdForm->setCommandMessage (messageBody);
    cmdForm->exec();
    
    cmdForm->setParent (0);

    delete cmdForm;

    commandEIO->release ();
    //commandIO->release ();
}

void KKSJMonitor::slotViewResult(int idJournal, bool bShowExecButton)
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//cmdJF;

    QString sql = QString("select cmdGetResult(%1)").arg(idJournal);
    KKSResult * res = m_loader->getDb()->execute(sql);
    if(!res)
        return;

    if(res->getRowCount() <= 0){
        delete res;
        return;
    }

    int idObject = res->getCellAsInt(0, 0);
    delete res;

    if(idObject <= 0)
        return;

    KKSObject * o = m_loader->loadIO(idObject, true);
    if(!o)
        return;

    KKSCategory * c = o->category();
    if(!c){
        o->release();
        return;
    }

    KKSObjEditor * editor = m_oef->createObjEditor(IO_IO_ID, 
                                                   idObject, 
                                                   KKSList<const KKSFilterGroup*>(), 
                                                   "",
                                                   c,
                                                   false,
                                                   QString(),
                                                   bShowExecButton,
                                                   Qt::NonModal,
                                                   parent);

    o->release();
    editor->setIdJournal(idJournal);
    editor->setAsDraft(true);
    
    emit editorCreated(editor);
}

void KKSJMonitor::slotViewCmdDoc(int idObject)
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//cmdJF;

    KKSObject * o = m_loader->loadIO(idObject, true);
    if(!o)
        return;

    KKSCategory * c = o->category();
    if(!c){
        o->release();
        return;
    }

    KKSObjEditor * editor = m_oef->createObjEditor(IO_IO_ID, 
                                                   idObject, 
                                                   KKSList<const KKSFilterGroup*>(), 
                                                   "",
                                                   c,
                                                   false, 
                                                   QString(),
                                                   false,
                                                   Qt::NonModal,
                                                   parent);

    o->release();

    emit editorCreated(editor);
}

void KKSJMonitor::slotCreateOutMsg()
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//(qobject_cast <QMainWindow* >(msgJF->parentWidget()))->centralWidget ();

    KKSObject * messageIO = m_loader->loadIO (IO_MSGJ_ID, true);
    if (!messageIO)
        return;

    KKSObjectExemplar * messageEIO = new KKSObjectExemplar (-1, tr ("New message"), messageIO);
    if (!messageEIO)
    {
        messageIO->release ();
        return;
    }

    KKSCategory * tc0 = messageEIO->io()->category()->tableCategory();

    KKSCategoryAttr * ca = tc0->attribute(1);
    messageEIO->addAttrValue(QString::number(-1), ca);
    
    ca = tc0->attribute(16);//insert_time
    messageEIO->addAttrValue(QDateTime::currentDateTime().toString(Qt::ISODate), ca);

    ca = tc0->attribute(99);//input_number
    messageEIO->addAttrValue(QString::null, ca);

    ca = tc0->attribute(100);//output_number
    messageEIO->addAttrValue(QString::null, ca);

    ca = tc0->attribute(45);//message_body
    messageEIO->addAttrValue(QString::null, ca);

    ca = tc0->attribute(48);//id_io_object
    messageEIO->addAttrValue(QString::null, ca);

    ca = tc0->attribute(51);//id_dl_sender
    messageEIO->addAttrValue(QString::number (m_loader->getDlId()), ca);

//    ca = tc0->attribute("id_dl_receiver");
//    messageEIO->addAttrValue(QString::number(-1), ca);

    ca = tc0->attribute(54);//sent_datetime
    messageEIO->addAttrValue(QDateTime::currentDateTime().toString (Qt::ISODate), ca);

    ca = tc0->attribute(55);//receive_datetime
    messageEIO->addAttrValue(QDateTime::currentDateTime().toString (Qt::ISODate), ca);

    ca = tc0->attribute(56);//read_datetime
    messageEIO->addAttrValue(QDateTime().toString (Qt::ISODate), ca);

    ca = tc0->attribute(165);//id_urgency_level
    messageEIO->addAttrValue(QString::number(1), ca);

    QMap<int, QString> pos;// = m_loader->getDls();
    QMap<int, QString> urgencyLevels = m_loader->getUrgencyLevels ();
    MessageForm * messForm = new MessageForm (pos, urgencyLevels, true, parent, Qt::Dialog);
    messForm->setMessage (messageEIO);
    connect (messForm, 
             SIGNAL (loadReference(QString, QWidget *, int)), 
             m_oef, 
             SLOT (loadAttributeReference (QString, QWidget *, int)) );
    connect (messForm,
             SIGNAL (loadReceivers (QAbstractItemModel *)),
             m_oef,
             SLOT (loadExecReference (QAbstractItemModel *)) );
    connect (messForm,
             SIGNAL (addReceiver (QString, QAbstractItemModel *, int)),
             m_oef,
             SLOT (addExecReference (QString, QAbstractItemModel *, int)) ); 

    if (messForm->getMessage () && messForm->exec () == QDialog::Accepted)
    {
        KKSObjectExemplar *mEIO = messForm->getMessage ();
        QList<int> id_receivers = messForm->getReceivers ();

        for (int i=0; i<id_receivers.count(); i++)
        {
            ca = tc0->attribute (52);//id_dl_receiver
            if (i>0)
                mEIO->removeAttrValue (mEIO->attrValues().count()-1);
            mEIO->addAttrValue (QString::number (id_receivers[i]), ca);
            
            int res = m_eiof->insertMessage (mEIO);
            if (res< 0)
            {
                QMessageBox::warning (messForm, tr ("Send message"), tr ("Send message failed"), QMessageBox::Ok);
                continue;
            }
            queryMsgJournal();
            queryWorkModeInfo();
        }
    }

    messForm->setParent (0);
    delete messForm;

    messageEIO->release ();
    messageIO->release ();
}

void KKSJMonitor::slotViewMsg(int idJournal)
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//(qobject_cast <QMainWindow* >(msgJF->parentWidget()))->centralWidget ();
    //KKSObject * messageIO = m_loader->loadIO (IO_MSGJ_ID);
    //if (!messageIO)
    //    return;

    KKSObjectExemplar * messageEIO = m_loader->getMessage (idJournal);
    if (!messageEIO)
    {
        //messageIO->release ();
        return;
    }

    QMap<int, QString> pos;// = m_loader->getDls();
    QMap<int, QString> urgencyLevels = m_loader->getUrgencyLevels ();
    MessageForm * messForm = new MessageForm (pos, urgencyLevels, false, parent, Qt::Dialog);
    connect (messForm, 
             SIGNAL (loadReference(QString, QWidget *, int)), 
             m_oef, 
             SLOT (loadAttributeReference (QString, QWidget *, int)) );
    connect (messForm,
             SIGNAL (loadReceivers (QAbstractItemModel *)),
             m_oef,
             SLOT (loadExecReference (QAbstractItemModel *)) );
    connect (messForm,
             SIGNAL (addReceiver (QString, QAbstractItemModel *, int)),
             m_oef,
             SLOT (addExecReference (QString, QAbstractItemModel *, int)) ); 

    messForm->setMessage (messageEIO);

    //KKSList<KKSAttrValue *> avals = messageEIO->attrValues ();
    
    int idIO = messageEIO->attrValue(ATTR_ID_IO_OBJECT)->value().value().toInt ();
    int idSender = messageEIO->attrValue(ATTR_ID_DL_SENDER)->value().value().toInt ();
    int idRecv = messageEIO->attrValue(ATTR_ID_DL_RECEIVER)->value().value().toInt ();
    int idUrgency = messageEIO->attrValue (ATTR_ID_URGENCY_LEVEL)->value().value().toInt ();
    
    //KKSMap<int, KKSCategoryAttr*>::const_iterator pa = messageIO->category()->tableCategory()->attributes().constFind (messageEIO->attrValue("id_io_object")->attribute()->id());
    
    QMap<int, QString> refValues;
    QMap<int, QString> values = m_loader->loadAttributeValues (messageEIO->attrValue(ATTR_ID_IO_OBJECT)->attribute(), refValues);
    QMap<int, QString>::const_iterator pv = values.constFind (idIO);

    if (pv != values.constEnd())
        messForm->setDocumentName (pv.value());

    QDateTime insertDT = QDateTime::fromString (messageEIO->attrValue(ATTR_INSERT_TIME)->value().value(), Qt::ISODate);
    messForm->setInsertTime(insertDT);

    QDateTime readDT = QDateTime::fromString (messageEIO->attrValue(ATTR_READ_DATETIME)->value().value(), Qt::ISODate);
    messForm->setReadTime(readDT);

    QDateTime sentDT = QDateTime::fromString (messageEIO->attrValue(ATTR_SENT_DATETIME)->value().value(), Qt::ISODate);
    messForm->setSentTime(sentDT);

    QDateTime receiveDT = QDateTime::fromString (messageEIO->attrValue(ATTR_RECEIVE_DATETIME)->value().value(), Qt::ISODate);
    messForm->setReceiveTime(receiveDT);

    //pa = messageIO->category()->tableCategory()->attributes().constFind (messageEIO->attrValue("id_dl_sender")->attribute()->id());
    values = m_loader->loadAttributeValues (messageEIO->attrValue(ATTR_ID_DL_SENDER)->attribute(), refValues);
    pv = values.constFind (idSender);
    if (pv != values.constEnd())
        messForm->setSenderName (pv.value());

    QString inputNumber = messageEIO->attrValue(ATTR_INPUT_NUMBER)->value().value();
    messForm->setInputNumber(inputNumber);

    QString outputNumber = messageEIO->attrValue(ATTR_OUTPUT_NUMBER)->value().value();
    messForm->setOutputNumber(outputNumber);

    QString messBody = messageEIO->attrValue(ATTR_MESSAGE_BODY)->value().value();
    messForm->setMessageBody(messBody);

    //pa = messageIO->category()->tableCategory()->attributes().constFind (messageEIO->attrValue("id_dl_receiver")->attribute()->id());
    values = m_loader->loadAttributeValues (messageEIO->attrValue(ATTR_ID_DL_RECEIVER)->attribute(), refValues);
    pv = values.constFind (idRecv);
    if (pv != values.constEnd())
    {
        QMap<int, QString> recv;
        recv.insert (pv.key(), pv.value());
        messForm->setReceivers (recv);
        messForm->selectReceiver (pv.key());
    }

    //pa = messageIO->category()->tableCategory()->attributes().constFind (messageEIO->attrValue("id_urgency_level")->attribute()->id());
    values = m_loader->loadAttributeValues (messageEIO->attrValue(ATTR_ID_URGENCY_LEVEL)->attribute(), refValues);
    pv = values.constFind (idUrgency);
    if (pv != values.constEnd())
        messForm->setUrgency (pv.key());

    if (messForm->getMessage () && messForm->exec () == QDialog::Accepted)
    {
        KKSObjectExemplar *mEIO = messForm->getMessage ();
        int res = m_eiof->updateEIO (mEIO);
        Q_UNUSED (res);
        //
        // TODO: сделать обработку при ошибке
        //
    }

    messForm->setParent (0);
    delete messForm;

    messageEIO->release ();
    //messageIO->release ();
}

void KKSJMonitor::slotViewResultMsg(int idObject)
{
    if(!m_oef)
        return;

    QWidget * parent = 0;//msgJF;

    KKSObject * o = m_loader->loadIO(idObject, true);
    if(!o)
        return;

    KKSCategory * c = o->category();
    if(!c){
        o->release();
        return;
    }

    KKSObjEditor * editor = m_oef->createObjEditor(IO_IO_ID, 
                                                   idObject, 
                                                   KKSList<const KKSFilterGroup*>(), 
                                                   "",
                                                   c,
                                                   false, 
                                                   QString(),
                                                   false,
                                                   Qt::NonModal,
                                                   parent);

    o->release();
    //editor->setIdJournal(idJournal);

    emit editorCreated(editor);
}

void KKSJMonitor::slotMarkMsgAsReaded(int idJournal)
{
    QString sql = QString("select msgSetAsReaded(%1)").arg(idJournal);
    KKSResult * res = m_loader->getDb()->execute(sql);
    
    if(!res || res->getRowCount() == 0 || res->getCellAsInt(0, 0) <= 0){
        if(res)
            delete res;

        qWarning() << "ERROR! Cannot set message as received!";
        emit signalMsgError(QObject::tr("An error was occured while updating the message!"));
    }

    m_timer.stop();
    
    queryMsgJournal();

    m_timer.start();
}

void KKSJMonitor::slotMarkCmdAsReaded(int idJournal)
{
    QString sql = QString("select cmdSetAsReaded(%1)").arg(idJournal);
    KKSResult * res = m_loader->getDb()->execute(sql);
    
    if(!res || res->getRowCount() == 0 || res->getCellAsInt(0, 0) <= 0){
        if(res)
            delete res;

        qWarning() << "ERROR! Cannot set command as readed!";
        emit signalMsgError(QObject::tr("An error was occured while updating the command!"));
    }

    m_timer.stop();
    
    queryCmdJournal();

    m_timer.start();
}

void KKSJMonitor::setMsgBeepFile(const QString & f)
{
    m_msgBeepFile = f;
}

const QString & KKSJMonitor::msgBeepFile() const
{
    return m_msgBeepFile;
}

int KKSJMonitor::inputCmdCount()
{
    int cnt = 0;

    KKSMap<int, KKSEIOData*>::const_iterator pd;
    for (pd = m_commands.constBegin(); pd != m_commands.constEnd(); pd++)
    {
        KKSEIOData * record = pd.value();
        if(!record)
            continue;

        int idDlExecutor = record->fieldValue("id_dl_executor").toInt();
        int idDlFrom = record->fieldValue("id_dl_from").toInt();
        int jrType = record->fieldValue("cmd_type").toInt();
        int jrState = record->fieldValue("id_jr_state").toInt();
        
        KKSCommand::CmdType type;

        if(idDl == idDlExecutor)
            type = KKSCommand::ctIn;
        else if(idDl == idDlFrom)
            type = KKSCommand::ctOut;
        else if(jrType == KKSCommand::jtIn)
            type = KKSCommand::ctAllIn;
        else
            type = KKSCommand::ctAllOut;

        if(type == KKSCommand::ctIn || type == KKSCommand::ctAllIn){
            if(jrState != 4) //executed
                cnt ++;
        }
    }

    return cnt;
}

int KKSJMonitor::inputMsgCount()
{
    int cnt = 0;

    KKSMap<int, KKSEIOData*>::const_iterator pd;
    for (pd = m_messages.constBegin(); pd != m_messages.constEnd(); pd++)
    {
        KKSEIOData * record = pd.value();
        if(!record)
            continue;

        int idDlReceiver = record->fieldValue("id_dl_receiver").toInt();
        int idDlSender = record->fieldValue("id_dl_sender").toInt();
        int jrType = record->fieldValue("msg_type").toInt();
        Q_UNUSED (jrType);

        KKSMessage::MsgType type;

        if(idDl == idDlReceiver)
            type = KKSMessage::mtIn;
        else if(idDl == idDlSender)
            type = KKSMessage::mtOut;
        else
            type = KKSMessage::mtIn;

        if(type == KKSMessage::mtIn)
            cnt ++;
    }

    return cnt;
}

void KKSJMonitor::initWorkModeInfo(QLabel * orgLogo, QLabel * workMode)
{
    m_modeName = workMode;
    m_orgLogo = orgLogo;
}

void KKSJMonitor::slotLoadFiles (int idObject)
{
    if (idObject <= 0)
        return;

    KKSObject * io = m_loader->loadIO (idObject, false);//потому что потом файлы у ИО загружаются
    if (!io)
        return;

    const KKSList<KKSFile *> ioFiles = io->files ();
    for (int i=0; i<ioFiles.count(); i++)
        if (ioFiles[i])
            ioFiles[i]->addRef ();
    emit filesAreLoaded (ioFiles);
    io->release ();
}
/*
void KKSJMonitor::slotDownloadFile (KKSFile * f)
{
    m_oef->slotDownloadFile (f);
}
*/
