
#include "kksdebug.h"
#include "kkssito.h"
#include <defines.h>
#include "KKSDbgOutputWidget.h"

#include <QDate>
#include <QDateTime>
#include <QTime>

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

KKSDebug::KKSDebug(KKSMsgType type) : ts(&m_buffer, QIODevice::WriteOnly)
{
    m_msgType = type;
}

KKSDebug::KKSDebug(const KKSDebug & d) : ts(&m_buffer, QIODevice::WriteOnly)
{
    m_msgType = d.m_msgType;
}

KKSDebug::~KKSDebug()
{
    if(this->m_buffer.isEmpty())
        return;

    this->qDebug() << m_buffer;
    
    print(m_buffer);
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

    ts << t;

    return *this;
}

KKSDebug & KKSDebug::operator << (const QByteArray & t) 
{ 
    if(m_msgType < m_minMsgType)
        return *this;

    ts << t;

    return *this;
}

KKSDebug & KKSDebug::operator << (QTextStreamFunction f)
{
    if(m_msgType < m_minMsgType)
        return *this;

    ts << f;
    return *this;
}

KKSDebug & KKSDebug::operator << (QTextStreamManipulator m)
{ 
    if(m_msgType < m_minMsgType)
        return *this;
    ts << m; 
    return *this; 
}

KKSDebug & KKSDebug::operator << (int t) 
{ 
    if(m_msgType < m_minMsgType)
        return *this;

    ts << t;
    
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
