#ifndef KKSCMDJOURNALITEM_H
#define KKSCMDJOURNALITEM_H

#include <QWidget>
#include <QItemDelegate>
#include <QDateTime>
#include <QPixmap>

#include "kksgui_config.h"

class QPainter;
class KKSCmdJournalModel;

class _GUI_EXPORT KKSCommand
{
public:

    enum CmdType
    {
        ctIn = 0,
        ctOut = 1,
        ctAllIn = 2,
        ctAllOut = 3
    };

    enum CmdTypeEx
    {
        cteIn = 1,
        cteControl = 2,
        cteOut = 3
    };

    enum JrType
    {
        jtIn = 1,
        jtOut = 2
    };

    enum CmdState
    {
        csPlanned = 1, //запланировано
        csOuted = 2, //выдано задание
        csExecuting = 3, //исполняется
        csExecuted = 4, //выполнено
        csTimeElapsed = 5, //просрочено
        csSavedAsDraft = 6 //сохранено как черновик
    };

    static QPixmap pixmap(KKSCommand::CmdType type);
    static QPixmap archivedPixmap();
    static void setPxIn(const QPixmap & px);
    static void setPxInAll(const QPixmap & px);
    static void setPxOut(const QPixmap & px);
    static void setPxOutAll(const QPixmap & px);
    static void setPxArchived(const QPixmap & px);

    KKSCommand();
    KKSCommand(const KKSCommand & cmd);
    ~KKSCommand();

    double timePercentEstimated() const;

    int id() const;
    void setId(int _id);

    const QString & dlFrom() const;
    void setDlFrom(const QString & v);

    const QString & dlExecutor() const;
    void setDlExecutor(const QString & v);

    const QString & dlTo() const;
    void setDlTo(const QString & v);

    const QString & categoryName() const;
    void setCategoryName(const QString & v);

    int categoryId() const;
    void setCategoryId(int _id);

    int ioObjectId() const;
    void setIoObjectId(int _id);

    CmdType cmdType() const;
    void setCmdType(CmdType _cmdType);

    CmdTypeEx cmdTypeEx() const;
    void setCmdTypeEx(CmdTypeEx _cmdTypeEx);

    CmdState cmdState() const;
    void setCmdState(CmdState _cmdState);

    const QString & cmdStateName() const;
    void setCmdStateName(const QString & v);

    const QDateTime & insertTime() const;
    void setInsertTime(const QDateTime & v);

    const QDateTime & receiveDateTime() const;
    void setReceiveDateTime(const QDateTime & v);
    
    const QDateTime & acceptedDateTime() const;
    void setAcceptedDateTime(const QDateTime & v);
    
    const QDateTime & readDateTimeE() const;
    void setReadDateTimeE(const QDateTime & v);

    const QDateTime & readDateTimeC() const;
    void setReadDateTimeC(const QDateTime & v);

    const QDateTime & execDateTime() const;//planned_exec
    void setExecDateTime(const QDateTime & v);

    const QDateTime & realExec() const;
    void setRealExec(const QDateTime & v);

    const QString & execPeriod() const;
    void setExecPeriod(const QString & v);

    const QString & messageBody() const;
    void setMessageBody(const QString & v);

    bool isArchived() const;
    void setIsArchived(bool _is);

    KKSCommand & operator = ( const KKSCommand & in );
    bool operator == (const KKSCommand & in) const;
    bool operator != (const KKSCommand & in) const;


private:
    int m_id;
    QString m_messageBody;
    QString m_categoryName;

    QString m_dlFrom;
    QString m_dlTo;
    QString m_dlExecutor;

    QDateTime m_execDateTime;//planned_exec
    QDateTime m_insertTime;
    QString m_execPeriod;
    QDateTime m_receiveDateTime;
    QDateTime m_acceptedDateTime;
    QDateTime m_readDateTimeE;//дата и время прочтения распоряжения исполнителем
    QDateTime m_readDateTimeC;//дата и время прочтения распоряжения контроллером
    QDateTime m_realExec;
    QString m_cmdStateName;

    int m_categoryId;
    int m_ioObjectId;//прикрепленный документ
    CmdType m_cmdType;//in/out
    CmdTypeEx m_cmdTypeEx;//in/control/out
    CmdState m_cmdState;

    bool m_isArchived;
};

Q_DECLARE_METATYPE (KKSCommand);

class _GUI_EXPORT KKSCmdJournalItemData : public QObject
{
    Q_OBJECT

signals:
    void dataChanged();

public:

    KKSCmdJournalItemData( const KKSCmdJournalModel * parent = NULL, int row = -1 );
    KKSCmdJournalItemData( const KKSCmdJournalItemData & item );
    KKSCmdJournalItemData( const KKSCmdJournalModel * parent, int row, const KKSCommand & cmd);
    ~KKSCmdJournalItemData();

    void setParent( const KKSCmdJournalModel * parent, int row) {m_pParent = parent; m_row = row;}

    void drawItem( QPainter * painter, 
                   QRect rect, 
                   int column);//, 
                   //bool bFavourites = false );

    const KKSCommand & getCommand( ) const;
    KKSCommand & getCommand();
    void setCommand( const KKSCommand & cmd);

    //QPixmap getPixmap(KKSCommand::CmdType cmdType);

private:
    const KKSCmdJournalModel * m_pParent;
    int m_row;
    KKSCommand m_cmd;
};

Q_DECLARE_METATYPE( KKSCmdJournalItemData );


class _GUI_EXPORT KKSCmdJournalItemDelegate  : public QItemDelegate
{
    public:
        KKSCmdJournalItemDelegate  (QObject * parent=NULL);
        ~KKSCmdJournalItemDelegate  ( void ) {};

        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

        QSize sizeHint(const QStyleOptionViewItem &option,
                const QModelIndex &index ) const;

    private:
        Q_OBJECT
};

#endif
