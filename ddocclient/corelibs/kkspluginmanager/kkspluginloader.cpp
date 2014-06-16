#include <QList>
#include <QAction>
#include <QDir>
#include <QPluginLoader>
#include <QtDebug>

#include "kkspluginloader.h"

#include "kksbaseplugin.h"
#include "kksioplugin.h"
#include "kksattrwidgetplugin.h"
#include "kksgisplugin.h"
#include "kksimportexportplugin.h"

/*!\brief Деструктор класса.

Производит освобождение памяти, выделенной под список элементов управления плагинами.*/
KKSPluginLoader::~KKSPluginLoader( )
{
    if (m_pluginActions){
        while (!m_pluginActions->isEmpty()) 
            delete m_pluginActions->takeFirst();
        
        delete m_pluginActions;
    }
    
    if(m_baseActions) 
        delete m_baseActions;
    if(m_impExpActions) 
        delete m_impExpActions;
    if(m_ioActions) 
        delete m_ioActions;
    if(m_attrWidgetActions) 
        delete m_attrWidgetActions;
    if(m_gisActions) 
        delete m_gisActions;
      
    if (m_plugins){
        while (!m_plugins->isEmpty()) 
            delete m_plugins->takeFirst();
        
        delete m_plugins;
    }
    
    if(m_basePlugins) 
        delete m_basePlugins;
    if(m_impExpPlugins) 
        delete m_impExpPlugins;
    if(m_ioPlugins) 
        delete m_ioPlugins;
    if(m_attrWidgetPlugins) 
        delete m_attrWidgetPlugins;
    if(m_gisPlugins) 
        delete m_gisPlugins;      
}

/*!\brief Конструктор класса.

Создает и инициализирует загрузчик плагинов. После создания можно воспользоваться методом getPluginActions(). Каталог, в котором будет осуществлятся поиск плагинов, задается так: <LIBDIR>/libxgis. <LIBDIR> - переменная, задаваемая при сборке библиотеки классов XGIS, как параметр скрипта configure (./configure --libdir=DIR). По умолчанию используется значение /usr/local/lib.

Нельзя создавать экземпляр данного класса явно. Это происходит в классе GISdb (см. метод GISdb::getPluginLoader()).*/
/*
KKSPluginLoader::KKSPluginLoader( )
{
    m_pluginActions = NULL;
    m_baseActions = NULL;
    m_impExpActions = NULL;
    m_ioActions = NULL;
    m_gisActions = NULL;
    m_attrWidgetActions = NULL;
      
    m_plugins = NULL;
    m_basePlugins = NULL;
    m_impExpPlugins = NULL;
    m_ioPlugins = NULL;
    m_attrWidgetPlugins = NULL;
    m_gisPlugins = NULL;
  
}
*/

/*!\brief Конструктор класса.

Создает и инициализирует загрузчик плагинов. После создания можно воспользоваться методом getPluginActions(). Каталог, в котором будет осуществлятся поиск плагинов, задается параметром dir.

Не требуется создавать экземпляр данного класса явно. Это происходит в классе GISdb (см. метод GISdb::getPluginLoader()).*/
KKSPluginLoader::KKSPluginLoader( QDir & dir )
{
    m_pluginActions = NULL;
    m_baseActions = NULL;
    m_impExpActions = NULL;
    m_ioActions = NULL;
    m_gisActions = NULL;
    m_attrWidgetActions = NULL;
      
    m_plugins = NULL;
    m_basePlugins = NULL;
    m_impExpPlugins = NULL;
    m_ioPlugins = NULL;
    m_attrWidgetPlugins = NULL;
    m_gisPlugins = NULL;

    dirs.clear ();
    dirs.append (QDir (dir));
}

int KKSPluginLoader::initPlugins()
{
    for (int ip=0; ip<dirs.count(); ip++)
    {
        QDir d(dirs[ip]);
        d.setNameFilters(QStringList()<<"*.so"<<"*.dll");
        QStringList plugins = d.entryList();
        int count = plugins.count();
        if (count == 0)
            continue;

        int res = 0;
        for(int i=0; i<count; i++)
        {
            res += initPlugin(d.absoluteFilePath(plugins.at(i)));
        }

        if (res != count)
            continue;
    }

    return 1;
}

