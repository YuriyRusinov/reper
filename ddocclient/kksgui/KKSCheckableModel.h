/***********************************************************************
 * Module:  KKSCheckableModel.h
 * Author:  yuriy
 * Modified: 19 мая 2009 г. 11:59
 * Purpose: Declaration of the class KKSCheckableModel
 * Comment: Данный класс унаследован от QStandardItemModel и 
 * предназначен для выделения чекбоксов в записях.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSCheckableModel_h)
#define __KKSSITOOOM_KKSCheckableModel_h

#include "kksgui_config.h"

#include <QStandardItemModel>

class _GUI_EXPORT KKSCheckableModel : public QStandardItemModel
{
    public:
        KKSCheckableModel (QObject *parent=0);
        KKSCheckableModel (int rows, int columns, QObject * parent=0);
        ~KKSCheckableModel (void);

        virtual Qt::ItemFlags flags (const QModelIndex& index) const;
        virtual QVariant data (const QModelIndex& index, int role) const;
        virtual bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    private:
        Q_OBJECT
};

#endif
