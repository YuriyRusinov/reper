#ifndef DYNDOC_SYNC_FORM_H
#define DYNDOC_SYNC_FORM_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QtNetwork>

#include "kksdatabase.h"

#include "Namespaces/netStruct.h"
#include "UserInterface/DataElements/dyndocView.h"
#include "UserInterface/DataElements/dyndocModel.h"
#include "UserInterface/DataElements/viewElement.h"
#include "UserInterface/DataElements/dyndoc_delegate.h"
#include "optionsdialog.h"

#include <ddocinteractorwindow.h>
#include <ddocinteractorbase.h>
#include <timerform.h>

#include <syncqueueviewerform.h>

class dyndocModel;

namespace Ui
{
    class dyndoc_sync_form;
}

class DyndocSyncForm : public QDialog
{
    Q_OBJECT
    
public:
    explicit DyndocSyncForm(KKSDatabase* adb, QWidget *parent = 0);
    ~DyndocSyncForm();

    void init();
    void setViewWidget(dyndocView* new_ViewWidget = 0);

    void setLocalId(int id);

    void setDbInf(dyndoc_mainStructs::dbInf dataBaseInf);
    void reciveData(const QList<dyndoc_mainStructs::hostModelInf> &orgsForModel);

    void setConnectionSettings();
    void loadSettings();

signals:
    void signalStartSyncronization();
    void signalStopSyncronization();

private slots:
    void slot_startSyncronizationClicked();
    void slot_stopSyncronizationClicked();

    void slot_pollClicked();
    void slot_implementInitialSyncronizationClicked();

    void slot_displayLogClicked();
    void slot_parametersClicked();

    void slot_optionsAccepted();
    void slot_optionsRejected();

    void slot_pings(QMap<QString,JKKSPing> rhs);

    void slot_closeLog();

private:
    Ui::dyndoc_sync_form *ui;

    dyndocView* viewWidget;
    dyndocModel* viewModel;

    OptionsDialog* optionsForm;

    int localOrganizationId;

    dyndoc_mainStructs::dbInf db;

    DDocInteractorWindow* httpWin;
    DDocInteractorBase* m_base;

    KKSDatabase* dbLog;
    SyncQueueViewerForm * log_form;

    DyndocSyncForm(DyndocSyncForm& adb);
    const DyndocSyncForm& operator=(const DyndocSyncForm& rhs);

    inline void initData();
    inline void initSlots() const;
    inline void initInterface();

    void newViewWidgetIsTrue(dyndocView* new_viewWidget);
    void newViewWidgetIsFalse();

    bool verificationExistingFrameLayout();
    void deleteOldViewWidget();
    void createFrameLayout();
    void addViewWidgetToFrameLayout(dyndocView *new_viewWidget);

    int addChilds(QStandardItem& rhs, const QString& text, const QList<dyndoc_mainStructs::hostModelInf> &dataTable);

    int addColumns(QList<QStandardItem*>& rhs, int query_index, const QList<dyndoc_mainStructs::hostModelInf> &dataTable);
    inline int addColumnName(QList<QStandardItem*>& rhs, int query_index, const QList<dyndoc_mainStructs::hostModelInf> &dataTable);
    inline int addColumnProtocol(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable);
    inline int addColumnAdress(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable);
    inline int addColumnPort(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable);
    inline int addColumnSynch(QList<QStandardItem*>& rhs);
    inline int addColumnDb(QList<QStandardItem*>& rhs);
    inline int addColumnInit(QList<QStandardItem*>& rhs);
    inline int addColumnTrans(QList<QStandardItem*>& rhs);

    void setColor(QString key,int color);
    QStandardItem* findItem(QStandardItem* it, QString key, bool &findFlag);
};

#endif // DYNDOC_SYNC_FORM_H
