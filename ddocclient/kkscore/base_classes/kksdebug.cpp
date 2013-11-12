
#include "kksdebug.h"
#include "kkssito.h"
#include <defines.h>
#include "KKSDbgOutputWidget.h"


KKSMsgType m_minMsgType = KKSDebugMsg;
bool m_useQDebug = true;

void KKSDebug::setMinMsgType(KKSMsgType t)
{
    m_minMsgType = t;
}

void KKSDebug::setUseQDebug(bool b)
{
    m_useQDebug = b;
}

KKSDebug::KKSDebug(KKSMsgType type)
{
    m_msgType = type;
}

KKSDebug::KKSDebug(const KKSDebug & d)
{
    m_msgType = d.m_msgType;
}

KKSDebug::~KKSDebug()
{

}

QDebug KKSDebug::qDebug()
{
    switch(m_msgType){
        case KKSDebugMsg: return ::qDebug(); break;
        case KKSWarningMsg: return ::qWarning(); break;
        case KKSCriticalMsg:return ::qCritical(); break;
        case KKSFatalMsg: return ::qCritical(); break;
        case KKSInfoMsg: return ::qDebug(); break;
        case KKSImportantInfoMsg: return ::qDebug(); break;
        default: return ::qDebug(); break;
    }

    return ::qDebug();
}

KKSDebug & KKSDebug::operator << (const QString & t) 
{ 
    if(m_msgType < m_minMsgType)
        return *this;

    this->qDebug() << t;
    
    print(t);
    
    return *this;
}

KKSDebug & KKSDebug::operator << (const QByteArray & t) 
{ 
    if(m_msgType < m_minMsgType)
        return *this;

    this->qDebug() << t;
    
    print(t);

    return *this;
}

KKSDebug & KKSDebug::operator << (const QDateTime & t) 
{ 
    if(m_msgType < m_minMsgType)
        return *this;

    this->qDebug() << t;

    print(t.toString("dd.MM.yyyy hh:mm:ss"));

    return *this;
}

KKSDebug & KKSDebug::operator << (const QDate & t) 
{ 
    if(m_msgType < m_minMsgType)
        return *this;

    this->qDebug() << t;

    print(t.toString("hh.MM.yyyy"));

    return *this;
}

KKSDebug & KKSDebug::operator << (const QTime & t) 
{ 
    if(m_msgType < m_minMsgType)
        return *this;

    this->qDebug() << t;

    print(t.toString("hh:mm:ss"));

    return *this;
}

KKSDebug & KKSDebug::operator << (int t) 
{ 
    if(m_msgType < m_minMsgType)
        return *this;

    this->qDebug() << t;

    print(QString::number(t));
    
    return *this;
}

void KKSDebug::print(const QString & msg)
{
    if(!kksSito->dbgWidget())
        return;

    KKSDbgOutputWidget::Criticality c;
    switch(m_msgType){
        case KKSDebugMsg: c = KKSDbgOutputWidget::cDebugMsg; break;
        case KKSWarningMsg: c = KKSDbgOutputWidget::cWarningMsg; break;
        case KKSInfoMsg: c = KKSDbgOutputWidget::cInfoMsg; break;
        case KKSImportantInfoMsg: c = KKSDbgOutputWidget::cImportantInfoMsg; break;
        case KKSCriticalMsg: c = KKSDbgOutputWidget::cCriticalMsg; break;
        case KKSFatalMsg: c = KKSDbgOutputWidget::cFatalMsg; break;
    }
    
    kksSito->dbgWidget()->printMessage( c, msg );
}

/*
KKSError::KKSError()
{
    m_type = etNoError;
    m_code = "00000"; 
    m_name = "SUCCESSFUL COMPLETION"; 
    m_message = "SUCCESSFUL COMPLETION"; 
    m_group = "SUCCESSFUL COMPLETION";
}

KKSError::KKSError(ErrorTypes type, QString code, QString name, QString message, QString group)
{
    m_type = type;
    m_code = code; 
    m_name = name; 
    m_message = message; 
    m_group = group;
}

KKSError::~KKSError()
{
    if(log.isOpen())
        log.flush();
}

void KKSError::print(PrintFlags flags, QWidget * parent)
{
    if(flags & asQDebug){
        qDebug() << "ERROR! Code:" << code() << " : " << message();
    }
    if(flags & asQWarning){
        qWarning() << "ERROR! Code:" << code() << " : " << message().toUtf8().constData();
    }
    if(flags & asInfoDialog){
        QMessageBox::information(parent, 
                                 name(), 
                                 QString("Error code: %1\n\n%2\n%3").arg(code()).arg(name()).arg(message()), 
                                 QMessageBox::Ok);
    }
    if(flags & asWarningDialog){
        QMessageBox::warning(parent, 
                             name(), 
                             QString("Error code: %1\n\n%2\n%3").arg(code()).arg(name()).arg(message()), 
                             QMessageBox::Ok, 
                             QMessageBox::NoButton);
    }
    if(flags & asCriticalDialog){
        QMessageBox::critical(parent, 
                              name(), 
                              QString("Error code: %1\n\n%2\n%3").arg(code()).arg(name()).arg(message()), 
                              QMessageBox::Ok, 
                              QMessageBox::NoButton);
    }

    if(flags & asLogFile){
        if(!log.isOpen())
            return;

        QByteArray b, b1, b2;
        b = code().toLocal8Bit();
        b1 = message().toLocal8Bit();
        b2 = b + QByteArray(" : ") + b1;
        log.write( b2 );
        log.putChar('\n');
    }

}

void KKSError::init(ErrorTypes type, QString code, QString name, QString message, QString group)
{
    setType(type);
    setCode(code);
    setName(name);
    setMessage(message);
    setGroup(group);
}

int KKSError::setLogFile(QString & fileName)
{
    log.setFileName(fileName);
    bool ok = log.open(QFile::WriteOnly);
    if(!ok || !log.isOpen())
        return ERROR_CODE;

    log.write(QDateTime::currentDateTime().toString(Qt::TextDate).toLocal8Bit());
    log.putChar('\n');
    return OK_CODE;
}

*/