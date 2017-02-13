#include "kkscmdjournalitem.h"
#include "kkscmdjournallv.h"

#include <QtDebug>
#include <QPainter>
#include <QModelIndex>
#include <QFont>
#include <QFontMetrics>
#include <math.h>

QPixmap * pxIn = NULL;
QPixmap * pxOut = NULL;
QPixmap * pxInAll = NULL;
QPixmap * pxOutAll = NULL;
QPixmap * pxArchived = NULL;

QPixmap KKSCommand::pixmap(KKSCommand::CmdType type)
{
    QPixmap px;
    
    switch(type){
        case KKSCommand::ctIn:
            px = QPixmap(*pxIn);
            break;
        case KKSCommand::ctOut:
            px = QPixmap(*pxOut);
            break;
        case KKSCommand::ctAllIn:
            px = QPixmap(*pxInAll);
            break;
        case KKSCommand::ctAllOut:
            px = QPixmap(*pxOutAll);
            break;
    }
    
    return px;
}

QPixmap KKSCommand::archivedPixmap()
{
    QPixmap px = QPixmap(*pxArchived);
    return px;
}

void KKSCommand::setPxIn(const QPixmap & px)
{
    if(pxIn)
        delete pxIn;

    pxIn = new QPixmap(px);
}

void KKSCommand::setPxInAll(const QPixmap & px)
{
    if(pxInAll)
        delete pxInAll;

    pxInAll = new QPixmap(px);
}

void KKSCommand::setPxOut(const QPixmap & px)
{
    if(pxOut)
        delete pxOut;

    pxOut = new QPixmap(px);
}

void KKSCommand::setPxOutAll(const QPixmap & px)
{
    if(pxOutAll)
        delete pxOutAll;

    pxOutAll = new QPixmap(px);
}

void KKSCommand::setPxArchived(const QPixmap & px)
{
    if(pxArchived)
        delete pxArchived;

    pxArchived = new QPixmap(px);
}

KKSCommand::~KKSCommand()
{

}

KKSCommand::KKSCommand()
{
    m_id = -1;
    m_categoryId = -1;
    m_ioObjectId = -1;
    m_cmdType = ctIn;
    m_cmdTypeEx = cteIn;
    m_cmdState = csPlanned;
    m_isArchived = false;
    m_realExec = QDateTime::currentDateTime();
    m_execDateTime = QDateTime::currentDateTime();
    m_insertTime = QDateTime::currentDateTime();
    m_receiveDateTime = QDateTime::currentDateTime();
    m_acceptedDateTime = QDateTime::currentDateTime();
    m_readDateTimeE = QDateTime::currentDateTime();
    m_readDateTimeC = QDateTime::currentDateTime();
}

KKSCommand::KKSCommand(const KKSCommand & in)
{
    m_categoryId = in.m_categoryId;
    m_ioObjectId = in.m_ioObjectId;
    m_categoryName = in.m_categoryName;             
    m_cmdState = in.m_cmdState;
    m_cmdStateName = in.m_cmdStateName;
    m_cmdType = in.m_cmdType;   
    m_cmdTypeEx = in.m_cmdTypeEx;
    m_dlExecutor = in.m_dlExecutor;
    m_dlFrom = in.m_dlFrom;
    m_dlTo = in.m_dlTo;
    m_execDateTime = in.m_execDateTime;
    m_acceptedDateTime = in.m_acceptedDateTime;
    m_readDateTimeE = in.m_readDateTimeE;
    m_readDateTimeC = in.m_readDateTimeC;
    m_receiveDateTime = in.m_receiveDateTime;
    m_execPeriod = in.m_execPeriod;
    m_realExec = in.m_realExec;
    m_id = in.m_id;
    m_insertTime = in.m_insertTime;
    m_isArchived = in.m_isArchived;
    m_messageBody = in.m_messageBody;
}

