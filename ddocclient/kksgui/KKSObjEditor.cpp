/***********************************************************************
 * Module:  KKSObjEditor.cpp
 * Author:  yuriy, sergey
 * Modified: 19 декабря 2008 г. 19:00:00
 * Purpose: Implementation of the class KKSObjEditor
 * Comment: Данный класс унаследован от KKSDialog
 * и предназначен для визуализации и редактирования ИО или ЭИО
 ***********************************************************************/

#include <QPushButton>
#include <QGridLayout>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QSizePolicy>
#include <QToolButton>
#include <QTextEdit>
#include <QModelIndex>
#include <QItemSelectionModel>
#include <QHeaderView>
#include <QKeySequence>
#include <QEventLoop>
#include <QPointer>
#include <QScrollArea>
#include <QMessageBox>
#include <QCloseEvent>
#include <QModelIndex>
#include <QItemSelection>
#include <QToolBar>
#include <QTabWidget>
#if QT_VERSION >= 0x040600
#include <QXmlSchema>
#endif
#include <QtDebug>

#include "KKSRecWidget.h"
#include "ui/kksfilewidget.h"
#include "kksincludeswidget.h"
#include "KKSTemplate.h"
#include "KKSObjectExemplar.h"
#include "KKSObject.h"
#include "KKSPrinter.h"
#include "KKSObjEditor.h"
#include "KKSAttrType.h"
#include "KKSCheckBox.h"
#include "KKSComboBox.h"
#include "KKSDateEdit.h"
#include "KKSDateTimeEdit.h"
#include "KKSEdit.h"
#include "KKSCategory.h"
#include "KKSState.h"
#include "KKSCategoryAttr.h"
#include <KKSAddTable.h>
#include <KKSAccessEntity.h>
#include <KKSIndAttr.h>
#include <KKSAttrValue.h>
#include <KKSIndicator.h>
#include <KKSRubric.h>
#include "KKSAttrUUIDWidget.h"
#include "defines.h"


KKSObjEditor :: KKSObjEditor (const KKSTemplate *t, 
                              const KKSTemplate * _ioTemplate,
                              const KKSTemplate * rTemplate,
                              const KKSCategory * ioCat,
                              const KKSList<const KKSFilterGroup*> & filters,
                              const QString & extraTitle,
                              bool mode, 
                              KKSObjectExemplar *objE, 
                              KKSObject * obj,
                              bool isIO,
                              QToolBar * tBar,
                              QWidget *parent, 
                              Qt::WindowFlags f)
    : KKSDialog (parent, f),
    m_sysTemplate (t),
    m_ioTemplate (_ioTemplate),
    m_recTemplate (rTemplate),
    pObjectEx (objE),
    pObj(obj),
    pCat (ioCat),
    currTablename (QString()),
    nTable (-1),
    m_isIO(isIO),
    m_idJournal(-1),
    m_draft (false),
    m_showExecButton(false),
    indNumW (-1),
    numCopies (1),
    pbOk (mode ? new QPushButton (tr("&OK")) : 0),
    pbCancel (mode ? new QPushButton (tr("&Cancel")) : 0),
    pbApply (mode ? new QPushButton (tr("&Apply")) : 0),
    tbActions (tBar),
    mainLayout (new QGridLayout()),
    tabObj (0),
    tabEnclosures (0),
    sysAttrWidget (0),
    scSysAttrs (0),
    sysAttrsW (0),
    ioAttrWidget (0),
    scIOAttrs (0),
    ioAttrsW (0),
    stateWidget (0),
    recWidget (0),
    fileWidget(0),
    includesWidget(0),
    includesRecWidget (0),
    isChanged (false),
    pRecModel (0)
{
    if (m_sysTemplate)
        m_sysTemplate->addRef ();

    if (m_ioTemplate)
        m_ioTemplate->addRef ();

    if (m_recTemplate)
        m_recTemplate->addRef ();

    if (pObjectEx)
        pObjectEx->addRef ();
    
    if (pObj)
        pObj->addRef();

    if (pCat)
        pCat->addRef ();

    m_filters = filters;

    this->setLayout (mainLayout);
    
    if(pObj){
        QString title;
        if(filters.count() == 0){
            title = pObj->name();
            if(title.isEmpty()){
                title = tr("New information object");
                if(m_ioTemplate){
                    title += QString(" (") + m_ioTemplate->name() + ")";
                }
            }
        }
        else{
            if(!extraTitle.isEmpty()){
                title = QString("%1 (%2)").arg(pObj->name()).arg(extraTitle);
            }
            else
                title = QString("%1 (%2)").arg(pObj->name()).arg(tr("Filters applied"));
        }
        setWindowTitle(title);
    }
    else{
        QString title;
        if (pObjectEx){
            title = pObjectEx->name();
            if(title.isEmpty()){
                title = tr("Record #%1").arg(pObjectEx->id());
            }
            if(pObjectEx->io())
                title += " - (" + pObjectEx->io()->name() + ")";
        }
        setWindowTitle(title);
    }

    if (!m_sysTemplate)
        return;

    if (mode)
    {
        connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
        pbOk->setDefault (true);
        pbOk->setAutoDefault (true);
        connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
        QKeySequence cancel_key (Qt::Key_Escape);
        pbCancel->setShortcut (cancel_key);
        connect (pbApply, SIGNAL (clicked()), this, SLOT (apply()) );
    }
}

KKSObjEditor :: ~KKSObjEditor()
{
    if (m_sysTemplate)
        m_sysTemplate->release();

    if (m_ioTemplate)
        m_ioTemplate->release();

    if (m_recTemplate)
        m_recTemplate->release ();

    if (pObjectEx)
        pObjectEx->release();

    if (pObj)
        pObj->release();

    if (pCat)
        pCat->release ();

    this->clearAttributes ();
//    this->clearCbList ();
}

const KKSTemplate* KKSObjEditor :: sysTemplate (void) const
{
    return m_sysTemplate;
}

void KKSObjEditor :: setSysTemplate (const KKSTemplate *_sysT)
{
    if (m_sysTemplate == _sysT)
        return;

    if (m_sysTemplate)
        m_sysTemplate->release ();

    m_sysTemplate = _sysT;

    if (m_sysTemplate)
        m_sysTemplate->addRef ();
}

const KKSTemplate* KKSObjEditor :: ioTemplate (void) const
{
    return m_ioTemplate;
}

void KKSObjEditor :: setIoTemplate (const KKSTemplate *_ioT)
{
    if (m_ioTemplate == _ioT)
        return;

    if (m_ioTemplate)
        m_ioTemplate->release ();

    m_ioTemplate = _ioT;

    if (m_ioTemplate)
        m_ioTemplate->addRef ();
}

const KKSTemplate* KKSObjEditor :: recTemplate (void) const
{
    return m_recTemplate;
}

void KKSObjEditor :: setRecTemplate (const KKSTemplate *_rT)
{
    if (m_recTemplate == _rT)
        return;

    if (m_recTemplate)
        m_recTemplate->release ();

    m_recTemplate = _rT;

    if (m_recTemplate)
        m_recTemplate->addRef ();
}

void KKSObjEditor :: set_object_ex_data (KKSObjectExemplar* ioObjectE)
{
    if (pObjectEx)
        pObjectEx->release();

    pObjectEx = ioObjectE;

    if (pObjectEx)
        pObjectEx->addRef ();
}

void KKSObjEditor :: setObj (KKSObject * ioObj)
{
    if (pObj)
        pObj->release();

    pObj= ioObj;

    if (pObj)
        pObj->addRef ();
}

KKSObjectExemplar* KKSObjEditor :: getObjectEx (void)
{
    return pObjectEx;
}

KKSObject* KKSObjEditor :: getObj(void)
{
    return pObj;
}

void KKSObjEditor :: accept (void)
{
    if (constructObject () == ERROR_CODE)
        return;
    this->saveToDb (numCopies);
    if (!isChanged)
    {
        KKSDialog::accept ();
        close ();
    }
}

