#if !defined (__KKSSITOOOM_JKKSType_H)
#define __KKSSITOOOM_JKKSType_H

#include "kksinteractor_config.h"
#include "jkksuid.h"

class QDataStream;

class _I_EXPORT JKKSType : public JKKSUID
{
public:
    JKKSType (int idType=-1, const QString& name=QString(), const QString& description=QString(), const QString& uid = QString());
    JKKSType (const JKKSType& orgType);
    ~JKKSType (void);

    int getIdType (void) const;
    void setIdType (int id);

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
    int m_id;
    QString m_name;
    QString m_desc;
};

#endif
