/***********************************************************************
 * Module: JKKSIOTable.h
 * Author: yuriyrusinov
 * Modified: 10 июня 2010 г. 17:47
 * Purpose: Declaration of the class JKKSIOTable
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSIOTable_h)
#define __KKSSITOOOM_JKKSIOTable_h

#include <QMap>
#include <QString>

#include "kksinteractor_config.h"
#include "jkksuid.h"
/*
 * Опциональная дополнительная таблица документов-справочников для передачи по Юпитеру
 */
class JKKSCategory;

class _I_EXPORT JKKSIOTable : public JKKSUID
{
public:
    JKKSIOTable (qint64 idIOTable = -1,
                 const QString & table_name = QString(),
//                 const QMap<int, JKKSCategory>& catMap,// = QMap<int, JKKSCategory>(),
                 const QString & title = QString(),
                 const QString & uid = QString()
                 );
    JKKSIOTable (const JKKSIOTable& T);

    ~JKKSIOTable (void);

    qint64 getIDTable (void) const;
    void setIDTable (qint64 _id);

    QString getName (void) const;
    void setName (QString tName);

    const QMap<qint64, JKKSCategory>& getCategory (void) const;
    void setCategory (const QMap<qint64, JKKSCategory>& catMap);

    QString getTitle (void) const;
    void setTitle (QString tTitle);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSIOTable& T);
    friend QDataStream& operator>> (QDataStream& in, JKKSIOTable& T);

private:
    //
    // Variables
    //
    qint64 id;
    QString tableName;
    QMap<qint64, JKKSCategory> category;
    QString Title;
};

#endif