int KKSObjEditor :: constructObject()
{
    KKSList<KKSAttrValue*> avalList;
    KKSMap<int, KKSAttrValue*>::const_iterator pSysAttrValues;
    
    for (pSysAttrValues=sysAttrValues.constBegin (); pSysAttrValues != sysAttrValues.constEnd(); pSysAttrValues++)
    {
         const KKSAttrValue *cAttrValue = pSysAttrValues.value();
        //int idAttr = cAttr->id();
        KKSValue v;
        /*if (cAttr->id() == 97)
            qDebug () << __PRETTY_FUNCTION__ << sysAttributesValues.value (pSysAttrs.key());*/
        if (!chSysOptWidgets.contains (pSysAttrValues.key()) || chSysOptWidgets.value (pSysAttrValues.key())->isEnabled())
        {
            //
            // Mandatory or enabled optional parameter
            //
            //qWarning() << cAttr->code();
            QString value;
            int type = cAttrValue->attribute()->type()->attrType();
            QVariant val = cAttrValue->value().valueVariant(); //sysAttributesValues.value (pSysAttrs.key());
            if (type == KKSAttrType::atCheckList || type == KKSAttrType::atCheckListEx)
            {
                value = val.toStringList().join(",");
                //qDebug () << __PRETTY_FUNCTION__ << cAttrValue->attribute()->id() << value;
            }
            else if (type == KKSAttrType::atRecordColor || type == KKSAttrType::atRecordTextColor)
            {
                QColor v_col = val.value<QColor>();
                //qDebug () << __PRETTY_FUNCTION__ << v_col << v_col.value() << v_col.rgba() << (pObj ? pObj->recordFillColor() : QString());
                if (v_col.isValid())
                {
                    QRgb v_rgba = v_col.rgba ();
                    //value = QString ("(%1)::bigint").arg (v_rgba);
                    value = QString ("%1").arg (v_rgba);
                }
                else
                    value = QString();//::number (v_col.value());
            }
            else if (type == KKSAttrType::atXMLDoc && !val.isNull() && !val.toByteArray().isEmpty())
            {
#if QT_VERSION >= 0x040600
                /*--ksa 
                //Данный код не работает! Даже при поавильном XML выдается, что инвалидный контент
                QXmlSchema * xmlSchema = new QXmlSchema ();
                if (!xmlSchema)
                    return ERROR_CODE;
                ////qDebug () << __PRETTY_FUNCTION__ << val << val.toByteArray();
                xmlSchema->load (val.toByteArray());
                if (!xmlSchema->isValid())
                {
                    QMessageBox::warning (this, tr ("XML Document"), tr ("Invalid xml contents"), QMessageBox::Ok, QMessageBox::NoButton);
                    return ERROR_CODE;
                }
                delete xmlSchema;
                */
#endif
                value = val.toString();
            }
            else if (type == KKSAttrType::atDate)// || type== KKSAttrType::atDateTime)
            {
                if (val.toDate().isValid())
                    value = val.toDate().toString (Qt::ISODate);
                else
                    value = QString ();
            }
            else if (type == KKSAttrType::atDateTime)
            {
                if (val.toDateTime().isValid())
                    value = val.toDateTime().toString (Qt::ISODate);
                else
                    value = QString ();
            }
            else if (type == KKSAttrType::atTime)
            {
                if (val.toTime().isValid())
                    value = val.toTime().toString (Qt::ISODate);
                else
                    value = QString ();
            }
            else if (type == KKSAttrType::atIntervalH)
                value = QString ("{%1}").arg (val.toStringList().join(","));
            else
                value = val.toString(); 

            if (type == KKSAttrType::atUUID && (val.isNull() || value.isEmpty()) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id())) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id()))->checkState() != Qt::Checked)
            {
                int res = QMessageBox::question(this, tr ("Set UUID"), tr ("Unique identificator does not set. Generate automatically (Yes) or set manually (No) ?"), QMessageBox::Yes | QMessageBox::No);
                if (res == QMessageBox::No)
                    return ERROR_CODE;
                else
                    emit generateUUID (cAttrValue->attribute()->id(), cAttrValue);
            }
            else if (type == KKSAttrType::atUUID && (val.isNull() || value.isEmpty()) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id())) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id()))->checkState() == Qt::Checked)
                emit generateUUID (cAttrValue->attribute()->id(), cAttrValue);
            if (cAttrValue->attribute()->isMandatory() && cAttrValue->attribute()->defValue().value().isEmpty() && (val.isNull() || value.isEmpty()))
            {
                QMessageBox::warning (this, tr ("Save document"), tr ("Mandatory attribute %1 has to be set").arg (cAttrValue->attribute()->name()), QMessageBox::Ok);
                return ERROR_CODE;
            }
            else if (cAttrValue->attribute()->isMandatory() && (val.isNull() || value.isEmpty())){
                if(type == KKSAttrType::atCheckList ||
                   type == KKSAttrType::atCheckListEx)
                {
                    value = cAttrValue->attribute()->defValue().valueVariant().toStringList().join(",");
                }
                else
                    value = cAttrValue->attribute()->defValue().value();
            }
            else if (!cAttrValue->attribute()->isMandatory() && (val.isNull() || value.isEmpty()))
            {
                if(type == KKSAttrType::atCheckList || 
                   type == KKSAttrType::atCheckListEx)
                {
                    value = cAttrValue->attribute()->defValue().valueVariant().toStringList().join(",");
                }
                else
                    value = QString();
            }

            v = KKSValue(value, type);
            if (type == KKSAttrType::atList ||
                type == KKSAttrType::atParent ||
                type == KKSAttrType::atRecordColorRef ||
                type == KKSAttrType::atRecordTextColorRef)
            {
                v.setColumnValue (cAttrValue->value().columnValue());
                //qDebug () << __PRETTY_FUNCTION__ << v.value() << v.columnValue();
            }
        }
        else
            v = cAttrValue->attribute()->defValue ();

        KKSAttrValue *av = new KKSAttrValue (*cAttrValue);
        if (av->attribute()->code(false)==QString("id_maclabel"))
            qDebug () << __PRETTY_FUNCTION__ << v.valueForInsert() << v.isNull();
        av->setValue(v);
        
        avalList.append (av);
    }

    /*
    if(pObjectEx->attrValue(ATTR_TABLE_NAME) != NULL){
        QString tableName;
        //QString ioCode;
        tableName = pObjectEx->attrValue(ATTR_TABLE_NAME)->value().value();
        //ioCode = pObjectEx->attrValue("code")->value().value();
        
        pObjectEx->setAttrValues (avalList);
        
        KKSAttrValue * a = pObjectEx->attrValue(ATTR_TABLE_NAME);
        if(a != NULL){
            //if(a->value().value().trimmed().isEmpty()){
                KKSValue v(tableName, KKSAttrType::atString);
                a->setValue(v);
            //}
        }

        //a = pObjectEx->attrValue("code");
        //if(a != NULL){
            //if(a->value().value().trimmed().isEmpty()){
        //        KKSValue v(ioCode, KKSAttrType::atString);
        //        a->setValue(v);
            //}
        //}
    }
    else{*/
        pObjectEx->setAttrValues (avalList);
    //}

    KKSList<KKSAttrValue*> avalIndList;
    KKSMap<int, KKSAttrValue*>::const_iterator pIndAttrValues;
    
    for (pIndAttrValues=ioIndicatorValues.constBegin (); pIndAttrValues != ioIndicatorValues.constEnd(); pIndAttrValues++)
    {
        const KKSAttrValue *cAttrValue = pIndAttrValues.value();
        //int idAttr = cAttr->id();
        KKSValue v;
        /*if (cAttr->id() == 97)
            qDebug () << __PRETTY_FUNCTION__ << sysAttributesValues.value (pSysAttrs.key());*/
        if (!chIndOptWidgets.contains (pIndAttrValues.key()) || chIndOptWidgets.value (pIndAttrValues.key())->isEnabled())
        {
            //
            // Mandatory or enabled optional parameter
            //
            //qWarning() << cAttr->code();
            QString value;
            int type = cAttrValue->attribute()->type()->attrType();
            QVariant val = cAttrValue->value().valueVariant(); //sysAttributesValues.value (pSysAttrs.key());
            if (type == KKSAttrType::atCheckList || type == KKSAttrType::atCheckListEx)
            {
                value = val.toStringList().join(",");
                //qDebug () << __PRETTY_FUNCTION__ << cAttrValue->attribute()->id() << value;
            }
            else if (type == KKSAttrType::atRecordColor || type == KKSAttrType::atRecordTextColor)
            {
                QColor v_col = val.value<QColor>();
                //qDebug () << __PRETTY_FUNCTION__ << v_col << v_col.value() << v_col.rgba() << (pObj ? pObj->recordFillColor() : QString());
                if (v_col.isValid())
                {
                    QRgb v_rgba = v_col.rgba ();
                    //value = QString ("(%1)::bigint").arg (v_rgba);
                    value = QString ("%1").arg (v_rgba);
                }
                else
                    value = QString();//::number (v_col.value());
            }
            else if (type == KKSAttrType::atXMLDoc && !val.isNull() && !val.toByteArray().isEmpty())
            {
#if QT_VERSION >= 0x040600
                /*--ksa 
                //Данный код не работает! Даже при поавильном XML выдается, что инвалидный контент
                QXmlSchema * xmlSchema = new QXmlSchema ();
                if (!xmlSchema)
                    return ERROR_CODE;
                ////qDebug () << __PRETTY_FUNCTION__ << val << val.toByteArray();
                xmlSchema->load (val.toByteArray());
                if (!xmlSchema->isValid())
                {
                    QMessageBox::warning (this, tr ("XML Document"), tr ("Invalid xml contents"), QMessageBox::Ok, QMessageBox::NoButton);
                    return ERROR_CODE;
                }
                delete xmlSchema;
                */
#endif
                value = val.toString();
            }
            else if (type == KKSAttrType::atDate)// || type== KKSAttrType::atDateTime)
            {
                if (val.toDate().isValid())
                    value = val.toDate().toString (Qt::ISODate);
                else
                    value = QString ();
            }
            else if (type == KKSAttrType::atDateTime)
            {
                if (val.toDateTime().isValid())
                    value = val.toDateTime().toString (Qt::ISODate);
                else
                    value = QString ();
            }
            else if (type == KKSAttrType::atTime)
            {
                if (val.toTime().isValid())
                    value = val.toTime().toString (Qt::ISODate);
                else
                    value = QString ();
            }
            else if (type == KKSAttrType::atIntervalH)
                value = QString ("{%1}").arg (val.toStringList().join(","));
            else
                value = val.toString(); 

            qDebug () << __PRETTY_FUNCTION__ << val << value;
            if (type == KKSAttrType::atUUID && (val.isNull() || value.isEmpty()) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id())) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id()))->checkState() != Qt::Checked)
            {
                int res = QMessageBox::question(this, tr ("Set UUID"), tr ("Unique identificator does not set. Generate automatically (Yes) or set manually (No) ?"), QMessageBox::Yes | QMessageBox::No);
                if (res == QMessageBox::No)
                    return ERROR_CODE;
                else
                    emit generateUUID (cAttrValue->attribute()->id(), cAttrValue);
            }
            else if (type == KKSAttrType::atUUID && (val.isNull() || value.isEmpty()) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id())) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id()))->checkState() == Qt::Checked)
                emit generateUUID (cAttrValue->attribute()->id(), cAttrValue);
            if (cAttrValue->attribute()->isMandatory() && cAttrValue->attribute()->defValue().value().isEmpty() && (val.isNull() || value.isEmpty()))
            {
                QMessageBox::warning (this, tr ("Save document"), tr ("Mandatory attribute %1 has to be set").arg (cAttrValue->attribute()->name()), QMessageBox::Ok);
                return ERROR_CODE;
            }
            else if (cAttrValue->attribute()->isMandatory() && (val.isNull() || value.isEmpty())){
                if(type == KKSAttrType::atCheckList ||
                   type == KKSAttrType::atCheckListEx)
                {
                    value = cAttrValue->attribute()->defValue().valueVariant().toStringList().join(",");
                }
                else
                    value = cAttrValue->attribute()->defValue().value();
            }
            else if (!cAttrValue->attribute()->isMandatory() && (val.isNull() || value.isEmpty()))
            {
                if(type == KKSAttrType::atCheckList || 
                   type == KKSAttrType::atCheckListEx)
                {
                    value = cAttrValue->attribute()->defValue().valueVariant().toStringList().join(",");
                }
                else
                    value = QString();
            }

            v = KKSValue(value, type);
            if (type == KKSAttrType::atList ||
                type == KKSAttrType::atParent ||
                type == KKSAttrType::atRecordColorRef ||
                type == KKSAttrType::atRecordTextColorRef)
            {
                v.setColumnValue (cAttrValue->value().columnValue());
            }
        }
        else
            v = cAttrValue->attribute()->defValue ();

        KKSAttrValue *av = new KKSAttrValue (*cAttrValue);
        qDebug () << __PRETTY_FUNCTION__ << v.valueForInsert() << v.isNull();
        av->setValue(v);
        
        avalIndList.append (av);
    }
    pObjectEx->setIndValues (avalIndList);
    if(fileWidget)
        pObjectEx->setFiles(fileWidget->files());
    //
    // добавляем вложения
    //
    if (includesRecWidget)
        pObjectEx->setRootRubric(includesRecWidget->rootRubric());

    //
    // если редактируемый ЭИО является ИО, то надо сохранить его атрибуты
    // а также задать ему его системные атрибуты, фактически преобразовав ЭИО в ИО
    //
    // а также сохранить прикрепленные файлы
    //int idAuthor = pObj ? pObj->author() : -1;
    if (pObj)
    {
        //qDebug () << __PRETTY_FUNCTION__ << idAuthor;
        KKSList<KKSAttrValue*> avalList;
        KKSMap<int, KKSAttrValue*>::const_iterator pIOAttrValues;

        for (pIOAttrValues=ioAttrValues.constBegin(); pIOAttrValues != ioAttrValues.constEnd(); pIOAttrValues++)
        {
            const KKSAttrValue *cAttrValue = pIOAttrValues.value();
            //KKSCategoryAttr* cAttr = const_cast<KKSCategoryAttr*> (pIOAttrs.value());//m_ioAttrWidgets[i]->getCategoryAttr ());
            QVariant val = cAttrValue->value().valueVariant();

            QString value;
            int type = cAttrValue->attribute()->type()->attrType();
            if(type == KKSAttrType::atCheckList ||
                type == KKSAttrType::atCheckListEx)
                value = val.toStringList().join(","); 
            else if (type == KKSAttrType::atRecordColor || type == KKSAttrType::atRecordTextColor)
            {
                QColor v_col = val.value<QColor>();
                if (v_col.isValid())
                {
                    QRgb v_rgba = v_col.rgba ();
                    //value = QString ("(%1)::bigint").arg (v_rgba);
                    value = QString ("%1").arg (v_rgba);
                }
                else
                    value = QString();
            }
            else if (type == KKSAttrType::atXMLDoc && !val.isNull() && !val.toByteArray().isEmpty())
            {
#if QT_VERSION >= 0x040600
                /*--ksa 
                //Данный код не работает! Даже при поавильном XML выдается, что инвалидный контент                
                QXmlSchema * xmlSchema = new QXmlSchema ();
                if (!xmlSchema)
                    return ERROR_CODE;
                ////qDebug () << __PRETTY_FUNCTION__ << val << val.toByteArray();
                xmlSchema->load (val.toByteArray());
                if (!xmlSchema->isValid())
                {
                    QMessageBox::warning (this, tr ("XML Document"), tr ("Invalid xml contents"), QMessageBox::Ok, QMessageBox::NoButton);
                    return ERROR_CODE;
                }
                delete xmlSchema;
                */
#endif
                value = val.toString();
            }
            else if (type == KKSAttrType::atDate)// || type== KKSAttrType::atDateTime)
            {
                if (val.toDate().isValid())
                    value = val.toDate().toString (Qt::ISODate);
                else
                    value = QString ();
            }
            else if (type == KKSAttrType::atDateTime)
            {
                if (val.toDateTime().isValid())
                    value = val.toDateTime().toString (Qt::ISODate);
                else
                    value = QString ();
            }
            else if (type == KKSAttrType::atTime)
            {
                if (val.toTime().isValid())
                    value = val.toTime().toString (Qt::ISODate);
                else
                    value = QString ();
            }
            else if (type == KKSAttrType::atIntervalH)
                value = QString ("{%1}").arg (val.toStringList().join(","));
            else
                value = val.toString(); 
            
            if (type == KKSAttrType::atUUID && (val.isNull() || value.isEmpty()) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id())) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id()))->checkState() != Qt::Checked)
            {
                int res = QMessageBox::question(this, tr ("Set UUID"), tr ("Unique identificator does not set. Generate automatically (Yes) or set manually (No) ?"), QMessageBox::Yes | QMessageBox::No);
                if (res == QMessageBox::No)
                    return ERROR_CODE;
                else
                    emit generateUUID (cAttrValue->attribute()->id(), cAttrValue);
            }
            else if (type == KKSAttrType::atUUID && (val.isNull() || value.isEmpty()) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id())) && qobject_cast<KKSAttrUUIDWidget *>(awAttrs.value(cAttrValue->id()))->checkState() == Qt::Checked)
                emit generateUUID (cAttrValue->attribute()->id(), cAttrValue);
            if (cAttrValue->attribute()->isMandatory() && cAttrValue->attribute()->defValue().value().isEmpty() && (val.isNull() || value.isEmpty()))
            {
                QMessageBox::warning (this, tr ("Save document"), tr ("Mandatory attribute %1 has to be set").arg (cAttrValue->attribute()->name()), QMessageBox::Ok);
                return ERROR_CODE;
            }
            else if (cAttrValue->attribute()->isMandatory() && (val.isNull() || value.isEmpty())){
                if(type == KKSAttrType::atCheckList || 
                   type == KKSAttrType::atCheckListEx)
                    value = cAttrValue->attribute()->defValue().valueVariant().toStringList().join(",");
                else
                    value = cAttrValue->attribute()->defValue().value();
            }
            else if (!cAttrValue->attribute()->isMandatory() && (val.isNull() || value.isEmpty()))
            {
                //
                // При заполнении пользовательских атрибутов необходимо, чтобы в запросе не писалось NULL-значение,
                // поэтому идет заполнение пустой, но не null-строкой
                //
                if(type == KKSAttrType::atCheckList ||
                    type == KKSAttrType::atCheckListEx)
                    value = cAttrValue->attribute()->defValue().valueVariant().toStringList().join(",");
                else
                    value = QString("");
            }

            KKSValue v (value, type);
            if (type == KKSAttrType::atList ||
                type == KKSAttrType::atParent ||
                type == KKSAttrType::atRecordColorRef ||
                type == KKSAttrType::atRecordTextColorRef)
            {
                v.setColumnValue (cAttrValue->value().columnValue());
                //qDebug () << __PRETTY_FUNCTION__ << v.value() << v.columnValue();
            }
            //qDebug () << __PRETTY_FUNCTION__ << type << v.valueForInsert() << value;
            
            KKSAttrValue * av = new KKSAttrValue (*cAttrValue); 
            av->setValue(v);
            avalList.append (av);
            av->release();
        }

        pObj->setAttrValues(avalList);

