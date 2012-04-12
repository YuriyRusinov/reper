#include <QTreeView>
#include <QMessageBox>
#include <QItemDelegate>
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QModelIndex>
#include <QtDebug>

#include <kksincludeswidget.h>
#include <rubricform.h>
#include "KKSLoader.h"
#include <KKSRubric.h>
#include <KKSEventFilter.h>
#include "KKSPPFactory.h"
#include "KKSObjEditorFactory.h"
#include "KKSStuffFactory.h"
#include <KKSObjEditor.h>
#include <KKSObject.h>
#include <KKSObjectExemplar.h>
#include "KKSAttrType.h"
#include <KKSCategory.h>
#include <defines.h>
#include <KKSSearchTemplate.h>
#include "kksstuffform.h"
#include "KKSAccessEntity.h"

#include "KKSRubricFactory.h"

KKSRubricFactory :: KKSRubricFactory (KKSLoader *l, KKSPPFactory *_ppf, KKSObjEditorFactory *_oef, KKSStuffFactory * _stf, QObject* parent)
    : KKSEntityFactory(parent),
    loader (l),
    ppf (_ppf),
    oef (_oef),
    stf (_stf)
{
}

KKSRubricFactory :: ~KKSRubricFactory (void)
{
}

KKSIncludesWidget * KKSRubricFactory :: createRubricEditor (int mode, const KKSList<const KKSFilterGroup *> & filters, QWidget* parent)
{
    Q_UNUSED (filters);
    bool isDocs (mode==atMyDocsRubric);

    KKSRubric * rootR = loader->loadRubricators ( isDocs );
    if (!rootR){
        rootR = new KKSRubric(-1, "root rubric for all tree");
        KKSRubric * rubrMyDocs = new KKSRubric (-1, tr("My Documents"));
        if (!rubrMyDocs)
            return 0;
        rootR->addRubric (rubrMyDocs);
        int res = ppf->updateRubricators (rootR, isDocs);
        if (res != OK_CODE)
        {
            QMessageBox::warning (parent, tr("Rubricator"), tr ("Cannot load My documents"), QMessageBox::Ok);
            return 0;
        }

        ppf->createMyDocsRubricator (rubrMyDocs->id());
    }

    QString title;
    switch (mode)
    {
        case atRootRubric:
            {
                title = tr ("All rubrics");
                break;
            }
        case atMyDocsRubric:
            {
                title = tr ("My Documents");
                break;
            }
        default:
            {
                rootR->release();
                return 0;
                break;
            }
    }

    KKSIncludesWidget *iW = new KKSIncludesWidget (rootR, false, isDocs, false, parent);
    QTreeView *tv = iW->tvRubr();
    KKSEventFilter *ef = new KKSEventFilter (iW);
    iW->setWindowTitle (title);
    tv->viewport()->installEventFilter (ef);
    if (rootR)
        rootR->release ();

    //QAbstractItemDelegate *iDeleg = tv->itemDelegate ();
    //qDebug () << __PRETTY_FUNCTION__ << iDeleg;

    connect (iW, SIGNAL (saveRubric (KKSRubric *, bool)), this, SLOT (saveRubric (KKSRubric *, bool)) );
    connect (iW, SIGNAL (rubricItemRequested ()), this, SLOT (rubricItemUpload()) );
    connect (iW, SIGNAL (openRubricItemRequested (int)), this, SLOT (openRubricItem (int)) );
    connect (iW, SIGNAL (loadStuffModel (RubricForm *)), this, SLOT (loadRubricPrivilegies(RubricForm *)) );
    connect (iW, SIGNAL (loadSearchtemplate (RubricForm *)), this, SLOT (loadSearchTemplate (RubricForm *)) );
    connect (iW, SIGNAL (loadCategory (RubricForm *)), this, SLOT (loadCategory (RubricForm *)) );
    connect (iW, SIGNAL (rubricAttachmentsView (QAbstractItemModel *, const KKSRubric *)), this, SLOT (viewAttachments (QAbstractItemModel *, const KKSRubric *)) );
    connect (iW, SIGNAL (copyFromRubr(KKSRubric *, QAbstractItemModel *, const QModelIndex&)), this, SLOT (copyFromRubric (KKSRubric *, QAbstractItemModel *, const QModelIndex&)) );
    emit rubricEditorCreated (iW);

    return iW;
}

