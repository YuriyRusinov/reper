#ifndef KKSSTATISTICVIEW_H
#define KKSSTATISTICVIEW_H

#include <QWidget>
#include <kksgui_config.h>

#include <QTreeView>
#include <KKSList.h>
#include <KKSStatElement.h>
#include <QModelIndex>
#include <QAbstractItemDelegate>

class _GUI_EXPORT KKSStatisticView: public QTreeView
{
    Q_OBJECT

    public:
        KKSStatisticView (KKSList<KKSStatElement*> items,
                          QWidget *parent = 0);

        ~KKSStatisticView();

    private slots:
        void init();
    
    private:

        KKSList<KKSStatElement*> m_items;
};


class KKSStatisticDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
    public:
        KKSStatisticDelegate(QObject * parent = 0);
        virtual ~KKSStatisticDelegate ();

        void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
        QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif // KKSSTATISTICVIEW_H
