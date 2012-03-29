#include "KKSAclTemplateCheckableModel.h"

KKSAclTemplateCheckableModel :: KKSAclTemplateCheckableModel (QObject *parent)
    : QStandardItemModel (parent)
{
}

KKSAclTemplateCheckableModel :: KKSAclTemplateCheckableModel (int nrow, int ncol, QObject *parent)
    : QStandardItemModel (nrow, ncol, parent)
{
}

KKSAclTemplateCheckableModel :: ~KKSAclTemplateCheckableModel (void)
{
}

Qt::ItemFlags KKSAclTemplateCheckableModel :: flags (const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.parent().isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
