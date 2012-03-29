/***********************************************************************
 * Module:  KKSAttributesModel.h
 * Author:  yuriyrusinov
 * Modified: 06.09.2010 г. 16:07:00
 * Purpose: Declaration of the class KKSAttributesModel
 * Comment: Данный класс унаследован от StandardItemModel и предназначен для
 *  отображения списка атрибутов с возможностью добавления их в категорию
 *  с помощью drag-and-drop технологий
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttributesModel_h)
#define __KKSSITOOOM_KKSAttributesModel_h

#include "kksgui_config.h"

#include <QStandardItemModel>

class _GUI_EXPORT KKSAttributesModel : public QStandardItemModel
{
public:
    KKSAttributesModel (QObject *parent=0);
    KKSAttributesModel (int rows, int cols, QObject *parent=0);
    virtual ~KKSAttributesModel (void);

    Qt::ItemFlags flags (const QModelIndex &index) const;

    QMimeData *mimeData (const QModelIndexList &indexes) const;

    QStringList mimeTypes (void) const;

    Qt::DropActions supportedDropActions (void) const;

private:
    Q_OBJECT
};

#endif
