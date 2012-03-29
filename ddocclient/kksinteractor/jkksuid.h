/***********************************************************************
 * Module: jkksuid.h
 * Author: ksa
 * Modified: 27 августа 2009 г. 17:46:40
 * Purpose: Declaration of the class JKKSUID
 ***********************************************************************/

#if !defined(__KKSSITOOOM_jkksuid_h)
#define __KKSSITOOOM_jkksuid_h

#include <QString>
#include "kksinteractor_config.h"
/*
 * Это глобальный уникальный идентификатор
 */

class _I_EXPORT JKKSUID
{
    public:
        JKKSUID(const QString & u) {m_uid = u;}
        JKKSUID (const JKKSUID& U) : m_uid (U.m_uid) {};
        JKKSUID(){}
        ~JKKSUID(){}

        const QString & uid() const {return m_uid;}
        void setUid(const QString & u) { m_uid = u; }

    protected:
        QString m_uid;
};

#endif
