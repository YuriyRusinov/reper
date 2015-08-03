/* 
 * File:   KKSAttrHistoryProxyModel.h
 * Author: rusinov
 *
 * Created on 31 ���� 2013 �., 17:10
 */

#ifndef KKSATTRHISTORYPROXYMODEL_H
#define	KKSATTRHISTORYPROXYMODEL_H

#include "kksgui_config.h"

#include <QSortFilterProxyModel>

class _GUI_EXPORT KKSAttrHistoryProxyModel : public QSortFilterProxyModel
{
public:
    KKSAttrHistoryProxyModel(QObject * parent=0);
    virtual ~KKSAttrHistoryProxyModel();

protected:
    //
    // Overrides functions
    //
    virtual bool lessThan (const QModelIndex & left, const QModelIndex & right) const;
        
private:
    Q_OBJECT
};

#endif	/* KKSATTRHISTORYPROXYMODEL_H */

