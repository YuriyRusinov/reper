/***********************************************************************
 * Module:  JKKSMessage.h
 * Author:  saa & yuriyrusinov
 * Modified: 07 ������ 2009 �. 11:10:40
 * Purpose: Declaration of the class JKKSPMessage
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSPMessage_h)
#define __KKSSITOOOM_JKKSPMessage_h

#include "kksinteractor_config.h"

#include <QByteArray>
#include <QString>
#include <QBuffer>
#include <QDataStream>
#include <QStringList>
#include <QDebug>

#include "JKKSMessage.h"

class _I_EXPORT JKKSPMessage
{
    public:
        JKKSPMessage (const QByteArray& mess, 
                      const int & type, 
                      const QString& sender_uid=QString(),
                      const QString& receiver_uid=QString());

        JKKSPMessage (void);
        JKKSPMessage (const JKKSPMessage& pmess);
        JKKSPMessage (const QByteArray& mess);

        QByteArray getData (void) const;
        int getType (void) const;
        void setType(int t);
        QByteArray serialize (void) const;

        QStringList getFiles (void) const;
        void setFiles (const QStringList& files);

        const QByteArray & cryptoHash() const; //hash-�����. ������������ ����������� ��������. ���� ��� ������, �� ������� ���������������
        const QByteArray & calculateCryptoHash() const;//���������� hash-�����. ���������� ����� serialize()


        unsigned char urgencyLevel() const {return m_urgencyLevel;}//��������� ���������. ������������ � �������, �������������� ��� ���������� ������
        unsigned char macLabel() const {return m_macLabel;}//���� �����������. ������������ � �������, �������������� ��� ���������� ������

        void setUrgencyLevel(const QString & code);//������� ������ ������
        void setMacLabel(const QString & mac);//������� ������ ������

        const QString & senderUID() const{return m_senderUID;}
        const QString & receiverUID() const {return m_receiverUID;}
        bool verifyReceiver() const {return m_verifyReceiver;}

        void setVerifyReceiver(bool b) {m_verifyReceiver = b;}
        void setSenderUID(const QString & s) {m_senderUID = s;}
        void setReceiverUID(const QString & s) {m_receiverUID = s;}


private:
        QByteArray data;
        int messType;
        QStringList urlList;//�����
        mutable QByteArray m_cryptoHash;// hash-����� ����������� ���������

    //public:
        QString m_senderUID;//���������� ������������� �����������-�����������
        QString m_receiverUID;//���������� ������������� �����������-����������
        bool m_verifyReceiver;//���� ������������� �������� ���������� �� �������� ����� (�.�. ��� ������� ��������������� ������ ��� �����������, ������� ��������� �� �������� �����
        unsigned char m_urgencyLevel;//�������� � ���� code ������� urgency_levels (������ ���� A, B, C, �������������� "������������", "�������", "�������")
        unsigned char m_macLabel;//������������ �������� ������� (������ ���� 0, 1, 2, 3 - ��, �, ��, ��)
        
};

class _I_EXPORT JKKSPMessWithAddr
{
    public:
        JKKSPMessage pMess;
        JKKSAddress addr; //����� (IP+����), �� ������� ������������ ���������
        QString unp;// �������� ����� ���������� ���������. � �������� �������� ������������ ���� email_prefix ����������� (� ��������� ����� ������������ ������ ��� �������� ����� ���)
        qint64 id; //������������� ��������� �� ������� out_sync_queue ��� message_journal ��� command_journal ��� out_external_queue

        JKKSPMessWithAddr (void);
        JKKSPMessWithAddr (const JKKSPMessage& pm, const JKKSAddress & a, const qint64& c);
        JKKSPMessWithAddr (JKKSPMessage *, const JKKSAddress & a, const qint64& c);
        ~JKKSPMessWithAddr (void);
};

#endif
