#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QLineEdit>
#include <QtDebug>

#include <KKSAttrValue.h>
#include <defines.h>
#include "KKSSyncWidget.h"
#include "KKSSyncDialog.h"

KKSSyncDialog :: KKSSyncDialog (QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    syncW (new KKSSyncWidget (this, true)),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr("&Cancel"), this)),
    m_sync (false)
{
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (syncW, 0, 0, 1, 1);

    QHBoxLayout * hButtonsLay = new QHBoxLayout ();
    hButtonsLay->addStretch ();
    hButtonsLay->addWidget (pbOk);
    hButtonsLay->addWidget (pbCancel);
    gLay->addLayout (hButtonsLay, 1, 0, 1, 1);

    QAbstractItemModel * syncOrgModel = new QStandardItemModel (0, 3);
    syncOrgModel->setHeaderData (0, Qt::Horizontal, tr ("ID"), Qt::DisplayRole);
    syncOrgModel->setHeaderData (1, Qt::Horizontal, tr ("Short name"), Qt::DisplayRole);
    syncOrgModel->setHeaderData (2, Qt::Horizontal, tr ("Name"), Qt::DisplayRole);
    syncW->setSyncOrgModel (syncOrgModel);

    connect (syncW, SIGNAL (setIsGlobal(bool)), this, SLOT (setIOGlobal (bool)) );
    connect (syncW, SIGNAL (setSyncType (QLineEdit *)), this, SLOT (setSyncType (QLineEdit *)) );
    connect (syncW, SIGNAL (addSyncOrganization (KKSAttrValue *, QAbstractItemModel *)), this, SLOT (addSyncOrg (KKSAttrValue *, QAbstractItemModel *)) );
    connect (syncW, SIGNAL (delSyncOrganization (KKSAttrValue *, const QModelIndex&, QAbstractItemModel *)), this, SLOT (delSyncOrg (KKSAttrValue *, const QModelIndex&, QAbstractItemModel *)) );

    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSSyncDialog :: ~KKSSyncDialog (void)
{
}

KKSSyncWidget * KKSSyncDialog :: getSyncWidget (void) const
{
    return syncW;
}

void KKSSyncDialog :: setIOGlobal (bool isSync)
{
    m_sync = isSync;
}

void KKSSyncDialog :: setSyncType (QLineEdit * leType)
{
    QString tableName = QString("io_sync_types");
    QWidget * aSyncW = leType;
    qint64 attrId = ATTR_ID_SYNC_TYPE;
    emit loadSyncRef (tableName, aSyncW, attrId);
}

void KKSSyncDialog :: addSyncOrg (KKSAttrValue * av, QAbstractItemModel * sMod)
{
    if (!av || !sMod)
        return;
    emit loadAddSyncOrg (av, sMod);
}

void KKSSyncDialog :: delSyncOrg (KKSAttrValue * av, const QModelIndex& oInd, QAbstractItemModel * sMod)
{
    if (!av || !sMod || !oInd.isValid())
        return;
    emit loadDelSyncOrg (av, oInd, sMod);

}
