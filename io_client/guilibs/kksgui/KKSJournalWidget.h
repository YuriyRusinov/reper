/* 
 * File:   KKSJournalWidget.h
 * Author: yuriyrusinov
 *
 * Created on 21 јпрель 2011 г., 16:57
 */

#ifndef _KKSJOURNALWIDGET_H
#define	_KKSJOURNALWIDGET_H

#include <QDockWidget>
#include "kksgui_config.h"

#include <KKSEIOData.h>
#include <KKSMap.h>
#include <KKSList.h>

class QMenu;
class QAction;
class QActionGroup;
class QTreeView;
class QModelIndex;
class QItemSelection;

class KKSCmdJSettings;
class KKSMsgJSettings;
class KKSFilter;
class KKSFile;
class KKSMessage;
class KKSCommand;

class _GUI_EXPORT KKSJournalWidget : public QDockWidget
{
public:
    KKSJournalWidget (KKSCmdJSettings * cmdSettings,
                      KKSMsgJSettings * msgSettings,
                      const KKSMap<int, KKSEIOData*> & cmdData,
                      const KKSMap<int, KKSEIOData*> & msgData,
                      int idDl,
                      QWidget *parent = 0,
                      Qt::WindowFlags flags = 0);
    ~KKSJournalWidget (void);

    void setCmdData (const KKSMap<int, KKSEIOData*> & cmdData);
    void setMsgData (const KKSMap<int, KKSEIOData*> & msgData);

public slots:
    void changeFloating(bool on);

    void placeLeft(bool p);
    void placeRight(bool p);
    void placeTop(bool p);
    void placeBottom(bool p);

    void isFilesEnabled (const KKSList<KKSFile *>& files);

    void viewEntity (const QModelIndex& wIndex);

private slots:
    void messageSettingsChanged (void);
    void createOutMessage (void);
    void markAsRead (void);// on_aSetRead_triggered
    void viewMessageAttachments (void);// on_aViewOutMsgResult_triggered
    void viewMessage (void);// on_aViewMsg_triggered
    void msgRefresh (void);

    void commandSettingsChanged (void);
    void createOutCommand (void);
    void editOutCommand (void);
    void deleteOutCommand (void);
    void dropPeriodCommand (void);
    void viewCommand (void);
    void viewCommandDoc (void);
    void viewCommandResults (void);
    void executeCommand (void);// on_aExecute_triggered
    void setDocumentAsResult (void);// on_aExecuteEx_triggered
    void setCommandOnExecute (void);// on_aSetAsExecuting_triggered
    void cmdRefresh (void);

    void cmdMsgSelectionChanged (const QItemSelection& selected, const QItemSelection& deselected);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *e);
    //virtual bool event(QEvent *);

signals:
    void msgSettingsChanged();
    void msgExecuteClicked(int idJournal, int idCategory);
    void msgMarkAsRead(int idJournal);
    void msgCreateOutMsg();
    void msgViewResultMsg(int idObject);
    void viewMsg(int idJournal);

    void loadMsgFiles (int idObject);
    void downloadMsgFile (KKSFile * f);

    void cmdSettingsChanged ();
    void createOutCmd ();
    void setAsArchived (int, bool);
    void editOutCmd (int);
    void dropPeriodCmd (int);
    void viewCmd (int);
    void viewCmdDoc (int);
    void viewResultCmd (int idJournal, bool bShowExecButton);
    void executeClicked (int idJournal, int idCategory);
    void executeExClicked (int idJournal, int idCategory);
    void setAsExecuting(int idJournal);

private:
    //
    // Functions
    //
    void initModel (void);

    void initMsg (void);
    void initCmd (void);
    void initSysMenu();
    void initMsgWidgetMenu (void);
    void initCmdWidgetMenu (void);

    void updateMsgMenuItems (const QModelIndex& wIndex);
    void updateCmdMenuItems (const QModelIndex& wIndex);

    int currentMsgJournalRecord (void) const;
    void slotShowMsgError(QString error) const;

    bool canViewAttachement(const KKSMessage & msg);
    bool canSetAsRead(const KKSMessage & msg);
    void place(Qt::DockWidgetArea area, bool place);

    bool canDeleteOutCmd (const KKSCommand & cmd);
    bool canEditOutCmd (const KKSCommand & cmd);
    bool canDropPeriodCmd (const KKSCommand & cmd);
    bool canExecuteCmd(const KKSCommand & cmd);
    bool canSetAsExecuting(const KKSCommand & cmd);
    bool canViewResultsCmd(const KKSCommand & cmd);
    bool canViewCmdDoc(const KKSCommand & cmd);

private:
    //
    // Variables
    //
    KKSMap<int, KKSEIOData*> m_cmdData;
    KKSMap<int, KKSEIOData*> m_msgData;

    int m_idDl;
    QTreeView * tvJournal;

    KKSCmdJSettings * m_cmdSettings;
    KKSMsgJSettings * m_msgSettings;

    QMenu * m_pSysMenu;
    QMenu * m_pWidgetMsgMenu;
    QMenu * m_pWidgetCmdMenu;

    QAction *floatingAction;

    QActionGroup *areaActions;
    QAction *leftAction;
    QAction *rightAction;
    QAction *topAction;
    QAction *bottomAction;

    //
    // Messages
    //
    QAction * aMsgRefresh;
    QAction * aMsgRefreshSeparator;
    QAction * aSettingsMsg;

    QAction * aCreateOutMsg;
    QAction * aMsgCreateSeparator;
    QAction * aViewOutMsgResult;
    QAction * aViewMsg;

    QAction * aViewOutMsgFile;
    QAction * aSetRead;

    KKSList<KKSFile *> msgFiles;

    //
    // Commands
    //
    QAction * aCmdRefresh;
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


private:
    Q_OBJECT
};

#endif	/* _KKSJOURNALWIDGET_H */

