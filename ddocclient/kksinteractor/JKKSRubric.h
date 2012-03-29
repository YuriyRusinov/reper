/***********************************************************************
 * Module: JKKSRubric.h
 * Author: yuriyrusinov
 * Modified: 27 августа 2009 г. 17:46:40
 * Purpose: Declaration of the class JKKSRubric
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSRubric_h)
#define __KKSSITOOOM_JKKSRubric_h

#include "kksinteractor_config.h"
/*
 * Это рубрикатор
 */

//#include "JKKSCategory.h"
//#include "JKKSSearchTemplate.h"

class JKKSCategory;
class JKKSGlobalRubric;

class _I_EXPORT JKKSRubric
{
public:
    JKKSRubric (int id=-1, 
                const QString & uid = QString(),
                int idp=-1, 
                int idc=-1, 
                const QString& name=QString(), 
                const QString& code=QString(), 
                const QString& desc=QString());
    JKKSRubric (const JKKSRubric& R);
    ~JKKSRubric (void);
    JKKSRubric& operator= (const JKKSRubric& R);

    int getId (void) const;
    void setId (int idr);

    const QString & getUid (void) const;
    void setUid (const QString & uid);

    int getParent (void) const;
    void setParent (int idp);

    int getEntity (void) const;
    void setEntity (int idc);

    const QString& getName (void) const;
    void setName (const QString& name);

    const QString& getCode (void) const;
    void setCode (const QString& code);

    const QString& getDesc (void) const;
    void setDesc (const QString& desc);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSRubric& R);
    friend QDataStream& operator>> (QDataStream& in, JKKSRubric& R);
    friend QDataStream& operator<< (QDataStream& out, const JKKSGlobalRubric& R);
    friend QDataStream& operator>> (QDataStream& in, JKKSGlobalRubric& R);


private:
    //
    // Variables
    //
    int idRubric;
    QString uidRubric;
    int idParent;
    int idEntity;
    QString rubricName;
    QString rubricCode;
    QString rubricDesc;

};

#endif