/*
        KKSList<KKSIndicatorValue *> rIVals;
        for (int i=0; i<pObj->indicatorValues().count(); i++)
        {
            int idInd = pObj->indicatorValues().at (i)->id();
            const KKSIndicator * ind = pObj->indicatorValue(i)->indicator();//ioIndicators.value (idInd);
            int type = ind->type()->attrType();
            QString v = ioIndicatorValues.value (idInd).toString();
            qDebug () << __PRETTY_FUNCTION__ << v << idInd;
            KKSValue val (v, type);
            KKSIndicatorValue * iVal = new KKSIndicatorValue(idInd, const_cast<KKSIndicator *>(ind), val);
            rIVals.append (iVal);
        }

        pObj->setIndicatorValues (rIVals);
*/        
        
        //
        // добавляем файлы
        //
        if (fileWidget)
            pObj->setFiles(fileWidget->files());

        //
        // добавляем вложения
        //
        if (includesWidget)
            pObj->setRootRubric(includesWidget->rootRubric());

        //emit saveObj (this, pObj, pObjectEx, num, pRecModel);
        //emit ioChanged();
    }
    return OK_CODE;

}

int KKSObjEditor :: apply (int num)
{
    Q_UNUSED (num);
    int res = constructObject();
    if (res == ERROR_CODE)
        return res;
    this->saveToDb (numCopies);// : num);
    return res;
}

void KKSObjEditor :: saveToDb (int num)
{
    bool isMain = false;
    if (!m_isIO)
    {
        //qDebug () << __PRETTY_FUNCTION__ << (pCat ? pCat->id () : -1) << pObjectEx->io()->category()->id () << currTablename << pObjectEx->io()->tableName() << nTable;
        if (pCat && pCat->id () == pObjectEx->io()->category()->id ())
            isMain = true;
        emit saveObjE (this, pObjectEx, (isMain ? 0 : pCat), (isMain ? QString() : currTablename), num, pRecModel);
    }

    //
    // если редактируемый ЭИО является ИО, то надо сохранить его атрибуты
    // а также задать ему его системные атрибуты, фактически преобразовав ЭИО в ИО
    //
    // а также сохранить прикрепленные файлы
    if (pObj)
    {
        emit saveObj (this, pObj, pObjectEx, num, pRecModel);
        //emit ioChanged();
    }

}

void KKSObjEditor :: recSaved (KKSObjectExemplar * rec)
{
    //
    // Это надо вызывать всегда после сохранения в БД
    //
    if (rec->id() != pObjectEx->id())
        return;
    KKSMap<qint64, KKSObjectExemplar *> idL;
    if (!additionalCopies.contains (rec->id()))
        idL.insert (rec->id(), rec);
    for (KKSMap<qint64, KKSObjectExemplar *>::const_iterator p=additionalCopies.constBegin();
            p != additionalCopies.constEnd();
            p++)
        idL.insert (p.key(), p.value());
    //idL += additionalCopies;
    emit eioChanged (rec->io(), idL, pRecModel);//pObjectEx->id());
}

void KKSObjEditor :: save (int num)
{
    apply (num);
}

void KKSObjEditor :: saveAsCommandResult()
{
    if(!pObj)
        return;

    if(m_idJournal <= 0)
        return;

    int res = constructObject();
    if (res == ERROR_CODE)
        return;

    emit saveObjAsCommandResult(this, pObj, pObjectEx, 1, pRecModel);
/*
    //
    // Это надо вызывать всегда
    //
    KKSMap<qint64, KKSObjectExemplar *> idL;
    if (!additionalCopies.contains (pObjectEx->id()))
        idL.insert (pObjectEx->id(), pObjectEx);
    for (KKSMap<qint64, KKSObjectExemplar *>::const_iterator p=additionalCopies.constBegin();
            p != additionalCopies.constEnd();
            p++)
        idL.insert (p.key(), p.value());
    //idL += additionalCopies;
    emit eioChanged (pObjectEx->io(), idL, pRecModel);//pObjectEx->id());
 */
/*    QList<qint64> idL;
    if (!additionalCopies.contains (pObjectEx->id()))
        idL.append (pObjectEx->id());
    idL += additionalCopies;
    emit eioChanged (idL, 0, QString(), 0, pRecModel);//pObjectEx->id());*/
}

