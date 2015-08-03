/* 
 * File:   JKKSPing.h
 * Author: ksa
 *
 * Created on 29.10.2013 �., 12:17
 */

#if !defined (__KKSSITOOOM_JKKSPING_H)
#define	__KKSSITOOOM_JKKSPING_H

#include "kksinteractor_config.h"
#include "JKKSMessage.h"

class _I_EXPORT JKKSPing : public JKKSMessage
{
public:
    JKKSPing (qint64 id=-1, 
              const JKKSAddress & addr=JKKSAddress(), 
              const QString & kvs=QString(),
              bool isResponse = false);
    JKKSPing (const JKKSPing & ping);
    ~JKKSPing (void);

    qint64 id (void) const;
    void setId (qint64 _id);

    QByteArray serialize (void) const;
    int unserialize (const QByteArray& mess);

    int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);

    JKKSMessageType getMessageType (void) const;

    qint64 idOrgTo() const;
    void setIdOrgTo(qint64 id);

    qint64 idOrgFrom() const;
    void setIdOrgFrom(qint64 id);

    const QString & versionFrom() const;
    const QString & versionTo() const;
    void setVersionFrom(const QString & v);
    void setVersionTo(const QString & v);

    const QString & uidFrom() const;
    const QString & uidTo() const;
    void setUidFrom(const QString & u);
    void setUidTo(const QString & u);

    const QString & nameFrom() const;
    const QString & nameTo() const;
    void setNameFrom(const QString & n);
    void setNameTo(const QString & n);

    int state1() const;
    int state2() const;
    int state3() const;
    int state4() const;

    void setState1(int s);
    void setState2(int s);
    void setState3(int s);
    void setState4(int s);

    int created() const;
    bool completed() const;
    void setCreated(int c);
    void setCompleted(bool c);

    //void setSenderAddress(const JKKSAddress & addr);
    //const JKKSAddress & senderAddress() const;

    bool operator == (const JKKSPing & in) const;
    JKKSPing & operator = ( const JKKSPing & in );

private:
    //
    // Variables
    //
    qint64 m_id; //id ������ �����. ������������ ��� ���������� �����
    qint64 m_idOrgTo; //id ������� �����������, � ������� ������������ ����
    qint64 m_idOrgFrom;//id �����������, ������� ��������� ����

    QString m_uidFrom;//���������� ������������� �����������, ������� ��������� ����
    QString m_uidTo;//���������� ������������� �����������, � ������� ������������ ����

    QString m_nameFrom;//�������� �����������, ������� ��������� ����
    QString m_nameTo;//�������� �����������, � ������� ������������ ����

    QString m_versionFrom;//������ �� DynamicDocs �����������, ������� ��������� ����
    QString m_versionTo; //������ �� DynamicDocs �����������, � ������� ������������ ����

    //��������� ������� �����������
    //0 - ��������� (��� ��������� ��������� �� �������)
    //1 - �������
    //2 - �� ������������ (��� m_state2)
    int m_state1; //������� �� http_connector, ������� � ��� ��������
    int m_state2; //������� �� ����� ���, ���� �������������� ���� ����� ���� ���. ����� ��������, ������ �� ����� ��� ��������� ������ �� �������� http_connector'�
    int m_state3; //������� �� �� DynamicDocs ������� �����������
    int m_state4; //������������� �� ������ �� DynamicDocs ������� ���������� ������ ��, ������� ��������� ����

    //������ ���� ������ ������� (� ���������) �������� ����� � ������ ������������.
    //��������� ��������:
    //0 - ��������� �������� ����� (�������� �� ���������)
    //1 - �������� ����� ��������� �������
    //2 - �������� ����� ����������� � �������
    //�������� ����� ������ � �������������, ������� ����� 1 � �������� �������� ������� ����
    int m_created;

    //����, ������� ����������, ��� �������� � ����������� ����� ����������� ���������. 
    //��� ����� �������� ������� ����� � FALSE �� TRUE ����������� �������� �������� ����� � ��������� ������������.
    //���� �������� ��� ������� ��� ������ ����������� ���� �� ������ ���������� � ���������� ������ (� ����������� organization_transport)
    bool m_completed;

    bool m_isResponse;//���� true - �� �������� ������� �� ���� (� ���� ������ ��� ��������� ����� �� ��������
    //JKKSAddress m_senderAddress;//����� ����������� �����. ����� ������������ �� �������� ����� � ������� queue_results
};


#endif	/* __KKSSITOOOM_JKKSPING_H */

