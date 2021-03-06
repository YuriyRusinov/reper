#include <QColor>
#include <QBrush>
#include <QPixmap>
#include <QImage>
#include <QIcon>
#include <QSize>
#include <QTimer>
#include <QApplication>
#include <QPalette>
#include <QtDebug>

#include "KKSTemplate.h"
#include "KKSCategory.h"
#include "KKSAttrType.h"
#include "KKSAttribute.h"
#include "KKSAttrView.h"
#include "KKSTreeItem.h"
#include "KKSEIODataModel.h"
#include <defines.h>

KKSEIODataModel :: KKSEIODataModel (const KKSTemplate * t, const KKSMap< qint64, KKSEIOData * > & objRecs, QObject *parent)
    : QAbstractItemModel (parent),
    tRef (t),
    cAttrP (0),
    objRecords (objRecs),
    //parIndexList (QMap<qint64, QModelIndex>()),
    //indList (QMap<qint64, QList<qint64> >()),
    rootItem (0),
    cAttrBackground (0),
    cAttrForeground (0)
{
    if (tRef)
        tRef->addRef();
    cAttrP = getFirstAttribute (KKSAttrType::atParent);
    cAttrBackground = getFirstAttribute (KKSAttrType::atRecordColor);
    if (!cAttrBackground)
        cAttrBackground = getFirstAttribute (KKSAttrType::atRecordColorRef);
    cAttrForeground = getFirstAttribute (KKSAttrType::atRecordTextColor);
    if (!cAttrForeground)
        cAttrForeground = getFirstAttribute (KKSAttrType::atRecordTextColorRef);
    setVisibleAttrs ();
    rootItem = new KKSTreeItem(-1, 0, t, visibleAttrs);
    setupData (rootItem);
}

KKSEIODataModel :: ~KKSEIODataModel ()
{
    if (tRef)
        tRef->release();
    delete rootItem;
}

int KKSEIODataModel :: columnCount (const QModelIndex& parent) const
{
    Q_UNUSED (parent);
    if (!tRef)
        return 0;
    return visibleAttrs.count();
}

int KKSEIODataModel :: rowCount (const QModelIndex& parent) const
{
    KKSTreeItem * parentItem = getItem (parent);
    int nr = parentItem->childCount();
    /*bool v (false);
    for (int i=0; i<parentItem->childCount(); i++)
        if (!parentItem->child(i) || !parentItem->child(i)->getData() || !parentItem->child(i)->getData()->isVisible())
        {
            v = true;
            nr--;
        }
    if (v)
        qDebug () << __PRETTY_FUNCTION__ << parent << nr;*/

    return nr;
}
 

QModelIndex KKSEIODataModel :: index (int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex (row, column, parent))
        return QModelIndex ();

    //qDebug () << __PRETTY_FUNCTION__ << row << parent;
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    KKSTreeItem *parentItem = getItem(parent);

    KKSTreeItem *childItem = parentItem->child(row);
    if ((childItem && childItem->getData() && childItem->getData()->isVisible()) ||
        (childItem && !childItem->getData()))
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex KKSEIODataModel :: parent (const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    KKSTreeItem *childItem = getItem(index);
    KKSTreeItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem )//|| !parentItem->getData() || !parentItem->getData()->isVisible())
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

Qt::ItemFlags KKSEIODataModel :: flags (const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 0 && tRef && tRef->category()->searchAttributesByType(KKSAttrType::atCheckListEx).size() > 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    
}

