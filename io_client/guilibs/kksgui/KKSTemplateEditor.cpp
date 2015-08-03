#include <QPushButton>
#include <QGridLayout>
#include <QTabWidget>
#include <QTreeView>
#include <QTableView>
#include <QLineEdit>
#include <QLabel>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QHBoxLayout>
#include <QSize>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QAbstractItemModel>
#include <QAbstractItemDelegate>
#include <QItemSelection>
#include <QGroupBox>
#include <QMessageBox>
#include <QtDebug>

#include <KKSTemplate.h>
#include <KKSCategory.h>
#include <defines.h>
#include "KKSTemplateEditor.h"
#include "KKSEventFilter.h"

KKSTemplateEditor :: KKSTemplateEditor (KKSTemplate *t, bool mode, QWidget *parent, Qt::WindowFlags f)
    : KKSDialog (parent, f),
    tRef (t),
    lEID (new QLineEdit (QString::number (t ? t->id() : 0))),
    lETitle (new QLineEdit (t->name())),
    lECat (new QLineEdit ((tRef ? tRef->category()->name() : QString()))),
    tvGroups (new QTreeView ()),
    pbAddGroup (new QPushButton (tr("Add"))),
    pbEditGroup (new QPushButton (tr("Edit"))),
    pbDelGroup (new QPushButton (tr("Delete"))),
    pbAddAttr (new QPushButton (tr("&Paste"))),
    pbEditAttr (new QPushButton (tr("Edit"))),
    pbDelAttr (new QPushButton (tr("Cu&t"))),
    pbUp (new QPushButton (tr("Up"))),
    pbDown (new QPushButton (tr("Down"))),
    tableAttrs (new QTableView()),
    pbOk (new QPushButton (tr("&OK"))),
    pbCancel (new QPushButton (tr("&Cancel"))),
    pbApply (new QPushButton (tr("A&pply"))),
    dbErr (false)
{
    if (t)
        tRef->addRef ();

    init_widgets (mode);
    tvGroups->setSelectionBehavior (QAbstractItemView::SelectRows);

    connect (pbAddGroup, SIGNAL (clicked()), this, SLOT (addTemplateGroup()) );
    connect (pbEditGroup, SIGNAL (clicked()), this, SLOT (editTemplateGroup()) );
    connect (pbDelGroup, SIGNAL (clicked()), this, SLOT (delTemplateGroup()) );

    connect (pbAddAttr, SIGNAL (clicked()), this, SLOT (addAttribute()) );
    connect (pbEditAttr, SIGNAL (clicked()), this, SLOT (editAttribute()) );
    connect (pbDelAttr, SIGNAL (clicked()), this, SLOT (delAttribute()) );

    connect (tvGroups, SIGNAL(expanded(const QModelIndex &)), this, SLOT(itemExpanded(const QModelIndex &)));
    connect (tvGroups, SIGNAL(collapsed(const QModelIndex &)), this, SLOT(itemCollapsed(const QModelIndex &)));

    //pbUp->setEnabled (false);
    //pbDown->setEnabled (false);

    pbApply->setVisible (false);
    connect (pbUp, SIGNAL (clicked()), this, SLOT (moveUp()) );
    connect (pbDown, SIGNAL (clicked()), this, SLOT (moveDown()) );

    connect (pbOk, SIGNAL (clicked()), this, SLOT (saveClose()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
    connect (pbApply, SIGNAL (clicked()), this, SLOT (save()) );

    m_icon = QIcon(":/ddoc/organize_1949.png");
}

KKSTemplateEditor :: ~KKSTemplateEditor ()
{
    if (tRef)
        tRef->release ();
}

int KKSTemplateEditor :: getTemplateID (void) const
{
    if (tRef)
        return tRef->id ();
    return -1;
}

int KKSTemplateEditor :: getCategoryID (void) const
{
    if (tRef && tRef->category())
        return tRef->category()->id();
    return -1;
}

void KKSTemplateEditor :: save (void)
{
    if (lETitle->text().isEmpty())
        return;
    tRef->setName (lETitle->text());
    emit saveTemplate (tRef, this);
}

void KKSTemplateEditor :: saveClose (void)
{
    save ();
    if (!dbErr)
        accept ();
}

void KKSTemplateEditor :: init_widgets (bool mode)
{
    QGridLayout *gLayout = new QGridLayout ();
    this->setLayout (gLayout);
    QTabWidget * tabWidget = new QTabWidget ();
    gLayout->addWidget (tabWidget, 0, 0, 1, 1);

    this->init_par_widget (tabWidget);
    this->init_group_widget (tabWidget);
//    this->init_attr_widget (tabWidget);

    QHBoxLayout *hButtonsLay = new QHBoxLayout ();
    hButtonsLay->addStretch ();
    hButtonsLay->addWidget (pbOk);
    hButtonsLay->addWidget (pbCancel);
    hButtonsLay->addWidget (pbApply);
    gLayout->addLayout (hButtonsLay, 1, 0, 1, 1);

    pbOk->setVisible (mode);
    pbCancel->setVisible (mode);
    pbApply->setVisible (mode);
}

void KKSTemplateEditor :: init_par_widget (QTabWidget * tWidget)
{
    QWidget *parWidget = new QWidget ();
    tWidget->addTab (parWidget, tr("Parameters"));
    QGridLayout *gPLay = new QGridLayout ();
    parWidget->setLayout (gPLay);

    QLabel *lID = new QLabel (QString("ID :"));
    gPLay->addWidget (lID, 0, 0, 1, 1, Qt::AlignRight);
    lEID->setReadOnly (true);
    gPLay->addWidget (lEID, 0, 1, 1, 1);

    QLabel *lTitle = new QLabel (tr("Title :"));
    gPLay->addWidget (lTitle, 1, 0, 1, 1, Qt::AlignRight);
    gPLay->addWidget (lETitle, 1, 1, 1, 1);

    QLabel *lCat = new QLabel (tr("Category :"));
    gPLay->addWidget (lCat, 2, 0, 1, 1, Qt::AlignRight);
    lECat->setReadOnly (true);
    gPLay->addWidget (lECat, 2, 1, 1, 1);
    lECat->adjustSize ();

    QSpacerItem *sPar = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gPLay->addItem (sPar, 3, 1, 1, 1);
}

void KKSTemplateEditor :: init_group_widget (QTabWidget *tWidget)
{
    QWidget *groupWidget = new QWidget ();
    tWidget->addTab (groupWidget, tr("Groups"));
    QGridLayout *gGLay = new QGridLayout ();
    groupWidget->setLayout (gGLay);
    KKSEventFilter *ef = new KKSEventFilter (groupWidget);
    tvGroups->viewport()->installEventFilter (ef);

    gGLay->addWidget (tvGroups, 0, 0, 5, 1);

    QGroupBox *gbAttrGroup = new QGroupBox (tr("Group of attributes"));
    gGLay->addWidget (gbAttrGroup, 0, 1, 1, 1);
    QGridLayout *gAttrGLay = new QGridLayout ();
    gbAttrGroup->setLayout (gAttrGLay);
    gAttrGLay->addWidget (pbAddGroup, 0, 0, 1, 1);
    gAttrGLay->addWidget (pbEditGroup, 1, 0, 1, 1);
    gAttrGLay->addWidget (pbDelGroup, 2, 0, 1, 1);

    QGroupBox *gbAttr = new QGroupBox (tr("Attributes"));
    gGLay->addWidget (gbAttr, 1, 1, 1, 1);
    QGridLayout *gAttrLay = new QGridLayout ();
    gbAttr->setLayout (gAttrLay);
    gAttrLay->addWidget (pbAddAttr, 0, 0, 1, 1);
    gAttrLay->addWidget (pbEditAttr, 1, 0, 1, 1);
    gAttrLay->addWidget (pbDelAttr, 2, 0, 1, 1);

    gGLay->addWidget (pbUp, 2, 1, 1, 1);
    gGLay->addWidget (pbDown, 3, 1, 1, 1);

    QSpacerItem *sGroup = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gGLay->addItem (sGroup, 4, 1, 1, 1);
}

void KKSTemplateEditor :: init_attr_widget (QTabWidget *tWidget)
{
    QWidget *attrWidget = new QWidget ();
    tWidget->addTab (attrWidget, tr ("Attributes"));
    QGridLayout *gAttrsLay = new QGridLayout ();
    attrWidget->setLayout (gAttrsLay);

    gAttrsLay->addWidget (tableAttrs, 0, 0, 1, 1);
}

void KKSTemplateEditor :: addTemplateGroup (void)
{
    QModelIndex gIndex = this->getSelectedIndex (); //всегда возвращает колонку с индексом 0

    while (gIndex.isValid() && gIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0) //Qt::UserRole+USER_ENTITY - это флаг (0 - группа / 1 - атрибут
        gIndex = gIndex.parent ();

    emit addAttrGroup (tRef, gIndex, this);
}

void KKSTemplateEditor :: editTemplateGroup (void)
{
    QModelIndex gIndex = this->getSelectedIndex ();//всегда возвращает колонку с индексом 0
    if (!gIndex.isValid () || gIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)//Qt::UserRole+USER_ENTITY - это флаг (0 - группа / 1 - атрибут
    {
        qWarning() << tr("Please select group");
        QMessageBox::warning (this, tr("Edit group of attributes"), tr("Please select group"));
        return;
    }

    emit editAttrGroup (gIndex.data (Qt::UserRole).toInt(), gIndex, tRef, this);
}

void KKSTemplateEditor :: delTemplateGroup (void)
{
    QModelIndex gIndex = this->getSelectedIndex ();//всегда возвращает колонку с индексом 0
    if (!gIndex.isValid () || gIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0) //Qt::UserRole+USER_ENTITY - это флаг (0 - группа / 1 - атрибут
    {
        qWarning() << tr("Please select group");
        QMessageBox::warning (this, tr("Delete group of attributes"), tr("Please select group"));
        return;
    }

    if (gIndex.model()->rowCount() == 1)
    {
        //
        // Template contains only one group
        //
        qWarning() << tr("You cannot delete single group");
        QMessageBox::warning (this, tr("Delete group of attributes"), tr ("You cannot delete single group"));
        return;
    }

    if (gIndex.child (0, 0).isValid ())
    {
        qWarning() << tr ("You cannot delete not empty group");
        QMessageBox::warning (this, tr("Delete group of attributes"), tr ("You cannot delete not empty group"));
        return;
    }

    int res = QMessageBox::question (this, tr("Delete group of attributes"), tr ("Do you really want to delete group %1 ?").arg (gIndex.data (Qt::DisplayRole).toString()), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
    if (res == QMessageBox::Yes)
        emit delAttrGroup (gIndex.data (Qt::UserRole).toInt(), gIndex, tRef, this);
}

void KKSTemplateEditor :: addAttribute (void)
{
    const KKSMap<int, KKSCategoryAttr *> avAttrs = tRef->availableAttrs();
    if (avAttrs.isEmpty())
    {
        qWarning() << tr("No available attributes");
        QMessageBox::warning (this, tr("Add attributes into template"), tr("No available attributes"), QMessageBox::Ok);
        return;
    }

    QModelIndex gIndex = this->getSelectedIndex ();//всегда возвращает колонку с индексом 0
    
    //добавлять атрибут можно только в группу. Qt::UserRole+USER_ENTITY - это флаг (0 - группа / 1 - атрибут
    if (!gIndex.isValid () || gIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0) 
    {
        qWarning() << tr("Please select target group");
        QMessageBox::warning (this, tr("Add attribute"), tr("Please select target group"), QMessageBox::Ok);
        return;
    }
    emit addAttrIntoGroup (gIndex.data (Qt::UserRole).toInt(), gIndex, tRef, this);
}

void KKSTemplateEditor :: editAttribute (void)
{
    QModelIndex aIndex = this->getSelectedIndex();//selModel->currentIndex ();
    if (!aIndex.isValid () || aIndex.data (Qt::UserRole+USER_ENTITY).toInt() != 0)
        return;

    QModelIndex gIndex = aIndex.parent ();
    emit editAttrInGroup (aIndex.data (Qt::UserRole).toInt(), gIndex.data (Qt::UserRole).toInt(), aIndex, gIndex, tRef, this);
}

void KKSTemplateEditor :: delAttribute (void)
{
    QModelIndex aIndex = this->getSelectedIndex ();//selModel->currentIndex ();
    if (!aIndex.isValid () || aIndex.data (Qt::UserRole+USER_ENTITY).toInt() != 0 )
        return;

    QModelIndex gIndex = aIndex.parent ();
    emit delAttrFromGroup (aIndex.data (Qt::UserRole).toInt(), gIndex.data (Qt::UserRole).toInt(), aIndex, gIndex, tRef, this);
}

void KKSTemplateEditor :: moveUp (void)
{
    QModelIndex gIndex = this->getSelectedIndex ();
    if (!gIndex.isValid())
        return;

    int eType = gIndex.data(Qt::UserRole+USER_ENTITY).toInt();
    if(eType == 0){//0 - атрибут
        //
        // Attribute (и у него однозначно есть родитель (это группа))
        //
        int idAttrGroup = gIndex.parent().data (Qt::UserRole).toInt();
        int idAttr = gIndex.data (Qt::UserRole).toInt ();
        KKSAttrGroup * aGroup = tRef->searchGroupById(idAttrGroup);
        if (!aGroup)
            return;
        KKSAttrView * aView = aGroup->attrView (idAttr);
        if (!aView)
            return;

        int parentRow = gIndex.parent().row();
        int row = gIndex.row();

        if ( row > 0) //меняем порядок атрибутов внутри группы
        {
            int prevRow = row;//-1;
            int eType = 0;
            QModelIndex gIndexPrev;// = gIndex.sibling (prevRow, 0);
            do{
                gIndexPrev = gIndex.sibling (--prevRow, 0);
                eType = gIndexPrev.data(Qt::UserRole+USER_ENTITY).toInt();
            }
            while(prevRow >= 0 && eType == 1); //1 - группа

            if (prevRow < 0 || !gIndexPrev.isValid())
                return;

            int idAttrPrev = gIndexPrev.data (Qt::UserRole).toInt();
            KKSAttrView * aViewPrev = aGroup->attrView (idAttrPrev);
            if (!aViewPrev)
                return;

            this->swapAttrs (aView, aViewPrev, gIndex, gIndexPrev);
        }
        else //атрибут в группе стал самым первым, надо перенести его в предыдущую группу (если она есть)
        {
            QModelIndex groupIndex = gIndex.parent();
            int prevGroupRow = groupIndex.row() - 1;


            QModelIndex prevGroupIndex;
            bool b_prepend = false;
            //переносим группу из текущей группы в предыдущую
            if(prevGroupRow < 0){
                //пробуем найти родителя этой предыдущей группы (рекурсивно)
                prevGroupIndex = findTopLevelGroupIndex(groupIndex);
                b_prepend = true; //в этом случае нам необходимо сделать группу по порядку самой первой, т.к. мы ее вверх переносим на предыдущий уровень
            }
            else{
                prevGroupIndex = groupIndex.sibling(prevGroupRow, 0);
            }

            if (!prevGroupIndex.isValid())
                return;

            int idAttrGroupPrev = prevGroupIndex.data (Qt::UserRole).toInt ();
            emit delAttrFromGroup (idAttr, idAttrGroup, gIndex, groupIndex, tRef, this);
            emit appendAttrIntoGroup (idAttr, idAttrGroupPrev, prevGroupIndex, tRef, this, b_prepend);
            
            //сохраняем перемещенный атрибут выделенным

            QModelIndex newIndex = findModelIndex1(idAttr);
            if(newIndex.isValid()){
                QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
                this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
                int columnCount = this->tvGroups->model()->columnCount(newIndex.parent());
                QModelIndex topLeft = newIndex.sibling (newIndex.row(), 0);
                QModelIndex bottomRight = newIndex.sibling(newIndex.row(), columnCount-1);
                QItemSelection newSel = QItemSelection (topLeft, bottomRight);
                this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
                this->tvGroups->selectionModel ()->setCurrentIndex (newIndex, QItemSelectionModel::Select);   
            }                    
        }
    }
    else{
        //
        //Group
        //
        int idAttrGroup = gIndex.data (Qt::UserRole).toInt();
        KKSAttrGroup * aGroup = tRef->searchGroupById(idAttrGroup);
        if (!aGroup)
            return;

        int row = gIndex.row();
        int prevRow = row;
        int eType = 0;
        QModelIndex gIndexPrev;// = gIndex.sibling (prevRow, 0);
        do{
            gIndexPrev = gIndex.sibling (--prevRow, 0);
            eType = gIndexPrev.data(Qt::UserRole+USER_ENTITY).toInt();
        }
        while(prevRow >= 0 && eType == 0); //1 - атрибут

        if (prevRow < 0 || !gIndexPrev.isValid()){
            //если группа в группе стала самой первой, то ее необходимо переместить в предыдущую группу
            
            QModelIndex groupIndex = gIndex.parent();
            int prevGroupRow = groupIndex.row() - 1;
            
            QModelIndex prevGroupIndex;
            bool b_prepend = false;
            //переносим группу из текущей группы в предыдущую только в случае если текущая группа не первая по списку
            if(prevGroupRow < 0){
                //пробуем найти родителя этой предыдущей группы (рекурсивно)
                prevGroupIndex = findTopLevelGroupIndex(groupIndex);
                b_prepend = true; //в этом случае нам необходимо сделать группу по порядку самой первой, т.к. мы ее вверх переносим на предыдущий уровень
            }
            else{
                prevGroupIndex = groupIndex.sibling(prevGroupRow, 0);
            }
            
            if (!prevGroupIndex.isValid())
                return;

            //idAttrGroup - перемещаемая группа
            //из какой удаляем
            int idAttrGroupFrom = groupIndex.data(Qt::UserRole).toInt();
            //в какую добавляем
            int idAttrGroupTo = prevGroupIndex.data(Qt::UserRole).toInt();

            KKSAttrGroup * aGroupFrom = tRef->searchGroupById(idAttrGroupFrom, true);
            KKSAttrGroup * aGroupTo = tRef->searchGroupById(idAttrGroupTo, true);
            if(!aGroupFrom || !aGroupTo)
                return;

            aGroup->addRef();
            aGroup->setOrder(-1);//чтобы автоматически задать последний доступный номер по порядку
            aGroupFrom->removeChildGroup(idAttrGroup);
            if(b_prepend)
                aGroupTo->prependChildGroup(idAttrGroup, aGroup);
            else
                aGroupTo->addChildGroup(idAttrGroup, aGroup);
            aGroup->release();

            emit updateAttrGroups (tRef, this);
            
            //сохраним выделенным перемещенный элемент
            //как с атрибутами поступить не получится - при отработке предыдущего сигнала в методе KKSViewFactory :: initTemplateGroups
            //происходит удаление всех строк и заново создается модель.
            //поэтому нам необходимо найти строку, которая содержит QModelIndex с userData = idAttrGroup
            QModelIndex newIndex = findModelIndex(idAttrGroup);
            if(newIndex.isValid()){
                QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
                this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
                int columnCount = this->tvGroups->model()->columnCount(newIndex.parent());
                QModelIndex topLeft = newIndex.sibling (newIndex.row(), 0);
                QModelIndex bottomRight = newIndex.sibling(newIndex.row(), columnCount-1);
                QItemSelection newSel = QItemSelection (topLeft, bottomRight);
                this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
                this->tvGroups->selectionModel ()->setCurrentIndex (newIndex, QItemSelectionModel::Select);   
            }                    
            
            return;
            
        }
            
        //в противном случае - меняем значение параметра order между группами
        int idAttrGroupPrev = gIndexPrev.data (Qt::UserRole).toInt ();
        KKSAttrGroup *aGroupPrev = tRef->searchGroupById(idAttrGroupPrev);
        if (!aGroupPrev)
            return;
        
        int worder = aGroup->order ();
        int prevOrder = aGroupPrev->order();
        aGroup->setOrder (prevOrder);
        aGroupPrev->setOrder (worder);

        emit updateAttrGroups (tRef, this);
        
        //сохраним выделенным перемещенный элемент
        //как с атрибутами поступить не получится - при отработке предыдущего сигнала в методе KKSViewFactory :: initTemplateGroups
        //происходит удаление всех строк и заново создается модель.
        //поэтому нам необходимо найти строку, которая содержит QModelIndex с userData = idAttrGroup
        QModelIndex newIndex = findModelIndex(idAttrGroup);
        if(newIndex.isValid()){
            QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
            this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
            int columnCount = this->tvGroups->model()->columnCount(newIndex.parent());
            QModelIndex topLeft = newIndex.sibling (newIndex.row(), 0);
            QModelIndex bottomRight = newIndex.sibling(newIndex.row(), columnCount-1);
            QItemSelection newSel = QItemSelection (topLeft, bottomRight);
            this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
            this->tvGroups->selectionModel ()->setCurrentIndex (newIndex, QItemSelectionModel::Select);   
        }
    }
}

void KKSTemplateEditor :: moveDown (void)
{
    QModelIndex gIndex = this->getSelectedIndex ();
    if (!gIndex.isValid())
        return;

    int eType = gIndex.data(Qt::UserRole+USER_ENTITY).toInt();
    if(eType == 0){//0 - атрибут
        //
        // Attribute (и у него однозначно есть родитель (это группа))
        //
        int idAttrGroup = gIndex.parent().data (Qt::UserRole).toInt();
        int idAttr = gIndex.data (Qt::UserRole).toInt ();
        KKSAttrGroup * aGroup = tRef->searchGroupById(idAttrGroup);
        if (!aGroup)
            return;
        KKSAttrView * aView = aGroup->attrView (idAttr);
        if (!aView)
            return;

        int parentRow = gIndex.parent().row();
        int row = gIndex.row();
        //int rowCount = tvGroups->model()->rowCount(gIndex.parent());

        int nr = tvGroups->model()->rowCount (gIndex.parent());
        int iLastAttr = 0;
        for (int i=0; i<nr; i++)
        {
            QModelIndex wIndex = tvGroups->model()->index (i, 0, gIndex.parent());
            if (wIndex.isValid() && wIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0) //0 - атрибут
                iLastAttr = i;
        }

        if (tvGroups->model()->rowCount (gIndex.parent()) != 0 && tvGroups->model()->index (0, 0, gIndex.parent()).data (Qt::UserRole+USER_ENTITY).toInt ()==0)
            iLastAttr++;

        int rowCount = iLastAttr;

        if ( row < rowCount-1) //меняем порядок атрибутов внутри группы
        {
            int nextRow = row;//-1;
            int eType = 0;
            QModelIndex gIndexNext;
            do{
                gIndexNext = gIndex.sibling (++nextRow, 0);
                eType = gIndexNext.data(Qt::UserRole+USER_ENTITY).toInt();
            }
            while(nextRow <= rowCount && eType == 1); //1 - группа

            if (nextRow >= rowCount || !gIndexNext.isValid())
                return;

            int idAttrNext = gIndexNext.data (Qt::UserRole).toInt();
            KKSAttrView * aViewNext = aGroup->attrView (idAttrNext);
            if (!aViewNext)
                return;

            this->swapAttrs (aView, aViewNext, gIndex, gIndexNext);
        }
        else if(rowCount < nr){ //атрибут в группе стал последним, но есть в текущей группе подгруппы. Переносим атрибут в первую подгруппу текущей группы
            
            QModelIndex groupIndex = gIndex.parent();
            //while(1){
            //    groupIndex = 
            //}
            
            int nextGroupRow = gIndex.row() + 1;
            

            QModelIndex nextGroupIndex;
            bool b_prepend = false;
            //int groupRowCount = tvGroups->model()->rowCount(groupIndex.parent());
            //переносим атрибут из текущей группы в следующую

            //if(nextGroupRow >= groupRowCount){
            //    //пробуем найти следующего родителя этой следующей группы (рекурсивно)
            //    nextGroupIndex = findTopLevelGroupIndex1(groupIndex);
            //    b_prepend = true; //в этом случае нам необходимо сделать группу по порядку самой первой, т.к. мы ее вниз переносим на предыдущий уровень
            //}
            //else{
                nextGroupIndex = gIndex.sibling(nextGroupRow, 0);
                b_prepend = true; //в этом случае нам необходимо сделать группу по порядку самой первой, т.к. мы ее вниз переносим на предыдущий уровень
            //}

            if (!nextGroupIndex.isValid())
                return;

            if(nextGroupIndex.data(Qt::UserRole+USER_ENTITY).toInt() != 1) //1 - группа
                return;

            int idAttrGroupNext = nextGroupIndex.data (Qt::UserRole).toInt ();
            //в данном случае в результате выполнения этого эмита строка с атрибутом (groupIndex-ом) будет удалена. Количество строк в группе уменьшится на единицу
            //поэтому nextGroupIndex будет указывать на следующую строку, а не на ту, которая нам нужна
            //соответственно надо будет найти нужный нам QModelIndex заново
            emit delAttrFromGroup (idAttr, idAttrGroup, gIndex, groupIndex, tRef, this);
            nextGroupIndex = findModelIndex(idAttrGroupNext);

            emit appendAttrIntoGroup (idAttr, idAttrGroupNext, nextGroupIndex, tRef, this, b_prepend);
            
            //сохраняем перемещенный атрибут выделенным

            QModelIndex newIndex = findModelIndex1(idAttr);
            if(newIndex.isValid()){
                QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
                this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
                int columnCount = this->tvGroups->model()->columnCount(newIndex.parent());
                QModelIndex topLeft = newIndex.sibling (newIndex.row(), 0);
                QModelIndex bottomRight = newIndex.sibling(newIndex.row(), columnCount-1);
                QItemSelection newSel = QItemSelection (topLeft, bottomRight);
                this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
                this->tvGroups->selectionModel ()->setCurrentIndex (newIndex, QItemSelectionModel::Select);   
            }          
        }
        else //атрибут в группе стал самым последним, надо перенести его в следующую группу того же уровня (если она есть)
        {
            QModelIndex groupIndex = gIndex.parent();
            int nextGroupRow = groupIndex.row() + 1;


            QModelIndex nextGroupIndex;
            bool b_prepend = false;
            int groupRowCount = tvGroups->model()->rowCount(groupIndex.parent());
            //переносим атрибут из текущей группы в следующую

            if(nextGroupRow >= groupRowCount){
                //пробуем найти следующего родителя этой следующей группы (рекурсивно)
                nextGroupIndex = findTopLevelGroupIndex1(groupIndex);
                b_prepend = true; //в этом случае нам необходимо сделать группу по порядку самой первой, т.к. мы ее вниз переносим на предыдущий уровень
            }
            else{
                nextGroupIndex = groupIndex.sibling(nextGroupRow, 0);
                b_prepend = true; //в этом случае нам необходимо сделать группу по порядку самой первой, т.к. мы ее вниз переносим на предыдущий уровень
            }

            if (!nextGroupIndex.isValid())
                return;

            int idAttrGroupNext = nextGroupIndex.data (Qt::UserRole).toInt ();
            emit delAttrFromGroup (idAttr, idAttrGroup, gIndex, groupIndex, tRef, this);
            emit appendAttrIntoGroup (idAttr, idAttrGroupNext, nextGroupIndex, tRef, this, b_prepend);
            
            //сохраняем перемещенный атрибут выделенным

            QModelIndex newIndex = findModelIndex1(idAttr);
            if(newIndex.isValid()){
                QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
                this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
                int columnCount = this->tvGroups->model()->columnCount(newIndex.parent());
                QModelIndex topLeft = newIndex.sibling (newIndex.row(), 0);
                QModelIndex bottomRight = newIndex.sibling(newIndex.row(), columnCount-1);
                QItemSelection newSel = QItemSelection (topLeft, bottomRight);
                this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
                this->tvGroups->selectionModel ()->setCurrentIndex (newIndex, QItemSelectionModel::Select);   
            }                    
        }
    }
    else{
        //
        //Group
        //
        int idAttrGroup = gIndex.data (Qt::UserRole).toInt();
        KKSAttrGroup * aGroup = tRef->searchGroupById(idAttrGroup);
        if (!aGroup)
            return;

        int row = gIndex.row();
        int nextRow = row;
        int eType = 0;
        int rowCount = tvGroups->model()->rowCount(gIndex.parent());
        QModelIndex gIndexNext;
        do{
            gIndexNext = gIndex.sibling (++nextRow, 0);
            eType = gIndexNext.data(Qt::UserRole+USER_ENTITY).toInt();
        }
        while(nextRow <= rowCount && eType == 0); //0 - атрибут

        if (nextRow >= rowCount || !gIndexNext.isValid()){
            //если группа в группе стала самой последней, то ее необходимо переместить в следующую группу
            
            QModelIndex groupIndex = gIndex.parent();
            int nextGroupRow = groupIndex.row() + 1;
            
            QModelIndex nextGroupIndex;
            bool b_prepend = false;
            //переносим группу из текущей группы в следующую только в случае если текущая группа не последняя по списку
            if(nextGroupRow >= rowCount){
                //пробуем найти родителя этой следующей группы (рекурсивно)
                nextGroupIndex = findTopLevelGroupIndex1(groupIndex);
                b_prepend = true; //в этом случае нам необходимо сделать группу по порядку самой первой, т.к. мы ее вверх переносим на предыдущий уровень
            }
            else{
                nextGroupIndex = groupIndex.sibling(nextGroupRow, 0);
                b_prepend = true; //в этом случае нам необходимо сделать группу по порядку самой первой, т.к. мы ее вверх переносим на предыдущий уровень
            }
            
            if (!nextGroupIndex.isValid())
                return;

            //idAttrGroup - перемещаемая группа
            //из какой удаляем
            int idAttrGroupFrom = groupIndex.data(Qt::UserRole).toInt();
            //в какую добавляем
            int idAttrGroupTo = nextGroupIndex.data(Qt::UserRole).toInt();

            KKSAttrGroup * aGroupFrom = tRef->searchGroupById(idAttrGroupFrom, true);
            KKSAttrGroup * aGroupTo = tRef->searchGroupById(idAttrGroupTo, true);
            if(!aGroupFrom || !aGroupTo)
                return;

            aGroup->addRef();
            aGroup->setOrder(-1);//чтобы автоматически задать последний (или первый) доступный номер по порядку
            aGroupFrom->removeChildGroup(idAttrGroup);
            if(b_prepend)
                aGroupTo->prependChildGroup(idAttrGroup, aGroup);
            else
                aGroupTo->addChildGroup(idAttrGroup, aGroup);
            aGroup->release();

            emit updateAttrGroups (tRef, this);
            
            //сохраним выделенным перемещенный элемент
            //как с атрибутами поступить не получится - при отработке предыдущего сигнала в методе KKSViewFactory :: initTemplateGroups
            //происходит удаление всех строк и заново создается модель.
            //поэтому нам необходимо найти строку, которая содержит QModelIndex с userData = idAttrGroup
            QModelIndex newIndex = findModelIndex(idAttrGroup);
            if(newIndex.isValid()){
                QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
                this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
                int columnCount = this->tvGroups->model()->columnCount(newIndex.parent());
                QModelIndex topLeft = newIndex.sibling (newIndex.row(), 0);
                QModelIndex bottomRight = newIndex.sibling(newIndex.row(), columnCount-1);
                QItemSelection newSel = QItemSelection (topLeft, bottomRight);
                this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
                this->tvGroups->selectionModel ()->setCurrentIndex (newIndex, QItemSelectionModel::Select);   
            }                    
            
            return;
            
        }
            
        //в противном случае - меняем значение параметра order между группами
        int idAttrGroupNext = gIndexNext.data (Qt::UserRole).toInt ();
        KKSAttrGroup *aGroupNext = tRef->searchGroupById(idAttrGroupNext);
        if (!aGroupNext)
            return;
        
        int worder = aGroup->order ();
        int nextOrder = aGroupNext->order();
        aGroup->setOrder (nextOrder);
        aGroupNext->setOrder (worder);

        emit updateAttrGroups (tRef, this);
        
        //сохраним выделенным перемещенный элемент
        //как с атрибутами поступить не получится - при отработке предыдущего сигнала в методе KKSViewFactory :: initTemplateGroups
        //происходит удаление всех строк и заново создается модель.
        //поэтому нам необходимо найти строку, которая содержит QModelIndex с userData = idAttrGroup
        QModelIndex newIndex = findModelIndex(idAttrGroup);
        if(newIndex.isValid()){
            QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
            this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
            int columnCount = this->tvGroups->model()->columnCount(newIndex.parent());
            QModelIndex topLeft = newIndex.sibling (newIndex.row(), 0);
            QModelIndex bottomRight = newIndex.sibling(newIndex.row(), columnCount-1);
            QItemSelection newSel = QItemSelection (topLeft, bottomRight);
            this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
            this->tvGroups->selectionModel ()->setCurrentIndex (newIndex, QItemSelectionModel::Select);   
        }
    }
}

QModelIndex KKSTemplateEditor :: findTopLevelGroupIndex(const QModelIndex & child) const
{
    QModelIndex gIndex;

    if(!child.isValid())
        return gIndex;

    gIndex = child.parent();
    if(!gIndex.isValid())
        return gIndex;

    int prevRow = gIndex.row();
    if(prevRow == 0)
        gIndex = findTopLevelGroupIndex(gIndex);

    return gIndex;
}

QModelIndex KKSTemplateEditor :: findTopLevelGroupIndex1(const QModelIndex & child) const
{
    QModelIndex gIndex;

    if(!child.isValid())
        return gIndex;

    gIndex = child.parent();
    if(!gIndex.isValid())
        return gIndex;

    int nextRow = gIndex.row()+1;
    int rowCount = tvGroups->model()->rowCount(gIndex.parent());
    if(nextRow == rowCount)
        gIndex = findTopLevelGroupIndex1(gIndex);
    else
        gIndex = gIndex.sibling(nextRow, 0);

    return gIndex;
}

QModelIndex KKSTemplateEditor :: findModelIndex(int idAttrGroup, const QModelIndex & parent) const
{
    QModelIndex gIndex;
    QModelIndexList mList;
    
    if(parent.isValid()){
        mList = tvGroups->model()->match(parent, Qt::UserRole, idAttrGroup, 2, Qt::MatchExactly | Qt::MatchRecursive);
    }
    else{
        QModelIndex startIndex = tvGroups->model()->sibling(0, 0, QModelIndex());
        mList = tvGroups->model()->match(startIndex, Qt::UserRole, idAttrGroup, 2, Qt::MatchExactly | Qt::MatchRecursive);
    }

    if(mList.isEmpty())
        return gIndex;

    for(int i=0; i<mList.count(); i++){
        QModelIndex indx = mList.at(i);
        int eType = indx.data(Qt::UserRole+USER_ENTITY).toInt();
        if(eType == 1){//1 - группа
            gIndex = indx;
            break;
        }
    }

    return gIndex;
}

QModelIndex KKSTemplateEditor :: findModelIndex1(int idAttr, const QModelIndex & parent) const
{
    QModelIndex aIndex;
    QModelIndexList mList;
    
    if(parent.isValid()){
        mList = tvGroups->model()->match(parent, Qt::UserRole, idAttr, 2, Qt::MatchExactly | Qt::MatchRecursive);
    }
    else{
        QModelIndex startIndex = tvGroups->model()->sibling(0, 0, QModelIndex());
        mList = tvGroups->model()->match(startIndex, Qt::UserRole, idAttr, 2, Qt::MatchExactly | Qt::MatchRecursive);
    }

    if(mList.isEmpty())
        return aIndex;

    for(int i=0; i<mList.count(); i++){
        QModelIndex indx = mList.at(i);
        int eType = indx.data(Qt::UserRole+USER_ENTITY).toInt();
        if(eType == 0){//0 - атрибут
            aIndex = indx;
            break;
        }
    }

    return aIndex;
}

QModelIndex KKSTemplateEditor :: getSelectedIndex (void) const
{
    if (!tRef)
        return QModelIndex();

    QItemSelectionModel *selModel = tvGroups->selectionModel ();
    if (!selModel)
        return QModelIndex ();

    QItemSelection sel = selModel->selection();
    if (sel.indexes().isEmpty())
        return QModelIndex ();
    QModelIndex wIndex = sel.indexes ().at (0);// (selModel->currentIndex ();
    return wIndex.sibling (wIndex.row(), 0);
}

void KKSTemplateEditor :: swapAttrs (KKSAttrView *aView, KKSAttrView *aViewPrev, const QModelIndex& gIndex, const QModelIndex& gIndexP)
{
    int worder = aView->order ();
    aView->setOrder (aViewPrev->order());
    aViewPrev->setOrder (worder);

    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 0), aView->title(), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 0), aView->id(), Qt::UserRole);
    
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 1), aView->defValue().valueVariant(), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 1), aView->defValue().value(), Qt::UserRole);
    
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 2), aView->isReadOnly() ? QObject::tr("Yes") : QObject::tr ("No"), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 2), aView->isReadOnly(), Qt::UserRole);

    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 3), aView->isMandatory() ? tr("Yes") : tr ("No"), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 3), aView->isMandatory(), Qt::UserRole);

    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 4), aView->order(), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 4), aView->order(), Qt::UserRole);

    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 0), aViewPrev->title(), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 0), aViewPrev->id(), Qt::UserRole);

    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 1), aViewPrev->defValue().valueVariant(), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 1), aViewPrev->defValue().value(), Qt::UserRole);
    
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 2), aViewPrev->isReadOnly() ? QObject::tr("Yes") : QObject::tr ("No"), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 2), aViewPrev->isReadOnly(), Qt::UserRole);

    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 3), aViewPrev->isMandatory() ? tr("Yes") : tr ("No"), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 3), aViewPrev->isMandatory(), Qt::UserRole);


    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 4), aViewPrev->order(), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 4), aViewPrev->order(), Qt::UserRole);

    QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
    QItemSelection newSel = QItemSelection (gIndexP.sibling (gIndexP.row(), 0), gIndexP.sibling(gIndexP.row(), gIndexP.model()->columnCount (gIndexP.parent())-1));
    this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
    this->tvGroups->selectionModel ()->setCurrentIndex (gIndexP, QItemSelectionModel::Select);
    this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
}

