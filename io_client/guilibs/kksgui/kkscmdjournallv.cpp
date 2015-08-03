#include <QHeaderView>
#include <QMessageBox>
#include <QEvent>
#include <QHelpEvent>
#include <QToolTip>
#include <QModelIndex>
#include <QMenu>
#include <QtDebug>

#include "kkscmdjournallv.h"
//#include "fblvurlsfilter.h"

#define NCOLS_COUNT 13

KKSCmdJournalLv::KKSCmdJournalLv(//const FBSettings * pSettings, 
                                 //QString phrase, 
                                 //bool bFavourites, 
                                 QWidget * parent)
:QTreeView(parent)
{
    
    KKSCmdJournalModel * model = new KKSCmdJournalModel(//pSettings, 
                                                        //bFavourites, 
                                                        this);
    setModel(model);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    //m_bFavourites = bFavourites;
    m_pMenu = NULL; 

    KKSCmdJournalItemDelegate * delegate = new KKSCmdJournalItemDelegate();
    setItemDelegate(delegate);

    setRootIsDecorated(false);
    
    QHeaderView * hv = new QHeaderView(Qt::Horizontal);
    setHeader(hv);
    hv->resizeSection(0, 50);//No
    hv->resizeSection(1, 300);//message_body
    hv->resizeSection(2, 215);//category_name
    hv->resizeSection(3, 160);//sender (org+dl)
    hv->resizeSection(4, 160);//receiver (org+dl)
    hv->resizeSection(5, 160);//executor (org+dl)
    hv->resizeSection(6, 120);//planned_exec (datetime)
    hv->resizeSection(7, 120);//insert_datetime
    hv->resizeSection(8, 120);//periodical
    hv->resizeSection(9, 120);//received_datetime
    hv->resizeSection(10, 140);//accepted_datetime
    hv->resizeSection(11, 120);//exec_datetime
    hv->resizeSection(12, 150);//id_state_name
    
    hv->setSortIndicator(1, Qt::AscendingOrder);
    hv->setSortIndicatorShown(true);
    hv->setClickable(true);

    connect(hv, SIGNAL(sectionClicked(int)), this, SLOT(sortByColumnEx(int)));
    /*disconnect(model,
               SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), 
               this,
               SLOT(dataChanged(const QModelIndex&, const QModelIndex&)));
*/
  /*  
    connect(model, 
            SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), 
            this, 
            SLOT(slotDataChanged(const QModelIndex&, const QModelIndex&)));
*/
    //filter = new FBLvUrlsFilter();
    
    //this->phrase = phrase;
    
   // setColumnHidden(0, !bFavourites);

    setMouseTracking(true);
    //QItemSelectionModel * sm = selectionModel();
    //connect(sm, 
    //        SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), 
    //        this, 
    //        SLOT(slotSelectionChanged(const QItemSelection &, const QItemSelection &))
    //        );

    //connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(slotActivated(const QModelIndex&)));
}

KKSCmdJournalLv::~KKSCmdJournalLv()
{
    //if(filter)
    //    delete filter;
}

void KKSCmdJournalLv::addCommand(const KKSCommand & cmd)
{
    KKSCmdJournalItemData * data = new KKSCmdJournalItemData(NULL, -1, cmd);
    KKSCmdJournalModel * mdl = (KKSCmdJournalModel *) model();
    if(!mdl->insertRows(mdl->rowCount(), 1))
        return;

    QModelIndex mdlIndex = mdl->index(mdl->rowCount()-1, 0);

    data->setParent(mdl, mdlIndex.row());
    QVariant value = qVariantFromValue (*data);
    mdl->setData(mdlIndex, value);
}

void KKSCmdJournalLv::removeCommand(const KKSCommand & cmd)
{
    KKSCmdJournalModel * mdl = (KKSCmdJournalModel *) model();

    int row = 0;
    while(row < rowCount()){
        QModelIndex index = mdl->index(row, 0);
        KKSCmdJournalItemData itemData = qVariantValue<KKSCmdJournalItemData>(index.data());
        if(itemData.getCommand() == cmd)
            mdl->removeRows(row, 1);
        else
            row++;
    }
}

void KKSCmdJournalLv::removeCommand(const QModelIndex & index)
{
    if(!index.isValid())
        return;

    KKSCmdJournalModel * mdl = (KKSCmdJournalModel *) model();
    int row = index.row();
    mdl->removeRows(row, 1);
}

int KKSCmdJournalLv::rowCount() const
{
    return model()->rowCount();
}

