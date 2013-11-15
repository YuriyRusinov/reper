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
    KKSDebugMsg         =   0, //����������
    KKSInfoMsg          =   1, //���� ����������
    KKSImportantInfoMsg =   2, //���� ������ ����������, ������� ����� ������ ���� ����������� � ���� QMessageBox
    KKSWarningMsg       =   3, //��������������
    KKSCriticalMsg      =   4, //������
    KKSFatalMsg         =   5  //��������� ������
};


class __CORE_EXPORT KKSDebug
{
public:
    
    KKSDebug(const KKSDebug & d);
    KKSDebug(KKSMsgType type);
    virtual ~KKSDebug();
    
    static void setMinMsgType(KKSMsgType t);
    static void setUseQDebug(bool b = true);
       
    KKSDebug & operator << (const QString & t);
    KKSDebug & operator << (const QByteArray & t);
    KKSDebug & operator << (int t);
    KKSDebug &operator<<(QTextStreamFunction f); 
    KKSDebug &operator<<(QTextStreamManipulator m);

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