int KKSPluginLoader::initPlugin( QString path )
{
    QPluginLoader loader(path);
    qDebug() << path;
    QObject *plugin = loader.instance();
    if (plugin){
        
        KKSBasePlugin *iBase = qobject_cast<KKSBasePlugin *>(plugin);
        QAction * action = NULL;
        if (iBase){
            action = new QAction(iBase->getPixmap(), iBase->getName(), NULL);
            action->setToolTip(iBase->getDesc());
            action->setStatusTip(iBase->getDesc());
            action->setWhatsThis(iBase->getDesc());
            action->setObjectName(path);
            action->setData(ptKKSBasePlugin);
            iBase->setAction(action);
            
            KKSIOPlugin *iIOPlugin = qobject_cast<KKSIOPlugin *>(plugin);
            if(iIOPlugin){
                action->setData(ptKKSIOPlugin);
            }

            KKSAttrWidgetPlugin *iAWPlugin = qobject_cast<KKSAttrWidgetPlugin *>(plugin);
            if(iAWPlugin){
                action->setData(ptAttrWidgetPlugin);
            }

            KKSImportExportPlugin *iIEPlugin = qobject_cast<KKSImportExportPlugin *>(plugin);
            if(iIEPlugin){
                action->setData(ptImportExportPlugin);
            }

            KKSGISPlugin *iGISPlugin = qobject_cast<KKSGISPlugin *>(plugin);
            if(iGISPlugin){
                action->setData(ptKKSGISPlugin);
            }

            if(!m_pluginActions) 
                m_pluginActions = new QList<QAction*>();

            m_pluginActions->append(action);
            
            if(!m_plugins) 
                m_plugins = new QList<QObject*>();

            m_plugins->append(plugin);
        }

        return 1;
    }

    return 0;
}

/*!\brief Метод возвращает список элементов управления всеми загруженными плагинами.

Если список пуст, то плагины сначала загружаются и инициализируются. Поиск плагинов для загрузки осуществляется в каталоге, указанном в качестве параметра в конструкторе класса. 
Если использовался конструктор без параметра, то поиск осуществляется в каталоге <LIBDIR>/ddocplugins. Для каждого загруженного плагина создается элемент управления (new QAction()). 

Элементы управления создаются следующим образом:
\code
KKSBasePlugin *iBase = qobject_cast<KKSBasePlugin *>(plugin);
QAction * action = NULL;
if (iBase){
  action = new QAction(iBase->getPixmap(), iBase->getName(), NULL);
  action->setStatusTip(iBase->getDesc());
  action->setWhatsThis(iBase->getDesc());
  action->setObjectName(path);//path - путь к файлу, где лежит плагин

  if(!m_pluginActions) 
    m_pluginActions = new QList<QAction*>();

  m_pluginActions->append(action); //m_pluginActions - внутренний список элементов управления
}
\endcode*/
QList< QAction * > * KKSPluginLoader::getPluginActions( )
{
    if(!m_pluginActions)
        initPlugins();

    return m_pluginActions;
}

/*!\brief Метод возвращает список элементов управления загруженными плагинами заданного типа.

Если список пуст, то плагины сначала загружаются и инициализируются (загружаются и инициализируются все плагины, вне зависимости от их типа). 
Поиск плагинов для загрузки осуществляется в каталоге, указанном в качестве параметра в конструкторе класса. Если использовался конструктор без параметра, 
то поиск осуществляется в каталоге <LIBDIR>/ddocclient. Для каждого загруженного плагина создается элемент управления (new QAction()). 

Элементы управления создаются следующим образом:
\code
KKSBasePlugin *iBase = qobject_cast<KKSBasePlugin *>(plugin);
QAction * action = NULL;
if (iBase){
  action = new QAction(iBase->getPixmap(), iBase->getName(), NULL);
  action->setStatusTip(iBase->getDesc());
  action->setWhatsThis(iBase->getDesc());
  action->setObjectName(path);//path - путь к файлу, где лежит плагин
  if(!m_pluginActions) 
    m_pluginActions = new QList<QAction*>();
  m_pluginActions->append(action); //m_pluginActions - внутренний список элементов управления
}
\endcode*/
QList< QAction * > * KKSPluginLoader::getPluginActions( KKSPluginType type)
{
    switch (type) {
        case ptNone: return getPluginActions(); break;
        case ptKKSBasePlugin: return getBPActions(); break;
        case ptKKSIOPlugin: return getIOPActions(); break;
        case ptAttrWidgetPlugin: return getAWPActions(); break;
        case ptImportExportPlugin: return getIEPActions(); break;
        case ptKKSGISPlugin: return getGISPActions(); break;
        default: return NULL;
    }

    return NULL;    
}


