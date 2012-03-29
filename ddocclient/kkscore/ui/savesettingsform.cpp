#include <QtCore>
#include <QtGui>

#include "savesettingsform.h"
#include "editsettingsform.h"
#include <kkssito.h>
#include <KKSItemDelegate.h>

SaveSettingsForm :: SaveSettingsForm (QSettings * set/*=NULL*/, QWidget * parent/*=NULL*/)
    : QDialog (parent),
      settings (set),
      FileName (QString())
{
    setupUi (this);
    if (settings)
        this->InitForm ();
    this->treeView->setSelectionBehavior (QAbstractItemView::SelectItems);
    this->treeView->setSelectionMode (QAbstractItemView::SingleSelection);
    KKSItemDelegate * itemDeleg = new KKSItemDelegate (this);
    this->treeView->setItemDelegate (itemDeleg);
    this->changedIndexes.clear ();

    connect (treeView, SIGNAL (doubleClicked (const QModelIndex & )), this, SLOT ( editSlot (const QModelIndex & )) );
    connect (okButton, SIGNAL (clicked ()), this, SLOT (pbOkClicked ()) );
}

SaveSettingsForm :: ~SaveSettingsForm ( void )
{
}

void SaveSettingsForm :: pbOkClicked ( void )
{
    for (int i=0; i<changedIndexes.count(); i++)
    {
        QModelIndex ind = changedIndexes [i];
        QModelIndex keyIndex = ind.sibling (ind.row(), ind.column()-1);
        QString key = keyIndex.data().toString();
        QString val = ind.data().toString ();
        QModelIndex indPar = keyIndex.parent ();
        for ( ;indPar.isValid(); indPar=indPar.parent())
            key.prepend (QString("%1/").arg(indPar.data().toString()));
        //qDebug () << __PRETTY_FUNCTION__ << key << val;
        settings->setValue (key, val);
    }
    this->accept ();
}

void SaveSettingsForm :: editSlot (const QModelIndex & index)
{
    if (index.column() == 1)
    {
        QString data = index.data().toString().trimmed();
        EditSettingsForm * wForm = new EditSettingsForm (data);
        if (wForm->exec () == QDialog::Accepted)
        {
            QVariant val = wForm->getText ();
            QAbstractItemModel * mod = const_cast<QAbstractItemModel*>(index.model ());
            mod->setData (index, val);
            changedIndexes.append (index);
        }
        delete wForm;
    }
}

void SaveSettingsForm :: InitForm ( void )
{
    QStringList header;
    header << QString (tr("Key")) << QString (tr("Value")) << QString (tr("Type"));

    QStandardItemModel * tModel = new QStandardItemModel (0, 3);
    qDebug () << __PRETTY_FUNCTION__ << settings->childGroups().count ();
    this->setModelData (tModel);
    for (int i=0; i<header.count(); i++)
        tModel->setHeaderData (i, Qt::Horizontal, header[i], Qt::DisplayRole);
    this->treeView->setModel (tModel);
    for (int column = 0; column < tModel->columnCount (QModelIndex()); ++column)
        this->treeView->resizeColumnToContents (column);
}

void SaveSettingsForm :: setModelData (QAbstractItemModel *tModel, const QModelIndex& pIndex)
{
    if (!tModel)
        return;
    tModel->removeRows (0, tModel->rowCount (pIndex), pIndex);
    if (tModel->columnCount (pIndex) < 3)
        tModel->insertColumns (0, 3-tModel->columnCount (pIndex), pIndex);
    QStringList groups = settings->childGroups();
    QStringList keys = settings->childKeys();
    int nKeys = keys.count();
    tModel->insertRows (0, nKeys, pIndex);

    for (int i=0; i<nKeys; i++)
    {
        QModelIndex kIndex = tModel->index (i, 0, pIndex);
        tModel->setData (kIndex, keys[i], Qt::DisplayRole);
        QModelIndex kvIndex = tModel->index (i, 1, pIndex);
        tModel->setData (kvIndex, settings->value (keys[i]), Qt::DisplayRole);
    }
    tModel->insertRows (nKeys, groups.count(), pIndex);
    for (int i=0; i<groups.count(); i++)
    {
        QModelIndex gIndex = tModel->index (nKeys+i, 0, pIndex);
        qDebug () << __PRETTY_FUNCTION__ << gIndex;
        tModel->setData (gIndex, groups[i], Qt::DisplayRole);
        settings->beginGroup (groups[i]);
        this->setModelData (tModel, gIndex);
        settings->endGroup ();
    }
}
