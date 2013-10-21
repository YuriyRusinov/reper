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
    JKKSRubric (qint64 id=-1, 
                const QString & uid = QString(),
                qint64 idp=-1, 
                qint64 idc=-1, 
                const QString& name=QString(), 
                const QString& code=QString(), 
                const QString& desc=QString());

    JKKSRubric (const JKKSRubric& R);
    ~JKKSRubric (void);
    JKKSRubric& operator= (const JKKSRubric& R);

    qint64 getId (void) const;
    void setId (qint64 idr);

    const QString & getUid (void) const;
    void setUid (const QString & uid);

    qint64 getParent (void) const;
    void setParent (qint64 idp);

    qint64 getEntity (void) const;
    void setEntity (qint64 idc);

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
    qint64 idRubric;
    QString uidRubric;
    qint64 idParent;
    qint64 idEntity;
    QString rubricName;
    QString rubricCode;
    QString rubricDesc;

};

#endif