KKSCommand & KKSCommand::operator = ( const KKSCommand & in )
{
    m_categoryId = in.m_categoryId;  
    m_ioObjectId = in.m_ioObjectId;
    m_categoryName = in.m_categoryName;             
    m_cmdState = in.m_cmdState;      
    m_cmdStateName = in.m_cmdStateName;
    m_cmdType = in.m_cmdType;   
    m_cmdTypeEx = in.m_cmdTypeEx;
    m_dlExecutor = in.m_dlExecutor;
    m_dlFrom = in.m_dlFrom;
    m_dlTo = in.m_dlTo;
    m_execDateTime = in.m_execDateTime;
    m_receiveDateTime = in.m_receiveDateTime;
    m_acceptedDateTime = in.m_acceptedDateTime;
    m_readDateTimeE = in.m_readDateTimeE;
    m_readDateTimeC = in.m_readDateTimeC;
    m_realExec = in.m_realExec;
    m_execPeriod = in.m_execPeriod;
    m_id = in.m_id;
    m_insertTime = in.m_insertTime;
    m_isArchived = in.m_isArchived;
    m_messageBody = in.m_messageBody;

    return * this;
}

bool KKSCommand::operator == ( const KKSCommand & in ) const
{
    if(m_categoryId != in.m_categoryId)
        return false;
    if(m_ioObjectId != in.m_ioObjectId)
        return false;
    if(m_categoryName != in.m_categoryName)
        return false;
    if(m_cmdState != in.m_cmdState)
        return false;
    if(m_cmdStateName != in.m_cmdStateName)
        return false;
    if(m_cmdType != in.m_cmdType)
        return false;
    if(m_cmdTypeEx != in.m_cmdTypeEx)
        return false;
    if(m_dlExecutor != in.m_dlExecutor)
        return false;
    if(m_dlFrom != in.m_dlFrom)
        return false;
    if(m_dlTo != in.m_dlTo)
        return false;
    if(m_execDateTime != in.m_execDateTime)
        return false;
    if(m_acceptedDateTime != in.m_acceptedDateTime)
        return false;
    if(m_readDateTimeE != in.m_readDateTimeE)
        return false;
    if(m_readDateTimeC != in.m_readDateTimeC)
        return false;
    if(m_receiveDateTime != in.m_receiveDateTime)
        return false;
    if(m_realExec != in.m_realExec)
        return false;
    if(m_execPeriod != in.m_execPeriod)
        return false;
    if(m_id != in.m_id)
        return false;
    if(m_insertTime != in.m_insertTime)
        return false;
    if(m_isArchived != in.m_isArchived)
        return false;
    if(m_messageBody != in.m_messageBody)
        return false;

    return true;
}

bool KKSCommand::operator != ( const KKSCommand & in ) const
{
    if(! (*this == in) )
        return true;

    return false;
}

double KKSCommand::timePercentEstimated() const
{
    double percent = 100.0; 

    QDateTime now = QDateTime::currentDateTime();
    
    
    double secsFromNowToEnd = (double)(now.secsTo(m_execDateTime));
    double secsFromInsertToEnd = (double)(m_insertTime.secsTo(m_execDateTime));

    if(secsFromInsertToEnd == 0)
        percent = 0.0;
    else if(secsFromNowToEnd < 0)
        percent = 0.0;
    else{
        percent = ( fabs(secsFromNowToEnd) / fabs(secsFromInsertToEnd) ) * 100.0;
    }
	
    return percent;
}

int KKSCommand::id() const
{
    return m_id;
}

void KKSCommand::setId(int _id)
{
    m_id = _id;
}

const QString & KKSCommand::dlFrom() const
{
    return m_dlFrom;
}

void KKSCommand::setDlFrom(const QString & v)
{
    m_dlFrom = v;
}

const QString & KKSCommand::dlExecutor() const
{
    return m_dlExecutor;
}

void KKSCommand::setDlExecutor(const QString & v)
{
    m_dlExecutor = v;
}

const QString & KKSCommand::dlTo() const
{
    return m_dlTo;
}

void KKSCommand::setDlTo(const QString & v)
{
    m_dlTo = v;
}

const QString & KKSCommand::categoryName() const
{
    return m_categoryName;
}

void KKSCommand::setCategoryName(const QString & v)
{
    m_categoryName = v;
}

int KKSCommand::categoryId() const
{
    return m_categoryId;
}

void KKSCommand::setCategoryId(int _id)
{
    m_categoryId = _id;
}

int KKSCommand::ioObjectId() const
{
    return m_ioObjectId;
}

void KKSCommand::setIoObjectId(int _id)
{
    m_ioObjectId = _id;
}

KKSCommand::CmdType KKSCommand::cmdType() const
{
    return m_cmdType;
}