void KKSTemplateEditor :: setModel (QAbstractItemModel *model)
{
    QAbstractItemModel *oldModel = this->tvGroups->model ();
    this->tvGroups->setModel (model);
    if (oldModel)
        delete oldModel;
}

void KKSTemplateEditor :: setItemDelegate (QAbstractItemDelegate *iDeleg)
{
    QAbstractItemDelegate *oldDeleg = this->tvGroups->itemDelegate ();
    this->tvGroups->setItemDelegate (iDeleg);
    if (oldDeleg)
        delete oldDeleg;
}

void KKSTemplateEditor :: saveErr (void)
{
    dbErr = true;
}

void KKSTemplateEditor :: dropErr (void)
{
    dbErr = false;
}

void KKSTemplateEditor :: itemExpanded(const QModelIndex & item)
{
    if(!item.isValid())
        return;

    int id = item.data(Qt::UserRole).toInt(); //id or group or attrview
    KKSAttrGroup * g = tRef->searchGroupById(id);
    if(!g)
        return;
    g->setExpanded(true);
}

void KKSTemplateEditor :: itemCollapsed(const QModelIndex & item)
{
    if(!item.isValid())
        return;

    int id = item.data(Qt::UserRole).toInt(); //id or group or attrview
    KKSAttrGroup * g = tRef->searchGroupById(id);
    if(!g)
        return;
    g->setExpanded(false);
}


