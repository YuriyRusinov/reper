#ifndef CMDJOURNALFORM_H
#define CMDJOURNALFORM_H

#include <QDockWidget>
#include <kksgui_config.h>

#include <KKSEIOData.h>
#include <KKSMap.h>
#include "kkscmdjournalitem.h"

class KKSFilter;

class QMenu;
class QAction;
class QActionGroup;

class KKSCmdJSettings;
class KKSCmdJournalLv;

//namespace Ui
//{
//    class cmd_journal_form;
//}

class _GUI_EXPORT KKSCmdJournal: public QDockWidget
{
    Q_OBJECT

public:
    KKSCmdJournal(KKSCmdJSettings * m_settings,
                  const KKSMap<int, KKSEIOData*> & data, 
                  int idDl,
                  //QMenu * widgetMenu = 0,
                  QWidget *parent = 0, 
                  Qt::WFlags flags = 0);
    ~KKSCmdJournal();

    QMenu * sysMenu() const { return m_pSysMenu; }
    //void setWidgetMenu(QMenu * widgetMenu);
    QMenu * widgetMenu() const { return m_pWidgetMenu;}

public slots:
    void setData(const KKSMap<int, KKSEIOData*> & data);
    void changeFloating(bool on);

    void placeLeft(bool p);
    void placeRight(bool p);
    void placeTop(bool p);
    void placeBottom(bool p);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *e);
    virtual bool event(QEvent *);

private slots:
    void on_aSettings_triggered();
    void on_aExecute_triggered();
    void on_aExecuteEx_triggered();
    //void on_aSetAsExecuted_triggered();
    void on_aSetAsExecuting_triggered();
    
    void on_aDeleteOutCmd_triggered();
    void on_aCreateOutCmd_triggered();
    void on_aEditOutCmd_triggered();
    void on_aDropPeriodCmd_triggered();
    void on_aViewOutCmdResult_triggered();
    void on_aViewCmd_triggered();
    void on_aViewCmdDoc_triggered();

    void init();
    void initSysMenu();
    void initWidgetMenu();

    void updateMenuItems();
    
    int currentJournalRecord() const;

signals:
    void settingsChanged();
    void executeClicked(int idJournal, int idCategory);
    void executeExClicked(int idJournal, int idCategory);
    void setAsExecuted(int idJournal);
    void setAsExecuting(int idJournal);
    void setAsArchived(int idJournal, bool yes);
    void createOutCmd();
    void editOutCmd(int idJournal);
    void viewResultCmd(int idJournal, bool bShowExecButton);
    void dropPeriodCmd(int idJournal);
    void viewCmd(int idJournal);
    void viewCmdDoc(int idObject);

private:
    void place(Qt::DockWidgetArea area, bool place);

    KKSCmdJSettings setingsFromFilters();
    void filtersFromSettings(const KKSCmdJSettings & settings);

    bool canSetAsExecuting(const KKSCommand & cmd);
    bool canExecuteCmd(const KKSCommand & cmd);
    bool canDeleteOutCmd(const KKSCommand & cmd);
    bool canEditOutCmd(const KKSCommand & cmd);
    bool canViewResultsCmd(const KKSCommand & cmd);
    bool canDropPeriodCmd(const KKSCommand & cmd);
    bool canViewCmdDoc(const KKSCommand & cmd);

    KKSMap<int, KKSEIOData*> m_data;
    KKSCmdJournalLv * m_cmdJournalLv;
    int m_idDl;
    
    KKSCmdJSettings * m_settings;

    QMenu * m_pSysMenu;
    QMenu * m_pWidgetMenu;

    QAction *floatingAction;

    QActionGroup *areaActions;
    QAction *leftAction;
    QAction *rightAction;
    QAction *topAction;
    QAction *bottomAction;

    //действия с журналом команд
    QAction * aSettingsCmd;
    
    QAction * aCreateOutCmd;
    QAction * aDeleteOutCmd;
    QAction * aEditOutCmd;
    QAction * aDropPeriodCmd;
    QAction * aViewOutCmdResult;
    QAction * aViewCmd;
    QAction * aViewCmdDoc;
    
    QAction * aExecuteInCmd;
    QAction * aExecuteExInCmd;
    QAction * aSetExecuting;
    //QAction * aSetExecuted;

    //действия с журналом сообщений
    QAction * aSettingsMsg;
    QAction * aCreateOutMsg;
    //QAction * aDeleteOut;
    //QAction * aEditOutCmd;
    QAction * aReadInMsg;

};

#endif // CMDJOURNALFORM_H