/*!\brief Метод возвращает список элементов управления загруженными плагинами, порожденными только лишь от абстрактного класса KKSBasePlugin.

Если список пуст, то плагины сначала загружаются и инициализируются (путем вызова метода getPluginActions()). */
QList< QAction * > * KKSPluginLoader::getBPActions( )
{
    QList<QAction*> * allActions = getPluginActions();
    if(!allActions) 
        return NULL;
    
    if(!m_baseActions){
        m_baseActions = new QList<QAction*>();

        QListIterator<QAction*> iterator(*allActions);
        while (iterator.hasNext()) {
           if(iterator.peekNext()->data().toInt() == ptKKSBasePlugin) 
               m_baseActions->append(iterator.next());
           else 
               iterator.next();
        }
    }

    if(m_baseActions->count() == 0) 
        return NULL;
    
    return m_baseActions;
}

/*!\brief Метод возвращает список элементов управления загруженными плагинами, порожденными от абстрактного класса KKSIOPlugin.

Если список пуст, то плагины сначала загружаются и инициализируются (путем вызова метода getPluginActions()). */
QList< QAction * > * KKSPluginLoader::getIOPActions( )
{
    QList<QAction*> * allActions = getPluginActions();
    if(!allActions) 
        return NULL;
    
    if(!m_ioActions){
        m_ioActions = new QList<QAction*>();

        QListIterator<QAction*> iterator(*allActions);
        while (iterator.hasNext()) {
            if(iterator.peekNext()->data().toInt() == ptKKSIOPlugin) 
                m_ioActions->append(iterator.next());
            else 
                iterator.next();
        }
    }
    if(m_ioActions->count() == 0) 
        return NULL;
    
    return m_ioActions;
}

/*!\brief Метод возвращает список элементов управления загруженными плагинами, порожденными от абстрактного класса KKSImportExportPlugin.

Если список пуст, то плагины сначала загружаются и инициализируются (путем вызова метода getPluginActions()). */
QList< QAction * > * KKSPluginLoader::getIEPActions( )
{
    QList<QAction*> * allActions = getPluginActions();
    if(!allActions) 
        return NULL;
    
    if(!m_impExpActions){
        m_impExpActions = new QList<QAction*>();

        QListIterator<QAction*> iterator(*allActions);
        while (iterator.hasNext()) {
            if(iterator.peekNext()->data().toInt() == ptImportExportPlugin) 
                m_impExpActions->append(iterator.next());
            else 
                iterator.next();
        }
    }

    if(m_impExpActions->count() == 0) 
        return NULL;
    
    return m_impExpActions;
}

/*!\brief Метод возвращает список элементов управления загруженными плагинами, порожденными от абстрактного класса KKSAttrWidgetPlugin.

Если список пуст, то плагины сначала загружаются и инициализируются (путем вызова метода getPluginActions()).*/
QList< QAction * > * KKSPluginLoader::getAWPActions( )
{
    QList<QAction*> * allActions = getPluginActions();
    if(!allActions) 
        return NULL;

    if(!m_attrWidgetActions){
        m_attrWidgetActions = new QList<QAction*>();

        QListIterator<QAction*> iterator(*allActions);
        while (iterator.hasNext()) {
            if(iterator.peekNext()->data().toInt() == ptAttrWidgetPlugin) 
                    m_attrWidgetActions->append(iterator.next());
            else 
                iterator.next();
        }
    }

    if(m_attrWidgetActions->count() == 0) 
        return NULL;

    return m_attrWidgetActions;
}

/*!\brief Метод возвращает список элементов управления загруженными плагинами, порожденными от абстрактного класса KKSGISPlugin.

Если список пуст, то плагины сначала загружаются и инициализируются (путем вызова метода getPluginActions()).*/
QList< QAction * > * KKSPluginLoader::getGISPActions( )
{
    QList<QAction*> * allActions = getPluginActions();
    if(!allActions) 
        return NULL;

    if(!m_gisActions){
        m_gisActions = new QList<QAction*>();

        QListIterator<QAction*> iterator(*allActions);
        while (iterator.hasNext()) {
            if(iterator.peekNext()->data().toInt() == ptKKSGISPlugin) 
                m_gisActions->append(iterator.next());
            else 
                iterator.next();
        }
    }

    if(m_gisActions->count() == 0) 
        return NULL;

    return m_gisActions;
}

/*!\brief Метод возвращает список загруженных плагинов всех типов.

Если список пуст, то плагины сначала загружаются и инициализируются (загружаются и инициализируются все плагины, вне зависимости от их типа). 
Поиск плагинов для загрузки осуществляется в каталоге указанном в качестве параметра в конструкторе класса. Если использовался конструктор без параметра, 
то поиск осуществляется в каталоге <LIBDIR>/ddocclient. Для каждого загруженного плагина создается элемент управления (new QAction()). */
QList< QObject * > * KKSPluginLoader::getPlugins( )
{
    if(!m_plugins)
        initPlugins();

    return m_plugins;
}

