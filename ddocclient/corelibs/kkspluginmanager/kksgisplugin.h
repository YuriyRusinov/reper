#ifndef KKSGISPLUGIN_H
#define KKSGISPLUGIN_H

#include <QPixmap>
#include <QString>
#include <QAction>

#include <kkspluginmanager_config.h>

/*!\ingroup PluginGroup
\brief Базовый абстрактный класс (интерфейс) для всех плагинов, реализующих специальные алгоритмы обработки ГИС-данных

Все такие плагины должны быть порождены напрямую или ненапрямую от данного класса.*/
class _KKSPLUGIN_EXPORT KKSGISPlugin
{
    public:

        KKSGISPlugin() {}
        virtual ~KKSGISPlugin(){}


        virtual void setLogWidget(QTextEdit * logWidget) {Q_UNUSED(logWidget);}
        virtual QTextEdit * logWidget() {return NULL;}


};

Q_DECLARE_INTERFACE(KKSGISPlugin, "org.dynamicsoft.dynamicdocs.KKSGISPlugin/1.0");

#endif
