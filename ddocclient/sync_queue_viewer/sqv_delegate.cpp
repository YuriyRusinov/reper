#include "sqv_delegate.h"

sqv_delegate::sqv_delegate(QObject *parent)
	: QItemDelegate(parent)
{

}

sqv_delegate::~sqv_delegate()
{

}

void sqv_delegate::paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index ) const
{
	QItemDelegate::paint(painter,option,index);
}

QSize sqv_delegate::sizeHint(const QStyleOptionViewItem & option,const QModelIndex & index) const
{
	QSize res = QItemDelegate::sizeHint(option,index);
	res.setHeight(24);
	return res;
}