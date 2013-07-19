#include <QtCore>
#include <QtGui>

#include "choosedlform.h"


ChooseDlForm :: ChooseDlForm (QWidget * parent)
    : QDialog (parent)
{
    setupUi (this);

    connect (twDls, SIGNAL (doubleClicked (const QModelIndex & )), this, SLOT ( dblClickedSlot(const QModelIndex & )) );
    connect (pbOk, SIGNAL (clicked ()), this, SLOT (pbOKClicked ()) );
    connect (pbCancel, SIGNAL (clicked ()), this, SLOT (pbCancelClicked ()) );
}

ChooseDlForm :: ~ChooseDlForm ( )
{
}

void ChooseDlForm :: dblClickedSlot(const QModelIndex & i)
{
    Q_UNUSED(i);

    pbOKClicked();
}

void ChooseDlForm :: pbOKClicked ( void )
{
    if(currentDl() == -1){
        qWarning() << tr("You should choose any position");
        QMessageBox::warning(this, 
                             tr("position choosing"), 
                             tr("You should choose any position"), 
                             QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    accept ();
}


int ChooseDlForm :: currentDl ( void ) const
{
    int id = -1;

    QTreeWidgetItem* item = twDls->currentItem();
    if(!item)
        return id;

    id = item->data(0, Qt::UserRole).toInt();

    return id;
}

void ChooseDlForm :: addDl(int id, const QString & dlName, const QString & posName, const QString & unitName)
{
    QStringList strings;
    strings << dlName << posName << unitName;
    QTreeWidgetItem * item = new QTreeWidgetItem(twDls, strings);
    QVariant data = QVariant(id);
    item->setData(0, Qt::UserRole, data);
}

void ChooseDlForm :: pbCancelClicked()
{
   reject();
}