void KKSCommand::setCmdType(CmdType _cmdType)
{
    m_cmdType = _cmdType;
}

KKSCommand::CmdTypeEx KKSCommand::cmdTypeEx() const
{
    return m_cmdTypeEx;
}

void KKSCommand::setCmdTypeEx(CmdTypeEx _cmdTypeEx)
{
    m_cmdTypeEx = _cmdTypeEx;
}

KKSCommand::CmdState KKSCommand::cmdState() const
{
    return m_cmdState;
}

void KKSCommand::setCmdState(CmdState v)
{
    m_cmdState = v;
}

const QString & KKSCommand::cmdStateName() const
{
    return m_cmdStateName;
}

void KKSCommand::setCmdStateName(const QString & v)
{
    m_cmdStateName = v;
}


const QDateTime & KKSCommand::insertTime() const
{
    return m_insertTime;
}

void KKSCommand::setInsertTime(const QDateTime & v)
{
    m_insertTime = v;
}

const QDateTime & KKSCommand::execDateTime() const
{
    return m_execDateTime;
}

void KKSCommand::setExecDateTime(const QDateTime & v)
{
    m_execDateTime = v;
}

const QDateTime & KKSCommand::acceptedDateTime() const
{
    return m_acceptedDateTime;
}

void KKSCommand::setAcceptedDateTime(const QDateTime & v)
{
    m_acceptedDateTime = v;
}

const QDateTime & KKSCommand::readDateTimeE() const
{
    return m_readDateTimeE;
}

void KKSCommand::setReadDateTimeE(const QDateTime & v)
{
    m_readDateTimeE = v;
}

const QDateTime & KKSCommand::readDateTimeC() const
{
    return m_readDateTimeC;
}

void KKSCommand::setReadDateTimeC(const QDateTime & v)
{
    m_readDateTimeC = v;
}

const QDateTime & KKSCommand::receiveDateTime() const
{
    return m_receiveDateTime;
}

void KKSCommand::setReceiveDateTime(const QDateTime & v)
{
    m_receiveDateTime = v;
}

const QDateTime & KKSCommand::realExec() const
{
    return m_realExec;
}

void KKSCommand::setRealExec(const QDateTime & v)
{
    m_realExec = v;
}

const QString & KKSCommand::execPeriod() const
{
    return m_execPeriod;
}

void KKSCommand::setExecPeriod(const QString & v)
{
    m_execPeriod = v;
}

const QString & KKSCommand::messageBody() const
{
    return m_messageBody;
}

void KKSCommand::setMessageBody(const QString & v)
{
    m_messageBody = v;
}

bool KKSCommand::isArchived() const
{
    return m_isArchived;
}
void KKSCommand::setIsArchived(bool _is)
{
    m_isArchived = _is;
}


KKSCmdJournalItemDelegate :: KKSCmdJournalItemDelegate (QObject * parent) :
    QItemDelegate (parent)
{
}

void KKSCmdJournalItemDelegate :: paint(QPainter *painter, 
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    painter->save();

    QFont font;
    QColor color;

    if(!index.isValid())
        return;
    if(index.data().isNull())
        return;
    if(!index.data().isValid())
        return;

    KKSCmdJournalItemData cmdItemData = index.data().value<KKSCmdJournalItemData>();
        // 
        // qVariantValue<KKSCmdJournalItemData> (index.data());
        // deprecated in qt5

    
    KKSCommand cmd = cmdItemData.getCommand();
    
    if(cmd.cmdState() == KKSCommand::csExecuted){
        color = Qt::white;
    }
    else{
        double percent = cmd.timePercentEstimated();
        if( percent >= 90.0 ){
            color = Qt::white;
        }
        else if(percent >= 50.0 && percent < 90.0){
            color = Qt::darkGreen;
        }
        else if(percent >= 20.0 && percent < 50.0){
            color = Qt::darkYellow;
        }
        else{
            color = Qt::darkRed;
        }
    }
    
    QBrush brush(color);
    painter->setBrush(brush);
    painter->setPen(Qt::NoPen);
    painter->drawRect(option.rect);

    switch ( cmd.cmdState() ){
        case KKSCommand::csPlanned:
            font = QFont("Calibri", 10);
            color = Qt::black;
            break;
        case KKSCommand::csOuted:
            font = QFont("Calibri", 10);
            color = Qt::black;
            break;
        case KKSCommand::csExecuting:
            font = QFont("Calibri", 10);
            color = Qt::darkBlue; //QColor(244, 247, 213);
            break;
        case KKSCommand::csExecuted:
            font = QFont("Calibri", 10);
            color = Qt::darkGreen; //QColor(209, 231, 207);//green
            break;
        case KKSCommand::csTimeElapsed:
            font = QFont("Calibri", 10);
            color = Qt::black;//QColor(245, 231, 235);//red
            break;
        default:
            font = QFont("Calibri", 10);
            color = Qt::black;
            break;
    }

    painter->setFont(font);
    painter->setPen(color);

    if (option.state & QStyle::State_Selected){
        painter->setBrush(option.palette.highlight());
        painter->drawRect(option.rect);
        painter->setPen(option.palette.highlightedText().color());
    }

    int column = index.column();
    cmdItemData.drawItem(painter, option.rect, column);
   
    painter->restore();
}