void KKSObjEditor::print()
{
    constructObject();
    
    KKSPrinter printer;
    if(!m_isIO){
        printer.print(pObjectEx);
    }

    if(pObj){
        if(pObj->attrValueId(ATTR_SHB) == NULL)
            printer.print(pObj);
        else{
            emit printReport(pObj);
            //printReport(pObj);
        }
    }
}

void KKSObjEditor :: filterObjectE (void)
{
    //для ЭИО, которые имеют подчиненную таблицу, 
    //и соответственно для которых доступна данная операция
    //идентификатор ЭИО является идентификатором ИО
    if(!pObjectEx)
        return;

    int idObject = pObjectEx->id();
    const KKSCategory * c = 0;
    int i = tabEnclosures ? tabEnclosures->currentIndex () : 0;
    QString tabName = QString ();
    qDebug () << __PRETTY_FUNCTION__ << i;
    if (i == 0)
    {
        c = 0;//pObjectEx->io()->category()->tableCategory ();
        tabName = pObj->tableName ();
    }
    else if (i <= addCats.count())
    {
        c = addCats [i-1];
        KKSMap<int, KKSAddTable*>::const_iterator p = addTables.constBegin()+i-1;
        tabName = p.value()->getTableName();
    }
    qDebug () << __PRETTY_FUNCTION__ << (c ? c->id () : -1) << pCat->id () << tabName;
    emit filterObjectEx (this, idObject, c, tabName);
}

void KKSObjEditor :: refreshRecModel (QAbstractItemModel * sourceMod)
{
    //для ЭИО, которые имеют подчиненную таблицу, 
    //и соответственно для которых доступна данная операция
    //идентификатор ЭИО является идентификатором ИО
    if(!pObjectEx || !sourceMod)
        return;

    int idObject = pObjectEx->id();
    const KKSCategory * c = 0;
    int i = tabEnclosures ? tabEnclosures->currentIndex () : 0;
    QString tabName = QString ();
    qDebug () << __PRETTY_FUNCTION__ << i;
    if (i == 0)
    {
        c = 0;//pObjectEx->io()->category()->tableCategory ();
        tabName = pObj->tableName ();
    }
    else if (i <= addCats.count())
    {
        c = addCats [i-1];
        KKSMap<int, KKSAddTable*>::const_iterator p = addTables.constBegin()+i-1;
        tabName = p.value()->getTableName();
    }
    emit refreshObjectEx (this, idObject, c, tabName, sourceMod);
}

void KKSObjEditor :: filterObjectT (void)
{
    //для ЭИО, которые имеют подчиненную таблицу,
    //и соответственно для которых доступна данная операция
    //идентификатор ЭИО является идентификатором ИО
    if(!pObjectEx)
        return;

    int idObject = pObjectEx->id();
    const KKSCategory * c = 0;
    int i = tabEnclosures ? tabEnclosures->currentIndex () : 0;
    QString tabName = QString ();
    qDebug () << __PRETTY_FUNCTION__ << i;
    if (i == 0)
    {
        c = 0;//pObjectEx->io()->category()->tableCategory ();
        tabName = pObj->tableName ();
    }
    else if (i <= addCats.count())
    {
        c = addCats [i-1];
        KKSMap<int, KKSAddTable*>::const_iterator p = addTables.constBegin()+i-1;
        tabName = p.value()->getTableName();
    }
    qDebug () << __PRETTY_FUNCTION__ << (c ? c->id () : -1) << pCat->id () << tabName;
    emit filterObjectTemplateEx (this, idObject, c, tabName);
    
}

void KKSObjEditor :: addObjectE (void)
{
    this->clearW ();
    //для ЭИО, которые имеют подчиненную таблицу, 
    //и соответственно для которых доступна данная операция
    //идентификатор ЭИО является идентификатором ИО
    if(!pObjectEx)
        return;

    int idObject = pObjectEx->id();
    const KKSCategory * c = 0;
    int i = tabEnclosures ? tabEnclosures->currentIndex () : 0;
    QString tabName = QString ();
    qDebug () << __PRETTY_FUNCTION__ << i;
    if (i == 0)
    {
        c = pCat;//pObjectEx->io()->category()->tableCategory ();
        tabName = pObj->tableName ();
    }
    else if (i <= addCats.count())
    {
        c = addCats [i-1];
        KKSMap<int, KKSAddTable*>::const_iterator p = addTables.constBegin();
        p += i-1;
        tabName = (p != addTables.constEnd() && p.value ()) ? p.value()->getTableName() : QString();
    }
    qDebug () << __PRETTY_FUNCTION__ << (c ? c->id () : -1) << (pCat ? pCat->id () : -1) << tabName;
    KKSRecWidget * rw = qobject_cast<KKSRecWidget *>(this->sender()->parent());
    QAbstractItemModel * sRecModel (0);
    if (rw)
        sRecModel = rw->getSourceModel ();
            //(tabEnclosures->currentWidget());
    //Q_UNUSED (rw);
    emit newObjectEx (this, idObject, c , tabName, i, false, sRecModel);
}

void KKSObjEditor :: editObjectE (void)
{
    this->clearW ();
    if (!pObjectEx)
        return;

    int idObjectE=-1;
    int i = tabEnclosures ? tabEnclosures->currentIndex () : 0;
    QModelIndex recIndex;
    if (i == 0 && recWidget && recWidget->getID() > 0)
    {
        idObjectE = recWidget->getID();//index.data(Qt::UserRole).toInt();
        editRows.insert (idObjectE, recWidget->getSourceIndex().row());
        recIndex = recWidget->getSourceIndex();
    }
    else if (i > 0 && i <= addCats.count())
    {
        idObjectE = addRecWidgets[i-1]->getID ();
        editRows.insert (idObjectE, addRecWidgets[i-1]->getSourceIndex().row());
        recIndex = addRecWidgets[i-1]->getSourceIndex();
    }
    else
        return;

/*--ksa
    QModelIndexList selIndexes = selModel->selectedRows ();

    if (selIndexes.count () != 1)
        return;
*/
    //--ksaQModelIndex wIndex = selIndexes[0];
    //--ksaint idObjectE = wIndex.data (Qt::UserRole).toInt ();
    
//--ksa start
    //if(selModel->selectedRows ().count() != 1)
    //    return;

    //return;

    //int idObjectE = selModel->selectedRows()[0].data (Qt::UserRole).toInt ();
//--ksa end

    //для ЭИО, которые имеют подчиненную таблицу, 
    //и соответственно для которых доступна данная операция
    //идентификатор ЭИО является идентификатором ИО
    int idObject = pObjectEx->id();
    const KKSCategory * c = 0;
    QString tabName = QString ();
    qDebug () << __PRETTY_FUNCTION__ << i;
    if (i == 0)
    {
        c = pCat;//pObjectEx->io()->category()->tableCategory ();
        tabName = pObj->tableName ();
    }
    else if (i <= addCats.count())
    {
        c = addCats [i-1];
        KKSMap<int, KKSAddTable*>::const_iterator p = addTables.constBegin()+i-1;
        tabName = p.value()->getTableName();
    }
    qDebug () << __PRETTY_FUNCTION__ << c->id () << pCat->id () << tabName;
    KKSRecWidget * rw = qobject_cast<KKSRecWidget *>(this->sender()->parent());
    QAbstractItemModel * sRecModel (0);
    if (rw)
        sRecModel = rw->getSourceModel ();
    emit editObjectEx (this, idObject, idObjectE, c, tabName, i, false, sRecModel, recIndex);
}

void KKSObjEditor :: delObjectE (void)
{
    QMessageBox::StandardButton res = QMessageBox::question (this, 
                                                             tr ("Delete"), 
                                                             tr ("Do you really want to delete ?"), 
                                                             QMessageBox::Yes | 
                                                             QMessageBox::No,
                                                             QMessageBox::No);
    if (res != QMessageBox::Yes)
        return;
    this->clearW ();
    if (!recWidget || recWidget->getID() <= 0)
        return;

    QList<qint64> idObjRecs;//recWidget->getID();//wIndex.data (Qt::UserRole).toInt ();

    QModelIndexList delInds;
    QAbstractItemModel * recModel (0);
    int i = tabEnclosures ? tabEnclosures->currentIndex () : 0;
    if (i == 0 && recWidget)
    {
        recModel = recWidget->getSourceModel ();
        delInds = recWidget->getSourceIndexes();
    }
    else if (i <= addCats.count())
    {
        recModel = addRecWidgets[i-1]->getSourceModel ();
        delInds = addRecWidgets[i-1]->getSourceIndexes();
    }

    for (int ii=0; ii<delInds.count(); ii++)
    {
        int idObjE = delInds[ii].data (Qt::UserRole).toInt ();
        if (idObjE > 0 && !idObjRecs.contains (idObjE))
            idObjRecs.append (idObjE);
    }

    //для ЭИО, которые имеют подчиненную таблицу, 
    //и соответственно для которых доступна данная операция
    //идентификатор ЭИО является идентификатором ИО
    int idObject = pObjectEx->id();
    QModelIndex dIndex;// = (i == 0 ? recWidget->getSourceIndex () : addRecWidgets[i-1]->getSourceIndex ()) ;
/*    if (recModel->rowCount(dIndex) > 0)
    {
        QMessageBox::warning (this, tr ("Delete record"), tr("Error ! Record contains children. Removing is impossible."), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
 */
    QString tabName = QString ();
    qDebug () << __PRETTY_FUNCTION__ << i;
    if (i == 0)
        tabName = pObj->tableName ();
    else if (i <= addCats.count())
    {
        KKSMap<int, KKSAddTable*>::const_iterator p = addTables.constBegin()+i-1;
        tabName = p.value()->getTableName();
    }
    qDebug () << __PRETTY_FUNCTION__ << tabName;
    for (int ii=0; ii<idObjRecs.count(); ii++)
    {
        QModelIndexList dInds = (i == 0 ? recWidget->getSourceIndexes () : addRecWidgets[i-1]->getSourceIndexes ());
        dIndex = dInds.at(0);
        dIndex = dIndex.sibling (dIndex.row(), 0);
        qDebug () << __PRETTY_FUNCTION__ << dIndex;
        emit delObjectEx (this, idObject, idObjRecs[ii], tabName, recModel, dIndex);
    }
}

