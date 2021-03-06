/***********************************************************************
 * Module:  KKSAttributesFactory.h
 * Author:  yuriy
 * Purpose: Declaration of the class KKSAttributesFactory
 * Comment: ������ ����� �������� �������� ���������
 ***********************************************************************/

#include <QModelIndex>
#include <QMessageBox>
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QStandardItemModel>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QLabel>
#include <QCheckBox>
#include <QGridLayout>
#include <QToolButton>
#include <QDateEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QScrollArea>
#include <QRegExpValidator>
#include <QRegExp>
#include <QColor>
#include <QTreeView>
#include <QAction>
#include <QHeaderView>
#include <QPushButton>
#include <QSpacerItem>
#include <QHBoxLayout>

#include <KKSAttributesEditor.h>
#include <KKSObjEditor.h>
#include <KKSRecDialog.h>
#include <kksfilterseditorform.h>
#include <kksattreditor.h>
#include <KKSCheckBox.h>
#include <KKSAttrRefWidget.h>
#include <KKSAttrUUIDWidget.h>
#include <KKSListWidget.h>
#include <KKSDateEdit.h>
#include <KKSDateTimeEdit.h>
#include <KKSTimeEdit.h>
#include <KKSComplexAttrWidget.h>
#include <KKSEdit.h>
#include <KKSIntervalWidget.h>
#include <KKSText.h>
#include <KKSAttrValueLabel.h>
#include <KKSPixmap.h>
#include <KKSAttrCheckListWidget.h>
#include <KKSHIntervalW.h>
#include <KKSSvgWidget.h>
#include <KKSPointTable.h>
#include <KKSColorWidget.h>
#include <KKSSearchTemplate.h>
#include <KKSVideoPlayer.h>
#include <KKSBinaryWidget.h>

#include <KKSRecWidget.h>
#include <KKSCatAttrsModel.h>
#include <KKSItemDelegate.h>
#include <kksattrattreditor.h>
#include <kkscatattreditor.h>
#include <KKSAValWidget.h>
#include <KKSAttrValuePropsForm.h>
#include <attrhistory.h>

#include <KKSFilter.h>
#include <KKSObject.h>
#include <KKSCategory.h>
#include <KKSObjectExemplar.h>
#include <KKSEIOData.h>
#include <KKSEIODataModel.h>
#include <KKSAttrValue.h>
#include <KKSHistogram.h>

#include "KKSViewFactory.h"
#include "KKSAttributesFactory.h"
#include "KKSObjEditorFactory.h"
#include "KKSLoader.h"
#include "KKSConverter.h"
#include "KKSEIOFactory.h"
#include "KKSPPFactory.h"
#include "KKSHistWidgetEx.h"
#include "KKSMapWidget.h"

#include <defines.h>
#include <kksdatabase.h>
#include <KKSSortFilterProxyModel.h>



/* ����� ��������� ������ � ����������� ���������� ����� ��������� � ���������.
 * ���������:
 * filters -- ������� ���������
 * mode -- �������/���������� ������ OK, Cancel, Apply
 * parent -- ������������ ������
 * f -- ����� Qt.
 */
KKSAttributesEditor * KKSAttributesFactory :: viewAttributes (const KKSList<const KKSFilterGroup *>& filters, bool mode, QWidget * parent, Qt::WFlags f)
{
    KKSAttributesEditor *aEditor = KKSViewFactory::createAttrView (loader, filters, mode, parent, f);
    if (!aEditor)
        return 0;
    aEditor->setWindowModality (mode ? Qt::WindowModal : Qt::NonModal);

    connect (aEditor, SIGNAL (findInAttributes (int, QAbstractItemModel *, KKSAttributesEditor *)), this, SLOT (findAttributes (int, QAbstractItemModel *, KKSAttributesEditor * )) );
    connect (aEditor, SIGNAL (insertAttr (const QModelIndex&, QAbstractItemModel *, KKSAttributesEditor *)), this, SLOT (saveAttribute (const QModelIndex&, QAbstractItemModel*, KKSAttributesEditor *)) );
    connect (aEditor, 
             SIGNAL (updateAttr (int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)), 
             this, 
             SLOT (loadAttribute(int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)) );
    
    connect (aEditor, SIGNAL (deleteAttr (int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)), this, SLOT (delAttribute(int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)) );
    connect (aEditor, SIGNAL (getFieldsOfReference (KKSAttribute *, int, KKSAttrEditor * )), this, SLOT (loadAttrsRefFields (KKSAttribute *, int, KKSAttrEditor *)) );
    connect (aEditor, SIGNAL (getSearchTemplate (KKSAttrEditor * )), this, SLOT (loadSearchTemplates (KKSAttrEditor *)) );
    
    connect (aEditor, SIGNAL (insertAttrGroup (QAbstractItemModel *,const QModelIndex&, KKSAttributesEditor *)), this, SLOT (addAttrGroup (QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)) );
    connect (aEditor, SIGNAL (updateAttrGroup (int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)), this, SLOT (editAttrGroup (int, QAbstractItemModel *, const QModelIndex&,  KKSAttributesEditor *)) );
    connect (aEditor, SIGNAL (deleteAttrGroup (int, QAbstractItemModel *, const QModelIndex&,  KKSAttributesEditor *)), this, SLOT (delAttrGroup (int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)) );

    connect (aEditor, 
             SIGNAL(showAttrsWidget(KKSAttribute *, KKSAttrEditor *)), 
             this, 
             SLOT(showAttrsWidget(KKSAttribute *, KKSAttrEditor *)) );

    connect (aEditor, 
             SIGNAL(addAttribute(KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)), 
             this, 
             SLOT(addAttribute(KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)) );

    connect (aEditor, 
             SIGNAL(editAttribute(int, KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)), 
             this, 
             SLOT(editAttribute(int, KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)) );

    connect (aEditor, 
             SIGNAL(delAttribute(int, KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)), 
             this, 
             SLOT(delAttribute(int, KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)) );
    
    connect (aEditor,
             SIGNAL (updateAttributesModel (QAbstractItemModel *)),
             this,
             SLOT (updateAttrModel (QAbstractItemModel *))
            );

    connect (this, SIGNAL(expandIndex (const QModelIndex&)), aEditor, SLOT (expandAttrInd (const QModelIndex&)) );

    return aEditor;
}

KKSAttributesFactory :: KKSAttributesFactory (KKSLoader *l, KKSEIOFactory *_eiof, KKSObjEditorFactory * _oef, KKSPPFactory * kks_pp)
    : loader (l),
    eiof (_eiof),
    m_oef (_oef),
    m_ppf (kks_pp)
{
}
void KKSAttributesFactory ::setOEF(KKSObjEditorFactory * _oef)
{
    m_oef = _oef;
}

KKSAttributesFactory :: ~KKSAttributesFactory (void)
{
    qDebug () << __PRETTY_FUNCTION__;
}

/* ����� ������������ ����� ��������� � ������.
 * ���������:
 * idAttrs -- ������������� ��������
 * aModel -- ������ ���������. �����: ������ ������ ���� ��������������� ������� ������, � �� QAbstractProxyModel.
 * attrsEditor -- ������ �� ������� ���������.
 */
void KKSAttributesFactory :: findAttributes (int idAttrs, QAbstractItemModel * aModel, KKSAttributesEditor * attrsEditor)
{
    if (idAttrs < 0 || !attrsEditor)
        return;

    KKSObject * o = loader->loadIO(idAttrs, true);
    if(!o)
        return;

    KKSCategory * c = o->category()->tableCategory();
    if(!c){
        o->release();
        return;
    }

    bool forIO = false;
    KKSMap<int, KKSAttribute*> attrs;

    KKSFiltersEditorForm * f = new KKSFiltersEditorForm (c, o->tableName(), attrs, forIO, attrsEditor);
    if (f->exec() == QDialog::Accepted)
    {
        KKSList<KKSTemplate*> tListDb = loader->loadCategoryTemplates (c);
        const KKSTemplate * t = (tListDb.empty() ? new KKSTemplate (c->defTemplate()) : tListDb[0]);
        KKSList<const KKSFilterGroup *> filters;
        filters = f->filters();
        
        // 
        // ���� ���������� ������� ��� ����������� ��, �� �� ���� ���� ��������� ��������� ����������� � �������
        // ��� �������� ������ �������������� ��� �� ����������� ����� �������
        //
        KKSViewFactory::filterAttributesModel (loader, filters, aModel);
        //KKSViewFactory::loadEIOEx (attrsEditor, o, loader, t, attrsEditor->getView (), filters);
        if (tListDb.empty())
            t->release();
    }
    delete f;

    o->release();

}

/* ����� ������������ ���������� ������/����������� �������� � �� � ����� ��� ������ � ������.
 * ���������:
 * cAttr -- �������
 * idType -- ������������� ���� ��������
 * aModel -- ����������� ������ ���������
 * idAttrGroup -- ������������� ������ ���������
 * aEditor -- ������ �� ������� ���������
 */
void KKSAttributesFactory :: saveAttribute (const QModelIndex& parent, QAbstractItemModel* aModel, KKSAttributesEditor *attrEditor)
{
    if (!parent.isValid() || !aModel || !attrEditor || parent.data(Qt::UserRole+USER_ENTITY).toInt() != 0)
        return;

    KKSAttribute *cAttr = new KKSAttribute ();
    if (!cAttr)
        return;

    KKSMap<int, KKSAttrType *> attrTypes=attrEditor->getTypes();
    KKSMap<int, KKSAGroup *> attrsGroups=attrEditor->getAvailableGroups();
    QMap<int, QString> ioRefs=attrEditor->getReferences();
    
    KKSAttrEditor *aEditor = new KKSAttrEditor (cAttr, attrTypes, attrsGroups, ioRefs, attrEditor);
    
    int idAttrGroup = parent.data(Qt::UserRole).toInt();
    aEditor->setGroupId(idAttrGroup);
    
    connect (aEditor, 
             SIGNAL (getReferenceFields (KKSAttribute *, int, KKSAttrEditor *)),
             attrEditor,
             SLOT (getRefFields (KKSAttribute *, int, KKSAttrEditor *)));

    connect (aEditor, 
             SIGNAL (getSearchTemplate (KKSAttrEditor *)),
             attrEditor,
             SLOT (slotGetSearchTemplate (KKSAttrEditor *)) );
    
    connect (aEditor, 
             SIGNAL(showAttrsWidget(KKSAttribute *, KKSAttrEditor *)), 
             attrEditor, 
             SIGNAL(showAttrsWidget(KKSAttribute *, KKSAttrEditor *)) ); //�������� ��� � KKSAttributesFactory
    
    connect (aEditor, 
             SIGNAL(addAttribute(KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)), 
             this, 
             SLOT(addComplexAttr(KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)) );

    connect (aEditor, 
             SIGNAL(editAttribute(int, KKSAttribute *, QAbstractItemModel*, const QModelIndex&, KKSAttrEditor *)), 
             this, 
             SLOT(editComplexAttr(int, KKSAttribute *, QAbstractItemModel*, const QModelIndex&, KKSAttrEditor *)) );

    connect (aEditor, 
             SIGNAL(delAttribute(int, KKSAttribute *, QAbstractItemModel*, const QModelIndex&, KKSAttrEditor *)), 
             this, 
             SLOT(delComplexAttr(int, KKSAttribute *, QAbstractItemModel*, const QModelIndex&, KKSAttrEditor *)) );

    connect (aEditor,
             SIGNAL (loadHistRefs (KKSAttribute *, int, KKSAttrEditor *)),
             this,
             SLOT (loadHistReferences (KKSAttribute *, int, KKSAttrEditor *))
            );
    
    //connect (this, SIGNAL(expandIndex (const QModelIndex&)), aEditor, SLOT (expandAttrInd (const QModelIndex&)) );

    aEditor->checkForHistogramRefs();

    if (aEditor->exec() != QDialog::Accepted)
    {
        cAttr->release();
        return;
    }

    KKSAttrType *aType = aEditor->getAttribute()->type();

    int idNewAttrGroup = aEditor->getGroupId();
    KKSAGroup * aGr = loader->loadAttrGroup(idNewAttrGroup);
    cAttr->setGroup (aGr);
    aGr->release ();

    KKSObjectExemplar * objC = KKSConverter :: attributeToExemplar (loader, cAttr);
    int res;
    bool isInsert (cAttr->id() < 0);
    if (isInsert)
        res = eiof->insertEIO (objC);
    else
        res = eiof->updateEIO (objC);

    if (res != OK_CODE)
    {
        if (aType)
            aType->release ();
        qCritical() << tr ("Cannot save attribute %1").arg (cAttr->title());
        QMessageBox::critical (aEditor, tr ("Save attribute"), tr ("Cannot save attribute %1").arg (cAttr->title()), QMessageBox::Ok);
        return;
    }
    if (isInsert)
        cAttr->setId (objC->id());

    QModelIndex aInd = (isInsert ? QModelIndex() : KKSViewFactory::searchModelRowsIndexMultiType(aModel, cAttr->id(), 1, parent));
    QModelIndex pGroupInd = KKSViewFactory::searchModelRowsIndexMultiType(aModel, idNewAttrGroup, 0);
    if (aInd.isValid() && pGroupInd != parent)
    {
        //
        // ���� ������ ����������, �� ������� ������ �������� � ������ � ������ �����
        //
        int iRow = aInd.row();
        aModel->removeRows (iRow, 1, parent);
        int nr = aModel->rowCount(pGroupInd);
        aModel->insertRows(nr, 1, pGroupInd);
        aInd = aModel->index(nr, 0, pGroupInd);
    }
    else if (!aInd.isValid())
    {
        int nr = aModel->rowCount(pGroupInd);
        bool isIns = aModel->insertRows(nr, 1, pGroupInd);
        if (aModel->columnCount(pGroupInd) == 0 && isIns)
            aModel->insertColumns(0, 4, pGroupInd);
        aInd = aModel->index(nr, 0, pGroupInd);
    }
    aModel->setData(aInd, cAttr->id(), Qt::UserRole);
    aModel->setData(aInd, cAttr->name(), Qt::DisplayRole);
    aModel->setData(aInd, QIcon(":/ddoc/rubric_item.png").pixmap(24, 24), Qt::DecorationRole);
    aModel->setData(aInd, 1, Qt::UserRole+USER_ENTITY);
    
    aInd = aInd.sibling (aInd.row(), 1);
    aModel->setData(aInd, cAttr->type()->name(), Qt::DisplayRole);
    aInd = aInd.sibling (aInd.row(), 2);
    aModel->setData(aInd, cAttr->title(), Qt::DisplayRole);
    QModelIndex nIndex = aInd.sibling (aInd.row(), 3);
    if (cAttr->type()->id() == 2 ||
        cAttr->type()->id() == 7 ||
        cAttr->type()->id() == 12 ||
        cAttr->type()->id() == 17 ||
        cAttr->type()->id() == 19 ||
        cAttr->type()->id() == 26
    )
    {
        KKSObject * io = loader->loadIO (cAttr->tableName(), true);
        QString refVal = QString("%1(%2)").arg ((io ? io->name() : QString())).arg (cAttr->name());
        aModel->setData (nIndex, refVal, Qt::DisplayRole);
        if (io)
            io->release();
    }
    
    m_ppf->insertAttrAttrs(cAttr);

    aInd = aInd.sibling (aInd.row(), 0);
    emit expandIndex (aInd);

    if (aType)
        aType->release ();
}

/* ����� ������������ �������������� �������� �������� �� ��.
 * ���������:
 * idAttr -- ������������� ��������
 * aModel -- ������ ���������
 * aIndex -- ������ �������� � ������ ������������ ��� ������ ������������ ������
 * aEditor -- ������ � ������� ���������
 */