bool KKSCmdJournalLv::isEmpty() const
{
    if(model()->rowCount() > 0)
        return false;

    return true;
}

KKSCommand KKSCmdJournalLv::currentCommand() const
{
    KKSCommand cmd;
    if(!model())
        return cmd;
    if(rowCount() <= 0)
        return cmd;

    QItemSelectionModel * sm = selectionModel();
    if(!sm)
        return cmd;

    QModelIndex index = sm->currentIndex();
    if(!index.isValid())
        return cmd;

    KKSCmdJournalItemData cmdItemData = qVariantValue<KKSCmdJournalItemData> (model()->data(index));
    cmd = cmdItemData.getCommand();
    
    return cmd;
}

void KKSCmdJournalLv::clear()
{
    int cnt = rowCount();
    if(cnt > 0)
        model()->removeRows(0, cnt);
}

void KKSCmdJournalLv::mouseMoveEvent(QMouseEvent * e)
{
    int column = columnAt(e->pos().x());
    if(column != 1)
        return;

    QModelIndex index = indexAt(e->pos());
    if(!index.isValid())
        return;

    KKSCmdJournalItemData cmdItemData = qVariantValue<KKSCmdJournalItemData> (model()->data(index));
    KKSCommand cmd = cmdItemData.getCommand();
    
    QToolTip::showText(e->globalPos(), cmd.messageBody());
}

void KKSCmdJournalLv::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
    if(m_pMenu){
        emit menuStarted();
        m_pMenu->exec(event->globalPos());
        emit menuStopped();
    }
}

void KKSCmdJournalLv::focusInEvent(QFocusEvent * e)
{
    QTreeView::focusInEvent(e);
    
    emit focusIn();
}

void KKSCmdJournalLv::focusOutEvent(QFocusEvent * e)
{
    QTreeView::focusOutEvent(e);
    
    emit focusOut();
}

void KKSCmdJournalLv::sortByColumnEx(int column)
{
    KKSCmdJournalModel * mdl = (KKSCmdJournalModel *) model();
    
    
    KKSCmdJournalModel::SortType type;
    switch (column){
        case 0: 
            type = KKSCmdJournalModel::byExecTime;
            break;
        case 1:
            type = KKSCmdJournalModel::byInsertTime;
            break;
        default:
            type = KKSCmdJournalModel::byExecTime;
            break;
    }

    mdl->sort(type, header()->sortIndicatorOrder() == Qt::AscendingOrder ? true : false);
    this->repaint();
}


void KKSCmdJournalLv::slotDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    /*this->dataChanged(topLeft, bottomRight);
    
    QRect rect1 = visualRect(topLeft);
    QRect rect2 = visualRect(bottomRight);
    QRect rect;
    rect.setTopLeft(rect1.topLeft());
    rect.setBottomRight(rect2.bottomRight());
    
    viewport()->repaint(rect);*/
}
/*
void KKSCmdJournalLv::slotSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{

}

void KKSCmdJournalLv::slotActivated(const QModelIndex& index)
{
    if(index.isValid()){
        setCurrentIndex(index);

        //selectionModel()->setCurrentIndex(index, QItemSelectionModel::Rows);
        //selectionModel()->select(index, QItemSelectionModel::Rows);
        //if(selectionModel()->isRowSelected(index.row(), QModelIndex())){
        //    int a=0;
        //}
        
        repaint();
    }
}
*/

//=======================================
//=======================================
//=======================================
KKSCmdJournalModel::KKSCmdJournalModel( //const FBSettings * pSettings, 
                                        const QList< KKSCmdJournalItemData * > &commands, 
                                        //bool bFavourites,
                                        QObject * parent )
    :QAbstractItemModel (parent), ncols(NCOLS_COUNT)
{
    //this->pSettings = pSettings;
    //m_bFavourites = bFavourites;
    
    initHeaderData();

    m_commands.clear();
    m_commands = QList<KKSCmdJournalItemData *>(commands);
}

KKSCmdJournalModel::KKSCmdJournalModel( //const FBSettings * pSettings, 
                                        //bool bFavourites,
                                        QObject * parent )
    :QAbstractItemModel (parent), ncols(NCOLS_COUNT)
{
    //this->pSettings = pSettings;

    initHeaderData();
}

void KKSCmdJournalModel::initHeaderData()
{
    header << tr("serial number")
           << tr("Message body")
           << tr("Category")
           << tr("Dl from")
           << tr("Dl to")
           << tr("Dl executor")
           << tr("Exec date/time (planned)") 
           << tr("Inserted at") 
           << tr("Period") 
           << tr("Receive datetime") 
           << tr("Accepted datetime") 
           << tr("Real exec datetime")
           << tr("State");
}

