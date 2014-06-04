#include <QLineEdit>
#include "KKSOrgItemDelegate.h"

KKSOrgItemDelegate :: KKSOrgItemDelegate (QObject * parent)
                    : QItemDelegate (parent)
{
}

KKSOrgItemDelegate :: ~KKSOrgItemDelegate (void)
{
}

QWidget * KKSOrgItemDelegate :: createEditor (QWidget * parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.column() == 1)
        return 0;
    else if (index.column()==0 || index.column()==3)
        return QItemDelegate::createEditor (parent, option, index);
    else
    {
        QLineEdit * le = new QLineEdit (parent);
        le->repaint (option.rect);
        return le;
    }
}

void KKSOrgItemDelegate :: setEditorData (QWidget * editor, const QModelIndex& index) const
{
    if (qobject_cast<QLineEdit*>(editor))
    {
        QLineEdit * le = qobject_cast<QLineEdit*>(editor);
        le->setText (index.data (Qt::DisplayRole).toString());
        return;
    }
    else if (editor)
    {
        QItemDelegate::setEditorData (editor, index);
        return;
    }
    else
        return;
}

void KKSOrgItemDelegate :: setModelData (QWidget * editor, QAbstractItemModel * model, const QModelIndex& index) const
{
    if (qobject_cast<QLineEdit*>(editor))
    {
        QLineEdit * le = qobject_cast<QLineEdit*>(editor);
        model->setData (index, le->text(), Qt::DisplayRole);
    }
    else if (editor)
    {
        QItemDelegate::setModelData (editor, model, index);
        return;
    }
    else
        return;
}
