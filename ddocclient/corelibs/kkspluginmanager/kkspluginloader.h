#ifndef KKSPLUGINLOADER_H
#define KKSPLUGINLOADER_H

#include <QDir>

class QAction;

#include <kkspluginmanager_config.h>

/*!\ingroup PluginGroup
\brief �����, ���������� �� �������� ��������, ����������� �� ������������ ������ KKSBasePlugin (������� ����� ���� ����� ��������� � �� ������ ����������� �������, �� ������� ����� �����������).

����� ����� ��� �������� ������ getPluginActions() � getPlugins(), ������� ���������� ������ ��������� ���������� (QAction) ������ �� ����������� �������� � 
��������������� ���� ����������� ������� ��������������. ������� ������������ ������� ������������� ����� ���� ��������� QAction. 
����� ����� ���������: i-� ������ � ������ �������� ������������� i-�� �������� ���������� � ������ ��������� QAction.
 ���� ������ ��������� ���������� ����, �� ����� ������� ���������� �������� � ������������� ��������.

�������� �������� ���������� �� ��������, ����������� ���������� ���������� dir. ������ ���������� ���������������� � ������������� ���� ����� 
������ �������� (����������� � ����������) ��� �������� (����������� ��� ���������). � ��������� ������ ������������ �������� <LIBDIR>/ddocplugins. 
<LIBDIR> - ����������, ���������� ��� ������ ���������� ������� DynamicDocs, ��� �������� libdir ������� configure. �������� ���:
\code
./configure --libdir=/opt/ddocclient/lib 
\endcode

��� ��� �������� <PREFIX>/lib:
\code
./configure --prefix=/opt/ddocclient
\endcode

�� ��������� ������������ �������� /usr/local/lib.

����� ��������, ��� ��� �������� �������� ������������ ����� QPluginLoader. � ������� ����� ���� �������� ������ ���� ��������� (instance) ������� �������. 
��� ������� ����������� �� ������ � ������ ���������� ������ ����������.

�� ��������� ��������� ��������� ������� ������ ����. ��� ���������� � ������ KKSSito (��. ����� KKSSito::getPluginLoader()).

����� �������� � ������� ���������� (��������) ���������� DynamicDocs �������� \link Plugins ����� \endlink. */
class _KKSPLUGIN_EXPORT KKSPluginLoader{
    friend class KKSCoreApplication;
    private:    
        //KKSPluginLoader();
        KKSPluginLoader(QDir & dir);

    public:  
  
        /*!\brief ���� ��������*/
        enum KKSPluginType{
            ptNone = 0,//!<�� ���������. �������� ������������ � ������� getPluginActions() ��� �������� ����, ��� �� ��������� ����������� ������� �� �����.
            ptKKSBasePlugin = 1,//!<������ �������� ������ �� KKSBasePlugin
            ptKKSIOPlugin = 2,//!<������ �������� �� KKSIOPlugin
            ptAttrWidgetPlugin = 3,//!<������ �������� �� KKSAttrWidgetPlugin
            ptKKSGISPlugin = 4,//!<������ �������� �� KKSGISPlugin
            ptImportExportPlugin = 5//!<������ �������� �� KKSImportExportPlugin
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