KKSCmdJournalModel::~KKSCmdJournalModel ( )
{
}

QVariant KKSCmdJournalModel :: data ( const QModelIndex & index, int role ) const
{
    int i=index.row();

    if ( index.isValid() && role == Qt::DisplayRole && i<m_commands.count() )
    {
        if (!m_commands.at(i))
            return QVariant();
        
        QVariant cmd = qVariantFromValue (*(m_commands.at(i)));
        return cmd;
    }
    else
    {
        return QVariant ();
    }
}

int KKSCmdJournalModel :: rowCount ( const QModelIndex & parent ) const
{
    int nrows = m_commands.count();

    if (parent.isValid())
        return parent.row();
    else
        return nrows;
}

int KKSCmdJournalModel :: columnCount ( const QModelIndex & parent ) const
{
    if (parent.isValid())
        return parent.column();
    else
        return ncols;
}

bool KKSCmdJournalModel :: insertRows ( int row, int count, const QModelIndex & parent /*= QModelIndex()*/ )
{
    beginInsertRows(parent, row, row+count-1);
    for (int i=0; i<count; i++)
    {
        KKSCmdJournalItemData * cmdItemData=0;
        m_commands.insert(row+i, cmdItemData);
    }
    endInsertRows();

    return true;
}

bool KKSCmdJournalModel :: removeRows ( int row, int count, const QModelIndex & parent /*= QModelIndex()*/ )
{
    int nrows = rowCount();
    if (nrows-count < 0)
        return false;
    else
    {
        beginRemoveRows(parent, row, row+count-1);
        nrows -= count;
        for (int i=0; i<count; i++)
        {
            KKSCmdJournalItemData * cmdItemData = m_commands.at (row);
            if (cmdItemData)
                delete cmdItemData;
            m_commands.removeAt(row);
        }
        endRemoveRows();
    }
    
    return true;
}

bool KKSCmdJournalModel :: setData ( const QModelIndex & index, const QVariant & value, int role )
{
    int i=index.row();
    if ( index.isValid() && role == Qt::EditRole && i>=0 && i<m_commands.count() )
    {
        KKSCmdJournalItemData cmdItemData = qVariantValue<KKSCmdJournalItemData> (value);
        KKSCmdJournalItemData *cmd = new KKSCmdJournalItemData( cmdItemData );
        m_commands.replace(i, cmd);

        emit dataChanged(index, index);

        return true;
    }
    else
        return false;
}

QModelIndex KKSCmdJournalModel::index(int row, int column, const QModelIndex &)
            const
{
    if(row < 0 || row >= m_commands.count())
        return QModelIndex();

    KKSCmdJournalItemData * data = m_commands[row];

    return createIndex(row, column, data);
}

QModelIndex KKSCmdJournalModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

Qt::ItemFlags KKSCmdJournalModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant KKSCmdJournalModel :: headerData (int section, 
                                           Qt::Orientation orientation,
                                           int role ) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(header.at(section));
    else
        return QVariant();
}


bool cmdSortAscending;
KKSCmdJournalModel::SortType cmdSortType;

bool compareCommands(KKSCmdJournalItemData * item1, KKSCmdJournalItemData * item2)
{
    
    switch(cmdSortType){
        case KKSCmdJournalModel::byExecTime :
            if(item1->getCommand().execDateTime() < item2->getCommand().execDateTime())
                return cmdSortAscending ? true : false;
            return cmdSortAscending ? false : true;
            break;
        case KKSCmdJournalModel::byInsertTime:
            if(item1->getCommand().insertTime() < item2->getCommand().insertTime())
                return cmdSortAscending ? true : false;
            return cmdSortAscending ? false : true;
            break;
    } 

    return cmdSortAscending ? true : false;
}

void KKSCmdJournalModel::sort(SortType type, bool asc)
{
    ::cmdSortAscending = asc;
    ::cmdSortType = type;
    qSort(m_commands.begin(), m_commands.end(), compareCommands);

    emit layoutChanged();
}

void KKSCmdJournalModel::slotDataChanged(int row)
{
    QModelIndex indexStart = index(row, 0);
    //QModelIndex indexEnd = index(row, 7);
    
    emit dataChanged(indexStart, indexStart);
    KKSCmdJournalItemData itemData = qVariantValue<KKSCmdJournalItemData>(indexStart.data());
    KKSCommand cmd = itemData.getCommand();
    emit dataChangedEx(cmd);
}
