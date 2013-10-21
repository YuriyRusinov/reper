#if !defined (__KKSSITOOOM_JKKSType_H)
#define __KKSSITOOOM_JKKSType_H

#include "kksinteractor_config.h"
#include "jkksuid.h"

class QDataStream;

class _I_EXPORT JKKSType : public JKKSUID
{
public:
    JKKSType (qint64 idType=-1, const QString& name=QString(), const QString& description=QString(), const QString& uid = QString());
    JKKSType (const JKKSType& orgType);
    ~JKKSType (void);

    qint64 getIdType (void) const;
    void setIdType (qint64 id);

    const QString & getName (void) const;
    void setName (const QString& name);

    const QString & getDesc (void) const;
    void setDesc(const QString& description);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSType& type);
    friend QDataStream& operator>> (QDataStream& in, JKKSType& type);

private:
    //
    // Variables
    //
    qint64 m_id;
    QString m_name;
    QString m_desc;
};

#endif
