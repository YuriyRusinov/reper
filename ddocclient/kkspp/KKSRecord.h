/***********************************************************************
 * Module:  KKSRecord.h
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:11:43
 * Purpose: Declaration of the class KKSRecord
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSRecord_h)
#define __KKSSITOOOM_KKSRecord_h

#include <QString>
#include <QDateTime>

#include "kkspp_config.h"
#include "KKSData.h"
#include <string>

class KKSState;

class _PP_EXPORT KKSRecord : public KKSData
{
public:

   qint64 id(void) const;
   virtual void setId(qint64 newId);

   const QDateTime & lastUpdate() const;
   virtual void setLastUpdate(const QDateTime & l);
   
   const KKSState * state () const;
   KKSState * state ();
   virtual void setState(KKSState * s);
   
   const QString & uuid() const;
   virtual void setUuid(const QString & u);

   const QString & uniqueId() const;
   virtual void setUniqueId(const QString & u);
    
    
    
    /* ���������� �������� ������ (�� ��� ���. ��) */
   const QString & name(void) const;
   virtual void setName(const QString & newName);
   const QString & desc(void) const;
   virtual void setDesc(const QString & newDesc);
   const QString & code(bool quoted = false) const;
   virtual void setCode(const QString & newCode);


   virtual void setParent(KKSRecord * p);
   const KKSRecord * parent() const;
   KKSRecord * parent();
   
   KKSRecord();
   KKSRecord(const KKSRecord & r);
   KKSRecord(qint64 id, const QString & name, const QString & desc = QString::null, const QString & code = QString::null, bool isKKSState = false);
   virtual ~KKSRecord();

protected:
private:
   qint64 m_id;//������� ������������� ������, ������� ������������ � ������� ����������� ��������� ����������� (��������� ����)
   QDateTime m_lastUpdate;//���� � ����� ���������� ��������� ������
   KKSState * m_state; //���������, � ������� ��������� ������. �� ��������� = KKSState::defState1()
   QString m_uuid; //��������� ������������� ������ � ������� ��������� ���������� ��������������� uuid-ossp
   QString m_uniqueId; //���������� ������������� ������ � ������� ��������� ���������� ��������������� DynamicDocs

   bool m_isKKSState;
   
   
   
   QString m_name;

   //������� ����� ��������� ���� �������. 
   //� ���� ������ ������ ��������� ������� "��������" � "�������� ������� �������". 
   //������ ���� ������ "�������� ������� �������"
   QString m_code;
   QString m_quotedCode;

   QString m_desc;
   
   KKSRecord* m_parent;




};

#endif
