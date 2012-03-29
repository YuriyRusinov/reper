#include "KKSPrivilege.h"
#include "KKSAccessEntity.h"
#include <defines.h>

KKSAccessEntity :: KKSAccessEntity ()
    : KKSRecord (),
      m_bossPrivs (KKSMap<int, KKSPrivilege*>()),
      m_unitPrivs (KKSMap<int, KKSPrivilege*>()),
      otherPrivilege (new KKSPrivilege(OTHERS_ROLE, true, true, false, false, true)),
      m_Privs (KKSMap<int, KKSPrivilege*>())
{
}

KKSAccessEntity :: KKSAccessEntity (const KKSMap<int, KKSPrivilege*>& bPrivs,
                                    const KKSMap<int, KKSPrivilege*>& uPrivs,
                                    KKSPrivilege * otherPriv,
                                    const KKSMap<int, KKSPrivilege*> privs)
    : KKSRecord(),
      m_bossPrivs (bPrivs),
      m_unitPrivs (uPrivs),
      otherPrivilege (otherPriv),
      m_Privs (privs)
{
    if (otherPrivilege)
        otherPrivilege->addRef ();
}

KKSAccessEntity :: KKSAccessEntity (const KKSAccessEntity& anotherAccEnt)
    : KKSRecord (anotherAccEnt),
      m_bossPrivs (anotherAccEnt.m_bossPrivs),
      m_unitPrivs (anotherAccEnt.m_unitPrivs),
      otherPrivilege (anotherAccEnt.otherPrivilege),
      m_Privs (anotherAccEnt.m_Privs)
{
    if (otherPrivilege)
        otherPrivilege->addRef ();
}

KKSAccessEntity :: ~KKSAccessEntity (void)
{
    if (otherPrivilege)
        otherPrivilege->release ();
}

const KKSMap<int, KKSPrivilege*>& KKSAccessEntity :: getBossPrivilegies (void) const
{
    return m_bossPrivs;
}

KKSMap<int, KKSPrivilege*>& KKSAccessEntity :: getBossPrivilegies (void)
{
    return m_bossPrivs;
}

void KKSAccessEntity :: setBossPrivilegies (const KKSMap<int, KKSPrivilege*>& bossPList)
{
    m_bossPrivs = bossPList;
}

const KKSMap<int, KKSPrivilege*>& KKSAccessEntity :: getUnitPrivilegies (void) const
{
    return m_unitPrivs;
}

KKSMap<int, KKSPrivilege*>& KKSAccessEntity :: getUnitPrivilegies (void)
{
    return m_unitPrivs;
}

void KKSAccessEntity :: setUnitPrivilegies (const KKSMap<int, KKSPrivilege*>& unitPList)
{
    m_unitPrivs = unitPList;
}

KKSPrivilege * KKSAccessEntity :: getOthersPrivilege (void)
{
    return otherPrivilege;
}

KKSPrivilege * KKSAccessEntity :: getOthersPrivilege (void) const
{
    return otherPrivilege;
}

void KKSAccessEntity :: setOthersPrivilege (KKSPrivilege * p)
{
    if (otherPrivilege)
        otherPrivilege->release ();

    otherPrivilege = p;

    if (otherPrivilege)
        otherPrivilege->addRef ();
}

const KKSMap<int, KKSPrivilege*>& KKSAccessEntity :: getPrivilegies (void) const
{
    return m_Privs;
}

KKSMap<int, KKSPrivilege*>& KKSAccessEntity :: getPrivilegies (void)
{
    return m_Privs;
}

void KKSAccessEntity :: setPrivilegies (const KKSMap<int, KKSPrivilege*>& pList)
{
    m_Privs = pList;
}

int KKSAccessEntity :: addPrivilege (int idRole, KKSPrivilege * p)
{
    int cnt = m_Privs.insert (idRole, p);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;

}

int KKSAccessEntity :: removePrivilege (int idRole)
{
    m_Privs.remove(idRole);
    return OK_CODE;

}

int KKSAccessEntity :: replacePrivilege (int idRole, KKSPrivilege * p)
{
    removePrivilege (idRole);
    int cnt = addPrivilege (idRole, p);

    return cnt == 0 ? ERROR_CODE : OK_CODE;
}

const KKSPrivilege * KKSAccessEntity :: privilege(int idRole) const
{
    KKSPrivilege * p = NULL;

    QMap<int, KKSPrivilege*>::const_iterator i = m_Privs.find(idRole);
    while (i != m_Privs.end() && i.key() == idRole) {
        p = i.value();
        //берем всегда первый элемент
        break;
    }

    return p;
}

KKSPrivilege * KKSAccessEntity :: privilege(int idRole)
{
    KKSPrivilege * p = NULL;

    QMap<int, KKSPrivilege*>::const_iterator i = m_Privs.find(idRole);
    while (i != m_Privs.end() && i.key() == idRole) {
        p = i.value();
        //берем всегда первый элемент
        break;
    }

    return p;
}

int KKSAccessEntity :: setBossPrivilege(int idRole, KKSPrivilege * p)
{
    if(!p)
        return ERROR_CODE;

    KKSPrivilege * pr = m_bossPrivs.value (idRole, 0);
    if(!pr)
        return ERROR_CODE;

    m_bossPrivs.remove(idRole);
    int cnt = m_bossPrivs.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;


    return OK_CODE;
}

int KKSAccessEntity :: setUnitPrivilege(int idRole, KKSPrivilege * p)
{
    if(!p)
        return ERROR_CODE;

    KKSPrivilege * pr = m_unitPrivs.value (idRole, 0);
    if(!pr)
        return ERROR_CODE;

    m_unitPrivs.remove(idRole);
    int cnt = m_unitPrivs.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;


    return OK_CODE;
}

int KKSAccessEntity :: addBossPrivilege(int idRole, KKSPrivilege * p)
{
    int cnt = m_bossPrivs.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSAccessEntity :: addUnitPrivilege(int idRole, KKSPrivilege * p)
{
    int cnt = m_unitPrivs.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;
}

bool KKSAccessEntity :: isNamed (void) const
{
    return false;
}
