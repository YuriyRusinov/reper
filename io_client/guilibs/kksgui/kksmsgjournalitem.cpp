#include "kksmsgjournalitem.h"
#include "kksmsgjournallv.h"

#include <QtDebug>
#include <QPainter>
#include <QModelIndex>



QPixmap * pxMsgIn = NULL;
QPixmap * pxMsgOut = NULL;

QPixmap KKSMessage::pixmap(KKSMessage::MsgType type)
{
    QPixmap px;
    
    switch(type){
        case KKSMessage::mtIn:
            px = QPixmap(*pxMsgIn);
            break;
        case KKSMessage::mtOut:
            px = QPixmap(*pxMsgOut);
            break;
    }
    
    return px;
}

void KKSMessage::setPxIn(const QPixmap & px)
{
    if(pxMsgIn)
        delete pxMsgIn;

    pxMsgIn = new QPixmap(px);
}

void KKSMessage::setPxOut(const QPixmap & px)
{
    if(pxMsgOut)
        delete pxMsgOut;

    pxMsgOut = new QPixmap(px);
}

KKSMessage::~KKSMessage()
{

}

KKSMessage::KKSMessage()
{
    m_id = -1;
    m_categoryId = -1;
    m_msgType = mtIn;

    m_receiveDateTime = QDateTime::currentDateTime();
    m_readDateTime = QDateTime::currentDateTime();
    m_sentDateTime = QDateTime::currentDateTime();
    m_insertTime = QDateTime::currentDateTime();
}

KKSMessage::KKSMessage(const KKSMessage & in)
{
    m_id = in.m_id;

    m_dlFrom = in.m_dlFrom;
    m_dlTo = in.m_dlTo;

    m_objectId = in.m_objectId;
    m_objectName = in.m_objectName;

    m_categoryId = in.m_categoryId;  
    m_categoryName = in.m_categoryName;             

    m_msgType = in.m_msgType;   

    m_insertTime = in.m_insertTime;

    m_sentDateTime = in.m_sentDateTime;
    m_receiveDateTime = in.m_receiveDateTime;
    m_readDateTime = in.m_readDateTime;

    m_messageBody = in.m_messageBody;
}

KKSMessage & KKSMessage::operator = ( const KKSMessage & in )
{
    m_id = in.m_id;

    m_dlFrom = in.m_dlFrom;
    m_dlTo = in.m_dlTo;

    m_objectId = in.m_objectId;
    m_objectName = in.m_objectName;

    m_categoryId = in.m_categoryId;  
    m_categoryName = in.m_categoryName;             

    m_msgType = in.m_msgType;   

    m_insertTime = in.m_insertTime;

    m_sentDateTime = in.m_sentDateTime;
    m_receiveDateTime = in.m_receiveDateTime;
    m_readDateTime = in.m_readDateTime;

    m_messageBody = in.m_messageBody;

    return * this;
}

bool KKSMessage::operator == ( const KKSMessage & in ) const
{
    if(m_categoryId != in.m_categoryId)
        return false;
    if(m_categoryName != in.m_categoryName)
        return false;

    if(m_objectId != in.m_objectId)
        return false;
    if(m_objectName != in.m_objectName)
        return false;

    if(m_msgType != in.m_msgType)
        return false;

    if(m_dlFrom != in.m_dlFrom)
        return false;
    if(m_dlTo != in.m_dlTo)
        return false;

    if(m_sentDateTime != in.m_sentDateTime)
        return false;
    if(m_receiveDateTime != in.m_receiveDateTime)
        return false;
    if(m_readDateTime != in.m_readDateTime)
        return false;
    if(m_insertTime != in.m_insertTime)
        return false;

    if(m_id != in.m_id)
        return false;

    if(m_messageBody != in.m_messageBody)
        return false;

    return true;
}

bool KKSMessage::operator != ( const KKSMessage & in ) const
{
    if(! (*this == in) )
        return true;

    return false;
}

int KKSMessage::id() const
{
    return m_id;
}

