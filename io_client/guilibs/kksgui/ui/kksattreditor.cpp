#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegExp>
#include <QRegExpValidator>
#include <QSpacerItem>
#include <QMap>
#include <QMultiMap>
#include <QMessageBox>
#include <QtDebug>

#include <KKSValue.h>
#include <KKSAttrType.h>
#include <KKSAttribute.h>
#include <KKSObject.h>
#include <KKSRecWidget.h>
#include <KKSSearchTemplate.h>
#include "KKSSortFilterProxyModel.h"
#include "KKSAttrModel.h"
#include "kksattreditor.h"
#include "ui_kksattr_editor.h"

KKSAttrEditor :: KKSAttrEditor (KKSAttribute *attr, 
                                const KKSMap<int, KKSAttrType *>& aTypes,
                                const KKSMap<int, KKSAGroup*>& aGroups,
                                const QMap<int, QString>& Refs, 
                                QWidget *parent)
    : QDialog (parent),
    attribute (attr),
    attrTypes (aTypes),
    attrsGroups (aGroups),
    ioRefs (Refs),
    ui (new Ui::kksattr_editor),
    io (0),
    searchTemplate(NULL),
    m_recW(new KKSRecWidget(false, Qt::Vertical, this))
{
    if (attribute)
        attribute->addRef ();

    ui->setupUi (this);

    QRegExp cRegExp ("^([a-z])+([a-z0-9_])*");
    QRegExpValidator * vAttrCode = new QRegExpValidator (cRegExp, this);
    //ui->lECode->setValidator (vAttrCode);
    ui->leFilter->setReadOnly(true);
    QGridLayout * gExtLay = new QGridLayout;
    ui->tabExtended->setLayout (gExtLay);
    gExtLay->addWidget (m_recW, 0, 0, 1, 1);
    
    m_recW->hideActionGroup(_ID_FILTER_GROUP);
    for (int i=2; i<=_GROUP_COUNT; i++)
        m_recW->hideActionGroup(i);

    if (attribute && attribute->id() >= 0)
    {
        //
        // ������������ �������
        //
        ui->lETitle->setText (attribute->title());
        ui->cbType->setEnabled (false);
        ui->lEName->setText (attribute->name());
        ui->lECode->setText (attribute->code());
        ui->lEDefWidth->setText (QString::number (attribute->defWidth()));
        ui->lECode->setReadOnly (true);
        if(attribute->searchTemplate()){
            ui->leFilter->setText(attribute->searchTemplate()->name());
            searchTemplate = new KKSSearchTemplate(*(attribute->searchTemplate()));
        }
        ui->chManually->setCheckState (Qt::Checked);
        setCodeEnabled (Qt::Checked);
        bool isComplex (attribute->type()->attrType()==KKSAttrType::atComplex);
        ui->attrTabW->setTabEnabled(1, isComplex);
        if (isComplex)
        {
            QSortFilterProxyModel * sortModel = new KKSSortFilterProxyModel();
            QAbstractItemModel * attrModel = new KKSAttrModel (attribute);
            sortModel->setSourceModel (attrModel);
            m_recW->setEIOModel (sortModel);
        }

//        ui->pbAddAttrs->setText(tr("Show extended attributes"));
    }
    else
    {
        ui->lEDefWidth->setText (QString::number (100));
        setCodeEnabled (Qt::Unchecked);
//        ui->pbAddAttrs->setText(tr("Add extended attributes"));
    }

    QIntValidator *vDefWidthVal = new QIntValidator (0, 300, this);
    ui->lEDefWidth->setValidator (vDefWidthVal);

    connect (ui->lEName, SIGNAL (textEdited(const QString&)), this, SLOT (setTitleText (const QString&)) );
    connect (ui->cbType, SIGNAL (currentIndexChanged(int)), this, SLOT (currentAttrTypeChanged(int)) );
    connect (ui->cbRefName, SIGNAL (currentIndexChanged(int)), this, SLOT (currentReferenceChanged(int)) );
    connect (ui->chManually, SIGNAL (stateChanged (int)), this, SLOT (setCodeEnabled (int)) );
    connect (ui->tbAddFilter, SIGNAL (clicked()), this, SLOT (slotAddFilterClicked()) );
    connect (ui->tbDelFilter, SIGNAL (clicked()), this, SLOT (slotDelFilterClicked()) );
    connect (m_recW, SIGNAL (addEntity (QAbstractItemModel *, const QModelIndex&)), this, SLOT(addTriggered(QAbstractItemModel *, const QModelIndex&)));
    connect (m_recW, SIGNAL (editEntity(QAbstractItemModel *, const QModelIndex&)), this, SLOT(editTriggered(QAbstractItemModel *, const QModelIndex& )));
    connect (m_recW, SIGNAL (delEntity(QAbstractItemModel *, const QModelIndex&)), this, SLOT(delTriggered(QAbstractItemModel *, const QModelIndex&)));
//    connect (ui->pbAddAttrs, SIGNAL(clicked()), this, SLOT(addAttrs()));
    
    cRefTypes << KKSAttrType::atList 
              << KKSAttrType::atParent 
              << KKSAttrType::atCheckList 
              << KKSAttrType::atCheckListEx 
              << KKSAttrType::atRecordColorRef
              << KKSAttrType::atRecordTextColorRef
              << KKSAttrType::atSysChildCategoryRef
              << KKSAttrType::atHistogram; //ksa


    this->init_widgets ();

    //--ksa
    if(attribute && 
       attribute->id() >= 0 && 
       attribute->type()->attrType() == KKSAttrType::atParent )
    {
        ui->cbRefName->setCurrentIndex(-1);
        ui->cbRefName->setEnabled(false);
        ui->cbRefField->setEditable(true);
        ui->cbRefField->setEditText(attribute->columnName());
    }

    if(attribute && 
       attribute->id() >= 0 && 
       attribute->type()->attrType() == KKSAttrType::atHistogram)
    {
        ui->cbRefName->setCurrentIndex(-1);
        ui->cbRefName->setEnabled(true);
        ui->cbRefField->setEditText(tr("��"));
        ui->cbRefField->setEnabled(false);
    }

    //--ksa

    ui->pbOK->setDefault (true);
    ui->pbOK->setAutoDefault (true);
    QKeySequence enter_key (Qt::Key_Enter);
    ui->pbOK->setShortcut (enter_key);
    QKeySequence cancel_key (Qt::Key_Escape);
    ui->pbCancel->setShortcut (cancel_key);

    connect (ui->pbOK, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (ui->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSAttrEditor :: ~KKSAttrEditor (void)
{
    if (attribute)
        attribute->release ();

    if (io)
        io->release ();

    if(searchTemplate)
        searchTemplate->release();
}

void KKSAttrEditor :: accept ()
{
    int cType = ui->cbType->itemData (ui->cbType->currentIndex()).toInt();
    bool isRef (cRefTypes.contains (cType));
    if (isRef && ui->cbRefName->currentIndex () < 0 )
    {
        if(cType != KKSAttrType::atParent){//3 - ��� ��������/�������
            qWarning() << tr ("Select reference");
            QMessageBox::warning (this, tr ("Attribute edit"), tr ("Select reference"), QMessageBox::Ok);
            return;
        }
    }

    if (isRef )
    {
        if(cType == KKSAttrType::atParent && ui->cbRefField->currentText().isEmpty()){//��������/�������
            qWarning() << tr ("Set reference field");
            QMessageBox::warning (this, tr ("Attribute edit"), tr ("Set reference field"), QMessageBox::Ok);
            return;
        }
        else if(cType == KKSAttrType::atHistogram){
            int a = 0;
        }
        else if(cType != KKSAttrType::atParent && ui->cbRefField->currentIndex () < 0){
            qWarning() << tr ("Select reference field");
            QMessageBox::warning (this, tr ("Attribute edit"), tr ("Select reference field"), QMessageBox::Ok);
            return;
        }
    }

    QDialog::accept ();
    if (!attribute)
        return;

    attribute->setTitle (ui->lETitle->text());
    attribute->setName (ui->lEName->text());
    
    if (ui->chManually->checkState () == Qt::Checked && !ui->lECode->isReadOnly ())
        attribute->setCode (ui->lECode->text());
    else if (!ui->lECode->isReadOnly ())
        attribute->setCode (QString());
    
    attribute->setType (attrTypes.value (cType) );
    
    if (attribute->type ()->attrType() == KKSAttrType::atList ||
        attribute->type ()->attrType() == KKSAttrType::atCheckList ||
        attribute->type ()->attrType() == KKSAttrType::atCheckListEx ||
        attribute->type ()->attrType() == KKSAttrType::atRecordColorRef ||
        attribute->type ()->attrType() == KKSAttrType::atRecordTextColorRef ||
        attribute->type ()->attrType() == KKSAttrType::atSysChildCategoryRef )
    {
        attribute->setTableName (io->tableName());
        attribute->setColumnName (ui->cbRefField->itemData (ui->cbRefField->currentIndex()).toString());
    }
    if (attribute->type ()->attrType() == KKSAttrType::atHistogram)
    {
        attribute->setTableName (io->tableName());
        attribute->setColumnName ("id");
    }
    if (attribute->type ()->attrType() == KKSAttrType::atParent)
    {
        attribute->setTableName ("");
        attribute->setColumnName (ui->cbRefField->currentText());
    }

    //
    // ��� ������������ �������� ��� ������� ���� �� �����,
    // � ��� �������� ������ �������� ���������� ��������������� ������.
    //
    attribute->setDefWidth (ui->lEDefWidth->text().toInt());

    if(isRef)
        attribute->setSearchTemplate(searchTemplate);
    else
        attribute->setSearchTemplate(NULL);
}

void KKSAttrEditor :: currentAttrTypeChanged (int index)
{
    if (index < 0 || !ui->cbType->isEnabled())
        return;

    //qDebug () << __PRETTY_FUNCTION__ << index;
    int cType = ui->cbType->itemData (index, Qt::UserRole).toInt();
    bool isRef (cRefTypes.contains (cType));
    ui->gbRef->setEnabled (isRef);

    bool isHist (cType==KKSAttrType::atHistogram);
    if (isHist)
        emit loadHistRefs (attribute, cType, this);

    //--ksa
    if(isRef){
        if(cType == KKSAttrType::atParent){//��������/�������
            ui->cbRefName->setCurrentIndex(-1);
            ui->cbRefName->setEnabled(false);
            ui->cbRefField->setEditable(true);
            ui->cbRefField->setEditText("name");
        }
        else if(cType == KKSAttrType::atHistogram){
            ui->cbRefName->setCurrentIndex(-1);
            ui->cbRefName->setEnabled(true);
            ui->cbRefField->setEditText(tr("��"));
            ui->cbRefField->setEnabled(false);
        }
        else{
            ui->cbRefName->setCurrentIndex(-1);
            ui->cbRefName->setEnabled(true);
            ui->cbRefField->setEditText("");
            ui->cbRefField->setEditable(false);

        }
    }
    //--ksa
    bool isComplex (cType==KKSAttrType::atComplex);
    if (isComplex && !this->m_recW->getSourceModel())
    {
        QAbstractItemModel * attrMod = new KKSAttrModel (this->attribute);
        QAbstractProxyModel * sortAttrMod = new KKSSortFilterProxyModel();
        sortAttrMod->setSourceModel(attrMod);
        m_recW->setEIOModel(sortAttrMod);
    }

    ui->attrTabW->setTabEnabled(1, isComplex);
}

void KKSAttrEditor :: init_widgets (void)
{
    this->set_types ();
    this->set_references ();
    this->set_groups ();
}

KKSAttribute * KKSAttrEditor :: getAttribute (void)
{
    return attribute;
}

const KKSAttribute * KKSAttrEditor :: getAttribute (void) const
{
    return attribute;
}

void KKSAttrEditor :: set_types (void)
{
    ui->cbType->clear ();
    int ind=-1;
    KKSMap<int, KKSAttrType *>::const_iterator p;
    int i=0;
    int cType = ( attribute && attribute->id() >= 0 ? attribute->type ()->attrType() : 9);
    bool isRef (cRefTypes.contains (cType));
    ui->gbRef->setEnabled (isRef);
    QMap<QString, int> type_titles;
    for (p = attrTypes.constBegin(); p != attrTypes.constEnd(); p++)
        type_titles.insert (p.value()->name(), p.key());

    QMap<QString, int>::const_iterator pt = type_titles.constBegin();
    for (; pt != type_titles.constEnd(); pt++)
    {
        ui->cbType->addItem (pt.key(), pt.value());
        if (pt.value () == cType && ind < 0)
            ind = i;
        i++;
    }
    ui->cbType->setCurrentIndex (ind);
}

void KKSAttrEditor :: set_groups (void)
{
    ui->cbAttrGroup->clear ();
    KKSMap<int, KKSAGroup *>::const_iterator pg;
    for (pg = attrsGroups.constBegin(); pg != attrsGroups.constEnd(); pg++)
        ui->cbAttrGroup->addItem (pg.value()->name(), pg.key());
}

int KKSAttrEditor :: getGroupId (void) const
{
    int ind = ui->cbAttrGroup->currentIndex ();
    if (ind >= 0)
        return ui->cbAttrGroup->itemData (ind).toInt();
    else
        return -1;
}

void KKSAttrEditor :: setGroupId (int idGroup)
{
    int ind = ui->cbAttrGroup->findData(QVariant(idGroup));
    if (ind >= 0)
        ui->cbAttrGroup->setCurrentIndex (ind);
}

int KKSAttrEditor :: getTypeID (void) const
{
    return ui->cbType->itemData (ui->cbType->currentIndex ()).toInt();
}

void KKSAttrEditor :: set_references (void)
{
    ui->cbRefName->clear ();
//    ui->cbRefName->setCurrentIndex (-1);
    QMultiMap<QString, int> refs;
    QMap<int, QString>::const_iterator pRefs;

    for (pRefs = ioRefs.constBegin(); pRefs != ioRefs.constEnd(); pRefs++)
        refs.insertMulti (pRefs.value(), pRefs.key());

    for (QMultiMap<QString, int>::const_iterator p=refs.constBegin(); p != refs.constEnd(); p++)
        for (int i=0; i<refs.values (p.key()).count(); i++)
            ui->cbRefName->addItem (p.key(), refs.values(p.key()).at(i));

    if (attribute->id() <= 0 )//����� �������
        ui->cbRefName->setCurrentIndex (-1);
    else if (cRefTypes.contains ( getTypeID() ) && getTypeID() != 3)
        emit getReferences (attribute, this);
}

void KKSAttrEditor :: currentReferenceChanged (int index)
{
    int idRef = ui->cbRefName->itemData (index, Qt::UserRole).toInt();
    if (index >= 0)
        emit getReferenceFields (attribute, idRef, this);
}

void KKSAttrEditor :: uploadReferenceFields (const QMap<QString, QString>& fields)
{
    ui->cbRefField->clear ();
    QMultiMap<QString, QString> fieldList;

    for (QMap<QString, QString>::const_iterator pf = fields.constBegin(); pf != fields.constEnd(); pf++)
        fieldList.insertMulti (pf.value(), pf.key());
    
    int ind = -1;
    int i=0;
    for (QMultiMap<QString, QString>::const_iterator p = fieldList.constBegin(); p != fieldList.constEnd(); p++)
    {
        if (fieldList.values (p.key()).contains (attribute->columnName()))
            ind = i;
        i++;
        for (int i=0; i<fieldList.values (p.key()).count(); i++)
            ui->cbRefField->addItem (p.key(), fieldList.values (p.key()).at (i));
    }

    if (attribute->id() <= 0)
        ui->cbRefField->setCurrentIndex (-1);
    else if (io && ind >=0 )
        ui->cbRefField->setCurrentIndex (ind);
    else
        ui->cbRefField->setCurrentIndex (-1);
}

const QMap<int, QString>& KKSAttrEditor :: getReferences (void) const
{
    return ioRefs;
}

void KKSAttrEditor :: setReferences (const QMap<int, QString>& refs)
{
    ioRefs = refs;
    this->set_references();
}

void KKSAttrEditor :: setIO (KKSObject *o)
{
    if (io)
        io->release ();

    io = o;

    if (io)
        io->addRef ();
}

void KKSAttrEditor :: setCurrentRef (int index)
{
    ui->cbRefName->setCurrentIndex (index);
    ui->cbRefName->setEnabled (false);
}

void KKSAttrEditor :: setCurrentRef (const QString& text)
{
    int id = ui->cbRefName->findText (text);
    this->setCurrentRef (id);
}

void KKSAttrEditor :: setCodeEnabled (int state)
{
    ui->lECode->setEnabled (state == Qt::Checked);
}

void KKSAttrEditor :: setSearchTemplate(KKSSearchTemplate * st)
{
    if(!st){
        if(searchTemplate)
            searchTemplate->release();
        searchTemplate = NULL;
        ui->leFilter->setText("");

        return;
    }

    if(searchTemplate)
        searchTemplate->release();
    searchTemplate = st;
    searchTemplate->addRef();

    ui->leFilter->setText(st->name());
}

void KKSAttrEditor :: slotAddFilterClicked()
{
    emit getSearchTemplate(this);
}

void KKSAttrEditor :: slotDelFilterClicked()
{
    ui->leFilter->setText("");
    if(searchTemplate)
        searchTemplate->release();
    
    searchTemplate = NULL;
}

void KKSAttrEditor :: setTitleText (const QString& text)
{
    QString titleStr = ui->lETitle->text ();
    //int oldLength = titleStr.length ();
    //int newLength = text.length ();
    //int nstr = oldLength < newLength ? oldLength : newLength;
    if (text.startsWith(titleStr) || text.endsWith(titleStr) ||
        titleStr.isEmpty())
        ui->lETitle->setText (text);
}

void KKSAttrEditor::addAttrs()
{
    emit showAttrsWidget(attribute, this);
}

void KKSAttrEditor::setRecWidget(KKSRecWidget* recW)
{
    m_recW = recW;
}

void KKSAttrEditor::addTriggered(QAbstractItemModel * sourceModel, const QModelIndex& parent)
{
    qDebug () << __PRETTY_FUNCTION__ << parent;
    emit addAttribute(attribute, sourceModel, this);
}

void KKSAttrEditor::editTriggered(QAbstractItemModel * sourceMod, const QModelIndex& wIndex)
{
    //int answer = QMessageBox::question(this, tr(""), tr(), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    //if(answer == QMessageBox::Yes)
    int idAttr = wIndex.sibling(wIndex.row(), 0).data(Qt::UserRole).toInt();
    emit editAttribute(idAttr, attribute, sourceMod, wIndex, this);
}

void KKSAttrEditor::delTriggered(QAbstractItemModel * sourceMod, const QModelIndex& wIndex)
{
    int answer = QMessageBox::question(this, tr("Remove attribute"), tr("Do you really want to remove selected attribute from list?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if(answer == QMessageBox::Yes)
    {
        int idAttr = wIndex.sibling(wIndex.row(), 0).data(Qt::UserRole).toInt();
        emit delAttribute(idAttr, attribute, sourceMod, wIndex, this);
    }
}

void KKSAttrEditor::checkForHistogramRefs()
{
    if(attribute && 
       attribute->id() >= 0 && 
       attribute->type()->attrType() == KKSAttrType::atHistogram)
    {
        emit loadHistRefs (attribute, attribute->type()->attrType(), this);
    }
}
