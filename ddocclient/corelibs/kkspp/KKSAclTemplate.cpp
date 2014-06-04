#include "KKSPrivilege.h"
#include "KKSAclTemplate.h"

KKSAclTemplate :: KKSAclTemplate (int id, QString name, bool isSys)
    : KKSAccessEntity(),
      //aclId (id),
      //aclName (name),
      m_isSystem (isSys)
{
    Q_UNUSED(id);
}

KKSAclTemplate :: KKSAclTemplate (const KKSAclTemplate& ACT)
    : KKSAccessEntity (ACT),
      //aclId (ACT.aclId),
      //aclName (ACT.aclName),
      m_isSystem (ACT.m_isSystem)
{
}
/*
int KKSAclTemplate :: id (void) const
{
    return aclId;
}

void KKSAclTemplate :: setId (int id)
{
    aclId = id;
}

QString KKSAclTemplate :: name (void) const
{
    return aclName;
}

void KKSAclTemplate :: setName (QString name)
{
    aclName = name;
}
*/
bool KKSAclTemplate :: isSystem (void) const
{
    return m_isSystem;
}

void KKSAclTemplate :: setSystem (bool isSys)
{
    m_isSystem = isSys;
}
/*
const KKSMap<int, KKSPrivilege*>& KKSAclTemplate :: getPrivilegies (void) const
{
    return privList;
}

KKSMap<int, KKSPrivilege*>& KKSAclTemplate :: getPrivilegies (void)
{
    return privList;
}

void KKSAclTemplate :: setPrivilegies (const KKSMap<int, KKSPrivilege*>& pList)
{
    privList = pList;
}

const KKSMap<int, KKSPrivilege*>& KKSAclTemplate :: getBossPrivilegies (void) const
{
    return bossPrivList;
}

KKSMap<int, KKSPrivilege*>& KKSAclTemplate :: getBossPrivilegies (void)
{
    return bossPrivList;
}

void KKSAclTemplate :: setBossPrivilegies (const KKSMap<int, KKSPrivilege*>& bossPList)
{
    bossPrivList = bossPList;
}

const KKSMap<int, KKSPrivilege*>& KKSAclTemplate :: getUnitPrivilegies (void) const
{
    return unitPrivList;
}

KKSMap<int, KKSPrivilege*>& KKSAclTemplate :: getUnitPrivilegies (void)
{
    return unitPrivList;
}

void KKSAclTemplate :: setUnitPrivilegies (const KKSMap<int, KKSPrivilege*>& unitPList)
{
    unitPrivList = unitPList;
}

KKSPrivilege * KKSAclTemplate :: getOthersPrivilege (void)
{
    return otherPrivilege;
}

KKSPrivilege * KKSAclTemplate :: getOthersPrivilege (void) const
{
    return otherPrivilege;
}

void KKSAclTemplate :: setOthersPrivilege (KKSPrivilege * p)
{
    if (otherPrivilege)
        otherPrivilege->release ();

    otherPrivilege = p;

    if (otherPrivilege)
        otherPrivilege->addRef ();
}
 */

bool KKSAclTemplate :: isNamed (void) const
{
    return true;
}