void KKSAttributesFactory :: loadAttribute (int idAttr, QAbstractItemModel * aModel, const QModelIndex& aIndex, KKSAttributesEditor *aEditor)
{
    if (!aModel || !aIndex.isValid())
        return;
    KKSAttribute * attr = loader->loadAttribute (idAttr);
    if (!attr)
        return;

    //KKSCategoryAttr *cAttr = KKSCategoryAttr :: create (attr, false, false, QString());
    //if (!cAttr)
    //    return;

    loader->loadAttrAttrs(attr);

    KKSAttrEditor *attrEditor = new KKSAttrEditor (attr, aEditor->getTypes(), aEditor->getAvailableGroups(), aEditor->getReferences (), aEditor);
    if (attr->type()->attrType () == KKSAttrType::atList || 
        attr->type()->attrType () == KKSAttrType::atParent || 
        attr->type()->attrType () == KKSAttrType::atCheckList ||
        attr->type()->attrType () == KKSAttrType::atCheckListEx ||
        attr->type()->attrType () == KKSAttrType::atRecordColorRef ||
        attr->type()->attrType () == KKSAttrType::atRecordTextColorRef ||
        attr->type()->attrType () == KKSAttrType::atSysChildCategoryRef)
    {
        loadAttrsRefs (attr, attrEditor);
    }
    else if (attr->type()->attrType () == KKSAttrType::atHistogram)
    {
        this->loadHistReferences(attr, KKSAttrType::atHistogram, attrEditor);
    }

    connect (attrEditor, SIGNAL (getReferences (KKSAttribute *, KKSAttrEditor *)),
             this,
             SLOT (loadAttrsRefs (KKSAttribute *, KKSAttrEditor *))
             );

    connect (attrEditor, SIGNAL (getReferenceFields (KKSAttribute *, int, KKSAttrEditor *)),
             this,
             SLOT (loadAttrsRefFields (KKSAttribute *, int, KKSAttrEditor *))
             );
    connect (attrEditor, 
             SIGNAL (getSearchTemplate (KKSAttrEditor *)),
             this,
             SLOT (loadSearchTemplates (KKSAttrEditor *)) );
    connect (attrEditor, 
             SIGNAL(showAttrsWidget(KKSAttribute *, KKSAttrEditor *)), 
             this, 
             SLOT(showAttrsWidget(KKSAttribute *, KKSAttrEditor *)) );

    connect (attrEditor, 
             SIGNAL(addAttribute(KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)), 
             this, 
             SLOT(addComplexAttr(KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)) );

    connect (attrEditor, 
             SIGNAL(editAttribute(int, KKSAttribute *, QAbstractItemModel*, const QModelIndex&, KKSAttrEditor *)), 
             this, 
             SLOT(editComplexAttr(int, KKSAttribute *, QAbstractItemModel*, const QModelIndex&, KKSAttrEditor *)) );

    connect (attrEditor, 
             SIGNAL(delAttribute(int, KKSAttribute *, QAbstractItemModel*, const QModelIndex&, KKSAttrEditor *)), 
             this, 
             SLOT(delComplexAttr(int, KKSAttribute *, QAbstractItemModel*, const QModelIndex&, KKSAttrEditor *)) );
    
    connect (attrEditor,
             SIGNAL (loadHistRefs (KKSAttribute *, int, KKSAttrEditor *)),
             this,
             SLOT (loadHistReferences (KKSAttribute *, int, KKSAttrEditor *))
            );

    connect (this, SIGNAL(expandIndex (const QModelIndex&)), aEditor, SLOT (expandAttrInd (const QModelIndex&)) );

    attrEditor->checkForHistogramRefs();

    QModelIndex pIndex = aIndex;
    while (pIndex.parent().isValid() && pIndex.data(Qt::UserRole+USER_ENTITY).toInt() > 0)
        pIndex = pIndex.parent();
    attrEditor->setGroupId(pIndex.data(Qt::UserRole).toInt());

    QModelIndex aInd (aIndex);
    if (attrEditor->exec () == QDialog::Accepted)
    {
        KKSAttribute *cAttrRes = attrEditor->getAttribute ();

        int idAttrGr = attrEditor->getGroupId ();
        KKSAGroup * aGr = loader->loadAttrGroup(idAttrGr);
        cAttrRes->setGroup (aGr);
        aGr->release ();

        KKSObjectExemplar * oe = KKSConverter::attributeToExemplar (loader, cAttrRes);
        if (!oe)
            return;
        
        int ok = eiof->updateEIO (oe);
        
        //��������� ������������ �������� ������� ��������
        if(ok == OK_CODE){
            m_ppf->insertAttrAttrs(cAttrRes);
        }

        oe->release ();
        QModelIndex pGroupInd = KKSViewFactory::searchModelRowsIndexMultiType(aModel, idAttrGr, 0);
        if (pGroupInd != pIndex)
        {
            //
            // ���� ������ ����������, �� ������� ������ �������� � ������ � ������ �����
            //
            int iRow = aIndex.row();
            aModel->removeRows (iRow, 1, pIndex);
            int nr = aModel->rowCount(pGroupInd);
            aModel->insertRows(nr, 1, pGroupInd);
            aInd = aModel->index(nr, 0, pGroupInd);
        }
        aModel->setData(aInd, cAttrRes->id(), Qt::UserRole);
        aModel->setData(aInd, cAttrRes->name(), Qt::DisplayRole);
        aModel->setData(aInd, QIcon(":/ddoc/rubric_item.png").pixmap(24, 24), Qt::DecorationRole);
        aModel->setData(aInd, 1, Qt::UserRole+USER_ENTITY);

        aInd = aInd.sibling (aInd.row(), 1);
        aModel->setData(aInd, cAttrRes->type()->name(), Qt::DisplayRole);
        aInd = aInd.sibling (aInd.row(), 2);
        aModel->setData(aInd, cAttrRes->title(), Qt::DisplayRole);
        QModelIndex nIndex = aInd.sibling (aInd.row(), 3);
        if (cAttrRes->type()->id() == 2 ||
            cAttrRes->type()->id() == 7 ||
            cAttrRes->type()->id() == 12 ||
            cAttrRes->type()->id() == 17 ||
            cAttrRes->type()->id() == 19 ||
            cAttrRes->type()->id() == 26
        )
        {
            KKSObject * io = loader->loadIO (cAttrRes->tableName(), true);
            QString refVal = QString("%1(%2)").arg ((io ? io->name() : QString())).arg (cAttrRes->name());
            aModel->setData (nIndex, refVal, Qt::DisplayRole);
            if (io)
                io->release();
        }
        aInd = aInd.sibling (aInd.row(), 0);
        emit expandIndex (aInd);

        //KKSViewFactory::updateAttributesModel (loader, aModel);
    }

    if (attrEditor)
    {
        attrEditor->disconnect ();
        delete attrEditor;
    }
    attr->release ();
    //cAttr->release ();
}

/* ����� ������������ �������� �������� �� ��.
 * ���������:
 * idAttr -- ������������� ��������
 * aModel -- ������ ���������
 * aIndex -- ������ �������� � ������
 * aEditor -- ������ � ������� ���������
 */
void KKSAttributesFactory :: delAttribute (int idAttr, QAbstractItemModel * aModel, const QModelIndex& aIndex, KKSAttributesEditor *aEditor)
{
    int row = aIndex.row();//recW->tv->selectionModel ()->currentIndex().row();
    //qDebug () << __PRETTY_FUNCTION__ << row << idAttr;
    KKSAttribute *attr = loader->loadAttribute (idAttr);
    if (!attr)
        return;
    KKSObjectExemplar * oe = KKSConverter::attributeToExemplar (loader, attr);
    if (!oe)
        return;

    if (eiof->deleteEIO(oe) != ERROR_CODE)
        aModel->removeRows (row, 1, aIndex.parent());
    else{
        qWarning() << tr("Attribute %1 is used").arg (attr->title());
        QMessageBox::warning (aEditor, tr("Delete Attribute"), tr("Attribute %1 is used").arg (attr->title()), QMessageBox::Ok);
    }

    if (oe)
        oe->release ();
    if (attr)
        attr->release ();
}

/* ����� ������������ �������� �������� � ������ ��������/�������������� ��������-������ �� ����������
 * ���������:
 * aEditor -- �������� ��������.
 */
void KKSAttributesFactory :: loadSearchTemplates (KKSAttrEditor * aEditor)
{
    if(!m_oef || !loader)
        return;

    QWidget * parent = aEditor;
    
    KKSList<const KKSFilterGroup *> filters;
    KKSObject * o = loader->loadIO(IO_SEARCH_ID, true);
    if(!o)
        return;
    KKSCategory * c = o->category()->tableCategory();
    if(!c)
        return;

    KKSRecDialog *objEditor = m_oef->createObjRecEditor(IO_IO_ID, 
                                                        IO_SEARCH_ID, 
                                                        filters, 
                                                        "",
                                                        o->category(),
                                                        true,
                                                        true,
                                                        Qt::WindowModal,
                                                        parent, 
                                                        Qt::Dialog);

    KKSRecWidget * rw = objEditor->getRecordsWidget();
    disconnect (rw);
    disconnect (objEditor);
    rw->actAdd->disconnect (objEditor);
    rw->actEdit->disconnect (objEditor);
    rw->actDel->disconnect (objEditor);
    connect (rw->actAdd, SIGNAL (triggered()), m_oef, SLOT (addAttrSearchTemplate()) );
    connect (rw->actEdit, SIGNAL (triggered()), m_oef, SLOT (editAttrSearchTemplate()) );
    connect (rw->actDel, SIGNAL (triggered()), m_oef, SLOT (delAttrSearchTemplate()) );
    int idSearchTemplate = 0;
    if (objEditor->exec () == QDialog::Accepted)
    {
        idSearchTemplate = objEditor->getID();
        if(idSearchTemplate <= 0)
        {
            o->release ();
            objEditor->setParent (0);
            delete objEditor;
            return;
        }
    }
    else
    {
        o->release ();
        objEditor->setParent (0);
        delete objEditor;
        return;
    }

    
    KKSSearchTemplate * st = loader->loadSearchTemplate(idSearchTemplate);
    aEditor->setSearchTemplate(st);
    if(st)
        st->release();
    o->release ();
    objEditor->setParent (0);
    delete objEditor;
}

/* ����� ������������ �������� ������� ����������� ��� ��������-������.
 * ���������:
 * attr -- ������� �������
 * idRef -- ������������� ��-�����������
 * aEditor -- �������� ��������
 */
void KKSAttributesFactory :: loadAttrsRefFields (KKSAttribute * attr, int idRef, KKSAttrEditor * aEditor)
{
     if (!attr || !aEditor)
        return;

    KKSObject *io = loader->loadIO (idRef, true);
    if (!io)
        return;

    KKSCategory *c = io->category()->tableCategory ();
    if (!c)
    {
        io->release ();
        return;
    }

    QMap<QString, QString> fields;
    for (KKSMap<int, KKSCategoryAttr*>::const_iterator pc = c->attributes().constBegin(); pc != c->attributes().constEnd(); pc++)
        fields.insert (pc.value()->code(), pc.value()->title ());//name ());

    //qDebug () << __PRETTY_FUNCTION__ << fields;
    aEditor->setIO (io);
    aEditor->uploadReferenceFields (fields);
    io->release ();
}

/* ����� ������������ �������� ������ ������������ ��� ������������ ��������--������.
 * ���������:
 * attr -- ������� �������
 * aEditor -- �������� ��������.
 */
void KKSAttributesFactory :: loadAttrsRefs (KKSAttribute * attr, KKSAttrEditor * aEditor)
{
    QString val_ref = QString("select id from io_categories where id_io_category_type in (1,2,8,9)"); // ,2,8,9
    int attrId = 12;

    KKSList<const KKSFilterGroup *> filterGroupsRefs = KKSViewFactory::AttrRefsFilters (loader, attrId, val_ref);
    QMap<int, QString> io_refs = KKSViewFactory::loadAttrRefs (loader, filterGroupsRefs);
    //qDebug () << __PRETTY_FUNCTION__ << io_refs;

    int ind = -1;
    int i = 0;
    int idRef = -1;
    for (QMap<int, QString>::const_iterator pio = io_refs.constBegin(); pio != io_refs.constEnd() && ind < 0; pio++)
    {
        KKSObject *io = loader->loadIO (pio.key(), true);
        if(!io)
            continue;

        if (io->tableName() == attr->tableName())
        {
            qDebug () << __PRETTY_FUNCTION__ << attr->tableName() << io->id() << i << pio.value() << pio.key();
            ind = i;
            idRef = pio.key();//io->id ();
            aEditor->setCurrentRef (pio.value());//ind);
        }
        i++;
        io->release ();
    }

    if (ind >= 0)
        loadAttrsRefFields (attr, idRef, aEditor);
}

/* ����� ������������ ���������� ����� ������ ��������� � ������.
 * ���������:
 * aModel -- ������ ���������
 * attrEditor -- ������������ ������
 */
void KKSAttributesFactory :: addAttrGroup (QAbstractItemModel *aModel, const QModelIndex& pIndex, KKSAttributesEditor *attrEditor)
{
    if (!aModel || !attrEditor)
        return;

    KKSObject * refIO = loader->loadIO (IO_ATTRS_GROUPS_ID, true);
    if (!refIO)
    {
        qCritical() << tr ("Cannot load referece IO");
        QMessageBox::critical(attrEditor, tr ("Add new group"), tr ("Cannot load referece IO"), QMessageBox::Ok);
        return;
    }
    KKSCategory * c = refIO->category();
    if (c)
        c = c->tableCategory();
    if (!c)
    {
        qCritical() << tr ("Incorrect load referece IO");
        QMessageBox::critical(attrEditor, tr ("Add new group"), tr ("Incorrect load referece IO"), QMessageBox::Ok);
        return;
    }
    KKSList<const KKSFilterGroup *> filters = KKSList<const KKSFilterGroup *>();
    
    KKSMap<qint64, KKSAttrValue *> ioAvals;
    KKSMap<qint64, KKSAttrValue *> aVals;
    if (pIndex.isValid())
    {
        int idGroup = pIndex.data(Qt::UserRole).toInt();
        KKSValue val = KKSValue (QString::number(idGroup), KKSAttrType::atParent);
        KKSAttribute * aPar = loader->loadAttribute(ATTR_ID_PARENT);
        KKSCategoryAttr * cAttr = KKSCategoryAttr::create(aPar, false, true, QString::number(idGroup), 1, QString::null);
        aPar->release();
        KKSAttrValue * av = new KKSAttrValue(val, cAttr);
        if (av)
            aVals.insert(cAttr->id(), av);
        cAttr->release();
        av->release();
    }
    KKSObjEditor * oEditor = m_oef->createObjEditorParam(IO_ATTRS_GROUPS_ID, -1, filters, tr("Add/edit group of attributes"), c, ioAvals, aVals, true, QString(), false, Qt::WindowModal, attrEditor, Qt::Dialog);
    //m_oef->createNewEditorParam(attrEditor, IO_ATTRS_GROUPS_ID, c, refIO->tableName(), 0, (attrEditor->windowModality() != Qt::NonModal), ioAvals, aVals, aModel);
    if (oEditor->exec() == QDialog::Accepted)
    {
        KKSObjectExemplar * pObjEx = oEditor->getObjectEx();
        int pId = pObjEx->attrValue(ATTR_ID_PARENT)->value().value().toInt();
        QString name = pObjEx->attrValue(ATTR_NAME)->value().value();
        QModelIndex parIndex = KKSViewFactory::searchModelRowsIndexMultiType (aModel, pId);
        //qDebug () << __PRETTY_FUNCTION__ << pId << name << parIndex;
        int nr = aModel->rowCount(parIndex);
        aModel->insertRows(nr, 1, parIndex);
        QModelIndex gInd = aModel->index(nr, 0, parIndex);
        int gId = pObjEx->id();
        aModel->setData(gInd, gId, Qt::UserRole);
        aModel->setData(gInd, name, Qt::DisplayRole);
        aModel->setData(gInd, QIcon(":/ddoc/rubric.png").pixmap(24, 24), Qt::DecorationRole);
        aModel->setData(gInd, 0, Qt::UserRole+USER_ENTITY);
        KKSAGroup * agr = loader->loadAttrGroup(gId);
        //qDebug () << __PRETTY_FUNCTION__ << agr->name ();
        attrEditor->addAGroup(agr);
        if (agr)
            agr->release ();
    }
//    m_oef->createNewEditor (attrEditor, IO_ATTRS_GROUPS_ID, c, refIO->tableName(), 0, (attrEditor->windowModality() != Qt::NonModal));
    refIO->release ();
}

