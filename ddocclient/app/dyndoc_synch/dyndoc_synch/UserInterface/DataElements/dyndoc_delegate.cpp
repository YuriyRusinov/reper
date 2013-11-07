#include "dyndoc_delegate.h"

dyndoc_delegate::dyndoc_delegate(QObject *parent)
	: QItemDelegate(parent)
{

}

dyndoc_delegate::~dyndoc_delegate()
{

}

void dyndoc_delegate::paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index ) const
{
	QItemDelegate::paint(painter,option,index);
}

QSize dyndoc_delegate::sizeHint(const QStyleOptionViewItem & option,const QModelIndex & index) const
{
	QSize res = QItemDelegate::sizeHint(option,index);
	res.setHeight(24);
	return res;
}
