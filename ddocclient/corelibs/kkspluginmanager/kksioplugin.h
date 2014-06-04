#ifndef KKSIOPLUGIN_H
#define KKSIOPLUGIN_H

#include <QPixmap>
#include <QString>
#include <QAction>

#include <kkspluginmanager_config.h>

class KKSObject;

/*!\ingroup PluginGroup
\brief Базовый абстрактный класс (интерфейс) для всех плагинов, обрабатываемых информационные объекты.

Все такие плагины должны быть порождены напрямую или ненапрямую от данного класса.*/
class _KKSPLUGIN_EXPORT KKSIOPlugin
{
    public:

        enum KKSIOPluginType
        {
            iotAttrValues = 0,
            iotRubrif = 1,
            iotFiles = 2,
            iotIO = 3
        };

        KKSIOPlugin() {}
        virtual ~KKSIOPlugin(){}

        virtual void setIO(KKSObject * io) = 0;
        virtual KKSIOPluginType getIOPluginType() = 0;

        virtual void setLogWidget(QTextEdit * logWidget) {Q_UNUSED(logWidget);}
        virtual QTextEdit * logWidget() {return NULL;}

    signals:

       void ioChanged(KKSObject * io);

};

Q_DECLARE_INTERFACE(KKSIOPlugin, "org.dynamicsoft.dynamicdocs.KKSIOPlugin/1.0");

#endif
