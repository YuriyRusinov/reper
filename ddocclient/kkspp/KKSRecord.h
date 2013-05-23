/***********************************************************************
 * Module:  KKSRecord.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:11:43
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
    
    
    
    /* Возвращает название записи (ИО или экз. ИО) */
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
   qint64 m_id;//рабочий идентификатор записи, который используется в системе поддержания ссылочной целостности (первичный ключ)
   QDateTime m_lastUpdate;//дата и время последнего изменения записи
   KKSState * m_state; //состояние, в котором находится запись. По умолчанию = KKSState::defState1()
   QString m_uuid; //уникальый идентификатор записи в системе генерации уникальных идентификаторов uuid-ossp
   QString m_uniqueId; //уникальный идентификатор записи в системе генерации уникальных идентификаторов DynamicDocs

   bool m_isKKSState;
   
   
   
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