void KKSObjEditor :: saveChildObjE (KKSObjectExemplar *childObjE)
{
    this->clearW ();
    emit saveObjE (this, childObjE, pCat, currTablename, 1, pRecModel);
}

void KKSObjEditor :: setObjectEx (KKSObjectExemplar *newObj)
{
    pObjectEx = newObj;
    if (pObjectEx)
        pObjectEx->addRef ();
}

void KKSObjEditor :: updateEIOEx (KKSObject * refObj, const KKSMap<qint64, KKSObjectExemplar *>& recs, QAbstractItemModel * recModel)
{
    //this->clearW ();
    //для ЭИО, которые имеют подчиненную таблицу, 
    //и соответственно для которых доступна данная операция
    //идентификатор ЭИО является идентификатором ИО
    int idObject = pObjectEx->id();
    if (idObject < 0)
        return;
/*
    int i = tabEnclosures->currentIndex ();
    QAbstractItemModel *mod = (i==0 ? recWidget->getSourceModel () : addRecWidgets[i-1]->getSourceModel ());

    for (int i=0; i<idL.count(); i++)
    {
        qint64 id = idL[i];
        int wrow = editRows.value (id, -1);
        if (wrow < 0)
        {
            int num = -1;
            for (int i=0; i<mod->rowCount() && num<0; i++)
                if (mod->data (mod->index (i, 0), Qt::UserRole).toInt() == id)
                    num = i;
            if (num < 0 && id > 0)
            {
                mod->insertRows (mod->rowCount(), 1);
                num = mod->rowCount() - 1;
                mod->setData (mod->index (num, 0), id, Qt::UserRole);
            }
            editRows.insert (id, num);
            wrow = num;
        }
        qDebug () << __PRETTY_FUNCTION__ << id << pObjectEx->id() << wrow;
    }
 */
//    QString tabName = tableName;//QString ();
//    qDebug () << __PRETTY_FUNCTION__ << tabName << nTab;
    int nTab (-1);
    QModelIndexList selIndexes;// = (nTab == 0 ? recWidget->getSourceIndexes() : addRecWidgets[nTab-1]->getSourceIndexes());
    if (recWidget && recModel == recWidget->getSourceModel())
    {
        nTab = 0;
        selIndexes = recWidget->getSourceIndexes();
    }
    for (int i=0; i<addRecWidgets.count() && nTab < 0; i++)
        if (addRecWidgets[i] && recModel == addRecWidgets[i]->getSourceModel())
        {
            nTab = i+1;
            selIndexes = addRecWidgets[i]->getSourceIndexes();
        }
    QItemSelection sel;
    if (selIndexes.size() > 0)
        sel = QItemSelection (selIndexes[0], selIndexes[selIndexes.size()-1]);
    emit updateEIO (refObj, recs, recModel, sel);
 
}

void KKSObjEditor :: clearAdditionalCopies (void)
{
    additionalCopies.clear ();
}

void KKSObjEditor :: addAdditionalCopy (KKSObjectExemplar * rec)
{
    additionalCopies.insert (rec->id(), rec);
}

void KKSObjEditor :: paintEvent (QPaintEvent * event)
{
    QWidget::paintEvent (event);
    if (recWidget && !hAttrWidths.isEmpty())
        recWidget->resizeSections (hAttrWidths);
    this->clearW ();
 }

void KKSObjEditor :: clearAttributes (void)
{
    sysAttrValues.clear ();
    //sysAttributesValues.clear ();

    ioAttrValues.clear ();
    //ioAttributesValues.clear ();
}

void KKSObjEditor :: setSysAttrValue(KKSAttrValue * attrValue)
{
    int id = attrValue->id();
    attrValue->addRef ();
    if (sysAttrValues.contains (id))
    {
        //sysAttrValues.value (id)->release ();
        sysAttrValues.remove (id);
    }
    sysAttrValues.insert (id, attrValue);
    //sysAttributesValues.insert (id, val);
}

void KKSObjEditor :: setIOAttrValue(KKSAttrValue * av)
{
    int id = av->id ();
    av->addRef ();
    if (ioAttrValues.contains (id))
    {
        //ioAttrValues.value (id)->release ();
        ioAttrValues.remove (id);
    }
    ioAttrValues.insert (id, av);
}

KKSMap<int, KKSAttrValue*> & KKSObjEditor :: getSysAttrValues()
{
    return sysAttrValues;
}

KKSMap<int, KKSAttrValue*> & KKSObjEditor :: getIOAttrValues()
{
    return ioAttrValues;
}

KKSMap<int, KKSAttrValue*> & KKSObjEditor :: getRecAttrValues()
{
    return ioIndicatorValues;
}

void KKSObjEditor :: setValue (int idAttrValue, KKSIndAttr::KKSIndAttrClass sys, QVariant val)
{
    
    switch (sys)
    {
#ifdef Q_CC_MSVC
    case KKSIndAttr::KKSIndAttrClass::iacTableAttr: default:
#else
    case KKSIndAttr::iacTableAttr: default:
#endif
        {
            if (!sysAttrValues.contains (idAttrValue))
                return;

            KKSAttrValue * av = sysAttrValues[idAttrValue];
            KKSValue v(val.toString(), av->attribute()->type()->attrType());
            if (av->attribute()->type()->attrType() == KKSAttrType::atCheckList)
                v.setValue (QString("{%1}").arg (val.toStringList().join(",")), KKSAttrType::atCheckList);

            av->setValue(v);
            isChanged = true;
            break;
        }
#ifdef Q_CC_MSVC
        case KKSIndAttr::KKSIndAttrClass::iacIOUserAttr:
#else
        case KKSIndAttr::iacIOUserAttr:
#endif
        {
            if (!ioAttrValues.contains (idAttrValue))
                return;

            KKSAttrValue * av = ioAttrValues[idAttrValue];
            KKSValue v(val.toString(), av->attribute()->type()->attrType());
            if (av->attribute()->type()->attrType() == KKSAttrType::atCheckList)
                v.setValue (QString("{%1}").arg (val.toStringList().join(",")), KKSAttrType::atCheckList);
            qDebug () << __PRETTY_FUNCTION__ << val << val.toString() << v.value(); 
            av->setValue(v);
            isChanged = true;
            break;
        }
#ifdef Q_CC_MSVC
    case KKSIndAttr::KKSIndAttrClass::iacEIOUserAttr:
#else
    case KKSIndAttr::iacEIOUserAttr:
#endif
        {
            if (!ioIndicatorValues.contains (idAttrValue))
                return;
            KKSAttrValue * av = ioIndicatorValues [idAttrValue];
            KKSValue v(val.toString(), av->attribute()->type()->attrType());
            if (av->attribute()->type()->attrType() == KKSAttrType::atCheckList)
                v.setValue (QString("{%1}").arg (val.toStringList().join(",")), KKSAttrType::atCheckList);
            av->setValue(v);
            isChanged = true;
            break;
        }
    }
}

void KKSObjEditor :: generateIOUUID (int idAttrVal)
{
    qDebug () << __PRETTY_FUNCTION__ << idAttrVal;
    QToolButton *tb = qobject_cast<KKSAttrUUIDWidget *>(this->sender())->getButton();
    if (!tb)
        return;
    int id = tbMaps.value (tb);
    if (!listAttrValues.value (id))
        return;

    const KKSAttrValue * av = listAttrValues.value(id);
    if(!av)
        return;
    
    int idAttr = av->attribute()->id();
    emit generateUUID (idAttr, av);
}

void KKSObjEditor :: setIOUUID (QString uuid)
{
    emit setUUID (uuid);
}
/*
void KKSObjEditor :: addTbQList (QToolButton * tbView, int idIndicator)
{
    if (tbView && idIndicator > 0)
    {
        tbViews.insert (tbView, idIndicator);
        connect (tbView, SIGNAL (clicked()), this, SLOT (viewIndicator()) );
    }
}

void KKSObjEditor :: addTbEditList (QToolButton * tbEdit, int idIndicator)
{
    if (tbEdit && idIndicator > 0)
    {
        tbIEdit.insert (tbEdit, idIndicator);
        connect (tbEdit, SIGNAL (clicked()), this, SLOT (editIndicator()) );
    }
}

void KKSObjEditor :: addTbDelList (QToolButton * tbDel, int idIndicator)
{
    if (tbDel && idIndicator > 0)
    {
        this->tbDelInds.insert(tbDel, idIndicator);
        connect (tbDel, SIGNAL (clicked()), this, SLOT (delIndicator()) );
    }
}
 */

void KKSObjEditor :: clearOptionals (void)
{
    chIOOpt.clear ();
    chIOOptWidgets.clear ();
}

void KKSObjEditor :: clearSysOpts (void)
{
    chSysOpt.clear ();
    chSysOptWidgets.clear ();
}

void KKSObjEditor :: setOpt (int id, KKSIndAttr::KKSIndAttrClass isSystem, QCheckBox* ch)
{
    if (!ch)
        return;
#ifdef Q_CC_MSVC
    if (isSystem == KKSIndAttr::KKSIndAttrClass::iacTableAttr)
#else
    if (isSystem == KKSIndAttr::iacTableAttr)
#endif
        chSysOpt.insert (ch, id);
    else
        chIOOpt.insert (ch, id);

    connect (ch, SIGNAL (stateChanged(int)), this, SLOT (setEnableO(int)) );
}

void KKSObjEditor :: addOptWidget (int id, KKSIndAttr::KKSIndAttrClass isSystem, QWidget *w)
{
    QList<QCheckBox *> keys;
#ifdef Q_CC_MSVC
    if (isSystem == KKSIndAttr::KKSIndAttrClass::iacTableAttr)
#else
    if (isSystem == KKSIndAttr::iacTableAttr)
#endif
    {
        chSysOptWidgets.insert (id, w);
        keys = chSysOpt.keys (id);
    }
    else
    {
        chIOOptWidgets.insert (id, w);
        keys = chIOOpt.keys (id);
    }

    QCheckBox *ch = keys[0];
    if (!keys.isEmpty() 
            && ch 
            && w
            && qobject_cast<QCheckBox *>(ch) != 0)
    {
        bool isEnable (ch->checkState() == Qt::Checked);
        w->setEnabled (isEnable);
    }
}

