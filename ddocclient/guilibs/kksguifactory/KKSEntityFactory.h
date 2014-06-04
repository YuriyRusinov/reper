/***********************************************************************
 * Module:  KKSEntityFactory.h
 * Author:  yuriy
 * Modified: 3 марта 2009 г. 14:15:40
 * Purpose: Declaration of the class KKSEntityFactory
 * Comment: данный класс €вл€етс€ базовым классом дл€ фабрик редакторов »ќ,
 * категорий, шаблонов и рубрик
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSEntityFactory_h)
#define __KKSSITOOOM_KKSEntityFactory_h

#include "kksguifactory_config.h"

#include <QObject>

class KKSLoader;

class _GF_EXPORT KKSEntityFactory : public QObject
{
    public:
        KKSEntityFactory (QObject *parent=0);
        ~KKSEntityFactory (void);

    private:
        Q_OBJECT
};
#endif
