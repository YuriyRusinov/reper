/***********************************************************************
 * Module: jkksuid.h
 * Author: ksa
 * Modified: 27 ������� 2009 �. 17:46:40
 * Purpose: Declaration of the class JKKSUID
 ***********************************************************************/

#if !defined(__KKSSITOOOM_jkksuid_h)
#define __KKSSITOOOM_jkksuid_h

#include <QString>
#include <QColor>
#include <QIcon>
#include "kksinteractor_config.h"
/*
 * ��� ���������� ���������� ������������� + ��� ��������� ����������, ����� ��� �� � ������� ������������
 */

class _I_EXPORT JKKSUID
{
    public:
        JKKSUID(const QString & u, 
                const QString & uuid,
                qint64 idState = 1,
                const QColor bkCol = QColor(),
                const QColor fgCol = QColor(),
                const QString & rIcon = QString(),
                const QString & rName = QString()) : 
            m_uid(u),
            m_uuid(uuid),
            m_idState(idState),
            m_fgColor(fgCol),
            m_bgColor(bkCol),
            m_rIcon(rIcon),
            m_rName(rName)
        {
            if(m_idState <= 0)
                m_idState = 1;
        }

        JKKSUID (const JKKSUID& U) : 
            m_uid (U.m_uid), 
            m_uuid(U.m_uuid),
            m_idState(U.m_idState),
            m_bgColor(U.m_bgColor),
            m_fgColor(U.m_fgColor),
            m_rIcon(U.m_rIcon),
            m_rName(U.m_rName)
        {
            
        };

        JKKSUID() : m_idState(1){}
        ~JKKSUID(){}

        const QString & uid() const {return m_uid;}
        void setUid(const QString & u) { m_uid = u; }

        const QString & uuid() const {return m_uuid;}
        void setUuid(const QString & uuid) {m_uuid = uuid;}

        qint64 idState() const {return m_idState;}
        void setIdState(qint64 id) {m_idState = id > 0 ? id : 1;}

        const QColor & bgColor() const {return m_bgColor;}
        void setBgColor(const QColor & c){ m_bgColor = c;}

        const QColor & fgColor() const {return m_fgColor;}
        void setFgColor(const QColor & c){ m_fgColor = c;}

        const QString & rIcon() const {return m_rIcon;}
        void setRIcon(const QString & i){ m_rIcon = i;}

        const QString & rName() const {return m_rName;}
        void setRName(const QString & i){ m_rName = i;}
protected:
        QString m_uid;
        QString m_uuid;
        qint64 m_idState;
        QColor m_bgColor;
        QColor m_fgColor;
        QString m_rIcon;
        QString m_rName;
};

#endif