QSize KKSCmdJournalItemDelegate :: sizeHint(const QStyleOptionViewItem& option,
                                            const QModelIndex& index ) const
{
    QSize ws = option.rect.size ();
    if (!index.isValid())
        return QSize ();

    if (index.data().isNull())
        return QSize ();

    KKSCmdJournalItemData cmdItemData = index.data().value<KKSCmdJournalItemData>();

    KKSCommand m_cmd = cmdItemData.getCommand();
    QString text;// = index.data (Qt::DisplayRole).toString ();
    QPixmap px;

    switch(index.column())
    {
    case 0:
        text = QString::number(m_cmd.id());
        if(m_cmd.isArchived())
            px = KKSCommand::archivedPixmap();
        else
            px = KKSCommand::pixmap(m_cmd.cmdType());
        break;
    case 1:
        text = m_cmd.messageBody();
        break;
    case 2:
        text = m_cmd.categoryName();
        break;
    case 3:
        text = m_cmd.dlFrom();
        break;
    case 4:
        text = m_cmd.dlTo();
        break;
    case 5:
        text = m_cmd.dlExecutor();
        break;
    case 6:
        text = m_cmd.execDateTime().toString("dd.MM.yyyy h:mm");//planned_exec
        break;
    case 7:
        text = m_cmd.insertTime().toString("dd.MM.yyyy h:mm");
        break;
    case 8: 
        text = m_cmd.execPeriod();
        break;
    case 9:
        text = (m_cmd.receiveDateTime().isValid() == TRUE) ? m_cmd.receiveDateTime().toString("dd.MM.yyyy h:mm") : QString("");
        if(text.isEmpty()){
            text = QObject::tr("Not received yet!");
        }
        break;
    case 10:
        text = (m_cmd.acceptedDateTime().isValid() == TRUE) ? m_cmd.acceptedDateTime().toString("dd.MM.yyyy h:mm") : QString("");
        if(text.isEmpty()){
            if(m_cmd.cmdType() == KKSCommand::ctIn)
                text = QObject::tr("Mark as accepted!");
            else
                text = QObject::tr("Not accepted yet!");
        }
        break;
    case 11:
        text = (m_cmd.realExec().isValid() == TRUE) ? m_cmd.realExec().toString("dd.MM.yyyy h:mm") : QString("");
        break;
    case 12:
        if(m_cmd.isArchived())
            text = QObject::tr("Archived");
        else
            text = m_cmd.cmdStateName();
        break;
    default:
        text = QObject::tr("Not defined");
    }

    //int height = px.height ();

    QStringList wstrL = text.split ("\n");
    QFont fn = option.font;
    QFontMetrics fm (fn);
    QString str_max = wstrL[0];
    for (int i=1; i<wstrL.size(); i++)
        if (wstrL[i].size () > str_max.size())
            str_max = wstrL[i];
    int width = qMax (fm.width (str_max), 30);
    ws.rwidth () += width;
    ws.rheight () += wstrL.size()*qMax (fm.height(), 19);

    return ws;
}

