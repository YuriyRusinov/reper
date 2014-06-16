#ifndef KKSIOPLUGIN_H
#define KKSIOPLUGIN_H

#include <QPixmap>
#include <QString>
#include <QAction>

#include <kkspluginmanager_config.h>

class KKSObject;
class KKSObjectExemplar;

/*!\ingroup PluginGroup
\brief Базовый абстрактный класс (интерфейс) для всех плагинов, обрабатываемых информационные объекты и записи справочников.

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
        virtual KKSIOPluginType getIOPluginType() = 0;

        virtual void setLogWidget(QTextEdit * logWidget) {Q_UNUSED(logWidget);}
        virtual QTextEdit * logWidget() {return NULL;}

    public slots:
        virtual void setIO(KKSObject * io) = 0;
        virtual void setEIO(KKSObjectExemplar * io) = 0;

    signals:

        virtual void getCurrentIO(KKSObject ** io) = 0;
        virtual void getCurrentEIO(KKSObjectExemplar ** eio) = 0;
        virtual void ioChanged(KKSObject * io) = 0;
        virtual void eioChanged(KKSObjectExemplar * io) = 0;

};

Q_DECLARE_INTERFACE(KKSIOPlugin, "org.dynamicsoft.dynamicdocs.KKSIOPlugin/1.0");

#endif
