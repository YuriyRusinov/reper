/***********************************************************************
 * Module:  JKKSMessage.h
 * Author:  saa & yuriyrusinov
 * Modified: 30 ����� 2009 �. 18:05:40
 * Purpose: Declaration of the class JKKSMessage
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSMessage_h)
#define __KKSSITOOOM_JKKSMessage_h

#include "kksinteractor_config.h"

#include <QString>
#include <QMap>

#include "JKKSCategory.h"

class JKKSCategory;
class JKKSLoader;

/*����� ���������� ���������*/
class _I_EXPORT JKKSAddress
{
public:
    JKKSAddress(const QString & addr, int port = 0, bool useGateway = false);
    JKKSAddress();
    JKKSAddress(const JKKSAddress & a);

    virtual ~JKKSAddress();

    const QString & address() const;
    int port() const;
    bool useGateway() const;

    void setAddress(const QString & addr);
    void setPort(int p);
    void setUseGateway(bool b);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSAddress& T);
    friend QDataStream& operator>> (QDataStream& in, JKKSAddress& T);

private:
    QString m_address;
    int m_port;
    bool m_useGateway;
};

/*������� ����� ��� ���������, ������������� ���������� (pure virtual)*/
class _I_EXPORT JKKSMessage
{
    public:
        JKKSMessage (const JKKSAddress & address=JKKSAddress(), const QString& code=QString());
        JKKSMessage (const JKKSMessage& mess);
        virtual ~JKKSMessage (void);

        const JKKSAddress & getAddr (void) const;
        void setAddr (const JKKSAddress & addr);

        QString getCode (void) const;
        void setCode (const QString& code);

        virtual QByteArray serialize (void) const;
        virtual int unserialize (const QByteArray& mess)=0;

        virtual const QMap<qint64, JKKSCategory>& getCategory (void) const;
        virtual void setCategory (const QMap<qint64, JKKSCategory>& catMap);
        
        virtual qint64 id (void) const=0;
        virtual int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID) = 0;

        //�����!!! 
        //��� �������� ������ ����� ���� ��� ������������ �������� ��������������� ����������� �������������� ������������� ���������
        //� ����� ���������� ������������� �������������� ������������� ��������� ����������� ��� ����� (��� �����), ��������������� ���
        //(�.�. �� ����� ������� ���� �������������). ��� ����������� �� ������ �� ������ ��������� ������������ ������ ����������� kksinteractor � http_connector
        //������ � ���� ��������� ���������, ����������� � ���� ���, �����������.
        //
        //������ ���������: mesid=<ID><type_digit_1><type_digit_2>&unp=<email_prefix>&data="........(base64)......"
        //���� ��� ����� ���� �����, ����� ��� �������� 0. �.�. 00 - atCommand, 02 - atDocument, � �.�.
        enum JKKSMessageType
        {
            atUnknownType = -1,
            atCommand = 0,
            atDocument = 1,
            atMailMessage = 2,
            atMailConfirmation = 3,
            atCmdConfirmation = 4,
            atRecord = 5,
            atRecConfirmation = 6,
            atCategory = 7,
            atOrganization = 8,
            atPosition = 10,
            atOrgPackage = 11,
            atFilePart = 12,
            atPing = 13,
            atPingResponse = 14,
            atEcho = 15 //���������� ������ �� ������� out_sync_queue � entity_type = 6 (�������� �����). ��������� ��� �����, ����� � DDocInteractorClient ����� ���� ��������� ����� ��������� ���� ��� "�������" �����
        }; 

        virtual JKKSMessageType getMessageType (void) const;


    private:
        JKKSAddress m_addr;
        QString m_kvs;
        QMap<qint64, JKKSCategory> c;
};

#endif