QVariant KKSEIODataModel :: data (const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole+2)
        return QVariant::fromValue<const KKSTemplate *>(tRef);
    else if (role == Qt::UserRole+3)
        return QVariant::fromValue<const KKSCategoryAttr *>(cAttrP);
    else if (role == Qt::UserRole+4)
        return QVariant::fromValue<const KKSCategoryAttr *>(cAttrBackground);
    else if (role == Qt::UserRole+5)
        return QVariant::fromValue<const KKSCategoryAttr *>(cAttrForeground);

    if (!index.isValid() )//&& role <= Qt::UserRole+1)
        return QVariant();

    KKSTreeItem * wItem = static_cast<KKSTreeItem*>(index.internalPointer());
    if (!wItem)
        return QVariant ();

    qint64 idw = wItem->id();//index.internalId();
    KKSList<KKSAttrView*> avList = tRef ? tRef->sortedAttrs() : KKSList<KKSAttrView*>();
    int iCol (index.column());
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
    {
        KKSAttrView * v = avList[index.column()];
        if ((v->type()->attrType() == KKSAttrType::atBinary ||
              (v->refType() && v->refType()->attrType() == KKSAttrType::atBinary)) && role == Qt::DisplayRole)
            return QVariant ();

        return wItem->columnData(iCol);
    }
    else if (role == Qt::UserRole)
    {
        return idw;//wItem->id();//p.key();
    }
    else if (role == Qt::UserRole+1)
        return QVariant::fromValue<KKSEIOData *>(wItem->getData());
    else if (role == Qt::BackgroundRole || role == Qt::BackgroundColorRole)
    {
        if (!wItem->getData())
            return QVariant();
        KKSEIOData * d = wItem->getData ();
        bool ok;
        //qDebug () << __PRETTY_FUNCTION__ << d->fields().value (cAttrBackground->code(false).toLower());
        quint64 vl = cAttrBackground ? d->fields().value (cAttrBackground->code(false)).toULongLong (&ok) : d->sysFieldValue("record_fill_color").toULongLong (&ok);
        if (!ok)
        {
            QVariant defBrushV;
            //defBrushV = QBrush (QColor(Qt::white));
            return defBrushV;
        }
        QVariant vc = QColor::fromRgba (vl);
        return vc;
    }
    else if (role == Qt::ForegroundRole )
    {
        if (!wItem->getData())
            return QVariant();
        KKSEIOData * d = wItem->getData ();
        bool ok;
        //qDebug () << __PRETTY_FUNCTION__ << d->fields().value (cAttrBackground->code(false).toLower());
        quint64 vl = cAttrForeground ? d->fields().value (cAttrForeground->code(false)).toULongLong (&ok) : d->sysFields().value("record_text_color").toULongLong (&ok);
        if (!ok)
        {
            QVariant defBrushV;
            //defBrushV = QBrush (QColor(Qt::black));
            return defBrushV;
        }
        QVariant vc = QColor::fromRgba (vl);
        return vc;
    }
    else if (role == Qt::DecorationRole)
    {
        KKSAttrView * v = avList[index.column()];
        if (index.column() == 0 && 
                (v->type()->attrType() != KKSAttrType::atSVG || (v->refType() && v->refType()->attrType() != KKSAttrType::atSVG)) &&
                 (v->type()->attrType() != KKSAttrType::atVideo || (v->refType() && v->refType()->attrType() != KKSAttrType::atVideo)) &&
                 (v->type()->attrType() != KKSAttrType::atBinary || (v->refType() && v->refType()->attrType() != KKSAttrType::atBinary))
                )
            return wItem->getIcon();
        if( v->type()->attrType() == KKSAttrType::atJPG ||
            (v->refType() && v->refType()->attrType() == KKSAttrType::atJPG)
            )
        {
            return QIcon (":/ddoc/jpg_icon.png");
        }
        else if ( v->type()->attrType() == KKSAttrType::atSVG ||
                  (v->refType() && v->refType()->attrType() == KKSAttrType::atSVG))
            return QIcon (":/ddoc/svg_icon.png");
        else if ( v->type()->attrType() == KKSAttrType::atVideo ||
                  (v->refType() && v->refType()->attrType() == KKSAttrType::atVideo))
            return QIcon (":/ddoc/movie_track.png");
        else if (v->type()->attrType() == KKSAttrType::atBinary ||
                  (v->refType() && v->refType()->attrType() == KKSAttrType::atBinary))
        {
            QVariant cData = wItem->columnData(iCol);
            QByteArray binaryData = cData.toByteArray ();
            if (binaryData.contains("\\x"))
                binaryData = QByteArray::fromHex(binaryData);
            QTextStream bIm (binaryData);
            unsigned az_dd;
            bIm >> az_dd;
            if (az_dd > 360)
                return QVariant();
            unsigned elev_dd;
            bIm >> elev_dd;
            unsigned nW, nH, nd;
            bIm >> nW;
            bIm >> nH;
            bIm >> nd;
            QImage pIm (nW, nH, QImage::Format_ARGB32);
            //bIm.readRawData(imData, nW*nH);
            QString imStr;// = bIm.read(nW*nH);
            bIm >> imStr;
            QByteArray imArr = imStr.toAscii();
            //qDebug () << __PRETTY_FUNCTION__ << imArr;
            if (imStr.isEmpty())
                return QVariant();
            int ncount (0);
            for (uint i=0; i<nW; i++)
            {
                for (uint j=0; j<nH; j++)
                {
                    // = new char;// = (uchar)imData[ncount++];
                    //bIm >> imData;
                    //QByteArray s = QByteArray (imArr.at(ncount++));
                    uint c = (uint)imStr.at(ncount++).digitValue();//s.toUInt();
                    c *= 255;
                    pIm.setPixel(i, j, qRgb(c,c,c));
                }
            }
            return pIm;
        }
        else
            return QVariant();
    }
    else if (role == Qt::SizeHintRole)
    {
        KKSAttrView * v = avList[index.column()];
        if (!tRef->category()->searchAttributesByType (KKSAttrType::atBinary).isEmpty())
            return QSize (v->defWidth(), 32);
        else
            return QSize (v->defWidth(),24);
    }
    return QVariant();
}

