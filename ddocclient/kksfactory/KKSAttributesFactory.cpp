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

#include <KKSAttributesEditor.h>
#include <KKSObjEditor.h>
#include <kksfilterseditorform.h>
#include <kksattreditor.h>
#include <KKSCheckBox.h>
#include <KKSAttrRefWidget.h>
#include <KKSListWidget.h>
#include <KKSDateEdit.h>
#include <KKSDateTimeEdit.h>
#include <KKSTimeEdit.h>
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

#include <KKSFilter.h>
#include <KKSObject.h>
#include <KKSCategory.h>
#include <KKSObjectExemplar.h>
#include <KKSEIOData.h>

#include "KKSViewFactory.h"
#include "KKSAttributesFactory.h"
#include "KKSObjEditorFactory.h"
#include "KKSLoader.h"
#include "KKSConverter.h"
#include "KKSEIOFactory.h"

#include <defines.h>

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
    connect (aEditor, SIGNAL (insertAttr (KKSAttribute *, int, QAbstractItemModel *, int, KKSAttributesEditor *)), this, SLOT (saveAttribute (KKSAttribute *, int, QAbstractItemModel*, int, KKSAttributesEditor *)) );
    connect (aEditor, SIGNAL (updateAttr (int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)), this, SLOT (loadAttribute(int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)) );
    connect (aEditor, SIGNAL (deleteAttr (int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)), this, SLOT (delAttribute(int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)) );
    connect (aEditor, SIGNAL (getFieldsOfReference (KKSAttribute *, int, KKSAttrEditor * )), this, SLOT (loadAttrsRefFields (KKSAttribute *, int, KKSAttrEditor *)) );
    connect (aEditor, SIGNAL (getSearchTemplate (KKSAttrEditor * )), this, SLOT (loadSearchTemplates (KKSAttrEditor *)) );
    connect (aEditor, SIGNAL (insertAttrGroup (QAbstractItemModel *,KKSAttributesEditor *)), this, SLOT (addAttrGroup (QAbstractItemModel *, KKSAttributesEditor *)) );
    connect (aEditor, SIGNAL (updateAttrGroup (int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)), this, SLOT (editAttrGroup (int, QAbstractItemModel *, const QModelIndex&,  KKSAttributesEditor *)) );
    connect (aEditor, SIGNAL (deleteAttrGroup (int, QAbstractItemModel *, const QModelIndex&,  KKSAttributesEditor *)), this, SLOT (delAttrGroup (int, QAbstractItemModel *, const QModelIndex&, KKSAttributesEditor *)) );
    return aEditor;
}

KKSAttributesFactory :: KKSAttributesFactory (KKSLoader *l, KKSEIOFactory *_eiof, KKSObjEditorFactory * _oef)
    : loader (l),
    eiof (_eiof),
    m_oef (_oef)
{
}
void KKSAttributesFactory ::setOEF(KKSObjEditorFactory * _oef)
{
    m_oef = _oef;
}

