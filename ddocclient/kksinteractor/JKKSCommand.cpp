/**************************************************************************************
 * Реализация класса передаваемых команд
 **************************************************************************************/

#include <QDataStream>
#include <QTextStream>
#include <QStringList>
#include <QBuffer>
#include <QIODevice>
#include <QtDebug>

#include "JKKSCommand.h"
#include "JKKSCategory.h"
#include "JKKSLoader.h"
#include "kksresult.h"
#include "defines.h"

JKKSCommand :: JKKSCommand (int id,
                            const QString& u_idFrom,
                            const QString& dl_from_name,
                            const QString& u_idExec,
                            const QString& dl_executor_name,
                            const JKKSAddress & eAddr,
                            const JKKSAddress& eOrgAddr,
                            const QString& u_idTo,
                            const QString& dl_to_name,
                            int idIoCat,
                            const QString& category_code,
                            int idJrSt,
                            const QDateTime& iTime,
                            const QDateTime& eTime,
                            int e_t_interval,
                            int t_unit,
                            const QString& mess_body,
                            const JKKSAddress & address,
                            const QString& code,
                            const QString & uid,
                            const QString & inputNumber,
                            const QString & outputNumber,
                            int   idUL,
                            int   idObj)
    : JKKSMessage (address, code), JKKSUID(uid, QString()),
    idCommand (id),
    u_idDlFrom (u_idFrom),
    dlFromName (dl_from_name),
    u_idDlExecutor (u_idExec),
    dlExecutorName (dl_executor_name),
    executorAddress (eAddr),
    execOrgAddress (eOrgAddr),
    u_idDlTo (u_idTo),
    dlToName (dl_to_name),
    idIOCategory (idIoCat),
    categoryCode (category_code),
    idJrState (idJrSt),
    insertTime (iTime),
    execTime (eTime),
    exec_time_interval (e_t_interval),
    time_unit (t_unit),
    messageBody (mess_body),
    m_inputNumber(inputNumber),
    m_outputNumber(outputNumber),
    m_idUrgencyLevel(idUL),
    m_idObject(idObj),
    ioDoc (JKKSDocument())
{
}

JKKSCommand :: JKKSCommand (const JKKSCommand& command)
: JKKSMessage (command), JKKSUID(command.m_uid, command.m_uuid),
    idCommand (command.idCommand),
    u_idDlFrom (command.u_idDlFrom),
    dlFromName (command.dlFromName),
    u_idDlExecutor (command.u_idDlExecutor),
    dlExecutorName (command.dlExecutorName),
    executorAddress (command.executorAddress),
    execOrgAddress (command.execOrgAddress),
    u_idDlTo (command.u_idDlTo),
    dlToName (command.dlToName),
    idIOCategory (command.idIOCategory),
    categoryCode (command.categoryCode),
    idJrState (command.idJrState),
    insertTime (command.insertTime),
    execTime (command.execTime),
    exec_time_interval (command.exec_time_interval),
    time_unit (command.time_unit),
    messageBody (command.messageBody),
    m_inputNumber (command.m_inputNumber),
    m_outputNumber (command.m_outputNumber),
    m_idUrgencyLevel (command.m_idUrgencyLevel),
    m_idObject (command.m_idObject),
    category (command.category),
    ioDoc (command.ioDoc)
{
}

JKKSCommand :: ~JKKSCommand (void)
{
}

QByteArray JKKSCommand :: serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&qBuffer);

    out << getAddr();
    out << getCode();
    out << idCommand;
    out << u_idDlFrom;
    out << dlFromName;
    out << u_idDlExecutor;
    out << dlExecutorName;
    out << executorAddress;
    out << execOrgAddress;
    out << u_idDlTo;
    out << dlToName;
    out << idIOCategory;
    out << categoryCode;
    out << idJrState;
    out << insertTime;
    out << execTime;
    out << exec_time_interval;
    out << time_unit;
    out << messageBody;
    out << category;
    out << m_uid;
    out << m_inputNumber;
    out << m_outputNumber;
    out << m_idUrgencyLevel;
    out << m_idObject;
    if (m_idObject > 0)
    {
        QByteArray docArr = ioDoc.serialize ();
        out << docArr;
    }

    return qBuffer.buffer();
}