void KKSObjEditor :: setEnableO (int state)
{
    qDebug () << __PRETTY_FUNCTION__ << state;
    QCheckBox * ch = qobject_cast<QCheckBox *>(this->sender());

    bool isEnable (state == Qt::Checked);

    int id;
    QList <QWidget *> wList;
    bool isSystem = chSysOpt.contains (ch);
    if (isSystem)
    {
        id = chSysOpt.value (ch);
        wList = chSysOptWidgets.values (id);
    }
    else
    {
        id = chIOOpt.value (ch);
        wList = chIOOptWidgets.values (id);
    }

    for (int i=0; i<wList.count(); i++)
        if (wList[i])
        {
            wList[i]->setEnabled (isEnable);
            if (qobject_cast<QDateTimeEdit *>(wList[i]))
            {
                QVariant val;
                if (qobject_cast<QDateEdit*>(wList[i]))
                {
                    QDateEdit * dEdit = qobject_cast<QDateEdit*>(wList[i]);
                    if (isEnable)
                    {
                        val = dEdit->date().isNull() ? QDate::currentDate() : dEdit->date();
                        dEdit->setDate (val.toDate());
                    }
                    else
                    {
                        val = QVariant ();
                        dEdit->setDate (QDate());
                    }
#ifdef Q_CC_MSVC
                    setValue (id, isSystem ? KKSIndAttr::KKSIndAttrClass::iacTableAttr : KKSIndAttr::KKSIndAttrClass::iacIOUserAttr, val);
#else
                    setValue (id, isSystem ? KKSIndAttr::iacTableAttr : KKSIndAttr::iacIOUserAttr, val);
#endif
                }
                else if (qobject_cast<QTimeEdit*>(wList[i]))
                {
                    QTimeEdit * dtEdit = qobject_cast<QTimeEdit*>(wList[i]);
                    if (isEnable)
                    {
                        val = dtEdit->time().isNull() ? QTime::currentTime() : dtEdit->time();
                        dtEdit->setTime (val.toTime());
                    }
                    else
                    {
                        val = QVariant ();
                        dtEdit->setTime (QTime());
                    }
#ifdef Q_CC_MSVC
                    setValue (id, isSystem ? KKSIndAttr::KKSIndAttrClass::iacTableAttr : KKSIndAttr::KKSIndAttrClass::iacIOUserAttr, val);
#else
                    setValue (id, isSystem ? KKSIndAttr::iacTableAttr : KKSIndAttr::iacIOUserAttr, val);
#endif
                }
                else
                {
                    QDateTimeEdit * dtEdit = qobject_cast<QDateTimeEdit*>(wList[i]);
                    if (isEnable)
                    {
                        val = dtEdit->dateTime().isNull() ? QDateTime::currentDateTime() : dtEdit->dateTime();
                        dtEdit->setDateTime (val.toDateTime());
                    }
                    else
                    {
                        val = QVariant ();
                        dtEdit->setDateTime (QDateTime());
                    }
#ifdef Q_CC_MSVC
                    setValue (id, isSystem ? KKSIndAttr::KKSIndAttrClass::iacTableAttr : KKSIndAttr::KKSIndAttrClass::iacIOUserAttr, val);
#else
                    setValue (id, isSystem ? KKSIndAttr::iacTableAttr : KKSIndAttr::iacIOUserAttr, val);
#endif
                }
            }
        }

}

void KKSObjEditor :: clearW (void)
{
    hAttrWidths.clear ();
}

void KKSObjEditor :: addWidth (int w)
{
    hAttrWidths.append (w);
}

const KKSList<const KKSFilterGroup*> & KKSObjEditor :: filters() const
{
    return m_filters;
}

void KKSObjEditor :: clearCbList (void)
{
    QMap<QToolButton*, int>::const_iterator ptb;
    for (ptb=tbMaps.constBegin(); ptb != tbMaps.constEnd(); ptb++)
    {
        QToolButton *tb = ptb.key();
        if (tb)
            delete tb;
    }
    tbMaps.clear ();

    QMap<int, QWidget*>::const_iterator pcb;
    for (pcb=awAttrs.constBegin(); pcb != awAttrs.constEnd(); pcb++)
        if (pcb.value())
            delete pcb.value();

    awAttrs.clear ();
//    KKSMap<int, const KKSCategoryAttr*>::const_iterator pAttrs;

//    for (pAttrs=listAttributes.constBegin(); pAttrs!=listAttributes.constEnd(); pAttrs++)
//        pAttrs.value ()->release ();
}

void KKSObjEditor :: addListAttrWidget (QToolButton *tb, QWidget *aw, const KKSAttrValue* av)
{
    int id = av->id();
    tbMaps.insert (tb, id);
    awAttrs.insert (id, aw);
    listAttrValues.insert (id, av);
}

void KKSObjEditor :: setList (void)
{
    QToolButton *tb = qobject_cast<QToolButton *>(this->sender());
    if (!tb)
        return;
    int id = tbMaps.value (tb);
    if (!listAttrValues.value (id))
        return;

    const KKSAttrValue * av = listAttrValues.value(id);
    if(!av)
        return;

    QString tableName = av->attribute()->tableName ();
    if(tableName.isEmpty()){
        if(av->attribute()->type()->attrType() == KKSAttrType::atParent){
            tableName =  pObjectEx->io()->tableName();
        }
    }

    if(tableName.isEmpty())
        return;

    QWidget *awAttr = awAttrs.value (id);
    qDebug () << __PRETTY_FUNCTION__ << id << tableName << this << awAttr;
    emit loadAttrRef (tableName, awAttr, av->attribute()->id());//awAttr->getIdAttr());

//    emit newObjectEx (this, refObj->id(), 0, true);
}

void KKSObjEditor :: setStateList (void)
{
    emit loadStateRef (this, stateWidget, pObjectEx);
}

void KKSObjEditor :: childWidget (KKSObjEditor *editor)
{
    QObject *sObj = this->sender ();
    if (sObj)
        disconnect (this);
    editor->setFocus (Qt::MouseFocusReason);
    qDebug () << __PRETTY_FUNCTION__ << editor << this;//editor->isHidden() << editor->windowModality();
//    if (pEventL && pEventL->isRunning ())
//        pEventL->exit (0);
//    if (editor->pEventL && editor->pEventL->isRunning ())
//        editor->pEventL->exit (0);
//    qDebug () << __PRETTY_FUNCTION__ << pEventL << editor->pEventL;

    if (editor->exec () == QDialog::Accepted)
    {
        qDebug () << __PRETTY_FUNCTION__;
    }
//    } while (editor->res == QDialog::Accepted || editor->res == QDialog::Rejected);
    //editor->hide();//
    //editor->show ();
}

void KKSObjEditor :: setRecordsWidget (KKSRecWidget *rw)
{
    if (recWidget)
    {
        delete recWidget;
        recWidget = 0;
    }
    recWidget = rw;
}

void KKSObjEditor :: addRecordsWidget (KKSRecWidget *rw)
{
    if (!recWidget)
        setRecordsWidget (rw);

    if (rw)
    {
        addRecWidgets.append (rw);
        //rw->setParent (this);
    }
}
void KKSObjEditor :: addAdditionalCategory (const KKSCategory * wCat)
{
    if (wCat)
        addCats.append (wCat);
}

void KKSObjEditor :: addAdditionalTable (KKSAddTable * wTable)
{
    if (wTable)
        addTables.insert (wTable->id(), wTable);
}

int KKSObjEditor :: addCatsCount (void) const
{
    return addCats.count ();
}

void KKSObjEditor :: setAdditionalTables (const KKSMap<int, KKSAddTable*>& wTables)
{
    if (!addTables.isEmpty())
    {
        for (KKSMap<int, KKSAddTable*>::const_iterator pt=addTables.constBegin(); \
                pt != addTables.constEnd(); \
                pt++)
        {
            KKSAddTable *t = pt.value ();
            if (t)
                t->release ();
        }
        addTables.clear ();
    }
    addTables = wTables;
}

void KKSObjEditor :: addFileWidget (KKSFileWidget *fw)
{
    if (fileWidget)
    {
        delete fileWidget;
        fileWidget = 0;
    }
    fileWidget = fw;
    fileWidget->setParent(this);
    connect(fileWidget, SIGNAL(fileListChanged()), this, SLOT(filesChanged()));
}

void KKSObjEditor :: addIncludesWidget (KKSIncludesWidget *iw)
{
    if (includesWidget)
    {
        delete includesWidget;
        includesWidget = 0;
    }
    includesWidget = iw;
    includesWidget->setParent(this);
    //connect (includesWidget, SIGNAL(rubricItemRequested()), this, SLOT(slotIncludeRequested()));
    //connect (includesWidget, SIGNAL(openRubricItemRequested(int)), this, SLOT(slotOpenRubricItemRequested(int)));
    connect (includesWidget, SIGNAL (rubricsChanged()), this, SLOT (rubricsChanged()) );
    connect (this, SIGNAL(rubricItemSelected(int, QString)), includesWidget, SLOT(slotAddRubricItem(int, QString)));
}

void KKSObjEditor :: addIncludesRecWidget (KKSIncludesWidget *iw)
{
    if (includesRecWidget)
    {
        delete includesRecWidget;
        includesRecWidget = 0;
    }
    includesRecWidget = iw;
    includesRecWidget->setParent(this);
    //connect (includesRecWidget, SIGNAL(rubricItemRequested()), this, SLOT(slotIncludeRecRequested()));
    connect (includesRecWidget, SIGNAL(openRubricItemRequested(int)), this, SLOT(slotOpenRubricItemRecRequested(int)));
    connect (includesRecWidget, SIGNAL (rubricsChanged()), this, SLOT (rubricsChanged()) );
    //connect (this, SIGNAL(rubricItemRecSelected(int, QString)), includesRecWidget, SLOT(slotAddRubricItem(int, QString)));
}

void KKSObjEditor::slotIncludeSelected(int idObject, QString name)
{
    emit rubricItemSelected(idObject, name);
}

void KKSObjEditor::slotIncludeRequested()
{
    emit includeRequested(this);
}

void KKSObjEditor::slotOpenRubricItemRequested(int idObject)
{
    emit openRubricItemRequested(idObject, this);
}

void KKSObjEditor::slotIncludeRecSelected(int idObjectE, int idObject, QString name)
{
    qDebug () << __PRETTY_FUNCTION__ << idObjectE << idObject << name; 
    emit rubricItemRecSelected (idObjectE, name);
    Q_UNUSED (idObject);
}

