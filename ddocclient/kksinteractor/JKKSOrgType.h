#if !defined (__KKSSITOOOM_JKKSOrgType_H)
#define __KKSSITOOOM_JKKSOrgType_H

#include "kksinteractor_config.h"
#include "jkksuid.h"

class QDataStream;

class _I_EXPORT JKKSOrgType : public JKKSUID
{
public:
    JKKSOrgType (int id_org_type=-1, const QString& orgTypeName=QString(), const QString& orgTypeSName=QString(), const QString& uid = QString());
    JKKSOrgType (const JKKSOrgType& orgType);
    ~JKKSOrgType (void);

    int getIdOrgType (void) const;
    void setIdOrgType (int id);

    QString getOrgTypeName (void) const;
    void setOrgTypeName (const QString& orgTypeName);

    QString getOrgTypeShortName (void) const;
    void setOrgTypeShortName (const QString& orgTypeSName);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSOrgType& orgType);
    friend QDataStream& operator>> (QDataStream& in, JKKSOrgType& orgType);

private:
    //
    // Variables
    //
    int idOrgType;
    QString organizationTypeName;
    QString organizationTypeShortName;
};

#endif
