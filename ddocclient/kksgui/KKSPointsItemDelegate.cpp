#include <QLineEdit>
#include <QDoubleValidator>
#include <QModelIndex>

#include "KKSPointsItemDelegate.h"

KKSPointsItemDelegate :: KKSPointsItemDelegate (QObject *parent)
    : QItemDelegate (parent)
{
}

KKSPointsItemDelegate :: ~KKSPointsItemDelegate (void)
{
}

QWidget * KKSPointsItemDelegate :: createEditor (QWidget * parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;

    QLineEdit * w = new QLineEdit (parent);
    w->setAlignment (option.displayAlignment);
    QDoubleValidator *dVal = new QDoubleValidator (w);
    w->setValidator (dVal);

    return w;
}

void KKSPointsItemDelegate :: setEditorData (QWidget * editor, const QModelIndex& index) const
{
    if (!index.isValid() || !qobject_cast<QLineEdit *>(editor))
        return;

    QLineEdit * le = qobject_cast<QLineEdit *>(editor);
    le->setText (QString::number (index.data (Qt::EditRole).toDouble()));
}

void KKSPointsItemDelegate :: setModelData (QWidget * editor, QAbstractItemModel * model, const QModelIndex& index) const
{
    if (!index.isValid() || !model || !qobject_cast<QLineEdit *>(editor))
        return;

    QLineEdit * le = qobject_cast<QLineEdit *>(editor);
    QString str = le->text ();
    model->setData (index, str, Qt::DisplayRole);
}
