#ifndef KKSMSGJOURNALITEM_H
#define KKSMSGJOURNALITEM_H

#include <QWidget>
#include <QItemDelegate>
#include <QDateTime>
#include <QPixmap>

#include "kksgui_config.h"

class QPainter;
class KKSMsgJournalModel;

class _GUI_EXPORT KKSMessage
{
public:

    enum MsgType
    {
        mtIn = 0,
        mtOut = 1
    };

/*    enum JrType
    {
        jtIn = 1,
        jtOut = 2
    };
*/

    static QPixmap pixmap(KKSMessage::MsgType type);
    //static QPixmap archivedPixmap();
    static void setPxIn(const QPixmap & px);
    //static void setPxInAll(const QPixmap & px);
    static void setPxOut(const QPixmap & px);
    //static void setPxOutAll(const QPixmap & px);
    //static void setPxArchived(const QPixmap & px);

    KKSMessage();
    KKSMessage(const KKSMessage & msg);
    ~KKSMessage();

    //double timePercentEstimated() const;

    int id() const;
    void setId(int _id);

    //const QString & executor() const;
    //void setExecutor(const QString & v);
    
    const QString & dlFrom() const;
    void setDlFrom(const QString & v);

    //const QString & dlExecutor() const;
    //void setDlExecutor(const QString & v);

    const QString & dlTo() const;
    void setDlTo(const QString & v);

    const QString & categoryName() const;
    void setCategoryName(const QString & v);

    int categoryId() const;
    void setCategoryId(int _id);

    const QString & objectName() const;
    void setObjectName(const QString & v);

    int objectId() const;
    void setObjectId(int _id);

    MsgType msgType() const;
    void setMsgType(MsgType _msgType);

    //CmdState cmdState() const;
    //void setCmdState(CmdState _cmdState);

    //const QString & cmdStateName() const;
    //void setCmdStateName(const QString & v);

    const QDateTime & insertTime() const;
    void setInsertTime(const QDateTime & v);

    const QDateTime & sentDateTime() const;
    void setSentDateTime(const QDateTime & v);

    const QDateTime & receiveDateTime() const;
    void setReceiveDateTime(const QDateTime & v);

    const QDateTime & readDateTime() const;
    void setReadDateTime(const QDateTime & v);

    //bool isAutomated() const;
    //void setIsAutomated(bool _is);

    //const QString & execPeriod() const;
    //void setExecPeriod(const QString & v);

    const QString & messageBody() const;
    void setMessageBody(const QString & v);

    KKSMessage & operator = ( const KKSMessage & in );
    bool operator == (const KKSMessage & in) const;
    bool operator != (const KKSMessage & in) const;


private:
    int m_id;
    QString m_dlFrom;
    QString m_dlTo;
    QString m_categoryName;
    int m_categoryId;
    QString m_objectName;
    int m_objectId;
    MsgType m_msgType;
    QDateTime m_insertTime;
    QDateTime m_sentDateTime;
    QString m_messageBody;
    QDateTime m_receiveDateTime;
    QDateTime m_readDateTime;
};

Q_DECLARE_METATYPE (KKSMessage);

class _GUI_EXPORT KKSMsgJournalItemData : public QObject
{
    Q_OBJECT

signals:
    void dataChanged();

public:

    KKSMsgJournalItemData( const KKSMsgJournalModel * parent = NULL, int row = -1 );
    KKSMsgJournalItemData( const KKSMsgJournalItemData & item );
    KKSMsgJournalItemData( const KKSMsgJournalModel * parent, int row, const KKSMessage & msg);
    ~KKSMsgJournalItemData();

    void setParent( const KKSMsgJournalModel * parent, int row) {m_pParent = parent; m_row = row;}

    void drawItem( QPainter * painter, 
                   QRect rect, 
                   int column);//, 
                   //bool bFavourites = false );

    const KKSMessage & getMessage( ) const;
    KKSMessage & getMessage();
    void setMessage( const KKSMessage & msg);

    //QPixmap getPixmap(KKSCommand::CmdType cmdType);

private:
    const KKSMsgJournalModel * m_pParent;
    int m_row;
    KKSMessage m_msg;
};

Q_DECLARE_METATYPE( KKSMsgJournalItemData );


class _GUI_EXPORT KKSMsgJournalItemDelegate  : public QItemDelegate
{
    public:
        KKSMsgJournalItemDelegate  (QObject * parent=NULL);
        ~KKSMsgJournalItemDelegate  ( void ) {};

        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

        QSize sizeHint(const QStyleOptionViewItem &option,
                const QModelIndex &index ) const;

    private:
        Q_OBJECT
};

#endif
