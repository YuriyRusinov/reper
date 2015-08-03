#include <QListView>
#include <QPushButton>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QAbstractItemDelegate>
#include <QModelIndex>
#include <QGridLayout>

#include <KKSFile.h>
#include "KKSFilesForm.h"
#include "KKSItemDelegate.h"

KKSFilesForm :: KKSFilesForm (const KKSList<KKSFile *>& files, QWidget * parent, Qt::WFlags f)
    : QDialog (parent, f),
    m_fileList (files),
    lvFiles (new QListView(this)),
    filesModel (new QStandardItemModel (files.count(), 1)),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr("&Cancel"), this))
{
    lvFiles->setModel (filesModel);
    QAbstractItemDelegate *iDeleg = new KKSItemDelegate (this);
    lvFiles->setItemDelegate (iDeleg);

    this->initWidgets ();
    this->initModel ();

    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSFilesForm :: ~KKSFilesForm (void)
{
}

KKSFile * KKSFilesForm :: getFile (void) const
{
    QItemSelectionModel *selModel = lvFiles->selectionModel ();
    if (!selModel)
        return 0;

    QModelIndex wIndex = selModel->currentIndex ();
    if (!wIndex.isValid())
        return 0;

    int i= wIndex.row();
    return m_fileList[i];
}

void KKSFilesForm :: initWidgets (void)
{
    QGridLayout *gLay = new QGridLayout (this);
    gLay->addWidget (lvFiles, 0, 0, 1, 3);
    gLay->addWidget (pbOk, 1, 0, 1, 2, Qt::AlignRight | Qt::AlignVCenter);
    gLay->addWidget (pbCancel, 1, 2, 1, 1, Qt::AlignRight | Qt::AlignVCenter);
}

void KKSFilesForm :: initModel (void)
{
    for (int i=0; i<m_fileList.count(); i++)
    {
        if (!m_fileList[i])
            continue;
        QModelIndex wIndex = filesModel->index (i, 0);
        filesModel->setData (wIndex, m_fileList[i]->name(), Qt::DisplayRole);
        filesModel->setData (wIndex, m_fileList[i]->id(), Qt::UserRole);
    }
}