void KKSMessage::setId(int _id)
{
    m_id = _id;
}

const QString & KKSMessage::objectName() const
{
    return m_objectName;
}

void KKSMessage::setObjectName(const QString & v)
{
    m_objectName = v;
}

int KKSMessage::objectId() const
{
    return m_objectId;
}

void KKSMessage::setObjectId(int _id)
{
    m_objectId = _id;
}


const QString & KKSMessage::dlFrom() const
{
    return m_dlFrom;
}

void KKSMessage::setDlFrom(const QString & v)
{
    m_dlFrom = v;
}

const QString & KKSMessage::dlTo() const
{
    return m_dlTo;
}

void KKSMessage::setDlTo(const QString & v)
{
    m_dlTo = v;
}

const QString & KKSMessage::categoryName() const
{
    return m_categoryName;
}

void KKSMessage::setCategoryName(const QString & v)
{
    m_categoryName = v;
}

int KKSMessage::categoryId() const
{
    return m_categoryId;
}

void KKSMessage::setCategoryId(int _id)
{
    m_categoryId = _id;
}

KKSMessage::MsgType KKSMessage::msgType() const
{
    return m_msgType;
}

void KKSMessage::setMsgType(MsgType _msgType)
{
    m_msgType = _msgType;
}

const QDateTime & KKSMessage::insertTime() const
{
    return m_insertTime;
}

void KKSMessage::setInsertTime(const QDateTime & v)
{
    m_insertTime = v;
}

const QDateTime & KKSMessage::sentDateTime() const
{
    return m_sentDateTime;
}

void KKSMessage::setSentDateTime(const QDateTime & v)
{
    m_sentDateTime = v;
}

const QDateTime & KKSMessage::receiveDateTime() const
{
    return m_receiveDateTime;
}

void KKSMessage::setReceiveDateTime(const QDateTime & v)
{
    m_receiveDateTime = v;
}

const QDateTime & KKSMessage::readDateTime() const
{
    return m_readDateTime;
}

void KKSMessage::setReadDateTime(const QDateTime & v)
{
    m_readDateTime = v;
}

const QString & KKSMessage::messageBody() const
{
    return m_messageBody;
}

void KKSMessage::setMessageBody(const QString & v)
{
    m_messageBody = v;
}

KKSMsgJournalItemDelegate :: KKSMsgJournalItemDelegate (QObject * parent) :
    QItemDelegate (parent)
{
}

void KKSMsgJournalItemDelegate :: paint(QPainter *painter, 
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    painter->save();

    QFont font;
    QColor color;
    color = Qt::white;

    KKSMsgJournalItemData msgItemData = index.data().value<KKSMsgJournalItemData> ();

    
    KKSMessage msg = msgItemData.getMessage();

 
    QBrush brush(color);
    painter->setBrush(brush);
    painter->setPen(Qt::NoPen);
    painter->drawRect(option.rect);
/*
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
*/

    font = QFont("Calibri", 10);
    color = Qt::black;

    painter->setFont(font);
    painter->setPen(color);

    if (option.state & QStyle::State_Selected){
        painter->setBrush(option.palette.highlight());
        painter->drawRect(option.rect);
        painter->setPen(option.palette.highlightedText().color());
        //qWarning() << "sELECTED Column #" << index.column();
    }
    
    //qWarning() << "Column #" << index.column();

    int column = index.column();
    //bool bFavourites = ((FBLvUrlModel *)(index.model()))->isForFavourites();
    msgItemData.drawItem(painter, option.rect, column);//, bFavourites);
   
    painter->restore();
}

