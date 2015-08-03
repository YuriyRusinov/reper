/* 
 * File:   KKSAclTemplate.h
 * Author: rusinov
 *
 * Created on 6 Май 2011 г., 17:32
 */

#ifndef _KKSACLTEMPLATE_H
#define	_KKSACLTEMPLATE_H

#include <KKSRecord.h>
#include <KKSAccessEntity.h>
#include <KKSMap.h>
#include "kkspp_config.h"

class KKSPrivilege;

class _PP_EXPORT KKSAclTemplate : public KKSAccessEntity
{
public:
    KKSAclTemplate (int id=-1, QString name=QString(), bool isSys=false);
    KKSAclTemplate (const KKSAclTemplate& ACT);

/*    int id (void) const;
    void setId (int id);

    QString name (void) const;
    void setName (QString name);
*/
    bool isSystem (void) const;
    void setSystem (bool isSys);

    virtual bool isNamed (void) const;
/*
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
*/
private:
    //int aclId;
    //QString aclName;
    bool m_isSystem;

/*    KKSMap<int, KKSPrivilege*> bossPrivList;
    KKSMap<int, KKSPrivilege*> unitPrivList;
    KKSPrivilege * otherPrivilege;
    KKSMap<int, KKSPrivilege*> privList;
 */
};

#endif	/* _KKSACLTEMPLATE_H */

