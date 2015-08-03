#ifndef SQV_DELEGATE_H
#define SQV_DELEGATE_H

#include <QItemDelegate>

class sqv_delegate : public QItemDelegate
{
	Q_OBJECT

public:
	sqv_delegate(QObject *parent);
	~sqv_delegate();

	void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index ) const;
	QSize sizeHint(const QStyleOptionViewItem & option,const QModelIndex & index) const;

private:
	
};

#endif // SQV_DELEGATE_H
