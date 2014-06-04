/***********************************************************************
 * Module:  KKSSearchTemplateFilterProxyModel.h
 * Author:  yuriy
 * Modified: 6 ����� 2013 �. 16:59
 * Purpose: Declaration of the class KKSSearchTemplateFilterProxyModel
 * Comment: ������ ����� ����������� �� QSortFilterProxyModel � 
 * ������������ ��� ���������� �������� ������ � ��-������������.
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
