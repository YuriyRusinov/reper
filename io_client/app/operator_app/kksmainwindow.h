#ifndef KKSMainWindow_H
#define KKSMainWindow_H

#ifdef __USE_QGIS__
#include <kksqgis_config.h>
#endif


#include <QObject>
#include <QMainWindow>
#include <QtGui>

#include <KKSList.h>
#include <KKSFilter.h>

namespace Ui
{
    class KKSMainWindowDlg;
}

class ReportHandler;
class KKSJMonitor;
class KKSCmdJournal;
class KKSMsgJournal;
class KKSJournalWidget;
class KKSDbgOutputWidget;
class KKSObjEditor;
class KKSCatEditor;
class KKSDialog;
class KKSCategoryTemplateWidget;
class KKSTemplateEditor;
class KKSIncludesWidget;
class IndicatorForm;
class kkslifecycleform;
class KKSSearchTemplateForm;

class MassWidget;
//class FBFavourites;
class KKSStatusBar;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QLabel;

class KKSMainWindow : public QMainWindow
{
Q_OBJECT
    public:
        KKSMainWindow(QWidget *parent=0);
        virtual ~KKSMainWindow();


    protected:
        void closeEvent(QCloseEvent *event);

    signals: 
        void reportSubWindowActivated(QMdiSubWindow*);//for openRPT

    private slots:

        void slotSubWindowActivated(QMdiSubWindow*);

        void setActionsEnabled(bool enabled);

        void slotCreateNewObjEditor(KKSObjEditor * editor);
        void slotCreateNewObjEditor(int idObject, 
                                    int idObjExemplar, 
                                    const KKSList<const KKSFilterGroup *> & filters,
                                    const QString & extraTitle);

        void slotCreateNewObjEditor(int attrId, 
                                    const QString & value, 
                                    const QString & extraTitle);

        void addMapDockWidgets(QDockWidget *, QDockWidget *);
        void destroyMapDockWidgets(QDockWidget *, QDockWidget *);

        void slotCreateNewCatEditor (KKSCatEditor *cEditor);
        void slotCreateNewTemplateEditor (KKSTemplateEditor *tEditor);
        void slotCreateNewRubricEditor (KKSIncludesWidget *rEditor);
        void slotIndicatorEdit (IndicatorForm * iForm);

        void slotSearchTemplateEdit (QWidget * stForm);

        void updateMenus();
        void updateWindowMenu(bool bFirstTime = false);
        void setActiveSubWindow(QWidget *window);
        void saveActiveSubWindow();
        void printActiveSubWindow();
        void closeSubWindow(QWidget * t);


        void slotConnect();
        void slotDisconnect();
        void slotChangeUser();
        void slotConnInfo();
        void slotClose();

        void slotShowJournls(bool yes);
        void slotShowDbgWin(bool yes);


        void isActiveSubWindow(const KKSObjEditor * editor, bool * yes);

        void slotCreateDoc();
        void slotFindDoc();
        void slotMyDocs ();
        void slotRubricControl ();
        void slotPrintDoc();

        void slotCreateSearchQuery();
        void slotShowSearchQueries();

        void slotCreateMsg();
        void slotCreateCmd();
        void slotCreateTSDRecord();
        void slotShowTSD();
        
        void slotCreateCat();
        void slotViewCat();
        void slotViewAttrs();
        void slotEditOSS();
        void slotSysQualifiers();
        void slotCreateLC();
        void slotOpenLC();

        void slotSettings();

        void slotAbout();
        void slotCmdParamsHelp();
        void slotMess();

        void slotCreateNewReportViewer(qint64 idReport);
        void slotCreateNewReportEditor(qint64 idReport);
    
    private:
        Ui::KKSMainWindowDlg *ui;
        QMdiArea * m_mdiArea;
        QSignalMapper *m_windowMapper;

        QString m_userName;
        QString m_dlName;
        QString m_maclabelName;
        QString m_modeName;
        QByteArray m_orgSymbol;
    
    
        void init();
        void initActions();
        void initToolBars();
        void initIcons();
        void initReports();

        void initJournal (void);
        void initDebugWindow();
        void initStatusBar();
        void initConnections();
        bool connectToDb();
        bool disconnectFromDb();

        KKSStatusBar * getStatusBar();
        
        QWidget * activeKKSSubWindow();
        KKSObjEditor * activeObjEditor();
        KKSCatEditor * activeCatEditor ();
        KKSTemplateEditor *activeTemplateEditor ();
        KKSIncludesWidget *activeRubricEditor ();
        kkslifecycleform * activeLCEditor ();
        KKSSearchTemplateForm * activeSTEditor ();

        KKSJMonitor * m_jmonitor;
        //KKSMsgJournal * m_msgJ;
        KKSJournalWidget * m_journalW;
        KKSDbgOutputWidget * m_debugWidget;
        ReportHandler * m_reportHandler;


        QAction *aCloseWindow;
        QAction *aCloseAllWindows;
        QAction *aTile;
        QAction *aCascade;
        QAction *aNextWindow;
        QAction *aPreviousWindow;
        QAction *aSeparator;
        QAction *aGenerateMess;


        QAction *aSaveIO;
        MassWidget *m_masscreateW;

        QToolBar * tbSubWindows;
        QToolBar * tbActions;
        QToolBar * tbPost;
        QToolBar * tbQuery;
        QToolBar * tbAdmin;
        QToolBar * tbOrg;
        
        QLabel * lModeName;
        QLabel * lOrgLogo;
};

#endif
