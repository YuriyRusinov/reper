/***********************************************************************
 * Module:  KKSPrivilege.cpp
 * Author:  ksa
 * Modified: 19 декабря 2008 г. 16:57:28
 * Purpose: Implementation of the class KKSPrivilege
 ***********************************************************************/

#include "KKSPrivilege.h"
#include "defines.h"

KKSPrivilege::KKSPrivilege() : KKSData()
{
    m_idRole = -1;
    clear();
}

KKSPrivilege::KKSPrivilege(int idRole,
                           bool allowReadList, 
                           bool allowRead, 
                           bool allowDelete, 
                           bool allowUpdate,
                           bool allowSend)
            : KKSData(),
            m_allowReadList (allowReadList),
            m_allowRead (allowRead),
            m_allowDelete (allowDelete),
            m_allowUpdate (allowUpdate),
            m_allowSend (allowSend),
            m_idRole (idRole)
{
}

KKSPrivilege::KKSPrivilege(const KKSPrivilege & copy)
            : KKSData(),
            m_allowReadList (copy.m_allowReadList),
            m_allowRead (copy.m_allowRead),
            m_allowDelete (copy.m_allowDelete),
            m_allowUpdate (copy.m_allowUpdate),
            m_allowSend (copy.m_allowSend),
            m_idRole (copy.m_idRole)
            
{
}

KKSPrivilege::~KKSPrivilege()
{
}


void KKSPrivilege::clear()
{
    m_allowDelete = false;
    m_allowRead = false;
    m_allowUpdate = false;
    m_allowReadList = false;
    m_allowSend = false;
}

bool KKSPrivilege::allowDelete() const
{
    return m_allowDelete;
}

bool KKSPrivilege::allowRead() const
{
    return m_allowRead;
}

bool KKSPrivilege::allowReadList() const
{
    return m_allowReadList;
}

bool KKSPrivilege::allowUpdate() const
{
    return m_allowUpdate;
}

bool KKSPrivilege::allowSend () const
{
    return m_allowSend;
}

int KKSPrivilege::idRole() const
{
    return m_idRole;
}

void KKSPrivilege::setRole(int idRole)
{
    m_idRole = idRole;
}

void KKSPrivilege::setPrivileges(bool allowReadList, 
                                 bool allowRead, 
                                 bool allowDelete, 
                                 bool allowUpdate,
                                 bool allowSend)
{
    m_allowDelete = allowDelete;
    m_allowRead = allowRead;
    m_allowReadList = allowReadList;
    m_allowUpdate = allowUpdate;
    m_allowSend = allowSend;
}

bool operator== (const KKSPrivilege P1, const KKSPrivilege P2)
{
    return P1.m_idRole == P2.m_idRole && P1.m_allowReadList == P2.m_allowReadList && P1.m_allowRead == P2.m_allowRead && P1.m_allowUpdate == P2.m_allowUpdate && P1.m_allowDelete == P2.m_allowDelete && P1.m_allowSend == P2.m_allowSend;
}

bool operator!= (const KKSPrivilege P1, const KKSPrivilege P2)
{
    return !(P1 == P2);
}

KKSPrivilege * KKSPrivilege::authPrivilege (int idAuthor)
{
    KKSPrivilege * p = new KKSPrivilege (idAuthor, true, true, true, true, true);
    return p;
}

KKSPrivilege * KKSPrivilege::bossPrivilege (int idBoss)
{
    KKSPrivilege * p = new KKSPrivilege (idBoss, true, true, false, false, false);
    return p;
}

KKSPrivilege * KKSPrivilege::unitPrivilege (int idUnit)
{
    KKSPrivilege * p = new KKSPrivilege (idUnit, true, true, false, false, false);
    return p;
}