/* ����� ������������ �������������� ������ ���������.
 * ���������:
 * idAttrGroup -- ������������� ������ ���������
 * aModel -- ������ ���������
 * aIndex -- ������ ������������� ������
 * attrEditor -- ������������ ������
 */
void KKSAttributesFactory :: editAttrGroup (int idAttrGroup, QAbstractItemModel *aModel, const QModelIndex& aIndex, KKSAttributesEditor *attrEditor)
{
    if (!aModel || !attrEditor)
        return;

    KKSObject * refIO = loader->loadIO (IO_ATTRS_GROUPS_ID, true);
    if (!refIO)
    {
        qCritical() << tr ("Cannot load referece IO");
        QMessageBox::critical(attrEditor, tr ("Edit attributes group"), tr ("Cannot load referece IO"), QMessageBox::Ok);
        return;
    }
    KKSCategory * c = refIO->category();
    if (c)
        c = c->tableCategory();
    if (!c)
    {
        qCritical() << tr ("Incorrect load referece IO");
        QMessageBox::critical(attrEditor, tr ("Edit attributes group"), tr ("Incorrect load referece IO"), QMessageBox::Ok);
        return;
    }
    KKSList<const KKSFilterGroup *> filters = KKSList<const KKSFilterGroup *>();
    KKSObjEditor * oEditor = m_oef->createObjEditor(IO_ATTRS_GROUPS_ID, idAttrGroup, filters, tr("Add/edit group of attributes"), c, true, QString(), false, Qt::WindowModal, attrEditor, Qt::Dialog);
    if (oEditor->exec() == QDialog::Accepted)
    {
        int iRow = aIndex.row();
        QModelIndex pIndex = aIndex.parent();
        aModel->removeRows(iRow, 1, pIndex);
        KKSObjectExemplar * pObjEx = oEditor->getObjectEx();
        int pId = pObjEx->attrValue(ATTR_ID_PARENT)->value().value().toInt();
        QString name = pObjEx->attrValue(ATTR_NAME)->value().value();
        QModelIndex parIndex = KKSViewFactory::searchModelRowsIndexMultiType (aModel, pId);
        //qDebug () << __PRETTY_FUNCTION__ << pId << name << parIndex;
        int nr = aModel->rowCount(parIndex);
        aModel->insertRows(nr, 1, parIndex);
        QModelIndex gInd = aModel->index(nr, 0, parIndex);
        int gId = pObjEx->id();
        aModel->setData(gInd, gId, Qt::UserRole);
        aModel->setData(gInd, name, Qt::DisplayRole);
        aModel->setData(gInd, QIcon(":/ddoc/rubric.png").pixmap(24, 24), Qt::DecorationRole);
        aModel->setData(gInd, 0, Qt::UserRole+USER_ENTITY);
    }
}

/* ����� ������������ �������� ������ ���������.
 * ���������:
 * idAttrGroup -- ������������� ������ ���������
 * aModel -- ������ ���������
 * aIndex -- ������ ������������� ������
 * attrEditor -- ������������ ������
 */
