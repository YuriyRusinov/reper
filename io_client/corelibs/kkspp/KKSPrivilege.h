/***********************************************************************
 * Module:  KKSPrivilege.h
 * Author:  ksa
 * Modified: 19 декабря 2008 г. 16:57:28
 * Purpose: Declaration of the class KKSPrivilege
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSPrivilege_h)
#define __KKSSITOOOM_KKSPrivilege_h

//#include <QMap>
//#include <QString>

#include "KKSData.h"
#include "KKSMap.h"
#include "kkspp_config.h"

class _PP_EXPORT KKSPrivilege : public KKSData
{
public:
    KKSPrivilege();
    KKSPrivilege(int idRole,
                 bool allowReadList, 
                 bool allowRead, 
                 bool allowDelete, 
                 bool allowUpdate,
                 bool allowSend);

    KKSPrivilege(const KKSPrivilege & copy);
    ~KKSPrivilege();
    
    bool allowReadList() const;
    bool allowRead() const;
    bool allowDelete() const;
    bool allowUpdate() const;
    bool allowSend () const;

    int idRole() const;

    void setPrivileges(bool allowReadList, 
                       bool allowRead, 
                       bool allowDelete, 
                       bool allowUpdate,
                       bool allowSend);

    void setRole(int idRole);

    void clear();
    friend bool operator== (const KKSPrivilege P1, const KKSPrivilege P2);
    friend bool operator!= (const KKSPrivilege P1, const KKSPrivilege P2);

    static KKSPrivilege * authPrivilege (int idAuthor);
    static KKSPrivilege * bossPrivilege (int idBoss);
    static KKSPrivilege * unitPrivilege (int idUnit);

protected:
private:

    bool m_allowReadList;
    bool m_allowRead;
    bool m_allowDelete;
    bool m_allowUpdate;
    bool m_allowSend;

    int m_idRole;
    
};


#endif
