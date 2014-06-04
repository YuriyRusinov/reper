/***********************************************************************
 * Module:  KKSCatAttrsModel.h
 * Author:  yuriyrusinov
 * Modified: 06.09.2010 г. 19:32:00
 * Purpose: Declaration of the class KKSCatAttrsModel
 * Comment: Данный класс унаследован от StandardItemModel и предназначен для
 *  отображения списка атрибутов в категории
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSCatAttrsModel_h)
#define __KKSSITOOOM_KKSCatAttrsModel_h

#include <QStandardItemModel>

#include "kksgui_config.h"

class _GUI_EXPORT KKSCatAttrsModel : public QStandardItemModel
{
public:
    KKSCatAttrsModel (QObject *parent=0);
    KKSCatAttrsModel (int rows, int cols, QObject *parent=0);
    virtual ~KKSCatAttrsModel (void);

    Qt::ItemFlags flags (const QModelIndex &index) const;

    bool dropMimeData (const QMimeData *data, Qt::DropAction action,
                       int row, int column, const QModelIndex &parent);

    Qt::DropActions supportedDropActions() const;

private:
    Q_OBJECT
};

#endif