void KKSRubricFactory :: saveRubric (KKSRubric * rootR, bool isMyDocs)
{
    int res = ppf->updateRubricators (rootR, isMyDocs);

    KKSIncludesWidget *wParent = qobject_cast<KKSIncludesWidget *>(this->sender ());
    if (res != OK_CODE)
    {
        QMessageBox::warning (wParent, tr ("Rubricators"), tr ("Cannot save rubricators"), QMessageBox::Ok);
    }
    else
        wParent->setSaved (true);
}

void KKSRubricFactory :: rubricItemUpload (void)
{
    KKSIncludesWidget *editor = qobject_cast<KKSIncludesWidget *>(this->sender());
    if(!editor)
        return;

    KKSList<const KKSFilter*> filters;
    
    KKSObject * o = loader->loadIO(IO_IO_ID, true);
    if(!o)
        return;
    KKSCategory * c = o->category()->tableCategory();
    if(!c){
        o->release();
        return;
    }

    //KKSObjEditorFactory * oef = kksSito->oef();

    int idUser = loader->getUserId();
    KKSFilter * filter = c->createFilter(1, QString::number(idUser), KKSFilter::foEq);
    if(!filter){
        o->release();
        return;
    }

    filters.append(filter);
    filter->release();
    KKSList<const KKSFilterGroup *> filterGroups;
    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();

    KKSObjEditor *objEditor = oef->createObjEditor(IO_IO_ID, 
                                                   IO_IO_ID, 
                                                   filterGroups, 
                                                   "",
                                                   c,
                                                   true,
                                                   QString(),
                                                   false,
                                                   Qt::WindowModal,
                                                   editor,
                                                   Qt::Dialog);
    int res = objEditor->exec();
    
    int idObject = -1;
    QString name;

    if(res == QDialog::Accepted)
    {
        idObject = objEditor->getID();
        KKSObject * o = loader->loadIO(idObject, true);
        if(!o){
            delete objEditor;
            return;
        }
        name = o->name();
        o->release();
    }
    else
        return;
    editor->slotAddRubricItem (idObject, name);
    
    delete objEditor;
    o->release();
}

void KKSRubricFactory :: openRubricItem (int idObject)
{
    KKSIncludesWidget *editor = qobject_cast<KKSIncludesWidget *>(this->sender());
    if(!editor)
        return;

    KKSList<const KKSFilter*> filters;
    
    KKSObject * o = loader->loadIO (idObject, true);
    if(!o)
        return;
    KKSCategory * c = o->category();//->tableCategory();
    if(!c){
        o->release();
        return;
    }

    KKSList<const KKSFilterGroup *> filterGroups;
    bool mode (editor->windowModality () == Qt::NonModal);

    KKSObjEditor *objEditor = oef->createObjEditor(IO_IO_ID, 
                                                   idObject, 
                                                   filterGroups, 
                                                   "",
                                                   c,
                                                   !mode,
                                                   QString(),
                                                   false,
                                                   editor->windowModality(),
                                                   0,
                                                   Qt::Dialog);

    if (mode)
        emit objEditorCreated (objEditor);

    o->release();
}

void KKSRubricFactory :: loadRubricPrivilegies (RubricForm * rForm)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!rForm)
        return;

    KKSAccessEntity * acl = new KKSAccessEntity();
    KKSStuffForm * sForm = stf->createStuffEditorForm (acl, 1, rForm);
    acl->release ();
    rForm->setStuffForm (sForm);
    //QAbstractItemModel * sModel = stf->getStuffModel();
    //rForm->setStuffModel (sModel);
}

void KKSRubricFactory :: loadSearchTemplate (RubricForm * rForm)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!rForm)
        return;

    KKSSearchTemplate * st = oef->loadSearchTemplate ();
    rForm->setSearchTemplate (st);
    if (st)
        st->release ();
//    QAbstractItemModel
}

void KKSRubricFactory :: loadCategory (RubricForm * rForm)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!rForm)
        return;

    KKSObject *refObj = loader->loadIO (IO_CAT_ID, true);
    if (!refObj)
        return;
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    KKSList<const KKSFilterGroup*> filters;
    KKSCategory * ct = c->tableCategory ();
    if (!ct){
        refObj->release();
        return;
    }

    QString value = QString ("select id from io_categories where id_io_category_type != 10 and not is_system");
    const KKSFilter * f = ct->createFilter (1, value, KKSFilter::foInSQL);
    if (!f){
        refObj->release();
        return;
    }

    KKSList<const KKSFilter*> fl;
    fl.append (f);
    f->release ();
    KKSFilterGroup * fg = new KKSFilterGroup (true);
    fg->setFilters (fl);
    filters.append (fg);
    fg->release ();
    KKSObjEditor * recEditor = oef->createObjRecEditor (IO_IO_ID, 
                                                        refObj->id (), 
                                                        filters, 
                                                        "",
                                                        c,
                                                        true, false,
                                                        Qt::ApplicationModal,
                                                        rForm, 
                                                        Qt::Dialog);
    if (!recEditor){
        refObj->release();
        return;
    }

    if (recEditor->exec () == QDialog::Accepted)
    {
        int idCat = recEditor->getID();
        KKSCategory * cat = loader->loadCategory (idCat);
        if (!cat)
        {
            refObj->release ();
            return;
        }
        rForm->setCategory (cat);
        cat->release ();
    }
    refObj->release ();
}

