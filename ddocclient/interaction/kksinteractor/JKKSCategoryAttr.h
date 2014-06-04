/***********************************************************************
 * Module:  JKKSCategoryAttr.h
 * Author:  saa & yuriyrusinov
 * Modified: 30 марта 2009 г. 18:45:40
 * Purpose: Declaration of the class JKKSCategoryAttr
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSCategoryAttr_h)
#define __KKSSITOOOM_JKKSCategoryAttr_h

#include "kksinteractor_config.h"
#include "jkksuid.h"

#include <QString>
#include <QMap>

class QDataStream;
class QTextStream;
/*
 * Это атрибут
 */

class _I_EXPORT JKKSCategoryAttr : public JKKSUID
{
    public:
        //
        // Functions
        //
        JKKSCategoryAttr (qint64 idAttr=-1, 
                          qint64 idAttrType=-1, 
                          const QString& aCode=QString(), 
                          const QString& aName=QString(), 
                          const QString& aTitle=QString(), 
                          const QString& aTable=QString(), 
                          const QString& aTableUid=QString(),
                          const QString& aColumn=QString(), 
                          int aDefWidth=-1, 
                          const QString& aDefVal=QString(), 
                          bool isMandatory=false, 
                          bool isReadOnly=false,
                          const QString & uid = QString());

        JKKSCategoryAttr (const JKKSCategoryAttr& cAttr);
        ~JKKSCategoryAttr (void);

        qint64 id (void) const;
        void setId (qint64 id);

        qint64 idAttrType (void) const;
        void setIdAttrType (qint64 id);

        const QString & code (void) const;
        void setCode (const QString& nCode);

        const QString & name (void) const;
        void setName (const QString& nName);

        const QString & title (void) const;
        void setTitle (const QString& nTitle);

        const QString & table (void) const;
        void setTable (const QString& nTable);

        const QString& getTableUID (void) const;
        void setTableUid (const QString& table_uid);

        const QString & column (void) const;
        void setColumn (const QString& col);

        int defWidth (void) const;
        void setDefWidth (int w);

        const QString & defValue (void) const;
        void setDefValue (const QString& val);

        bool isMandatory (void) const;
        void setMandatory (bool v);

        bool isReadOnly (void) const;
        void setReadOnly (bool v);

        bool transferrable (void) const;
        void setTransferrable (bool v);

        const QString & attrAttrUid() const;
        void setAttrAttrUid(const QString & uid);

        void setAttrs(const QMap<qint64, JKKSCategoryAttr> & attrs);
        const QMap<qint64, JKKSCategoryAttr> & attrs() const;
        QMap<qint64, JKKSCategoryAttr> & attrs();

        bool operator< (const JKKSCategoryAttr& A) const;

    private:
        //
        // Functions
        //
        friend QDataStream& operator<< (QDataStream& out, const JKKSCategoryAttr& attr);
        friend QDataStream& operator>> (QDataStream& in, JKKSCategoryAttr& attr);
        QTextStream& loadFromBuffer (QTextStream& f, QString& str);

    private:
        //
        // Variables
        //
        qint64 m_idAttribute;
        qint64 m_idAttrType;
        //QString m_uid;
        QString m_aCode;
        QString m_aName;
        QString m_aTitle;
        QString m_aTable;
        QString m_aTableUid;//unique_id ИО, на таблицу которого ссылается данный атрибут (по нему будем получать настоящее название таблицы на приемном конце)
        QString m_aColumn;
        int m_aDefWidth;
        QString m_aDefValue;
        bool m_isMandatory;
        bool m_isReadOnly;
        QString m_attrAttrUid; //unique_id in attrs_attrs table. Если данный атрибут входит в состав составного

        QMap<qint64, JKKSCategoryAttr> m_attrs; //список атрибутов, описывающих данный атрибут. Для составных атрибутов

        bool m_transferrable;//специальный параметр, отвечающий за 
                             //синхронизацию только определенных колонок таблицы
                             //Если он задан как TRUE (значение по умолчанию), то
                             //колонка передается (синхронизируется)
                             //в противном случае колонка (ее значение) не синхронизируется
                             //предполагается, что на приемном конце будет либо NULL, 
                             //либо будет настроен триггер который задаст недостающее значение.
                             //Сделано это может быть при помощи механизма пользовательских обработчиков
};

QDataStream& operator<< (QDataStream& out, const JKKSCategoryAttr& attr);
QDataStream& operator>> (QDataStream& in, JKKSCategoryAttr& attr);

#endif
