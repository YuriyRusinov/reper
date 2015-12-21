/***********************************************************************
 * Module:  KKSViewFactory.cpp
 * Author:  sergey
 * Modified: 27 ноября 2008 г. 13:28:59
 * Purpose: Implementation of the class KKSViewFactory
 * Comment: данный класс предназначен для загрузки из БД всего необходимого для инициализации класса KKSView
 *    Он имеет статический метод createView() для этого
 ***********************************************************************/

#include <QObject>
#include <QWidget>
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QModelIndex>
#include <QObject>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
//#include <QToolButton>
#include <QTreeView>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QGroupBox>
#include <QTime>
#include <QProgressDialog>
#include <QKeySequence>
#include <QColor>
#include <QSize>
#include <QtDebug>

#include "KKSViewFactory.h"

#include <KKSTemplate.h>
#include <KKSObject.h>
#include "KKSLoader.h"
#include <KKSObjEditor.h>
#include <KKSCategoryTemplateWidget.h>
#include <KKSRecWidget.h>
#include <KKSAttributesEditor.h>
#include <KKSAttrType.h>
#include <KKSCategory.h>
#include <KKSEIOData.h>
#include <KKSEIODataModel.h>
#include <KKSRecProxyModel.h>
#include <KKSCategory.h>
#include <KKSAttribute.h>
#include <KKSSortFilterProxyModel.h>
#include <KKSCatAttrsModel.h>
#include <KKSItemDelegate.h>
#include <KKSObjectExemplar.h>
#include <KKSEventFilter.h>
#include <KKSCheckableModel.h>
#include <KKSAttrGroup.h>
#include <KKSSearchTemplate.h>
#include <KKSSearchTemplateType.h>
#include <KKSAttrModel.h>
#include <kksresult.h>
#include <defines.h>

////////////////////////////////////////////////////////////////////////
// Name:       KKSViewFactory::createView(KKSTemplate* theTemplate, QString tableName)
// Purpose:    Implementation of KKSViewFactory::createView()
// Parameters:
// - theTemplate -- шаблон табличной категории
// - objEditor -- редактор ИО
// - obj -- ИО-справочник
// - l -- загрузчик данных из БД
// - filters -- применяемые фильтры
// - parent -- родительский виджет
// - f -- флаги визуального отображения
// Return:     KKSRecWidget * с моделью данных
////////////////////////////////////////////////////////////////////////

