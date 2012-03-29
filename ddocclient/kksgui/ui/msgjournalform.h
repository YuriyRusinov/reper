#ifndef MSGJOURNALFORM_H
#define MSGJOURNALFORM_H

#include <QDockWidget>
#include <kksgui_config.h>

#include <KKSEIOData.h>
#include <KKSMap.h>
#include <KKSList.h>
#include "kksmsgjournalitem.h"

class KKSFilter;

class QMenu;
class QAction;
class QActionGroup;

class KKSMsgJSettings;
class KKSMsgJournalLv;
class KKSFile;

//namespace Ui
//{
//    class cmd_journal_form;
//}

class _GUI_EXPORT KKSMsgJournal: public QDockWidget
{
    Q_OBJECT

public:
    KKSMsgJournal(KKSMsgJSettings * m_settings,
                  const KKSMap<int, KKSEIOData*> & data, 
                  int idDl,
                  //QMenu * widgetMenu = 0,
                  QWidget *parent = 0, 
                  Qt::WFlags flags = 0);
    ~KKSMsgJournal();

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

    void isFilesEnabled (const KKSList<KKSFile *>& files);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *e);
    virtual bool event(QEvent *);

private slots:
    void on_aSettings_triggered();
    void on_aSetReaded_triggered();
    
    void on_aCreateOutMsg_triggered();
    void on_aViewOutMsgResult_triggered();
    void on_aViewMsg_triggered();
    void viewMsgFiles (void);

    void init();
    void initSysMenu();
    void initWidgetMenu();

    void updateMenuItems();
    
    int currentJournalRecord() const;
    void slotShowError(QString error) const;

signals:
    void settingsChanged();
    void executeClicked(int idJournal, int idCategory);
    void markAsReaded(int idJournal);
    void createOutMsg();
    void viewResultMsg(int idObject);
    void viewMsg(int idJournal);

    void loadMsgFiles (int idObject);
    void downloadFile (KKSFile * f);

private:
    //
    // Functions
    //
    void place(Qt::DockWidgetArea area, bool place);

    KKSMsgJSettings setingsFromFilters();
    void filtersFromSettings(const KKSMsgJSettings & settings);

    bool canSetAsReaded(const KKSMessage & msg);
    bool canViewAttachement(const KKSMessage & msg);
    bool canViewFiles (const KKSMessage & msg);

    void prepareFile (KKSFile *f);

private:
    //
    // Variables
    //
    KKSMap<int, KKSEIOData*> m_data;
    KKSMsgJournalLv * m_msgJournalLv;
    int m_idDl;
    
    KKSMsgJSettings * m_settings;

    QMenu * m_pSysMenu;
    QMenu * m_pWidgetMenu;

    QAction *floatingAction;

    QActionGroup *areaActions;
    QAction *leftAction;
    QAction *rightAction;
    QAction *topAction;
    QAction *bottomAction;

    QAction * aSettingsMsg;
    
    QAction * aCreateOutMsg;
    QAction * aViewOutMsgResult;
    QAction * aViewMsg;

    QAction * aViewOutMsgFile;
    QAction * aSetReaded;

    KKSList<KKSFile *> msgFiles;
};

#endif // CMDJOURNALFORM_H
