/***********************************************************************
 * Module:  KKSSortFilterProxyModel.h
 * Author:  yuriy
 * Modified: 2 марта 2009 г. 16:59
 * Purpose: Declaration of the class KKSSortFilterProxyModel
 * Comment: Данный класс унаследован от QSortFilterProxyModel и 
 * предназначен для сортировки записей ИО.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSSortFilterProxyModel_h)
#define __KKSSITOOOM_KKSSortFilterProxyModel_h

#include "kksgui_config.h"

#include <QSortFilterProxyModel>

#include <KKSMap.h>

class KKSAttrView;

class _GUI_EXPORT KKSSortFilterProxyModel : public QSortFilterProxyModel
{
    public:
        KKSSortFilterProxyModel (QObject *parent=0);
        ~KKSSortFilterProxyModel (void);

        void clearAttrs (void);
        void addAttrView (KKSAttrView *av);

    protected:
        //
        // Overrides functions
        //
        virtual bool filterAcceptColumns (int source_column, const QModelIndex & source_parent) const;
        virtual bool lessThan (const QModelIndex & left, const QModelIndex & right) const;
        virtual bool filterAcceptsRow (int source_row, const QModelIndex& source_parent) const;

    private:
        //
        // Variables
        //
        KKSMap<int, KKSAttrView *> attrViews;

    private:
        Q_OBJECT
};

#endif