KKSRecWidget * KKSViewFactory :: createViewOld (KKSTemplate* theTemplate, 
                                        KKSObjEditor *objEditor, 
                                        KKSObject* obj, 
                                        KKSLoader *l,
                                        const KKSList<const KKSFilterGroup *> & filters,
                                        QWidget *parent,
                                        Qt::WindowFlags f)
{

    KKSRecWidget *resWidget = new KKSRecWidget (false, Qt::Vertical, parent, f);
    QTreeView *tv = resWidget->getView();//new QTreeView ();
    tv->header()->setClickable (true);
    tv->header()->setSortIndicatorShown (true);
    tv->header()->setSortIndicator (0, Qt::AscendingOrder);
    tv->setSortingEnabled (true);
    KKSItemDelegate *itemDeleg = new KKSItemDelegate (objEditor);
    itemDeleg->setTemplate (theTemplate);
    //setCategory (theTemplate->category());
    tv->setItemDelegate (itemDeleg);
    resWidget->showFilter ();
    KKSEventFilter *ef = new KKSEventFilter (resWidget);
    tv->viewport()->installEventFilter (ef);
    if (tv->selectionModel())
        tv->selectionModel()->clearSelection ();

    //filter group
    if (resWidget->actFilter)
    {
        resWidget->actFilter->setToolTip(QObject::tr("Filter records by criterion(ia)"));
        QObject::connect (resWidget->actFilter, SIGNAL (triggered()), objEditor, SLOT (filterObjectE()) );
    }
    if (resWidget->actSearchT)
    {
        resWidget->actSearchT->setToolTip(QObject::tr("Filter records by preared search templates"));
        QObject::connect (resWidget->actSearchT, SIGNAL (triggered()), objEditor, SLOT (filterObjectT()) );
    }

    //add_edit_del group
    if (resWidget->actAdd)
    {
        resWidget->actAdd->setToolTip(QObject::tr("Add new record"));
        QObject::connect (resWidget->actAdd, SIGNAL (triggered()), objEditor, SLOT (addObjectE()) );
    }
    if (resWidget->actEdit)
    {
        QKeySequence kEdit (Qt::Key_F4);
        resWidget->actEdit->setShortcut (kEdit);
        resWidget->actEdit->setToolTip (QObject::tr("Edit selected record"));
        QObject::connect (resWidget->actEdit, SIGNAL (triggered()), objEditor, SLOT (editObjectE()) );
    }
    if (resWidget->actDel)
    {
        QKeySequence kDel (Qt::Key_Delete);
        resWidget->actDel->setShortcut (kDel);
        resWidget->actDel->setToolTip(QObject::tr("Delete selected record"));
        QObject::connect (resWidget->actDel, SIGNAL (triggered()), objEditor, SLOT (delObjectE()) );
    }

    //openRPT group
    if (resWidget->actReportEdit)
    {
        resWidget->actReportEdit->setToolTip (QObject::tr("Edit selected record in openRPT report editor"));
        QObject::connect (resWidget, SIGNAL (showReportEditor(qint64)), objEditor, SLOT (showReportEditor(qint64)) );
    }
    if (resWidget->actReportOpen)
    {
        resWidget->actReportOpen->setToolTip (QObject::tr("Print selected record in openRPT report viewer"));
        QObject::connect (resWidget, SIGNAL (showReportViewer(qint64)), objEditor, SLOT (showReportViewer(qint64)) );
    }

    //import group
    if (resWidget->actImport)
    {
        resWidget->actImport->setToolTip(QObject::tr("Import records"));
        QObject::connect (resWidget->actImport, SIGNAL (triggered()), objEditor, SLOT (importObjectE()) );
    }
    if (resWidget->actExport)
    {
        resWidget->actExport->setToolTip(QObject::tr("Export records"));
        QObject::connect (resWidget->actExport, SIGNAL (triggered()), objEditor, SLOT (exportObjectE()) );
    }

    //view group
    if (resWidget->actSetView)
    {
        resWidget->actSetView->setToolTip(QObject::tr("Set view template"));
        QObject::connect (resWidget->actSetView, SIGNAL (triggered()), objEditor, SLOT (setView()) );
    }


    QObject::connect (resWidget, SIGNAL (refreshMod (QAbstractItemModel *)), objEditor, SLOT (refreshRecModel(QAbstractItemModel *)) );
    tv->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tv->setSelectionBehavior(QAbstractItemView::SelectRows);

    QObject::connect (tv, SIGNAL (doubleClicked(const QModelIndex&)), objEditor, SLOT (editCurrentRec(const QModelIndex&)) );
    
    loadEIOEx (objEditor, obj, l, theTemplate, tv, filters, true);
    const KKSCategory * cat = theTemplate->category();
    QList<int> idP = cat->searchAttributesByType(KKSAttrType::atParent);
    bool isHier (!idP.isEmpty());
    resWidget->enableGroupMenu (!isHier);
    resWidget->enableFilterMenu (false);//isHier);
    //
    // пока с проксевыми моделями имеются проблемы, обсуждаемые в Qt
    // разумнее сделать пока фильтрацию по регулярному выражению, отложив
    // более правильное вычленение записей до лучших времен.
    //
    return resWidget;
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSViewFactory::createView(KKSTemplate* theTemplate, QString tableName)
// Purpose:    Implementation of KKSViewFactory::createView()
// Parameters:
// - theTemplate -- шаблон табличной категории
// - objEditor -- редактор ИО
// - obj -- ИО-справочник
// - l -- загрузчик данных из БД
// - filters -- применяемые фильтры
// - parent -- родительский виджет
// - f -- флаги визуального отображения
// Return:     KKSRecWidget * с моделью данных
////////////////////////////////////////////////////////////////////////

KKSRecWidget * KKSViewFactory :: createView (KKSTemplate* theTemplate, 
                                             KKSObject* obj, 
                                             KKSLoader *l,
                                             const KKSList<const KKSFilterGroup *> & filters,
                                             QWidget *parent,
                                             Qt::WindowFlags f)
{
//    KKSView *tv = new KKSView ();
    KKSRecWidget *resWidget = new KKSRecWidget (false, Qt::Vertical, parent, f);
    
    QTreeView *tv = resWidget->getView();//new QTreeView ();
    tv->header()->setClickable (true);
    tv->header()->setSortIndicatorShown (true);
    tv->header()->setSortIndicator (0, Qt::AscendingOrder);
    tv->setSortingEnabled (true);

    KKSItemDelegate *itemDeleg = new KKSItemDelegate (parent);
    itemDeleg->setTemplate (theTemplate);
    //setCategory (theTemplate->category());
    tv->setItemDelegate (itemDeleg);
    resWidget->showFilter ();
    KKSEventFilter *ef = new KKSEventFilter (resWidget);
    tv->viewport()->installEventFilter (ef);
    if (tv->selectionModel())
        tv->selectionModel()->clearSelection ();

    tv->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tv->setSelectionBehavior(QAbstractItemView::SelectRows);

    loadEIOEx (0, obj, l, theTemplate, tv, filters, true);
    
    const KKSCategory * cat = theTemplate->category();
    QList<int> idP = cat->searchAttributesByType(KKSAttrType::atParent);
    bool isHier (!idP.isEmpty());
    resWidget->enableGroupMenu (!isHier);
    resWidget->enableFilterMenu (false);//isHier);
    //
    // пока с проксевыми моделями имеются проблемы, обсуждаемые в Qt
    // разумнее сделать пока фильтрацию по регулярному выражению, отложив
    // более правильное вычленение записей до лучших времен.
    //
    return resWidget;
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSViewFactory::KKSViewFactory()
// Purpose:    Implementation of KKSViewFactory::KKSViewFactory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSViewFactory::KKSViewFactory()
{
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSViewFactory::~KKSViewFactory()
// Purpose:    Implementation of KKSViewFactory::~KKSViewFactory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSViewFactory::~KKSViewFactory()
{
   // TODO : implement
    qDebug () << __PRETTY_FUNCTION__;
}

/* Метод осуществляет загрузку записей ИО-справочника в модель.
 * Параметры:
 * editor -- редактор ИО
 * pObj -- ИО-справочник
 * l -- загрузчик данных из БД
 * tv -- целевой виджет для модели
 * filters -- применяемые фильтры
 * resize -- переразмерка колонок
 * cat -- категория справочника
 * tableName -- название таблицы
 * pgDial -- если не 0, то индикатор загрузки данных
 * isCheckable -- флаг наличия чекбоксов в 1й колонке
 */
void KKSViewFactory :: loadEIOEx (KKSObjEditor * editor, 
                                  const KKSObject *pObj, 
                                  KKSLoader *l, 
                                  const KKSTemplate *t, 
                                  QTreeView *tv, 
                                  const KKSList<const KKSFilterGroup *> & filters,
                                  bool resize,
                                  const KKSCategory * cat,
                                  const QString& tableName,
                                  QProgressDialog *pgDial,
                                  bool isCheckable
                                  )
{
    if ( !t || !tv)
        return; 

    //
    // сортируем будущие колонки таблицы по порядку (KKSAttrView::order())
    //
    Q_UNUSED (isCheckable);
    QList<KKSAttrView*> attrs_list = t->sortedAttrs();
    int ncols = 0;
    QStringList headers;
    QList<int> visible_attrs;
    for (int i=0; i<attrs_list.count(); i++)
        if (attrs_list[i]->isVisible() && \
                (attrs_list[i]->type ()->attrType () != KKSAttrType::atRecordColor) && \
                (attrs_list[i]->type ()->attrType () != KKSAttrType::atRecordColorRef) && \
                (attrs_list[i]->type ()->attrType () != KKSAttrType::atRecordTextColor) && \
                (attrs_list[i]->type ()->attrType () != KKSAttrType::atRecordTextColorRef)
           )
        {
            ncols++;
            visible_attrs.append (i);
            headers << attrs_list[i]->title();
        }

    KKSSortFilterProxyModel *sortModel = 0;

    if (!tv->model())
    {
        sortModel = new KKSSortFilterProxyModel (tv);//ksa было editor
        tv->setModel (sortModel);
    }
    else
    {
        sortModel = qobject_cast <KKSSortFilterProxyModel *>(tv->model());
        if (!sortModel)
            sortModel = new KKSSortFilterProxyModel (tv); //ksa было editor
        sortModel->clearAttrs ();
        //itemDeleg->clearAttrs ();
        //qDebug () << __PRETTY_FUNCTION__ << "Model was previously set";
    }
    sortModel->setDynamicSortFilter (true);

    KKSMap<qint64, KKSEIOData *> objEx;
    if (!cat || tableName.isEmpty())
        objEx = l->loadEIOList (pObj, filters);
    else
        objEx = l->loadEIOList (cat, tableName, filters, pObj->id() <= _MAX_SYS_IO_ID_ ? true : false);
    
    QAbstractItemModel *objModel = sortModel->sourceModel();//new QStandardItemModel ();
    if (!objModel)
        objModel = new KKSEIODataModel (t, objEx);//isCheckable ? new KKSCheckableModel (0, ncols) : new QStandardItemModel (0, ncols);
    else
    {
        sortModel->setSourceModel (0);
        delete objModel;
        objModel = new KKSEIODataModel (t, objEx);
    }

/*    KKSAttribute * aColBG = l->loadAttribute(ATTR_RECORD_FILL_COLOR);
    const KKSCategoryAttr * caColBG = KKSCategoryAttr::create(aColBG,false,false);
    bool isBGSet = objModel->setData(QModelIndex(), QVariant::fromValue<const KKSCategoryAttr *>(caColBG), Qt::UserRole+4);
    //caColBG->release();
    KKSAttribute * aColFG = l->loadAttribute(ATTR_RECORD_TEXT_COLOR);
    const KKSCategoryAttr * caColFG = KKSCategoryAttr::create(aColFG,false,false);
    bool isFGSet = objModel->setData(QModelIndex(), QVariant::fromValue<const KKSCategoryAttr *>(caColFG), Qt::UserRole+5);
    //caColFG->release ();
    //qDebug () << __PRETTY_FUNCTION__ << isBGSet << isFGSet;
 */
    QItemSelectionModel *selModel = tv->selectionModel ();
    if (editor)
    {
        QObject::connect (selModel, SIGNAL (currentChanged(const QModelIndex&, const QModelIndex&)), editor, SLOT (currentRecordChanged (const QModelIndex&, const QModelIndex&)) );
        QObject::connect (selModel, SIGNAL (selectionChanged (const QItemSelection&, const QItemSelection&)), editor, SLOT (currentRecSelChanged (const QItemSelection& , const QItemSelection&)) );
    }

    int i=0;

    bool isSetH ( objModel->rowCount () == 0);
/*
    if (objModel->rowCount () > 0)
    {
        objModel->removeRows (0, objModel->rowCount());
        if (resize)
            objModel->removeColumns (0, objModel->columnCount());
    }
    if (objModel->columnCount () == 0)
        objModel->insertColumns (0, ncols);
*/
    if (editor)
        editor->clearW ();

    KKSMap<qint64, KKSEIOData *>::const_iterator p;
    int nObjC = objEx.count ();
    //qDebug () << __PRETTY_FUNCTION__ << nObjC;
    //objModel->insertRows (0, nObjC);

    if (pgDial)
    {
        pgDial->setMinimum (0);
        pgDial->setMaximum (nObjC*ncols);
    }

    int c = 0;
    QModelIndex wIndex;
    QModelIndex prevIndex (wIndex);
    QList<int> pattrs;
    const KKSCategory * cobjCat (0);//= pObj->category()->tableCategory();
    
    int idPAttr (-1);
    Q_UNUSED (idPAttr);
    
    if (cat && cat->isAttrTypeContains(KKSAttrType::atParent) )
        pattrs = cat->searchAttributesByType (KKSAttrType::atParent);
    else if (pObj && pObj->category() && pObj->category()->tableCategory())
    {
        cobjCat = pObj->category()->tableCategory();
        pattrs = cobjCat->searchAttributesByType (KKSAttrType::atParent);
    }
    
    if (!pattrs.isEmpty())
        idPAttr = pattrs[0];
    
    /*
    for (p=objEx.constBegin(); p!=objEx.constEnd(); p++)
    {
        QTime time;
        time.start ();
        if (pgDial)
            pgDial->setValue (c);
        KKSEIOData * d = p.value();
        Q_UNUSED (d);
        
        i++;

//закомментировано было отсюда /*        
        if (idPAttr > 0)
        {
            const KKSCategoryAttr * cAttr = cat ? cat->attribute(idPAttr) : cobjCat->attribute (idPAttr);
            QString attrValue = d->fields().value ((cAttr->code(false)));
            KKSObjectExemplar * ioc = l->loadEIO(p.key(), pObj);
            KKSAttrValue * av = ioc ? ioc->attrValue (idPAttr) : 0;
            int idp = av ? av->value().valueVariant().toInt() : 0;
            if (idp > 0)
            {
                prevIndex = searchModelIndex (objModel, idp, QModelIndex(), Qt::UserRole);//wIndex;
                if (!prevIndex.isValid())
                    prevIndex = searchModelIndex (objModel, idp, QModelIndex(), Qt::DisplayRole);
                if(!prevIndex.isValid())
                    prevIndex = QModelIndex();
                //while (prevIndex.isValid() && prevIndex.data (Qt::UserRole).toInt() != idp)
                //    prevIndex = prevIndex.parent();
            }
            else
                prevIndex = QModelIndex();
            //qDebug () << __PRETTY_FUNCTION__ << av->value().valueVariant().toInt() << cAttr->code (false);
        }
        int nr = objModel->rowCount(prevIndex);
        objModel->insertRows (nr, 1, prevIndex);
        if (objModel->columnCount (prevIndex) == 0)
            objModel->insertColumns (0, ncols, prevIndex);
        wIndex = objModel->index (nr, 0, prevIndex);

        //QVariant bkColVal = QVariant ();
        objModel->setData (wIndex, p.key(), Qt::UserRole);

        const KKSCategory * cat = t->category();
        QVariant bkColVal = drawViewCells (cat, d, KKSAttrType::atRecordColor, l, objModel, wIndex);
        QVariant fgColVal = drawViewCells (cat, d, KKSAttrType::atRecordTextColor, l, objModel, wIndex);

        for (int ii=0; ii<ncols; ii++)
        {
            if (pgDial)
                pgDial->setValue (c++);
            int j = visible_attrs[ii];
            KKSAttrView * v = attrs_list [j];
            QString attrCode = v->code();
            
            QString attrValue;
            if( v->type()->attrType() == KKSAttrType::atJPG || 
               (v->refType() && v->refType()->attrType() == KKSAttrType::atJPG)
               )
            {
                attrValue = QObject::tr("<Image data %1>").arg (i);
            }
            else if( v->type()->attrType() == KKSAttrType::atSVG || 
                    (v->refType() && v->refType()->attrType() == KKSAttrType::atSVG)
                   )
            {
                attrValue = QObject::tr("<SVG data %1>").arg (i);
            }
            else if( v->type()->attrType() == KKSAttrType::atXMLDoc || 
                    (v->refType() && v->refType()->attrType() == KKSAttrType::atXMLDoc)
                   )
            {
                attrValue = QObject::tr("<XML document %1>").arg (i);
            }
            else if( v->type()->attrType() == KKSAttrType::atVideo || 
                    (v->refType() && v->refType()->attrType() == KKSAttrType::atVideo)
                   )
            {
                attrValue = QObject::tr("<Video data %1>").arg (i);
            }
            else{
                attrValue = d->fields().value (attrCode);
                if (attrValue.isEmpty())
                    attrValue = d->fields().value (attrCode.toLower());
                else if (attrValue.contains ("\n"))
                    attrValue = attrValue.mid (0, attrValue.indexOf("\n")) + "...";
            }

            QModelIndex wcIndex = objModel->index (wIndex.row(), ii, prevIndex);
            objModel->setData (wcIndex, attrValue, Qt::DisplayRole);
            if (bkColVal.isValid ())
                objModel->setData (wcIndex, bkColVal, Qt::BackgroundRole);
            if (fgColVal.isValid ())
                objModel->setData (wcIndex, fgColVal, Qt::ForegroundRole);
            if (ii == 0 && isCheckable)
                objModel->setData (wcIndex, false, Qt::CheckStateRole);
        }
// закомментировано было до сюда 


    }
*/


    if (isSetH || resize)
        for (int j=0; j<qMin (objModel->columnCount(), headers.count()); j++)
            objModel->setHeaderData (j, Qt::Horizontal, headers[j], Qt::DisplayRole);

    for (int ii=0; ii<ncols; ii++)
    {
        KKSAttrView * v = attrs_list [visible_attrs[ii]];
        //QString attrCode = v->code();
        sortModel->addAttrView (v);
//        itemDeleg->addAttrView (v);

        if ((isSetH || resize) && v )
        {
            int w = v->defWidth();
            if (editor)
                editor->addWidth (w);
            //else
            tv->header()->resizeSection (ii, w);
        }
    }

    KKSRecProxyModel * proxyModel = new KKSRecProxyModel ();
    Q_UNUSED (proxyModel);
    //proxyModel->setSourceModel (objModel);
    //sortModel->setSourceModel (proxyModel);
    //--ksa
    sortModel->setSourceModel (objModel);
    int sortCol = tv->header()->sortIndicatorSection ();
    Qt::SortOrder sOrder = tv->header()->sortIndicatorOrder ();
    sortModel->sort (sortCol, sOrder);
}

/* Метод осуществляет отрисовку строк ИО-справочника в случае наличия атрибутов,
 * ответственных за цвет текста-фона.
 * Параметры:
 * cat -- категория таблицы
 * d -- данные
 * type -- тип атрибута
 * l -- загрузчик из БД
 * objModel -- модель с данными
 * wIndex -- индекс записи
 * icolor -- если атрибутов несколько, то номер требуемого атрибута (обычно 0).
 * В рамках данного метода коды атрибутов категории являются уникальными,
т.к. категория описывает таблицу. Поэтому использование кода атрибута в качестве ключа допустимо
*/
QVariant KKSViewFactory :: drawViewCells (const KKSCategory * cat, 
                                          KKSEIOData * d, 
                                          int type, 
                                          KKSLoader *l, 
                                          QAbstractItemModel * objModel, 
                                          const QModelIndex& wIndex, 
                                          int icolor)
{
    if (!cat || !d || !l)
        return QVariant();

    if ( (KKSAttrType::KKSAttrTypes)type != KKSAttrType::atRecordColor && 
         (KKSAttrType::KKSAttrTypes)type != KKSAttrType::atRecordTextColor
         )
    {
        return QVariant();
    }

    bool isBackGround ((KKSAttrType::KKSAttrTypes)type == KKSAttrType::atRecordColor);
    KKSAttrType::KKSAttrTypes aType = ( isBackGround ? KKSAttrType::atRecordColorRef : KKSAttrType::atRecordTextColorRef);
    
    QList<int> colors = cat ? cat->searchAttributesByType (type) : QList<int>();
    QVariant colVal = QVariant ();
    
    if (colors.isEmpty() && cat)
        colors += cat->searchAttributesByType (aType);
    
    if (!colors.isEmpty())
    {
        int idAttr = -1;
        if (icolor >= 0 && icolor < colors.size())
            idAttr = colors[icolor];
        else if (icolor >= colors.size())
            idAttr = colors [colors.size()-1];
        else
            idAttr = colors[0];

        KKSAttrView * vcol = new KKSAttrView (*(cat->attribute(idAttr)));

        QRgb rgb_col ((isBackGround ? 0xFFFFFFFF : 0x00000000));
        QVariant vc;
        
        if (!d->fields().value (vcol->code().toLower()).isEmpty() && 
            vcol->type()->attrType() == type)
        {
            unsigned int vl = d->fields().value (vcol->code().toLower()).toUInt();
            rgb_col = vl;
            vc = QColor::fromRgba(rgb_col);//d->fields().value (vcol->code().toLower()));
            colVal = QBrush (vc.value<QColor>());
        }
        else if (vcol->type()->attrType() == aType)//KKSAttrType::atRecordColorRef)
        {
            int idVal = d->fields().value (vcol->code().toLower()).toInt ();
            KKSObject * objCRef = l->loadIO (vcol->tableName(), true);
            KKSCategory * cRef = objCRef->category()->tableCategory();
            if(!cRef){
                objCRef->release ();
                return QVariant();
            }
            
            //cRef является табличной категорией, поэтому в ней все атрибуты имеют уникальные коды
            //использование данного вызова допустимо
            KKSAttribute * a = cRef->attribute(vcol->columnName());
            if(!a){
                objCRef->release ();
                return QVariant();
            }

            KKSObjectExemplar * colorC = l->loadEIO (idVal, objCRef);
            
            KKSAttrValue * av = colorC->attrValue (a->id());
            
            //rgb_col = av->value().valueVariant().toQColor();
            vc = av->value().valueVariant(); //(QColor::fromRgba(rgb_col));//d->fields().value (vcol->code().toLower()));
            colorC->release ();
            objCRef->release ();
            //qDebug () << __PRETTY_FUNCTION__ << vc << vc.value<QColor>();
            colVal = QBrush (vc.value<QColor>());
        }
        bool ier = false;
        if (isBackGround)
            ier = objModel->setData (wIndex, colVal, Qt::BackgroundRole);
        else
            ier = objModel->setData (wIndex, colVal, Qt::ForegroundRole);
        vcol->release ();
        if (ier)
            return colVal;
        else
            return QVariant();
    }
    
    return colVal;
}

/* Метод осуществляет загрузку записей ИО-справочника в модель.
 * Параметры:
 * editor -- редактор ИО
 * pObj -- ИО-справочник
 * l -- загрузчик данных из БД
 * t -- шаблон табличной категории
 * tv -- целевой виджет для модели
 * filters -- применяемые фильтры
 * pgDial -- если не 0, то индикатор загрузки данных
 * isCheckable -- флаг наличия чекбоксов в 1й колонке
 */
void KKSViewFactory :: loadEIOEx (QWidget *editor,
                                  const KKSObject *pObj, 
                                  KKSLoader *l, 
                                  const KKSTemplate *t, 
                                  QTreeView *tv, 
                                  const KKSList<const KKSFilterGroup *> & filters,
                                  QProgressDialog *pgDial
                                 )
{
    if (!t || !tv)
        return; 

    KKSMap<qint64, KKSEIOData *> objEx = l->loadEIOList (pObj, filters);
    KKSMap<qint64, KKSEIOData *>::const_iterator p;

    //
    // сортируем будущие колонки таблицы по порядку (KKSAttrView::order())
    //
    QList<KKSAttrView*> attrs_list = t->sortedAttrs();

    KKSSortFilterProxyModel *sortModel = new KKSSortFilterProxyModel (editor);
    QAbstractItemModel *oldModel = tv->model ();
    tv->setModel (sortModel);

    sortModel->setDynamicSortFilter (true);

    if (!sortModel->sourceModel())
        sortModel->setSourceModel (new QStandardItemModel());

    QAbstractItemModel *objModel = sortModel->sourceModel();//new QStandardItemModel ();
    QItemSelectionModel *selModel = tv->selectionModel ();
    if (qobject_cast<KKSObjEditor*>(editor))
    {
        QObject::connect (selModel, SIGNAL (currentChanged(const QModelIndex&, const QModelIndex&)), editor, SLOT (currentRecordChanged (const QModelIndex&, const QModelIndex&)) );
        QObject::connect (selModel, SIGNAL (selectionChanged (const QItemSelection&, const QItemSelection&)), editor, SLOT (currentRecSelChanged (const QItemSelection& , const QItemSelection&)) );
    }

    int i=0;

    if (objModel->rowCount () > 0)
    {
        objModel->removeRows (0, objModel->rowCount());
        objModel->removeColumns (0, objModel->columnCount());
    }

    QAbstractItemModel *oldObjModel = qobject_cast <QAbstractProxyModel *>(oldModel) ? qobject_cast <QAbstractProxyModel *>(oldModel)->sourceModel () : oldModel;
    objModel->insertColumns (0, oldObjModel ? oldObjModel->columnCount() : 1);
    if (oldObjModel)
        for (int i=0; i<objModel->columnCount(); i++)
            objModel->setHeaderData (i, Qt::Horizontal, oldObjModel->headerData (i, Qt::Horizontal));

    QStringList headers;

    if (pgDial)
    {
        pgDial->setMinimum (0);
        pgDial->setMaximum (objEx.count()*attrs_list.count());
    }
    int ncols = 1;
    int counter = 0;
    QModelIndex wIndex;
    QModelIndex prevIndex (wIndex);
    QList<int> pattrs;
    const KKSCategory * cobjCat (0);//= pObj->category()->tableCategory();
    int idPAttr (-1);
    if (pObj && pObj->category() && pObj->category()->tableCategory())
    {
        cobjCat = pObj->category()->tableCategory();
        pattrs = cobjCat->searchAttributesByType (KKSAttrType::atParent);
    }
    if (!pattrs.isEmpty())
        idPAttr = pattrs[0];
    for (p=objEx.constBegin(); p!=objEx.constEnd(); p++)
    {
        if (pgDial)
            pgDial->setValue (counter);
        
        KKSEIOData * d = p.value();
        if (idPAttr > 0)
        {
            const KKSCategoryAttr * cAttr = cobjCat->attribute (idPAttr);
            QString attrValue = d->fields().value ((cAttr->code(false)));
            KKSObjectExemplar * ioc = l->loadEIO(p.key(), pObj);
            KKSAttrValue * av = ioc ? ioc->attrValue (idPAttr) : 0;
            int idp = av ? av->value().valueVariant().toInt() : 0;
            if (idp > 0)
            {
                prevIndex = wIndex;
                while (prevIndex.isValid() && prevIndex.data (Qt::UserRole).toInt() != idp)
                    prevIndex = prevIndex.parent();
            }
            else
                prevIndex = QModelIndex();
            //qDebug () << __PRETTY_FUNCTION__ << av->value().valueVariant().toInt() << cAttr->code (false);
        }
        int nr = objModel->rowCount(prevIndex);
        objModel->insertRows (nr, 1, prevIndex);
        if (objModel->columnCount (prevIndex) == 0)
            objModel->insertColumns (0, ncols, prevIndex);
        wIndex = objModel->index (nr, 0, prevIndex);

        const KKSCategory * cat = t->category();
        QVariant bkColVal = drawViewCells (cat, d, KKSAttrType::atRecordColor, l, objModel, wIndex);
        QVariant fgColVal = drawViewCells (cat, d, KKSAttrType::atRecordTextColor, l, objModel, wIndex);

        int ii=0;
        
        for (int index=0; index<attrs_list.count(); index++)
        {
            if (pgDial)
                pgDial->setValue (counter++);
            KKSAttrView * v = attrs_list [index];
            QString attrCode = v->code();
            QString attrValue;
            if(v->type()->attrType() == KKSAttrType::atJPG){
                attrValue = QObject::tr("<Image data %1>").arg (i);
            }
            else if(v->type()->attrType() == KKSAttrType::atSVG){
                attrValue = QObject::tr("<SVG data %1>").arg (i);
            }
            else if(v->type()->attrType() == KKSAttrType::atXMLDoc){
                attrValue = QObject::tr("<XML document %1>").arg (i);
            }
            else if(v->type()->attrType() == KKSAttrType::atVideo){
                attrValue = QObject::tr("<Video data %1>").arg (i);
            }
            else
            {
                attrValue = d->fields().value(attrCode);
                if(attrValue.isEmpty())
                    attrValue = d->fields().value(attrCode.toLower());
                else if (attrValue.contains ("\n"))
                    attrValue = attrValue.mid (0, attrValue.indexOf("\n")) + "...";
            }

            if (i==0 && v->isVisible())
            {
                headers << v->title();
                sortModel->addAttrView (v);
                if (ii>0)
                {
                    if (!oldModel)
                        objModel->insertColumns (0, 1);
                    ncols++;
                }
            }
            QModelIndex wcIndex = objModel->index (wIndex.row(), ii, prevIndex);
            if (bkColVal.isValid ())
                objModel->setData (wcIndex, bkColVal, Qt::BackgroundRole);
            if (fgColVal.isValid ())
                objModel->setData (wcIndex, fgColVal, Qt::ForegroundRole);
            if (v->isVisible())
            {
                objModel->setData (wcIndex, attrValue, Qt::DisplayRole);
                ii++;
            }
            else
                objModel->setData (wcIndex, attrValue, Qt::UserRole+1);
        }
        i++;
    }

    if (!oldModel)
        for (int j=0; j<qMin (objModel->columnCount(), headers.count()); j++)
            objModel->setHeaderData (j, Qt::Horizontal, headers[j], Qt::DisplayRole);

    if (oldModel)
        delete oldModel;

}

/* Метод осуществляет обновление записи ИО-справочника в модели.
 * Параметры:
 * l -- загрузчик данных из БД
 * pObj -- ИО-справочник
 * idObjEx -- идентификатор ЭИО в ИО-справочнике
 * t -- шаблон табличной категории
 * sourceModel -- модель с ЭИО
 * recIndex -- индекс записи в модели
 * cat -- категория справочника
 * tableName -- название таблицы
 */
void KKSViewFactory :: updateEIOEx (KKSLoader *l,
                                    KKSObject *pObj,
                                    const KKSMap<qint64, KKSObjectExemplar*>& objRecs,
                                    const KKSTemplate *t,
                                    QAbstractItemModel *sourceModel,
                                    QAbstractItemView * recView
                                    )
{
    Q_UNUSED (l);
    if (!pObj || !t || !sourceModel)
        return;
    

    const KKSCategoryAttr * cAttrP = sourceModel->data(QModelIndex(), Qt::UserRole+3).value<const KKSCategoryAttr*>();
    const KKSCategoryAttr * cAttrB = sourceModel->data(QModelIndex(), Qt::UserRole+4).value<const KKSCategoryAttr*>();
    if (!cAttrB)
    {
        KKSAttribute * a = l->loadAttribute(ATTR_RECORD_FILL_COLOR);
        cAttrB = KKSCategoryAttr::create(a,false,false, QString());
        sourceModel->setData(QModelIndex(), cAttrB, Qt::UserRole+4);
        a->release();
        
    }
    const KKSCategoryAttr * cAttrF = sourceModel->data(QModelIndex(), Qt::UserRole+5).value<const KKSCategoryAttr*>();
    if (!cAttrF)
    {
        KKSAttribute * a = l->loadAttribute(ATTR_RECORD_TEXT_COLOR);
        cAttrF = KKSCategoryAttr::create(a,false,false, QString());
        sourceModel->setData(QModelIndex(), cAttrF, Qt::UserRole+5);
        a->release ();
    }
    for (KKSMap<qint64, KKSObjectExemplar*>::const_iterator pr = objRecs.constBegin();
            pr != objRecs.constEnd(); 
            pr++)
    {
        QModelIndex recIndex = searchModelRowsIndex (sourceModel, pr.key());
        KKSObjectExemplar * rec = pr.value();
        KKSEIOData * d = l->loadEIOInfo(rec->io()->id(), pr.key());//getRecordData (rec);
        if (cAttrP)
        {
            //
            // Если запись не содержится в модели иерархического справочника,
            // то мы находим подходящий родительский индекс, и добавляем запись в конец ветки.
            //
            int idParent = rec->attrValue(cAttrP->id())->value().value().toInt();
            QModelIndex pIndex = searchModelRowsIndex (sourceModel, idParent);
            QModelIndex pOldInd = recIndex.parent();
            if (recIndex.isValid() && pOldInd != pIndex)
            {
                sourceModel->removeRows(recIndex.row(), 1, pOldInd);
                int nr = sourceModel->rowCount(pIndex);
                bool isInserted = sourceModel->insertRows(nr, 1, pIndex);
                if (!isInserted)
                    return;
                recIndex = sourceModel->index(nr, 0, pIndex);
            }
            else if (!recIndex.isValid())
            {
                int nr = sourceModel->rowCount(pIndex);
                bool isInserted = sourceModel->insertRows(nr, 1, pIndex);
                if (!isInserted)
                    return;
                recIndex = sourceModel->index(nr, 0, pIndex);
            }
            sourceModel->setData (recIndex, rec->id(), Qt::UserRole);
            /*bool isTempl = */
            sourceModel->setData (recIndex, QVariant::fromValue<const KKSTemplate *>(t), Qt::UserRole+2);
            /*bool isData = */
            sourceModel->setData (recIndex, QVariant::fromValue<KKSEIOData *>(d), Qt::UserRole+1);
            //qDebug () << __PRETTY_FUNCTION__ << idParent << pIndex << recIndex << isData << isTempl;
        }
        else
        {
            bool isInserted (false);
            if (!recIndex.isValid())
            {
                int nr = sourceModel->rowCount();
                isInserted = sourceModel->insertRows(nr, 1);
                recIndex = sourceModel->index(nr, 0);
            }
            sourceModel->setData (recIndex, rec->id(), Qt::UserRole);
            /*bool isTempl = */
            sourceModel->setData (recIndex, QVariant::fromValue<const KKSTemplate *>(t), Qt::UserRole+2);
            /*bool isData = */
            sourceModel->setData (recIndex, QVariant::fromValue<KKSEIOData *>(d), Qt::UserRole+1);
            //qDebug () << __PRETTY_FUNCTION__ << recIndex << isInserted << isData << isTempl;
        }
        QIcon tIcon;
        if (d)
        {
            QString strIcon = d->sysFieldValue("r_icon");
            if (!strIcon.isEmpty())
            {
                QPixmap pIcon;
                pIcon.loadFromData (strIcon.toUtf8());
                tIcon = QIcon (pIcon);
            }
            else
                tIcon = QIcon(":/ddoc/rubric_item.png");
            bool ok;
            QString colString = d->fields().value (cAttrB->code(false));
            if (colString.isEmpty())
                colString = d->sysFields().value (cAttrB->code(false));
            qDebug () << __PRETTY_FUNCTION__ << cAttrB->code(false) << colString;
            quint64 vlb = colString.toLongLong (&ok);
            if (recView && (!ok || vlb == 0 || colString.isEmpty()))
            {
                QColor bc = recView->viewport()->palette().brush(QPalette::Base).color();
                vlb = bc.rgba();
            }
            sourceModel->setData (recIndex, vlb, Qt::BackgroundRole);
                //qDebug () << __PRETTY_FUNCTION__ << colString << isBSet;
            colString = d->fields().value (cAttrF->code(false));
            if (colString.isEmpty())
                colString = d->sysFields().value (cAttrF->code(false));
            quint64 vlf = colString.toLongLong (&ok);
            if (recView && (!ok || vlf == 0 || colString.isEmpty()))
            {
                QColor fc = recView->viewport()->palette().brush(QPalette::WindowText).color();//(Qt::black);
                vlf = fc.rgba();
            }
            sourceModel->setData (recIndex, vlf, Qt::ForegroundRole);
                //qDebug () << __PRETTY_FUNCTION__ << colString << isFSet;
            sourceModel->setData(recIndex.sibling(recIndex.row(), 0), tIcon, Qt::DecorationRole );
        }
        
    }
}

KKSEIOData * KKSViewFactory :: getRecordData (const KKSObjectExemplar * rec)
{
    KKSEIOData * dRec = new KKSEIOData;
    bool hasInterval = false;
    KKSCategoryAttr * a = NULL;
    const KKSCategory * c = rec->io()->category()->tableCategory();
    KKSMap<int, KKSCategoryAttr *>::const_iterator pca;
    for (pca = c->attributes().constBegin(); pca != c->attributes().constEnd() && !hasInterval; pca++)
    {
        a = pca.value();
        hasInterval = (a && 
            (a->type()->attrType() == KKSAttrType::atInterval ||
             a->type()->attrType() == KKSAttrType::atIntervalH) 
           );
    }
    int columns = rec->attrValues().count();
    
    for (int col=0; col<columns; col++)
    {
        QString code = QString(rec->attrValueIndex(col)->attribute()->code(false));//использование названия колонки для ключа QMap в классе KKSEIOData допустимо
        QString value = QString(rec->attrValueIndex(col)->value().value());

        if(code == "ii_rec_order" || 
           code == "id" || 
           code == "unique_id" ||
           code == "rr_name" ||
           code == "uuid_t" ||
           code == "last_update" ||
           code == "id_io_state" ||
           code == "r_icon" ||
           code == "record_fill_color" ||
           code == "record_text_color"
           )
        {
            dRec->addSysField(code, value);
        }
        //в данном случае получение атрибута по его коду (хотя он и не является уникальным) 
        //допустимо, поскольку категория описывает таблицу, а в таблице не может быть 
        //двух колонок (атрибутов) с одинаковым названием                    
        const KKSCategoryAttr* a = c->attribute(code);
        if (!a)
            continue;


        //
        // проверим на тип ИНТЕРВАЛ
        //
        if (hasInterval)
        {
            if (rec->attrValueIndex(col)->attribute()->type()->attrType() == KKSAttrType::atInterval ||
                rec->attrValueIndex(col)->attribute()->type()->attrType() == KKSAttrType::atIntervalH)
            {
                const KKSCategoryAttr* a = c->attribute(code);
                if (a && a->type()->attrType() == KKSAttrType::atInterval)
                {
                    KKSValue v;
                    v.setValue(value, KKSAttrType::atInterval);
                    value = v.value();
                }
                if (a && a->type()->attrType() == KKSAttrType::atIntervalH)
                {
                    KKSValue v;
                    v.setValue(value, KKSAttrType::atIntervalH);
                    value = v.value();
                }
            }
        }

        if(rec->attrValueIndex(col)->attribute()->type()->attrType() == KKSAttrType::atBool)
        {
            QString s = value.toLower();
            if(s == "t" || s == "true" || s == "1")
                value = QObject::tr("Yes");
            else
                value = QObject::tr("No");
        }

        if (rec->attrValueIndex(col)->attribute()->type()->attrType() == KKSAttrType::atDateTime)
        {
            KKSValue v(value, KKSAttrType::atDateTime);
            QDateTime dt = v.valueVariant().toDateTime();
            value = dt.toString("dd.MM.yyyy hh:mm:ss");
        }
        
        if (rec->attrValueIndex(col)->attribute()->type()->attrType() == KKSAttrType::atDateTimeEx)
        {
            KKSValue v(value, KKSAttrType::atDateTimeEx);
            QDateTime dt = v.valueVariant().toDateTime();
            value = dt.toString("dd.MM.yyyy hh:mm:ss.zzz");
        }

        int ier = 0;
        if(a->type()->attrType() == KKSAttrType::atList ||
           a->type()->attrType() == KKSAttrType::atSysChildCategoryRef ||
           a->type()->attrType() == KKSAttrType::atParent
            )
        {
            value = rec->attrValueIndex(col)->value().columnValue();
            QString sysValue = rec->attrValueIndex(col)->value().value();
            ier = dRec->addField(code, value);
            ier = dRec->addSysField(code, sysValue);
        }
        else if (a->type()->attrType() == KKSAttrType::atRecordColor ||
                 a->type()->attrType() == KKSAttrType::atRecordColorRef ||
                 a->type()->attrType() == KKSAttrType::atRecordTextColor ||
                 a->type()->attrType() == KKSAttrType::atRecordTextColorRef)
        {
            QString val = rec->attrValue(a->id())->value().valueForInsert();
            qint64 vl = val.toLongLong();
            ier = dRec->addField(code, QString::number (vl));
            ier = dRec->addSysField(code, QString::number (vl));
        }
        else            
            ier = dRec->addField(code, value);

        if (ier != OK_CODE)
        {
            qDebug () << __PRETTY_FUNCTION__ << QObject::tr ("Error");
            dRec->release ();
            return 0;
        }
        
    }
    return dRec;
}

/* Метод загружает категории верхнего уровня в соответствующий виджет.
 * Параметры:
 * catTemplW -- виджет с моделью
 * l -- загрузчик из БД
 * filters -- применяемые фильтры
 */
void KKSViewFactory :: loadCategories (KKSCategoryTemplateWidget* catTemplW, 
                                       KKSLoader* l, 
                                       const KKSList<const KKSFilterGroup *> & filters)
{
    KKSObject * refCatObj = l->loadIO (IO_CAT_ID, true);
    if (!refCatObj)
        return;

    KKSFilter * cMainFilter = refCatObj->category()->tableCategory()->createFilter (ATTR_IS_MAIN, QString("true"), KKSFilter::foEq);
    if (!cMainFilter)
        return;

    KKSFilter * cArchFilter = refCatObj->category()->tableCategory()->createFilter (ATTR_IS_ARCHIVED, QString("FALSE"), KKSFilter::foEq);
    if (!cArchFilter)
        return;
    KKSList<const KKSFilterGroup *> cFilterGroups;
    KKSList<const KKSFilter*> catFilters;
    
    catFilters.append (cMainFilter);
    catFilters.append (cArchFilter);
    
    KKSFilterGroup * cGroup = new KKSFilterGroup (true);
    
    cGroup->setFilters (catFilters);
    cFilterGroups.append (cGroup);
    cGroup->release ();
    QAbstractItemModel * catTypeTemplModel = initCategoriesModel (l, filters, cFilterGroups);
    catTemplW->uploadModel (catTypeTemplModel);
}

QAbstractItemModel* KKSViewFactory :: initCategoriesModel (KKSLoader* l, const KKSList<const KKSFilterGroup *> & filters, const KKSList<const KKSFilterGroup *>& catFilters)
{
    KKSObject *catTypeObj = l->loadIO (IO_CAT_TYPE_ID, true);
    if(!catTypeObj)
        return 0;

    KKSMap<qint64, KKSEIOData *> categTypeMap = l->loadEIOList (catTypeObj, filters);
    
    int n = categTypeMap.count ();
    KKSObject * refCatObj = l->loadIO (IO_CAT_ID, true);
    if (!refCatObj)
        return 0;
    QStandardItemModel *catTypeTemplModel = new QStandardItemModel (n, 1);
    catTypeTemplModel->setHeaderData (0, Qt::Horizontal, QObject::tr("Select category"), Qt::DisplayRole);
    KKSMap<qint64, KKSEIOData *>::const_iterator pCatTypes;


    KKSList<const KKSFilterGroup *> catByTypeFilters (catFilters);
    int itype=0;
    for (pCatTypes=categTypeMap.constBegin(); pCatTypes != categTypeMap.constEnd(); ++pCatTypes)
    {
        QModelIndex ctIndex = catTypeTemplModel->index (itype++, 0);
        catTypeTemplModel->setData (ctIndex, pCatTypes.value()->fields().value("name"), Qt::DisplayRole);
        catTypeTemplModel->setData (ctIndex, pCatTypes.key(), Qt::UserRole);
        catTypeTemplModel->setData (ctIndex, 0, Qt::UserRole+USER_ENTITY);
        catTypeTemplModel->setData (ctIndex, QIcon(":/ddoc/category_folder.png"), Qt::DecorationRole);
        QSize catRow = catTypeTemplModel->data (ctIndex, Qt::SizeHintRole).toSize();
        catRow.rheight() = 24;
        catTypeTemplModel->setData (ctIndex, catRow, Qt::SizeHintRole);
        

        KKSFilterGroup * cGroup = new KKSFilterGroup(true);
        KKSFilter * cTypeFilter = refCatObj->category()->tableCategory()->createFilter (ATTR_ID_IO_CAT_TYPE, QString::number (pCatTypes.key()), KKSFilter::foEq);
        if (!cTypeFilter)
            continue;

        cGroup->addFilter (cTypeFilter);
        catByTypeFilters.append (cGroup);
        cTypeFilter->release ();
        cGroup->release ();
        KKSMap<qint64, KKSEIOData *> categMap = l->loadEIOList (refCatObj, catByTypeFilters);
        catByTypeFilters.removeAt(catByTypeFilters.count()-1);

        catTypeTemplModel->insertRows (0, categMap.count(), ctIndex);
        catTypeTemplModel->insertColumns (0, 1, ctIndex);

        KKSMap<qint64, KKSEIOData *>::const_iterator pCat;
        
        int i=0;
        for (pCat=categMap.constBegin(); pCat != categMap.constEnd(); pCat++){
            KKSEIOData * cat = pCat.value();
            if(!cat)
                continue;

            QModelIndex cIndex = catTypeTemplModel->index (i, 0, ctIndex);

            catTypeTemplModel->setData (cIndex, cat->fieldValue("name"), Qt::DisplayRole);
            catTypeTemplModel->setData (cIndex, cat->fieldValue("id"), Qt::UserRole);
            catTypeTemplModel->setData (cIndex, 1, Qt::UserRole+USER_ENTITY);
            catTypeTemplModel->setData (cIndex, QIcon(":/ddoc/category.png"), Qt::DecorationRole);
            QSize catRow = catTypeTemplModel->data (cIndex, Qt::SizeHintRole).toSize();
            catRow.rheight() = 24;
            catTypeTemplModel->setData (cIndex, catRow, Qt::SizeHintRole);
            i++;
        }
    }

//    cMainFilter->release ();
//    cArchFilter->release ();

    refCatObj->release ();
    catTypeObj->release ();

    return catTypeTemplModel;
}

/* Метод создает виджет с записями шаблонов категорий.
 * Параметры:
 * idCat -- идентификатор категории
 * l -- загрузчик из БД
 * parent, f -- атрибуты Qt для целевого виджета.
 * Результат:
 * виджет с записями шаблонов.
 */
KKSRecWidget * KKSViewFactory :: createCategoryTemplates (int idCat, KKSLoader *l, QWidget *parent, Qt::WindowFlags f)
{
    KKSRecWidget *resWidget = new KKSRecWidget (false, Qt::Vertical, parent, f);
    QTreeView *tv = resWidget->getView();//new QTreeView ();
    if (!tv)
        return 0;
    tv->header()->setClickable (true);
    tv->header()->setSortIndicatorShown (true);
    tv->header()->setSortIndicator (0, Qt::AscendingOrder);
    tv->setSortingEnabled (true);
    
    resWidget->hideActionGroup (_ID_FILTER_GROUP);
    resWidget->hideActionGroup (_ID_VIEW_GROUP);
    resWidget->hideActionGroup (_ID_IMPORT_GROUP);
    resWidget->hideActionGroup (_ID_REPORT_GROUP);

    KKSItemDelegate *itemDeleg = new KKSItemDelegate (resWidget);
    tv->setItemDelegate (itemDeleg);
    QSortFilterProxyModel *sortTemplModel = new QSortFilterProxyModel (resWidget);
    if (!sortTemplModel)
        return resWidget;

    sortTemplModel->setSortCaseSensitivity (Qt::CaseInsensitive);
    sortTemplModel->setDynamicSortFilter (true);
    tv->setModel (sortTemplModel);
    QStandardItemModel * tModel = new QStandardItemModel (0, 1);
    sortTemplModel->setSourceModel (tModel);
    if (idCat > 0)
    {
        KKSList<KKSTemplate*> lTempls = l->loadCategoryTemplates (idCat);
        loadCategoryDbTemplates (lTempls, tModel);
    }

    tModel->setHeaderData (0, Qt::Horizontal, QObject::tr ("Category templates"));

    return resWidget;
}

/* Метод осуществляет загрузку шаблонов категорий в модель.
 * Параметры:
 * lTempls -- список шаблонов категории
 * model -- модель данных
 * parent -- индекс родительской категории.
 */
void KKSViewFactory :: loadCategoryDbTemplates (const KKSList<KKSTemplate*>& lTempls,
                                                QAbstractItemModel *model,
                                                const QModelIndex& parent)
{
    if (!model || lTempls.isEmpty())
        return;

    int nTempls = lTempls.count();

    if (model->rowCount (parent) > 0)
        model->removeRows (0, model->rowCount (parent), parent);

    if (model->columnCount (parent) > 1)
        model->removeColumns (0, model->columnCount (parent)-1, parent);
    else if (model->columnCount (parent) == 0)
        model->insertColumns (0, 1, parent);

    model->insertRows (0, nTempls, parent);
    for (int i=0; i<nTempls; i++)
    {
        QModelIndex wIndex = model->index (i, 0, parent);
        model->setData (wIndex, lTempls[i]->name(), Qt::DisplayRole);
        model->setData (wIndex, lTempls[i]->id(), Qt::UserRole);
        model->setData (wIndex, 0, Qt::UserRole+USER_ENTITY);
    }
}

/* Метод осуществляет загрузку шаблонов категорий в модель.
 * Параметры:
 * idCat -- идентификатор категории
 * l -- загрузчик из БД
 * model -- модель данных
 * parent -- индекс родительской категории.
 */
void KKSViewFactory :: loadCategoryTemplates (int idCat,
                                              KKSLoader* l,
                                              QAbstractItemModel *model,
                                              const QModelIndex& parent)
{
    if (!model || !l)
        return;

    KKSCategory *cat = l->loadCategory (idCat);
    if (!cat)
    {
        qCritical() << QObject::tr("Cannot load category id=%1").arg (idCat);
        QMessageBox::critical (0, QObject::tr("Category"), QObject::tr("Cannot load category id=%1").arg (idCat));
        return;
    }

    if (!model)
        return;

    KKSTemplate *defT = new KKSTemplate (cat->defTemplate());
    if (!defT)
    {
        qCritical() << QObject::tr("Cannot load default template");
        QMessageBox::critical (0, QObject::tr("Default template"), QObject::tr("Cannot load default template"));
        return;
    }

    KKSList<KKSTemplate*> lTempls = l->loadCategoryTemplates (idCat);
    int nTempls = lTempls.count();
    if (model->rowCount (parent) > 0)
        model->removeRows (0, model->rowCount (parent), parent);
    model->insertRows (0, (nTempls > 0 ? 2 : 1), parent);
    if (model->columnCount (parent) == 0)
        model->insertColumns (0, 1, parent);
    QModelIndex defTInd = model->index (0, 0, parent);
    if (!defTInd.isValid())
        return;
    model->setData (defTInd, defT->name(), Qt::DisplayRole);
    model->setData (defTInd, defT->id(), Qt::UserRole);
    model->setData (defTInd, 0, Qt::UserRole+USER_ENTITY);

    if (nTempls>0)
    {
        QModelIndex dTInd = model->index (1, 0, parent);
        model->setData (dTInd, QObject::tr("Templates"), Qt::DisplayRole);
        model->setData (dTInd, -1, Qt::UserRole);
        model->setData (dTInd, -1, Qt::UserRole+USER_ENTITY);

        loadCategoryDbTemplates (lTempls, model, dTInd);
    }
    defT->release ();

    if (cat->tableCategory())
    {
        model->insertRows (0, 1, parent);
        QModelIndex childCatIndex = model->index (0, 0, parent);
        if (!childCatIndex.isValid())
            return;
        int idChildCat = cat->tableCategory()->id();
        model->setData (childCatIndex, cat->tableCategory()->name(), Qt::DisplayRole);
        model->setData (childCatIndex, idChildCat, Qt::UserRole);
        model->setData (childCatIndex, 1, Qt::UserRole+USER_ENTITY);
        loadCategoryTemplates (idChildCat, l, model, childCatIndex);
    }

    cat->release ();
}

/* Метод осуществляет загрузку записей атрибутов категории.
 * Параметры:
 * cat -- категория
 * parent -- родительский виджет
 * f -- флаги визуального отображения
 * Результат:
 * виджет с атрибутами
 */
KKSRecWidget * KKSViewFactory :: createCategAttrsView (KKSLoader * loader, 
                                                       const KKSCategory *cat,
                                                       QWidget *parent,
                                                       Qt::WindowFlags f)
{
    if (!cat)
        return 0;

    KKSRecWidget * recWidget = new KKSRecWidget (false, Qt::Vertical, parent, f);
    QTreeView *tv = recWidget->getView();//new QTreeView ();
    tv->header()->setClickable (true);
    tv->header()->setSortIndicatorShown (true);
    tv->header()->setSortIndicator (5, Qt::AscendingOrder);
    tv->setSortingEnabled (true);


    recWidget->hideActionGroup (_ID_FILTER_GROUP);
    recWidget->hideActionGroup (_ID_IMPORT_GROUP);
    recWidget->hideActionGroup (_ID_VIEW_GROUP);
    recWidget->hideActionGroup (_ID_REPORT_GROUP);

    tv->setDragEnabled (true);
    tv->setAcceptDrops (true);
    tv->setDropIndicatorShown (true);
    KKSItemDelegate *itemDeleg = new KKSItemDelegate (recWidget);
    tv->setItemDelegate (itemDeleg);

    //делаем список с атрибутами в категории сортируемым
    KKSSortFilterProxyModel *sortModel = 0;
    if (!tv->model())
    {
        sortModel = new KKSSortFilterProxyModel (tv);//ksa было editor
        tv->setModel (sortModel);
    }
    else
    {
        sortModel = qobject_cast <KKSSortFilterProxyModel *>(tv->model());
        if (!sortModel)
            sortModel = new KKSSortFilterProxyModel (tv); 
        sortModel->clearAttrs ();
    }

    QAbstractItemModel *acModel = sortModel->sourceModel();//new QStandardItemModel ();
    if (!acModel)
        acModel = new KKSAttrModel (cat);
    else{
        sortModel->setSourceModel (0);
        delete acModel;
        acModel = new KKSAttrModel (cat);
    }

    //добавим в KKSSortFilterProxyModel набор атрибутов, которые задаются справочником атрибутов в категории
    //это надо для правильной фильтрации по соответствующим колонкам в таблице (с учетом типа колонок(атрибутов))
    KKSMap<int, KKSCategoryAttr *> acList = loader->loadCategoryAttrs(ATTRS_CAT_TABLE_CATEGORY_ID);
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


    sortModel->setSourceModel (acModel);
    int sortCol = tv->header()->sortIndicatorSection ();
    Qt::SortOrder sOrder = tv->header()->sortIndicatorOrder ();
    sortModel->sort (sortCol, sOrder);
/**/

    //sortModel->setDynamicSortFilter (true);

    QHeaderView * header = tv->header();
    int lIndex = header->logicalIndex(0);
    header->setResizeMode(lIndex, QHeaderView::ResizeToContents);
    int size = header->sectionSize(lIndex);
    if(size > 300){
        header->setResizeMode(QHeaderView::Interactive);
        header->resizeSection(lIndex, 300);
    }

    return recWidget;
}

/* Метод осуществляет загрузку записей атрибутов атрибута.
 * Параметры:
 * a -- атрибут
 * parent -- родительский виджет
 * f -- флаги визуального отображения
 * Результат:
 * виджет с атрибутами
 */
KKSRecWidget * KKSViewFactory :: createAttrAttrsView (KKSLoader * loader, 
                                                      const KKSAttribute *a,
                                                      bool mode,
                                                      QWidget *parent,
                                                      Qt::WindowFlags f)
{
    if (!a)
        return 0;
    
    KKSRecWidget * recWidget = new KKSRecWidget (mode, Qt::Vertical, parent, f);
    QTreeView *tv = recWidget->getView();//new QTreeView ();
    tv->header()->setClickable (true);
    tv->header()->setSortIndicatorShown (true);
    tv->header()->setSortIndicator (4, Qt::AscendingOrder);
    tv->setSortingEnabled (true);

    recWidget->hideActionGroup (_ID_FILTER_GROUP);
    recWidget->hideActionGroup (_ID_VIEW_GROUP);
    recWidget->hideActionGroup (_ID_IMPORT_GROUP);
    recWidget->hideActionGroup (_ID_REPORT_GROUP);
    
    QStandardItemModel * acModel = new KKSCatAttrsModel (0, 6);//QStandardItemModel (0, 4);
    acModel->setHeaderData (0, Qt::Horizontal, QObject::tr ("Name"));
    acModel->setHeaderData (1, Qt::Horizontal, QObject::tr ("Default value"));
    acModel->setHeaderData (2, Qt::Horizontal, QObject::tr ("Mandatory"));
    acModel->setHeaderData (3, Qt::Horizontal, QObject::tr ("Read only"));
    acModel->setHeaderData (4, Qt::Horizontal, QObject::tr ("Order"));
    acModel->setHeaderData (5, Qt::Horizontal, QObject::tr ("Directives"));

    updateAttrAttrsModel (a, acModel);
    
    //делаем список с атрибутами в атрибуте сортируемым
    KKSSortFilterProxyModel *sortModel = 0;
    sortModel = new KKSSortFilterProxyModel (tv);//ksa было editor
    tv->setModel (sortModel);
    sortModel->setDynamicSortFilter (true);
    sortModel->setSourceModel (acModel);
    
    //добавим в KKSSortFilterProxyModel набор атрибутов, которые задаются справочником атрибутов в атрибутах
    //это надо для правильной фильтрации по соответствующим колонкам в таблице (с учетом типа колонок(атрибутов))
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

    int sortCol = tv->header()->sortIndicatorSection ();
    Qt::SortOrder sOrder = tv->header()->sortIndicatorOrder ();
    sortModel->sort (sortCol, sOrder);

    QHeaderView * header = tv->header();
    int lIndex = header->logicalIndex(0);
    header->setResizeMode(lIndex, QHeaderView::ResizeToContents);
    int size = header->sectionSize(lIndex);
    if(size > 300){
        header->setResizeMode(QHeaderView::Interactive);
        header->resizeSection(lIndex, 300);
    }

    tv->setDragEnabled (true);
    tv->setAcceptDrops (true);
    tv->setDropIndicatorShown (true);
    KKSItemDelegate *itemDeleg = new KKSItemDelegate (recWidget);
    tv->setItemDelegate (itemDeleg);

    return recWidget;
}

/* Метод осуществляет обновление модели атрибутов в категории.
 * Параметры:
 * cat --категория
 * model -- целевая модель.
 */
void KKSViewFactory :: updateAttrModel (const KKSCategory *cat, QAbstractItemModel *model)
{
    if (!cat || !model)
        return;
    int n=model->rowCount ();
    KKSMap<int, KKSCategoryAttr *> attrs = cat->attributes();
    int na = attrs.size();
    int nCol = model->columnCount ();
    if (nCol < 6)
        return;
    bool isOk = true;
    if (n < na)
        isOk = model->insertRows (n, na-n);
    else if (n > na)
        isOk = model->removeRows (na, n-na);

    if (!isOk)
        return;

    int i=0;
    KKSMap<int, KKSCategoryAttr*>::const_iterator p;
    for (p=attrs.constBegin(); p != attrs.constEnd(); p++)
    {
        QModelIndex wIndex = model->index (i, 0);
        
        if (!wIndex.isValid())
            continue;
        
        model->setData (wIndex, p.key(), Qt::UserRole);
        if (!p.value())
            continue;

        p.value()->addRef ();

        //QVariant v = QVariant::fromValue<KKSCategoryAttr*>(p.value());
        //model->setData(wIndex, v, Qt::UserData+3); //сам атрибут

        wIndex = model->index (i, 1);
        QString ctitle (p.value()->title());
        model->setData (wIndex, ctitle, Qt::DisplayRole);

        wIndex = model->index (i, 2);
        model->setData (wIndex, p.value()->defValue().valueVariant(), Qt::DisplayRole);

        wIndex = model->index (i, 3);
        model->setData (wIndex, (p.value()->isMandatory() ? QObject::tr("Yes") : QObject::tr("No")), Qt::DisplayRole);

        wIndex = model->index (i, 4);
        model->setData (wIndex, (p.value()->isReadOnly() ? QObject::tr("Yes") : QObject::tr("No")), Qt::DisplayRole);

        wIndex = model->index (i, 5);
        model->setData (wIndex, QString::number(p.value()->order()), Qt::DisplayRole);

        wIndex = model->index (i, 6);
        model->setData (wIndex, p.value()->directives(), Qt::DisplayRole);

        p.value()->release ();
        i++;
    }
}

/* Метод осуществляет обновление модели атрибутов в атрибуте.
 * Параметры:
 * a --атрибут
 * model -- целевая модель.
 */
void KKSViewFactory :: updateAttrAttrsModel (const KKSAttribute *a, QAbstractItemModel *model)
{
    if (!a || !model)
        return;
    
    int n=model->rowCount ();
    KKSMap<int, KKSCategoryAttr *> attrs = a->attrs();
    
    int nCol = model->columnCount ();
    if (nCol < 6)
        return;
    
    bool isOk = true;
    if (n < attrs.size())
        isOk = model->insertRows (n, attrs.size()-n);
    else if (n > attrs.size())
        isOk = model->removeRows (attrs.size(), n-attrs.size());

    if (!isOk)
        return;

    int i=0;
    KKSMap<int, KKSCategoryAttr*>::const_iterator p;
    for (p=attrs.constBegin(); p != attrs.constEnd(); p++)
    {
        QModelIndex wIndex = model->index (i, 0);
        if (!wIndex.isValid())
            continue;
        model->setData (wIndex, p.key(), Qt::UserRole);
        if (!p.value())
            continue;

        p.value()->addRef ();
        
        wIndex = model->index (i, 1);
        QString ctitle (p.value()->title());
        model->setData (wIndex, ctitle, Qt::DisplayRole);

        wIndex = model->index (i, 2);
        model->setData (wIndex, p.value()->defValue().valueVariant(), Qt::DisplayRole);

        wIndex = model->index (i, 3);
        model->setData (wIndex, (p.value()->isMandatory() ? QObject::tr("Yes") : QObject::tr("No")), Qt::DisplayRole);

        wIndex = model->index (i, 4);
        model->setData (wIndex, (p.value()->isReadOnly() ? QObject::tr("Yes") : QObject::tr("No")), Qt::DisplayRole);
        
        wIndex = model->index (i, 5);
        model->setData (wIndex, QString::number(p.value()->order()), Qt::DisplayRole);

        wIndex = model->index (i, 6);
        model->setData (wIndex, p.value()->directives(), Qt::DisplayRole);

        p.value()->release ();
        
        i++;
    }
}

/* Метод осуществляет загрузку списка атрибутов по группам.
 * Параметры:
 * l -- загрузчик из БД
 * filters -- применяемые фильтры
 * mode -- наличие-отсутствие кнопок OK, Cancel, Apply
 * parent -- родительский виджет
 * f -- флаги визуального отображения.
 * Результат:
 * виджет с атрибутами
 */
KKSAttributesEditor * KKSViewFactory :: createAttrView (KKSLoader *l,
                                                        const KKSList<const KKSFilterGroup *> & filters,
                                                        bool mode,
                                                        QWidget *parent,
                                                        Qt::WindowFlags f)
{
    if (!l)
        return 0;

    KKSObject *attrTypesIO = l->loadIO (IO_ATTR_TYPE_ID, true);
    if (!attrTypesIO)
    {
        qCritical() << QObject::tr ("Cannot load attribute types");
        QMessageBox::critical (parent, QObject::tr ("Attributes types"), QObject::tr ("Cannot load attribute types"), QMessageBox::Ok);
        return 0;
    }

    KKSMap<int, KKSAttrType*> availAttrTypes;
    KKSMap<qint64, KKSEIOData *> attrTypesList = l->loadEIOList (attrTypesIO);
    KKSMap<qint64, KKSEIOData *>::const_iterator pAttrs;
    for (pAttrs = attrTypesList.constBegin(); pAttrs != attrTypesList.constEnd(); pAttrs++)
    {
        KKSAttrType *aType = new KKSAttrType ();
        if (!aType)
            continue;
        aType->setId (pAttrs.key());
        aType->setName (pAttrs.value()->fields ().value ("name"));
        aType->setCode (pAttrs.value()->fields ().value ("code"));
        availAttrTypes.insert (pAttrs.key(), aType);
        if (aType)
            aType->release ();
    }

    QString val_ref = QString("select id from io_categories where id_io_category_type in (1,2,8,9)");
    int attrId = 12;

    KKSList<const KKSFilterGroup *> filterGroupsRefs = AttrRefsFilters (l, attrId, val_ref);
    
    QMap<int, QString> io_refs = loadAttrRefs (l, filterGroupsRefs);
    
    KKSMap<int, KKSAGroup*> avAttrGroups = l->loadAvailAttrsGroups ();

    KKSAttributesEditor *aEditor = new KKSAttributesEditor (availAttrTypes, avAttrGroups, io_refs, parent, f);
    KKSObject *attrIO = l->loadIO (IO_ATTR_ID, true);
    if (!attrIO)
    {
        qCritical() << QObject::tr ("Cannot load attributes");
        QMessageBox::critical (parent, QObject::tr ("Attributes"), QObject::tr ("Cannot load attributes"), QMessageBox::Ok);
        return 0;
    }

    KKSRecWidget *recW = new KKSRecWidget (mode, Qt::Vertical, aEditor);
    QTreeView *tvAttrs = recW->getView();//new QTreeView ();
    tvAttrs->header()->setClickable (true);
    tvAttrs->header()->setSortIndicatorShown (true);
    tvAttrs->header()->setSortIndicator (0, Qt::AscendingOrder);
    tvAttrs->setSortingEnabled (true);
    tvAttrs->setSelectionMode (QAbstractItemView::ExtendedSelection);
    tvAttrs->setDragDropMode (QAbstractItemView::DragOnly);
    tvAttrs->setDropIndicatorShown (true);

    aEditor->setRecordsWidget (recW);
    recW->hideActionGroup (_ID_VIEW_GROUP);
    recW->hideActionGroup (_ID_IMPORT_GROUP);
    recW->hideActionGroup (_ID_REPORT_GROUP);

    recW->pbApply->setVisible (false);
    KKSEventFilter *ef = new KKSEventFilter (recW);
    tvAttrs->viewport()->installEventFilter (ef);

    //KKSMap<int, KKSEIOData *> attrList = l->loadEIOList (attrIO, filters);
    //KKSMap<int, KKSEIOData *>::const_iterator p;
    QSortFilterProxyModel *sortFilterProxyModel = new KKSSortFilterProxyModel (recW);
    tvAttrs->setModel (sortFilterProxyModel);
    sortFilterProxyModel->setDynamicSortFilter (true);
    KKSItemDelegate *itemDeleg = new KKSItemDelegate (aEditor);
    tvAttrs->setItemDelegate (itemDeleg);
    
    KKSMap<int, KKSAGroup*> attrsGroups = l->loadAttrsGroups();
    
    QAbstractItemModel *wModel = new QStandardItemModel (attrsGroups.size(), 4);//QStandardItemModel  (attrList.size(), 3);
    KKSMap<int, KKSAGroup*>::const_iterator p;
    sortFilterProxyModel->setSourceModel (wModel);
    sortFilterProxyModel->setSortCaseSensitivity (Qt::CaseInsensitive);
    sortFilterProxyModel->sort (0, Qt::AscendingOrder);
    wModel->setHeaderData (0, Qt::Horizontal, QObject::tr("Attribute"));
    wModel->setHeaderData (1, Qt::Horizontal, QObject::tr("Attribute type"));
    wModel->setHeaderData (2, Qt::Horizontal, QObject::tr("Attribute name"));
    wModel->setHeaderData (3, Qt::Horizontal, QObject::tr("Attribute reference"));
    int i=0;
    for (p=attrsGroups.constBegin(); p != attrsGroups.constEnd(); p++)
    {
        QModelIndex wIndex = wModel->index (i, 0);
        parseAttrGroup (l, p.value(), filters, wModel, wIndex);
        i++;
    }
    
    QHeaderView * header = tvAttrs->header();
    int lIndex = header->logicalIndex(0);
    header->setResizeMode(lIndex, QHeaderView::ResizeToContents);
    int size = header->sectionSize(lIndex);
    if(size > 300){
        header->setResizeMode(QHeaderView::Interactive);
        header->resizeSection(lIndex, 300);
    }


    tvAttrs->setRootIsDecorated(true);
    tvAttrs->update ();
    //qDebug () << __PRETTY_FUNCTION__ << tvAttrs->itemsExpandable() << tvAttrs->expandsOnDoubleClick () << tvAttrs->rootIsDecorated();

    if (mode)
    {
        recW->pbOk->setDefault (true);
        recW->pbOk->setAutoDefault (true);
        QKeySequence cancel_key (Qt::Key_Escape);
        recW->pbCancel->setShortcut (cancel_key);

        QObject::connect (recW->pbOk, SIGNAL(clicked()), aEditor, SLOT (accept()));
        QObject::connect (recW->pbCancel, SIGNAL(clicked()), aEditor, SLOT (reject()));
    }
    QObject::connect (recW->getView (), SIGNAL (doubleClicked (const QModelIndex &)), aEditor, SLOT (setAttr (const QModelIndex &)) );

    if (attrIO)
        attrIO->release ();

    if (attrTypesIO)
        attrTypesIO->release ();

    return aEditor;
}

/* Метод осуществляет обновление модели атрибутов.
 * Параметры:
 * l -- загрузчик из БД
 * wModel -- модель с атрибутами
 */
void KKSViewFactory :: updateAttributesModel (KKSLoader *l, QAbstractItemModel * wModel)
{
    if (!l || !wModel)
        return;

    KKSMap<int, KKSAGroup*> attrsGroups = l->loadAttrsGroups();
    if (wModel->rowCount() > 0)
    {
        int nr = wModel->rowCount();
        wModel->removeRows (0, nr);
    }
    wModel->insertRows (0, attrsGroups.count());
    if (wModel->columnCount() < 4)
    {
        int nc = wModel->columnCount();
        wModel->insertColumns (nc, 4-nc);
    }
    KKSMap<int, KKSAGroup*>::const_iterator p;
    int i=0;
    for (p=attrsGroups.constBegin(); p != attrsGroups.constEnd(); p++)
    {
        QModelIndex wIndex = wModel->index (i, 0);
        parseAttrGroup (l, p.value(), KKSList<const KKSFilterGroup *>(), wModel, wIndex);
        i++;
    }
}

/* Метод осуществляет фильтрацию списка атрибутов.
 * Параметры:
 * l -- загрузчик из БД
 * filters -- применяемые фильтры
 * aModel -- модель атрибутов
 */
void KKSViewFactory :: filterAttributesModel (KKSLoader *l, const KKSList<const KKSFilterGroup *> & filters, QAbstractItemModel * aModel)
{
    if (!l || !aModel)
        return;
    KKSMap<int, KKSAGroup*> attrsGroups = l->loadAttrsGroups();
    if (aModel->rowCount() > 0)
    {
        int nr = aModel->rowCount();
        aModel->removeRows (0, nr);
    }
    aModel->insertRows (0, attrsGroups.count());
    if (aModel->columnCount() < 4)
    {
        int nc = aModel->columnCount();
        aModel->insertColumns (nc, 4-nc);
    }
    KKSMap<int, KKSAGroup*>::const_iterator p;
    int i=0;
    for (p=attrsGroups.constBegin(); p != attrsGroups.constEnd(); p++)
    {
        QModelIndex wIndex = aModel->index (i, 0);
        parseAttrGroup (l, p.value(), filters, aModel, wIndex);
        i++;
    }

}

void KKSViewFactory :: parseAttrGroup (KKSLoader *l, const KKSAGroup* aGroup, const KKSList<const KKSFilterGroup *> & filters, QAbstractItemModel * aModel, const QModelIndex& parent)
{
    //Q_UNUSED (filters);
    if (!l || !aGroup || !aModel)
        return;

    KKSObject * io = l->loadIO (IO_ATTR_ID, true);
    if (!io)
        return;

    //--KKSMap<int, KKSEIOData *> attrs = l->loadEIOList (io, filters);
    //--QList<int> attrsKeys = attrs.keys();
    
    QModelIndex pIndex;
    if (parent.isValid())
        pIndex = parent;
    else
    {
        aModel->insertRows (0, 1, parent);
        pIndex = aModel->index (0, 0, parent);
    }
    
    aModel->setData (pIndex, aGroup->id(), Qt::UserRole);
    aModel->setData (pIndex, aGroup->name(), Qt::DisplayRole);
    aModel->setData (pIndex, 0, Qt::UserRole+USER_ENTITY);
    aModel->setData (pIndex, QIcon(":/ddoc/rubric.png").pixmap(24, 24), Qt::DecorationRole);
    
    int nChild = aGroup->childGroups().size();
    aModel->insertRows (0, nChild, pIndex);
    if (!aModel->columnCount (pIndex))
        aModel->insertColumns (0, 4, pIndex);
    
    int i=0;
    for (KKSMap<int, KKSAGroup*>::const_iterator p = aGroup->childGroups().constBegin();
            p != aGroup->childGroups().constEnd();
            p++)
    {
        QModelIndex wIndex = aModel->index (i, 0, pIndex);
        aModel->setData (wIndex, p.key(), Qt::UserRole);
        aModel->setData (wIndex, p.value()->name(), Qt::DisplayRole);
        aModel->setData (wIndex, 0, Qt::UserRole+USER_ENTITY);
        aModel->setData (wIndex, QIcon(":/ddoc/rubric.png").pixmap(24, 24), Qt::DecorationRole);
        parseAttrGroup (l, p.value(), filters/*KKSList<const KKSFilterGroup *>()*/, aModel, wIndex);
        i++;
    }
    //int nAttrs = aGroup->getAttributes().count();
    //aModel->insertRows (nChild, nAttrs, pIndex);
    for (KKSMap<int, KKSAttribute*>::const_iterator pa = aGroup->getAttributes().constBegin();
            pa != aGroup->getAttributes().constEnd();
            pa++)
    {
        //--if (!attrsKeys.contains (pa.key()))
        //--    continue;
        bool isIns = aModel->insertRows (i, 1, pIndex);
        if (!isIns)
            continue;
        if (aModel->columnCount(pIndex) == 0)
            aModel->insertColumns (0, 4, pIndex);
        QModelIndex wIndex = aModel->index (i, 0, pIndex);
        KKSAttribute *attr = pa.value();//l->loadAttribute (pa.key());
        if (attr)
        {
            //qDebug () << __PRETTY_FUNCTION__ << pa.key() << attr->id() << attr->name() << wIndex;
            aModel->setData (wIndex, pa.key(), Qt::UserRole);
            aModel->setData (wIndex, attr->name(), Qt::DisplayRole);
            aModel->setData (wIndex, 1, Qt::UserRole+USER_ENTITY);
            aModel->setData (wIndex, QIcon(":/ddoc/rubric_item.png").pixmap(24, 24), Qt::DecorationRole);
            QModelIndex tIndex = aModel->index (i, 1, pIndex);
            aModel->setData (tIndex, attr->type()->name(), Qt::DisplayRole);
            QModelIndex titleIndex = aModel->index (i, 2, pIndex);
            aModel->setData (titleIndex, attr->title(), Qt::DisplayRole);
            QModelIndex nIndex = aModel->index (i, 3, pIndex);
            if (attr->type()->id() == 2 ||
                attr->type()->id() == 7 ||
                attr->type()->id() == 12 ||
                attr->type()->id() == 17 ||
                attr->type()->id() == 19 ||
                attr->type()->id() == 26
            )
            {
                KKSObject * io = l->loadIO (attr->tableName(), true);
                QString refVal = QString("%1(%2)").arg ((io ? io->name() : QString())).arg (attr->name());
                aModel->setData (nIndex, refVal, Qt::DisplayRole);
                if (io)
                    io->release();
            }
        }
        i++;
    }
    io->release ();
}

/* Метод осуществляет зугрузку атрибутов в виджет. Используется для случая с пользовательскими шаблонами отображения
 * Параметры:
 * avAttrs -- списк возможный атрибутов
 * parent -- родительский виджет
 * f -- флаги визуального отображения
 * Результат:
 * виджет со списком атрибутов
 */
KKSAttributesEditor * KKSViewFactory :: createAvailAttrView (const KKSMap<int, KKSCategoryAttr *>& avAttrs,
                                                             QWidget *parent,
                                                             Qt::WindowFlags f)
{
    KKSAttributesEditor *aEditor = new KKSAttributesEditor ( parent, f);

    KKSRecWidget *recW = new KKSRecWidget (true, Qt::Vertical, aEditor);
    QTreeView *tvAttrs = recW->getView();//new QTreeView ();
    aEditor->setRecordsWidget (recW);
    recW->pbApply->setVisible (false);

    for (int i=0; i<=_GROUP_COUNT; i++)
        recW->hideActionGroup (i);

    QAbstractProxyModel * aSortModel = new KKSSortFilterProxyModel ();
    QStandardItemModel *wModel = new QStandardItemModel (avAttrs.size(), 1);
    wModel->setHeaderData (0, Qt::Horizontal, QObject::tr("Attribute"));
    int i=0;
    KKSMap<int, KKSCategoryAttr *>::const_iterator p;
    for (p=avAttrs.constBegin(); p!= avAttrs.constEnd(); p++)
    {
        QModelIndex wIndex = wModel->index (i, 0);
        wModel->setData (wIndex, p.key(), Qt::UserRole);
        wModel->setData (wIndex, p.value()->title(), Qt::DisplayRole);
        wModel->setData (wIndex, 1, Qt::UserRole+USER_ENTITY);
        KKSAttrView * av = new KKSAttrView (*p.value());
        (qobject_cast<KKSSortFilterProxyModel *>(aSortModel))->addAttrView (av);
        av->release ();
        i++;
    }
    aSortModel->setSourceModel (wModel);
    tvAttrs->setModel (aSortModel);
    KKSItemDelegate *itemDeleg = new KKSItemDelegate (aEditor);
    tvAttrs->setItemDelegate (itemDeleg);

    QObject::connect (recW->pbOk, SIGNAL(clicked()), aEditor, SLOT (accept()));
    QObject::connect (recW->pbCancel, SIGNAL(clicked()), aEditor, SLOT (reject()));

    return aEditor;
}

/* Метод осуществляет загрузку групп шаблона в модель.
 * Параметры
 * t -- шаблон
 * tModel -- модель
 */
void KKSViewFactory :: initTemplateGroups (KKSTemplate *t, QAbstractItemModel *tModel, QTreeView * tv)
{
    if (!tModel || (tModel->columnCount () == 0 && !tModel->insertColumns (0, 1)))
        return;

    KKSMap<int, KKSAttrGroup *> tGroups = t->groups ();
    tModel->removeRows (0, tModel->rowCount());
    if (tModel->columnCount () > 5)
        tModel->removeColumns (0, tModel->columnCount()-5);
    
    if (!tModel->insertRows (0, tGroups.count()))
        return;


    QList<KKSAttrGroup*> sortedGroups = tGroups.values();
    qSort (sortedGroups.begin(), sortedGroups.end(), compareAttrGroups);
    for (int i=0; i<sortedGroups.count(); i++)
    {
        insertTemplateGroup (sortedGroups[i], QModelIndex(), i, tModel, tv);
    }
}

/* Метод добавляет группу атрибутов шаблона в модель.
 * Параметры:
 * tAGroup -- группа атрибутов
 * parent -- родительский индекс
 * ind -- приоритет группы (добавляем в конец списка - соответственно здесь должен быть номер строки, где будет располагаться запись)
 * tModel -- модель шаблона
 */
void KKSViewFactory :: insertTemplateGroup (KKSAttrGroup *tAGroup, const QModelIndex& parent, int ind, QAbstractItemModel *tModel, QTreeView * tv)
{
    if (!tModel || !tAGroup || (!parent.isValid() && tAGroup->order()-ind > 1) )
        return;

    int nR = tModel->rowCount (parent);
    if (nR <= ind)
        tModel->insertRows (nR, ind-nR+1, parent);

    QModelIndex wIndex = tModel->index (ind, 0, parent);
    if (!wIndex.isValid())
        return;

    tModel->setData (wIndex, tAGroup->name(), Qt::DisplayRole);
    tModel->setData (wIndex, tAGroup->id(), Qt::UserRole);
    tModel->setData (wIndex, 1, Qt::UserRole+USER_ENTITY);
    tModel->setData (wIndex, QIcon(":/ddoc/rubric.png"), Qt::DecorationRole);

    QModelIndex wIOrder = wIndex.sibling(wIndex.row(), 4);
    tModel->setData (wIOrder, tAGroup->order(), Qt::UserRole);
    tModel->setData (wIOrder, tAGroup->order(), Qt::DisplayRole);


    KKSMap<int, KKSAttrView *>::const_iterator p = tAGroup->attrViews().constBegin ();
    if ((tModel->columnCount (wIndex) < 5 && 
         !tModel->insertColumns (0, 5, wIndex)) || 
         (tModel->rowCount (wIndex) < tAGroup->attrViews ().size() && 
         !tModel->insertRows (0, tAGroup->attrViews ().size(), wIndex)))
        return;

    KKSList < KKSAttrView *>  avList;
    for (;p != tAGroup->attrViews().constEnd(); p++)
        avList.append (p.value());

    qStableSort (avList.begin(), avList.end(), compareAttrViews);
    for (int i=0; i<avList.count(); i++)
    {
        QModelIndex attrIndex = tModel->index (i, 0, wIndex);
        tModel->setData (attrIndex, avList[i]->title(), Qt::DisplayRole);
        tModel->setData (attrIndex, avList[i]->id (), Qt::UserRole);
        tModel->setData (attrIndex, 0, Qt::UserRole+USER_ENTITY);
        tModel->setData (attrIndex, QIcon(":/ddoc/show_attrs.png"), Qt::DecorationRole);

        attrIndex = tModel->index (i, 1, wIndex);
        tModel->setData (attrIndex, avList[i]->defValue().valueVariant(), Qt::DisplayRole);
        tModel->setData (attrIndex, avList[i]->defValue().value(), Qt::UserRole);

        attrIndex = tModel->index (i, 2, wIndex);
        tModel->setData (attrIndex, avList[i]->isReadOnly() ? QObject::tr("Yes") : QObject::tr ("No"), Qt::DisplayRole);
        tModel->setData (attrIndex, avList[i]->isReadOnly(), Qt::UserRole);

        attrIndex = tModel->index (i, 3, wIndex);
        tModel->setData (attrIndex, avList[i]->isMandatory() ? QObject::tr("Yes") : QObject::tr ("No"), Qt::DisplayRole);
        tModel->setData (attrIndex, avList[i]->isMandatory(), Qt::UserRole);

        attrIndex = tModel->index (i, 4, wIndex);
        tModel->setData (attrIndex, avList[i]->order(), Qt::DisplayRole);
        tModel->setData (attrIndex, avList[i]->order(), Qt::UserRole);
    }

    const KKSList<KKSAttrGroup *> childGroups = tAGroup->sortedChildGroups ();
    for (int i=0; i<childGroups.count(); i++)
    {
        insertTemplateGroup (childGroups[i], wIndex, avList.count()+i, tModel, tv);
    }

    if(tAGroup->isExpanded())
        if(tv)
            tv->setExpanded(wIndex, true);
}

/* Метод обновляет группу атрибутов шаблона в модели.
 * Параметры:
 * tAGroup -- группа атрибутов
 * gIndex -- индекс группы
 * tModel -- модель шаблона
 */
void KKSViewFactory :: updateTemplateGroup (KKSAttrGroup *tAGroup, const QModelIndex& gIndex, QAbstractItemModel *tModel)
{
    if (!tModel || !tAGroup || !gIndex.isValid())// && tAGroup->order() != ind))
        return;

    QModelIndex wIndex = gIndex;//tModel->index (ind, 0, parent);
    if (!wIndex.isValid())
        return;

    tModel->setData (wIndex, tAGroup->name(), Qt::DisplayRole);
    tModel->setData (wIndex, tAGroup->id(), Qt::UserRole);
    tModel->setData (wIndex, 1, Qt::UserRole+USER_ENTITY);
    tModel->setData (wIndex, QIcon(":/ddoc/rubric.png"), Qt::DecorationRole);
    
    QModelIndex wIOrder = wIndex.sibling(wIndex.row(), 4);
    tModel->setData (wIOrder, tAGroup->order(), Qt::UserRole);
    tModel->setData (wIOrder, tAGroup->order(), Qt::DisplayRole);

    if (tModel->rowCount (wIndex) > 0)
    {
        int nr = tModel->rowCount (wIndex);
        int nc = tModel->columnCount (wIndex);
        tModel->removeRows (0, nr, wIndex);
        tModel->removeColumns (0, nc, wIndex);
    }

    KKSMap<int, KKSAttrView *>::const_iterator p = tAGroup->attrViews().constBegin ();
    if ((tModel->columnCount (wIndex) < 5 && !tModel->insertColumns (0, 5, wIndex)) || (tModel->rowCount (wIndex) < tAGroup->attrViews ().size() && !tModel->insertRows (0, tAGroup->attrViews ().size(), wIndex)))
        return;

    KKSList < KKSAttrView *>  avList;
    for (;p != tAGroup->attrViews().constEnd(); p++)
        avList.append (p.value());

    qStableSort (avList.begin(), avList.end(), compareAttrViews);
    for (int i=0; i<avList.count(); i++)
    {
        QModelIndex attrIndex = tModel->index (i, 0, wIndex);
        tModel->setData (attrIndex, avList[i]->title(), Qt::DisplayRole);
        tModel->setData (attrIndex, avList[i]->id (), Qt::UserRole);
        tModel->setData (attrIndex, 0, Qt::UserRole+USER_ENTITY);
        tModel->setData (attrIndex, QIcon(":/ddoc/show_attrs.png"), Qt::DecorationRole);

        attrIndex = tModel->index (i, 1, wIndex);
        tModel->setData (attrIndex, avList[i]->defValue().valueVariant(), Qt::DisplayRole);
        tModel->setData (attrIndex, avList[i]->defValue().value(), Qt::UserRole);

        attrIndex = tModel->index (i, 2, wIndex);
        tModel->setData (attrIndex, avList[i]->isReadOnly() ? QObject::tr("Yes") : QObject::tr ("No"), Qt::DisplayRole);
        tModel->setData (attrIndex, avList[i]->isReadOnly(), Qt::UserRole);

        attrIndex = tModel->index (i, 3, wIndex);
        tModel->setData (attrIndex, avList[i]->isMandatory() ? QObject::tr("Yes") : QObject::tr ("No"), Qt::DisplayRole);
        tModel->setData (attrIndex, avList[i]->isMandatory(), Qt::UserRole);

        attrIndex = tModel->index (i, 4, wIndex);
        tModel->setData (attrIndex, avList[i]->order(), Qt::DisplayRole);
        tModel->setData (attrIndex, avList[i]->order(), Qt::UserRole);
    }
}

/* Метод осуществляет загрузку id и названий ИО в соответствии с применяемыми фильтрами
 * Параметры:
 * l -- загрузчик из БД
 * filters -- применяемые фильтры
 * Результат:
 * QMap<int, QString> контейнер, содержащий ключи и названия ЭИО.
 */
QMap<int, QString> KKSViewFactory :: loadAttrRefs (KKSLoader *l, const KKSList<const KKSFilterGroup *> & filters)
{
    QMap<int, QString> io_refs;

    KKSObject *io = l->loadIO (IO_IO_ID, true);
    if(!io)
        return io_refs;

    KKSMap<qint64, KKSEIOData *> io_data = l->loadEIOList (io, filters);
    KKSMap<qint64, KKSEIOData *>::const_iterator p;

    io_refs.clear ();
    for (p=io_data.constBegin(); p != io_data.constEnd(); p++)
        io_refs.insert (p.key(), p.value()->fields ().value ("name"));

    io->release ();
    return io_refs;
}

KKSList<const KKSFilterGroup *> KKSViewFactory :: AttrRefsFilters (KKSLoader *l, int attrId, QString val_ref)
{
    KKSList<const KKSFilterGroup *> filterGroupsRefs;

    KKSList<const KKSFilter *> filtersRefs;

    KKSObject *io = l->loadIO (IO_IO_ID, true);
    if (!io)
        return filterGroupsRefs;

    KKSCategory *c = io->category()->tableCategory();
    if(!c)
    {
        io->release();
        return filterGroupsRefs;
    }
    const KKSFilter * fc = c->createFilter (attrId, val_ref, KKSFilter::foInSQL);
    
    io->release();

    if (!fc)
        return filterGroupsRefs;
    
    filtersRefs.append (fc);
    fc->release();

    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filtersRefs);
    filterGroupsRefs.append(group);
    group->release();

    return filterGroupsRefs;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSViewFactory::createAdditionalView(KKSTemplate* theTemplate, QString tableName)
// Purpose:    Implementation of KKSViewFactory::createAdditionalView()
// Метод осуществляет загрузку данных дополнительной таблицы
// Parameters:
// - t -- шаблон табличной категории
// - objEditor -- редактор ИО
// - pObj -- ИО, для которого добавляется в редактор доп. таблица
// - cat -- категория доп. таблицы
// - l -- загрузчик данных из БД
// - tableName -- название доп. таблицы
// - filters -- применяемые фильтры
// - parent -- родительский виджет
// - f -- флаги визуального отображения
// Return:     KKSRecWidget * с моделью данных
////////////////////////////////////////////////////////////////////////


KKSRecWidget * KKSViewFactory :: createAdditionalView (KKSTemplate *t,
                                                       KKSObjEditor *objEditor,
                                                       const KKSObject * pObj,
                                                       const KKSCategory *cat,
                                                       KKSLoader *l,
                                                       QString tableName,
                                                       const KKSList<const KKSFilterGroup *> & filters,
                                                       QWidget *parent,
                                                       Qt::WindowFlags f)
{
    KKSRecWidget *resWidget = new KKSRecWidget (false, Qt::Vertical, parent, f);
    QTreeView *tv = resWidget->getView();//new QTreeView ();
    tv->header()->setClickable (true);
    tv->header()->setSortIndicatorShown (true);
    tv->header()->setSortIndicator (0, Qt::AscendingOrder);
    tv->setSortingEnabled (true);
    KKSItemDelegate *itemDeleg = new KKSItemDelegate (tv);
    tv->setItemDelegate (itemDeleg);
    KKSEventFilter *ef = new KKSEventFilter (resWidget);
    tv->viewport()->installEventFilter (ef);
    if (tv->selectionModel())
        tv->selectionModel()->clearSelection ();

    //filter group
    if (resWidget->actFilter)
        QObject::connect (resWidget->actFilter, SIGNAL (triggered()), objEditor, SLOT (filterObjectE()) );
    
    //add_edit_del group
    if (resWidget->actAdd)
        QObject::connect (resWidget->actAdd, SIGNAL (triggered()), objEditor, SLOT (addObjectE()) );
    if (resWidget->actEdit)
    {
        QKeySequence kEdit (Qt::Key_F4);
        resWidget->actEdit->setShortcut (kEdit);
        QObject::connect (resWidget->actEdit, SIGNAL (triggered()), objEditor, SLOT (editObjectE()) );
    }
    if (resWidget->actDel)
        QObject::connect (resWidget->actDel, SIGNAL (triggered()), objEditor, SLOT (delObjectE()) );

    //openRPT group
    if (resWidget->actReportEdit)
        QObject::connect (resWidget->actReportEdit, SIGNAL (triggered()), objEditor, SLOT (editObjectEAsReport()) );
    if (resWidget->actReportOpen)
        QObject::connect (resWidget->actReportOpen, SIGNAL (triggered()), objEditor, SLOT (openObjectEAsReport()) );

    //import group
    if (resWidget->actImport)
        QObject::connect (resWidget->actImport, SIGNAL (triggered()), objEditor, SLOT (importObjectE()) );
    if (resWidget->actExport)
        QObject::connect (resWidget->actExport, SIGNAL (triggered()), objEditor, SLOT (exportObjectE()) );
    
    //view group
    if (resWidget->actSetView)
        QObject::connect (resWidget->actSetView, SIGNAL (triggered()), objEditor, SLOT (setView()) );

    QObject::connect (tv, SIGNAL (doubleClicked(const QModelIndex&)), objEditor, SLOT (editCurrentRec(const QModelIndex&)) );
    if ( !t )
        return resWidget; 

    loadEIOEx (objEditor, pObj, l, t, tv, filters, true, cat, tableName);
    return resWidget;
}

/* Метод осуществляет поиск целочисленных данных iData внутри модели sourceMod, начиная с родительского индекса parent в роли role.
 */
QModelIndex KKSViewFactory :: searchModelIndex (QAbstractItemModel * sourceMod, qint64 iData, const QModelIndex& parent, int role)
{
    if (!sourceMod)
        return QModelIndex ();

    QModelIndex pIndex = parent;
    int nr = sourceMod->rowCount (pIndex);
    int nc = sourceMod->columnCount (pIndex);
    
    for (int i=0; i<nr; i++)
        for (int j=0; j<nc; j++)
        {
            QModelIndex wIndex = sourceMod->index (i, j, pIndex);
            int vData (wIndex.data (role).toInt());
            if (vData == iData)
                return wIndex;
            else if (sourceMod->rowCount (wIndex) > 0 && j == 0)
            {
                QModelIndex childIndex = searchModelIndex (sourceMod, iData, wIndex, role);
                if (childIndex.isValid())
                    return childIndex;
            }
            else
                continue;
        }
    return QModelIndex();
}

QModelIndex KKSViewFactory :: searchModelRowsIndex (QAbstractItemModel * sourceMod, qint64 iData, const QModelIndex& parent, int role)
{
    if (!sourceMod)
        return QModelIndex ();

    QModelIndex pIndex = parent;
    int nr = sourceMod->rowCount (pIndex);
    
    for (int i=0; i<nr; i++)
    {
        QModelIndex wIndex = sourceMod->index (i, 0, pIndex);
        int vData (wIndex.data (role).toInt());
        if (vData == iData)
            return wIndex;
        else if (sourceMod->rowCount (wIndex) > 0)
        {
            QModelIndex childIndex = searchModelRowsIndex (sourceMod, iData, wIndex, role);
            if (childIndex.isValid())
                return childIndex;
        }
        else
            continue;
    }
    return QModelIndex();
}

QModelIndex KKSViewFactory :: searchModelRowsIndexMultiType (QAbstractItemModel * sourceMod, qint64 iData, qint64 typeVal, const QModelIndex& parent, int role)
{
    if (!sourceMod)
        return QModelIndex ();

    QModelIndex pIndex = parent;
    int nr = sourceMod->rowCount (pIndex);
    
    for (int i=0; i<nr; i++)
    {
        QModelIndex wIndex = sourceMod->index (i, 0, pIndex);
        if (!wIndex.isValid())
            continue;
        int vData (wIndex.data (role).toInt());
        int typeData (wIndex.data (Qt::UserRole+USER_ENTITY).toInt());
        //qDebug () << __PRETTY_FUNCTION__ << typeData << typeVal;
        if (vData == iData && typeData == typeVal)
            return wIndex;
        else if (sourceMod->rowCount (wIndex) > 0)
        {
            QModelIndex childIndex = searchModelRowsIndexMultiType (sourceMod, iData, typeVal, wIndex, role);
            if (childIndex.isValid())
                return childIndex;
        }
        else
            continue;
    }
    return QModelIndex();
}

void KKSViewFactory::getSearchTemplates (KKSLoader * loader, QAbstractItemModel * searchTModel, const QModelIndex& pIndex, bool withSearchTemplates)
{
    if (!searchTModel || !loader)
        return;

    int ncount (0);
    int nCols = searchTModel->columnCount (pIndex);
    KKSMap<int, KKSSearchTemplateType *> stt = loader->loadSearchTemplateTypes();
    QModelIndex prevIndex (pIndex);
    for (KKSMap<int, KKSSearchTemplateType *>::const_iterator ps = stt.constBegin();
            ps != stt.constEnd();
            ++ps)
    {
        KKSSearchTemplateType * st = ps.value();
        if (!st)
            continue;

        QModelIndex wIndex;
//        qDebug () << __PRETTY_FUNCTION__ << st->id() << st->name();
/*        if ((prevIndex.isValid() && st->parent() && st->parent()->id() == prevIndex.data(Qt::UserRole).toInt())
            || (!prevIndex.isValid() && !st->parent()))
        {
            ncount = searchTModel->rowCount (prevIndex);
            searchTModel->insertRows(ncount, 1, prevIndex);
            if (searchTModel->columnCount (prevIndex) == 0)
                searchTModel->insertColumns (0, nCols, prevIndex);
            wIndex = searchTModel->index (ncount, 0, prevIndex);
            prevIndex = wIndex;
            ncount++;
        }
        else */
        if (st->parent() )//&& prevIndex.isValid())
        {
            prevIndex = searchModelRowsIndexMultiType (searchTModel, st->parent()->id(), 0, QModelIndex(), Qt::UserRole);
            ncount = searchTModel->rowCount (prevIndex);
            bool isIns = searchTModel->insertRows(ncount, 1, prevIndex);
            if (!isIns)
                continue;
            if (searchTModel->columnCount (prevIndex) == 0)
                searchTModel->insertColumns (0, nCols, prevIndex);
            wIndex = searchTModel->index (ncount, 0, prevIndex);
//            qDebug () << __PRETTY_FUNCTION__ << st->id() << st->parent()->id() << prevIndex << ncount << isIns << wIndex;
            prevIndex = wIndex;
            ncount++;
        }
        else
        {
            prevIndex = pIndex;
            ncount = searchTModel->rowCount (prevIndex);
            searchTModel->insertRows(ncount, 1, prevIndex);
            if (searchTModel->columnCount (prevIndex) == 0)
                searchTModel->insertColumns (0, nCols, prevIndex);
            wIndex = searchTModel->index (ncount, 0, prevIndex);
            prevIndex = wIndex;
            ncount++;
        }
        searchTModel->setData (wIndex, st->name(), Qt::DisplayRole);
        searchTModel->setData (wIndex, st->id (), Qt::UserRole);
        searchTModel->setData (wIndex, 0, Qt::UserRole+USER_ENTITY);
        searchTModel->setData (wIndex, QIcon(":/ddoc/rubric.png").pixmap(24, 24), Qt::DecorationRole);
        QSize searchRow = searchTModel->data (wIndex, Qt::SizeHintRole).toSize();
        searchRow.rheight() = 24;
        searchTModel->setData (wIndex, searchRow, Qt::SizeHintRole);
        
        if (!withSearchTemplates)
            continue;
        KKSMap<qint64, KKSSearchTemplate *> searchTemplates = loader->loadSearchTemplatesByType(st);
        int ii (searchTModel->rowCount (wIndex));
        searchTModel->insertRows (ii == 0 ? ii : ii-1, searchTemplates.count(), wIndex);
        if (searchTModel->columnCount (wIndex) == 0)
            searchTModel->insertColumns (0, nCols, wIndex);
        for (KKSMap<qint64, KKSSearchTemplate *>::const_iterator p=searchTemplates.constBegin();
                p != searchTemplates.constEnd(); p++)
        {
            QModelIndex wsIndex = searchTModel->index (ii, 0, wIndex);
            setSearchTemplateIntoModel (p.value(), searchTModel, wsIndex);
            ii++;
        }
        
        searchTemplates.clear ();
    }
}

void KKSViewFactory::setSearchTemplateIntoModel (const KKSSearchTemplate * stdb, QAbstractItemModel * searchMod, const QModelIndex& wIndex)
{
    if (!stdb || stdb->id() <= 0 || !searchMod || !wIndex.isValid())
        return;

    int ii = wIndex.row();
    int res = stdb->id();
    QString stName = stdb->name ();

    searchMod->setData (wIndex, stName, Qt::DisplayRole);
    searchMod->setData (wIndex, res, Qt::UserRole);
    QModelIndex wsIndex = wIndex.sibling (ii, 1);
    searchMod->setData (wsIndex, stdb->authorName(), Qt::DisplayRole);
    searchMod->setData (wsIndex, res, Qt::UserRole);
    searchMod->setData (wsIndex, stdb->idAuthor (), Qt::UserRole+1);
    searchMod->setData (wsIndex, 1, Qt::UserRole+USER_ENTITY);

    wsIndex = wIndex.sibling (ii, 2);
    searchMod->setData (wsIndex, stdb->creationDatetime().toString("dd.MM.yyyy"), Qt::DisplayRole);
    searchMod->setData (wsIndex, stdb->id (), Qt::UserRole);
    searchMod->setData (wsIndex, stdb->idAuthor (), Qt::UserRole+1);
    searchMod->setData (wsIndex, 1, Qt::UserRole+USER_ENTITY);

    wsIndex = wIndex.sibling (ii, 3);
    searchMod->setData (wsIndex, stdb->categoryName(), Qt::DisplayRole);
    searchMod->setData (wsIndex, stdb->id (), Qt::UserRole);
    searchMod->setData (wsIndex, stdb->idAuthor (), Qt::UserRole+1);
    searchMod->setData (wsIndex, 1, Qt::UserRole+USER_ENTITY);

    wsIndex = wIndex.sibling (ii, 4);
    searchMod->setData (wsIndex, stdb->type()->name(), Qt::DisplayRole);
    searchMod->setData (wsIndex, stdb->id (), Qt::UserRole);
    searchMod->setData (wsIndex, stdb->idAuthor (), Qt::UserRole+1);
    searchMod->setData (wsIndex, 1, Qt::UserRole+USER_ENTITY);

    searchMod->setData (wIndex, stdb->idAuthor(), Qt::UserRole+1);
    searchMod->setData (wIndex, QIcon(":/ddoc/rubric_item.png").pixmap(24, 24), Qt::DecorationRole);
    searchMod->setData (wIndex, 1, Qt::UserRole+USER_ENTITY);
    QSize searchRow = searchMod->data (wIndex, Qt::SizeHintRole).toSize();
    searchRow.rheight() = 24;
    searchMod->setData (wIndex, searchRow, Qt::SizeHintRole);
    
}