void KKSObjEditor::slotIncludeRecRequested()
{
    qDebug () << __PRETTY_FUNCTION__;
    emit includeRecRequested(this);
}

void KKSObjEditor::slotOpenRubricItemRecRequested(int idObjectE)
{
    qDebug () << __PRETTY_FUNCTION__ << idObjectE;
    emit openRubricItemRecRequested(idObjectE, this);
}

void KKSObjEditor :: setView (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (pObj)
        emit setTemplate (this, pObj);
}
void KKSObjEditor :: setIdJournal(int idJournal)
{
    m_idJournal = idJournal;
}

int KKSObjEditor :: idJournal() const
{
    return m_idJournal;

}

void KKSObjEditor :: setIOAttrWidgets (QWidget *wIOAttr, QScrollArea *scIOattr, QWidget *ioAttrs)
{
    if (ioAttrWidget && ioAttrWidget != wIOAttr)
    {
        ioAttrWidget->setParent (0);
        delete ioAttrWidget;
    }
    ioAttrWidget = wIOAttr;
    if (scIOAttrs && scIOAttrs != scIOattr)
    {
        scIOAttrs->setParent (0);
        scIOAttrs->setWidget (0);
        delete scIOAttrs;
    }
    scIOAttrs = scIOattr;
    scIOAttrs->setWidget (0);
    if (ioAttrsW && ioAttrsW != ioAttrs)
    {
        ioAttrsW->setParent (0);
        delete ioAttrsW;
    }
    ioAttrsW = ioAttrs;
    scIOAttrs->setWidget (ioAttrsW);
}

void KKSObjEditor :: setRecIndWidgets (QWidget *w, QScrollArea *sc, QWidget *indW)
{
    if (recIndWidget && recIndWidget != w)
    {
        recIndWidget->setParent (0);
        delete recIndWidget;
    }
    recIndWidget = w;
    if (scRecInd && scRecInd != sc)
    {
        scRecInd->setParent (0);
        scRecInd->setWidget (0);
        delete scRecInd;
    }
    scRecInd = sc;
    scRecInd->setWidget (0);
    if (recIndW && recIndW != indW)
    {
        recIndW->setParent (0);
        delete recIndW;
    }
    recIndW = indW;
    scRecInd->setWidget (recIndW);
}

void KKSObjEditor :: setSysAttrWidgets (QWidget *wSysAttr, QScrollArea *scSysAttr, QWidget *sysAttrs)
{
    if (sysAttrWidget && sysAttrWidget != wSysAttr)
    {
        sysAttrWidget->setParent (0);
        delete sysAttrWidget;
    }
    sysAttrWidget = wSysAttr;
    if (scSysAttrs && scSysAttrs != scSysAttr)
    {
        scSysAttrs->setParent (0);
        scSysAttrs->setWidget (0);
        delete scSysAttrs;
    }
    scSysAttrs = scSysAttr;
    scSysAttrs->setWidget (0);
    if (sysAttrsW && sysAttrsW != sysAttrs)
    {
        sysAttrsW->setParent (0);
        delete sysAttrsW;
    }
    sysAttrsW = sysAttrs;
    scSysAttrs->setWidget (sysAttrsW);
}

void KKSObjEditor :: setTabWidget (QTabWidget *tObj)
{
    if (tabObj && tabObj != tObj)
    {
        tabObj->setParent (0);
        delete tabObj;
    }
    tabObj = tObj;
}