KKSAttributesFactory :: ~KKSAttributesFactory (void)
{
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

    KKSFiltersEditorForm * f = new KKSFiltersEditorForm (c, attrs, forIO, attrsEditor);
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
void KKSAttributesFactory :: saveAttribute (KKSAttribute * cAttr, int idType, QAbstractItemModel* aModel, int idAttrGroup, KKSAttributesEditor *aEditor)
{
    if (!cAttr)
        return;

    qDebug () << __PRETTY_FUNCTION__ << cAttr->id ();
    if (idType < KKSAttrType::atUndef || idType > KKSAttrType::atUserDef)
        return;

    KKSAttrType::KKSAttrTypes idAttrType = (KKSAttrType::KKSAttrTypes)idType;
    KKSAttrType *aType = loader->loadAttrType (idAttrType);
    cAttr->setType (aType);
    if (cAttr->type() == 0)
        return;
    KKSAGroup * aGr = loader->loadAttrGroup(idAttrGroup);
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
        QMessageBox::warning (aEditor, tr ("Save attribute"), tr ("Cannot save attribute %1").arg (cAttr->title()), QMessageBox::Ok);
        return;
    }
    if (isInsert)
        cAttr->setId (objC->id());

    KKSViewFactory::updateAttributesModel (loader, aModel);

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

    KKSCategoryAttr *cAttr = KKSCategoryAttr :: create (attr, false, false, QString());
    if (!cAttr)
        return;

    KKSAttrEditor *attrEditor = new KKSAttrEditor (attr, aEditor->getTypes(), aEditor->getAvailableGroups(), aEditor->getReferences (), aEditor);
    if (attr->type()->attrType () == KKSAttrType::atList || 
        attr->type()->attrType () == KKSAttrType::atParent || 
        attr->type()->attrType () == KKSAttrType::atCheckList ||
        attr->type()->attrType () == KKSAttrType::atCheckListEx ||
        attr->type()->attrType () == KKSAttrType::atRecordColorRef ||
        attr->type()->attrType () == KKSAttrType::atRecordTextColorRef)
    {
        loadAttrsRefs (attr, attrEditor);
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

    QModelIndex pIndex = aIndex;
    while (pIndex.parent().isValid() && pIndex.data(Qt::UserRole+USER_ENTITY).toInt() > 0)
        pIndex = pIndex.parent();
    attrEditor->setGroupId(pIndex.data(Qt::UserRole).toInt());

    if (attrEditor->exec () == QDialog::Accepted)
    {
        KKSAttribute *cAttrRes = attrEditor->getAttribute ();
        int idAttrGr = attrEditor->getGroupId ();
        KKSAGroup * aGr = loader->loadAttrGroup(idAttrGr);
        cAttrRes->setGroup (aGr);
        aGr->release ();
        qDebug () << __PRETTY_FUNCTION__ << cAttrRes->id () << cAttrRes->tableName() << cAttrRes->columnName ();
        KKSObjectExemplar * oe = KKSConverter::attributeToExemplar (loader, cAttrRes);
        if (!oe)
            return;
        eiof->updateEIO (oe);
        oe->release ();
        aModel->setData (aIndex, cAttrRes->id(), Qt::UserRole);
        aModel->setData (aIndex, cAttrRes->name(), Qt::DisplayRole);
        QModelIndex titleIndex = aIndex.sibling (aIndex.row(), 2);
        aModel->setData (titleIndex, cAttrRes->title(), Qt::DisplayRole);
        KKSViewFactory::updateAttributesModel (loader, aModel);
    }

    if (attrEditor)
    {
        attrEditor->disconnect ();
        delete attrEditor;
    }
    attr->release ();
    cAttr->release ();
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
    qDebug () << __PRETTY_FUNCTION__ << row << idAttr;
    KKSAttribute *attr = loader->loadAttribute (idAttr);
    if (!attr)
        return;
    KKSObjectExemplar * oe = KKSConverter::attributeToExemplar (loader, attr);
    if (!oe)
        return;

    if (eiof->deleteEIO(oe) != ERROR_CODE)
        aModel->removeRows (row, 1, aIndex.parent());
    else
        QMessageBox::warning (aEditor, tr("Delete Attribute"), tr("Attribute %1 is used").arg (attr->title()), QMessageBox::Ok);

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

    KKSObjEditor *objEditor = m_oef->createObjRecEditor(IO_IO_ID, 
                                                        IO_SEARCH_ID, 
                                                        filters, 
                                                        "",
                                                        o->category(),
                                                        true,
                                                        Qt::WindowModal,
                                                        parent, 
                                                        Qt::Dialog);
    int idSearchTemplate = 0;
    if (objEditor->exec () == QDialog::Accepted){
        idSearchTemplate = objEditor->getID();
        if(idSearchTemplate <= 0)
            return;
    }
    else
        return;

    
    KKSSearchTemplate * st = loader->loadSearchTemplate(idSearchTemplate);
    aEditor->setSearchTemplate(st);
    if(st)
        st->release();
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

    qDebug () << __PRETTY_FUNCTION__ << fields;
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
void KKSAttributesFactory :: addAttrGroup (QAbstractItemModel *aModel, KKSAttributesEditor *attrEditor)
{
    if (!aModel || !attrEditor)
        return;

    KKSObject * refIO = loader->loadIO (IO_ATTRS_GROUPS_ID, true);
    if (!refIO)
    {
        QMessageBox::warning (attrEditor, tr ("Add new group"), tr ("Cannot load referece IO"), QMessageBox::Ok);
        return;
    }
    KKSCategory * c = refIO->category();
    if (c)
        c = c->tableCategory();
    if (!c)
    {
        QMessageBox::warning (attrEditor, tr ("Add new group"), tr ("Incorrect load referece IO"), QMessageBox::Ok);
        return;
    }
    KKSList<const KKSFilterGroup *> filters = KKSList<const KKSFilterGroup *>();
    /*KKSObjEditor * oEditor = */
    m_oef->createNewEditor (attrEditor, IO_ATTRS_GROUPS_ID, c, refIO->tableName(), 0, (attrEditor->windowModality() != Qt::NonModal));
    //m_oef->createObjEditor(IO_ATTRS_GROUPS_ID, -1, filters, tr("Add new group"), c, false, refIO->tableName(), false, Qt::NonModal, 0, 0);
    //oEditor->setAttribute (Qt::WA_DeleteOnClose);
    //oEditor->show ();
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
        QMessageBox::warning (attrEditor, tr ("Edit attributes group"), tr ("Cannot load referece IO"), QMessageBox::Ok);
        return;
    }
    KKSCategory * c = refIO->category();
    if (c)
        c = c->tableCategory();
    if (!c)
    {
        QMessageBox::warning (attrEditor, tr ("Edit attributes group"), tr ("Incorrect load referece IO"), QMessageBox::Ok);
        return;
    }
    KKSList<const KKSFilterGroup *> filters = KKSList<const KKSFilterGroup *>();
    m_oef->editExistOE (attrEditor, refIO->id(), idAttrGroup, c, refIO->tableName(), 0, false);
    Q_UNUSED (aIndex);
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
        aModel->removeRows (row, 1, aIndex.parent());
    else
        QMessageBox::warning (attrEditor, tr("Delete group of attributes"), tr("Cannot delete group of attributes"), QMessageBox::Ok);

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
void KKSAttributesFactory :: putAttrWidget (KKSAttrValue* av, KKSObjEditor * objEditor, QGridLayout *gLayout, int n_str, bool isSystem, QString tableName, int idCat)
{
    const KKSValue pVal = av->value();
    const KKSCategoryAttr * pCategAttr = av->attribute();
    QVariant V = pVal.valueVariant();
    bool isExist (objEditor->getObjectEx()->id()>0);

    if (!pCategAttr)
    {
        qDebug () << __PRETTY_FUNCTION__ << "Category attribute is null";
        return;
    }

    const KKSAttrType *pCatType = pCategAttr->type ();
    QToolButton *tbRef = 0;
    QLabel *lTitle = 0;
    QCheckBox *ch = 0;
    QWidget *aw = 0;
    
    if (pCatType->attrType() != KKSAttrType::atCheckListEx)
        aw = this->createAttrWidget (av, objEditor, pCategAttr->isMandatory(), pCatType, isSystem, gLayout, n_str, V, lTitle, tbRef, ch);
    else if (pCategAttr->id () == ATTR_IO_OBJECTS_ORGANIZATION)
        return;
    else
    {
        QTabWidget * tabEnc = objEditor->getRecTab();
        aw = this->createAttrCheckWidget (av, pCatType, isSystem, tabEnc);
    }

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
QLabel * KKSAttributesFactory :: createAttrTitle (KKSAttrValue * av, bool isSystem, KKSObjEditor *objEditor)
{
	if(!av)
        return new QLabel();

    QLabel * lTitle = new KKSAttrValueLabel(av, isSystem);
    connect(lTitle, SIGNAL(loadIOSrc(KKSObject ** )), this, SLOT(slotLoadIOSrc(KKSObject ** )));
    connect(lTitle, SIGNAL(viewIOSrc(KKSObject *, QWidget *)), this, SLOT(viewIOSrc(KKSObject *, QWidget *)));
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
 * isSystem -- ��������� �������
 * gLayout -- ������� ������ != 0
 * n_str -- ����� ������ � �������
 * V -- �������� �������� � ������ QVariant
 * lTitle -- �������� ��������,
 * tbRef -- ������ �������� �������� ��� �������� ���������� ����,
 * ch -- ������� enable/disable ��� �������������� ���������,
 * isRef -- true ��� �������� �������� ��� ��������, �� ������� ��������� ��������-������,
 *          false ��� ���� ��������� �������.
 * ���������:
 * ������, �������������� ��������.
 */
QWidget * KKSAttributesFactory :: createAttrWidget (KKSAttrValue * av, KKSObjEditor *objEditor, bool is_mandatory, const KKSAttrType *pCatType, bool isSystem, QGridLayout *gLayout, int n_str, const QVariant& V, QLabel *lTitle, QToolButton *&tbRef, QCheckBox *&ch, bool isRef)
{
    QWidget * attrWidget = 0;
    if (!pCatType)
        return 0;

    QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    if (isRef)
        qDebug () << __PRETTY_FUNCTION__ << pCatType->attrType();
   
    switch (pCatType->attrType())
    {
        case KKSAttrType::atUndef: default:
            break;
        case KKSAttrType::atBool:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, isSystem, objEditor);//pCategAttr->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);
                }
				attrWidget = new KKSCheckBox (av, isSystem);
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
            {
                const KKSCategoryAttr * pCategAttr = av->attribute();
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, isSystem);//, pCategAttr->title(), is_mandatory);
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
                             pCategAttr->refType()->attrType() != KKSAttrType::atParent);
                qDebug () << __PRETTY_FUNCTION__ << "Widget creation" << pCategAttr->refType()->attrType() << isWRef << pCategAttr->id();
                const KKSAttribute * refAttr = 0;
                if (pCategAttr->refType()->attrType () == KKSAttrType::atList ||
                    pCategAttr->refType()->attrType () == KKSAttrType::atParent)
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
                QWidget *arw = createAttrWidget (av, objEditor, is_mandatory, cAttr ? cAttr->refType() : pCategAttr->refType(), isSystem, qobject_cast<QGridLayout *>(attrWidget->layout ()), 0, vr, l, tb, chr, true);//isWRef);
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
        case KKSAttrType::atCheckList:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory );
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);
                }
                attrWidget = new KKSListWidget (av, isSystem);
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
            break;
        case KKSAttrType::atObjRef:
            break;
        case KKSAttrType::atDate:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    ch = this->createChDateTime (is_mandatory, gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                QDate v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QDate::currentDate();
                    QVariant var(v);
                }
                else
                    v = V.toDate ();

                attrWidget = new KKSDateEdit (av, isSystem, v);
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
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    ch = this->createChDateTime (is_mandatory, gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                QDateTime v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QDateTime::currentDateTime();
                    QVariant var(v);
                }
                else
                    v = V.toDateTime ();

                attrWidget = new KKSDateTimeEdit (av, isSystem, v);
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
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    ch = this->createChDateTime (is_mandatory, gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                QTime v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QTime::currentTime();
                    QVariant var(v);
                }
                else
                    v = V.toTime ();

                attrWidget = new KKSTimeEdit (av, isSystem, v);
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
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);
                }
                double v = V.toDouble ();
                int vi = V.toInt ();
                attrWidget = new KKSEdit (av, isSystem, (pCatType->attrType() == KKSAttrType::atDouble ? QString::number (v) : QString::number (vi)));
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
        case KKSAttrType::atString: 
        case KKSAttrType::atFixString: 
        case KKSAttrType::atUrl:
        case KKSAttrType::atMaclabel:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);
                }
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                attrWidget = new KKSEdit (av, isSystem, v);
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
        case KKSAttrType::atInterval:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    ch = this->createChDateTime (is_mandatory, gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::Expanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                attrWidget = new KKSIntervalWidget (av, isSystem);
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
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    ch = this->createChDateTime (is_mandatory, gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::Expanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                attrWidget = new KKSHIntervalW (av, isSystem);
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
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 2, 2, Qt::AlignRight | Qt::AlignTop);
                }
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                attrWidget = new KKSText (av, isSystem, v);
                qobject_cast<KKSText *>(attrWidget)->setFixedSymCount (av->attribute()->defWidth());
                qobject_cast<QTextEdit *>(attrWidget)->setReadOnly (isRef);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                attrWidget->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
            }
            break;
        case KKSAttrType::atJPG:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight);
                }
                
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");

                attrWidget = new KKSPixmap (av, isSystem, v);
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
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 1, Qt::AlignRight);
                }

                QByteArray v = V.toByteArray ();
                if (v.isNull())
                {
                    v = QByteArray();
                    attrWidget = new KKSSvgWidget (av, isSystem);
                }
                else
                    attrWidget = new KKSSvgWidget (av, isSystem, v);//, gLayout->parentWidget());
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
        case KKSAttrType::atPoints:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 2, 2, Qt::AlignRight | Qt::AlignTop);
                }
                QList<QVariant> v = V.toList();
                attrWidget = new KKSPointTable (av, isSystem, v);
                QSizePolicy hPwt (QSizePolicy::Expanding, QSizePolicy::Expanding);
                attrWidget->setSizePolicy (hPwt);
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);//, Qt::AlignJustify);//Qt::AlignCenter);
            }
            break;
        case KKSAttrType::atRecordColor:
        case KKSAttrType::atRecordTextColor:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2);//, Qt::AlignRight | Qt::AlignTop);
                }
                unsigned int vlc = V.toUInt ();
                QRgb rgb_col (vlc);// = V.value<QColor>();//toUInt ();
                QColor rgb_color = QColor::fromRgba (rgb_col);//V.value<QColor>();//toInt ();
                if (V.toString().isEmpty())
                    rgb_color =  QColor ();
                qDebug () << __PRETTY_FUNCTION__ << rgb_color << vlc << V;
                attrWidget = new KKSColorWidget (av, isSystem, rgb_color);
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
                    lTitle = this->createAttrTitle (av, isSystem);//, av->attribute()->title(), is_mandatory);
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2);//, Qt::AlignRight | Qt::AlignTop);
                }
                attrWidget = new KKSVideoPlayer (av, isSystem);
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
QWidget * KKSAttributesFactory :: createAttrCheckWidget (const KKSAttrValue * av, const KKSAttrType *pCatType,  bool isSystem, QTabWidget * tabW)
{
    QWidget * attrWidget = 0;
    if (!av || !pCatType || !tabW)
        return 0;

    if (pCatType->attrType() != KKSAttrType::atCheckListEx)
        return 0;

    attrWidget = new KKSAttrCheckWidget (av, isSystem);//KKSListWidget (pCategAttr, isSystem);
    attrWidget->setMinimumHeight(40);
    QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    attrWidget->setSizePolicy(hPw);
    tabW->addTab (attrWidget, av->attribute()->title());
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
                                       bool isSystem, 
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
                if (pCatType->attrType() == KKSAttrType::atParent || pCatType->attrType() == KKSAttrType::atList)
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
                QString tableName = av->attribute()->tableName ();
                KKSAttrCheckWidget * arw = qobject_cast<KKSAttrCheckWidget *>(aw);
                if (!arw)
                    break;

                QStringList vArray = V.toStringList();
                KKSObject * refIO = loader->loadIO (tableName, true);
                if (!refIO)
                    break;
                KKSCategory * c = refIO->category ();
                if (!c)
                {
                    refIO->release ();
                    break;
                }
                KKSCategory * ct = c->tableCategory ();
                if (!ct)
                {
                    refIO->release ();
                    break;
                }
                KKSList<const KKSFilterGroup*> filters;
                QString vals;
                for (int i=0; i<vArray.count(); i++)
                {
                    vals += vArray[i];
                    if (i < vArray.count()-1)
                        vals += ",";
                }
                if (!vals.isEmpty())
                {
                    QString value = QString ("select id from %1 where id in (%2) ").arg (tableName).arg (vals);
                    //const KKSFilter * filter = ct->createFilter ("id", value, KKSFilter::foInSQL);
                    const KKSFilter * filter = ct->createFilter (1, value, KKSFilter::foInSQL);
                    KKSList <const KKSFilter *> fl;
                    fl.append (filter);
                    filter->release ();
                    KKSFilterGroup * fg = new KKSFilterGroup(true);
                    fg->setFilters(fl);
                    filters.append(fg);
                    fg->release();
                }
                //QMap<int, QString> values = loader->loadAttributeValues (pCategAttr, true, tableName, filters);
                //qDebug () << __PRETTY_FUNCTION__ << values;
                KKSMap<int, KKSCategoryAttr *> attrs = ct->attributes ();
                QStandardItemModel * sAttrModel = new QStandardItemModel (0, attrs.count());
                int ic = 0;
                for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = attrs.constBegin(); \
                                                                    pa != attrs.constEnd(); \
                                                                    pa++)
                {
                    sAttrModel->setHeaderData (ic, Qt::Horizontal, pa.value()->title(), Qt::DisplayRole);
                    ic++;
                }
                int ii=0;
                if (!vals.isEmpty())
                {
                    KKSMap<int, KKSEIOData *> eioList = loader->loadEIOList (refIO, filters);
                    //
                    // ��������� �������
                    //
                    for (KKSMap<int, KKSEIOData *>::const_iterator pv = eioList.constBegin(); pv!=eioList.constEnd(); pv++)
                    {
                        sAttrModel->insertRows (ii, 1);
                        QModelIndex saInd = sAttrModel->index (ii, 0);
                        //QString v = pv.value();
                        int key = pv.key();
                        ic = 0;
                        KKSObjectExemplar * wObjE = loader->loadEIO (pv.key(), refIO);

                        for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = attrs.constBegin(); \
                                                                            pa != attrs.constEnd(); \
                                                                            pa++)
                        {
                            QModelIndex saInd = sAttrModel->index (ii, ic);
                            KKSAttrValue * av = wObjE->attrValue (pa.value()->id());
                            QVariant val = av ? av->value().valueVariant () : QVariant();
                            if (av->attribute()->type()->attrType() == KKSAttrType::atJPG)
                                val = QObject::tr("<Image data %1>").arg (ii);
                            sAttrModel->setData (saInd, key, Qt::UserRole);
                            sAttrModel->setData (saInd, val, Qt::DisplayRole);
                            ic++;
                        }
                        wObjE->release ();
                        ii++;
                    }
                }
                //c->release ();
                refIO->release ();
                QObject :: connect (arw, \
                         SIGNAL (addAttrRef (const KKSAttrValue*,  bool, QAbstractItemModel*)), \
                         wEditor, \
                         SLOT (addAttributeCheckReference (const KKSAttrValue*, bool, QAbstractItemModel *)) \
                        );
                QObject :: connect (arw, \
                         SIGNAL (delAttrRef (const KKSAttrValue*, bool, QAbstractItemModel*, const QModelIndex&)), \
                         wEditor, \
                         SLOT (delAttributeCheckReference (const KKSAttrValue*, bool, QAbstractItemModel*, const QModelIndex&)) \
                        );
                QObject :: connect (arw, \
                         SIGNAL (refIOOpen (QString)), \
                         wEditor, \
                         SLOT (openReferenceIO (QString))
                        );
                arw->setModel (sAttrModel);
                aw->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
                if (!isRef)
                    connectToSlots (aw, wEditor);
            }
            break;
        case KKSAttrType::atObjRef:
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
                    qobject_cast<QDateTimeEdit *>(aw)->setTime (v);
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
            {
                double v = V.toDouble ();
                int vi = V.toInt ();
                KKSEdit *lEdit = qobject_cast<KKSEdit *>(aw);
                if (pCatType->attrType() == KKSAttrType::atDouble)
                    lEdit->setVal (QString::number (v));
                else
                    lEdit->setVal (QString::number (vi));

                if (!isRef)
                    connectToSlots (aw, wEditor);
                else
                {
                    if (pCatType->attrType() == KKSAttrType::atDouble)
                        lEdit->setText (QString::number (v, 'f', 16));
                    else
                        lEdit->setText (QString::number (vi));
                }
                lEdit->setVal (lEdit->text());
                lEdit->setEnabled (!isObjExist || !av->attribute()->isReadOnly());
            }
            break;
        case KKSAttrType::atString: 
        case KKSAttrType::atFixString:
        case KKSAttrType::atUrl:
        case KKSAttrType::atMaclabel:
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
    }
}

void KKSAttributesFactory :: connectToSlots (QObject *aw, QWidget* wEditor)
{
//    if (aw->metaObject ()->indexOfSignal (SIGNAL (valueChanged(int, bool, QVariant))) >= 0)
    QObject::connect (aw, SIGNAL (valueChanged(int, bool, QVariant)), wEditor, SLOT (setValue (int, bool, QVariant)) );
}

void KKSAttributesFactory :: slotLoadIOSrc (KKSObject ** io)
{
    if (!io)
        return;

    KKSList<const KKSFilterGroup *> filters;
    
    QWidget * parentW = qobject_cast<QWidget *>(this->sender());
    
    KKSObjEditor* ioSrc = m_oef->createObjRecEditor(IO_IO_ID, IO_IO_ID, filters, tr ("Select document"), 0, true, Qt::WindowModal, parentW);
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
