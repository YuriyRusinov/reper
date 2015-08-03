#ifndef _KKS_ADD_TABLE_H
#define _KKS_ADD_TABLE_H

#include <KKSRecord.h>
#include "kkspp_config.h"

class KKSCategory;

class _PP_EXPORT KKSAddTable : public KKSRecord
{
public:
    KKSAddTable (int _idObject, 
                 int idt, 
                 const QString& table_name, 
                 const KKSCategory * c, 
                 const QString& ttitle,
                 int idSearchTemplate,
                 const QString & refTableName);
    KKSAddTable (const KKSAddTable& t);
    virtual ~KKSAddTable (void);

    int idIO (void) const;

    int id (void) const;
    void setId (int idt);

    QString getTableName (void) const;
    void setTableName (const QString& table_name);

    const KKSCategory * category (void) const;
    void setCategory (const KKSCategory * c);

    QString getTitle (void);
    void setTitle (const QString& ttitle);

    QString getRefTableName (void) const;
    void setRefTableName (const QString& ref_table_name);

    int idSearchTemplate (void) const;
    void setIdSearchTemplate (int idSt);

protected:
private:

    int idObject;
    int idTable;
    QString tableName;
    const KKSCategory * cat;
    QString title;

    int m_idSearchTemplate;
    QString m_refTableName;
};
#endif