QVariant KKSEIODataModel :: headerData (int section, Qt::Orientation orientation, int role) const
{
    KKSList<KKSAttrView*> avList(visibleAttrs);// = tRef ? tRef->sortedAttrs() : KKSList<KKSAttrView*>();
    if (orientation == Qt::Horizontal && section >= 0 && section < avList.count() && ((role == Qt::DisplayRole) || (role == Qt::EditRole)) )
    {
        QVariant v = avList[section]->title();
        return v;
    }
    else if (orientation == Qt::Horizontal && section >= 0 && section < avList.count() && role == Qt::SizeHintRole)
    {
        QSize hSize (avList[section]->defWidth(), 24);
        return hSize;
    }
    else
        return QVariant ();
}

bool KKSEIODataModel :: setData (const QModelIndex& index, const QVariant& value, int role)
{
    if (role == Qt::UserRole+4)
    {
        cAttrBackground = value.value<const KKSCategoryAttr*>();
        return true;
    }
    else if (role == Qt::UserRole+5)
    {
        cAttrForeground = value.value<const KKSCategoryAttr*>();
        return true;
    }
    KKSTreeItem * wItem = getItem (index);
    if (!wItem )//&& role != Qt::UserRole+1)
        return false;
    
    int irow = index.row ();
    QModelIndex pIndex = index.parent();
    QModelIndex topL = this->index (irow, 0, pIndex);
    QModelIndex bottomR = this->index (irow, columnCount(pIndex)-1, pIndex);
    //qDebug () << __PRETTY_FUNCTION__ << topL << bottomR;
    if (role == Qt::UserRole)
    {
        qint64 id = value.value<qint64>();
        wItem->setId (id);
        if (!objRecords.contains(id))
            objRecords.insert(id, 0);
        emit dataChanged (topL, bottomR);
    }
    else if (role == Qt::DisplayRole)
    {
        KKSEIOData d = value.value<KKSEIOData>();
        KKSEIOData * dVal = new KKSEIOData (d);
        if (objRecords.contains(wItem->id()))
        {
            objRecords.remove (wItem->id());
            objRecords.insert (wItem->id(), dVal);
        }
        wItem->setData (dVal, tRef, visibleAttrs);
        dVal->release ();
        emit dataChanged (topL, bottomR);
    }
    else if (role == Qt::EditRole)
    {
        //qDebug () << __PRETTY_FUNCTION__ << value;//dVal->fields().value("Goodness of fit");
        wItem->setData (index.column(), value);
        emit dataChanged (index, index);
    }
    else if (role == Qt::UserRole+1)
    {
        KKSEIOData * dVal = value.value<KKSEIOData *>();
        if (objRecords.contains(wItem->id()))
        {
            objRecords.remove (wItem->id());
            objRecords.insert (wItem->id(), dVal);
        }
        wItem->setData (dVal, tRef, visibleAttrs);
        emit dataChanged (topL, bottomR);
    }
    else if (role == Qt::UserRole+2)
    {
        const KKSTemplate * t = value.value<const KKSTemplate *>();
        if (!t)
            return false;
        if (t != tRef)
        {
            if (tRef)
                tRef->release ();

            tRef = t;
            tRef->addRef();
        }
        KKSEIOData * dVal = wItem->getData();;
        wItem->setData (dVal, tRef, visibleAttrs);
        emit dataChanged (topL, bottomR);
    }
    else if (role == Qt::DecorationRole && index.column() == 0)
    {
        QIcon icon;
        switch (value.type())
        {
            case QVariant::Icon:default:
            {
                icon = value.value<QIcon>();
                break;
            }
            case QVariant::Pixmap:
            {
                icon = QIcon (value.value<QPixmap>());
                break;
            }
            case QVariant::Image:
            {
                icon = QIcon (QPixmap::fromImage (value.value<QImage>()));
                break;
            }
        }
        wItem->setIcon (icon);
        emit dataChanged (topL, topL);
    }
    else if (role == Qt::BackgroundRole)
    {
        if (!wItem->getData() || !cAttrBackground)
            return false;
        KKSEIOData * d = wItem->getData ();
        bool ok;
        //qDebug () << __PRETTY_FUNCTION__ << d->fields().value (cAttrBackground->code(false).toLower());
        quint64 vl = value.toLongLong(&ok);//cAttrForeground ? d->fields().value (cAttrForeground->code(false)).toULongLong (&ok) : d->sysFields().value("record_text_color").toULongLong (&ok);
        if (!ok)
            return false;
        d->fields().insert (cAttrBackground->code(false), QString::number (vl));
        //QVariant vc = QColor::fromRgba (vl);
        emit dataChanged (topL, topL);
        return true;
    }
    else if (role == Qt::ForegroundRole)
    {
        if (!wItem->getData() || !cAttrForeground)
            return false;
        KKSEIOData * d = wItem->getData ();
        bool ok;
        //qDebug () << __PRETTY_FUNCTION__ << d->fields().value (cAttrBackground->code(false).toLower());
        quint64 vl = value.toLongLong(&ok);//cAttrForeground ? d->fields().value (cAttrForeground->code(false)).toULongLong (&ok) : d->sysFields().value("record_text_color").toULongLong (&ok);
        if (!ok)
            return false;
        d->fields().insert (cAttrForeground->code(false), QString::number (vl));
        //QVariant vc = QColor::fromRgba (vl);
        emit dataChanged (topL, topL);
        return true;
    }
    else
        return false;
    
    return true;
}

