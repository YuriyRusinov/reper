/* 
 * File:   KKSRecProxyModel.h
 * Author: yuriy
 *
 * Created on 17 Èþëü 2012 ã., 22:20
 */

#ifndef KKSRECPROXYMODEL_H
#define	KKSRECPROXYMODEL_H

#include <QAbstractProxyModel>
#include <QModelIndex>

#include "kksgui_config.h"

class _GUI_EXPORT KKSRecProxyModel : public QAbstractProxyModel
{
public:
    KKSRecProxyModel(QObject * parent=0);
    virtual ~KKSRecProxyModel();
    
    virtual QModelIndex mapFromSource (const QModelIndex& sourceIndex) const;
    virtual QModelIndex mapToSource (const QModelIndex& proxyIndex) const;
private:
    Q_OBJECT

};

#endif	/* KKSRECPROXYMODEL_H */

