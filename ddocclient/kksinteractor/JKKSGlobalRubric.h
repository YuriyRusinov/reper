/***********************************************************************
 * Module: JKKSGlobalRubric.h
 * Author: yuriyrusinov
 * Modified: 27 августа 2009 г. 17:46:40
 * Purpose: Declaration of the class JKKSGlobalRubric
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSGlobalRubric_h)
#define __KKSSITOOOM_JKKSGlobalRubric_h

#include "kksinteractor_config.h"
/*
 * Ёто рубрикатор дл€ глобальных рубрик
 */

#include "JKKSRubric.h"
#include "JKKSCategory.h"
#include "JKKSSearchTemplate.h"

class QDataStream;

class _I_EXPORT JKKSGlobalRubric : public JKKSRubric
{
public:
    JKKSGlobalRubric (qint64 id=-1, 
                      const QString & uid = QString(),
                      qint64 idp=-1, 
                      qint64 idc=-1, 
                      const QString& name=QString(), 
                      const QString& code=QString(), 
                      const QString& desc=QString());
    JKKSGlobalRubric (const JKKSGlobalRubric& R);
    ~JKKSGlobalRubric (void);
    JKKSGlobalRubric& operator= (const JKKSGlobalRubric& R);

    const QMap<qint64, JKKSCategory>& getCategory (void) const;
    QMap<qint64, JKKSCategory>& getCategory (void);
    void setCategory (const QMap<qint64, JKKSCategory>& c);

    const JKKSSearchTemplate& getSearchTemplate (void) const;
    void setSearchTemplate (const JKKSSearchTemplate& st);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSGlobalRubric& R);
    friend QDataStream& operator>> (QDataStream& in, JKKSGlobalRubric& R);

private:
    QMap<qint64, JKKSCategory> category;
    JKKSSearchTemplate searchTemplate;
};


#endif
