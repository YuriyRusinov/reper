/***********************************************************************
 * Module: JKKSSearchGroup.h
 * Author: yuriyrusinov
 * Modified: 09 февраля 2009 г. 12:18:00
 * Purpose: Declaration of the class JKKSSearchGroup
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSSearchGroup_h)
#define __KKSSITOOOM_JKKSSearchGroup_h

#include "kksinteractor_config.h"
#include <QMap>
#include "JKKSSearchCriterion.h"

class QDataStream;

/*
 * Это группа критериев поиска
 */

//class JKKSSearchCriterion;

class _I_EXPORT JKKSSearchGroup
{
public:
    JKKSSearchGroup (qint64 id=-1, qint64 id_parent=-1, int oper=-1, bool is_not=false, int gr_type=-1);
    JKKSSearchGroup (const JKKSSearchGroup& SG);
    ~JKKSSearchGroup (void);

    qint64 id (void) const;
    void setId (qint64 id);

    qint64 idParent (void) const;
    void setParent (qint64 id_parent);

    int getOper (void) const;
    void setOper (int op);

    bool isNegative (void) const;
    void setNegative (bool is_not);

    int getType (void) const;
    void setType (int type);

    const QMap<qint64, JKKSSearchCriterion>& getCriteria (void) const;
    QMap<qint64, JKKSSearchCriterion>& getCriteria (void);
    void setCriteria (const QMap<qint64, JKKSSearchCriterion>& crit);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSSearchGroup& sg);
    friend QDataStream& operator>> (QDataStream& in, JKKSSearchGroup& sg);

private:
    //
    // Variables
    //
    qint64 idGroup;
    qint64 idParentGroup;
    int operGroup;
    bool isNot;
    int typeGroup;

    QMap<qint64, JKKSSearchCriterion> searchCriteria;
};

#endif
