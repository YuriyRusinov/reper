#ifndef KKSACCLEVEL_H
#define KKSACCLEVEL_H


#include <QString>

#include <KKSData.h>
#include "config_core.h"

class __CORE_EXPORT KKSAccLevel : public KKSData
{
public:

    KKSAccLevel()
    {
        m_name = QObject::tr("Not Secret");
        m_port = "5432";
    }

    KKSAccLevel(const QString & name, const QString & port)
    {
        m_name = name;
        m_port = port;
    }

    virtual ~KKSAccLevel(){}
       
    void setPort(const QString & port) 
    {
        m_port = port;
    }

    void setName(const QString & name)
    {
        m_name = name;
    }
    
    const QString & name() const
    {
        return m_name;
    }

    const QString & port() const
    {
        return m_port;
    }

private:
    
    QString m_port;
    QString m_name;
};

#endif
