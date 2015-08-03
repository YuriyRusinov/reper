/***********************************************************************
 * Module:  KKSAclTemplateCheckableModel.h
 * Author:  yuriy
 * Modified: 10 июня 2009 г. 13:00
 * Purpose: Declaration of the class KKSAclTemplateCheckableModel
 * Comment: Данный класс унаследован от QStandardItemModel и 
 * предназначен для выделения чекбоксов в записях.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAclTemplateCheckableModel_h)
#define __KKSSITOOOM_KKSAclTemplateCheckableModel_h

#include "kksgui_config.h"

#include <QStandardItemModel>

class _GUI_EXPORT KKSAclTemplateCheckableModel : public QStandardItemModel
{
    public:
        KKSAclTemplateCheckableModel (QObject *parent=0);
        KKSAclTemplateCheckableModel (int nrow, int ncol, QObject *parent=0);
        virtual ~KKSAclTemplateCheckableModel (void);

        virtual Qt::ItemFlags flags (const QModelIndex& index) const;

    private:
        Q_OBJECT
};

#endif
