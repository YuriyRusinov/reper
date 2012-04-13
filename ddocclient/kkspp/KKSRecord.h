/***********************************************************************
 * Module:  KKSRecord.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:11:43
 * Purpose: Declaration of the class KKSRecord
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSRecord_h)
#define __KKSSITOOOM_KKSRecord_h

#include <QString>

#include "kkspp_config.h"
#include "KKSData.h"
#include <string>
class _PP_EXPORT KKSRecord : public KKSData
{
public:
   /* Возвращает название записи (ИО или экз. ИО) */
   const QString & name(void) const;
   virtual void setName(const QString & newName);
   int id(void) const;
   virtual void setId(int newId);
   const QString & desc(void) const;
   virtual void setDesc(const QString & newDesc);
   const QString & code(bool quoted = false) const;
   virtual void setCode(const QString & newCode);


   virtual void setParent(KKSRecord * p);
   const KKSRecord * parent() const;
   KKSRecord * parent();
   
   KKSRecord();
   KKSRecord(const KKSRecord & r);
   KKSRecord(int id, const QString & name, const QString & desc = QString::null, const QString & code = QString::null);
   virtual ~KKSRecord();

protected:
private:
   int m_id;
   QString m_name;

   //Атрибут может описывать поле таблицы. 
   //В этом случае удобно разделить понятие "Название" и "Название Колонки Таблицы". 
   //Данное поле хранит "Название Колонки Таблицы"
   QString m_code;
   QString m_quotedCode;

   QString m_desc;
   
   KKSRecord* m_parent;


};

#endif
