/***********************************************************************
 * Module:  KKSJMonitor.h
 * Author:  sergey
 * Modified: 28 декабря 2008 г. 12:00:39
 * Purpose: Declaration of the class KKSJMonitor
 * Comment: Предназначен для цикличного опроса содержимого ЖВС, ЖИС. ЖВР, ЖИР.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSJMonitor_h)
#define __KKSSITOOOM_KKSJMonitor_h

#include "kksfactory_config.h"
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QLabel>

#include "KKSMap.h"
#include "KKSList.h"
#include "KKSFilter.h"
#include "KKSEIOData.h"

class KKSLoader;
class KKSDatabase;
class KKSObject;
class KKSCmdJournal;
class KKSCmdJSettings;
class KKSMsgJournal;
class KKSMsgJSettings;
class KKSObjEditorFactory;
class KKSEIOFactory;
class KKSObjEditor;
class KKSFile;
class KKSJournalWidget;

class _F_EXPORT KKSJMonitor : public QObject
{
    Q_OBJECT
public:
    KKSJMonitor(KKSCmdJSettings * settings,
                KKSDatabase * db1, 
                KKSObjEditorFactory * oef,
                KKSEIOFactory * eiof,
                int interval=60000, //1 минута
                QWidget * parent = 0);
    KKSJMonitor(KKSCmdJSettings * cmdSettings,
                KKSMsgJSettings * msgSettings,
                KKSDatabase * db1, 
                KKSObjEditorFactory * oef, 
                KKSEIOFactory * eiof,
                int interval=60000, //1 минута
                QWidget * parent = 0);
    ~KKSJMonitor();

    int interval() const;

    const KKSMap<int, KKSEIOData*> & commands() const;
    const KKSMap<int, KKSEIOData*> & messages() const;

    void setMsgBeepFile(const QString & f);
    const QString & msgBeepFile() const;

    //void showCmdJournal(QWidget * parent = NULL);
    //void showMsgJournal(QWidget * parent = NULL);

    void showJournal (QWidget *parent=0);

    //KKSCmdJournal * cmdJournal();
    //KKSMsgJournal * msgJournal();
    KKSJournalWidget * journal ();
    
    int inputCmdCount();
    int inputMsgCount();

    void initWorkModeInfo(QLabel * orgLogo, QLabel * workMode);

    void createCmd();
    void createMsg();

public slots:
    void query();
    void setCmdJFData();
    void setMsgJFData();
    void setInterval(int interval);//задается в миллисекундах
    void setSettings(KKSCmdJSettings * settings);
    void setMsgSettings(KKSMsgJSettings * settings);

private slots:
    void queryCmdJournal();
    void queryMsgJournal();
    void queryWorkModeInfo();

    void slotCmdSettingsChanged();
    void slotMsgSettingsChanged();
    void applySettings();
    void applyMsgSettings();
    void slotExecuteCmd(int idJournal, int idCategory);
    void slotExecuteExCmd(int idJournal, int idCategory);
    //void slotSetAsExecuted(int idJournal);
    void slotSetAsExecuting(int idJournal);
    void slotSetAsArchived(int idJournal, bool yes);
    void slotEditOutCmd(int idJournal);
    void slotDropPeriodCmd(int idJournal);
    void slotCreateOutCmd();
    void slotViewResult(int idJournal, bool bShowExecButton = true);
    void slotViewCmd(int idJournal);
    void slotViewCmdDoc(int idDoc);

    void slotCreateOutMsg();
    void slotViewResultMsg(int idObject);
    void slotMarkMsgAsReaded(int idJournal);
    void slotMarkCmdAsReaded(int idJournal);
    void slotViewMsg(int idJournal);

    void slotLoadFiles (int idObject);
    //void slotDownloadFile (KKSFile * f);

signals:
    void cmdJournalQueried();
    void msgJournalQueried();
    void settingsChanged(KKSCmdJSettings * settings);
    void msgSettingsChanged(KKSMsgJSettings * settings);

    void editorCreated(KKSObjEditor * editor);
    void newCommandsFound(int cmdCount);
    void newMessagesFound(int msgCount);

    void signalMsgError(QString error);
    void filesAreLoaded (const KKSList<KKSFile *>& fileList);

private:
    //KKSCmdJournal * cmdJF;
    //KKSMsgJournal * msgJF;
    KKSJournalWidget * journalW;

    KKSLoader * m_loader;
    KKSObjEditorFactory * m_oef;
    KKSEIOFactory * m_eiof;
    KKSObject * msgJ;
    KKSObject * cmdJ;
    QString m_sqlCmd;//sql-запрос для журнала распоряжений
    QString m_sqlMsg;//sql-запрос для журнала сообщений
    KKSCmdJSettings * m_cmdSettings;
    KKSMsgJSettings * m_msgSettings;
    int idDl;

    KKSMap<int, KKSEIOData *> m_commands;
    KKSMap<int, KKSEIOData *> m_messages;

    QWidget * m_parent;
    QTimer m_timer;
    QString m_msgBeepFile;

    QLabel * m_modeName;
    QLabel * m_orgLogo;

    void init();

};

#endif