void KKSAttributesFactory :: delAttrGroup (int idAttrGroup, QAbstractItemModel *aModel, const QModelIndex& aIndex, KKSAttributesEditor *attrEditor)
{
    if (!aModel || !attrEditor)
        return;

    KKSObject * refIO = loader->loadIO (IO_ATTRS_GROUPS_ID, true);
    if (!refIO)
        return;
    KKSObjectExemplar * oe = loader->loadEIO(idAttrGroup, refIO);
    if (!oe)
    {
        refIO->release ();
        return;
    }

    int row = aIndex.row();
    if (eiof->deleteEIO(oe) != ERROR_CODE)
    {
        aModel->removeRows (row, 1, aIndex.parent());
        attrEditor->delAGroup(idAttrGroup);
    }
    else{
        qCritical() << tr("Cannot delete group of attributes");
        QMessageBox::critical(attrEditor, tr("Delete group of attributes"), tr("Cannot delete group of attributes"), QMessageBox::Ok);
    }

    if (oe)
        oe->release ();
    if (refIO)
        refIO->release ();
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttributesFactory::createAttrValue(const QString & xml)
// Purpose:    Implementation of KKSAttributesFactory::createAttrValue()
// Parameters:
// - xml
// Return:     KKSAttrValue*
////////////////////////////////////////////////////////////////////////

KKSAttrValue* KKSAttributesFactory::createAttrValue (const QString & xml)
{
    Q_UNUSED (xml);
    return NULL;
    // TODO : implement
}

/* ����� ������������ ��������� ������� �������������� �������� �������� � ������.
 * ���������:
 * av -- �������� ��������
 * objEditor -- �������� �� (���) != 0
 * gLayout -- ������� ������ != 0
 * n_str -- ������ � ��������
 * isSystem -- ���� �������� �� ��������������� ������� ��������� ��� ���
 * tableName -- �������� ������� ��� ���������, ��������� �� �������������
 * idCat -- ������������� ���������
 */
void KKSAttributesFactory :: putAttrWidget (KKSAttrValue* av, 
                                            KKSObjEditor * objEditor, 
                                            QGridLayout *gLayout, 
                                            int n_str, 
                                            KKSIndAttrClass isSystem, 
                                            QString tableName, 
                                            int idCat)
{
    const KKSValue pVal = av->value();
    const KKSCategoryAttr * pCategAttr = av->attribute();
    QVariant V = pVal.valueVariant();

    bool isExist (objEditor ? objEditor->getObjectEx()->id()>0 : false);

    if (!pCategAttr)
    {
        //qDebug () << __PRETTY_FUNCTION__ << "Category attribute is null";
        QMessageBox::warning(objEditor, tr("Set attribute"), tr ("Category attribute is null"), QMessageBox::Ok);
        return;
    }

    const KKSAttrType *pCatType = pCategAttr->type ();
    QToolButton *tbRef = 0;
    QLabel *lTitle = 0;
    QCheckBox *ch = 0;
    QWidget *aw = 0;
    
    if (pCatType->attrType() == KKSAttrType::atCheckListEx){
        if (pCategAttr->id () == ATTR_IO_OBJECTS_ORGANIZATION){
            return;
        }

        QTabWidget * tabEnc = objEditor->getRecTab();
        aw = this->createAttrCheckWidget (av, pCatType, isSystem, tabEnc, objEditor);
    }
    else if(pCatType->attrType() == KKSAttrType::atGISMap)
    {
        //ksa
        QWidget * mapWidget = objEditor->getMapWidget();
        aw = this->createMapWidget(av, pCatType, isSystem, mapWidget, objEditor);
    }
    else{
        aw = this->createAttrWidget (av, objEditor, pCategAttr->isMandatory(), pCatType, isSystem, gLayout, n_str, V, lTitle, tbRef, ch);
    }

    if(aw)
        this->setValue (aw, av, pCatType, isSystem, V, isExist, tableName, idCat, tbRef, ch, objEditor, lTitle);

    return;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttributesFactory::toXML(KKSCategoryAttr* attr)
// Purpose:    Implementation of KKSAttributesFactory::toXML()
// Parameters:
// - attr
// Return:     QString
////////////////////////////////////////////////////////////////////////

QString KKSAttributesFactory :: toXML (KKSCategoryAttr* attr)
{
    Q_UNUSED (attr);
    return QString::null;
}

/* ����� ������� ����� � ��������� ��������.
 * ���������:
 * av -- �������� ��������
 * isSystem -- ��������� (true) ��� ���������������� (false) �������
 * objEditor -- �������� �� (���).
 * ���������:
 * QLabel * c ��������� ��������
 */
QLabel * KKSAttributesFactory :: createAttrTitle (KKSAttrValue * av, KKSIndAttrClass isSystem, KKSObjEditor *objEditor)
{
    if(!av)
        return new QLabel();

    QLabel * lTitle = new KKSAttrValueLabel(av, isSystem);
    
    KKSObjectExemplar * wObjC = objEditor->getObjectEx();

    connect (lTitle, SIGNAL(loadIOSrc(KKSObject **, QWidget * )), this, SLOT(slotLoadIOSrc(KKSObject **, QWidget * )));
    connect (lTitle, SIGNAL(viewIOSrc(KKSObject *, QWidget *)), this, SLOT(viewIOSrc(KKSObject *, QWidget *)));
    if (wObjC && wObjC->id() > 0)
    {
        connect (lTitle, SIGNAL(loadHistory(const KKSAttrValue *, bool)), this, SLOT(loadIOAttrValueHistory(const KKSAttrValue *, bool)));
        connect (this, SIGNAL(viewHistory(const KKSAttrValue *, const KKSList<KKSAttrValue *> &)), lTitle, SLOT(viewAHist(const KKSAttrValue*, const KKSList<KKSAttrValue *> &)));
    }
    else if (objEditor)
    {
        connect (lTitle, SIGNAL(loadHistory(const KKSAttrValue *, bool)), objEditor, SLOT(loadAttrHistory(const KKSAttrValue *, bool)));
        connect (objEditor, SIGNAL (viewHist (const KKSAttrValue *, const KKSList<KKSAttrValue *>&)), lTitle, SLOT(viewAHist(const KKSAttrValue *, const KKSList<KKSAttrValue *> &)));
    }
    connect (lTitle, SIGNAL(viewDetailAttrVal (const KKSAttrValue *, int, int, QWidget *)), this, SLOT(viewAttrValue (const KKSAttrValue *, int, int, QWidget *)) );
    connect (lTitle, SIGNAL(refreshDetailAttrVal (const KKSAttrValue *, int)), this, SLOT (refreshAttrValue (const KKSAttrValue *, int)) );

    if(objEditor)
        connect(lTitle, SIGNAL(attrValueChanged()), objEditor, SLOT(attrValueChanged())); 
    av->release();
	
    lTitle->setMinimumHeight (20);
    QSizePolicy spLabel (QSizePolicy::Expanding, QSizePolicy::Fixed);
    lTitle->setSizePolicy (spLabel);

    return lTitle;
}

/* ����� ������� ������� ��� ��������������� �������� ���� ����-�����.
 * ���������:
 * isMandatory -- �������������� �������� ��������� ������� -- false, �������������� ��������� � ������ � ������������ 0 -- true
 * gLayout -- ������� ������ != 0
 * lTitle -- ���������
 * n_str -- ����� ������
 */
QCheckBox * KKSAttributesFactory :: createChDateTime (bool isMandatory, QGridLayout *gLayout, QLabel *lTitle, int n_str)
{
    QCheckBox * ch = 0;
    if (!isMandatory)
    {
        ch = new QCheckBox ();
        QSizePolicy hPol (QSizePolicy::Fixed, QSizePolicy::Fixed);
        hPol.setHorizontalStretch (0);
        gLayout->addWidget (ch, n_str, 0, 1, 1, Qt::AlignLeft);
        gLayout->addWidget (lTitle, n_str, 1, 1, 1, Qt::AlignRight);
    }
    else
        gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);

    return ch;
}

/* ����� ������� ������ �������� � �������� ��� � ������.
 * ���������:
 * av -- �������� ��������
 * objEditor -- �������� �� (���)
 * is_mandatory -- ������������ ������� (true) ��� ������������ (false)
 * pCatType -- ��� ��������
 * attrClass -- ����� �������� (��������������� ��� ��, ��������� ��� ���, ���������������� ��� ���, ������� � ����������� ��������)
 * gLayout -- ������� ������ != 0
 * n_str -- ����� ������ � �������
 * V -- �������� �������� � ������ QVariant
 * lTitle -- �������� �������� (������������ ��������), 
 * tbRef -- ������ �������� �������� ��� �������� ���������� ���� (������������ ��������),
 * ch -- ������� enable/disable ��� �������������� ��������� (������������ ��������,
 * isRef -- true ��� �������� �������� ��� ��������, �� ������� ��������� ��������-������,
 *          false ��� ���� ��������� �������.
 * ���������:
 * ������, �������������� ��������.
 */
QWidget * KKSAttributesFactory :: createAttrWidget (KKSAttrValue * av, 
                                                    KKSObjEditor *objEditor, 
                                                    bool is_mandatory, 
                                                    const KKSAttrType *pCatType, 
                                                    KKSIndAttrClass attrClass, 
                                                    QGridLayout *gLayout, 
                                                    int n_str, 
                                                    const QVariant& V, 
                                                    QLabel *lTitle, //������������ ��������
                                                    QToolButton *&tbRef,  //������������ ��������
                                                    QCheckBox *&ch, //������������ ��������
                                                    bool isRef)
{
    QWidget * attrWidget = 0;
    if (!pCatType)
        return 0;

    QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    int aType = pCatType->attrType();
   
    switch (aType)
    {
        case KKSAttrType::atUndef: default:
            break;
        case KKSAttrType::atBool:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//pCategAttr->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);
                }
                attrWidget = new KKSCheckBox (av, attrClass);
                attrWidget->setMinimumHeight (20);
                attrWidget->setSizePolicy (hPw);

                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);

            }
            break;
        case KKSAttrType::atList:
        case KKSAttrType::atParent:
        case KKSAttrType::atRecordColorRef:
        case KKSAttrType::atRecordTextColorRef:
        case KKSAttrType::atSysChildCategoryRef:
            {
                const KKSCategoryAttr * pCategAttr = av->attribute();
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, pCategAttr->title(), is_mandatory);
                    Qt::Alignment align=Qt::AlignRight;
                    if (pCategAttr->refType()->attrType() == KKSAttrType::atText )
                        align |= Qt::AlignTop;
                    
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, align);//Qt::AlignRight);
                }

                hPw.setHorizontalStretch (10);
                
                attrWidget = new KKSAttrRefWidget ();
                
                QLabel *l=0;
                QToolButton *tb = 0;
                QCheckBox *chr = 0;

                QVariant vr = QVariant ();
                bool isWRef (pCategAttr->refType()->attrType() != KKSAttrType::atList &&
                             pCategAttr->refType()->attrType() != KKSAttrType::atParent &&
                             pCategAttr->refType()->attrType() != KKSAttrType::atSysChildCategoryRef);
                
                qDebug () << __PRETTY_FUNCTION__ << "Widget creation" << pCategAttr->refType()->attrType() << isWRef << pCategAttr->id();
                
                const KKSAttribute * refAttr = 0;
                if (pCategAttr->refType()->attrType () == KKSAttrType::atList ||
                    pCategAttr->refType()->attrType () == KKSAttrType::atParent ||
                    pCategAttr->refType()->attrType () == KKSAttrType::atSysChildCategoryRef)
                {
                    QString aCode = pCategAttr->columnName ();
                    refAttr = loader->loadAttribute (aCode, pCategAttr->tableName());
                }
                else
                {
                    refAttr = pCategAttr;
                    refAttr->addRef ();
                }

                KKSAttribute * cAttr = 0;
                QWidget *arw = createAttrWidget (av, 
                                                 objEditor, 
                                                 is_mandatory, 
                                                 cAttr ? cAttr->refType() : pCategAttr->refType(), 
                                                 attrClass, 
                                                 qobject_cast<QGridLayout *>(attrWidget->layout ()), 
                                                 0, 
                                                 vr, 
                                                 l, 
                                                 tb, 
                                                 chr, 
                                                 true);//isWRef);

                qDebug () << __PRETTY_FUNCTION__ << "Widget has created" << pCategAttr->refType()->attrType();
                if (!arw)
                    break;
                if (cAttr)
                    cAttr->release ();
                
                qobject_cast<KKSAttrRefWidget *>(attrWidget)->setAttrWidget (arw);
                attrWidget->setMinimumHeight (20);
                
                if (!isRef)
                {
                    tbRef = new QToolButton ();
                    tbRef->setMinimumHeight (20);
                    tbRef->setText ("...");
                    QGridLayout *gLay = new QGridLayout ();
                    
                    int ng = 1;
                    if ( pCategAttr->refType()->attrType() == KKSAttrType::atJPG )
                        gLay->addWidget (attrWidget, 0, 0, ng, 1, Qt::AlignCenter);
                    else
                        gLay->addWidget (attrWidget, 0, 0, ng, 1);
                    
                    gLay->addWidget (tbRef, 0, 1, ng, 1, Qt::AlignTop);
                    
                    if (pCategAttr->refType()->attrType() == KKSAttrType::atText )
                        gLayout->addLayout (gLay, n_str, 2, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
                    else
                        gLayout->addLayout (gLay, n_str, 2, 1, 1);
                }

                attrWidget->setSizePolicy (hPw);
            }
            break;

        case KKSAttrType::atUUID:
            {
                const KKSCategoryAttr * pCategAttr = av->attribute();
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//pCategAttr->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight | Qt::AlignTop);
                }
                hPw.setHorizontalStretch (10);
                attrWidget = new KKSAttrUUIDWidget (isRef);
                
                qDebug () << __PRETTY_FUNCTION__ << "Widget has created" << pCategAttr->type()->attrType();
                attrWidget->setMinimumHeight (20);
                attrWidget->setSizePolicy (hPw);
                
                if (!isRef){
                    attrWidget->setEnabled(false);
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                }
            }
            break;
        case KKSAttrType::atCheckList:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory );
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);
                }
                attrWidget = new KKSListWidget (av, attrClass);
                attrWidget->setMinimumHeight(40);
                attrWidget->setSizePolicy(hPw);
                if (isRef)
                    break;
                QHBoxLayout *hLay = new QHBoxLayout ();
                hLay->addWidget (attrWidget);
                gLayout->addLayout (hLay, n_str, 2, 1, 1);
            }
            break;
        case KKSAttrType::atCheckListEx:
            {
                break;
            }
        case KKSAttrType::atDate:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    ch = this->createChDateTime (is_mandatory, gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                QDate v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QDate::currentDate();
                    //QVariant var(v);
                }
                else
                {
                    QString s = V.toString();
                    v = V.toDate ();
                    QString ss = v.toString("dd.MM.yyyy");
                    int a = 0;
                    Q_UNUSED (a);
                }

                attrWidget = new KKSDateEdit (av, attrClass, v);
                qobject_cast<QDateEdit *>(attrWidget)->setReadOnly (isRef);
                attrWidget->setMinimumHeight (20);
                attrWidget->setSizePolicy (hPw);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                if (ch)
                {
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
            }
            break;
        case KKSAttrType::atDateTime:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    ch = this->createChDateTime (is_mandatory, gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                QDateTime v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QDateTime::currentDateTime();
                    //QVariant var(v);
                }
                else
                    v = V.toDateTime ();

                attrWidget = new KKSDateTimeEdit (av, attrClass, v);
                qobject_cast<QDateTimeEdit *>(attrWidget)->setDisplayFormat("dd.MM.yyyy hh:mm:ss");
                qobject_cast<QDateTimeEdit *>(attrWidget)->setReadOnly (isRef);
                attrWidget->setMinimumHeight (20);
                attrWidget->setSizePolicy (hPw);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                if (ch)
                {
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
            }
            break;
        case KKSAttrType::atDateTimeEx:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    ch = this->createChDateTime (is_mandatory, gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                QDateTime v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QDateTime::currentDateTime();
                    //QVariant var(v);
                }
                else
                    v = V.toDateTime ();

                attrWidget = new KKSDateTimeEdit (av, attrClass, v);
                qobject_cast<QDateTimeEdit *>(attrWidget)->setDisplayFormat("dd.MM.yyyy hh:mm:ss.zzz");
                qobject_cast<QDateTimeEdit *>(attrWidget)->setReadOnly (isRef);
                attrWidget->setMinimumHeight (20);
                attrWidget->setSizePolicy (hPw);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                if (ch)
                {
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
            }
            break;
        case KKSAttrType::atTime:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    ch = this->createChDateTime (is_mandatory, gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                QTime v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QTime::currentTime();
                    //QVariant var(v);
                }
                else
                    v = V.toTime ();

                attrWidget = new KKSTimeEdit (av, attrClass, v);
                qobject_cast<QTimeEdit *>(attrWidget)->setReadOnly (isRef);
                attrWidget->setMinimumHeight (20);
                attrWidget->setSizePolicy (hPw);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                if (ch)
                {
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
            }
            break;
        case KKSAttrType::atDouble:
        case KKSAttrType::atInt:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);
                }
                double v = V.toDouble ();
                int vi = V.toInt ();
                QString val;
                if (pCatType->attrType() == KKSAttrType::atDouble && !V.toString().isEmpty())
                    val = QString::number (v);
                else if (pCatType->attrType() == KKSAttrType::atInt && !V.toString().isEmpty())
                    val = QString::number (vi);
                attrWidget = new KKSEdit (av, attrClass, val);//(pCatType->attrType() == KKSAttrType::atDouble ? QString::number (v) : QString::number (vi)));
                qobject_cast<QLineEdit *>(attrWidget)->setReadOnly (isRef);
                QValidator *dval = 0;
                if (pCatType->attrType() == KKSAttrType::atDouble)
                    dval = new QDoubleValidator (0);
                else
                    dval = new QIntValidator (0);
                qobject_cast <QLineEdit *>(attrWidget)->setValidator (dval);
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                attrWidget->setSizePolicy (hPw);
            }
            break;
        case KKSAttrType::atInt64:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);
                }
                qint64 vi = V.toLongLong ();
                QString val;
                if (!V.toString().isEmpty())
                    val = QString::number (vi);

                attrWidget = new KKSEdit (av, attrClass, val);//(pCatType->attrType() == KKSAttrType::atDouble ? QString::number (v) : QString::number (vi)));
                qobject_cast<QLineEdit *>(attrWidget)->setReadOnly (isRef);
                QValidator * dval = new QIntValidator (0);
                qobject_cast <QLineEdit *>(attrWidget)->setValidator (dval);
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                attrWidget->setSizePolicy (hPw);
            }
            break;
        case KKSAttrType::atString: 
        case KKSAttrType::atFixString: 
        case KKSAttrType::atUrl:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);
                }
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                attrWidget = new KKSEdit (av, attrClass, v);
                QLineEdit * lE = qobject_cast<QLineEdit *>(attrWidget);
                if (QString::compare (av->attribute()->code(false), QString("email_prefix"), Qt::CaseInsensitive) == 0)
                {
                    QRegExp rVal (QString("^([A-Za-z0-9_])+"));
                    QRegExpValidator * regVal = new QRegExpValidator (rVal, lE);
                    lE->setValidator (regVal);
                }
                lE->setReadOnly (isRef);
                if (pCatType->attrType() == KKSAttrType::atFixString)
                    qobject_cast<KKSEdit*>(attrWidget)->setFixedSymCount(av->attribute()->defWidth());
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                attrWidget->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
            }
            break;
        case KKSAttrType::atComplex:
            {
                loader->loadAttrAttrs(av->attribute());

                if(!av->attrsValuesLoaded()){
                    KKSMap<qint64, KKSAttrValue*> aav_list;
                    if(attrClass == iacIOUserAttr){
                        aav_list = loader->loadAttrAttrValues(av, false);
                    }
                    if(attrClass == iacEIOUserAttr){
                        aav_list = loader->loadAttrAttrValues(av, true);
                    }
                    if(attrClass == iacAttrAttr ){
                        //aav_list = loader->loadAttrAttrValues(av->id(), false);
                    }
                    if(attrClass == iacEIOSysAttr){
                        //aav_list = loader->loadAttrAttrValues(av->id(), false);
                    }
                    if(attrClass == iacTableAttr){
                        //aav_list = loader->loadAttrAttrValues(av->id(), false);
                    }

                    av->setAttrsValues(aav_list);
                }

                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 2, 2, Qt::AlignRight | Qt::AlignVCenter);
                }

                attrWidget = new KKSComplexAttrWidget (av, attrClass, objEditor);
                connect(attrWidget, 
                        SIGNAL(putAttrAttrOnWidget(KKSAttrValue*, KKSObjEditor*, QGridLayout*, int, KKSIndAttrClass, QString, int)), 
                        this, 
                        SLOT(putAttrWidget (KKSAttrValue*, KKSObjEditor*, QGridLayout*, int, KKSIndAttrClass, QString, int)));

                (qobject_cast<KKSComplexAttrWidget*>(attrWidget))->init();
                //qobject_cast<KKSComplexAttrWidget *>(attrWidget)->setFixedSymCount (av->attribute()->defWidth());
                //qobject_cast<QTextEdit *>(attrWidget)->setReadOnly (isRef);

                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignVCenter);

                attrWidget->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);                
            }

            break;

        case KKSAttrType::atInterval:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    ch = this->createChDateTime (is_mandatory, gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::Expanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                attrWidget = new KKSIntervalWidget (av, attrClass);
                attrWidget->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
                
                QString v = V.toStringList().join(" ");
                QStringList vl = v.split (" ");
                int vi = vl[0].toInt();
                QIntValidator *dVal = new QIntValidator( 1, 24, 0);
                QLineEdit *lEdit = new QLineEdit (QString::number (vi));
                
                lEdit->setMinimumHeight (20);
                lEdit->setValidator (dVal);
                QSizePolicy lPw (QSizePolicy::Expanding, QSizePolicy::Fixed, QSizePolicy::LineEdit);
                lPw.setHorizontalStretch (3);
                lEdit->setSizePolicy (lPw);
                qobject_cast<KKSIntervalWidget*>(attrWidget)->setLineEdit (lEdit);

                QComboBox *cbUnit = new QComboBox ();
                QSizePolicy cPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed, QSizePolicy::ComboBox);
                cPw.setHorizontalStretch (1);
                cbUnit->setSizePolicy (cPw);
                cbUnit->setMinimumHeight (20);
                
                cbUnit->addItem (QString(), QVariant());
                
                QStringList units = KKSAttrType::getIntervalUnits();
                for (int i=0; i<units.count(); i++)
                    cbUnit->addItem (units[i], i);
                if (vl.size()>1)
                {
                    int ind = cbUnit->findText (vl[1], Qt::MatchContains);
                    if (ind>0)
                        cbUnit->setCurrentIndex (ind);
                }
                cbUnit->setSizeAdjustPolicy (QComboBox::AdjustToContentsOnFirstShow);
                qobject_cast<KKSIntervalWidget*>(attrWidget)->setComboUnits (cbUnit);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignTop);
                if (ch)
                {
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
            }
            break;
        case KKSAttrType::atIntervalH:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    ch = this->createChDateTime (is_mandatory, gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::Expanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                attrWidget = new KKSHIntervalW (av, attrClass);
                attrWidget->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
                QString v = V.toStringList().join(" ");
                QStringList vl = v.split (" ");
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignTop);
                if (ch)
                {
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
            }
            break;
        case KKSAttrType::atText:
        case KKSAttrType::atXMLDoc:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 2, 2, Qt::AlignRight | Qt::AlignTop);
                }
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                attrWidget = new KKSText (av, attrClass, v);
                qobject_cast<KKSText *>(attrWidget)->setFixedSymCount (av->attribute()->defWidth());
                qobject_cast<QTextEdit *>(attrWidget)->setReadOnly (isRef);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                attrWidget->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
            }
            break;
        
        case KKSAttrType::atGeometry:
        //case KKSAttrType::atGeometryPoly:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 2, 2, Qt::AlignRight | Qt::AlignTop);
                }
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                attrWidget = new KKSText (av, attrClass, v);
                qobject_cast<KKSText *>(attrWidget)->setFixedSymCount (av->attribute()->defWidth());
                qobject_cast<QTextEdit *>(attrWidget)->setReadOnly (true);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                attrWidget->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
            }
            break;
        
        case KKSAttrType::atHistogram:
            {
                //ksa
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 2, 2, Qt::AlignRight | Qt::AlignCenter);
                }
                
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");

                attrWidget = new KKSHistWidgetEx(av, attrClass);
                
                KKSHistWidgetEx * haw = qobject_cast <KKSHistWidgetEx *>(attrWidget);

                //��������
                QMap<int, QString> scList;
                if(av->attribute()->tableName() == "histogram_params_chains"){                
                    scList = loader->getScenariosInfoForChains();
                }
                else if(av->attribute()->tableName() == "histogram_params_streams"){
                    scList = loader->getScenariosInfoForStreams();
                }
                haw->loadScenario (scList);
                
                //��������
                QMap<int, QString> vList;
                if(av->attribute()->tableName() == "histogram_params_chains"){                
                    vList = loader->getVariantsInfoForChains();
                }
                else if(av->attribute()->tableName() == "histogram_params_streams"){
                    vList = loader->getVariantsInfoForStreams();
                }
                haw->loadVariants (vList);

                //���������
                QMap<int, QString> cList;
                if(av->attribute()->tableName() == "histogram_params_chains"){
                    cList = loader->getCatInfoForChains();
                }
                else if(av->attribute()->tableName() == "histogram_params_streams"){
                    cList = loader->getCatInfoForStreams();
                }
                haw->loadCategories(cList);

                //��
                QMap<int, QString> ioList;
                if(av->attribute()->tableName() == "histogram_params_chains"){
                    //ioList = loader->getIOInfoForChains();
                }
                else if(av->attribute()->tableName() == "histogram_params_streams"){
                    ioList = loader->getIOInfoForStreams();
                }
                haw->loadIOList(ioList);

                //PartitionLows
                QMap<int, QString> plList;
                if(av->attribute()->tableName() == "histogram_params_streams"){
                    plList = loader->getPartLowInfoForStreams();
                }
                haw->loadPartLows(plList);

                //Services
                QMap<int, QString> sList;
                if(av->attribute()->tableName() == "histogram_params_chains"){
                    sList = loader->getServicesInfoForChains();
                }
                haw->loadServices(sList);

                haw->completed(true);

                //connect (haw, SIGNAL (loadCategory (int, KKSHistogram *)), objEditor, SLOT (loadHistCat (int, KKSHistogram *)) );
                //connect (haw, SIGNAL (loadIO (int, KKSHistogram *)), objEditor, SLOT (loadHistIO (int, KKSHistogram *)) );
                connect (objEditor, SIGNAL (needToUpdateHistogram (KKSValue &)), haw, SLOT (saveHist(KKSValue &)) );
                connect (objEditor, SIGNAL (updateHistogramGraphic()), haw, SLOT(needToUpdateHistogramGraphic()));
                connect (haw, SIGNAL (getIdForHistogramParams(const QString &, qint64 *)), this, SLOT(getIdForHistogramParams(const QString &, qint64 *)) );
                connect (haw, SIGNAL (getHistogramGraphic(KKSHistogram &, const QString &)), this, SLOT(getHistogramGraphic(KKSHistogram &, const QString &)));
                connect (haw, SIGNAL (loadHistogramData(int, const QString &, QWidget *)), this, SLOT(loadHistogramData(int, const QString &, QWidget *)));
               
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                {
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                    attrWidget->setSizePolicy (hPw);
					//((KKSHistWidgetEx*)attrWidget)->init();
                }   //break;
            }
            break;
        case KKSAttrType::atGISMap:
            {
                //ksa
                break;
            }
        case KKSAttrType::atJPG:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);
                }
                
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");

                attrWidget = new KKSPixmap (av, attrClass, v);
                QToolButton *tbRef = new QToolButton ();
                tbRef->setMinimumHeight (20);
                tbRef->setText ("...");
                //--px->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
                //px->setSizeAdjustPolicy (QComboBox::AdjustToMinimumContentsLength);
                //--QHBoxLayout *hLay = new QHBoxLayout ();
                //--hLay->addWidget (px);
                //--hLay->addWidget (tbRef);

                //--gLayout->addLayout (hLay, n_str, 2, 1, 1);
                if (!isRef)
                {
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignCenter);
                    gLayout->addWidget (tbRef, n_str, 3, 1, 1);
                
                    QObject::connect (tbRef, SIGNAL(pressed()), attrWidget, SLOT(openFile()));
                }
            }
            break;
        case KKSAttrType::atSVG:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 1, Qt::AlignRight);
                }

                QByteArray v = V.toByteArray ();
                if (v.isNull())
                {
                    v = QByteArray();
                    attrWidget = new KKSSvgWidget (av, attrClass);
                }
                else
                    attrWidget = new KKSSvgWidget (av, attrClass, v);//, gLayout->parentWidget());
                QToolButton *tbRef = new QToolButton ();
                tbRef->setMinimumHeight (20);
                tbRef->setText ("...");
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                {
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 2, Qt::AlignCenter);
                    gLayout->addWidget (tbRef, n_str, 4, 1, 1);
                
                    QObject::connect (tbRef, SIGNAL(pressed()), attrWidget, SLOT(openFile()));
                }
            }
            break;
        case KKSAttrType::atRecordColor:
        case KKSAttrType::atRecordTextColor:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2);//, Qt::AlignRight | Qt::AlignTop);
                }
                unsigned int vlc = V.toUInt ();
                QRgb rgb_col (vlc);// = V.value<QColor>();//toUInt ();
                QColor rgb_color = QColor::fromRgba (rgb_col);//V.value<QColor>();//toInt ();
                if (V.toString().isEmpty())
                    rgb_color =  QColor ();
                //qDebug () << __PRETTY_FUNCTION__ << rgb_color << vlc << V;
                attrWidget = new KKSColorWidget (av, attrClass, rgb_color, av->attribute()->type()->attrType());
                if (isRef)
                    qobject_cast<KKSColorWidget *>(attrWidget)->hideToolButton ();
                QSizePolicy hPwt (QSizePolicy::Expanding, QSizePolicy::Fixed);//Expanding);
                attrWidget->setSizePolicy (hPwt);
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);//, Qt::AlignJustify);//Qt::AlignCenter);
            }
            break;
        case KKSAttrType::atVideo:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2);//, Qt::AlignRight | Qt::AlignTop);
                }
                attrWidget = new KKSVideoPlayer (av, attrClass);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);//, Qt::AlignJustify);//Qt::AlignCenter);
                QToolButton *tbRef = new QToolButton ();
                tbRef->setMinimumHeight (20);
                tbRef->setText ("...");
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                {
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 2, Qt::AlignCenter);
                    gLayout->addWidget (tbRef, n_str, 4, 1, 1);

                    QObject::connect (tbRef, SIGNAL(pressed()), attrWidget, SLOT(openFile()));
                }
            }
            break;
        case KKSAttrType::atBinary:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, attrClass, objEditor);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2);//, Qt::AlignRight | Qt::AlignTop);
                }
                qDebug () << __PRETTY_FUNCTION__ << QString("Binary data");
                attrWidget = new KKSBinaryWidget (av, attrClass);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);//, Qt::AlignJustify);//Qt::AlignCenter);
                QToolButton *tbRef = new QToolButton ();
                tbRef->setMinimumHeight (20);
                tbRef->setText ("...");
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                {
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 2, Qt::AlignCenter);
                    gLayout->addWidget (tbRef, n_str, 4, 1, 1);

                    QObject::connect (tbRef, SIGNAL(pressed()), attrWidget, SLOT(openFile()));
                }
                break;
            }
    }
    return attrWidget;
}