bool KKSEIODataModel :: insertRows (int row, int count, const QModelIndex& parent)
{
    //qDebug () << __PRETTY_FUNCTION__ << row << count << parent;
    KKSTreeItem * pItem = getItem (parent);
    bool ok (true);
    beginInsertRows (parent, row, row+count-1);

    ok = pItem->insertChildren(row, count);
    endInsertRows ();
    return ok;
}

bool KKSEIODataModel :: removeRows (int row, int count, const QModelIndex& parent)
{
    if(count == 0)
        return true;

    KKSTreeItem * pItem = getItem (parent);
    bool ok (true);
    
    beginRemoveRows (parent, row, row+count-1);
    ok = pItem->removeChildren (row, count);
    endRemoveRows ();
    
    return ok;
}


const KKSCategoryAttr * KKSEIODataModel :: getFirstAttribute (KKSAttrType::KKSAttrTypes aType)
{
    if (!tRef)
        return 0;
    const KKSList<KKSAttrView*> tAttrs = tRef->sortedAttrs();
    int idAttr (-1);
    for (int i=0; i<tAttrs.count() && idAttr < 0; i++)
    {
        if (tAttrs[i]->type()->attrType() == aType)
            idAttr = tAttrs[i]->id();
    }
    if (idAttr < 0)
    {
        QList<int> pList = tRef->category()->searchAttributesByType(aType);
        if (!pList.empty())
            idAttr = pList[0];
    }
    const KKSCategoryAttr * cAttr = idAttr >= 0 ? tRef->category()->attribute(idAttr) : 0;
    return cAttr;
}