//===========================================
//===========================================
//===========================================
void KKSCmdJournalItemData::drawItem( QPainter * painter, 
                                      QRect rect, 
                                      int column)//, 
                                      //bool bFavourites)
{
    painter->setClipRect(rect);
    
    QString text;
    int flags = Qt::AlignVCenter;
    QRect bound = rect;
    
    QPixmap px;
    QRect px_rect;
    int shift = 0;
    switch(column)
    {
    case 0:
        text = QString::number(m_cmd.id());
        if(m_cmd.isArchived())
            px = KKSCommand::archivedPixmap();
        else
            px = KKSCommand::pixmap(m_cmd.cmdType());
        px_rect = QRect(rect.left()+3, rect.top(), 16+3, 16);
        
        if(!px.isNull())
            shift = 16;
        bound = QRect(rect.left()+5+shift, rect.top(), rect.width()-5-shift, rect.height());
        flags |= Qt::AlignHCenter;
        break;
    case 1:
        text = m_cmd.messageBody();
        bound = QRect(rect.left()+5, rect.top(), rect.width()-5, rect.height());
        break;
    case 2:
        text = m_cmd.categoryName();
        bound = QRect(rect.left()+5, rect.top(), rect.width()-5, rect.height());
        break;
    case 3:
        text = m_cmd.dlFrom();
        bound = QRect(rect.left()+5, rect.top(), rect.width()-5, rect.height());
        break;
    case 4:
        text = m_cmd.dlTo();
        bound = QRect(rect.left()+5, rect.top(), rect.width()-5, rect.height());
        break;
    case 5:
        text = m_cmd.dlExecutor();
        bound = QRect(rect.left()+5, rect.top(), rect.width()-5, rect.height());
        break;
    case 6:
        text = m_cmd.execDateTime().toString("dd.MM.yyyy h:mm");
        flags |= Qt::AlignHCenter;
        break;
    case 7:
        text = m_cmd.insertTime().toString("dd.MM.yyyy h:mm");
        flags |= Qt::AlignHCenter;
        break;
    case 8: 
        text = m_cmd.execPeriod();
        flags |= Qt::AlignHCenter;
        break;
    case 9:
        text = (m_cmd.receiveDateTime().isValid() == TRUE) ? m_cmd.receiveDateTime().toString("dd.MM.yyyy h:mm") : QString("");
        flags |= Qt::AlignHCenter;
        break;
    case 10:
        text = (m_cmd.acceptedDateTime().isValid() == TRUE) ? m_cmd.acceptedDateTime().toString("dd.MM.yyyy h:mm") : QString("");
        flags |= Qt::AlignHCenter;
        break;
    case 11:
        text = (m_cmd.realExec().isValid() == TRUE) ? m_cmd.realExec().toString("dd.MM.yyyy h:mm") : QString("");
        flags |= Qt::AlignHCenter;
        break;
    case 12:
        if(m_cmd.isArchived())
            text = QObject::tr("Archived");
        else
            text = m_cmd.cmdStateName();
        flags |= Qt::AlignHCenter;
        break;
    default:
        text = QObject::tr("Not defined");
    }

    if(!px.isNull()){
        painter->drawPixmap(px_rect, px);
    }

    painter->drawText(bound, flags, text);
    
    QPen pen;
    
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(1);
    pen.setColor(Qt::gray);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    painter->drawLine(rect.topLeft(), rect.bottomLeft());
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    if(column == 12)
        painter->drawLine(rect.topRight(), rect.bottomRight());
}

const KKSCommand & KKSCmdJournalItemData::getCommand( ) const
{
    return m_cmd;
}

KKSCommand & KKSCmdJournalItemData::getCommand( )
{
    return m_cmd;
}

void KKSCmdJournalItemData::setCommand( const KKSCommand & cmd )
{
    m_cmd = cmd;
}

KKSCmdJournalItemData::KKSCmdJournalItemData(const KKSCmdJournalModel * parent, int row)
{
    m_pParent = parent;
    m_row = row;
}

KKSCmdJournalItemData::KKSCmdJournalItemData( const KKSCmdJournalItemData & item )
{
    m_pParent = item.m_pParent;
    m_row = item.m_row;

    const KKSCommand cmd = item.getCommand();
    setCommand( cmd );
}

KKSCmdJournalItemData::KKSCmdJournalItemData( const KKSCmdJournalModel * parent, 
                                              int row, 
                                              const KKSCommand & cmd)
{
    m_pParent = parent;
    m_row = row;
    setCommand( cmd );
}

KKSCmdJournalItemData::~ KKSCmdJournalItemData( )
{
}