int JKKSCommand :: unserialize (const QByteArray& mess)
{
    qDebug () << __PRETTY_FUNCTION__ << mess;
    QBuffer buffer;
    buffer.setData (mess);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);

    JKKSAddress addr;
    QString code;
    in >> addr;
    in >> code;
    in >> idCommand;
    in >> u_idDlFrom;
    in >> dlFromName;
    in >> u_idDlExecutor;
    in >> dlExecutorName;
    in >> executorAddress;
    in >> execOrgAddress;
    in >> u_idDlTo;
    in >> dlToName;
    in >> idIOCategory;
    in >> categoryCode;
    in >> idJrState;
    in >> insertTime;
    in >> execTime;
    in >> exec_time_interval;
    in >> time_unit;
    in >> messageBody;
    in >> category;
    in >> m_uid;
    in >> m_inputNumber;
    in >> m_outputNumber;
    in >> m_idUrgencyLevel;
    in >> m_idObject;

    for (QMap<int, JKKSCategory>::const_iterator pc = category.constBegin (); \
            pc != category.constEnd (); \
            pc++)
    {
        qDebug () << __PRETTY_FUNCTION__ << pc.value().rubrics ().keys();
    }

    if (m_idObject > 0)
    {
        QByteArray docArr;
        in >> docArr;
        ioDoc.unserialize (docArr);
    }

    setAddr(addr);
    setCode(code);

    return OK_CODE;
}

int JKKSCommand :: writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    //qDebug() << __PRETTY_FUNCTION__;
    Q_UNUSED (senderUID);
    Q_UNUSED (receiverUID);
    return loader->writeMessage (this);
}

JKKSMessage::JKKSMessageType JKKSCommand :: getMessageType (void) const
{
    return JKKSMessage::atCommand;
}

const QMap<int, JKKSCategory>& JKKSCommand :: getCategory (void) const
{
    return category;
}

void JKKSCommand::setCategory (const QMap<int, JKKSCategory>& catMap)
{
    category = catMap;
}

int JKKSCommand :: id (void) const
{
    return idCommand;
}

void JKKSCommand :: setId (int id)
{
    idCommand = id;
}

QString JKKSCommand :: getDlFrom (void) const
{
    return u_idDlFrom;
}

void JKKSCommand :: setDlFrom (const QString& u_idFrom)
{
    u_idDlFrom = u_idFrom;
}

QString JKKSCommand :: getDlFromName (void) const
{
    return dlFromName;
}

void JKKSCommand :: setDlFromName (const QString& dl_from_name)
{
    dlFromName = dl_from_name;
}

QString JKKSCommand :: getDlExec (void) const
{
    return u_idDlExecutor;
}

void JKKSCommand :: setDlExec (const QString& u_idExec)
{
    u_idDlExecutor = u_idExec;
}

QString JKKSCommand::getDlExecutorName (void) const
{
    return dlExecutorName;
}

void JKKSCommand::setDlExecutorName (const QString& dl_executor_name)
{
    dlExecutorName = dl_executor_name;
}

const JKKSAddress & JKKSCommand :: getEAddr (void) const
{
    return executorAddress;
}

void JKKSCommand :: setEAddr (const JKKSAddress & eAddr)
{
    executorAddress = eAddr;
}

const JKKSAddress & JKKSCommand :: getEOrg (void) const
{
    return execOrgAddress;
}

void JKKSCommand :: setEOrg (const JKKSAddress & eOrg)
{
    execOrgAddress = eOrg;
}

QString JKKSCommand :: getDlTo (void) const
{
    return u_idDlTo;
}

void JKKSCommand :: setDlTo (const QString& u_idTo)
{
    u_idDlTo = u_idTo;
}

QString JKKSCommand :: getDlToName (void) const
{
    return dlToName;
}

void JKKSCommand :: setDlToName (const QString& dl_to_name)
{
    dlToName = dl_to_name;
}

int JKKSCommand :: getIdIoCat (void) const
{
    return idIOCategory;
}

void JKKSCommand :: setIdIoCat (int id_io_category)
{
    idIOCategory = id_io_category;
}

QString JKKSCommand :: getCategoryCode (void) const
{
    return categoryCode;
}

void JKKSCommand :: setCategoryCode (const QString& category_code)
{
    categoryCode = category_code;
}

int JKKSCommand :: getJournalState (void) const
{
    return idJrState;
}

void JKKSCommand :: setJournalState (int idState)
{
    idJrState = idState;
}

const QDateTime& JKKSCommand :: getInsertTime (void) const
{
    return insertTime;
}

