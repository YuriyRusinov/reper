#include <QDate>
#include <QDateTime>

#include <KKSAttrView.h>
#include <KKSAttrType.h>
#include "KKSSortFilterProxyModel.h"
#include "defines.h"

KKSSortFilterProxyModel :: KKSSortFilterProxyModel (QObject *parent)
    : QSortFilterProxyModel (parent)
{
    clearAttrs ();
}

KKSSortFilterProxyModel :: ~KKSSortFilterProxyModel (void)
{
    clearAttrs ();
}

void KKSSortFilterProxyModel :: clearAttrs (void)
{
    if (!attrViews.empty ())
        attrViews.clear ();
}

void KKSSortFilterProxyModel :: addAttrView (KKSAttrView *av)
{
    if (av)
        attrViews.insert (av->id(), av);
}

bool KKSSortFilterProxyModel :: filterAcceptColumns (int source_column, const QModelIndex & source_parent) const
{
    bool isAccepted ((!source_parent.isValid() || source_parent.column()==0) && source_column >=0 && source_column < attrViews.count());
    return isAccepted;
}

bool KKSSortFilterProxyModel :: filterAcceptsRow (int source_row, const QModelIndex& source_parent) const
{
    if (!sourceModel())
        return false;
    QModelIndex wIndex = sourceModel()->index (source_row, 0, source_parent);
    //QModelIndex pIndex = mapFromSource (wIndex);
    //if (rowCount (pIndex) > 0)
    //    return true;
    //else
        return QSortFilterProxyModel::filterAcceptsRow (source_row, source_parent);
}

bool KKSSortFilterProxyModel :: lessThan (const QModelIndex & left, const QModelIndex & right) const
{
    int leftDataEn = this->sourceModel()->data (left, Qt::UserRole+USER_ENTITY).toInt();
    int rightDataEn = this->sourceModel()->data (right, Qt::UserRole+USER_ENTITY).toInt();
    if (leftDataEn != rightDataEn)
        return leftDataEn < rightDataEn;
    QVariant leftData = this->sourceModel()->data (left, Qt::DisplayRole);
    QVariant rightData = this->sourceModel()->data (right, Qt::DisplayRole);

    KKSMap<int, KKSAttrView*>::const_iterator p = attrViews.constBegin() + left.column();
    if (p == attrViews.constEnd() || !p.value())
        return leftData.toString() < rightData.toString();
    switch (p.value()->type()->attrType())
    {
        case KKSAttrType::atString: return leftData.toString() < rightData.toString();
        case KKSAttrType::atInt: return leftData.toInt() < rightData.toInt();
        case KKSAttrType::atInt64: return leftData.toLongLong() < rightData.toLongLong();
        case KKSAttrType::atDouble: return leftData.toDouble() < rightData.toDouble();
        case KKSAttrType::atBool: return leftData.toBool() < rightData.toBool();
        case KKSAttrType::atDate: 
                                 {
                                      if (leftData.toString().isEmpty())
                                          return true;
                                      else if (rightData.toString().isEmpty())
                                          return false;
                                      else
                                          return QDate::fromString (leftData.toString(), QString("yyyy-MM-dd")) < QDate::fromString (rightData.toString(),  QString("yyyy-MM-dd"));
                                  }
                                  break;
        case KKSAttrType::atDateTime:
                                     {
                                          if (leftData.toString().isEmpty())
                                              return true;
                                          else if (rightData.toString().isEmpty())
                                              return false;
                                          else
                                              return QDateTime::fromString (leftData.toString(), QString("dd.MM.yyyy H:mm:ss")) < QDateTime::fromString (rightData.toString(),  QString("dd.MM.yyyy H:mm:ss"));
                                     }
                                     break;
        case KKSAttrType::atDateTimeEx:
                                     {
                                          if (leftData.toString().isEmpty())
                                              return true;
                                          else if (rightData.toString().isEmpty())
                                              return false;
                                          else
                                              return QDateTime::fromString (leftData.toString(), QString("dd.MM.yyyy H:mm:ss.zzz")) < QDateTime::fromString (rightData.toString(),  QString("dd.MM.yyyy H:mm:ss.zzz"));
                                     }
                                     break;
        default: return leftData.toString() < rightData.toString();
    }
    return leftData.toString() < rightData.toString();
}
