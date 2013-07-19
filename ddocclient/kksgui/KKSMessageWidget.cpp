#include <QModelIndex>
#include <QPushButton>
//#include <QToolButton>
#include <QAction>
#include <QTextEdit>
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCloseEvent>
#include <QMessageBox>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QtDebug>

#include "KKSMessageWidget.h"
#include "KKSRecWidget.h"

KKSMessageWidget :: KKSMessageWidget (KKSRecWidget *rp, KKSRecWidget *rio, QWidget *parent)
    : QDialog (parent),
    recWPos (rp),
    recWIOs (rio),
    teMessage (new QTextEdit (this)),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr("&Cancel"), this))
{
    if (!rp || !rio)
        return;

    setWindowTitle (tr ("Message parameters"));
    this->init_widgets ();
    recWIOs->actEdit->setVisible (false);

    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );

    connect (recWIOs->actAdd, SIGNAL (triggered()), this, SLOT (addAttachment()) );
    connect (recWIOs->actDel, SIGNAL (triggered()), this, SLOT (delAttachment()) );
}

KKSMessageWidget :: ~KKSMessageWidget (void)
{
}

QList<int> KKSMessageWidget :: getDlsList (void) const
{
    return getIDList (recWPos);
}

QString KKSMessageWidget :: message (void) const
{
    return this->teMessage->toPlainText ();
}

QList<int> KKSMessageWidget :: getAttachments (void) const
{
    QList<int> idIOList;
    if (!recWIOs)
        return idIOList;

    QAbstractItemModel * sourceModel = recWIOs->getSourceModel ();
    if (!sourceModel)
        return idIOList;

    for (int i=0; i<sourceModel->rowCount(); i++)
    {
        QModelIndex wIndex = sourceModel->index (i, 0);
        int idIO = sourceModel->data (wIndex).toInt ();
        if (!idIOList.contains (idIO) && idIO >= 0)
            idIOList.append (idIO);
    }
    return idIOList;//getIDList (recWIOs);
}

QList<int> KKSMessageWidget :: getIDList (KKSRecWidget *rw) const
{
    QList<int> idList;
    if (!rw)
        return idList;

//    QModelIndexList selIndexes = rw->getSourceIndexes();
    QAbstractItemModel * dlModel = rw->getSourceModel();
    if (!dlModel)
        return idList;
    
    QItemSelection sel = rw->getSourceSelection();

    int nr = sel.indexes().count();//dlModel->rowCount();
    for (int i=0; i<nr; i++)
    {
        QModelIndex wIndex = sel.indexes().at(i);//dlModel->index (i, 0);
        wIndex = wIndex.sibling(wIndex.row(), 0);
        int id = wIndex.data (Qt::UserRole).toInt();
        if (/*wIndex.data (Qt::CheckStateRole).toBool() &&*/ !idList.contains (id))
            idList.append (id);
    }

    return idList;
}

void KKSMessageWidget :: addAttachment (void)
{
    emit addDocument ();
}

void KKSMessageWidget :: delAttachment (void)
{
    if (recWIOs->getID() >= 0)
    {
        QModelIndexList wIndexes = recWIOs->getSourceIndexes ();
        if (wIndexes.isEmpty())
            return;
        int row = wIndexes[0].row ();
        recWIOs->getSourceModel()->removeRows (row, 1);
    }
}

void KKSMessageWidget :: init_widgets (void)
{
    recWPos->setParent (this);
    recWIOs->setParent (this);
    QGridLayout *messLayout = new QGridLayout ();
    this->setLayout (messLayout);

    QGroupBox *gbAddr = new QGroupBox (tr ("Addresses"), this);
    messLayout->addWidget (gbAddr, 0, 0, 1, 1);
    QGridLayout *gAddrLay = new QGridLayout ();
    gbAddr->setLayout (gAddrLay);
    gAddrLay->addWidget (recWPos);

    QGroupBox *gbMessage = new QGroupBox (tr ("Message content"), this);
    messLayout->addWidget (gbMessage, 1, 0, 1, 1);
    QGridLayout *gMessLay = new QGridLayout ();
    gbMessage->setLayout (gMessLay);
//    QLabel *lMessage = new QLabel (tr("Message"), this);
//    gMessLay->addWidget (lMessage, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    gMessLay->addWidget (teMessage, 0, 0, 1, 1);//, Qt::AlignLeft);

    QGroupBox *gbAttachments = new QGroupBox (tr ("Documents"), this);
    messLayout->addWidget (gbAttachments, 2, 0, 1, 1);
    QGridLayout *gDocLay = new QGridLayout ();
    gbAttachments->setLayout (gDocLay);
    gDocLay->addWidget (recWIOs);

    QHBoxLayout *hButtonsLay = new QHBoxLayout ();
    messLayout->addLayout (hButtonsLay, 3, 0, 1, 1);
    hButtonsLay->addStretch ();
    hButtonsLay->addWidget (pbOk);
    hButtonsLay->addWidget (pbCancel);
}

void KKSMessageWidget :: accept (void)
{
    QList<int> idDls = getDlsList();
    if (idDls.isEmpty())
    {
        qWarning() << tr ("Please select user");
        QMessageBox::warning (this, tr("Message parameters"), tr ("Please select user"), QMessageBox::Ok);
        return;
    }
    QDialog::accept ();
}

QAbstractItemModel *KKSMessageWidget :: getAttachmentsModel (void) const
{
    return recWIOs->getSourceModel ();
}
