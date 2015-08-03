#ifndef KKSPLUGINLOADER_H
#define KKSPLUGINLOADER_H

#include <QDir>

class QAction;

#include <kkspluginmanager_config.h>

/*!\ingroup PluginGroup
\brief Класс, отвечающий за загрузку плагинов, порожденных от абстрактного класса KKSBasePlugin (плагины могут быть также порождены и от других абстрактных классов, но наличие этого обязательно).

Класс имеет два основных метода getPluginActions() и getPlugins(), которые возвращают список элементов управления (QAction) каждым из загруженных плагинов и 
непосредственно сами загруженные плагины соответственно. Каждому загруженному плагину соответствует ровно один экземпляр QAction. 
Также верно следующее: i-й плагин в списке плагинов соответствует i-му элементу управления в списке элементов QAction.
 Если список элементов управления пуст, то метод сначала производит загрузку и инициализацию плагинов.

Загрузка плагинов происходит из каталога, задаваемого внутренней переменной dir. Данная переменная инициализируется в конструкторах либо путем 
явного указания (конструктор с параметром) или неявного (конструктор без параметра). В последнем случае используется значение <LIBDIR>/ddocplugins. 
<LIBDIR> - переменная, задаваемая при сборке библиотеки классов DynamicDocs, как параметр libdir скрипта configure. Задается так:
\code
./configure --libdir=/opt/ddocclient/lib 
\endcode

Или как параметр <PREFIX>/lib:
\code
./configure --prefix=/opt/ddocclient
\endcode

По умолчанию используется значение /usr/local/lib.

Важно отметить, что для загрузки плагинов используется класс QPluginLoader. В системе может быть загружен только ОДИН экземпляр (instance) каждого плагина. 
Все плагины выгружаются из памяти в момент завершения работы приложения.

Не требуется создавать экземпляр данного класса явно. Это происходит в классе KKSSito (см. метод KKSSito::getPluginLoader()).

Более подробно о системе расширений (плагинов) библиотеки DynamicDocs написано \link Plugins здесь \endlink. */
class _KKSPLUGIN_EXPORT KKSPluginLoader{
    friend class KKSCoreApplication;
    private:    
        //KKSPluginLoader();
        KKSPluginLoader(QDir & dir);

    public:  
  
        /*!\brief Типы плагинов*/
        enum KKSPluginType{
            ptNone = 0,//!<Не определен. Значение используется в методах getPluginActions() для указания того, что не требуется фильтровать плагины по типам.
            ptKKSBasePlugin = 1,//!<Плагин порожден только от KKSBasePlugin
            ptKKSIOPlugin = 2,//!<Плагин порожден от KKSIOPlugin
            ptAttrWidgetPlugin = 3,//!<Плагин порожден от KKSAttrWidgetPlugin
            ptKKSGISPlugin = 4,//!<Плагин порожден от KKSGISPlugin
            ptImportExportPlugin = 5//!<Плагин порожден от KKSImportExportPlugin
        };
        
        ~KKSPluginLoader();
  
        QList<QAction*> * getPluginActions(KKSPluginType type);
        QList<QAction*> * getPluginActions();
          
        QList<QObject*> * getPlugins(KKSPluginType type);
        QList<QObject*> * getPlugins();

        QAction * getPluginAction(KKSPluginType type, const QString & name) ;
        QAction * getPluginAction(const QString & name) ;
  
        QObject * getPlugin(KKSPluginType type, const QString & name) ;
        QObject * getPlugin(const QString & name) ;
  
    private:
        int initPlugins();
        int initPlugin(QString path);
  
        QList<QAction*> * getBPActions();
        QList<QAction*> * getIOPActions();
        QList<QAction*> * getAWPActions();
        QList<QAction*> * getGISPActions();
        QList<QAction*> * getIEPActions();

        QList<QObject*> * getBPlugins();
        QList<QObject*> * getIOPlugins();
        QList<QObject*> * getAWPlugins();
        QList<QObject*> * getGISPlugins();
        QList<QObject*> * getIEPlugins();

        QList<QAction*> * m_pluginActions;
        QList<QAction*> * m_baseActions;
        QList<QAction*> * m_ioActions;
        QList<QAction*> * m_impExpActions;
        QList<QAction*> * m_attrWidgetActions;
        QList<QAction*> * m_gisActions;
  
        QList<QObject*> * m_plugins;
        QList<QObject*> * m_basePlugins;
        QList<QObject*> * m_impExpPlugins;
        QList<QObject*> * m_ioPlugins;
        QList<QObject*> * m_attrWidgetPlugins;
        QList<QObject*> * m_gisPlugins;
  
        QList<QDir> dirs;
};

#endif
