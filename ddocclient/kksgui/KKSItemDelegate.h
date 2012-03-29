/***********************************************************************
 * Module:  KKSItemDelegate.h
 * Author:  yuriy
 * Modified: 2 марта 2009 г. 19:55
 * Purpose: Declaration of the class KKSItemDelegate
 * Comment: Данный класс унаследован от QItemDelegate и 
 * предназначен для отображения данных в таблицах.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSItemDelegate_h)
#define __KKSSITOOOM_KKSItemDelegate_h

#include <QItemDelegate>

#include "kksgui_config.h"

class KKSCategory;

class _GUI_EXPORT KKSItemDelegate : public QItemDelegate
{
public:
    KKSItemDelegate (QObject *parent=0);
    ~KKSItemDelegate (void);

    virtual QWidget * createEditor (QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    virtual void paint (QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    const KKSCategory * getCategory (void) const;
    void setCategory (const KKSCategory * c);

private:
    //
    // Variables
    //
    const KKSCategory * cat;
    int iBackGroundColorSet;
    int iTextColorSet;

private:
    Q_OBJECT
};

#endif
