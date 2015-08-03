#include <QTreeView>
#include <QModelIndex>
#include <QAbstractItemDelegate>
#include <QAbstractItemModel>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QtDebug>

#include "KKSEventFilter.h"
#include "KKSOrganizationAddrForm.h"

KKSOrganizationAddrForm :: KKSOrganizationAddrForm (int idOrg, 
                                                    QString orgName, 
                                                    const QMap<int, QString>& transporters, 
                                                    const QMap<int, QString>& aList, 
                                                    const QMap<int, bool>& actList, 
                                                    const QMap<int, int>& pList, 
                                                    const QMap<int, bool>& gtwList, 
                                                    QWidget * parent, 
                                                    Qt::WindowFlags flags)
        : QDialog (parent, flags),
        idOrganization (idOrg),
        organizationName (orgName),
        transp (transporters),
        addrs (aList),
        activeList (actList),
        portList (pList),
        useGatewayList(gtwList),
        tvTransporters (new QTreeView(this)),
        pbOk (new QPushButton (tr("&OK"), this)),
        pbCancel (new QPushButton (tr("&Cancel"), this))
{
    initWidget ();

    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSOrganizationAddrForm :: ~KKSOrganizationAddrForm (void)
{
}

const QMap<int, QString>& KKSOrganizationAddrForm :: getAddrs (void) const
{
    return addrs;
}

const QMap<int, bool>& KKSOrganizationAddrForm :: getActiveList (void) const
{
    return activeList;
}

const QMap<int, int>& KKSOrganizationAddrForm :: getPortList (void) const
{
    return portList;
}

const QMap<int, bool>& KKSOrganizationAddrForm :: getUseGatewayList (void) const
{
    return useGatewayList;
}

void KKSOrganizationAddrForm :: setItemDelegate (QAbstractItemDelegate * deleg)
{
    QAbstractItemDelegate * oldDeleg = tvTransporters->itemDelegate();
    tvTransporters->setItemDelegate (deleg);
    if (oldDeleg && oldDeleg != deleg)
        delete oldDeleg;
}

void KKSOrganizationAddrForm :: setModel (QAbstractItemModel * mod)
{
    QAbstractItemModel * oldModel = tvTransporters->model ();
    tvTransporters->setModel (mod);
    if (oldModel && oldModel != mod)
        delete oldModel;
    if (mod)
        connect (mod,
                 SIGNAL (dataChanged(const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (addrChanged(const QModelIndex&, const QModelIndex&))
                );
}

void KKSOrganizationAddrForm :: addrChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    qDebug () << __PRETTY_FUNCTION__ << topLeft << bottomRight;
    if (topLeft != bottomRight)
        return;
    QModelIndex wIndex (topLeft);
    int idTr = wIndex.sibling(wIndex.row(), 0).data (Qt::UserRole).toInt();
    if ((wIndex.column() == 0 || (wIndex.column() == 2)) && wIndex.sibling(wIndex.row(), 0).data(Qt::CheckStateRole).toInt() == Qt::Checked)
    {
        QString addrVal = wIndex.sibling(wIndex.row(), 2).data (Qt::DisplayRole).toString();
        qDebug () << __PRETTY_FUNCTION__ << addrVal;
        addrs.insert (idTr, addrVal);
    }
    else if (wIndex.column() == 0 && wIndex.sibling(wIndex.row(), 0).data(Qt::CheckStateRole).toInt() == Qt::Unchecked)
    {
        addrs.insert (idTr, QString());
    }
    else if (wIndex.column () == 3)
    {
        int port = wIndex.data (Qt::DisplayRole).toInt();
        portList.insert (idTr, port);
    }
    else if (wIndex.column () == 4)
    {
        bool isActive = (wIndex.data (Qt::CheckStateRole).toInt() == Qt::Checked);
        activeList.insert (idTr, isActive);
    }
    else if (wIndex.column () == 5)
    {
        bool useGateway = (wIndex.data (Qt::CheckStateRole).toInt() == Qt::Checked);
        useGatewayList.insert (idTr, useGateway);
    }
}

void KKSOrganizationAddrForm :: initWidget (void)
{
    setWindowTitle (organizationName);
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget(tvTransporters, 0, 0, 1, 1);
    tvTransporters->setSelectionBehavior (QAbstractItemView::SelectItems);
    tvTransporters->header()->setStretchLastSection (true);
    KKSEventFilter * ef = new KKSEventFilter (this);
    tvTransporters->installEventFilter (ef);
    QHBoxLayout * hButtonsLay = new QHBoxLayout;
    hButtonsLay->addStretch ();
    hButtonsLay->addWidget (pbOk);
    hButtonsLay->addWidget (pbCancel);
    gLay->addLayout (hButtonsLay, 1, 0, 1, 1);
}