/* ����� ������� ������ �������� ����������� ���� ������-��-������.
 * ���������:
 * av -- �������� ��������
 * pCatType -- ��� ����, �� ������� ���������
 * isSystem -- ��������� ������� ��� ���
 * tabW -- ������� QTabWidget
 * ���������:
 * ������ ��������, ����������� � tabW.
 */
QWidget * KKSAttributesFactory :: createAttrCheckWidget (const KKSAttrValue * av, 
                                                         const KKSAttrType *pCatType,  
                                                         KKSIndAttrClass isSystem, 
                                                         QTabWidget * tabW, 
                                                         KKSObjEditor * objEditor)
{
    QWidget * attrWidget = 0;
    if (!av || !pCatType || !tabW)
        return 0;

    if (pCatType->attrType() != KKSAttrType::atCheckListEx)
        return 0;

    qDebug () << __PRETTY_FUNCTION__ << av->id();
    attrWidget = new KKSAttrCheckWidget (av, isSystem);//KKSListWidget (pCategAttr, isSystem);
    attrWidget->setMinimumHeight(40);
    QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    attrWidget->setSizePolicy(hPw);
    tabW->addTab (attrWidget, av->attribute()->title());
    QLabel * lHist = qobject_cast<KKSAttrCheckWidget *>(attrWidget)->getHistLabel();
    if (lHist)
    {
        connect (lHist, SIGNAL(loadIOSrc(KKSObject **, QWidget * )), this, SLOT(slotLoadIOSrc(KKSObject **, QWidget * )));
        connect (lHist, SIGNAL(viewIOSrc(KKSObject *, QWidget *)), this, SLOT(viewIOSrc(KKSObject *, QWidget *)));
        KKSObjectExemplar * wObjC = objEditor->getObjectEx();
        if (wObjC && wObjC->id() > 0)
        {
            connect (lHist, SIGNAL(loadHistory(const KKSAttrValue *, bool)), this, SLOT(loadIOAttrValueHistory(const KKSAttrValue *, bool)));
            connect (this, SIGNAL(viewHistory(const KKSAttrValue *, const KKSList<KKSAttrValue *> &)), lHist, SLOT(viewAHist(const KKSAttrValue *, const KKSList<KKSAttrValue *> &)));
        }
        else if (objEditor)
        {
            connect (lHist, SIGNAL(loadHistory(const KKSAttrValue *, bool)), objEditor, SLOT(loadAttrHistory(const KKSAttrValue *, bool)));
            connect (objEditor, SIGNAL (viewHistory (const KKSAttrValue *, const KKSList<KKSAttrValue *>&)), lHist, SLOT(viewAHist(const KKSAttrValue *, const KKSList<KKSAttrValue *> &)));
//            connect (objEditor, SIGNAL(loadHistory(const KKSAttrValue *, qint64, qint64, bool)), this, SLOT(loadIOAttrValueHistoryR(const KKSAttrValue *, qint64, qint64, bool)));
//            connect (this, SIGNAL(viewHistory(const KKSList<KKSAttrValue *> &)), objEditor, SLOT(viewAHist(const KKSList<KKSAttrValue *> &)));
        }
        connect (lHist, SIGNAL(viewDetailAttrVal (const KKSAttrValue *, int, int, QWidget *)), this, SLOT(viewAttrValue (const KKSAttrValue *, int, int, QWidget *)) );
        connect (lHist, SIGNAL(refreshDetailAttrVal (const KKSAttrValue *, int)), this, SLOT (refreshAttrValue (const KKSAttrValue *, int)) );
    }
    return attrWidget;
}

/* ����� ������� ������ �������� ���� ��������� ����, ��������� ����, �����.
 * ���������:
 * av -- �������� ��������
 * pCatType -- ��� ����, �� ������� ���������
 * isSystem -- ��������� ������� ��� ���
 * tabW -- ������� QTabWidget
 * ���������:
 * ������ ��������, ����������� � tabW.
 */
//ksa
QWidget * KKSAttributesFactory :: createMapWidget (const KKSAttrValue * av, 
                                                   const KKSAttrType *pCatType,  
                                                   KKSIndAttrClass isSystem, 
                                                   QWidget * parent, 
                                                   KKSObjEditor * objEditor)
{
    KKSMapWidget * attrWidget = 0;
    if (!av || !pCatType || !parent)
        return 0;

    if (pCatType->attrType() != KKSAttrType::atGISMap)
    {
        return 0;
    }


    qint64 idObj = -1;
    if(objEditor->getObj())
        idObj = objEditor->getObj()->id();
    else
        idObj = objEditor->getObjectEx()->id();

    //QVBoxLayout * vBoxLayout = new QVBoxLayout();
    //parent->setLayout(vBoxLayout);

    //������������ ��� �������� ������� ��� ������� URI postgis-�����
    QMap<QString, QString> connectionParams;
    connectionParams.insert("dbname", loader->getDb()->getName());
    connectionParams.insert("host", loader->getDb()->getHost());
    connectionParams.insert("port", loader->getDb()->getPort());
    connectionParams.insert("user", loader->getDb()->getUser());
    connectionParams.insert("password", loader->getDb()->getPass());
    
    attrWidget = new KKSMapWidget(connectionParams, idObj, m_GISHomeDir, av, isSystem, parent);
    
    connect(attrWidget, SIGNAL(downloadGISFiles(bool, const QString &, qint64, QWidget *)), m_oef, SLOT(slotDownloadGISFiles(bool, const QString &, qint64, QWidget *)));
    connect(objEditor, SIGNAL(uploadGISFiles(qint64)), attrWidget, SLOT(slotUploadGISFiles(qint64)));//� ��������� �� ������ �� ������ "���������". ��������� ��������� ���-����� �� ������
    connect(objEditor, SIGNAL(needToSaveGISProject(KKSValue &)), attrWidget, SLOT(slotSaveGISProject(KKSValue &)));//��������� ������ � ���������� ��� XML � ���� �������� KKSValue
    connect(attrWidget, SIGNAL(uploadGISFiles(bool, const QStringList &, qint64, QWidget *)), m_oef, SLOT(slotUploadGISFiles(bool, const QStringList &, qint64, QWidget *)));
    connect(attrWidget, SIGNAL(signalShowIOEditor(QWidget *, const QString &)), m_oef, SLOT(slotShowIOEditor(QWidget *, const QString &)));
    connect(attrWidget, SIGNAL(constructNewEIO(QWidget *, KKSObjectExemplar **, const QString &, const QString &)), m_oef, SLOT(slotConstructNewEIO(QWidget *, KKSObjectExemplar **, const QString &, const QString &)));//������ �� �������� ������ E�� � �����������, ������� �������� ��������� ����� ������� � ��. 
    connect(attrWidget, SIGNAL(deleteEIOList(QWidget *, const QString &, const QList<qint64> &)), m_oef, SLOT(slotDeleteEIOList(QWidget *, const QString &, const QList<qint64> &)));//������ �� �������� ������ �������� �����, ������� �������� �������� � ����������� DynamicDocs. 
    
    //����� ��� ��������������� ���������� ����� ��� ��������� ������ � �� (����� notify)
    connect(loader, SIGNAL(databaseNotifyReceived(const QString&, const QString&, const QString&)), attrWidget, SLOT(slotUpdateMapByNotify(const QString&, const QString&, const QString&)));

    attrWidget->init();

    
    //delete attrWidget;
    //connect(attrWidget, SIGNAL(downloadFile(const QString&, const QString&, qint64)), this, SIGNAL(downloadFileForGIS(const QString&, const QString&, qint64)));
    //connect(attrWidget, SIGNAL(downloadFile(KKSFile*, QWidget *)), m_oef, SLOT(slotDownloadFile(KKSFile*, QWidget*)));    
    //delete attrWidget;
    //attrWidget = 0;


    
    if(attrWidget){
        //vBoxLayout->addWidget(attrWidget, 0);
        //vBoxLayout->setMargin(0);
        
        //attrWidget->setMinimumHeight(40);
        //QSizePolicy hPw (QSizePolicy::Expanding, QSizePolicy::Expanding);
        //attrWidget->setSizePolicy(hPw);


        QDockWidget * legendWidget = attrWidget->legendWidget();
        QDockWidget * layerOrderWidget = attrWidget->layerOrderWidget();
        if(legendWidget && layerOrderWidget){
            connect(attrWidget, SIGNAL(aboutToDestroy(QDockWidget *, QDockWidget *)), this, SIGNAL(mapAboutToDestroy(QDockWidget *, QDockWidget *)));
            emit mapCreated(legendWidget, layerOrderWidget);
        }
    }
    
    return attrWidget;
}


/* ����� �������������� ������ ��������
 * ���������:
 * aw -- ������ ��������
 * av -- �������� ��������
 * pCatType -- ��� ��������
 * isSystem -- ��������� �������
 * V -- �������� �������� � ������ QVariant
 * isObjExist -- ��(���) ���������� (true) ��� ����� ���������
 * tableName -- �������� �������, �� ������� ��������� �������
 * idCat -- ������������� ���������
 * tbRef -- ������ �������� �������� ��� �������� ���������� ����,
 * ch -- ������� enable/disable ��� �������������� ���������,
 * wEditor -- �������� �� (���)
 * lTitle -- ����� � ��������� ��������
 * isRef -- true ��� �������� �������� ��� ��������, �� ������� ��������� ��������-������,
 *          false ��� ���� ��������� �������.
 */
