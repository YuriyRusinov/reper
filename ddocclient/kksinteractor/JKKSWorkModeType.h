/* 
 * File:   JKKSWorkModeType.h
 * Author: yuriyrusinov
 *
 * Created on 28 Èþëü 2011 ã., 11:09
 */

#if !defined (__KKSSITOOOM_JKKSWORKMODETYPE_H)
#define	__KKSSITOOOM_JKKSWORKMODETYPE_H

#include "kksinteractor_config.h"
#include "jkksuid.h"

class _I_EXPORT JKKSWorkModeType : public JKKSUID
{
public:
    JKKSWorkModeType (int id=-1, const QString& nameType=QString(), const QString& shortNameType=QString(), const QString& uid = QString());
    JKKSWorkModeType (const JKKSWorkModeType& wmType);
    ~JKKSWorkModeType (void);

    int id (void) const;
    void setId (int _id);

    const QString& getName (void) const;
    void setName (const QString& name);

    const QString& getShortName (void) const;
    void setShortName (const QString& name);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSWorkModeType& wmType);
    friend QDataStream& operator>> (QDataStream& in, JKKSWorkModeType& wmType);

private:
    //
    // Variables
    //
    int idWMType;
    QString wmNameType;
    QString wmShortNameType;
};


#endif	/* __KKSSITOOOM_JKKSWORKMODETYPE_H */