void KKSEIODataModel :: setupData (KKSTreeItem * parent)
{
    int nr = rowCount();
    parent->clearChildren ();
    if (!cAttrP)
    {
        for (KKSMap<qint64, KKSEIOData* >::const_iterator p = objRecords.constBegin();
                p != objRecords.constEnd();
                p++)
        {
            QIcon tIcon;
            //qDebug () << __PRETTY_FUNCTION__ << p.value()->sysFields();
            if (p.value())
            {
                QString strIcon = p.value()->sysFieldValue("r_icon");
                if (strIcon.isEmpty())
                {
                    tIcon = QIcon(":/ddoc/rubric_item.png");
                }
                else
                {
                    //qDebug () << __PRETTY_FUNCTION__ << strIcon;
                    QPixmap pIcon;
                    pIcon.loadFromData (strIcon.toUtf8());
                    tIcon = QIcon (pIcon);
                }
            }
            KKSTreeItem * t = new KKSTreeItem (p.key(), p.value(), tRef, visibleAttrs, tIcon);
            parent->appendChild (t);
        }
        if (nr > 0 && parent->childCount() > nr)
        {
            beginInsertRows (QModelIndex(), nr, parent->childCount()-1);
            endInsertRows ();
        }
        else if (nr > 0 && parent->childCount() < nr )
        {
            beginRemoveRows (QModelIndex(), parent->childCount(), nr-1);
            endRemoveRows ();
        }
        return;
    }

    QModelIndex pIndex = QModelIndex ();

    KKSMap<qint64, KKSEIOData*> sortedData;
    for(KKSMap<qint64, KKSEIOData* >::const_iterator p = objRecords.constBegin();
            p != objRecords.constEnd();
            p++)
    {
        sortedData.insertMulti(p.value()->sysFieldValue("ii_rec_order").toLongLong(), p.value());
    }

    KKSTreeItem * prevItem (0);
    for (KKSMap<qint64, KKSEIOData* >::const_iterator p = sortedData.constBegin();
            p != sortedData.constEnd();
            p++)
    {
        QIcon tIcon;
        if (p.value())
        {
            QString strIcon = p.value()->sysFieldValue("r_icon");
            //qDebug () << __PRETTY_FUNCTION__ << strIcon;
            QPixmap pIcon;
            pIcon.loadFromData (strIcon.toUtf8());
            tIcon = QIcon (pIcon);
        }

        KKSTreeItem * t = new KKSTreeItem (p.value()->sysFieldValue("id").toLongLong(), p.value(), tRef, visibleAttrs, tIcon);
        if (!t->getData() || !t->getData()->isVisible())
            continue;
        
        QString valStr = p.value()->sysFieldValue(cAttrP->code(false));
        if (valStr.isEmpty())
            parent->appendChild (t);
        else
        {
            bool ok;
            qint64 iVal = valStr.toLongLong(&ok);
            if (ok && iVal > 0)
            {
                if (prevItem && prevItem->id() == iVal)
                    prevItem->appendChild (t);
                else if (prevItem)
                {
                    KKSTreeItem * p1 (0);
                    while (prevItem && prevItem->id() != iVal && !p1)
                    {
                        prevItem = prevItem->parent();
                        p1 = getModelItem (iVal, prevItem, pIndex);
                    }
                    if (!p1)
                        parent->appendChild (t);
                    else
                    {
                        prevItem = p1;
                        prevItem->appendChild (t);
                    }
                }
                else
                {
                    KKSTreeItem * parent1 = getModelItem (iVal, rootItem, pIndex);
                    if(!parent1){ //����� �������� ������-�� �������� ��������, ������� ����, ��� ���-�� �� ���
                        parent->appendChild(t);
                        prevItem = 0;
                    }
                    else{
                        parent1->appendChild (t);
                        prevItem = parent1;
                    }    
                    
                }
            }
            else
            {
                parent->appendChild (t);
                prevItem = 0;
            }
        }
    }

    return;
}

KKSTreeItem * KKSEIODataModel :: getModelItem (qint64 val, KKSTreeItem * parent, QModelIndex & pIndex)
{
    if (!parent)
        return rootItem;

    //if (parent->getData() && QString::compare (parent->getData()->fieldValue(cAttrP->columnName(false)), valStr) == 0)
    //    return parent;
    for (int i=0; i<parent->childCount(); i++)
    {
        KKSTreeItem * item = parent->child(i);
        QString refCol = cAttrP->refColumnName(false);
        if (refCol.isEmpty())
            refCol = QString ("id");
        QString pStr = item->getData()->sysFieldValue(refCol);
        //qDebug () << __PRETTY_FUNCTION__ << pStr << refCol << cAttrP->code(false) << item->getData()->fields();
        bool ok (true);
        qint64 iVal (-1);
        if (QString::compare(refCol, QString("id")) == 0)
            iVal = item->id();
        else
            pStr.toLongLong(&ok);// cAttrP->columnName(false)
        if (!ok)
            continue;
        if (iVal == val)
        {
            pIndex = this->index (i, 0, pIndex);
            return item;
        }
        else if (item->childCount() > 0)
        {
            KKSTreeItem * chItem = getModelItem (val, item, pIndex);
            if (chItem)
                return chItem;
        }
        else
            continue;
    }
    
    return 0;
}

KKSTreeItem * KKSEIODataModel :: getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        KKSTreeItem *item = static_cast<KKSTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;   
}

void KKSEIODataModel :: setVisibleAttrs (void)
{
    if (!tRef)
        visibleAttrs.clear ();
    QList<KKSAttrView*> attrs_list = tRef->sortedAttrs();
    for (int i=0; i<attrs_list.count(); i++)
        if (attrs_list[i]->isVisible() && \
                (attrs_list[i]->type ()->attrType () != KKSAttrType::atRecordColor) && \
                (attrs_list[i]->type ()->attrType () != KKSAttrType::atRecordColorRef) && \
                (attrs_list[i]->type ()->attrType () != KKSAttrType::atRecordTextColor) && \
                (attrs_list[i]->type ()->attrType () != KKSAttrType::atRecordTextColorRef)
           )
        {
            visibleAttrs.append (attrs_list[i]);
        }
    
}
