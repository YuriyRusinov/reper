#ifndef DYNDOC_SYNCH_FORM_H
#define DYNDOC_SYNCH_FORM_H

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

class dyndocModel;

namespace Ui
{
    class dyndoc_synch_form;
}

class dyndoc_synch_form : public QDialog
{
    Q_OBJECT
    
public:
    explicit dyndoc_synch_form(QWidget *parent = 0);
    ~dyndoc_synch_form();

    void init();
    void setViewWidget(dyndocView* new_ViewWidget = 0);

    void setDbInf(dyndoc_mainStructs::dbInf dataBaseInf);
    void reciveData(const QList<dyndoc_mainStructs::hostModelInf> &orgsForModel);

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

private:
    Ui::dyndoc_synch_form *ui;

    dyndocView* viewWidget;
    dyndocModel* viewModel;

    dyndoc_mainStructs::dbInf db;

    dyndoc_synch_form(dyndoc_synch_form& adb);
    const dyndoc_synch_form& operator=(const dyndoc_synch_form& rhs);

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
};

#endif // DYNDOC_SYNCH_FORM_H
