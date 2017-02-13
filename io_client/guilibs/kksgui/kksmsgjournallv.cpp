#include <QHeaderView>
#include <QMessageBox>
#include <QEvent>
#include <QHelpEvent>
#include <QToolTip>
#include <QModelIndex>
#include <QMenu>

#include "kksmsgjournallv.h"
//#include "fblvurlsfilter.h"

#define NCOLS_COUNT 9

KKSMsgJournalLv::KKSMsgJournalLv(//const FBSettings * pSettings, 
                                 //QString phrase, 
                                 //bool bFavourites, 
                                 QWidget * parent)
:QTreeView(parent)
{
    
    KKSMsgJournalModel * model = new KKSMsgJournalModel(//pSettings, 
                                                        //bFavourites, 
                                                        this);
    setModel(model);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    //m_bFavourites = bFavourites;
    m_pMenu = NULL; 

    KKSMsgJournalItemDelegate * delegate = new KKSMsgJournalItemDelegate();
    setItemDelegate(delegate);

    setRootIsDecorated(false);
    
    QHeaderView * hv = new QHeaderView(Qt::Horizontal);
    setHeader(hv);
    hv->resizeSection(0, 50);//id
    hv->resizeSection(1, 300);//message
    hv->resizeSection(2, 300);//document
    hv->resizeSection(3, 150);//category
    hv->resizeSection(4, 150);//sender
    hv->resizeSection(5, 150);//receiver
    hv->resizeSection(6, 120);//sended_time
    hv->resizeSection(7, 120);//received_time
    hv->resizeSection(8, 120);//readed_time

    hv->setSortIndicator(1, Qt::AscendingOrder);
    hv->setSortIndicatorShown(true);
    hv->setClickable(true);

    connect(hv, SIGNAL(sectionClicked(int)), this, SLOT(sortByColumnEx(int)));

    setMouseTracking(true);
}

KKSMsgJournalLv::~KKSMsgJournalLv()
{
}

void KKSMsgJournalLv::addMessage(const KKSMessage & msg)
{
    KKSMsgJournalItemData * data = new KKSMsgJournalItemData(NULL, -1, msg);
    KKSMsgJournalModel * mdl = (KKSMsgJournalModel *) model();
    if(!mdl->insertRows(mdl->rowCount(), 1))
        return;

    QModelIndex mdlIndex = mdl->index(mdl->rowCount()-1, 0);

    data->setParent(mdl, mdlIndex.row());
    QVariant value = qVariantFromValue (*data);
    mdl->setData(mdlIndex, value);
}

void KKSMsgJournalLv::removeMessage(const KKSMessage & msg)
{
    KKSMsgJournalModel * mdl = (KKSMsgJournalModel *) model();

    int row = 0;
    while(row < rowCount()){
        QModelIndex index = mdl->index(row, 0);
        KKSMsgJournalItemData itemData = index.data().value<KKSMsgJournalItemData>();
        if(itemData.getMessage() == msg)
            mdl->removeRows(row, 1);
        else
            row++;
    }
}

void KKSMsgJournalLv::removeMessage(const QModelIndex & index)
{
    if(!index.isValid())
        return;

    KKSMsgJournalModel * mdl = (KKSMsgJournalModel *) model();
    int row = index.row();
    mdl->removeRows(row, 1);
}

int KKSMsgJournalLv::rowCount() const
{
    return model()->rowCount();
}

bool KKSMsgJournalLv::isEmpty() const
{
    if(model()->rowCount() > 0)
        return false;

    return true;
}

KKSMessage KKSMsgJournalLv::currentMessage() const
{
    KKSMessage msg;
    if(!model())
        return msg;
    if(rowCount() <= 0)
        return msg;

    QItemSelectionModel * sm = selectionModel();
    if(!sm)
        return msg;

    QModelIndex index = sm->currentIndex();
    if(!index.isValid())
        return msg;

    KKSMsgJournalItemData msgItemData = model()->data(index).value<KKSMsgJournalItemData> ();
    msg = msgItemData.getMessage();
    
    return msg;
}

void KKSMsgJournalLv::clear()
{
    int cnt = rowCount();
    if(cnt > 0)
        model()->removeRows(0, cnt);
}

void KKSMsgJournalLv::mouseMoveEvent(QMouseEvent * e)
{
    int column = columnAt(e->pos().x());
    if(column != 1)
        return;

    QModelIndex index = indexAt(e->pos());
    if(!index.isValid())
        return;

    KKSMsgJournalItemData msgItemData = model()->data(index).value<KKSMsgJournalItemData> ();
    KKSMessage msg = msgItemData.getMessage();
    
    QToolTip::showText(e->globalPos(), msg.messageBody());
}

void KKSMsgJournalLv::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
    if(m_pMenu){
        emit menuStarted();
        m_pMenu->exec(event->globalPos());
        emit menuStopped();
    }
}

void KKSMsgJournalLv::focusInEvent(QFocusEvent * e)
{
    QTreeView::focusInEvent(e);
    
    emit focusIn();
}

void KKSMsgJournalLv::focusOutEvent(QFocusEvent * e)
{
    QTreeView::focusOutEvent(e);
    
    emit focusOut();
}

