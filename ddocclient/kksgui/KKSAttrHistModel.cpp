/* 
 * File:   KKSAttrHistModel.cpp
 * Author: yuriyrusinov
 * 
 * Created on 30 Август 2013 г., 16:16
 */
#include <QStringList>
#include <QSize>

#include <KKSAttrValue.h>
#include <KKSAttribute.h>
#include <KKSAttrType.h>
#include <KKSObject.h>
#include "KKSAttrHistModel.h"

KKSAttrHistModel::KKSAttrHistModel(const KKSList<KKSAttrValue*>& _histlist, QObject * parent)
    : QAbstractItemModel (parent),
      histList (_histlist)
{
}

KKSAttrHistModel::~KKSAttrHistModel()
{
}

int KKSAttrHistModel::columnCount (const QModelIndex& parent) const
{
    Q_UNUSED (parent);
    return 7;
}

int KKSAttrHistModel::rowCount (const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return histList.count();
}

QModelIndex KKSAttrHistModel::index (int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex (row, column, parent))
        return QModelIndex ();

    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
 
    KKSAttrValue * av = histList.at (row);
    return createIndex (row, column, av);
}

QModelIndex KKSAttrHistModel::parent (const QModelIndex& /*index*/) const
{
//    if (!index.isValid())
    return QModelIndex();
}

Qt::ItemFlags KKSAttrHistModel::flags (const QModelIndex& index) const
{
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant KKSAttrHistModel::data (const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int iRow = index.row();
    int iCol = index.column();
    KKSAttrValue * av = histList.at (iRow);
    if (!av)
        return QVariant();
    int idAv = av->id();
    KKSAttribute * a = av->attribute();
    if (!a)
        return QVariant();
    const KKSAttrType * t = a->type();
    const KKSAttrType * rt = a->refType();
    int tId = t->attrType();
    switch (role)
    {
        case Qt::UserRole: return idAv; break;
        case Qt::UserRole+1: return (QVariant::fromValue<KKSAttrValue *>(av)); break;
        case Qt::DisplayRole:
        {
            switch (iCol)
            {
                case 0: return idAv; break;
                case 3:
                    {
                        if (tId == KKSAttrType::atJPG ||
                            (rt && rt->attrType() == KKSAttrType::atJPG))
                            return tr("<Image data %1>").arg(iRow);
                        else if (tId == KKSAttrType::atSVG ||
                                 (rt && rt->attrType() == KKSAttrType::atSVG)
                                )
                            return tr("<SVG data %1>").arg(iRow);
                        else if( tId == KKSAttrType::atXMLDoc || 
                                (rt && rt->attrType() == KKSAttrType::atXMLDoc)
                               )
                            return tr("<XML document %1>").arg(iRow);
                        else if( tId == KKSAttrType::atVideo || 
                                (rt && rt->attrType() == KKSAttrType::atVideo)
                               )
                            return tr("<Video data %1>").arg(iRow);
                        else if( tId == KKSAttrType::atHistogram || 
                                (rt && rt->attrType() == KKSAttrType::atHistogram)
                               )
                            return tr("<Histogram data %1>").arg(iRow);
                        else if( tId == KKSAttrType::atGISMap || 
                                (rt && rt->attrType() == KKSAttrType::atGISMap)
                               )
                            return tr("<GIS map data %1>").arg(iRow);
                        else if (tId == KKSAttrType::atRecordColor || 
                                 tId == KKSAttrType::atRecordColorRef ||
                                 tId == KKSAttrType::atRecordTextColor || 
                                 tId == KKSAttrType::atRecordTextColorRef)
                        {
                            bool ok;
                            quint64 vl = av->value().valueForInsert().toLongLong(&ok);
                            if (!ok)
                                return QVariant();
                            return QString::number (vl);// ("Text example");
                        }
                        else if (tId == KKSAttrType::atInterval ||
                                 tId == KKSAttrType::atIntervalH )
                        {
                            return av->value().valueForInsert();
                        }
                        else
                        {
                            QVariant val;
                            QString sVal (av->value().valueVariant().toString());
                            if (sVal.contains (QString("\n")))
                                val = sVal.mid (0, sVal.indexOf("\n"))+ QString("...");
                            else
                                val = sVal;

                            return val;//av->value().valueVariant();
                        }
                        break;
                    }
                case 1:
                    return av->startDateTime().toString("dd.MM.yyyy hh:mm:ss"); break;
                case 2:
                    return av->stopDateTime().toString("dd.MM.yyyy hh:mm:ss"); break;
                case 4:
                    {
                        if (av->ioSrc())
                            return av->ioSrc()->name();
                        else
                            return QString();
                        break;
                    }
                case 5:
                    {
                        if (av->ioSrc1())
                            return av->ioSrc1()->name();
                        else
                            return QString();
                        break;
                    }
                case 6:
                    {
                        if (!av->desc().isEmpty())
                            return av->desc();
                        else
                            return QString();
                        break;
                    }
            }
            case Qt::BackgroundRole: 
            {
                if (tId == KKSAttrType::atRecordColor || 
                    tId == KKSAttrType::atRecordColorRef)
                {
                    bool ok;
                    quint64 vl = av->value().valueForInsert().toLongLong(&ok);
                    if (!ok)
                        return QVariant();
                    return QColor::fromRgba (vl);
                }
                break;
            }
            case Qt::ForegroundRole:
            {
                if (tId == KKSAttrType::atRecordTextColor || 
                    tId == KKSAttrType::atRecordTextColorRef)
                {
                    bool ok;
                    quint64 vl = av->value().valueForInsert().toLongLong(&ok);
                    if (!ok)
                        return QVariant();
                    return QColor::fromRgba (vl);
                }
                break;
            }
            case Qt::SizeHintRole:
            {
                switch (iCol)
                {
                    case 0: return QSize (20, 24); break;
                    case 1: case 2: return QSize (140, 24); break;
                    case 3: return QSize (150, 24); break;
                    case 4: case 5: return QSize (120, 24); break;
                    case 6: default: return QSize (100, 24); break;
                }
                break;
            }
        }
        default: break;
    }

    return QVariant();
}

QVariant KKSAttrHistModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    QStringList listHeader;
    listHeader <<tr("Id")<<tr("Start time")<<tr("Stop time")<<tr("Value")<<tr("Source")<<tr("Transfer")<<tr("Description");
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return listHeader[section];
    else
        return QVariant();
}
