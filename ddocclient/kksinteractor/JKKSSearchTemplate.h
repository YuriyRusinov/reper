/***********************************************************************
 * Module: JKKSSearchTemplate.h
 * Author: yuriyrusinov
 * Modified: 09 февраля 2009 г. 15:02:00
 * Purpose: Declaration of the class JKKSSearchTemplate
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSSearchTemplate_h)
#define __KKSSITOOOM_JKKSSearchTemplate_h

#include "kksinteractor_config.h"

#include <QMap>

#include "JKKSSearchGroup.h"

class QDataStream;
/*
 * Это шаблон поиска
 */

class _I_EXPORT JKKSSearchTemplate
{
public:
    JKKSSearchTemplate (qint64 id=-1, QString name=QString(), QString desc=QString());
    JKKSSearchTemplate (const JKKSSearchTemplate& ST);
    ~JKKSSearchTemplate (void);

    qint64 id (void) const;
    void setId (qint64 id);

    QString getName (void) const;
    void setName (QString name);

    QString getDesc (void) const;
    void setDesc (QString desc);

    const QMap<qint64, JKKSSearchGroup>& getGroups (void) const;
    QMap<qint64, JKKSSearchGroup>& getGroups (void);
    void setGroups (QMap<qint64, JKKSSearchGroup>& grs);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSSearchTemplate& st);
    friend QDataStream& operator>> (QDataStream& in, JKKSSearchTemplate& st);

private:
    //
    // Variables
    //
    qint64 idSearchTemplate;
    QString searchTemplateName;
    QString searchTemplateDesc;

    QMap<qint64, JKKSSearchGroup> groups;
};

#endif
