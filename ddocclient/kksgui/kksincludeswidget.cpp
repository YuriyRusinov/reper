#include "kksincludeswidget.h"
#include "KKSRecWidget.h"
#include "KKSItemDelegate.h"
#include "KKSEventFilter.h"
#include "KKSSortFilterProxyModel.h"

#include "KKSRubric.h"
#include <KKSCategory.h>
#include <defines.h>
#include "ui/rubricform.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QTreeView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
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

KKSIncludesWidget::KKSIncludesWidget(KKSRubric * rootRubric, 
                                     bool isAttach,
                                     bool isDocs,
                                     bool forCategory,
                                     QWidget *parent,
                                     Qt::WindowFlags flags)
    : QWidget (parent, flags),
    m_rootRubric (rootRubric),
    isMyDoc (isDocs),
    isChanged (false),
    tBRubrActions (new QToolBar (this)),
    spRubrics (new QSplitter (Qt::Vertical, this)),
    twIncludes (new QTreeView (spRubrics)),
    tvItems (new QTreeView ()),
    recWItems (new KKSRecWidget (tvItems, false, spRubrics))
{
    //Q_INIT_RESOURCE (kksgui_icon_set);

    if (m_rootRubric)
        m_rootRubric->addRef();

    tBRubrActions->setIconSize (QSize (24, 24));
    QHeaderView * hItems = tvItems->header();
    hItems->setSortIndicatorShown (true);
    hItems->setStretchLastSection (true);
    hItems->setClickable (true);
    KKSItemDelegate * iDeleg = new KKSItemDelegate ();
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
    connect (recWItems->actAdd, SIGNAL (triggered()), this, SLOT (addRubricItem()) );
    connect (recWItems->actEdit, SIGNAL (triggered()), this, SLOT (editRubricItem()) );
    connect (recWItems->getView(), SIGNAL (doubleClicked(const QModelIndex &)), this, SLOT (slotRubricItemDblClicked(const QModelIndex &)) );
    connect (recWItems->actDel, SIGNAL (triggered()), this, SLOT (delRubricItem()) );
}

KKSIncludesWidget::~KKSIncludesWidget()
{
    if(m_rootRubric)
        m_rootRubric->release();
}

void KKSIncludesWidget::init()
{
    QStandardItemModel * model = new QStandardItemModel (0, 1);
    model->setHeaderData(0, Qt::Horizontal, tr("Name"), Qt::DisplayRole);

    QItemDelegate * includesDelegate = new KKSItemDelegate ();//this);

    twIncludes->setItemDelegate (includesDelegate);
    twIncludes->header()->resizeSection(0, 550);
    twIncludes->setModel (model);

    initTwIncludes();
}

void KKSIncludesWidget :: initActions (void)
{
    QStringList icons;
    icons << ":/ddoc/rubric_add.png" 
          << ":/ddoc/rubric_add.png"
          << ":/ddoc/rubric_edit.png" 
          << ":/ddoc/rubric_delete.png"
          << ":/ddoc/rubric_item_add.png" 
          << ":/ddoc/rubric_item_open.png" 
          << ":/ddoc/rubric_item_remove.png";

    QStringList slotList;
    slotList << SLOT (addRubric())
             << SLOT (copyFromRubric())
             << SLOT (editRubric()) 
             << SLOT (delRubric()) 
             << SLOT (addRubricItem()) 
             << SLOT (editRubricItem()) 
             << SLOT (delRubricItem());

    QStringList titlesList;
    titlesList << tr ("&Add new rubric")
               << tr ("&Copy from rubric")
               << tr ("&Edit rubric") 
               << tr ("&Delete rubric") 
               << tr ("Add rubric item") 
               << tr ("&Open rubric item") 
               << tr ("&Delete rubric item");

    for (int i=0; i<=6; i++)
    {
        QAction * aRubricA = new QAction (QIcon(icons[i]), titlesList[i], this);
        tBRubrActions->addAction (aRubricA);
        connect (aRubricA, SIGNAL (triggered()), this, slotList[i].toAscii().constData() );
        if (i==3)
            tBRubrActions->addSeparator ();
    }
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
        QAbstractItemModel * model = twIncludes->model();
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
    if(!item)
        return QModelIndex();

    QAbstractItemModel * model = twIncludes->model();
    if(!model)
        return QModelIndex();

    int cnt = model->rowCount(index);
    
    model->insertRow(cnt, index);
    if(cnt == 0){
        if(model->columnCount(index) == 0)
            model->insertColumns(0, 1, index);
    }

    model->setData (model->index(cnt, 0, index), item->name(), Qt::DisplayRole);
    model->setData (model->index(cnt, 0, index), 2, Qt::UserRole);//is item
    model->setData (model->index(cnt, 0, index), item->id(), Qt::UserRole+1);// idObject
    model->setData (model->index(cnt, 0, index), QIcon(":/ddoc/rubric_item.png"), Qt::DecorationRole);
    
    QModelIndex cIndex = model->index(cnt, 0, index);
    twIncludes->setExpanded(index, true);
    
    return cIndex;
}

