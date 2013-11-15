#ifndef KKSDEBUG_H
#define KKSDEBUG_H

#include <QString>
#include <QTextStream>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QtDebug>

#include <QFile>
#include <QWidget>
#include "config_core.h"


enum __CORE_EXPORT KKSMsgType
{
    KKSDebugMsg         =   0, //отладочное
    KKSInfoMsg          =   1, //блок информации
    KKSImportantInfoMsg =   2, //блок важной информации, который также должен быть представлен в виде QMessageBox
    KKSWarningMsg       =   3, //предупреждение
    KKSCriticalMsg      =   4, //ошибка
    KKSFatalMsg         =   5  //системная ошибка
};


class __CORE_EXPORT KKSDebug
{
public:
    
    inline KKSDebug(KKSMsgType type) : ts(&m_buffer, QIODevice::WriteOnly){ m_msgType = type;}

    inline KKSDebug(const KKSDebug & d) : ts(&m_buffer, QIODevice::WriteOnly) { m_msgType = d.m_msgType;}

    inline ~KKSDebug(){
        if(this->m_buffer.isEmpty())
            return;
        this->qDebug() << m_buffer;
        print(m_buffer);
    }
    
    static void setMinMsgType(KKSMsgType t);
    static KKSMsgType minMsgType();
    static void setUseQDebug(bool b = true);
       
    inline KKSDebug & operator << (const QString & t)
    { 
        if(m_msgType < minMsgType())
            return *this;

        ts << t;

        return *this;
    }

    KKSDebug & operator << (const QByteArray & t);
    KKSDebug & operator << (int t);
    KKSDebug & operator << (QTextStreamFunction f); 
    KKSDebug & operator << (QTextStreamManipulator m);

private:

    QDebug qDebug();
    void print(const QString & msg);

    KKSMsgType m_msgType;
    
    QString m_buffer;
    QTextStream ts;

};

__CORE_EXPORT inline KKSDebug kksDebug() { return KKSDebug(KKSDebugMsg); }
__CORE_EXPORT inline KKSDebug kksWarning() { return KKSDebug(KKSWarningMsg); }
__CORE_EXPORT inline KKSDebug kksCritical() { return KKSDebug(KKSCriticalMsg); }
__CORE_EXPORT inline KKSDebug kksFatal() { return KKSDebug(KKSFatalMsg); }
__CORE_EXPORT inline KKSDebug kksInfo() { return KKSDebug(KKSInfoMsg); }
__CORE_EXPORT inline KKSDebug kksImportantInfo() { return KKSDebug(KKSImportantInfoMsg); }

#endif
