/***********************************************************************
 * Module:  KKSSearchTemplateFilterProxyModel.h
 * Author:  yuriy
 * Modified: 6 марта 2013 г. 16:59
 * Purpose: Declaration of the class KKSSearchTemplateFilterProxyModel
 * Comment: Данный класс унаследован от QSortFilterProxyModel и 
 * предназначен для сортировки шаблонов поиска в ИО-справочниках.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSSearchTemplateFilterProxyModel_h)
#define __KKSSITOOOM_KKSSearchTemplateFilterProxyModel_h

#include "kksgui_config.h"

#include <QSortFilterProxyModel>

class _GUI_EXPORT KKSSearchTemplateFilterProxyModel : public QSortFilterProxyModel
{
public:
    KKSSearchTemplateFilterProxyModel (QObject *parent=0);
    ~KKSSearchTemplateFilterProxyModel (void);

protected:
    //
    // Overrides functions
    //
    virtual bool filterAcceptsRow (int source_row, const QModelIndex& source_parent) const;
    virtual bool lessThan (const QModelIndex& left, const QModelIndex& right) const;

private:
    Q_OBJECT
};

#endif