void KKSObjEditor :: setAttrView (void)
{
    if (isChanged)
    {
        int res = QMessageBox::question (this, tr("Object editor"), tr ("Object was changed. Do you want to save it before ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        if (res == QMessageBox::Cancel)
            return;
        else if (res == QMessageBox::Yes)
        {
            int res = apply (numCopies);
            if (res == ERROR_CODE)
                return;
        }
    }
    if (sysAttrWidget && (!tabObj || tabObj->currentWidget() == sysAttrWidget ))
#ifdef Q_CC_MSVC
        emit updateAttributes (sysAttrWidget, scSysAttrs, sysAttrsW,  (pObj ? pObj->id() : pObjectEx->io()->id()), pObjectEx->io()->category()->tableCategory(), KKSIndAttr::KKSIndAttrClass::iacTableAttr, this);
#else
        emit updateAttributes (sysAttrWidget, scSysAttrs, sysAttrsW,  (pObj ? pObj->id() : pObjectEx->io()->id()), pObjectEx->io()->category()->tableCategory(), KKSIndAttr::iacTableAttr, this);
#endif
    else if (ioAttrWidget && (!tabObj || tabObj->currentWidget () == ioAttrWidget))
#ifdef Q_CC_MSVC
        emit updateAttributes (ioAttrWidget, scIOAttrs, ioAttrsW, pObj->id(), (pObjectEx->id() < 0 ? pCat: pObj->category()), KKSIndAttr::KKSIndAttrClass::iacIOUserAttr, this);
#else
        emit updateAttributes (ioAttrWidget, scIOAttrs, ioAttrsW, pObj->id(), (pObjectEx->id() < 0 ? pCat: pObj->category()), KKSIndAttr::iacIOUserAttr, this);
#endif
    else if (recWidget && (!tabObj || tabObj->currentIndex() == 0))
        this->setView ();
}

void KKSObjEditor :: closeEvent (QCloseEvent * event)
{
    emit aboutToClose();

    if (isChanged)
    {
        QMessageBox::StandardButton res = QMessageBox::question (this, tr ("Object editor"), tr("Information object was changed.\n Do you want to save ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);
        if (res == QMessageBox::Cancel)
        {
            event->ignore ();
            return;
        }
        else if (res == QMessageBox::Yes)
            save ();
        event->accept ();
    }
    else
        QWidget::closeEvent (event);
    
    emit closeEditor ();
}

void KKSObjEditor :: currentRecordChanged (const QModelIndex& current, const QModelIndex& previous)
{
    qDebug () << __PRETTY_FUNCTION__ << current << previous;
}

void KKSObjEditor :: currentRecSelChanged (const QItemSelection& selected, const QItemSelection& deselected)
{
    qDebug () << __PRETTY_FUNCTION__ << selected << deselected;
}

int KKSObjEditor :: getID (void) const
{
    if(recWidget)
	    return recWidget->getID ();

	return -1;
}

void KKSObjEditor :: rubricsChanged (void)
{
    isChanged = true;
    
    //qDebug () << __PRETTY_FUNCTION__ << this->pObj->rootRubric();//->rubrics().count();
    KKSIncludesWidget * iW = qobject_cast<KKSIncludesWidget *>(this->sender());
    if (!iW)
        return;

    if(pObj)
        pObj->setRootRubric (iW->rootRubric());
    else{
        if(pObjectEx)
            pObjectEx->setRootRubric (iW->rootRubric());
        else
            return;
    }
}

void KKSObjEditor :: attrValueChanged (void)
{
    isChanged = true;
}

void KKSObjEditor :: filesChanged (void)
{
    isChanged = true;
}

void KKSObjEditor :: importObjectE (void)
{
    //
    // операции импорта-экспорта доступны только для ЭИО, которые сами являются ИО
    //
    int i = tabEnclosures->currentIndex ();
    const KKSCategory * c = 0;
    int idObject = pObjectEx->id();
    QString tabName = QString ();
    qDebug () << __PRETTY_FUNCTION__ << i;
    KKSRecWidget * recW = qobject_cast<KKSRecWidget *>(this->sender()->parent());
    QAbstractItemModel * mod = recW ? recW->getSourceModel () : 0;
    if (i == 0)
    {
        c = pCat;//pObjectEx->io()->category()->tableCategory ();
        tabName = pObj->tableName ();
    }
    else if (i <= addCats.count())
    {
        c = addCats [i-1];
        KKSMap<int, KKSAddTable*>::const_iterator p = addTables.constBegin()+i-1;
        tabName = p.value()->getTableName();
        qDebug () << __PRETTY_FUNCTION__ << c->id() << tabName;
    }
    emit importObjectEx (this, idObject, c, tabName, mod);
}

void KKSObjEditor :: exportObjectE (void)
{
    int i = tabEnclosures->currentIndex ();
    const KKSCategory * c = 0;
    int idObject = pObjectEx->id();
    QString tabName = QString ();
    qDebug () << __PRETTY_FUNCTION__ << i;
    KKSRecWidget * recW = qobject_cast<KKSRecWidget *>(this->sender()->parent());
    QAbstractItemModel * mod = recW ? recW->getSourceModel () : 0;
    if (i == 0)
    {
        c = pCat;//pObjectEx->io()->category()->tableCategory ();
        tabName = pObj->tableName ();
    }
    else if (i <= addCats.count())
    {
        c = addCats [i-1];
        KKSMap<int, KKSAddTable*>::const_iterator p = addTables.constBegin()+i-1;
        tabName = p.value()->getTableName();
    }
    emit exportObjectEx (this, idObject, c, tabName, mod);
}

bool KKSObjEditor :: isObjChanged (void) const
{
    return isChanged;
}

void KKSObjEditor :: setObjChanged (bool val)
{
    isChanged = val;
}

void KKSObjEditor :: setFilters (const KKSList<const KKSFilterGroup *>& filters)
{
    m_filters = filters;
}

void KKSObjEditor :: sendIO (void)
{
    if (isChanged)
    {
        int res = QMessageBox::question (this, tr("Object editor"), tr ("Object was changed. Do you want to save it before ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        if (res == QMessageBox::Cancel)
            return;
        else if (res == QMessageBox::Yes)
            save ();
    }
    if (m_isIO)
        emit prepareIO (pObj, pObjectEx, this);
}

bool KKSObjEditor::asDraft() const
{
    return m_draft;
}

void KKSObjEditor::setAsDraft(bool draft)
{
    m_draft = draft;
}

bool KKSObjEditor::showExecButton() const
{
    return m_showExecButton;
}

void KKSObjEditor::setShowExecButton(bool yes)
{
    m_showExecButton = yes;
}

void KKSObjEditor :: editCurrentRec (const QModelIndex& index)
{
    if (index.isValid() && !pbOk)
        editObjectE ();
    else if (pbOk)
        accept ();
}

void KKSObjEditor :: addAttributeCheckReference (const KKSAttrValue * av, KKSIndAttr::KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel)
{
    qDebug () << __PRETTY_FUNCTION__ << av->id() << isSystem << sourceModel;

#ifdef Q_CC_MSVC
    if (m_isIO && isSystem == KKSIndAttr::KKSIndAttrClass::iacIOUserAttr)
#else
    if (m_isIO && isSystem == KKSIndAttr::iacIOUserAttr)
#endif
        emit editObjAttrRef (pObj, av, isSystem, sourceModel);
    else
        emit editObjCAttrRef (pObjectEx, av, isSystem, sourceModel);
}

void KKSObjEditor :: delAttributeCheckReference (const KKSAttrValue * av, KKSIndAttr::KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel, const QModelIndex& wInd)
{
#ifdef Q_CC_MSVC
    if (m_isIO && isSystem == KKSIndAttr::KKSIndAttrClass::iacIOUserAttr)
#else
    if (m_isIO && isSystem == KKSIndAttr::iacIOUserAttr)
#endif
        emit delObjAttrRef (pObj, av, isSystem, sourceModel, wInd);
    else 
        emit delObjCAttrRef (pObjectEx, av, isSystem, sourceModel, wInd);
}

const KKSCategory * KKSObjEditor :: getCategory (void) const
{
    return pCat;
}

void KKSObjEditor :: privChanged (KKSPrivilege *p, KKSObject * obj)
{
    if (!obj || !p)
        return;

    isChanged = true;
}

void KKSObjEditor :: addAnotherTable (void)
{
    qDebug () << __PRETTY_FUNCTION__;

    emit addAnotherTable (pObj, this);
}

QTabWidget * KKSObjEditor :: getRecTab (void) const
{
    return tabEnclosures;
}

void KKSObjEditor :: setRecTab (QTabWidget * tabEnc)
{
    if (tabEnclosures)
    {
        tabEnclosures->setParent (0);
        delete tabEnclosures;
        tabEnclosures = 0;
    }

    tabEnclosures = tabEnc;
}

void KKSObjEditor :: setCurrentTable (const QString& tName)
{
    currTablename = tName;
}

void KKSObjEditor :: setParentTab (int nt)
{
    nTable = nt;
}

void KKSObjEditor :: openReferenceIO (QString tableName)
{
    qDebug () << __PRETTY_FUNCTION__ << tableName;
    emit openRefIO (tableName);
}

//открывается в редакторе запись справочника, которая выбрана в виджете атрибута "многие ко многим"
void KKSObjEditor :: openReferenceRec (QString tableName, qint64 id)
{
    qDebug () << __PRETTY_FUNCTION__ << tableName;
    emit openRefRec (tableName, id);
}

KKSRecWidget * KKSObjEditor :: getRecordsWidget (void) const
{
    return recWidget;
}

void KKSObjEditor :: setSyncType (QLineEdit * leType)
{
    qDebug () << __PRETTY_FUNCTION__;
    QString tableName = QString("io_sync_types");
    QWidget * aSyncW = leType;
    int attrId = ATTR_ID_SYNC_TYPE;
    emit loadAttrRef (tableName, aSyncW, attrId);
    //qDebug () << __PRETTY_FUNCTION__ << (this->sender());
    isChanged = true;
}

void KKSObjEditor :: addSyncOrg (KKSAttrValue * av, QAbstractItemModel * sMod)
{
    if (!av || !sMod)
        return;
    if (!sysAttrValues.contains (av->id()))
    {
        setSysAttrValue(av);
    }
#ifdef Q_CC_MSVC
    emit editObjCAttrRef (pObjectEx, av, KKSIndAttr::KKSIndAttrClass::iacTableAttr, sMod);
#else
    emit editObjCAttrRef (pObjectEx, av, KKSIndAttr::iacTableAttr, sMod);
#endif
    
    setSysAttrValue(pObjectEx->attrValue (av->attribute()->id()) );
    
    isChanged = true;
}

void KKSObjEditor :: delSyncOrg (KKSAttrValue * av, const QModelIndex& wInd, QAbstractItemModel * sMod)
{
    if (!av || !sMod)
        return;

    if (!sysAttrValues.contains (av->id()))
    {
        setSysAttrValue(av);
    }

#ifdef Q_CC_MSVC
    emit delObjCAttrRef (pObjectEx, av, KKSIndAttr::KKSIndAttrClass::iacTableAttr, sMod, wInd);
#else
    emit delObjCAttrRef (pObjectEx, av, KKSIndAttr::iacTableAttr, sMod, wInd);
#endif
    setSysAttrValue(pObjectEx->attrValue (av->attribute()->id()) );
    isChanged = true;
}

void KKSObjEditor :: setIOGlobal (bool isGlobal)
{
    KKSMap<int, KKSAttrValue*>::const_iterator pSysAttrValues;
    
    int idAttrValue = 0;
    for (pSysAttrValues=sysAttrValues.constBegin (); pSysAttrValues != sysAttrValues.constEnd(); pSysAttrValues++){
        KKSAttrValue * av = pSysAttrValues.value();
        if(av->attribute()->id() == ATTR_IS_GLOBAL){
            idAttrValue = av->id();
            break;
        }
    }
    
    
    if (sysAttrValues.contains (idAttrValue))
    {
        KKSAttrValue * av = sysAttrValues[idAttrValue];
        KKSValue v(isGlobal ? "TRUE":"FALSE", KKSAttrType::atBool);
        av->setValue(v);

        setSysAttrValue(av);
        isChanged = true;
    }
}

void KKSObjEditor :: setAccessIO (KKSAccessEntity * acl)
{
    if (pObj)
    {
        pObj->setAccessRules (acl);
        isChanged = true;
    }
}

void KKSObjEditor :: addIndex (QAbstractItemModel * sourceMod, const QModelIndex& pIndex)
{
    qDebug () << __PRETTY_FUNCTION__ << sourceMod << pIndex;
    if (!sourceMod || !pObj)
        return;
    emit addIOIndex (pObj, pIndex, sourceMod);
}

void KKSObjEditor :: editIndex (QAbstractItemModel * sourceMod, const QModelIndex& wIndex)
{
    qDebug () << __PRETTY_FUNCTION__ << sourceMod << wIndex;
    if (!sourceMod || !wIndex.isValid() || !pObj)
        return;
    emit editIOIndex (pObj, wIndex, sourceMod);
}

void KKSObjEditor :: delIndex (QAbstractItemModel * sourceMod, const QModelIndex& wIndex)
{
    qDebug () << __PRETTY_FUNCTION__ << sourceMod << wIndex;
    if (!sourceMod || !wIndex.isValid() || !pObj)
        return;
    emit delIOIndex (pObj, wIndex, sourceMod);
}

void KKSObjEditor :: refreshIndices (QAbstractItemModel * sourceMod)
{
    qDebug () << __PRETTY_FUNCTION__ << sourceMod;
    if (!sourceMod || !pObj)
        return;
    emit refreshIOIndex (pObj, sourceMod);
}

void KKSObjEditor :: clearIndicators (void)
{
    this->ioIndicatorValues.clear();
    QList <QToolButton *> tbv = tbViews.keys();
    tbViews.clear ();
    for (int i=0; i<tbv.size(); i++)
        if (tbv[i])
            delete tbv[i];
}

void KKSObjEditor :: setIndValue (KKSAttrValue *av)
{
    int id = av->id ();
    av->addRef ();
    if (ioIndicatorValues.contains (id))
    {
        ioIndicatorValues.value (id)->release ();
        ioIndicatorValues.remove (id);
    }
    ioIndicatorValues.insert (id, av);

}
/*
void KKSObjEditor :: setIndicator (const KKSIndicator *ind, QVariant val)
{
    int id = ind->id ();
    ind->addRef ();
    if (ioIndicators.contains (id))
    {
        ioIndicators.value (id)->release ();
        ioIndicators.remove (id);
    }
    ioIndicators.insert (id, ind);
    ioIndicatorsValues.insert (id, val);
}
*/

void KKSObjEditor :: viewIndicator (void)
{
    QToolButton * tb = qobject_cast<QToolButton *>(this->sender());
    if (!tb)
        return;
    int idIndicator = tbViews.value (tb);
    emit viewIOIndicator (pObj, idIndicator);
}

/*
void KKSObjEditor :: editIndicator (void)
{
    QToolButton * tb = qobject_cast<QToolButton *>(this->sender());
    if (!tb)
        return;
    int idIndicator = tbIEdit.value (tb);
    QWidget * indW = indWidgets.value (idIndicator);
    emit editIOIndicator (pObj, idIndicator, indW);
}

void KKSObjEditor :: delIndicator (void)
{
    QToolButton * tb = qobject_cast<QToolButton *>(this->sender());
    if (!tb)
        return;
    int idIndicator = tbDelInds.value (tb);
    int i_index = -1;
    for (int i=0; i<pObj->indicatorValues().count() && i_index < 0; i++)
        if (pObj->indicatorValues().at (i)->indicator()->id() == idIndicator)
            i_index = i;
    int res = pObj->removeIndicatorValue (i_index);//Id (idIndicator);
    if (res < 0)
        return;
    QWidget * iEditor = indWidgets.value (idIndicator, 0);
    if (iEditor)
        iEditor->setEnabled (false);
    QLabel * iLab = indLabels.value (idIndicator, 0);
    if (iLab)
        iLab->setEnabled (false);
}

void KKSObjEditor :: addIndRow (int idIndicator, QWidget *editor, QLabel * lab)
{
    if (idIndicator > 0 )//&& editor && lab)
    {
        indWidgets.insert (idIndicator, editor);
        indLabels.insert (idIndicator, lab);
    }
}

void KKSObjEditor :: setIndicatorNumber (int num)
{
    indNumW = num;
}

void KKSObjEditor :: addIndicator (void)
{
    if (!tabObj || !pObj)
        return;
    QWidget * indW = tabObj->widget (indNumW);
    emit addIOIndicator (pObj, indW);
}
 */
void KKSObjEditor :: setNumCopies (int n)
{
    numCopies = n;
}

void KKSObjEditor :: setRecordsModel (QAbstractItemModel * recMod)
{
    if (pRecModel && pRecModel != recMod)
        delete pRecModel;
    
    pRecModel = recMod;
}

void KKSObjEditor :: setStateWidget (QWidget * w)
{
    if (stateWidget == w)
        return;
    
    if (stateWidget)
    {
        stateWidget->setParent(0);
        delete stateWidget;
    }
    
    stateWidget = w;
    stateWidget->setParent (this);
}