#ifndef KKSGISPLUGIN_H
#define KKSGISPLUGIN_H

#include <QPixmap>
#include <QString>
#include <QAction>

#include <kkspluginmanager_config.h>

/*!\ingroup PluginGroup
\brief ������� ����������� ����� (���������) ��� ���� ��������, ����������� ����������� ��������� ��������� ���-������

��� ����� ������� ������ ���� ��������� �������� ��� ���������� �� ������� ������.*/
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