void KKSMsgJournalLv::sortByColumnEx(int column)
{
    KKSMsgJournalModel * mdl = (KKSMsgJournalModel *) model();
    
    
    KKSMsgJournalModel::SortType type;
    switch (column){
        case 0: 
            type = KKSMsgJournalModel::bySentTime;
            break;
        case 1:
            type = KKSMsgJournalModel::byReadTime;
            break;
        default:
            type = KKSMsgJournalModel::bySentTime;
            break;
    }

    mdl->sort(type, header()->sortIndicatorOrder() == Qt::AscendingOrder ? true : false);
    this->repaint();
}


void KKSMsgJournalLv::slotDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
}

//=======================================
//=======================================
//=======================================
KKSMsgJournalModel::KKSMsgJournalModel( //const FBSettings * pSettings, 
                                        const QList< KKSMsgJournalItemData * > &messages, 
                                        //bool bFavourites,
                                        QObject * parent )
    :QAbstractItemModel (parent), ncols(NCOLS_COUNT)
{
    initHeaderData();

    m_messages.clear();
    m_messages = QList<KKSMsgJournalItemData *>(messages);
}

KKSMsgJournalModel::KKSMsgJournalModel( //const FBSettings * pSettings, 
                                        //bool bFavourites,
                                        QObject * parent )
    :QAbstractItemModel (parent), ncols(NCOLS_COUNT)
{
    //this->pSettings = pSettings;

    initHeaderData();
}

void KKSMsgJournalModel::initHeaderData()
{
    header << tr("serial number")
           << tr("Message body")
           << tr("Attached doc")
           << tr("Category")
           << tr("Dl from")
           << tr("Dl to")
           << tr("Sent date/time") 
           << tr("Receive date/time") 
           << tr("Read date/time");
}

KKSMsgJournalModel::~KKSMsgJournalModel ( )
{
}

QVariant KKSMsgJournalModel :: data ( const QModelIndex & index, int role ) const
{
    int i=index.row();
    if ( index.isValid() && role == Qt::DisplayRole && i<m_messages.count() )
    {
        QVariant msg = qVariantFromValue (*(m_messages.at(i)));
        return msg;
    }
    else
    {
        return QVariant();
    }
}

int KKSMsgJournalModel :: rowCount ( const QModelIndex & parent ) const
{
    int nrows = m_messages.count();

    if (parent.isValid())
        return parent.row();
    else
        return nrows;
}

int KKSMsgJournalModel :: columnCount ( const QModelIndex & parent ) const
{
    if (parent.isValid())
        return parent.column();
    else
        return ncols;
}

bool KKSMsgJournalModel :: insertRows ( int row, int count, const QModelIndex & parent /*= QModelIndex()*/ )
{
    beginInsertRows(parent, row, row+count-1);
    for (int i=0; i<count; i++)
    {
        KKSMsgJournalItemData * msgItemData=0;
        m_messages.insert(row+i, msgItemData);
    }
    endInsertRows();

    return true;
}

bool KKSMsgJournalModel :: removeRows ( int row, int count, const QModelIndex & parent /*= QModelIndex()*/ )
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
            KKSMsgJournalItemData * msgItemData=m_messages.at (row);
            if (msgItemData)
                delete msgItemData;
            m_messages.removeAt(row);
        }
        endRemoveRows();
    }
    
    return true;
}

bool KKSMsgJournalModel :: setData ( const QModelIndex & index, const QVariant & val, int role )
{
    int i=index.row();
    if ( index.isValid() && role == Qt::EditRole && i>=0 && i<m_messages.count() )
    {
        KKSMsgJournalItemData msgItemData = val.value<KKSMsgJournalItemData> ();
        KKSMsgJournalItemData *msg = new KKSMsgJournalItemData( msgItemData );
        m_messages.replace(i, msg);

        emit dataChanged(index, index);

        return true;
    }
    else
        return false;
}

QModelIndex KKSMsgJournalModel::index(int row, int column, const QModelIndex &)
            const
{
    if(row < 0 || row >= m_messages.count())
        return QModelIndex();

    KKSMsgJournalItemData * data = m_messages[row];

    return createIndex(row, column, data);
}

QModelIndex KKSMsgJournalModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

Qt::ItemFlags KKSMsgJournalModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant KKSMsgJournalModel :: headerData (int section, 
                                           Qt::Orientation orientation,
                                           int role ) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(header.at(section));
    else
        return QVariant();
}


bool msgSortAscending;
KKSMsgJournalModel::SortType msgSortType;

bool compareMessages(KKSMsgJournalItemData * item1, KKSMsgJournalItemData * item2)
{
    
    switch(msgSortType){
        case KKSMsgJournalModel::bySentTime :
            if(item1->getMessage().sentDateTime() < item2->getMessage().sentDateTime())
                return msgSortAscending ? true : false;
            return msgSortAscending ? false : true;
            break;
        case KKSMsgJournalModel::byReadTime:
            if(item1->getMessage().readDateTime() < item2->getMessage().readDateTime())
                return msgSortAscending ? true : false;
            return msgSortAscending ? false : true;
            break;
    } 

    return msgSortAscending ? true : false;
}

void KKSMsgJournalModel::sort(SortType type, bool asc)
{
    ::msgSortAscending = asc;
    ::msgSortType = type;
    qSort(m_messages.begin(), m_messages.end(), compareMessages);

    emit layoutChanged();
}

void KKSMsgJournalModel::slotDataChanged(int row)
{
    QModelIndex indexStart = index(row, 0);
    
    emit dataChanged(indexStart, indexStart);
    KKSMsgJournalItemData itemData = indexStart.data().value<KKSMsgJournalItemData>();
    KKSMessage msg = itemData.getMessage();
    emit dataChangedEx(msg);
}