QModelIndex KKSIncludesWidget::appendRubricRow(const KKSRubric * r, QModelIndex index)
{
    if(!r)
        return QModelIndex();

    QAbstractItemModel * model = twIncludes->model();
    if(!model)
        return QModelIndex();

    int cnt = model->rowCount(index);
    
    bool isInserted = model->insertRows(cnt, 1, index);
    if (!isInserted)
        return QModelIndex ();
    if(cnt == 0){
        if(model->columnCount(index) == 0)
            model->insertColumns(0, 1, index);
    }

    model->setData (model->index(cnt, 0, index), r->name(), Qt::DisplayRole);
    model->setData (model->index(cnt, 0, index), 1, Qt::UserRole);//is rubric
    model->setData (model->index(cnt, 0, index), r->id(), Qt::UserRole+1);// idRubric
    model->setData (model->index(cnt, 0, index), QIcon(":/ddoc/rubric.png"), Qt::DecorationRole);

    QModelIndex cIndex = model->index(cnt, 0, index);
    twIncludes->setExpanded(index, true);
    
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
    if(!sm || !(index = sm->currentIndex()).isValid()){
        return;
    }
    else{
        index = index.sibling(index.row(), 0);
        if(index.data(Qt::UserRole).toInt() == 2){ //item selected
            return;
        }
        else{//rubric selected
            int res = QMessageBox::question (this, tr ("Delete rubric"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (res != QMessageBox::Yes)
                return;
        }
    }
    
    KKSRubric * r = NULL;
    if(index.parent().isValid())
        r = getRubric(index.parent());
    else
        r = m_rootRubric;
    
    if(!r)
        return;
    
    r->removeRubric(index.row());
    
    twIncludes->model()->removeRow(index.row(), index.parent());
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
    if(!sm || !(index = sm->currentIndex()).isValid()){
        index = QModelIndex();
    }
    else{
        index = index.sibling(index.row(), 0);
        if(index.data(Qt::UserRole).toInt() == 2){ //item selected
            index = index.parent();
            index = index.sibling(index.row(), 0);
        }
        else{//rubric selected
            ;
        }
    }

    bool ok = false;
    Q_UNUSED (ok);
    QString rName;
    RubricForm * rForm = new RubricForm (tr("New rubric %1").arg(twIncludes->model()->rowCount(index)+1), this);
    if (!rForm)
        return;

    emit loadStuffModel (rForm);
    connect (rForm, SIGNAL (requestSearchTemplate()), this, SLOT (addSearchTemplateIntoRubric()) );
    connect (rForm, SIGNAL (requestCategory()), this, SLOT (addCategoryIntoRubric()) );
    KKSSearchTemplate * rst = 0;
    KKSCategory * rc = 0;
    KKSAccessEntity * ac = 0;
    if (rForm && rForm->exec() == QDialog::Accepted)
    {
        rName = rForm->getRubricName ();
        rst = rForm->getSearchTemplate ();
        rc = rForm->getCategory ();
        ac = rForm->getAccessEntity ();
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
    rForm->setParent (0);
    delete rForm;

    KKSRubric * pR = currentRubric();
    if(!pR){
        if(!m_rootRubric){
            m_rootRubric = new KKSRubric(-1, "root for object");
        }
        m_rootRubric->addRubric(r);
    }
    else
        pR->addRubric(r);

    QModelIndex cIndex = appendRubricRow(r, index);
    
    twIncludes->setCurrentIndex(cIndex);

    r->release();
    isChanged = true;
    emit rubricsChanged ();
}

void KKSIncludesWidget :: copyFromRubric (void)
{
    QItemSelectionModel * sm = twIncludes->selectionModel();

    QModelIndex index;
    if(!sm || !(index = sm->currentIndex()).isValid()){
        index = QModelIndex();
    }
    else{
        index = index.sibling(index.row(), 0);
        if(index.data(Qt::UserRole).toInt() == 2){ //item selected
            index = index.parent();
            index = index.sibling(index.row(), 0);
        }
        else{//rubric selected
            ;
        }
    }
    KKSRubric * pR = currentRubric();
    if (!pR)
        return;
    emit copyFromRubr (pR, twIncludes->model(), index);
    parseRubric (pR, index);
    emit rubricsChanged ();
}

void KKSIncludesWidget :: editRubric (void)
{
    QItemSelectionModel * sm = twIncludes->selectionModel();
    QModelIndex index;
    if(!sm || !(index = sm->currentIndex()).isValid()){
        index = QModelIndex();
    }
    else{
        index = index.sibling(index.row(), 0);
        if(index.data(Qt::UserRole).toInt() == 2){ //item selected
            index = index.parent();
            index = index.sibling(index.row(), 0);
        }
        else{//rubric selected
            ;
        }
    }

    bool ok = false;
    Q_UNUSED (ok);
    QString rName;

    KKSRubric * r = NULL;
    r = getRubric (index);
    if (!r)
        return;

    RubricForm * rForm = new RubricForm (index.data (Qt::DisplayRole).toString(), this);
    rForm->setSearchTemplate (r->getSearchTemplate());
    rForm->setCategory (r->getCategory());
    rForm->setAccessEntity (r->getAccessRules());
    if (!rForm)
        return;

    emit loadStuffModel (rForm);
    connect (rForm, SIGNAL (requestSearchTemplate()), this, SLOT (addSearchTemplateIntoRubric()) );
    connect (rForm, SIGNAL (requestCategory()), this, SLOT (addCategoryIntoRubric()) );
    KKSSearchTemplate * rst = 0;
    KKSCategory * rc = 0;
    KKSAccessEntity * ac =0;
    if (rForm && rForm->exec() == QDialog::Accepted)
    {
        rName = rForm->getRubricName ();
        rst = rForm->getSearchTemplate ();
        rc = rForm->getCategory ();
        ac = rForm->getAccessEntity();
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
    rForm->setParent (0);
    delete rForm;

    twIncludes->model()->setData (index, rName, Qt::DisplayRole);
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
void KKSIncludesWidget :: delRubricItem (void)
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

    QModelIndex index = sm ? sm->currentIndex() : QModelIndex ();
    if (!sm || !index.isValid())
    {
        return;
    }
    else
    {
        index = index.sibling(index.row(), 0);
        if (!isRubr || index.data(Qt::UserRole).toInt() == 2)
        {
            //
            // item selected
            //
            int res = QMessageBox::question (this, tr ("Delete rubric"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (res != QMessageBox::Yes)
                return;
        }
        else
        {
            //
            // rubric selected
            //
            return;
        }
    }

    QModelIndex wIndex = twIncludes->selectionModel()->currentIndex ();
    KKSRubric * r = getRubric(wIndex.parent());
    if(!r)
        return;
    
    r->removeItem (index.row());
    if (isRubr)
        twIncludes->model()->removeRow(wIndex.row(), wIndex.parent());
    else
        tvItems->model ()->removeRow (index.row(), index.parent());
    isChanged = true;
    emit rubricsChanged ();
}

void KKSIncludesWidget :: addRubricItem (void)
{
    QItemSelectionModel * sm (twIncludes->selectionModel());

//    sm = twIncludes->selectionModel();

    QModelIndex index = sm ? sm->currentIndex() : QModelIndex();
    if (!sm || !index.isValid())
    {
        QMessageBox::warning(this, 
                             tr("Warning"), 
                             tr("You should select rubric to add item"), 
                             QMessageBox::Ok);
        return;
    }
    
    KKSRubric * r = currentRubric();
    if(!r)
        return;

    emit rubricItemRequested();
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
    if(!sm || !(index = sm->currentIndex()).isValid()){
        QMessageBox::warning(this, 
                             tr("Warning"), 
                             tr("You should select rubric to add item"), 
                             QMessageBox::Ok);
        return;
    }
    
    KKSRubric * r = currentRubric();
    if (!r)
        return;

    KKSRubricItem * equalItem = r->itemForId(idObject);
    if(equalItem){
        QMessageBox::critical(this, 
                             tr("Error"), 
                             tr("You cannot add one item to rubric twise"), 
                             QMessageBox::Ok);
        return;
    }

    KKSRubricItem * item = new KKSRubricItem(idObject, name, false);

    bool rIsEmpty = r->items().isEmpty();

    r->addItem(item);

    //is rubric item
    if(index.data(Qt::UserRole) == 2)
        index = index.parent();

    if (r->getCategory())
    {
        qDebug () << __PRETTY_FUNCTION__ << r->name() << rIsEmpty;
        if (rIsEmpty)
        {
            QAbstractItemModel * rmodel = twIncludes->model();
            rmodel->insertRows (0, 1, index);
            if (rmodel->columnCount (index) == 0)
                rmodel->insertColumns (0, 1, index);
            QModelIndex wIndex = rmodel->index (0, 0, index);
            rmodel->setData (wIndex, tr ("View attachments ..."), Qt::DisplayRole);
            rmodel->setData (wIndex, 2, Qt::UserRole);//is item
        }
        return;
    }
    QModelIndex cIndex = appendItemRow(item, index);
    twIncludes->setCurrentIndex(cIndex);
    
    item->release();
    isChanged = true;
    emit rubricsChanged ();
}

void KKSIncludesWidget :: editRubricItem (void)
{
    int idObject = -1;

    QItemSelectionModel * sm;

    bool isRubr = true;
    if (recWItems->isVisible ())
    {
        sm = tvItems->selectionModel ();
        isRubr = false;
    }
    else
        sm = twIncludes->selectionModel();

    QModelIndex index = sm ? sm->currentIndex() : QModelIndex();
    if(!sm || !index.isValid())
    {
        return;
    }
    else{
        index = index.sibling(index.row(), 0);
        if (!isRubr || index.data(Qt::UserRole).toInt() == 2){ //item selected
            ;
        }
        else{//rubric selected
            return;
        }
    }
    
    idObject = index.data(Qt::UserRole+(isRubr ? 1 : 0)).toInt();//idObject
    
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
    Q_UNUSED(index);

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

KKSRubric * KKSIncludesWidget::getRubric(QModelIndex index)
{
    if (!m_rootRubric)
        return 0;

    if (index.parent().isValid())
    {
        KKSRubric * r = getRubric(index.parent());
        if(index.data(Qt::UserRole).toInt() == 2)//item
            return r;
        
        QAbstractItemModel * model = twIncludes->model();
        int rowCount = model->rowCount(index.parent());
        int row = 0;
        int currentRow = index.row();
        for(int i=0; i<rowCount; i++){
            QModelIndex ind = model->index(i, 0, index.parent());
            if(ind.data(Qt::UserRole).toInt() == 1)//rubric
                row++;
            if(ind.row() == currentRow){
                row--;
                break;
            }
        }
        
        return const_cast<KKSRubric*>(r->rubric(row));
    }
    
    return const_cast<KKSRubric*>(m_rootRubric->rubric(index.row()));
}


KKSRubric * KKSIncludesWidget::currentRubric()
{

    QItemSelectionModel * sm = twIncludes->selectionModel();
    QModelIndex index;
    if(!sm || !(index = sm->currentIndex()).isValid()){
        return NULL;
    }
    
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

    if (wIndexList.isEmpty() && oldIndexList.isEmpty())
    {
        recWItems->setVisible (false);
        return;
    }
    else if (wIndexList.isEmpty())
        return;

    QModelIndex wIndex = wIndexList[0];
    if (!wIndex.parent().isValid())
    {
        recWItems->setVisible (false);
        return;
    }
    KKSRubric * rubr = getRubric (wIndex.parent());
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
//        KKSItemDelegate * iDeleg = qobject_cast<KKSItemDelegate *>(recWItems->getView()->itemDelegate());
//        if (iDeleg)
//            iDeleg->setCategory (rubr->getCategory ());
        emit rubricAttachmentsView (attachModel, rubr);
    }
    else
        recWItems->setVisible (false);
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