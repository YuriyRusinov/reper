/***********************************************************************
 * Module:  KKSObjectExemplar.h
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:12:00
 * Purpose: Declaration of the class KKSObjectExemplar
 * Comment: ���
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSObjectExemplar_h)
#define __KKSSITOOOM_KKSObjectExemplar_h

class KKSObject;
class KKSAttrValue;
class KKSCategoryAttr;

#include <KKSRecord.h>
#include "kkspp_config.h"
#include "KKSList.h"

class _PP_EXPORT KKSObjectExemplar : public KKSRecord
{
public:
   KKSObjectExemplar();
   KKSObjectExemplar(const KKSObjectExemplar & eio);
   KKSObjectExemplar(int id, const QString & name, KKSObject * io);
   virtual ~KKSObjectExemplar();

   //������ ��� ������ � ���������� ������� ������������ (��������� ������)
   const KKSList<KKSAttrValue *> & attrValues() const;
   KKSList<KKSAttrValue *> & attrValues();
   void setAttrValues(const KKSList<KKSAttrValue*> & _attrValues);

   KKSAttrValue * attrValueIndex(int index);
   const KKSAttrValue * attrValueIndex(int index) const;
   KKSAttrValue * attrValue(int id);
   const KKSAttrValue * attrValue(int id) const;
   
   void addAttrValue(KKSAttrValue * av);
   void addAttrValue(const QString & value, KKSCategoryAttr * attr, bool * bBadValue = NULL);
   void removeAttrValue(KKSAttrValue * av);
   void removeAttrValue(int index);

   //������ ��� ������ � ������������ ������� ������������
   const KKSList<KKSAttrValue *> & indValues() const;
   KKSList<KKSAttrValue *> & indValues();
   void setIndValues(const KKSList<KKSAttrValue*> & _indValues);

   KKSAttrValue * indValueIndex(int index);
   const KKSAttrValue * indValueIndex(int index) const;
   KKSAttrValue * indValue(int id);
   const KKSAttrValue * indValue(int id) const;
   
   void addIndValue(KKSAttrValue * av);
   void addIndValue(const QString & value, KKSCategoryAttr * attr, bool * bBadValue = NULL);
   void removeIndValue(KKSAttrValue * av);
   void removeIndValue(int index);


   KKSObject * io() const;
   void setIo(KKSObject * _io);

   //��� ����������� �������������� �������� (�.�. ������� � �������� ��)
   //��������� name, id, code, desc ������������ � ������ ���������, 
   //��������� ���� ������ ��������� ��������� ������ � ������� ��.
   //� ����� � ���� ����� �������� ��������������� � ���� ������� ����������
   //� ��������� � ������ ���������, ���������� �������������� ������������� ������,
   //����� ��� ����� ������������� � ����� �������� � ��������������� ������ ���������� ��������.
   //���� ���������������� �������� ��� - ���� ������������ ��������������, 
   //��� �������� ��������, �������� ��� � ������ ���������
   void setName(const QString & newName);
   void setId(int newId);
   void setDesc(const QString & newDesc);
   void setCode(const QString & newCode);


protected:
private:
   KKSObject* m_io;
   KKSList<KKSAttrValue*> m_attrValues;
   KKSList<KKSAttrValue*> m_indValues;//���������� ������� ������������


};

#endif
