#ifndef KKSATTRWIDGETPLUGIN_H
#define KKSATTRWIDGETPLUGIN_H

#include <QPixmap>
#include <QString>
#include <QAction>

#include <kkspluginmanager_config.h>

class KKSAttrWidget;

/*!\ingroup PluginGroup
\brief Базовый абстрактный класс (интерфейс) для всех плагинов, реализующих специфические виджеты для визуализации атрибутов различных (в т.ч. пользовательских типов).

Все такие плагины должны быть порождены напрямую или ненапрямую от данного класса.*/
class _KKSPLUGIN_EXPORT KKSAttrWidgetPlugin
{
    public:

        KKSAttrWidgetPlugin() {}
        virtual ~KKSAttrWidgetPlugin(){}

        virtual void setParentWidget(const QWidget * parent) = 0;
        virtual QWidget * getWidget() = 0;
        virtual void setAttrWidget(KKSAttrWidget * aw) = 0;

        virtual void setLogWidget(QTextEdit * logWidget) {Q_UNUSED(logWidget);}
        virtual QTextEdit * logWidget() {return NULL;}

    signals:

       void valueChanged(KKSAttrWidget * aw);

};

Q_DECLARE_INTERFACE(KKSAttrWidgetPlugin, "org.dynamicsoft.dynamicdocs.KKSAttrWidgetPlugin/1.0");

#endif
