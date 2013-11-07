#ifndef DYNDOC_DELEGATE_H
#define DYNDOC_DELEGATE_H

#include <QItemDelegate>

class dyndoc_delegate : public QItemDelegate
{
	Q_OBJECT

public:
    dyndoc_delegate(QObject *parent);
    ~dyndoc_delegate();

	void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index ) const;
	QSize sizeHint(const QStyleOptionViewItem & option,const QModelIndex & index) const;

private:
	
};

#endif // SQV_DELEGATE_H