void KKSAttributesFactory :: setValue (QWidget *aw, 
                                       const KKSAttrValue * av, 
                                       const KKSAttrType *pCatType, 
                                       KKSIndAttrClass isSystem, 
                                       const QVariant& V, 
                                       bool isObjExist, 
                                       QString tableName, 
                                       int idCat, 
                                       QToolButton *tbRef, 
                                       QCheckBox *ch, 
                                       QWidget *wEditor, 
                                       QLabel *lTitle, 
                                       bool isRef)
{
    KKSObjEditor *objEditor = qobject_cast <KKSObjEditor *>(wEditor);

    if (isRef)
        qDebug () << __PRETTY_FUNCTION__ << pCatType->attrType();
    else
        qDebug () << __PRETTY_FUNCTION__ << V;

    switch (pCatType->attrType())
    {
        case KKSAttrType::atUndef: default:
            break;
        case KKSAttrType::atBool:
            {
                bool v = V.toBool ();
                qobject_cast<QCheckBox *>(aw)->setCheckState ( (v ? Qt::Checked : Qt::Unchecked) );
                qobject_cast<KKSCheckBox *>(aw)->setState ( (v ? Qt::Checked : Qt::Unchecked) );
                aw->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
                connectToSlots (aw, wEditor);
            }
            break;
        case KKSAttrType::atList: 
        case KKSAttrType::atParent:
        case KKSAttrType::atRecordColorRef:
        case KKSAttrType::atRecordTextColorRef:
        case KKSAttrType::atSysChildCategoryRef:
            {
                KKSAttrRefWidget *aRefW = qobject_cast<KKSAttrRefWidget *>(aw);
                if (!aRefW)
                    break;
                if (objEditor && !isRef)
                    objEditor->addListAttrWidget (tbRef, aRefW, av);

                int idUser = loader->getUserId ();
                int idMaclabel = loader->getCurrMacLabelId();
                int idOwnerOrg = loader->getOrgId();
                QVariant cV (V);

                //
                // ����� ��������� ����� ��, 
                //
//                qDebug () << __PRETTY_FUNCTION__ << tableName << V;
                if ((tableName == "io_objects" && av->attribute()->tableName() == "users" ) && V.toInt() <= 0)
                    cV = QString::number (idUser);
                else if ((tableName == "io_objects" && av->attribute()->tableName() == "io_categories") && V.toInt() <= 0)
                    cV = QString::number (idCat);
                else if ((tableName == "io_objects" && av->attribute()->tableName() == "organization") && V.toInt() <= 0)
                    cV = QString::number (idOwnerOrg);
                else if ((tableName == "maclabels" || av->attribute()->tableName() == "maclabels") && V.toInt() <= 0)
                    cV = QString::number (idMaclabel);
                
                QMap<int, QString> values;
                QMap<int, QString> refColumnValues;

                if (pCatType->attrType() != KKSAttrType::atParent )
                {
                    QString tName = av->attribute()->tableName ();
                    KKSObject * refObj = loader->loadIO (tName, true);
                    if (!refObj)
                        break;

                    KKSCategory * cRef = refObj->category();
                    if (!cRef)
                    {
                        refObj->release ();
                        break;
                    }
                    bool isXml = false;
                    cRef = cRef->tableCategory();
                    if (cRef)
                        isXml = isXml || cRef->isAttrTypeContains(KKSAttrType::atXMLDoc) || cRef->isAttrTypeContains (KKSAttrType::atSVG);
                    refObj->release ();
                    
                    values = loader->loadAttributeValues (av->attribute(), 
                                                          refColumnValues,
                                                          isXml, 
                                                          !isXml, 
                                                          QString::null, 
                                                          KKSList<const KKSFilterGroup*>());
                }
                else{
                    values = loader->loadAttributeValues (av->attribute(), 
                                                          refColumnValues,
                                                          true, 
                                                          true, 
                                                          (tableName.isEmpty() ? av->attribute()->tableName() : tableName),
                                                          KKSList<const KKSFilterGroup*>());
                }

                QString v_str;
                
                
                int iVal = 0;
                if(av->attribute()->refColumnName().isEmpty() || 
                   av->attribute()->refColumnName() == "id")
                {
                    bool ok = false;
                    iVal = cV.toString().toInt(&ok);
                    if(!ok){//???? ?? ?????????????. ??? ????????, ??? ???????? ? ?????????? ?? ??????.
                            //?? ?????? ?????? ?????????? ???????? id ?? refcolumnvalues. 
                        int id = refColumnValues.key(cV.toString());
                        if (id > 0)
                            iVal = id;
                    }
                }
                else{//???????, ??? ???? ?????? ???? ? refColumnValues
                    int id = refColumnValues.key(cV.toString());
                    if (id > 0)
                        iVal = id;
                }
                
                QMap<int, QString>::const_iterator pv = values.constFind (iVal);
                if (pv != values.constEnd())
                    v_str = pv.value();

                if (!isRef)
                {
                    connectToSlots (aRefW, wEditor);
                    aRefW->setValue (av->id(), isSystem, cV);
                }
                
                QVariant vRef;
                if (pCatType->attrType() == KKSAttrType::atParent || 
                    pCatType->attrType() == KKSAttrType::atList || 
                    pCatType->attrType() == KKSAttrType::atSysChildCategoryRef
                    )
                    vRef = QVariant (v_str);
                else if (!v_str.isEmpty())
                    vRef = QVariant (QColor::fromRgba (v_str.toUInt()));
                else
                    vRef = QVariant (QColor());
                
                setValue (aRefW->getAttrWidget(), 
                          av, 
                          av->attribute()->refType(), 
                          isSystem, 
                          vRef, 
                          isObjExist, 
                          tableName, 
                          idCat, 
                          tbRef, 
                          ch, 
                          wEditor, 
                          lTitle, 
                          true);

                if (!isRef)
                {
                    if (objEditor && tbRef )
                        QObject::connect (tbRef, SIGNAL (clicked()), objEditor, SLOT (setList()) );
                    if (tbRef)
                        tbRef->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
                }
            }
            break;
        case KKSAttrType::atCheckList:
            {
                QMap<int, QString> refValues;
                QMap<int, QString> values = loader->loadAttributeValues (av->attribute(), refValues);

                QStringList vArray = V.toStringList();
                for (QMap<int, QString>::const_iterator pv = values.constBegin(); pv!=values.constEnd(); pv++)
                {
                    QString v = pv.value();
                    int key = pv.key();
                    QVariant var = QVariant (key);
                    QListWidgetItem * item = new QListWidgetItem (v, qobject_cast<QListWidget *>(aw));
                    
                    if (vArray.contains(QString::number(key)))
                        item->setCheckState(Qt::Checked);
                    else
                        item->setCheckState(Qt::Unchecked);
                    
                    item->setData (Qt::UserRole, var);
                }
                aw->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
                if (!isRef)
                    connectToSlots (aw, wEditor);
            }
            break;
        case KKSAttrType::atCheckListEx:
            {
                KKSAttrCheckWidget * arw = qobject_cast<KKSAttrCheckWidget *>(aw);
                if (!arw)
                    break;
                QAbstractItemModel * sAttrModel = aValComplexModel(av);
                QObject :: connect (arw, 
                                    SIGNAL (addAttrRef (const KKSAttrValue*,  KKSIndAttrClass, QAbstractItemModel*)), 
                                    wEditor, 
                                    SLOT (addAttributeCheckReference (const KKSAttrValue*, KKSIndAttrClass, QAbstractItemModel *)) 
                                    );
                
                QObject :: connect (arw,
                                    SIGNAL (delAttrRef (const KKSAttrValue*, KKSIndAttrClass, QAbstractItemModel*, const QModelIndex&)), 
                                    wEditor, 
                                    SLOT (delAttributeCheckReference (const KKSAttrValue*, KKSIndAttrClass, QAbstractItemModel*, const QModelIndex&)) 
                                    );
                
                QObject :: connect (arw, 
                                    SIGNAL (refIOOpen (QString)), 
                                    wEditor, 
                                    SLOT (openReferenceIO (QString))
                                    );
                
                QObject :: connect (arw, 
                                    SIGNAL (refRecOpen (QString, qint64)), 
                                    wEditor, 
                                    SLOT (openReferenceRec (QString, qint64))
                                    );

                arw->setModel (sAttrModel);
                aw->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
                if (!isRef)
                    connectToSlots (aw, wEditor);
            }
            break;
        case KKSAttrType::atDate:
            {
                QDate v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QDate::currentDate();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, var);
                }
                else if (!av->attribute()->isMandatory() && V.isNull())
                {
                    v = QDate();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, var);
                }
                else
                {
                    v = V.toDate ();
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, V);
                }
                
                if (ch && objEditor)
                {
                    objEditor->setOpt (av->id(), isSystem, ch);
                    objEditor->addOptWidget (av->id(), isSystem, aw);
                }
                bool isEnable = (!ch || ch->checkState () == Qt::Checked);
                if (!isRef)
                {
                    connectToSlots (aw, wEditor);
                    qobject_cast <QDateEdit*>(aw)->setDate (v);
                }
                else
                {
                    KKSAttrRefWidget * rdw = qobject_cast<KKSAttrRefWidget *>(aw);
                    QDateEdit *dEdit = rdw ? qobject_cast<QDateEdit *>(rdw->getAttrWidget ()) : qobject_cast<QDateEdit *> (aw);
                    if (dEdit)
                        dEdit->setDate (v);
                }
                aw->setEnabled ((!isObjExist || !av->attribute()->isReadOnly()) && isEnable);
            }
            break;
        case KKSAttrType::atDateTime:
            {
                QDateTime v;
                if (av->attribute()->isMandatory() && (V.isNull() || V.toString() == "current_timestamp"))
                {
                    v = QDateTime::currentDateTime();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, var);
                }
                else if (!av->attribute()->isMandatory() && V.isNull())
                {
                    v = QDateTime();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, var);
                }
                else
                {
                    v = V.toDateTime ();
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, V);
                }

                if (ch && objEditor)
                {
                    objEditor->setOpt (av->id(), isSystem, ch);
                    objEditor->addOptWidget (av->id(), isSystem, aw);
                }
                bool isEnable = (!ch || ch->checkState () == Qt::Checked);
                if (!isRef)
                {
                    connectToSlots (aw, wEditor);
                    qobject_cast<QDateTimeEdit *>(aw)->setDateTime (v);
                }
                else
                {
                    KKSAttrRefWidget * rdw = qobject_cast<KKSAttrRefWidget *>(aw);
                    QDateTimeEdit *dtEdit = rdw ? qobject_cast<QDateTimeEdit *>(rdw->getAttrWidget ()) : qobject_cast<QDateTimeEdit *> (aw);
                    if (dtEdit)
                        dtEdit->setDateTime (v);
                }
                aw->setEnabled ((!isObjExist || !av->attribute()->isReadOnly()) && isEnable);
            }
            break;
        case KKSAttrType::atDateTimeEx:
            {
                QDateTime v;
                if (av->attribute()->isMandatory() && (V.isNull() || V.toString() == "current_timestamp"))
                {
                    v = QDateTime::currentDateTime();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, var);
                }
                else if (!av->attribute()->isMandatory() && V.isNull())
                {
                    v = QDateTime();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, var);
                }
                else
                {
                    v = V.toDateTime ();
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, V);
                }

                if (ch && objEditor)
                {
                    objEditor->setOpt (av->id(), isSystem, ch);
                    objEditor->addOptWidget (av->id(), isSystem, aw);
                }
                bool isEnable = (!ch || ch->checkState () == Qt::Checked);
                if (!isRef)
                {
                    connectToSlots (aw, wEditor);
                    qobject_cast<QDateTimeEdit *>(aw)->setDateTime (v);
                }
                else
                {
                    KKSAttrRefWidget * rdw = qobject_cast<KKSAttrRefWidget *>(aw);
                    QDateTimeEdit *dtEdit = rdw ? qobject_cast<QDateTimeEdit *>(rdw->getAttrWidget ()) : qobject_cast<QDateTimeEdit *> (aw);
                    if (dtEdit)
                        dtEdit->setDateTime (v);
                }
                aw->setEnabled ((!isObjExist || !av->attribute()->isReadOnly()) && isEnable);
            }
            break;
        case KKSAttrType::atTime:
            {
                QTime v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QTime::currentTime();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, var);
                }
                else if (!av->attribute()->isMandatory() && V.isNull())
                {
                    v = QTime();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, var);
                }
                else
                {
                    v = V.toTime ();
                    if (objEditor && !isRef)
                        objEditor->setValue (av->id(), isSystem, V);
                }

                if (ch && objEditor)
                {
                    objEditor->setOpt (av->id(), isSystem, ch);
                    objEditor->addOptWidget (av->id(), isSystem, aw);
                }
                bool isEnable = (!ch || ch->checkState () == Qt::Checked);
                if (!isRef)
                {
                    connectToSlots (aw, wEditor);
                    qobject_cast<QTimeEdit *>(aw)->setTime (v);
                }
                else
                {
                    KKSAttrRefWidget * rdw = qobject_cast<KKSAttrRefWidget *>(aw);
                    QTimeEdit *dtEdit = rdw ? qobject_cast<QTimeEdit *>(rdw->getAttrWidget ()) : qobject_cast<QTimeEdit *> (aw);
                    if (dtEdit)
                        dtEdit->setTime (v);
                }
                aw->setEnabled ((!isObjExist || !av->attribute()->isReadOnly()) && isEnable);
            }
            break;
        case KKSAttrType::atDouble:
        case KKSAttrType::atInt:
        case KKSAttrType::atInt64:
            {
                double v = V.toDouble ();
                int vi = V.toInt ();
                if (!isObjExist && av->attribute()->id() == ATTR_ID)
                {
                    vi = -1;
                    KKSValue val (QString::number(-1), KKSAttrType::atInt64);
                    (const_cast<KKSAttrValue *>(av))->setValue (val);
                }
                KKSEdit *lEdit = qobject_cast<KKSEdit *>(aw);
                if (pCatType->attrType() == KKSAttrType::atDouble && !V.toString().isEmpty())
                    lEdit->setVal (QString::number (v));
                else if ((pCatType->attrType() == KKSAttrType::atInt || pCatType->attrType() == KKSAttrType::atInt64)  && (!V.toString().isEmpty() || vi < 0))
                {
                    lEdit->setVal (QString::number (vi));
                    lEdit->QLineEdit::setText (QString::number (vi));
                }

                if (!isRef)
                    connectToSlots (aw, wEditor);
                else
                {
                    if (pCatType->attrType() == KKSAttrType::atDouble && !V.toString().isEmpty())
                        lEdit->setText (QString::number (v, 'f', 16));
                    else if ((pCatType->attrType() == KKSAttrType::atInt || pCatType->attrType() == KKSAttrType::atInt64) && !V.toString().isEmpty())
                        lEdit->setText (QString::number (vi));
                }
                lEdit->setVal (lEdit->text());
                lEdit->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
                if (av->attribute()->id() == ATTR_ID){
                    lEdit->setEnabled(false);
                }
            }
            break;
        case KKSAttrType::atString: 
        case KKSAttrType::atFixString:
        case KKSAttrType::atUrl:
            {
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                qDebug () << __PRETTY_FUNCTION__ << v;
                KKSEdit *lEdit = qobject_cast<KKSEdit *>(aw);
                lEdit->setVal (v);
                if (!isRef)
                {
                    connectToSlots (aw, wEditor);
                    lEdit->setVal (lEdit->text());
                    lEdit->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
                }
                else
                    lEdit->setText (v);
            }
            break;
        case KKSAttrType::atComplex:
            {
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                qDebug () << __PRETTY_FUNCTION__ << v;
                KKSComplexAttrWidget *lComplex = qobject_cast<KKSComplexAttrWidget *>(aw);
                lComplex->setVal (v);
                if (!isRef)
                {
                    connectToSlots (aw, wEditor);
                    //lComplex->setVal (lEdit->text());
                    lComplex->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
                }
                //else
                    //lComplex->setText (v);            
            }
            break;
        case KKSAttrType::atUUID:
            {
                QString v = V.toString();
                if (v.isNull())
                    v = QString("");
                qDebug () << __PRETTY_FUNCTION__ << v;
                KKSAttrUUIDWidget * uuidWidget = qobject_cast <KKSAttrUUIDWidget *>(aw);
                QToolButton * tbRef = uuidWidget->getButton ();
                if (objEditor && !isRef)
                    objEditor->addListAttrWidget (tbRef, uuidWidget, av);
                
                uuidWidget->setVal(av->id(), isSystem, v);
                
                if (!isRef)
                {
                    connectToSlots (aw, wEditor);
                    connect (uuidWidget, SIGNAL (generateUUID (qint64)), wEditor, SLOT (generateIOUUID (qint64)) );
                    connect (wEditor, SIGNAL (setUUID (QString)), uuidWidget, SLOT (setUUID (QString)) );
                    uuidWidget->setEnabled (!isObjExist);// || !av->attribute()->isReadOnly());
                }
            }
            break;
        case KKSAttrType::atInterval:
            {
                KKSIntervalWidget *intervalW = qobject_cast<KKSIntervalWidget *>(aw);
                if (ch && objEditor)
                {
                    objEditor->setOpt (av->id(), isSystem, ch);
                    objEditor->addOptWidget (av->id(), isSystem, lTitle);
                    objEditor->addOptWidget (av->id(), isSystem, intervalW);
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
                bool isEnable = (!ch || ch->checkState () == Qt::Checked);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                intervalW->setValue ();
                intervalW->setEnabled (isEnable);//!isExist || !pCategAttr->isReadOnly());
            }
            break;
        case KKSAttrType::atIntervalH:
            {
                KKSHIntervalW * intervalHW = qobject_cast<KKSHIntervalW *>(aw);
                if (ch && objEditor)
                {
                    objEditor->setOpt (av->id(), isSystem, ch);
                    objEditor->addOptWidget (av->id(), isSystem, lTitle);
                    objEditor->addOptWidget (av->id(), isSystem, intervalHW);
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
                bool isEnable = (!ch || ch->checkState () == Qt::Checked);
                intervalHW->setEnabled (isEnable);
                if (!isRef)
                    connectToSlots (aw, wEditor);

                QString v = V.toStringList().join(" ");
                QStringList vl = v.split (" ");
                if (vl.size() < 3)
                    intervalHW->setValue();
                else
                {
                    int h = vl[0].toInt();
                    int m = vl[1].toInt();
                    int s = vl[2].toInt();
                    intervalHW->setValue (h, m, s);
                }
            }
            break;
        case KKSAttrType::atText:
        case KKSAttrType::atXMLDoc:
            {
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                KKSText *lText = qobject_cast<KKSText*>(aw);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                else
                    lText->setText (v);
                lText->setVal (v);
                lText->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
            }
            break;
        case KKSAttrType::atJPG:
            {
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");

                KKSPixmap * px = qobject_cast<KKSPixmap *>(aw);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                if (isRef)
                    px->setPixmap (v);
                px->setVal (v);
                
                px->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
                if (tbRef)
                    tbRef->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
            }
            break;
        case KKSAttrType::atSVG:
            {
                QByteArray v = V.toByteArray ();
                //qDebug () << __PRETTY_FUNCTION__ << v;
                if (v.isNull())
                    v = QByteArray();

                KKSSvgWidget * px = qobject_cast<KKSSvgWidget *>(aw);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                if (isRef)
                    px->setContents (v);
                px->setVal (v);
                
                px->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
                if (tbRef)
                    tbRef->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
            }
            break;
        case KKSAttrType::atRecordColor:
        case KKSAttrType::atRecordTextColor:
            {
                //unsigned int vlc = V.toUInt ();
                qDebug () << __PRETTY_FUNCTION__ << V;// << vlc;
                //QRgb rgb_col (vlc);// = V.value<QColor>();//toUInt ();
                QColor rgb_color = V.value<QColor>();//QColor::fromRgba (rgb_col);//V.value<QColor>();//toInt ();
                if (V.toString().isEmpty())
                    rgb_color =  QColor ();
                KKSColorWidget * cw = qobject_cast<KKSColorWidget *>(aw);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                //rgb_color = V.value<QColor>();//QColor::fromRgba (av->value().value().toUInt());
                qDebug () << __PRETTY_FUNCTION__ << rgb_color;
                cw->setColor (rgb_color);
                cw->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
                if (tbRef)
                    tbRef->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
            }
            break;
        case KKSAttrType::atVideo:
            {
                KKSVideoPlayer * vw = qobject_cast< KKSVideoPlayer *>(aw);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                QByteArray v = V.toByteArray ();
                //qDebug () << __PRETTY_FUNCTION__ << v;
                if (v.isNull())
                    v = QByteArray();
                vw->setMovie (v);
            }
            break;
        case KKSAttrType::atBinary:
            {
                KKSBinaryWidget * bw = qobject_cast< KKSBinaryWidget *>(aw);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                QByteArray v = QByteArray::fromHex (V.toByteArray ());
                if (v.isNull())
                    v = QByteArray();
                qDebug () << __PRETTY_FUNCTION__ << v;
                bw->setData (v);
            }
            break;
        //ksa
        case KKSAttrType::atHistogram:
            {
                
                KKSHistWidgetEx * hw = qobject_cast<KKSHistWidgetEx *>(aw);
                hw->completed(false);
                hw->setHist(V.value<KKSHistogram>());
                if (!isRef)
                    connectToSlots (aw, wEditor);
                hw->completed(true);
                    
            }
            break;
        //ksa
        case KKSAttrType::atGISMap:
            {
                if (!isRef)
                    connectToSlots (aw, wEditor);
            }
            break;
    }
}

