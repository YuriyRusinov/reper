#include "kksincludeswidget.h"
#include "KKSRecWidget.h"
#include "KKSItemDelegate.h"
#include "KKSEventFilter.h"
#include "KKSSortFilterProxyModel.h"
#include "KKSRubricModel.h"
#include "KKSRubricMessageBox.h"

#include "KKSRubric.h"
#include <KKSCategory.h>
#include <defines.h>
#include "ui/rubricform.h"

#include <QInputDialog>
#include <QBuffer>
#include <QMessageBox>
#include <QTreeView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QHeaderView>
#include <QFileDialog>
#include <QtDebug>
#include <QTreeView>
#include <QGridLayout>
#include <QCloseEvent>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QSize>
#include <QToolBar>
#include <QAction>
#include <QSplitter>
#include <QStringList>
#include <QLayout>
#include <QMessageBox>
#include <QMenu>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QDir>

KKSIncludesWidget::KKSIncludesWidget(KKSRubric * rootRubric, 
                                     bool isAttach,
                                     bool isDocs,
                                     bool forCategory,
                                     bool forRecord,
                                     QWidget *parent,
                                     Qt::WindowFlags flags)
    : QWidget (parent, flags),
    m_rootRubric (rootRubric),
    isMyDoc (isDocs),
    isChanged (false),
    isRec (forRecord),
    tBRubrActions (new QToolBar (this)),
    spRubrics (new QSplitter (Qt::Horizontal, this)),
    twIncludes (new QTreeView (spRubrics)),
    tvItems (new QTreeView ()),
    recWItems (new KKSRecWidget (tvItems, false, spRubrics)),
    pMenu (new QMenu(this))
{
    //Q_INIT_RESOURCE (kksgui_icon_set);

    if (m_rootRubric)
        m_rootRubric->addRef();
    else
        m_rootRubric = new KKSRubric ();
    
    if (isRec)
        twIncludes->setSelectionMode (QAbstractItemView::ExtendedSelection);
    else
        tvItems->setSelectionMode (QAbstractItemView::ExtendedSelection);

    tBRubrActions->setIconSize (QSize (24, 24));
    QHeaderView * hItems = tvItems->header();
    hItems->setSortIndicatorShown (true);
    hItems->setStretchLastSection (true);
    hItems->setClickable (true);
    QAbstractItemDelegate * iDeleg = new KKSItemDelegate ();
    tvItems->setItemDelegate (iDeleg);
    KKSEventFilter * ef = new KKSEventFilter ();
    tvItems->viewport()->installEventFilter (ef);

    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (tBRubrActions, 0, 0, 1, 1);

    spRubrics->addWidget (twIncludes);
    if (recWItems->layout())
    {
        QLayout * rLay = recWItems->layout();
        rLay->setContentsMargins (0, 0, 0, 0);
        rLay->setSpacing (0);
    }
    spRubrics->addWidget (recWItems);
    gLay->addWidget (spRubrics, 1, 0, 1, 1);

    init ();
    initActions ();
    Q_UNUSED (isAttach);
    QSizePolicy rSizeP (QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    tvItems->setSizePolicy (rSizeP);
    recWItems->setVisible (false);
    recWItems->actAdd->setToolTip (tr("Create new document and put it into rubric"));
    recWItems->actEdit->setToolTip (tr("Edit document in rubric"));
    recWItems->actDel->setToolTip (tr("Delete document both from rubric and database"));
    recWItems->hideGroup (0);
    recWItems->hideGroup (2);//hideToolBar();//hideGroup (2);
    recWItems->hideGroup (3);

    if (forCategory)
        this->setWindowTitle (tr("Rubrics"));

    QItemSelectionModel * selModel = twIncludes->selectionModel ();
    if (selModel)
        connect (selModel, \
                 SIGNAL (selectionChanged (const QItemSelection&, const QItemSelection&)), \
                 this, \
                 SLOT (rubricSelectionChanged (const QItemSelection&, const QItemSelection&))
                );
    connect (recWItems, SIGNAL (addEntity(QAbstractItemModel *, const QModelIndex&)), this, SLOT (createRubricItem(QAbstractItemModel *, const QModelIndex&)) );
    connect (recWItems, SIGNAL (editEntitiesList (QAbstractItemModel *, const QItemSelection&)), this, SLOT (editSelectedDocs (QAbstractItemModel *, const QItemSelection&)) );
//    connect (recWItems, SIGNAL (editEntity(QAbstractItemModel *, const QModelIndex& )), this, SLOT (editRubricDoc(QAbstractItemModel *, const QModelIndex&)) );
    connect (twIncludes, SIGNAL (doubleClicked(const QModelIndex &)), this, SLOT (slotRubricItemDblClicked(const QModelIndex &)) );
    connect (recWItems->getView(), SIGNAL (doubleClicked(const QModelIndex &)), this, SLOT (slotRubricItemEdit(const QModelIndex &)) );
    connect (recWItems, SIGNAL (delEntitiesList (QAbstractItemModel *, const QItemSelection&)), this, SLOT (delSelectedDocs (QAbstractItemModel *, const QItemSelection&)) );
//    connect (recWItems->actDel, SIGNAL (triggered()), this, SLOT (delRubricItem()) );
}

KKSIncludesWidget::~KKSIncludesWidget()
{
    if(m_rootRubric)
        m_rootRubric->release();
}

void KKSIncludesWidget::init()
{
    QAbstractItemModel * model = new KKSRubricModel (m_rootRubric, isRec);
    //new KKSRubricModel (m_rootRubric);new QStandardItemModel (0, 1);
    model->setHeaderData(0, Qt::Horizontal, tr("Name"), Qt::DisplayRole);

    QItemDelegate * includesDelegate = new KKSItemDelegate ();//this);

    twIncludes->setItemDelegate (includesDelegate);
    twIncludes->header()->resizeSection(0, 550);
    twIncludes->setModel (model);

    QModelIndex pInd = QModelIndex();
    expandAllIndexes (pInd);

    //initTwIncludes();
}

void KKSIncludesWidget :: initActions (void)
{
    QStringList icons;
    icons << ":/ddoc/rubric_add.png" 
          //<< ":/ddoc/rubric_add.png"
          << ":/ddoc/rubric_edit.png" 
          << ":/ddoc/rubric_delete.png"
          << ":/ddoc/rubric_item_add.png" 
          << ":/ddoc/rubric_item_open.png" 
          << ":/ddoc/rubric_item_remove.png";

    QStringList slotList;
    slotList << SLOT (addRubric())
             //<< SLOT (copyFromRubric())
             << SLOT (editRubric()) 
             << SLOT (delRubric()) 
             << SLOT (addRubricItem()) 
             << SLOT (editRubricItem()) 
             << SLOT (delRubricItem());

    QStringList titlesList;
    titlesList << tr ("&Add new rubric")
               //<< tr ("&Copy from rubric")
               << tr ("&Edit rubric") 
               << tr ("&Delete rubric") 
               << tr ("Add rubric item") 
               << tr ("&Open rubric item") 
               << tr ("&Remove item(s) from selected rubric only");

    for (int i=0; i<=5; i++)
    {
        QAction * aRubricA = new QAction (QIcon(icons[i]), titlesList[i], this);
        tBRubrActions->addAction (aRubricA);
        if (!isRec && i != 4 && i != 5)
            pMenu->addAction (aRubricA);
        connect (aRubricA, SIGNAL (triggered()), this, slotList[i].toAscii().constData() );
        if (i==2)
        {
            tBRubrActions->addSeparator ();
            pMenu->addSeparator();
        }
    }
    tBRubrActions->addSeparator();
    pMenu->addSeparator();
    QAction * aRubrWidgetRotate = new QAction(QIcon(":/ddoc/orientation_icon.png"), tr ("Rotate widget"), this);
    connect (aRubrWidgetRotate, SIGNAL (triggered()), this, SLOT (turnRubricSplitter()) );
    tBRubrActions->addAction (aRubrWidgetRotate);
    pMenu->addAction (aRubrWidgetRotate);
    QAction * actSyncSep = new QAction (this);
    actSyncSep->setSeparator (true);
    this->recWItems->insertToolBarAction (recWItems->actRefresh, actSyncSep);

    QAction * actSyncSet = new QAction (QIcon(":/ddoc/sync.png"), tr ("Set synchronization parameters"), this);
    recWItems->insertToolBarAction (actSyncSep, actSyncSet);
    connect (actSyncSet, SIGNAL (triggered()), this, SLOT (setSyncSettings ()) );

    QAction * actPutIntoAnotherRubr = new QAction (QIcon(":/ddoc/copy_to_rubric.png"), tr ("Put into another rubric"), this);
    recWItems->insertToolBarAction (actSyncSep, actPutIntoAnotherRubr);
    connect (actPutIntoAnotherRubr, SIGNAL (triggered()), this, SLOT (putIntoAnotherRubric()) );

    QAction * actSendIO = new QAction (QIcon(":/ddoc/send_as_mail.png"), tr("Send as email"), this);
    recWItems->insertToolBarAction (actSyncSep, actSendIO);
    connect (actSendIO, SIGNAL (triggered()), this, SLOT (sendIOS()) );

    QAction * actAccess = new QAction (QIcon(":/ddoc/access_icon.png"), tr("Set access rules"), this);
    recWItems->insertToolBarAction (actSyncSep, actAccess);
    connect (actAccess, SIGNAL (triggered()), this, SLOT (setAccessRules()) );

    QAction * aRubrIconSet = new QAction (QIcon(":/ddoc/rubric_icon_set.png"), tr ("Set icon"), this);
    pMenu->addAction (aRubrIconSet);
    tBRubrActions->addAction (aRubrIconSet);
    connect (aRubrIconSet, SIGNAL (triggered()), this, SLOT (setRubricIcon()) );
}

void KKSIncludesWidget::initTwIncludes()
{
    if(!m_rootRubric)
        return;

    int cnt = m_rootRubric->rubrics().count();
    if(cnt == 0)
        return;

    parseRubric(m_rootRubric, QModelIndex());
}

void KKSIncludesWidget::parseRubric(KKSRubric * rubric, QModelIndex index)
{
    if(!rubric)
        return;

    int cnt = rubric->rubrics().count();
    
    for(int i=0; i<cnt; i++){
        KKSRubric * r = rubric->rubric(i);
        QModelIndex cIndex = appendRubricRow(r, index);
        QAbstractItemModel * model = twIncludes->model();
        model->setData(cIndex, r->name(), Qt::ToolTipRole);
        if (!r->getIcon().isNull())
            model->setData(cIndex, r->getIcon(), Qt::DecorationRole);
        parseRubric(r, cIndex);
        parseItems(r, cIndex);
    }
}

void KKSIncludesWidget::parseItems(KKSRubric * r, QModelIndex index)
{
    if(!r)
        return;

    if (r->getCategory ())
    {
/*        QAbstractItemModel * model = twIncludes->model();
        int cnt = model->rowCount(index);
        model->insertRows (cnt, 1, index);
        if (cnt == 0)
        {
            if (model->columnCount(index) == 0)
                model->insertColumns(0, 1, index);
        }
        QModelIndex cIndex = model->index (cnt, 0, index);
        model->setData (cIndex, tr ("View attachments ..."), Qt::DisplayRole);
        model->setData (cIndex, 2, Qt::UserRole);//is item
        model->setData (cIndex, QIcon(":/ddoc/rubric.png"), Qt::DecorationRole);
 */
        return;
    }
    int cnt = r->items().count();
    for(int i=0; i<cnt; i++){
        const KKSRubricItem * item = r->item(i);
        appendItemRow(item, index);
    }
}

QModelIndex KKSIncludesWidget::appendItemRow(const KKSRubricItem * item, QModelIndex index)
{
    Q_UNUSED (index);
    if(!item)
        return QModelIndex();

    QAbstractItemModel * model = recWItems->getSourceModel();//twIncludes->model();
    if(!model)
        return QModelIndex();

    int cnt = model->rowCount();//index);
    
    model->insertRows(cnt, 1, QModelIndex());//, index);
//    if(cnt == 0){
//        if(model->columnCount(index) == 0)
//            model->insertColumns(0, 1, index);
//    }

    QModelIndex cIndex = model->index(cnt, 0);
    model->setData (cIndex, QVariant::fromValue<const KKSRubricBase *>(item), Qt::UserRole+1);
//    model->setData (model->index(cnt, 0, index), item->name(), Qt::DisplayRole);
//    model->setData (model->index(cnt, 0, index), 2, Qt::UserRole+2);//is item
//    model->setData (model->index(cnt, 0, index), item->id(), Qt::UserRole);// idObject
//    model->setData (model->index(cnt, 0, index), item->getIcon().isNull() ? QIcon(":/ddoc/rubric_item.png") : item->getIcon(), Qt::DecorationRole);
    
//    twIncludes->setExpanded(index, true);
    
    return cIndex;
}

QAbstractItemModel * KKSIncludesWidget::rubrItemsModel (void) const
{
    return recWItems->getSourceModel();
}

QModelIndex KKSIncludesWidget::appendRubricRow(const KKSRubric * r, QModelIndex pindex)
{
    if(!r)
        return QModelIndex();

    QAbstractItemModel * model = twIncludes->model();
    if(!model)
        return QModelIndex();

    int cnt = model->rowCount(pindex);
    int rpos = 0;
    if (cnt > 0)
        for (int i=0; i<cnt-1 && rpos==0; i++)
        {
            int pType = model->data(model->index(i, 0, pindex), Qt::UserRole+2).toInt();
            int cType = model->data(model->index(i+1, 0, pindex), Qt::UserRole+2).toInt();
            if (pType != cType )
            {
                rpos = i+1;
            }
        }

    if (cnt > 0 && rpos == 0)
        rpos = cnt;
    
    bool isInserted = model->insertRows(rpos, 1, pindex);
    if (!isInserted)
        return QModelIndex ();
    if(cnt == 0){
        if(model->columnCount(pindex) == 0)
            model->insertColumns(0, 1, pindex);
    }

//    model->setData (model->index(cnt, 0, index), r->name(), Qt::DisplayRole);
//    model->setData (model->index(cnt, 0, index), 1, Qt::UserRole);//is rubric
//    model->setData (model->index(cnt, 0, index), r->id(), Qt::UserRole+1);// idRubric
//    model->setData (model->index(cnt, 0, index), QIcon(":/ddoc/rubric.png"), Qt::DecorationRole);
    QModelIndex cIndex = model->index(rpos, 0, pindex);
    bool isRubrSet = model->setData (cIndex, QVariant::fromValue<const KKSRubricBase *>(r), Qt::UserRole+1);
    qDebug () << __PRETTY_FUNCTION__ << cIndex << model->rowCount (pindex) << isRubrSet;

    twIncludes->setExpanded(pindex, true);
    
    return cIndex;
}

/*
void KKSFileWidget::on_pbCancel_clicked()
{
    //reject();
}

void KKSFileWidget::on_pbOK_clicked()
{
    //accept();
}
*/

KKSRubric * KKSIncludesWidget::rootRubric() const
{
    return m_rootRubric;
}

void KKSIncludesWidget::delRubric (void)
{
    QItemSelectionModel * sm = twIncludes->selectionModel();
    QModelIndex index;
    QItemSelection sel = sm ? sm->selection() : QItemSelection();
    if(!sm || sel.indexes().isEmpty()){
        return;
    }
    else{
        index = sel.indexes().at(0);
        index = index.sibling(index.row(), 0);
        int rType = twIncludes->model()->data(index, Qt::UserRole+2).toInt();
        qDebug () << __PRETTY_FUNCTION__ << rType;
        if(rType == KKSRubricBase::atRubricItem ||
           rType == KKSRubricBase::atRubricCategory ||
           rType == KKSRubricBase::atOthers)
        { 
            //
            // item or category-rubric selected
            //
            QMessageBox::warning(this, tr("Delete rubric"), tr ("Cannot remove category rubric/item"), QMessageBox::Ok);
            return;
        }
        else{//rubric selected
            int res = QMessageBox::question (this, tr ("Delete rubric"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (res != QMessageBox::Yes)
                return;
        }
    }
    
    const KKSRubric * r = NULL;
    if(index.parent().isValid())
        r = getRubric(index.parent());
    else
        r = m_rootRubric;
    
    if(!r)
        return;
    
    const_cast<KKSRubric *>(r)->removeRubric(index.row());
    
    twIncludes->model()->removeRows(index.row(), 1, index.parent());
    isChanged = true;
    emit rubricsChanged ();
}

void KKSIncludesWidget :: addRubric (void)
{
    QItemSelectionModel * sm = twIncludes->selectionModel();
//    if (/*isMyDoc &&*/ !sm->currentIndex().isValid())
//    {
//        QMessageBox::warning (this, tr ("Rubric editor"), tr ("You cannot add rubrics into root level"), QMessageBox::Ok, QMessageBox::NoButton);
//        return;
//    }

    QModelIndex index;
    QItemSelection sel = sm ? sm->selection() : QItemSelection();

    if(!sm || sel.indexes().isEmpty()){
        index = QModelIndex();
    }
    else{
        index = sel.indexes().at (0);
        index = index.sibling(index.row(), 0);
        if(index.data(Qt::UserRole+2).toInt() == KKSRubricBase::atRubricItem){ //item selected
            index = index.parent();
            index = index.sibling(index.row(), 0);
        }
        else if (index.data (Qt::UserRole+2).toInt() == KKSRubricBase::atRubricCategory ||
                 index.data (Qt::UserRole+2).toInt() == KKSRubricBase::atOthers)
        {
            QMessageBox::warning (this,
                                  tr ("Add rubric"),
                                  tr ("Cannot add subrubric into categorized rubric or another"),
                                  QMessageBox::Ok);
            return;
        }
        else{//rubric selected
            ;
        }
    }

    bool ok = false;
    Q_UNUSED (ok);
    QString rName;
    RubricForm * rForm = new RubricForm (tr("New rubric %1").arg(twIncludes->model()->rowCount(index)+1), QString(), isRec, this);
    if (!rForm)
        return;

    emit loadStuffModel (rForm);
    connect (rForm, SIGNAL (requestSearchTemplate()), this, SLOT (addSearchTemplateIntoRubric()) );
    connect (rForm, SIGNAL (requestCategory()), this, SLOT (addCategoryIntoRubric()) );
    KKSSearchTemplate * rst = 0;
    KKSCategory * rc = 0;
    KKSAccessEntity * ac = 0;
    QString rDesc;
    QString icon;
    if (rForm && rForm->exec() == QDialog::Accepted)
    {
        rName = rForm->getRubricName ();
        rst = rForm->getSearchTemplate ();
        rc = rForm->getCategory ();
        ac = rForm->getAccessEntity ();
        rDesc = rForm->getRubricDesc ();
        icon = rForm->getIconAsString();
        if (rName.trimmed().isEmpty())
            return;
    }
    else
    {
        rForm->setParent (0);
        delete rForm;
        return;
    }

    KKSRubric * r = new KKSRubric(-1, rName, rst, rc, ac);
    r->setIcon(icon);
    r->setDesc (rDesc);
    rForm->setParent (0);
    delete rForm;

    const KKSRubric * pR = currentRubric();
    if(!pR){
        if(!m_rootRubric){
            m_rootRubric = new KKSRubric(-1, "root for object");
        }
        m_rootRubric->addRubric(r);
        r->setParent (m_rootRubric);
    }
    else
    {
        const_cast<KKSRubric *>(pR)->addRubric(r);
        r->setParent (const_cast<KKSRubric *>(pR));
    }

    QModelIndex cIndex = appendRubricRow(r, index);
    qDebug () << __PRETTY_FUNCTION__ << cIndex;
    
    twIncludes->setCurrentIndex(cIndex);
    twIncludes->model()->setData (cIndex, r->getIcon(), Qt::DecorationRole);

    r->release();
    isChanged = true;
    emit rubricsChanged ();
}

void KKSIncludesWidget :: copyFromRubric (void)
{
    QItemSelectionModel * sm = twIncludes->selectionModel();

    QItemSelection sel = sm ? sm->selection() : QItemSelection();
    QModelIndex index;
    if(!sm || sel.indexes().isEmpty()){
        index = QModelIndex();
    }
    else{
        index = sel.indexes().at(0);
        index = index.sibling(index.row(), 0);
        if(index.data(Qt::UserRole).toInt() == 2){ //item selected
            index = index.parent();
            index = index.sibling(index.row(), 0);
        }
        else{//rubric selected
            ;
        }
    }
    const KKSRubric * pR = currentRubric();
    if (!pR)
        return;
    emit copyFromRubr (const_cast<KKSRubric *>(pR), twIncludes->model(), index);
    parseRubric (const_cast<KKSRubric *>(pR), index);
    emit rubricsChanged ();
}

void KKSIncludesWidget :: editRubric (void)
{
    QItemSelectionModel * sm = twIncludes->selectionModel();
    QItemSelection sel = sm ? sm->selection() : QItemSelection();
    QModelIndex index;
    if(!sm || sel.indexes().isEmpty()){
        index = QModelIndex();
    }
    else{
        index = sel.indexes().at (0);
        index = index.sibling(index.row(), 0);
        int rType = index.data(Qt::UserRole+2).toInt();
        if(rType == KKSRubricBase::atRubricItem){ //item selected
            index = index.parent();
            index = index.sibling(index.row(), 0);
        }
        else if (rType == KKSRubricBase::atOthers ||
                 rType == KKSRubricBase::atRubricCategory)
        {
            QMessageBox::warning(this, tr("Edit rubric"), tr ("Category or others rubric cannot been edited"), QMessageBox::Ok);
            return;
        }
        else{//rubric selected
            ;
        }
    }

    bool ok = false;
    Q_UNUSED (ok);
    QString rName;

    KKSRubric * r = NULL;
    r = const_cast<KKSRubric *>(getRubric (index));
    if (!r)
        return;

    RubricForm * rForm = new RubricForm (index.data (Qt::DisplayRole).toString(), r->desc(), isRec, this);
    rForm->setSearchTemplate (r->getSearchTemplate());
    rForm->setCategory (r->getCategory());
    rForm->setAccessEntity (r->getAccessRules());
    rForm->setIcon (r->getIcon().pixmap(22, 22));
    if (!rForm)
        return;

    emit loadStuffModel (rForm);
    connect (rForm, SIGNAL (requestSearchTemplate()), this, SLOT (addSearchTemplateIntoRubric()) );
    connect (rForm, SIGNAL (requestCategory()), this, SLOT (addCategoryIntoRubric()) );
    KKSSearchTemplate * rst = 0;
    KKSCategory * rc = 0;
    KKSAccessEntity * ac =0;
    QString rDesc;
    QString rIcon;
    if (rForm && rForm->exec() == QDialog::Accepted)
    {
        rName = rForm->getRubricName ();
        rst = rForm->getSearchTemplate ();
        rc = rForm->getCategory ();
        ac = rForm->getAccessEntity();
        rDesc = rForm->getRubricDesc ();
        rIcon = rForm->getIconAsString();
        if (rName.trimmed().isEmpty())
            return;
    }
    else
    {
        rForm->setParent (0);
        delete rForm;
        return;
    }

    r->setName (rName);
    r->setSearchTemplate (rst);
    r->setCategory (rc);
    r->setAccessRules (ac);
    r->setDesc (rDesc);
    r->setIcon (rIcon);
    rForm->setParent (0);
    delete rForm;

    twIncludes->model()->setData (index, rName, Qt::DisplayRole);
    twIncludes->model()->setData (index, r->getIcon(), Qt::DecorationRole);
    isChanged = true;
    emit rubricsChanged ();
}
/*
void KKSIncludesWidget::on_pbRenameRubricator_clicked ()
{
    QItemSelectionModel * sm = twIncludes->selectionModel();
    if (!sm)
        return;
    QModelIndex index = sm->currentIndex ();

    if (!index.isValid() || index.parent().isValid())
        return;

    index = index.sibling (index.row(), 0);
    bool ok = false;
    QString rName = QInputDialog::getText(this, 
                                         tr("Rename rubricator"), 
                                         tr("Input new rubricator name"), 
                                         QLineEdit::Normal, 
                                         index.data (Qt::DisplayRole).toString(),
                                         &ok);
    if(!ok || rName.trimmed().isEmpty())
        return;

    KKSRubric * r = NULL;
    r = getRubric (index);
    if (!r)
        return;
    r->setName (rName);
    twIncludes->model()->setData (index, rName, Qt::DisplayRole);
    isChanged = true;
    emit rubricsChanged ();
}

void KKSIncludesWidget::on_pbAddRubricator_clicked ()
{
    bool ok = false;
    QString rName = QInputDialog::getText(this, 
                                         tr("Add new rubricator"), 
                                         tr("Input new rubricator name"), 
                                         QLineEdit::Normal, 
                                         tr("New rubricator %1").arg(twIncludes->model()->rowCount()+1),
                                         &ok);
    if(!ok || rName.trimmed().isEmpty())
        return;

    QModelIndex index = QModelIndex ();
    KKSRubric * r = new KKSRubric(-1, rName);

    m_rootRubric->addRubric(r);
    QModelIndex cIndex = appendRubricRow(r, index);
    
    twIncludes->setCurrentIndex(cIndex);

    r->release();
    isChanged = true;
    emit rubricsChanged ();
}

void KKSIncludesWidget::on_pbDelRubricator_clicked ()
{
    QItemSelectionModel * sm = twIncludes->selectionModel();
    QModelIndex index;
    if(!sm || !(index = sm->currentIndex()).isValid()){
        return;
    }
    else{
        index = index.sibling(index.row(), 0);
        if(index.data(Qt::UserRole).toInt() == 2){ //item selected
            return;
        }
        else{//rubric selected
            ;
        }
    }
    
    KKSRubric * r = NULL;
    if(index.parent().isValid())
        return;

    r = m_rootRubric;
    
    if(!r)
        return;
    
    r->removeRubric(index.row());
    
    twIncludes->model()->removeRow(index.row(), index.parent());
    isChanged = true;
    emit rubricsChanged ();
}
*/

void KKSIncludesWidget :: delSelectedDocs (QAbstractItemModel * itemModel, const QItemSelection& selDocs)
{
    if (!itemModel || selDocs.indexes().isEmpty())
        return;
    
    KKSRubricMessageBox * rW = new KKSRubricMessageBox ();
    if (rW->exec() != QDialog::Accepted)
    {
        delete rW;
        return;
    }
    Qt::CheckState dbState = rW->getDbState();
    bool isDbDel (dbState == Qt::Checked);
    QItemSelectionModel * smInc = twIncludes->selectionModel();
    for (int i=0; i<selDocs.indexes().count(); i++)
    {
        QModelIndex index = selDocs.indexes().at(i);
        index = index.sibling (index.row(), 0);
        int pType = index.data (Qt::UserRole+2).toInt();
        if ((isRec && pType != KKSRubricBase::atRubricItem))
            continue;
        QModelIndex wIndex (smInc->currentIndex());
        if (isRec)
            wIndex = wIndex.parent ();
        KKSRubric * r = NULL;
    //    if(isRubr)
    //        r = const_cast<KKSRubric *>(getRubric(wIndex.parent()));
    //    else
        r = const_cast<KKSRubric *>(getRubric(wIndex));

        if(!r)
            continue;

        r->removeItem (index.row());

    //    if (isRubr)
    //        twIncludes->model()->removeRow(wIndex.row(), wIndex.parent());
    //    else
        itemModel->removeRows (index.row(), 1, index.parent());
    }
    isChanged = true;
    emit rubricsChanged ();
    qDebug () << __PRETTY_FUNCTION__ << isDbDel;
    delete rW;
}

void KKSIncludesWidget :: delRubricItem (void)
{
    QItemSelectionModel * sm = tvItems->selectionModel ();

    QItemSelectionModel * smInc = twIncludes->selectionModel();
    
    QItemSelection sel = sm ? sm->selection() : QItemSelection();
    QItemSelection selInc = smInc ? smInc->selection() : QItemSelection();

    QModelIndex index;// = sm ? sm->currentIndex() : QModelIndex ();
    QModelIndex indexInc;
    if (((!sm || sel.indexes().isEmpty()) && !isRec) || ((!smInc || selInc.indexes().isEmpty()) && isRec))
    {
        return;
    }
    else if (isRec && smInc && !selInc.indexes().isEmpty())
    {
        index = selInc.indexes().at (0);
        index = index.sibling(index.row(), 0);
        int pType = index.data (Qt::UserRole+2).toInt();
        if (pType == KKSRubricBase::atRubricItem)
        {
            int res = QMessageBox::question (this, tr ("Delete rubric item"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (res != QMessageBox::Yes)
                return;
        }
        else
            return;
    }
    else
    {
        index = sel.indexes().at (0);
        index = index.sibling(index.row(), 0);
        QModelIndex pIndex = selInc.indexes().at(0);
        pIndex = pIndex.sibling (pIndex.row(), 0);
        int pType = pIndex.data(Qt::UserRole+2).toInt();
        if ( pType == KKSRubricBase::atRubric || pType == KKSRubricBase::atRootRubric)
                //(index.data(Qt::UserRole+2).toInt() == KKSRubricBase::atRubricItem && 
                 //pType != KKSRubricBase::atRubricCategory)
                //)
        {
            //
            // item selected
            //
            int res = QMessageBox::question (this, tr ("Delete rubric item"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (res != QMessageBox::Yes)
                return;
        }
        else
        {
            //
            // rubric selected
            //
            if (pType == KKSRubricBase::atRubricCategory)
                QMessageBox::warning (this, tr ("Delete rubric item"), tr ("Cannot delete item from categorized rubric."), QMessageBox::Ok);
            return;
        }
    }

    QModelIndex wIndex = smInc->currentIndex ();
    if (isRec)
        wIndex = wIndex.parent ();
    KKSRubric * r = NULL;
//    if(isRubr)
//        r = const_cast<KKSRubric *>(getRubric(wIndex.parent()));
//    else
    r = const_cast<KKSRubric *>(getRubric(wIndex));

    if(!r)
        return;
    
    r->removeItem (index.row());

//    if (isRubr)
//        twIncludes->model()->removeRow(wIndex.row(), wIndex.parent());
//    else
    if (isRec)
        twIncludes->model()->removeRows (index.row(), 1, index.parent());
    else
        tvItems->model ()->removeRow (index.row(), index.parent());
    isChanged = true;
    emit rubricsChanged ();
}

void KKSIncludesWidget :: addRubricItem (void)
{
    QItemSelectionModel * sm (twIncludes->selectionModel());

//    sm = twIncludes->selectionModel();
    QItemSelection sel = sm ? sm->selection() : QItemSelection();

    QModelIndex index = sel.indexes().isEmpty() ? QModelIndex() : sel.indexes().at(0);
    if (!sm || !index.isValid())
    {
        QMessageBox::warning(this, 
                             tr("Warning"), 
                             tr("You should select rubric to add item"), 
                             QMessageBox::Ok);
        return;
    }
    else if (index.data (Qt::UserRole+2).toInt() == KKSRubricBase::atRubricCategory ||
             index.data (Qt::UserRole+2).toInt() == KKSRubricBase::atOthers)
    {
        QMessageBox::warning (this,
                              tr ("Add rubric item"),
                              tr ("Cannot add document into categorized rubric or another"),
                              QMessageBox::Ok);
        return;
    }
    
    const KKSRubric * r = currentRubric();
    if(!r)
        return;

    emit rubricItemRequested(isRec);
}

void KKSIncludesWidget :: createRubricItem (QAbstractItemModel * itemModel, const QModelIndex& parent)
{
    qDebug () << __PRETTY_FUNCTION__ << itemModel << parent;
    QItemSelectionModel * sm (twIncludes->selectionModel());

//    sm = twIncludes->selectionModel();
    QItemSelection sel = sm ? sm->selection() : QItemSelection();

    QModelIndex index = sel.indexes().isEmpty() ? QModelIndex() : sel.indexes().at(0);
    if (!sm || !index.isValid())
    {
        QMessageBox::warning(this, 
                             tr("Warning"), 
                             tr("You should select rubric to add item"), 
                             QMessageBox::Ok);
        return;
    }
    
    const KKSRubric * r = currentRubric();
    if (!r)
        return;

    if (!r->getCategory())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Creation of new IO allowed only for rubrics with accigned category!"), QMessageBox::Ok);
        return;
    }

    emit rubricItemCreationRequested(r, itemModel, parent);
}

void KKSIncludesWidget::slotAddRubricItem(int idObject, QString name)
{

    if(idObject <= 0){
        QMessageBox::critical(this, 
                              tr("Error"), 
                              tr("You should select object to add to rubric!"), 
                              QMessageBox::Ok);
        return;
    }

    QItemSelectionModel * sm = twIncludes->selectionModel();
    QModelIndex index;
    QItemSelection sel = sm ? sm->selection() : QItemSelection();
    if(!sm || sel.indexes().isEmpty())
    {
        QMessageBox::warning(this, 
                             tr("Warning"), 
                             tr("You should select rubric to add item"), 
                             QMessageBox::Ok);
        return;
    }
    index = sel.indexes().at(0);
    
    KKSRubric * r = const_cast<KKSRubric *>(currentRubric());
    if (!r)
        return;
    
    if (isRec)
    {
        emit appendRubricRecord (idObject, r, twIncludes->model(), index);
        isChanged = true;
        emit rubricsChanged ();
        return;
    }

    const KKSRubricItem * equalItem = r->itemForId(idObject);
    if(equalItem){
        QMessageBox::critical(this, 
                             tr("Error"), 
                             tr("You cannot add one item to rubric twise"), 
                             QMessageBox::Ok);
        return;
    }

    KKSRubricItem * item = new KKSRubricItem(idObject, name, false);

//    bool rIsEmpty = r->items().isEmpty();

    r->addItem(item);

    //
    // rubric item is selected
    //
    while (index.data(Qt::UserRole+2) == KKSRubricBase::atRubricItem)
        index = index.parent();

//    if (r->getCategory())
//    {
//        qDebug () << __PRETTY_FUNCTION__ << r->name() << rIsEmpty;
    QAbstractItemModel * attachModel = isRec ? twIncludes->model() : recWItems->getSourceModel ();
    if (!attachModel && !this->isRec)
        emit initAttachmentsModel (r);
    else if (isRec)
    {
        qDebug () << __PRETTY_FUNCTION__ ;
    }
    else
        emit appendRubricItemIntoModel (attachModel, item);
//            QSortFilterProxyModel * sortModel = new KKSSortFilterProxyModel();
//            attachModel = new QStandardItemModel (0, 0);
//            sortModel->setSourceModel (attachModel);
//            recWItems->setEIOModel (sortModel);
//        }
        //emit rubricAttachmentsView (attachModel, r);
        //return;
//    }
    //QModelIndex cIndex = appendItemRow(item, index);
    //twIncludes->setCurrentIndex(cIndex);
    
    item->release();
    isChanged = true;
    emit rubricsChanged ();
}

void KKSIncludesWidget :: slotInitAttachmentsModel (QAbstractItemModel * attachModel)
{
    if (!attachModel)
        return;
    
    if (qobject_cast<QAbstractProxyModel*>(recWItems->getModel()))
    {
        QAbstractProxyModel * proxyModel = qobject_cast<QAbstractProxyModel*>(recWItems->getModel());
        proxyModel->setSourceModel (attachModel);
    }
    else
    {
        QSortFilterProxyModel * sortModel = new KKSSortFilterProxyModel();
        sortModel->setSourceModel (attachModel);
        recWItems->setEIOModel (sortModel);
    }
    recWItems->setVisible (true);
    const KKSTemplate * t = attachModel->data(attachModel->index (0, 0), Qt::UserRole+2).value<const KKSTemplate *>();
    if (!t)
        return;
    QHeaderView * hItems = recWItems->getView()->header();
    for (int i=0; i<attachModel->columnCount(); i++)
    {
        QSize hSize = attachModel->headerData(i, Qt::Horizontal, Qt::SizeHintRole).toSize();
        hItems->resizeSection(i, hSize.width());
    }
}

void KKSIncludesWidget :: editRubricItem (void)
{
    QItemSelectionModel * sm;

    bool isRubr = true;
    if (recWItems->isVisible ())
    {
        sm = tvItems->selectionModel ();
        isRubr = false;
    }
    else
        sm = twIncludes->selectionModel();

    QItemSelection sel = sm ? sm->selection() : QItemSelection();
    QModelIndex index = sel.indexes().isEmpty() ? QModelIndex() : sel.indexes().at (0);
    if(!sm || !index.isValid())
    {
        return;
    }
    else{
        index = index.sibling(index.row(), 0);
        int rType = index.data(Qt::UserRole+2).toInt();
        if (!isRubr || rType == KKSRubricBase::atRubricItem){ //item selected
            ;
        }
        else{//rubric selected
            return;
        }
    }
    if (isRec)
    {
        emit openRubricItemRequested (index.data (Qt::UserRole).toInt());
        return;
    }
    editRubricDoc (tvItems->model(), index);

}

void KKSIncludesWidget :: editSelectedDocs (QAbstractItemModel * itemModel, const QItemSelection& selDocs)
{
    QModelIndexList sInds = selDocs.indexes();
    for (int i=0; i<sInds.count(); i++)
    {
        if (sInds[i].column() == 0)
            editRubricDoc (itemModel, sInds[i]);
    }
}

void KKSIncludesWidget :: editRubricDoc (QAbstractItemModel * itemModel, const QModelIndex& index)
{
    if (!itemModel)
        return;
    int idObject = -1;

    idObject = index.data(Qt::UserRole).toInt();//idObject
    
    if(idObject > 0){
        emit openRubricItemRequested(idObject);
        return;
    }

    QMessageBox::critical(this,
                          tr("Error"),
                          tr("Cannot open rubric item. Data is corrupt!"),
                          QMessageBox::Ok);

}

void KKSIncludesWidget::slotRubricItemDblClicked(const QModelIndex & index)
{
    qDebug () << __PRETTY_FUNCTION__ << index.data(Qt::UserRole+2).toInt();
    if (index.data(Qt::UserRole+2).toInt() == KKSRubricBase::atRubricItem)
        editRubricItem();
    else
        editRubric();
}

void KKSIncludesWidget::slotRubricItemEdit(const QModelIndex & /*index*/)
{
    editRubricItem();
}
/*
void KKSIncludesWidget::on_cbUseIncludes_stateChanged(int state)
{
    if(state == Qt::Checked){
        ui->gbIncludes->setEnabled(true);
    }
    else
        ui->gbIncludes->setEnabled(false);
    emit rubricsChanged ();
}
*/

const KKSRubric * KKSIncludesWidget::getRubric(QModelIndex index)
{
    if (!m_rootRubric)
        return 0;

    QAbstractItemModel * model = twIncludes->model();
    
    const KKSRubricBase * wr = model->data (index, Qt::UserRole+1).value<const KKSRubricBase *>();
    if (!wr)
        return 0;
    if (wr->rubricType() == KKSRubricBase::atRubricItem)
    {
        KKSRubricBase * wrp = index.parent().data (Qt::UserRole+1).value<KKSRubricBase *>();
        if (!wrp)
            return 0;
        return static_cast<const KKSRubric *>(wrp);
    }
    else if (wr->rubricType() != KKSRubricBase::atOthers)
        return static_cast<const KKSRubric *>(wr);
    else
        return 0;
/*    if (index.parent().isValid())
    {
        KKSRubric * r = getRubric(index.parent());
        if(index.data(Qt::UserRole+2).toInt() == 2)//item
            return r;
        
        QAbstractItemModel * model = twIncludes->model();
        int rowCount = model->rowCount(index.parent());
        int row = 0;
        int currentRow = index.row();
        for(int i=0; i<rowCount; i++){
            QModelIndex ind = model->index(i, 0, index.parent());
            if(ind.data(Qt::UserRole+2).toInt() == 1)//rubric
                row++;
            if(ind.row() == currentRow){
                row--;
                break;
            }
        }
        
        return const_cast<KKSRubric*>(r->rubric(row));
    }
    
    return const_cast<KKSRubric*>(m_rootRubric->rubric(index.row()));
 */
}


const KKSRubric * KKSIncludesWidget::currentRubric()
{

    QItemSelectionModel * sm = twIncludes->selectionModel();
    QItemSelection sel = sm ? sm->selection() : QItemSelection();
    QModelIndex index;
    if(!sm || sel.indexes().isEmpty()){
        return NULL;
    }

    index = sel.indexes().at (0);
    return getRubric(index);
}

void KKSIncludesWidget::hideIncludes (void)
{
//    ui->cbUseIncludes->setVisible (false);
}

void KKSIncludesWidget::showIncludes (void)
{
//    ui->cbUseIncludes->setVisible (true);
}

QTreeView *KKSIncludesWidget::tvRubr (void)
{
    return twIncludes;
}

void KKSIncludesWidget::save (void)
{
    emit saveRubric (m_rootRubric, isMyDoc);
}

void KKSIncludesWidget::closeEvent (QCloseEvent * event)
{
    if (isChanged)
    {
        QMessageBox::StandardButton res = QMessageBox::question (this, tr ("Rubric editor"), tr("Rubricator was changed.\n Do you want to save ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);
        if (res == QMessageBox::Cancel)
        {
            event->ignore ();
            return;
        }
        else if (res == QMessageBox::Yes)
            save ();
        
        emit aboutToClose();
        
        event->accept ();
    }
    else{
        emit aboutToClose();
        QWidget::closeEvent (event);
    }
}

void KKSIncludesWidget::setSaved (bool isSaved)
{
    isChanged = !isSaved;
}

void KKSIncludesWidget::addSearchTemplateIntoRubric (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    RubricForm * rForm = qobject_cast<RubricForm *>(this->sender());
    if (!rForm)
        return;

    qDebug () << __PRETTY_FUNCTION__;
    emit loadSearchtemplate (rForm);
}

void KKSIncludesWidget::addCategoryIntoRubric (void)
{
    RubricForm * rForm = qobject_cast<RubricForm *>(this->sender());
    if (!rForm)
        return;

    qDebug () << __PRETTY_FUNCTION__;
    emit loadCategory (rForm);
}

void KKSIncludesWidget::rubricSelectionChanged (const QItemSelection& selected, const QItemSelection& deselected)
{
    qDebug () << __PRETTY_FUNCTION__ << selected << deselected;
    QModelIndexList wIndexList (selected.indexes());
    QModelIndexList oldIndexList (deselected.indexes());

    if (rubricsOnly || isRec || (wIndexList.isEmpty() && oldIndexList.isEmpty()))
    {
        recWItems->setVisible (false);
        return;
    }
    else if (wIndexList.isEmpty())
        return;

    QModelIndex wIndex = wIndexList[0];
    int rType = wIndex.data (Qt::UserRole+2).toInt();
    int idr = wIndex.data (Qt::UserRole).toInt();
    if (!wIndex.isValid() || idr == 1 || rType == KKSRubricBase::atRootRubric 
                                      || rType == KKSRubricBase::atOthers)
    {
        recWItems->setVisible (false);
        return;
    }
    
    const KKSRubric * rubr = getRubric (wIndex);
    if (rubr)
    {
        emit initAttachmentsModel (rubr);
        recWItems->setVisible (true);
    }
/*
    if (rubr && rubr->getCategory())
    {
        recWItems->setVisible (true);
        QAbstractItemModel * attachModel = recWItems->getSourceModel ();
        if (!attachModel)
        {
            QSortFilterProxyModel * sortModel = new KKSSortFilterProxyModel();
            attachModel = new QStandardItemModel (0, 0);
            sortModel->setSourceModel (attachModel);
            recWItems->setEIOModel (sortModel);
        }
        emit rubricAttachmentsView (attachModel, rubr);
    }
    else
        recWItems->setVisible (false);
 */
}

void KKSIncludesWidget :: turnRubricSplitter (void)
{
    Qt::Orientation rorient = this->spRubrics->orientation();
    /*if (rorient == Qt::Horizontal)
        spRubrics->setOrientation (Qt::Vertical);
    else
        spRubrics->setOrientation (Qt::Horizontal);*/
    this->spRubrics->setOrientation ((Qt::Orientation)(0x3-rorient));
}

void KKSIncludesWidget :: contextMenuEvent (QContextMenuEvent * event)
{
    if (this->childAt(event->pos()) == this->twIncludes->viewport())
    {
        pMenu->exec (event->globalPos());
        event->accept();
    }
    else
        QWidget::contextMenuEvent (event);

}

void KKSIncludesWidget :: setRubricIcon (void)
{
    QItemSelectionModel * sm = twIncludes->selectionModel();
    QItemSelection sel = sm ? sm->selection() : QItemSelection();
    QModelIndex index;
    if(!sm || sel.indexes().isEmpty()){
        index = QModelIndex();
    }
    else{
        index = sel.indexes().at (0);
        index = index.sibling(index.row(), 0);
        int rType = index.data(Qt::UserRole+2).toInt();
        if(rType == KKSRubricBase::atRubricItem){ //item selected
            index = index.parent();
            index = index.sibling(index.row(), 0);
        }
        else if (rType == KKSRubricBase::atOthers ||
                 rType == KKSRubricBase::atRubricCategory)
        {
            QMessageBox::warning(this, tr("Edit rubric"), tr ("Category or others rubric cannot been edited"), QMessageBox::Ok);
            return;
        }
        else{//rubric selected
            ;
        }
    }
    QString iconFile = QFileDialog::getOpenFileName(this, tr("Open Image File"),
                                                    QDir::currentPath(),
                                                    tr("Image files (*.xpm *.png *.ico *.jpg *.jpeg *.bmp *.gif *.pbm *.pgm *.xbm);;All files (*)")
                                                    );
    if (iconFile.isEmpty())
        return;

    QPixmap rubrPixmap (iconFile);
    if(rubrPixmap.isNull()){
        QMessageBox::critical(this, 
                              tr("Error"), 
                              tr(""), 
                              QMessageBox::Ok);
        return;
    }

    QByteArray bytes;
    QBuffer buffer(&bytes);

    buffer.open(QIODevice::WriteOnly);
    rubrPixmap.save(&buffer, "XPM"); // writes pixmap into bytes in XPM format
    buffer.close();
    //qDebug () << __PRETTY_FUNCTION__ << rubrPixmap.size();

    const KKSRubric * rubr (0);
    QIcon icon;
    if (index.data(Qt::UserRole+2).toInt() == 2)
    {
        rubr = getRubric (index.parent());
        if (!rubr)
            return;
        KKSRubricItem * rubrItem = const_cast<KKSRubricItem *>(rubr->item(index.row()));
        rubrItem->setIcon (QString(bytes));
        icon = rubrItem->getIcon();
    }
    else
    {
        rubr = getRubric (index);
        if (!rubr)
            return;

        ((KKSRubricBase*)rubr)->setIcon (QString(bytes));
        icon = rubr->getIcon();
    }

    QAbstractItemModel * mod = twIncludes->model();
    //qDebug () << __PRETTY_FUNCTION__ << icon.availableSizes();
    mod->setData (index, icon, Qt::DecorationRole);
    isChanged = true;

}

QAbstractItemModel * KKSIncludesWidget :: rubrModel () const
{
    return twIncludes->model();
}

void KKSIncludesWidget :: setRubrModel (QAbstractItemModel * rModel)
{
    QAbstractItemModel * oldModel = twIncludes->model();
    twIncludes->setModel(rModel);
    if (oldModel && oldModel != rModel)
        delete oldModel;
}

void KKSIncludesWidget :: addRubricIntoModel (KKSRubric * cRubr, const QModelIndex& pIndex)
{
    if (!cRubr)
        return;

    m_rootRubric->addRubric (cRubr);
    parseRubric (cRubr, pIndex);
/*    QAbstractItemModel * mod = rubrModel ();
    int nr = rubrMod->rowCount();
    rubrMod->insertRows (nr, 1);
    QModelIndex wIndex = rubrMod->index (nr, 0);
    rubrMod->setData (wIndex, tr("Others"), Qt::DisplayRole);
    rubrMod->setData (wIndex, atOthers, Qt::UserRole);
    rubrMod->setData (wIndex, KKSRubric::icon().scaled(16, 16), Qt::DecorationRole);*/
}

void KKSIncludesWidget :: expandAllIndexes (const QModelIndex& parent) const
{
    twIncludes->expand (parent);
    QAbstractItemModel * mod = twIncludes->model();
    for (int i=0; i<mod->rowCount(parent); i++)
    {
        QModelIndex ind = mod->index (i, 0, parent);
        expandAllIndexes (ind);
    }
}

QList<int> KKSIncludesWidget :: getSelectedIOS (void) const
{
    QItemSelection sel = recWItems->getSourceSelection();
    QList<int> idIOList;
    for (int i=0; i<sel.indexes().count(); i++)
    {
        QModelIndex wIndex = sel.indexes().at(i);
        wIndex = wIndex.sibling (wIndex.row(), 0);
        int id = wIndex.data (Qt::UserRole).toInt();
        if (id > 0 && !idIOList.contains (id))
            idIOList.append (id);
    }
    qDebug () << __PRETTY_FUNCTION__ << idIOList;
    return idIOList;
}

void KKSIncludesWidget :: setSyncSettings (void)
{
    QList<int> selectedIO = getSelectedIOS ();
    if (selectedIO.isEmpty())
    {
        QMessageBox::warning (this, tr ("Set synchronization parameters"), tr("Select documents for set"), QMessageBox::Ok);
        return;
    }
    emit setSyncIO (selectedIO);
}

void KKSIncludesWidget :: putIntoAnotherRubric (void)
{
    QList<int> selectedIO = getSelectedIOS ();
    if (selectedIO.isEmpty())
    {
        QMessageBox::warning (this, tr ("Put selected documents"), tr("Select documents for copy"), QMessageBox::Ok);
        return;
    }
    const KKSRubric * r = currentRubric();
    emit putIOSIntoRubr (selectedIO, r);
}

void KKSIncludesWidget :: sendIOS (void)
{
    QList<int> selectedIO = getSelectedIOS ();
    if (selectedIO.isEmpty())
    {
        QMessageBox::warning (this, tr ("Send selected documents"), tr("Select documents for send"), QMessageBox::Ok);
        return;
    }
    emit sendIOS (selectedIO);
}

void KKSIncludesWidget :: setAccessRules (void)
{
    QList<int> selectedIO = getSelectedIOS ();
    if (selectedIO.isEmpty())
    {
        QMessageBox::warning (this, tr ("Set access rules"), tr("Select documents for set"), QMessageBox::Ok);
        return;
    }
    emit setAccessIOS (selectedIO);
}

const KKSRubric * KKSIncludesWidget :: getSelectedRubric (void)
{
    return this->currentRubric();
}

bool KKSIncludesWidget :: isRubrics (void) const
{
    return rubricsOnly;
}

void KKSIncludesWidget :: setForRubrics (bool isr)
{
    rubricsOnly = isr;
    if (isr)
    {
        recWItems->setVisible (false);
        tBRubrActions->setVisible (false);
    }
}
/*=================*/
KKSIncludesItemDelegate::KKSIncludesItemDelegate(QObject * parent) : QItemDelegate(parent)
{

}

KKSIncludesItemDelegate::~KKSIncludesItemDelegate ()
{

}
/*
void KKSIncludesItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    painter->save();

    QPixmap px;
    QRect px_rect;
    QRect bound = option.rect;

    int shift = 0;
    if(index.data(Qt::UserRole).toInt() == 1){//rubric
        px = KKSRubric::icon();
        if(!px.isNull())
            shift = 21;
    }
    else{
        px = KKSRubricItem::icon();
        if(!px.isNull())
            shift = 19;
    }

    px_rect = QRect(option.rect.left()+3, option.rect.top()+option.rect.height()/2-px.height()/2, 16+3, 16);
        
    bound = QRect(option.rect.left()+5+shift, option.rect.top(), option.rect.width()-5-shift, option.rect.height());

    if (option.state & QStyle::State_Selected){
        painter->setBrush(option.palette.highlight());
        painter->drawRect(option.rect);
        painter->setPen(option.palette.highlightedText().color());

    }
    else{
        painter->setBrush(Qt::NoBrush);
        painter->save();
        painter->setPen(Qt::NoPen);
        painter->drawRect(option.rect);
        painter->restore();
    }

    if(!px.isNull())
        painter->drawPixmap(px_rect, px);

    QString text = index.data(Qt::DisplayRole).toString();
    int flags = Qt::AlignVCenter;
    painter->drawText(bound, flags, text);

    painter->restore();
}

QSize KKSIncludesItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QSize ws = option.rect.size ();
    QString wstr = index.data (Qt::DisplayRole).toString ();

    QPixmap px;
    QRect px_rect;
    int height = 0;

    if (index.data(Qt::UserRole).toInt() == 1)
    {
        //
        // rubric
        //
        px = KKSRubric::icon();
    }
    else
    {
        //
        // document
        //
        px = KKSRubricItem::icon();
    }
    height = px.height ();
    QStringList wstrL = wstr.split ("\n");
    QFont fn = option.font;
    QFontMetrics fm (fn);
    QString str_max = wstrL[0];
    for (int i=1; i<wstrL.size(); i++)
        if (wstrL[i].size () > str_max.size())
            str_max = wstrL[i];
    int width = px.width() + fm.width (str_max);
    ws.rwidth () += width;
    ws.rheight () += wstrL.size()*qMax (height, fm.height());

    return ws;
}
*/