/*!\brief Метод возвращает список загруженных плагинов заданного типа.

Если список пуст, то плагины сначала загружаются и инициализируются (загружаются и инициализируются все плагины, вне зависимости от их типа). 
Поиск плагинов для загрузки осуществляется в каталоге указанном в качестве параметра в конструкторе класса. Если использовался конструктор без параметра, 
то поиск осуществляется в каталоге <LIBDIR>/ddocclient. Для каждого загруженного плагина создается элемент управления (new QAction()). */
QList< QObject * > * KKSPluginLoader::getPlugins( KKSPluginType type)
{
    switch (type) {
        case ptNone: return getPlugins(); break;
        case ptKKSBasePlugin: return getBPlugins(); break;
        case ptKKSIOPlugin: return getIOPlugins(); break;
        case ptAttrWidgetPlugin: return getAWPlugins(); break;
        case ptImportExportPlugin: return getIEPlugins(); break;
        case ptKKSGISPlugin: return getGISPlugins(); break;
        default: return NULL;
    }

    return NULL;
}

/*!\brief Метод возвращает список загруженных плагинов, порожденных только лишь от абстрактного класса KKSBasePlugin.

Если список пуст, то плагины сначала загружаются и инициализируются (путем вызова метода getPluginActions()).*/
QList< QObject * > * KKSPluginLoader::getBPlugins( )
{
    QList<QObject*> * allPlugins = getPlugins();
    if(!allPlugins) 
        return NULL;

    if(!m_basePlugins){
        m_basePlugins = new QList<QObject*>();
    
        QListIterator<QObject*> iterator(*allPlugins);
        while (iterator.hasNext()) {
            QObject * object = iterator.peekNext();
            KKSBasePlugin *iBase = qobject_cast<KKSBasePlugin *>(object);
            if(iBase->getAction()->data().toInt() == ptKKSBasePlugin) 
                m_basePlugins->append(iterator.next());
            else 
                iterator.next();
        }
    }

    if(m_basePlugins->count() == 0) 
        return NULL;

    return m_basePlugins;
}

/*!\brief Метод возвращает список загруженных плагинов, порожденных от абстрактного класса KKSIOPlugin.

Если список пуст, то плагины сначала загружаются и инициализируются (путем вызова метода getPluginActions()).*/
QList< QObject * > * KKSPluginLoader::getIOPlugins( )
{
    QList<QObject*> * allPlugins = getPlugins();
    if(!allPlugins) 
        return NULL;
    
    if(!m_ioPlugins){
        m_ioPlugins = new QList<QObject*>();
    
        QListIterator<QObject*> iterator(*allPlugins);
        while (iterator.hasNext()) {
            QObject * object = iterator.peekNext();
            KKSBasePlugin *iIO = qobject_cast<KKSBasePlugin *>(object);
            if(iIO->getAction()->data().toInt() == ptKKSIOPlugin) 
                m_ioPlugins->append(iterator.next());
            else 
                iterator.next();
        }
    }

    if(m_ioPlugins->count() == 0) 
        return NULL;
    
    return m_ioPlugins;
}

/*!\brief Метод возвращает список загруженных плагинов, порожденных от абстрактного класса KKSAttrWidgetPlugin.

Если список пуст, то плагины сначала загружаются и инициализируются (путем вызова метода getPluginActions()).*/
QList< QObject * > * KKSPluginLoader::getAWPlugins( )
{
    QList<QObject*> * allPlugins = getPlugins();
    if(!allPlugins) 
        return NULL;

    if(!m_attrWidgetPlugins){
        m_attrWidgetPlugins = new QList<QObject*>();
    
        QListIterator<QObject*> iterator(*allPlugins);
        while (iterator.hasNext()) {
            QObject * object = iterator.peekNext();
            KKSBasePlugin *iAW = qobject_cast<KKSBasePlugin *>(object);
            if(iAW->getAction()->data().toInt() == ptAttrWidgetPlugin) 
                m_attrWidgetPlugins->append(iterator.next());
            else 
                iterator.next();
        }
    }

    if(m_attrWidgetPlugins->count() == 0) 
        return NULL;
    
    return m_attrWidgetPlugins;
}