QSize KKSMsgJournalItemDelegate :: sizeHint(const QStyleOptionViewItem& option,
                                            const QModelIndex& index ) const
{
    QSize ws = option.rect.size ();
    if (!index.isValid())
        return QSize ();

    if (index.data().isNull())
        return QSize ();

    //return QSize(30, 19);
    KKSMsgJournalItemData msgItemData = index.data().value<KKSMsgJournalItemData> ();

    KKSMessage m_msg = msgItemData.getMessage();
    
    QString text;
    
    QPixmap px;
    switch(index.column())
    {
    case 0:
        text = QString::number(m_msg.id());
        px = KKSMessage::pixmap(m_msg.msgType());
        break;
    case 1:
        text = m_msg.messageBody();
        break;
    case 2:
        text = m_msg.objectName();
        break;
    case 3:
        text = m_msg.categoryName();
        break;
    case 4:
        text = m_msg.dlFrom();
        break;
    case 5:
        text = m_msg.dlTo();
        break;
    case 6:
        text = m_msg.sentDateTime().toString("dd.MM.yyyy h:mm");
        break;
    case 7:
        text = m_msg.receiveDateTime().toString("dd.MM.yyyy h:mm");
        break;
    case 8:
        text = m_msg.readDateTime().toString("dd.MM.yyyy h:mm");
        break;
    default:
        text = QObject::tr("Not defined");
    }

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
void KKSMsgJournalItemData::drawItem( QPainter * painter, 
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
        text = QString::number(m_msg.id());
        px = KKSMessage::pixmap(m_msg.msgType());
        px_rect = QRect(rect.left()+3, rect.top(), 16+3, 16);
        
        if(!px.isNull())
            shift = 16;
        bound = QRect(rect.left()+5+shift, rect.top(), rect.width()-5-shift, rect.height());
        flags |= Qt::AlignHCenter;
        break;
    case 1:
        text = m_msg.messageBody();
        bound = QRect(rect.left()+5, rect.top(), rect.width()-5, rect.height());
        break;
    case 2:
        text = m_msg.objectName();
        bound = QRect(rect.left()+5, rect.top(), rect.width()-5, rect.height());
        break;
    case 3:
        text = m_msg.categoryName();
        bound = QRect(rect.left()+5, rect.top(), rect.width()-5, rect.height());
        break;
    case 4:
        text = m_msg.dlFrom();
        bound = QRect(rect.left()+5, rect.top(), rect.width()-5, rect.height());
        break;
    case 5:
        text = m_msg.dlTo();
        bound = QRect(rect.left()+5, rect.top(), rect.width()-5, rect.height());
        break;
    case 6:
        text = m_msg.sentDateTime().toString("dd.MM.yyyy h:mm");
        flags |= Qt::AlignHCenter;
        break;
    case 7:
        text = m_msg.receiveDateTime().toString("dd.MM.yyyy h:mm");
        flags |= Qt::AlignHCenter;
        break;
    case 8:
        text = m_msg.readDateTime().toString("dd.MM.yyyy h:mm");
        flags |= Qt::AlignHCenter;
        if(text.isEmpty()){
            if(m_msg.msgType() == KKSMessage::mtIn)
                text = QObject::tr("Mark as readed!");
            else
                text = QObject::tr("Not readed yet!");
        }
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
    if(column == 8)
        painter->drawLine(rect.topRight(), rect.bottomRight());
}

const KKSMessage & KKSMsgJournalItemData::getMessage( ) const
{
    return m_msg;
}

KKSMessage & KKSMsgJournalItemData::getMessage ( )
{
    return m_msg;
}

void KKSMsgJournalItemData::setMessage( const KKSMessage & msg )
{
    m_msg = msg;
}

KKSMsgJournalItemData::KKSMsgJournalItemData(const KKSMsgJournalModel * parent, int row)
{
    m_pParent = parent;
    m_row = row;
}

KKSMsgJournalItemData::KKSMsgJournalItemData( const KKSMsgJournalItemData & item )
{
    m_pParent = item.m_pParent;
    m_row = item.m_row;

    setMessage( item.getMessage() );
}

KKSMsgJournalItemData::KKSMsgJournalItemData( const KKSMsgJournalModel * parent, 
                                              int row, 
                                              const KKSMessage & msg)
{
    m_pParent = parent;
    m_row = row;
    setMessage( msg );
}

KKSMsgJournalItemData::~ KKSMsgJournalItemData( )
{
}