void KKSAttributesFactory :: connectToSlots (QObject *aw, QWidget* wEditor)
{
//    if (aw->metaObject ()->indexOfSignal (SIGNAL (valueChanged(int, bool, QVariant))) >= 0)
    QObject::connect (aw, 
                      SIGNAL (valueChanged(qint64, KKSIndAttrClass, QVariant)), 
                      wEditor, 
                      SLOT (setValue (qint64, KKSIndAttrClass, QVariant)) );
}

void KKSAttributesFactory :: slotLoadIOSrc (KKSObject ** io, QWidget * parent)
{
    if (!io)
        return;

    KKSList<const KKSFilterGroup *> filters;
    
    //QWidget * parentW = qobject_cast<QWidget *>(this->sender());
    
    KKSRecDialog* ioSrc = m_oef->createObjRecEditor(IO_IO_ID, IO_IO_ID, filters, tr ("Select document"), 0, true, false, Qt::WindowModal, parent);
    if (ioSrc->exec () == QDialog::Accepted)
    {
        int idObject = ioSrc->getID();
        *io = loader->loadIO (idObject, true);
    }
}

//��������� �� ��� �������� � �������������� ����������� ������ (simplify = true)
//�� ��� ���� ��������� �� �� ���������
void KKSAttributesFactory :: viewIOSrc (KKSObject * io, QWidget * parent)
{
    if (!io || io->id() <= 0)
        return;

    int idObj = io->id ();
    bool mode = false;//���. ������ �� ����� ���� �� ������. ������ ��������� �� � ���������. ��������
    QString s;
    
    KKSObject * o = loader->loadIO (idObj, true);
    m_oef->editExistOE(parent, IO_IO_ID, idObj, o->category(), s, 0, mode);
}

void KKSAttributesFactory::loadIOAttrValueHistory(const KKSAttrValue * av, bool forRecords)
{
    KKSList<KKSAttrValue *> avList = loader->loadIOAttrValueHistory(av, forRecords);
    //qDebug () << __PRETTY_FUNCTION__ << avList.count();

    emit viewHistory(av, avList);
}

void KKSAttributesFactory::loadIOAttrValueHistoryR(const KKSAttrValue * av, qint64 idObj, qint64 idRec, bool forRecords)
{
    if (av && av->id() > 0)
    {
        loadIOAttrValueHistory (av, forRecords);
        return;
    }
    else if (!qobject_cast<KKSObjEditor *>(sender()))
        return;

    KKSList<KKSAttrValue *> avList;
    const KKSAttrValue * avR (0);
    if (forRecords)
    {
        KKSObject * io = loader->loadIO (idObj);
        if (!io)
            return;
        KKSObjectExemplar * recIO = loader->loadEIO(idRec, io);
        if (!recIO)
        {
            io->release();
            return;
        }
        avR = recIO->attrValue (av->attribute()->id());
        (const_cast<KKSAttrValue *>(av))->setId (avR->id());
        avList = loader->loadIOAttrValueHistory(avR, forRecords);
        recIO->release();
        io->release ();
    }
    else
    {
        KKSObject * io = loader->loadIO (idObj);
        if (!io)
            return;
        avR = io->attrValueId (av->attribute()->id());
        (const_cast<KKSAttrValue *>(av))->setId (avR->id());
        avList = loader->loadIOAttrValueHistory(avR, forRecords);
        
        //qDebug () << __PRETTY_FUNCTION__ << avList.count() << this->sender();
        io->release ();
        
    }
    emit viewHistory(avR, avList);
}

/*
��� ��������� � ���������
*/
void KKSAttributesFactory :: showAttrsWidget (KKSAttribute *a, KKSAttrEditor *parent)
{
    KKSRecWidget * rw (0);

    if (a){
        rw = KKSViewFactory :: createAttrAttrsView (loader, a, true, parent, parent->windowFlags());
        parent->setRecWidget(rw);
    }
    else
    {
        rw = new KKSRecWidget (true, Qt::Vertical, parent, parent->windowFlags());
        QTreeView * tvTableAttrs = rw->getView();//new QTreeView ();
        tvTableAttrs->header()->setClickable (true);
        tvTableAttrs->header()->setSortIndicatorShown (true);
        tvTableAttrs->header()->setSortIndicator (5, Qt::AscendingOrder);
        tvTableAttrs->setSortingEnabled (true);
        
        //tvTableAttrs->setSelectionMode (QAbstractItemView::ExtendedSelection);
        //tvTableAttrs->setDragDropMode (QAbstractItemView::DragOnly);
        //tvTableAttrs->setDropIndicatorShown (true);

        
        rw->hideActionGroup (_ID_FILTER_GROUP);
        rw->hideActionGroup (_ID_IMPORT_GROUP);
        rw->hideActionGroup (_ID_VIEW_GROUP);
        rw->hideActionGroup (_ID_REPORT_GROUP);

        QAbstractItemModel * acModel = new KKSCatAttrsModel (0, 6);//QStandardItemModel (0, 4);
        acModel->setHeaderData (0, Qt::Horizontal, QObject::tr ("Name"));
        acModel->setHeaderData (1, Qt::Horizontal, QObject::tr ("Default value"));
        acModel->setHeaderData (2, Qt::Horizontal, QObject::tr ("Mandatory"));
        acModel->setHeaderData (3, Qt::Horizontal, QObject::tr ("Read only"));
        acModel->setHeaderData (4, Qt::Horizontal, QObject::tr ("Order"));
        acModel->setHeaderData (5, Qt::Horizontal, QObject::tr ("Directives"));
        
        KKSViewFactory::updateAttrAttrsModel (0, acModel);
        //qDebug () << __PRETTY_FUNCTION__ << rw->pbOk->isVisible();
        
        //tvTableAttrs->setModel (acModel);

        //������ ������ � ���������� � �������� �����������
        KKSSortFilterProxyModel *sortModel = 0;
        sortModel = new KKSSortFilterProxyModel (tvTableAttrs);//ksa ���� editor
        tvTableAttrs->setModel (sortModel);
        sortModel->setDynamicSortFilter (true);
        sortModel->setSourceModel (acModel);


        //������� � KKSSortFilterProxyModel ����� ���������, ������� �������� ������������ ��������� � ���������
        //��� ���� ��� ���������� ���������� �� ��������������� �������� � ������� (� ������ ���� �������(���������))
        KKSMap<int, KKSCategoryAttr *> acList = loader->loadCategoryAttrs(ATTRS_ATTR_TABLE_CATEGORY_ID);
        if(acList.count() > 0){
            KKSAttrView * av = 0;
            KKSCategoryAttr * ac = 0;
            
            ac = acList.value(1); //id
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(2);//name
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(304);//def_value
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(128); //is_mandatory
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(305); //is_read_only
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(404); //order
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(405); //directives
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();
        }



        int sortCol = tvTableAttrs->header()->sortIndicatorSection ();
        Qt::SortOrder sOrder = tvTableAttrs->header()->sortIndicatorOrder ();
        sortModel->sort (sortCol, sOrder);

        QHeaderView * header = tvTableAttrs->header();
        int lIndex = header->logicalIndex(0);
        header->setResizeMode(lIndex, QHeaderView::ResizeToContents);
        int size = header->sectionSize(lIndex);
        if(size > 300){
            header->setResizeMode(QHeaderView::Interactive);
            header->resizeSection(lIndex, 300);
        }

        KKSItemDelegate *itemDeleg = new KKSItemDelegate (rw);
        tvTableAttrs->setItemDelegate (itemDeleg);

        //parent->setRecWidget(rw);
    }
    
    connect (rw->actAdd, SIGNAL (triggered()), parent, SLOT (addTriggered()) );
    connect (rw->actEdit, SIGNAL (triggered()), parent, SLOT (editTriggered()) );
    connect (rw->actDel, SIGNAL (triggered()), parent, SLOT (delTriggered()) );


    if(rw)
    {
        //rw->setWindowModality(Qt::WindowModal);
        QDialog * attrDial = new QDialog;
        QGridLayout * gLay = new QGridLayout (attrDial);
        gLay->addWidget (rw, 0, 0, 1, 1);
        //rw->move(parent->pos());
        attrDial->setWindowTitle(tr("Select attribute"));
        QObject::connect (rw->pbCancel, SIGNAL (clicked()), attrDial, SLOT (reject()));
        QObject::connect (rw->pbOk, SIGNAL (clicked()), attrDial, SLOT (accept()));
        if (attrDial->exec() != QDialog::Accepted)
            return;
    }
}

void KKSAttributesFactory :: addAttribute (KKSAttribute *a, QAbstractItemModel * attrModel, KKSAttrEditor *editor)
{
    if (!a || !attrModel)
        return;

    KKSList<const KKSFilterGroup *> filters;
    KKSAttributesEditor *aEditor = viewAttributes (filters, true, editor, Qt::Dialog);

    if (!aEditor)
        return;

    aEditor->setWindowModality (Qt::WindowModal);
    

    if (aEditor->exec() == QDialog::Accepted)
    {
        QList<int> idAttrsList = aEditor->getAttributesId();
        KKSMap<int, KKSCategoryAttr*> aAttrs = a->attrs();

        for (int i=0; i<idAttrsList.size(); i++)
        {
            KKSAttribute * attr = loader->loadAttribute (idAttrsList[i]);
            if (!attr)
                continue;

            bool is_bad;
            KKSCategoryAttr *aAttr = KKSCategoryAttr::create (attr, false, false, QString(), 1, QString::null, &is_bad);
            if (is_bad || !aAttr)
            {
                if (aAttr)
                    aAttr->release ();
                return;
            }

            KKSMap<int, KKSCategoryAttr *>::const_iterator pca;
            bool bFound = false;
            for (pca = aAttrs.constBegin(); pca != aAttrs.constEnd(); pca++)
            {
                KKSCategoryAttr * aa = pca.value();
                if(aa->id() == aAttr->id()){
                    bFound = true;
                    break;
                }
            }
            
            if(!bFound)
                aAttrs.insert(aAttr->idRow(), aAttr);
            
            aAttr->release ();
            attr->release();
        }
        
        a->setAttrs(aAttrs);
        
        KKSViewFactory::updateAttrAttrsModel(a, attrModel);
    }
    editor->update ();
}

void KKSAttributesFactory :: editAttribute (int id, KKSAttribute *a, QAbstractItemModel * attrModel, KKSAttrEditor *editor)
{
    if (!a || !attrModel)
        return;

    KKSMap<int, KKSCategoryAttr*> aaList = a->attrs();

    KKSCategoryAttr * aAttr = aaList.value(id, NULL);
    if (!aAttr)
        return;

    KKSCategoryAttr * aa = new KKSCategoryAttr(*aAttr);
    KKSAttrAttrEditor *acEditor = new KKSAttrAttrEditor (aa, false, editor);

    if (acEditor->exec () != QDialog::Accepted){
        if (acEditor)
        {
            acEditor->setParent (0);
            delete acEditor;
        }

        aa->release();
        return;
    }
    
    aaList.remove(aa->idRow());
    aaList.insert(aa->idRow(), aa);//will be replaced
    aa->release();

    a->setAttrs(aaList);

    KKSViewFactory::updateAttrAttrsModel (a, attrModel);
    editor->update ();
}

void KKSAttributesFactory :: delAttribute (int id, KKSAttribute *a, QAbstractItemModel * attrModel, KKSAttrEditor *editor)
{
    if (!a || !attrModel)
        return;


    KKSMap<int, KKSCategoryAttr*> aaList = a->attrs();
    aaList.remove(id);
    a->setAttrs(aaList);

    KKSViewFactory::updateAttrAttrsModel(a, attrModel);
    editor->update ();
}

void KKSAttributesFactory :: addComplexAttr (KKSAttribute *a, QAbstractItemModel * attrModel, KKSAttrEditor *editor)
{
    if (!a || !attrModel)
        return;
    //qDebug () << __PRETTY_FUNCTION__ << a->id() << a->name() << a->title();
    KKSList<const KKSFilterGroup*> filters;
    KKSObject * refAttrs = loader->loadIO(IO_ATTR_ID, true);
    if (!refAttrs)
        return;
    KKSCategory * catAttr = refAttrs->category();
    if (!catAttr || !catAttr->tableCategory())
    {
        refAttrs->release();
        return;
    }
    catAttr = catAttr->tableCategory();
    
    //
    // ����������� ���� ���������, ������� �� ����� ������� � ��������� �������
    //
    QStringList filtValues;
    filtValues << QString::number(KKSAttrType::atParent);
    filtValues << QString::number(KKSAttrType::atSysChildCategoryRef);
    filtValues << QString::number(KKSAttrType::atSysUUID_T);
    filtValues << QString::number(KKSAttrType::atCheckListEx);
    filtValues << QString::number(KKSAttrType::atRecordColor);
    filtValues << QString::number(KKSAttrType::atRecordColorRef);
    filtValues << QString::number(KKSAttrType::atRecordTextColor);
    filtValues << QString::number(KKSAttrType::atRecordTextColorRef);
    filtValues << QString::number(KKSAttrType::atComplex);
    filtValues << QString::number(KKSAttrType::atGISMap);

    const KKSFilter * f = catAttr->createFilter(1,filtValues,KKSFilter::foNotIn);

    KKSList<const KKSFilter*> filts;
    filts.append (f);
    KKSFilterGroup * fg = new KKSFilterGroup (true);
    fg->setFilters(filts);
    filters.append(fg);
    fg->release();
    KKSAttributesEditor * aListEditor = this->viewAttributes(filters, true, editor);
    if (aListEditor->exec() == QDialog::Accepted)
    {
        QList<int> attrsId = aListEditor->getAttributesId();
        //qDebug () << __PRETTY_FUNCTION__ << attrsId;
        if (attrsId.isEmpty())
        {
            refAttrs->release();
            return;
        }
        int nAttrs = attrsId.count();
        KKSCategoryAttr::initDefIdRow();
        KKSMap<int, KKSCategoryAttr*> cAttrList = a->attrs();
        int nr = attrModel->rowCount();
        attrModel->insertRows(nr, nAttrs);
        for (int i=0; i<nAttrs; i++)
        {
            //QModelIndex aInd = attrModel->index (nr+i, 0);
            KKSAttribute * attr = loader->loadAttribute(attrsId[i]);
            KKSCategoryAttr * cAttr = KKSCategoryAttr::create(attr,false,false);
            int a_key = KKSCategoryAttr::getDefIdRow();
            while (cAttrList.contains(a_key))
                a_key--;
            cAttrList.insert(a_key, cAttr);
            KKSCategoryAttr::decDefIdRow();
            //const KKSAttribute * m_a = (attrModel->data (QModelIndex(), Qt::UserRole+2).value<const KKSAttribute *>());
            //qDebug () << __PRETTY_FUNCTION__ << attrModel->rowCount() << attrModel->columnCount() << m_a << a;
            //attrModel->setData (aInd, a_key, Qt::UserRole);
            //attrModel->setData (aInd, attr->name(), Qt::DisplayRole);
        }
        a->setAttrs(cAttrList);
        attrModel->setData (QModelIndex(), (QVariant::fromValue<KKSAttribute *>(a)), Qt::UserRole+2);
    }
    refAttrs->release();
}