/*!\brief Метод возвращает список загруженных плагинов, порожденных от абстрактного класса KKSImportExportPlugin.

Если список пуст, то плагины сначала загружаются и инициализируются (путем вызова метода getPluginActions()).*/
QList< QObject * > * KKSPluginLoader::getIEPlugins( )
{
    QList<QObject*> * allPlugins = getPlugins();
    if(!allPlugins) 
        return NULL;

    if(!m_impExpPlugins){
        m_impExpPlugins = new QList<QObject*>();
    
        QListIterator<QObject*> iterator(*allPlugins);
        while (iterator.hasNext()) {
            QObject * object = iterator.peekNext();
            KKSBasePlugin *iIE = qobject_cast<KKSBasePlugin *>(object);
            if(iIE->getAction()->data().toInt() == ptImportExportPlugin) 
                m_impExpPlugins->append(iterator.next());
            else 
                iterator.next();
        }
    }

    if(m_impExpPlugins->count() == 0) 
        return NULL;

    return m_impExpPlugins;
}

/*!\brief Метод возвращает список загруженных плагинов, порожденных от абстрактного класса KKSGISPlugin.

Если список пуст, то плагины сначала загружаются и инициализируются (путем вызова метода getPluginActions()).*/
QList< QObject * > * KKSPluginLoader::getGISPlugins( )
{
    QList<QObject*> * allPlugins = getPlugins();
    if(!allPlugins) 
        return NULL;

    if(!m_gisPlugins){
        m_gisPlugins = new QList<QObject*>();
    
        QListIterator<QObject*> iterator(*allPlugins);
        while (iterator.hasNext()) {
            QObject * object = iterator.peekNext();
            KKSBasePlugin *iGis = qobject_cast<KKSBasePlugin *>(object);
            if(iGis->getAction()->data().toInt() == ptKKSGISPlugin) 
                m_gisPlugins->append(iterator.next());
            else 
                iterator.next();
        }
    }

    if(m_gisPlugins->count() == 0) 
        return NULL;

    return m_gisPlugins;
}

QAction * KKSPluginLoader::getPluginAction(KKSPluginType type, const QString & name) 
{
    if(name.isEmpty())
        return NULL;

    QList<QObject*> * allPlugins = getPlugins(type);
    if(!allPlugins) 
        return NULL;

    QListIterator<QObject*> iterator(*allPlugins);
    while (iterator.hasNext()) {
        QObject * object = iterator.peekNext();
        KKSBasePlugin *iNextPlugin = qobject_cast<KKSBasePlugin *>(object);
        
        if(iNextPlugin->getName() == name){
            return iNextPlugin->getAction();
            break;
        }
        else 
            iterator.next();
    }

    return NULL;
}

QAction * KKSPluginLoader::getPluginAction(const QString & name) 
{
    if(name.isEmpty())
        return NULL;

    QList<QObject*> * allPlugins = getPlugins();
    if(!allPlugins) 
        return NULL;

    QListIterator<QObject*> iterator(*allPlugins);
    while (iterator.hasNext()) {
        QObject * object = iterator.peekNext();
        KKSBasePlugin *iNextPlugin = qobject_cast<KKSBasePlugin *>(object);
        
        if(iNextPlugin->getName() == name){
            return iNextPlugin->getAction();
            break;
        }
        else 
            iterator.next();
    }

    return NULL;
}

QObject * KKSPluginLoader::getPlugin(KKSPluginType type, const QString & name) 
{
    if(name.isEmpty())
        return NULL;

    QList<QObject*> * allPlugins = getPlugins(type);
    if(!allPlugins) 
        return NULL;

    QListIterator<QObject*> iterator(*allPlugins);
    while (iterator.hasNext()) {
        QObject * object = iterator.peekNext();
        KKSBasePlugin *iNextPlugin = qobject_cast<KKSBasePlugin *>(object);
        
        if(iNextPlugin->getName() == name){
            return object;
            break;
        }
        else 
            iterator.next();
    }

    return NULL;
}

QObject * KKSPluginLoader::getPlugin(const QString & name) 
{
    if(name.isEmpty())
        return NULL;

    QList<QObject*> * allPlugins = getPlugins();
    if(!allPlugins) 
        return NULL;

    QListIterator<QObject*> iterator(*allPlugins);
    while (iterator.hasNext()) {
        QObject * object = iterator.peekNext();
        KKSBasePlugin *iNextPlugin = qobject_cast<KKSBasePlugin *>(object);
        
        if(iNextPlugin->getName() == name){
            return object;
            break;
        }
        else 
            iterator.next();
    }

    return NULL;
}