void KKSRubricFactory :: viewAttachments (QAbstractItemModel * attachModel, const KKSRubric * r)
{
    if (!attachModel || !r || !r->getCategory())
        return;

    KKSList<const KKSRubricItem *> docList = r->items();
    KKSCategory * c = r->getCategory();
    QAbstractItemModel * objModel = attachModel;
    while (qobject_cast<QAbstractProxyModel *>(objModel))
        objModel = qobject_cast<QAbstractProxyModel *>(objModel)->sourceModel ();
    if (objModel->rowCount () > 0)
    {
        int nr = objModel->rowCount ();
        objModel->removeRows (0, nr);
    }

    if (objModel->columnCount () > 0)
    {
        int nc = objModel->columnCount ();
        objModel->removeColumns (0, nc);
    }

    //QString aNameCode ("name");
    //KKSCategoryAttr * cAName = c->attribute (aNameCode);
    KKSMap<int, KKSCategoryAttr *> cAttrs = c->attributes();
    int nCols = cAttrs.count ()+1;

    QList<int> bkColors = c ? c->searchAttributesByType (KKSAttrType::atRecordColor) : QList<int>();
    if (bkColors.isEmpty() && c)
        bkColors += c->searchAttributesByType (KKSAttrType::atRecordColorRef);
    if (!bkColors.isEmpty())
        nCols--;
    QList<int> fgColors = c ? c->searchAttributesByType (KKSAttrType::atRecordTextColor) : QList<int>();
    if (fgColors.isEmpty() && c)
        fgColors += c->searchAttributesByType (KKSAttrType::atRecordTextColorRef);

    if (!fgColors.isEmpty())
        nCols--;
    
    objModel->insertColumns (0, nCols);
    int nRows = docList.count();
    objModel->insertRows (0, nRows);
    int i=1;
    objModel->setHeaderData (0, Qt::Horizontal, tr ("Name"), Qt::DisplayRole);
    
    for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa=cAttrs.constBegin(); \
            pa != cAttrs.constEnd(); pa++)
    {
        objModel->setHeaderData (i, Qt::Horizontal, pa.value()->title(), Qt::DisplayRole);
        if (pa.value()->type()->attrType () == KKSAttrType::atRecordColor ||
            pa.value()->type()->attrType () == KKSAttrType::atRecordColorRef ||
            pa.value()->type()->attrType () == KKSAttrType::atRecordTextColor ||
            pa.value()->type()->attrType () == KKSAttrType::atRecordTextColorRef )
            continue;
        i++;
    }
    
    for (int i=0; i<nRows; i++)
    {
        int j=1;
        int id_io = docList[i]->id();
        KKSObject * cio = loader->loadIO (id_io);
        if (!cio)
            continue;
        QModelIndex nIndex = objModel->index (i, 0);
        objModel->setData (nIndex, cio->name(), Qt::DisplayRole);
        objModel->setData (nIndex, cio->id(), Qt::UserRole);
        QVariant bkColVal;

        if (!bkColors.isEmpty())
        {
            int idAttr = bkColors[0];//всегда берем первый атрибут, который ссылаетс€ на цвет
            KKSAttrView * vcol = new KKSAttrView (*(c->attribute(idAttr)));
            //QRgb rgb_col (0xFFFFFFFF);
            QVariant vc;
            KKSAttrValue * av = cio->attrValueId (vcol->id());
            if (av && !av->value ().value().isEmpty() && vcol->type()->attrType() == KKSAttrType::atRecordColor)
            {
                vc = av->value ().valueVariant ();//QColor::fromRgba(rgb_col);
                qDebug () << __PRETTY_FUNCTION__ << vc << vc.value<QColor>() << av->value().valueVariant();
                bkColVal = QVariant (QBrush (vc.value<QColor>()));
            }
            else if (av && vcol->type()->attrType() == KKSAttrType::atRecordColorRef)
            {
                int idVal = av->value ().value().toInt ();
                KKSObject * objCRef = loader->loadIO (vcol->tableName(), true);
                
                //атрибут ссылаетс€ на значение в некоторой таблице, 
                //значит категори€ соответствующего »ќ описывает таблицу. ѕоэтому код атрибута можно использовать в качестве ключа
                KKSCategory * cRef = objCRef->category()->tableCategory();
                if(!cRef){
                    objCRef->release();
                    continue;
                }
                
                //cRef €вл€етс€ табличной категорией, поэтому в ней все атрибуты имеют уникальные коды
                //использование данного вызова допустимо
                KKSAttribute * a = cRef->attribute(vcol->columnName());
                if(!a){
                    objCRef->release();
                    continue;
                }

                KKSObjectExemplar * colorC = loader->loadEIO (idVal, objCRef);
                KKSAttrValue * avR = colorC->attrValue (a->id());
                
                //rgb_col = av->value().value().toUInt();
                vc = avR->value().valueVariant();//avR->value ().valueVariant ();//(QColor::fromRgba(rgb_col));//d->fields().value (vcol->code().toLower()));
                colorC->release ();
                objCRef->release ();
                //qDebug () << __PRETTY_FUNCTION__ << vc << vc.value<QColor>();
                bkColVal = QVariant (QBrush (vc.value<QColor>()));
            }
            objModel->setData (nIndex, bkColVal, Qt::BackgroundRole);//Qt::UserRole+1);
            vcol->release ();
        }

        QVariant fgColVal;
        if (!fgColors.isEmpty())
        {
            int idAttr = fgColors[0];
            KKSAttrView * vcol = new KKSAttrView (*(c->attribute(idAttr)));
            //QRgb rgb_colf (0x00000000);
            QVariant vc;
            KKSAttrValue * av = cio->attrValueId (vcol->id());
            if (av && !av->value ().value().isEmpty() && vcol->type()->attrType() == KKSAttrType::atRecordTextColor)
            {
                vc = av->value().valueVariant ();//(QColor::fromRgba(rgb_colf));
                qDebug () << __PRETTY_FUNCTION__ << vc << vc.value<QColor>();
                fgColVal = QVariant (QBrush (vc.value<QColor>()));
            }
            else if (vcol->type()->attrType() == KKSAttrType::atRecordTextColorRef)
            {
                int idVal = av->value ().value().toInt ();
                KKSObject * objCRef = loader->loadIO (vcol->tableName(), true);

                //атрибут ссылаетс€ на значение в некоторой таблице, 
                //значит категори€ соответствующего »ќ описывает таблицу. ѕоэтому код атрибута можно использовать в качестве ключа
                KKSCategory * cRef = objCRef->category()->tableCategory();
                if(!cRef){
                    objCRef->release();
                    continue;
                }
                
                //cRef €вл€етс€ табличной категорией, поэтому в ней все атрибуты имеют уникальные коды
                //использование данного вызова допустимо
                KKSAttribute * a = cRef->attribute(vcol->columnName());
                if(!a){
                    objCRef->release();
                    continue;
                }

                KKSObjectExemplar * colorC = loader->loadEIO (idVal, objCRef);
                KKSAttrValue * avR = colorC->attrValue (a->id());

                //rgb_colf = av->value().value().toUInt();
                vc = avR->value().valueVariant ();//(QColor::fromRgba(rgb_colf));//d->fields().value (vcol->code().toLower()));
                colorC->release ();
                objCRef->release ();
                //qDebug () << __PRETTY_FUNCTION__ << vc << vc.value<QColor>();
                fgColVal = QVariant (QBrush (vc.value<QColor>()));
            }
            objModel->setData (nIndex, fgColVal, Qt::ForegroundRole);//Qt::UserRole+2);
            vcol->release ();
        }

        for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa=cAttrs.constBegin(); \
             pa != cAttrs.constEnd(); pa++)
        {
            QModelIndex wIndex = objModel->index (i, j);
            if (pa.value()->type()->attrType () == KKSAttrType::atRecordColor ||
                pa.value()->type()->attrType () == KKSAttrType::atRecordColorRef ||
                pa.value()->type()->attrType () == KKSAttrType::atRecordTextColor ||
                pa.value()->type()->attrType () == KKSAttrType::atRecordTextColorRef )
                continue;
            const KKSAttrValue * av = cio->attrValueId (pa.key());//value()->id());
            qDebug () << __PRETTY_FUNCTION__ << pa.key() << pa.value()->code() << av;
            if (av)
            {
                if( av->attribute()->type()->attrType() == KKSAttrType::atJPG || 
                   (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atJPG)
                   )
                {
                    objModel->setData (wIndex, tr("<Image data %1>").arg (i), Qt::DisplayRole);
                }
                else if( av->attribute()->type()->attrType() == KKSAttrType::atSVG || 
                        (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atSVG)
                       )
                {
                    objModel->setData (wIndex, tr("<SVG data %1>").arg (i), Qt::DisplayRole);
                }
                else if( av->attribute()->type()->attrType() == KKSAttrType::atXMLDoc || 
                        (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atXMLDoc)
                       )
                {
                    objModel->setData (wIndex, tr("<XML document %1>").arg (i), Qt::DisplayRole);
                }
                else if( av->attribute()->type()->attrType() == KKSAttrType::atVideo || 
                        (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atVideo)
                       )
                {
                    objModel->setData (wIndex, tr("<Video data %1>").arg (i), Qt::DisplayRole);
                }
                else if (av->attribute()->type()->attrType() == KKSAttrType::atList ||
                         av->attribute()->type()->attrType() == KKSAttrType::atParent)
                {
                    bool ok;
                    int idVal = av->value().value().toInt(&ok);
                    if (!ok)
                        objModel->setData (wIndex, QVariant(), Qt::DisplayRole);

                    KKSObject *refObj = loader->loadIO (av->attribute()->tableName(), true);
                    if (!refObj)
                        continue;

                    //атрибут ссылаетс€ на значение в некоторой таблице, 
                    //значит категори€ соответствующего »ќ описывает таблицу. ѕоэтому код атрибута можно использовать в качестве ключа
                    KKSCategory * cRef = refObj->category()->tableCategory();
                    if(!cRef){
                        refObj->release();
                        continue;
                    }
                    
                    //cRef €вл€етс€ табличной категорией, поэтому в ней все атрибуты имеют уникальные коды
                    //использование данного вызова допустимо
                    KKSAttribute * a = cRef->attribute(av->attribute()->columnName());
                    if(!a){
                        refObj->release();
                        continue;
                    }

                    KKSObjectExemplar * oe = loader->loadEIO (idVal, refObj);
                    if (!oe){
                        refObj->release();
                        continue;
                    }

                    KKSAttrValue * avr = oe->attrValue (a->id());
                    if (!avr){
                        refObj->release();
                        continue;
                    }

                    objModel->setData (wIndex, avr->value().valueVariant(), Qt::DisplayRole);
                    oe->release ();
                    refObj->release ();
                }
                else if (av->attribute()->type()->attrType() == KKSAttrType::atDateTime)
                    objModel->setData (wIndex, QDateTime::fromString(av->value().value(), Qt::ISODate).toString("dd.MM.yyyy hh:mm:ss"), Qt::DisplayRole);
                else if (av->attribute()->type()->attrType() == KKSAttrType::atTime)
                    objModel->setData (wIndex, QDateTime::fromString(av->value().value(), Qt::ISODate).toString("hh:mm:ss"), Qt::DisplayRole);
                else
                    objModel->setData (wIndex, av->value().value(), Qt::DisplayRole);
            }
            //qDebug () << __PRETTY_FUNCTION__ << wIndex << bkColVal << fgColVal;
            if (bkColVal.isValid ())
                objModel->setData (wIndex, bkColVal, Qt::BackgroundRole);
            if (fgColVal.isValid ())
                objModel->setData (wIndex, fgColVal, Qt::ForegroundRole);
            j++;
        }
        cio->release ();
    }
}

void KKSRubricFactory :: copyFromRubric (KKSRubric * rDest, QAbstractItemModel * aModel, const QModelIndex& wIndex)
{
    //QWidget * parent = qobject_cast<QWidget *>(this->sender());

    KKSList<const KKSFilterGroup *> filters;
    KKSObject * refIO = loader->loadIO(IO_RUBR_ID, true);
    if (!refIO)
        return;
    KKSCategory * cat = refIO->category();
    if (cat)
        cat = cat->tableCategory();
    if (!cat){
        refIO->release();
        return;
    }

    KKSObjEditor * oEditor = oef->createObjRecEditor(IO_IO_ID, IO_RUBR_ID, filters, tr("Select source rubric"), cat, true, Qt::WindowModal);
    if (oEditor && oEditor->exec() == QDialog::Accepted)
    {
        int idRubr = oEditor->getID();
        KKSRubric * rSource = loader->loadRubric(idRubr);
        if (rSource)
            rDest->addRubric (rSource);
    }
    else
        if (oEditor)
            delete oEditor;
    refIO->release();
}