void KKSAttributesFactory :: editComplexAttr (int id, KKSAttribute *a, QAbstractItemModel * attrModel, const QModelIndex& attrInd, KKSAttrEditor *editor)
{
    Q_UNUSED(id);
    if (!a || !attrModel || !attrInd.isValid())
        return;
    //qDebug () << __PRETTY_FUNCTION__ << id << a->id() << a->name() << a->title();
    KKSObject *attrTypesIO = loader->loadIO (IO_ATTR_TYPE_ID, true);
    if (!attrTypesIO)
    {
        qCritical() << QObject::tr ("Cannot load attribute types");
        QMessageBox::critical (editor, QObject::tr ("Attributes types"), QObject::tr ("Cannot load attribute types"), QMessageBox::Ok);
        return;
    }

    KKSMap<int, KKSAttrType*> aTypes;
    KKSMap<qint64, KKSEIOData *> attrTypesList = loader->loadEIOList (attrTypesIO);
    KKSMap<qint64, KKSEIOData *>::const_iterator pAttrs;
    for (pAttrs = attrTypesList.constBegin(); pAttrs != attrTypesList.constEnd(); pAttrs++)
    {
        KKSAttrType *aType = new KKSAttrType ();
        if (!aType)
            continue;
        aType->setId (pAttrs.key());
        aType->setName (pAttrs.value()->fields ().value ("name"));
        aType->setCode (pAttrs.value()->fields ().value ("code"));
        aTypes.insert (pAttrs.key(), aType);
        if (aType)
            aType->release ();
    }
    attrTypesIO->release();
    KKSMap<int, KKSCategoryAttr *> cAttrs = a->attrs();
    KKSMap<int, KKSCategoryAttr *>::const_iterator pa = cAttrs.constBegin()+attrInd.row();
    KKSCategoryAttr * oldAttr = pa.value();
    KKSCategoryAttr * cAttr (oldAttr);
    cAttr->addRef();
    int ikey = pa.key();
    KKSCatAttrEditor * cEditor = new KKSCatAttrEditor (cAttr, aTypes, false, 0);
    if (cEditor->exec() == QDialog::Accepted)
    {
        cAttrs.insert (ikey, cAttr);
        attrModel->setData (attrInd, QVariant::fromValue<KKSCategoryAttr *>(cAttr), Qt::UserRole+1);
        a->setAttrs (cAttrs);
    }
}

void KKSAttributesFactory :: delComplexAttr (int id, KKSAttribute *a, QAbstractItemModel * attrModel, const QModelIndex& attrInd, KKSAttrEditor *editor)
{
    Q_UNUSED(id);

    if (!a || !attrModel || !attrInd.isValid())
        return;
    //qDebug () << __PRETTY_FUNCTION__ << id << a->id() << a->name() << a->title();
    KKSMap<int, KKSCategoryAttr*> cAttrList = a->attrs();
    int iRow = attrInd.row();
    KKSMap<int, KKSCategoryAttr*>::iterator p = cAttrList.begin()+iRow;
    cAttrList.erase (p);
    QModelIndex pInd = attrInd.parent();
    bool isRemoved = attrModel->removeRows (iRow, 1, pInd);
    if (!isRemoved)
        return;
    a->setAttrs(cAttrList);
    Q_UNUSED (editor);
}

void KKSAttributesFactory :: updateAttrModel (QAbstractItemModel * attrModel)
{
    KKSViewFactory::updateAttributesModel(loader,attrModel);
    KKSMap<int, KKSAGroup *> aGroups = loader->loadAvailAttrsGroups();
    KKSAttributesEditor * aEditor = qobject_cast<KKSAttributesEditor *>(this->sender());
    if (aEditor)
        aEditor->setAvailableGroups(aGroups);
}

void KKSAttributesFactory :: viewAttrValue (const KKSAttrValue * av, int idAVal, int isSys, QWidget * pWidget)
{
    if (!av || idAVal < 0)
        return;
    //qDebug () << __PRETTY_FUNCTION__ << av->id() << idAVal << isSys << (qobject_cast<QWidget *>(this->sender()));
    QWidget * avW = createAttrValWidget (av, idAVal, isSys, pWidget, Qt::Dialog);
    if (pWidget && pWidget->parentWidget() && qobject_cast<AttrHistory *>(pWidget->parentWidget()->parentWidget()))
    {
        connect (avW, SIGNAL (prevVal()), pWidget->parentWidget()->parentWidget(), SLOT (upClicked()) );
        connect (avW, SIGNAL (nextVal()), pWidget->parentWidget()->parentWidget(), SLOT (downClicked()) );
    }
    if (!avW)
        return;
    avW->adjustSize();
    avW->setWindowModality(Qt::ApplicationModal);
    if (qobject_cast<QDialog *>(avW))
        (qobject_cast<QDialog *>(avW))->exec();
    else
        avW->show();
}

QWidget * KKSAttributesFactory :: createAttrValWidget (const KKSAttrValue * pAttrValue, int idAVal, int isSys, QWidget * parent, Qt::WindowFlags flags)
{
    if (!pAttrValue)
        return 0;
    
    if (!pAttrValue->attribute())
        return 0;
    
    KKSAttrValue * av = loader->loadIOAttrValue(pAttrValue,idAVal,(isSys!=iacIOUserAttr));
    if (!av)
        return 0;
    if (av->attribute()->type()->attrType() == KKSAttrType::atComplex)
    {
        loader->loadAttrAttrs(av->attribute());
        if(!av->attrsValuesLoaded())
        {
            KKSMap<qint64, KKSAttrValue*> aav_list = loader->loadAttrAttrValues(av, false);
            av->setAttrsValues(aav_list);
        }
    }
    QWidget * avWidget = new KKSAValWidget (av, parent, flags);//QWidget (parent, flags);
    connect (avWidget, SIGNAL (updateMod (const KKSAttrValue *, const QVariant&, QAbstractItemModel *)),
             this,
             SLOT (updateAttrValueModel(const KKSAttrValue *, const QVariant&, QAbstractItemModel *))
            );
    connect (this, SIGNAL (aValRefresh (const KKSAttribute *, QVariant)),
             avWidget,
             SLOT (setValue (const KKSAttribute *, QVariant))
            );
//    QGridLayout * avGLay = new QGridLayout (avWidget);
    const KKSAttribute * a = pAttrValue->attribute();
    const KKSAttrType * aType = a->type();
    KKSAttrType::KKSAttrTypes idAttrType = aType->attrType();
    if (idAttrType == KKSAttrType::atCheckList ||
        idAttrType == KKSAttrType::atCheckListEx)
    {
        QAbstractItemModel * avMod = aValComplexModel (pAttrValue, av->value().valueVariant());
        (qobject_cast<KKSAValWidget *>(avWidget))->setValModel (avMod);
    }

    return avWidget;
}

QAbstractItemModel * KKSAttributesFactory :: aValComplexModel (const KKSAttrValue * pAttrValue, const QVariant& av)
{
    if (!pAttrValue || !pAttrValue->attribute())
        return 0;
    QString tableName = pAttrValue->attribute()->tableName ();
    QVariant V = av.isValid() ? av : pAttrValue->value().valueVariant();

    //QStringList vArray = V.toStringList();
    //qDebug () << __PRETTY_FUNCTION__ << vArray << pAttrValue->id() << pAttrValue->attribute()->id();
    KKSObject * refIO = loader->loadIO (tableName, true);
    if (!refIO)
        return 0;
    KKSCategory * c = refIO->category ();
    if (!c)
    {
        refIO->release ();
        return 0;
    }
    KKSCategory * ct = c->tableCategory ();
    if (!ct)
    {
        refIO->release ();
        return 0;
    }
    KKSMap<qint64, KKSEIOData *> eioList = this->getAttrList(pAttrValue, V);
    const KKSTemplate * ctempl = new KKSTemplate (ct->defTemplate());
    QAbstractItemModel * sAttrModel = new KKSEIODataModel (ctempl, eioList);
    ctempl->release();
    refIO->release ();
    return sAttrModel;
}

void KKSAttributesFactory :: updateAttrValueModel (const KKSAttrValue * pAttrValue, const QVariant& val, QAbstractItemModel * attrValModel)
{
    if (!pAttrValue || !pAttrValue->attribute())
        return;
    KKSMap<qint64, KKSEIOData *> eioList = this->getAttrList (pAttrValue, val);
    int nr = attrValModel->rowCount();
    attrValModel->removeRows(0, nr);
    nr = eioList.count();
    attrValModel->insertRows (0,nr);
    int i=0;
    for (KKSMap<qint64, KKSEIOData *>::const_iterator p = eioList.constBegin();
         p != eioList.constEnd(); p++)
    {
        QModelIndex wIndex = attrValModel->index (i, 0);
        attrValModel->setData(wIndex,QVariant::fromValue<KKSEIOData*>(p.value()), Qt::UserRole+1);
        
        ++i;
    }
}

KKSMap<qint64, KKSEIOData *> KKSAttributesFactory :: getAttrList (const KKSAttrValue * pAttrValue, const QVariant& val) const
{
    KKSMap<qint64, KKSEIOData *> eioList;
    if (!pAttrValue || !pAttrValue->attribute())
        return eioList;
    QString tableName = pAttrValue->attribute()->tableName ();
    QVariant V (val);//= av.isValid() ? av : pAttrValue->value().valueVariant();

    QStringList vArray = V.toStringList();
    //qDebug () << __PRETTY_FUNCTION__ << vArray << pAttrValue->id() << pAttrValue->attribute()->id();
    KKSObject * refIO = loader->loadIO (tableName, true);
    if (!refIO)
        return eioList;
    KKSCategory * c = refIO->category ();
    if (!c)
    {
        refIO->release ();
        return eioList;
    }
    KKSCategory * ct = c->tableCategory ();
    if (!ct)
    {
        refIO->release ();
        return eioList;
    }
    KKSList<const KKSFilterGroup*> filters;
    QString vals;
    for (int i=0; i<vArray.count(); i++)
    {
        vals += vArray[i];
        if (i < vArray.count()-1)
            vals += ",";
    }
    QString value;
    if (!vals.isEmpty())
    {
        value = QString ("select id from %1 where id in (%2) ").arg (tableName).arg (vals);
    }
    else
    {
        value = QString ("select id from %1 where id is null ").arg (tableName);
    }
    //const KKSFilter * filter = ct->createFilter ("id", value, KKSFilter::foInSQL);
    const KKSFilter * filter = ct->createFilter (ATTR_ID, value, KKSFilter::foInSQL);
    KKSList <const KKSFilter *> fl;
    fl.append (filter);
    filter->release ();
    KKSFilterGroup * fg = new KKSFilterGroup(true);
    fg->setFilters(fl);
    filters.append(fg);
    fg->release();
    eioList = loader->loadEIOList (refIO, filters);
    refIO->release();
    return eioList;
}

void KKSAttributesFactory :: initAttrValueModel (const KKSAttrValue * pAttrValue, const QVariant& val)
{
    KKSAValWidget * avW = qobject_cast<KKSAValWidget *> (sender());
    QAbstractItemModel * avMod = aValComplexModel (pAttrValue, val);
    if (avW)
        avW->setValModel (avMod);
    else
        delete avMod;
}

void KKSAttributesFactory :: refreshAttrValue (const KKSAttrValue * av, int idAVal)
{
    if (!av)
        return;
    KKSAttrValue * avNew = loader->loadIOAttrValue(av, idAVal, false);
    if (!avNew)
        return;
    QVariant val (avNew->value().valueVariant());
    if (avNew->attribute()->type()->attrType() == KKSAttrType::atComplex)
    {
        loader->loadAttrAttrs(avNew->attribute());
        if(!avNew->attrsValuesLoaded())
        {
            KKSMap<qint64, KKSAttrValue*> aav_list = loader->loadAttrAttrValues(avNew, false);
            avNew->setAttrsValues(aav_list);
        }
        QMap<QString, QVariant> valMap;
        for (KKSMap<qint64, KKSAttrValue*>::const_iterator p=avNew->attrsValues().constBegin();
                p != avNew->attrsValues().constEnd();
                p++)
        {
            QVariant v = p.value()->value().valueVariant();
            valMap.insert(QString::number(p.key()), v);
        }
        val = QVariant(valMap);
    }
    emit aValRefresh (av->attribute(), val);
    avNew->release();
}

/*
QMap<int, QString> KKSAttributesFactory :: getAttrValsList (qint64 idObject, const KKSList<const KKSFilterGroup *>& filters)
{
    QMap<int, QString> vList;
    KKSObject * refIO = loader->loadIO (idObject, true);
    KKSMap<qint64, KKSEIOData *> vMaps = loader->loadEIOList(refIO, filters);
    for (KKSMap<qint64, KKSEIOData *>::const_iterator p = vMaps.constBegin();
            p != vMaps.constEnd();
            p++)
    {
        vList.insert (p.key(), p.value()->fields().value("name"));
    }
    refIO->release ();
    vMaps.clear ();
    return vList;
}
*/
void KKSAttributesFactory :: getIdForHistogramParams(const QString & tableName, qint64 * id)
{
    if(tableName.isEmpty() || !id)
        return;

    if (
        tableName.compare(QString("histogram_params_streams"), Qt::CaseInsensitive) != 0 &&
        tableName.compare(QString("histogram_params_chains"), Qt::CaseInsensitive) != 0
       )
    {
        return;
    }
    
    *id = eiof->getNextSeq(tableName, "id");
    return;
}

void KKSAttributesFactory :: getHistogramGraphic(KKSHistogram & h, const QString & tName)
{
    QString hStr = loader->getHistogramValue(h, tName);
}

void KKSAttributesFactory :: loadHistogramData(int id, const QString & tableName, QWidget * parent)
{
    m_oef->loadHistogramData(id, tableName, parent);
}

void KKSAttributesFactory :: loadHistReferences (KKSAttribute * attr, int aType, KKSAttrEditor * aEditor)
{
    if (!attr || !aEditor)
        return;

    if (!attr->type())
    {
        KKSAttrType * t = new KKSAttrType(aType);
        attr->setType(t);
    }

    QMap<int, QString> io_refs;

    KKSObject *io = loader->loadIO (IO_IO_ID, true);
    if(!io)
        return;

    const KKSCategory * c = io->category();
    const KKSCategory * ct = c->tableCategory();

    KKSList<const KKSFilterGroup *> filters;

    if(aType == KKSAttrType::atHistogram)
    {
        const KKSFilter * f = ct->createFilter (1, "select id from tbl_io_objects where id in (108, 109)", KKSFilter::foInSQL);
        KKSFilterGroup * fg = new KKSFilterGroup (true);
        fg->addFilter (f);
        f->release ();
        filters.append (fg);
        fg->release ();
    }
    
    KKSMap<qint64, KKSEIOData *> io_data = loader->loadEIOList (io, filters);
    KKSMap<qint64, KKSEIOData *>::const_iterator p;

    io_refs.clear ();
    int ind = -1;
    int i = 0;
    int idRef = -1;
    for (p=io_data.constBegin(); p != io_data.constEnd(); p++)
    {
        io_refs.insert (p.key(), p.value()->fields ().value ("name"));
        KKSObject *io = loader->loadIO (p.key(), true);
        if(!io)
            continue;

        if (io->tableName() == attr->tableName())
        {
            ind = i;
            idRef = p.key();//io->id ();
            aEditor->setCurrentRef (p.value()->fields ().value ("name"));//ind);
        }
        i++;
        io->release ();
    }
    if (ind >= 0)
    {
        loadAttrsRefFields (attr, idRef, aEditor);
    }

    aEditor->setReferences(io_refs);
    QMap<QString, QString> fields;
    for (KKSMap<int, KKSCategoryAttr*>::const_iterator pc = ct->attributes().constBegin(); pc != ct->attributes().constEnd(); pc++)
        fields.insert (pc.value()->code(), pc.value()->title ());//name ());
    io->release ();
}
