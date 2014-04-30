#ifndef KKSIMPORTEXPORTPLUGIN_H
#define KKSIMPORTEXPORTPLUGIN_H

#include <QPixmap>
#include <QString>
#include <QAction>

#include <kkspluginmanager_config.h>

class KKSObject;

/*!\ingroup PluginGroup
\brief Базовый абстрактный класс (интерфейс) для всех плагинов импорта и экспорта данных, создаваемых для библиотеки DynamicDocs.

Все плагины импорта и экспорта данных должны быть порождены напрямую или ненапрямую от данного класса.*/
class _KKSPLUGIN_EXPORT KKSImportExportPlugin
{
    public:

        enum KKSFormatType
        {
            ftImport = 0,
            ftExport = 1
        };

        KKSImportExportPlugin() {}
        virtual ~KKSImportExportPlugin(){}

        virtual const QString & getFormat() = 0;
        virtual void setFile(const QString & url) = 0;
        virtual void setIO(const KKSObject * io) = 0;
        virtual KKSFormatType getFormatType() = 0;

        virtual void setLogWidget(QTextEdit * logWidget) {Q_UNUSED(logWidget);}
        virtual QTextEdit * logWidget() {return NULL;}

};

Q_DECLARE_INTERFACE(KKSImportExportPlugin, "org.dynamicsoft.dynamicdocs.KKSImportExportPlugin/1.0");

#endif
