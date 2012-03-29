/* 
 * File:   KKSAccessEntity.h
 * Author: yuriyrusinov
 *
 * Created on 29 Èþíü 2011 ã., 13:16
 */

#ifndef _KKSACCESSENTITY_H
#define	_KKSACCESSENTITY_H

#include <KKSRecord.h>
#include <KKSMap.h>
#include "kkspp_config.h"

class KKSPrivilege;

class _PP_EXPORT KKSAccessEntity : public KKSRecord
{
public:
    KKSAccessEntity (void);
    KKSAccessEntity (const KKSMap<int, KKSPrivilege*>& bPrivs,
                     const KKSMap<int, KKSPrivilege*>& uPrivs,
                     KKSPrivilege * otherPriv,
                     const KKSMap<int, KKSPrivilege*> privs);
    KKSAccessEntity (const KKSAccessEntity& anotherAccEnt);
    ~KKSAccessEntity (void);

    const KKSMap<int, KKSPrivilege*>& getBossPrivilegies (void) const;
    KKSMap<int, KKSPrivilege*>& getBossPrivilegies (void);
    void setBossPrivilegies (const KKSMap<int, KKSPrivilege*>& bossPList);

    const KKSMap<int, KKSPrivilege*>& getUnitPrivilegies (void) const;
    KKSMap<int, KKSPrivilege*>& getUnitPrivilegies (void);
    void setUnitPrivilegies (const KKSMap<int, KKSPrivilege*>& unitPList);

    KKSPrivilege * getOthersPrivilege (void);
    KKSPrivilege * getOthersPrivilege (void) const;
    void setOthersPrivilege (KKSPrivilege * p);

    const KKSMap<int, KKSPrivilege*>& getPrivilegies (void) const;
    KKSMap<int, KKSPrivilege*>& getPrivilegies (void);
    void setPrivilegies (const KKSMap<int, KKSPrivilege*>& pList);

    int addPrivilege(int idRole, KKSPrivilege * p);
    int removePrivilege(int idRole);
    int replacePrivilege(int idRole, KKSPrivilege * p);
    const KKSPrivilege * privilege(int idRole) const;
    KKSPrivilege * privilege(int idRole);

    int setBossPrivilege(int idRole, KKSPrivilege * p);
    int setUnitPrivilege(int idRole, KKSPrivilege * p);
    int addBossPrivilege(int idRole, KKSPrivilege * p);
    int addUnitPrivilege(int idRole, KKSPrivilege * p);

    virtual bool isNamed (void) const;

private:
    KKSMap<int, KKSPrivilege*> m_bossPrivs;
    KKSMap<int, KKSPrivilege*> m_unitPrivs;
    KKSPrivilege * otherPrivilege;
    KKSMap<int, KKSPrivilege*> m_Privs;
};



#endif	/* _KKSACCESSENTITY_H */

