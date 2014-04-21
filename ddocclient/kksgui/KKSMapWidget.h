/***********************************************************************
 * Module:  KKSMapWidget.h
 * Author:  sergey karin
 * Modified: 07 ������� 2013 �. 14:46:21
 * Purpose: Declaration of the class KKSMapWidget
 * ������ ��� ������ � ������
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSMapWidget_h)
#define __KKSSITOOOM_KKSMapWidget_h

#include "kksgui_config.h"

#include <QObject>
#include <KKSAttrWidget.h>
#include <QString>
#include <QList>
#include <QListWidget>
#include <QVariant>
#include <QDockWidget>
#include <QDomDocument>


class KKSAttrValue;
class KKSFile;

#include <KKSValue.h>

#ifdef __USE_QGIS__
#include "KKSGISWidget.h"
//#include "kksgiswidgetqgis.h"
//class KKSGISWidget;
#endif

#ifdef __USE_QGIS__
class _GUI_EXPORT KKSMapWidget : public KKSGISWidget, public KKSAttrWidget
#else
class _GUI_EXPORT KKSMapWidget : public QWidget, public KKSAttrWidget
#endif
{
    Q_OBJECT
public:
    KKSMapWidget(QMap<QString, QString> connectionParams,
                 qint64 idObj, 
                 const QString & gisHomeDir, 
                 const KKSAttrValue* attr = 0, 
                 KKSIndAttrClass isSys = iacIOUserAttr, 
                 QWidget *parent=NULL);

    virtual ~KKSMapWidget(void);

    QDockWidget * legendWidget() const;
    QDockWidget * layerOrderWidget() const;
    void init();

signals:
    void valueChanged (qint64 id, KKSIndAttrClass sys, QVariant val);
    void aboutToDestroy(QDockWidget *, QDockWidget *);
    void downloadGISFiles(bool bForRec, const QString & homeDir, qint64 idObj, QWidget * parent);//������ �� �������� ����� �� ����� �� ��. ���� �������� ��������������� �� ������� io_urls
    void uploadGISFiles(bool bForRec, const QStringList & files, qint64 idObj, QWidget * parent);//�������� ������ ���-������� �� ������. ����� �������� ����������� ������
public slots:
    void slotMapChanged(QDomDocument&);
    void slotDataChanged();
    void slotUploadGISFiles(qint64 idObj);
    void slotSaveGISProject(KKSValue & v);
private:
    QDockWidget * m_legendWidget;
    QDockWidget * m_layerOrderWidget;
    QString m_GISHomeDir;
    bool m_dataChanged;
    qint64 m_idObj;

    int openProject();//��������� ������, �������� � �������� �������� ��������
    int downloadLayers(const QString & homeDir, const QString & xmlPrj);//�������� �� ������ ������ �� ������, ���� ���� ������������ ������� (�.�. �� PostGIS)
    bool removeQGISPrjDir(const QString & path);
    QStringList readGISProjectLayerFiles(const QString & xml) const;
    QString updateProjectXML(const QString & xml, 
                             bool bForWriteToDb) const; //������ �������� ���� true, �� XML ����������� ��� ������ � ��.
                                                        //� ���� ������ ������ URI postgis-����� ���, ����� ��� �� ���� user � password
                                                        //� ����� URI �������� �����, ����� ��� ���� ������ ������������� ������ �� �����, ������ �� ������� ������� (�.�. ./ )
                                                        //���� �������� = false, XML ����������� ��� �������� ����� ������� � QGIS
                                                        //� ���� ������ ������ URI postgis-����� ���, ����� datasource �������� ��������� ���������� � ��, � ������� �������� DynamicDocs Client
    QString readGISProjectFile();

    QMap<QString, QString> m_connectionParams;//������������ ��� ������� URI postgis-�����

private:
    void initQGIS();

};

#endif