void JKKSCommand :: setInsertTime (const QDateTime& iTime)
{
    insertTime = iTime;
}

const QDateTime& JKKSCommand :: getExecTime (void) const
{
    return execTime;
}

void JKKSCommand :: setExecTime (const QDateTime& eTime)
{
    execTime = eTime;
}

QString JKKSCommand :: getTimeInterval (void) const
{
    if (exec_time_interval>= 0)
        return QString("'{%1,%2}'").arg (exec_time_interval).arg (time_unit);
    else
        return QString("NULL");
    //TODO
    //return (exec_time_interval = 0 ? QString::null : QString ("%1 %2").arg (exec_time_interval).arg (time_unit));
}

void JKKSCommand :: setTimeInterval (int val, int unit)
{
    exec_time_interval = val;
    time_unit = unit;
}

QString JKKSCommand :: getMessageBody (void) const
{
    return messageBody;
}

void JKKSCommand :: setMessageBody (const QString& mess)
{
    messageBody = mess;
}

const JKKSDocument& JKKSCommand :: getAttachedDoc (void) const
{
    return ioDoc;
}

void JKKSCommand :: setAttachment (const JKKSDocument& doc)
{
    ioDoc = doc;
}



/***************************/

JKKSCmdConfirmation::JKKSCmdConfirmation(int idCmd, 
                                         const QString & extraId,
                                         int idState,
                                         const QDateTime & acceptedDatetime,
                                         const QDateTime & receiveDatetime,
                                         const JKKSAddress & addr,
                                         const QString & kvs)
: JKKSMessage (addr, kvs),
m_id(idCmd),
unique_id(extraId),
m_idState(idState),
m_acceptedDatetime(acceptedDatetime),
m_receiveDatetime(receiveDatetime)
{
}

JKKSCmdConfirmation::JKKSCmdConfirmation(const JKKSCmdConfirmation & cfm)
: JKKSMessage (cfm),
m_id(cfm.m_id),
unique_id(cfm.unique_id),
m_idState(cfm.m_idState),
m_acceptedDatetime(cfm.m_acceptedDatetime),
m_receiveDatetime(cfm.m_receiveDatetime)
{

}

JKKSCmdConfirmation::~JKKSCmdConfirmation()
{

}

QByteArray JKKSCmdConfirmation::serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open (QIODevice::WriteOnly);
    QDataStream out (&qBuffer);

    out << getAddr();
    out << getCode();
    out << id();
    out << extraId();
    out << idState();
    out << acceptedDatetime();
    out << receiveDatetime();

    return qBuffer.buffer();
}

int JKKSCmdConfirmation::unserialize (const QByteArray& mess)
{
    QBuffer buffer;
    buffer.setData(mess);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);

    JKKSAddress addr;
    QString code;
    in >> addr;
    in >> code;
    in >> m_id;
    in >> unique_id;
    in >> m_idState;
    in >> m_acceptedDatetime;
    in >> m_receiveDatetime;

    setAddr (addr);
    setCode (code);

    return OK_CODE;
}

int JKKSCmdConfirmation::writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    Q_UNUSED (senderUID);
    Q_UNUSED (receiverUID);
    return loader->writeMessage (this);
}

JKKSMessage::JKKSMessageType JKKSCmdConfirmation::getMessageType (void) const
{
    return JKKSMessage::atCmdConfirmation;
}

int JKKSCmdConfirmation::id (void) const
{
    return m_id;
}

void JKKSCmdConfirmation::setId (int the_id)
{
    m_id = the_id;
}

const QString & JKKSCmdConfirmation::extraId (void) const
{
    return unique_id;
}

void JKKSCmdConfirmation::setExtraId (const QString & the_id)
{
    unique_id = the_id;
}

int JKKSCmdConfirmation::idState() const
{
    return m_idState;
}

void JKKSCmdConfirmation::setIdState(int state)
{
    m_idState = state;
}

const QDateTime & JKKSCmdConfirmation::acceptedDatetime() const
{
    return m_acceptedDatetime;
}

void JKKSCmdConfirmation::setAcceptedDatetime(const QDateTime & dt)
{
    m_acceptedDatetime = dt;
}

const QDateTime & JKKSCmdConfirmation::receiveDatetime() const
{
    return m_receiveDatetime;
}

void JKKSCmdConfirmation::setReceiveDatetime(const QDateTime & dt)
{
    m_receiveDatetime = dt;
}
